/**
 * @file cuesheet.cpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * Khopper is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Khopper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "cuesheetparser.hpp"
#include "rangedreader.hpp"
#include "freedb.hpp"

#include "khopper/error.hpp"
#include "khopper/track.hpp"

#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QtDebug>
#include <QtCore/QTextStream>
#include <QtCore/QEventLoop>

#include <algorithm>

namespace {

	inline QString stripQuote( const QString & s ) {
		if( s[0] == '\"' && s[s.length()-1] == '\"' ) {
			return s.mid( 1, s.length() - 2 );
		} else {
			return s;
		}
	}

}

using namespace khopper::album;
using khopper::error::BaseError;
using khopper::error::CodecError;
using khopper::error::ParsingError;
using khopper::error::RunTimeError;
using khopper::codec::ReaderSP;
using khopper::codec::RangedReader;
using khopper::plugin::getReaderCreator;

PlayList CueSheetParser::load( const QString & content, const QDir & dir, bool freedb ) {
	CueSheetParser parser;
	parser.parseCue_( content, dir, freedb );
	return parser.playList_;
}

CueSheetParser::CueSheetParser():
playList_(),
album_( new CueSheet ) {
}

void CueSheetParser::parseCue_( QString content, const QDir & dir, bool freedb ) {
	QRegExp COMMENT( "\\s*REM\\s+(.*)\\s+(.*)\\s*" );
	QRegExp SINGLE( "\\s*(CATALOG|CDTEXTFILE|ISRC|PERFORMER|SONGWRITER|TITLE)\\s+(.*)\\s*" );
	QRegExp FILES( "\\s*FILE\\s+(.*)\\s+(WAVE|BINARY)\\s*" );
	QRegExp FLAGS( "\\s*FLAGS\\s+(DATA|DCP|4CH|PRE|SCMS)\\s*" );
	QRegExp TRACK( "\\s*TRACK\\s+(\\d+)\\s+(AUDIO|CDG|MODE1/2048|MODE1/2352|MODE2/2336|MODE2/2352|CDI/2336|CDI/2352)\\s*" );
	QRegExp INDEX( "\\s*(INDEX|PREGAP|POSTGAP)\\s+((\\d+)\\s+)?(\\d+):(\\d+):(\\d+)\\s*" );

	this->trackIndex_ = 0;
	QTextStream sin( &content, QIODevice::ReadOnly );

	for( QString line = sin.readLine(); !line.isEmpty(); line = sin.readLine() ) {
		if( SINGLE.exactMatch( line ) ) {
			this->parseSingle_( SINGLE.cap( 1 ), SINGLE.cap( 2 ) );
		} else if( FILES.exactMatch( line ) ) {
			this->parseFile_( FILES.cap( 1 ), FILES.cap( 2 ), dir );
		} else if( FLAGS.exactMatch( line ) ) {
			this->parseFlags_( FLAGS.cap( 1 ) );
		} else if( INDEX.exactMatch( line ) ) {
			this->parseIndex_( INDEX.cap( 1 ), INDEX.cap( 3 ), INDEX.cap( 4 ), INDEX.cap( 5 ), INDEX.cap( 6 ) );
		} else if( COMMENT.exactMatch( line ) ) {
			this->parseComment_( COMMENT.cap( 1 ), COMMENT.cap( 2 ) );
		} else if( TRACK.exactMatch( line ) ) {
			this->parseTrack_( TRACK.cap( 1 ), TRACK.cap( 2 ) );
		} else {
			this->parseGarbage_( line );
		}
	}

	this->updateLastTrack_();

	if( freedb ) {
		try {
			this->queryFromCDDB_();
		} catch( BaseError & /*e*/ ) {
			// TODO: log a message
		}
	}
}

void CueSheetParser::parseSingle_( const QString & c, const QString & s ) {
	QString content = stripQuote( s );
	if( c == "CATALOG" ) {
		this->album_->setCatalog( content );
	} else if( c == "CDTEXTFILE" ) {
		this->album_->setCDTextFile( content );
	} else if( c == "ISRC" ) {
		if( this->currentTrack_ ) {
			this->currentTrack_->setISRC( content );
		}
	} else if( c == "PERFORMER" ) {
		if( this->trackIndex_ == 0 ) {
			this->album_->setArtist( content );
		} else {
			this->currentTrack_->setArtist( content );
		}
	} else if( c == "SONGWRITER" ) {
		if( this->trackIndex_ == 0 ) {
			this->album_->setSongWriter( content );
		} else {
			this->currentTrack_->setSongWriter( content );
		}
	} else if( c == "TITLE" ) {
		if( this->trackIndex_ == 0 ) {
			this->album_->setTitle( content );
		} else {
			this->currentTrack_->setTitle( content );
		}
	}
}

void CueSheetParser::parseFile_( const QString & fileName, const QString & type, const QDir & dir ) {
	if( this->trackIndex_ > 0 ) {
		this->updateLastTrack_();
	}

	this->currentFileURI_ = QUrl::fromLocalFile( dir.filePath( stripQuote( fileName ) ) );
	this->currentFileType_ = type;
}

void CueSheetParser::parseFlags_( const QString & flag ) {
	QSet< QString > flags( this->album_->getFlags() );
	flags.insert( flag );
	this->album_->setFlags( flags );
}

