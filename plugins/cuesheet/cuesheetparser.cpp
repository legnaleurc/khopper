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

#include "khopper/abstractreader.hpp"
#include "khopper/error.hpp"
#include "khopper/text.hpp"
#include "khopper/track.hpp"

#include <QtDebug>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

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

PlayList CueSheetParser::load( const QString & content, const QDir & dir ) {
	CueSheetParser parser( content, dir );
	return parser.playList_;
}

CueSheetParser::CueSheetParser( const QString & content, const QDir & dir ):
playList_(),
album_( new CueSheet ) {
	this->parseCue_( content, dir );
}

void CueSheetParser::parseCue_( QString content, const QDir & dir ) {
	QRegExp COMMENT( "\\s*REM\\s+(.*)\\s+(.*)\\s*" );
	QRegExp SINGLE( "\\s*(CATALOG|CDTEXTFILE|ISRC|PERFORMER|SONGWRITER|TITLE)\\s+(.*)\\s*" );
	QRegExp FILES( "\\s*FILE\\s+(.*)\\s+(WAVE)\\s*" );
	QRegExp FLAGS( "\\s*FLAGS\\s+(DATA|DCP|4CH|PRE|SCMS)\\s*" );
	QRegExp TRACK( "\\s*TRACK\\s+(\\d+)\\s+(AUDIO)\\s*" );
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

	// set track album
	foreach( TrackSP track, this->playList_ ) {
		track->setAlbum( this->album_ );
	}

//	this->setMedia_( QUrl::fromLocalFile( currentFile.first ) );
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
	// get the total length, because cue sheet don't provide it
	// FIXME: not always local file
	codec::ReaderSP decoder( plugin::createReader( text::getSuffix( this->currentFilePath_ ) ) );
	try {
		// NOTE: may throw exception
		decoder->open( QUrl::fromLocalFile( this->currentFilePath_ ) );
	} catch( error::CodecError & e ) {
		qDebug() << e.getMessage();
	}
	if( decoder->isOpen() ) {
		// set bit rate, channels, sample rate
		foreach( TrackSP track, this->playList_ ) {
			track->setBitRate( decoder->getBitRate() );
			track->setChannels( decoder->getChannels() );
			track->setSampleRate( decoder->getSampleRate() );
		}

		this->currentTrack_->setDuration( Timestamp::fromMillisecond( decoder->getDuration() ) - this->currentTrack_->getStartTime() );

		decoder->close();
	}

	this->currentFilePath_ = dir.filePath( stripQuote( fileName ) );
	this->currentFileType_ = type;
}

void CueSheetParser::parseFlags_( const QString & flag ) {
	QSet< QString > flags( this->album_->getFlags() );
	flags.insert( flag );
	this->album_->setFlags( flags );
}

void CueSheetParser::parseIndex_( const QString & type, const QString & num, const QString & m, const QString & s, const QString & f ) {
	Timestamp tmp( m.toInt(), s.toShort(), f.toShort() / 75.0 * 100 );

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
			// track start time
			this->currentTrack_->setStartTime( tmp );
			if( this->trackIndex_ > 1 && this->previousTrack_->getDuration().isZero() ) {
				this->previousTrack_->setDuration( this->currentTrack_->getStartTime() - this->previousTrack_->getStartTime() );
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
	this->currentTrack_.reset( new CueSheetTrack );

	this->trackIndex_ = num.toUInt();
	this->currentTrack_->setIndex( num.toShort() );
	this->currentTrack_->setFileType( this->currentFileType_ );
	this->currentTrack_->setDataType( type );
	this->currentTrack_->setURI( QUrl::fromLocalFile( this->currentFilePath_ ) );

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