void CueSheetParser::parseIndex_( const QString & type, const QString & num, const QString & m, const QString & s, const QString & f ) {
	Timestamp tmp( m.toInt(), s.toInt(), f.toInt() * 1000 / 75 );

	if( type == "INDEX" ) {
		short int n = num.toShort();
		switch( n ) {
		case 0:
			// starting time of pregap
			if( this->trackIndex_ > 1 ) {
				this->previousTrack_->setDuration( tmp - this->previousTrack_->getStartTime() );
			}
			break;
		case 1:
			// add toc
			this->currentTOCs_.push_back( tmp );
			this->currentFrames_.push_back( QString( "\"%1\"" ).arg( m.toUInt() * 60 * 75 + s.toUInt() * 75 + f.toUInt() ) );

			// track start time
			this->currentTrack_->setStartTime( tmp );
			if( this->trackIndex_ > 1 && ( this->previousTrack_->getStartTime() + this->previousTrack_->getDuration() > tmp || !this->previousTrack_->getDuration().isValid() ) ) {
				this->previousTrack_->setDuration( tmp - this->previousTrack_->getStartTime() );
			}
			break;
		default:
			// TODO: other values are subindexes
			;
		}
	} else if( type == "PREGAP" ) {
		this->currentTrack_->setPregap( tmp );
	} else if( type == "POSTGAP" ) {
		this->currentTrack_->setPostgap( tmp );
	}
}

void CueSheetParser::parseComment_( const QString & key, const QString & value ) {
	if( this->trackIndex_ == 0 ) {
		QStringList comments( this->album_->getComments() );
		comments.append( QString( "%1:%2;" ).arg( key ).arg( value ) );
		this->album_->setComments( comments );
	} else {
		QStringList comments( this->currentTrack_->getComments() );
		comments.append( QString( "%1:%2;" ).arg( key ).arg( value ) );
		this->currentTrack_->setComments( comments );
	}
}

void CueSheetParser::parseTrack_( const QString & num, const QString & type ) {
	this->previousTrack_ = this->currentTrack_;
	this->currentTrack_.reset( new CueSheetTrack( this->currentFileURI_ ) );

	this->trackIndex_ = num.toUInt();
	this->currentTrack_->setIndex( num.toShort() );
	this->currentTrack_->setFileType( this->currentFileType_ );
	this->currentTrack_->setDataType( type );
	this->currentTrack_->setAlbum( this->album_ );

	this->playList_.push_back( this->currentTrack_ );
}

void CueSheetParser::parseGarbage_( const QString & line ) {
	if( this->trackIndex_ == 0 ) {
		QStringList garbage( this->album_->getGarbage() );
		garbage.append( line );
		this->album_->setGarbage( garbage );
	} else {
		QStringList garbage( this->currentTrack_->getGarbage() );
		garbage.append( line );
		this->currentTrack_->setGarbage( garbage );
	}
}

void CueSheetParser::updateLastTrack_() {
	// get the total length, because cue sheet don't provide it
	ReaderSP decoder( getReaderCreator( this->currentFileURI_ )( this->currentFileURI_ ) );
	try {
		// NOTE: may throw exception
		decoder->open( QIODevice::ReadOnly );
	} catch( CodecError & e ) {
		qDebug() << e.getMessage();
	}
	if( decoder->isOpen() ) {
		Timestamp tmp( Timestamp::fromMillisecond( decoder->getDuration() ) );
		this->currentTrack_->setDuration( tmp - this->currentTrack_->getStartTime() );

		this->currentTOCs_.push_back( tmp );

		decoder->close();
	}
}

void CueSheetParser::queryFromCDDB_() {
	FreeDB query;
	query.connectToHost( "freedb.freedb.org", 8880 );
	std::pair< QString, QString > t( query.query( this->calcDiscID_(), this->currentFrames_, ( this->currentTOCs_.back().getMinute() * 60 + this->currentTOCs_.back().getSecond() ) - ( this->currentTOCs_.front().getMinute() * 60 + this->currentTOCs_.front().getSecond() ) ) );
	DiscData data( query.read( t.first, t.second ) );
	if( !data.artist.isEmpty() ) {
		this->album_->setArtist( data.artist );
	}
	if( !data.title.isEmpty() ) {
		this->album_->setTitle( data.title );
	}
	for( QMap< int, TrackData >::const_iterator it = data.tracks.begin(); it != data.tracks.end(); ++it ) {
		if( !it->artist.isEmpty() ) {
			this->playList_[it.key()]->setArtist( it->artist );
		}
		if( !it->title.isEmpty() ) {
			this->playList_[it.key()]->setTitle( it->title );
		}
	}
}

unsigned int CueSheetParser::calcDiscID_() {
	int t = 0, n = 0;
	unsigned int i = 0;

	while( i < ( this->currentTOCs_.size() - 1 ) ) {
		// NOTE: CDs have a 2-second offset from the start of disc data
		n += this->calcCDDBSum_( this->currentTOCs_[i].getMinute() * 60 + this->currentTOCs_[i].getSecond() + 2 );
		++i;
	}
	t = ( this->currentTOCs_.back().getMinute() * 60 + this->currentTOCs_.back().getSecond() ) - ( this->currentTOCs_.front().getMinute() * 60 + this->currentTOCs_.front().getSecond() );

	unsigned int tmp = ( ( n % 0xFF ) << 24 | t << 8 | ( this->currentTOCs_.size() - 1 ) );
	return tmp;
}

int CueSheetParser::calcCDDBSum_( int n ) {
	int ret = 0;
	while( n > 0 ) {
		ret += n % 10;
		n /= 10;
	}
	return ret;
}
