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
#include "cuesheet.hpp"
#include "track.hpp"

#include "khopper/abstractreader.hpp"
#include "khopper/error.hpp"
#include "khopper/text.hpp"

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

	struct setAlbum {
		setAlbum( const QString & title ) : title_( title ) {}
		void operator ()( khopper::album::TrackSP track ) {
			track->set( "album", this->title_ );
		}
	private:
		const QString & title_;
	};

	struct setFile {
		setFile( const QUrl & file ) : file_( file ) {}
		void operator ()( khopper::album::TrackSP track ) {
			track->setURI( this->file_ );
		}
	private:
		const QUrl & file_;
	};

	struct setBCS {
		setBCS( khopper::codec::ReaderCSP decoder ) : decoder_( decoder ) {}
		void operator ()( khopper::album::TrackSP track ) {
			track->set( "bit_rate", this->decoder_->getBitRate() );
			track->set( "sample_rate", this->decoder_->getSampleRate() );
			track->set( "channels", this->decoder_->getChannels() );
		}
	private:
		khopper::codec::ReaderCSP decoder_;
	};

}

using namespace khopper::album;

CUESheet::CUESheet() {}

CUESheet::CUESheet( const QString & content, const QString & dirPath ) {
	this->parseCUE_( content, dirPath );
}

void CUESheet::open( const QString & content, const QString & dirPath ) {
	this->parseCUE_( content, dirPath );
}

void CUESheet::setMedia( const QUrl & uri ) {
	std::for_each( this->trackBegin(), this->trackEnd(), setFile( uri ) );

	// get the total length, because cue sheet don't provide it
	// FIXME: not always local file
	codec::ReaderSP decoder( plugin::createReader( text::getSuffix( uri.toLocalFile() ) ) );
	// NOTE: may throw exception
	decoder->open( uri );
	if( decoder->isOpen() ) {
		// set bit rate, channels, sample rate
		std::for_each( this->trackBegin(), this->trackEnd(), setBCS( decoder ) );

		TrackSP last( this->trackBack() );
		last->set( "duration", QVariant::fromValue( Timestamp::fromMillisecond( decoder->getDuration() ) - last->get( "start_time" ).value< Timestamp >() ) );

		decoder->close();
	}
}

void CUESheet::parseCUE_( QString content, const QString & dirPath ) {
	QRegExp COMMENT( "\\s*REM\\s+(.*)\\s+(.*)\\s*" );
	QRegExp SINGLE( "\\s*(CATALOG|CDTEXTFILE|ISRC|PERFORMER|SONGWRITER|TITLE)\\s+(.*)\\s*" );
	QRegExp FILES( "\\s*FILE\\s+(.*)\\s+(WAVE)\\s*" );
	QRegExp FLAGS( "\\s*FLAGS\\s+(DATA|DCP|4CH|PRE|SCMS)\\s*" );
	QRegExp TRACK( "\\s*TRACK\\s+(\\d+)\\s+(AUDIO)\\s*" );
	QRegExp INDEX( "\\s*(INDEX|PREGAP|POSTGAP)\\s+((\\d+)\\s+)?(\\d+):(\\d+):(\\d+)\\s*" );

	int trackNO = -1;
	std::pair< QString, QString > currentFile;
	QTextStream sin( &content, QIODevice::ReadOnly );
	QString line = sin.readLine();

	while( !line.isEmpty() ) {
		if( SINGLE.exactMatch( line ) ) {
			this->parseSingle_( SINGLE.cap( 1 ), SINGLE.cap( 2 ), trackNO );
		} else if( FILES.exactMatch( line ) ) {
			currentFile = this->parseFile_( FILES.cap( 1 ), FILES.cap( 2 ), dirPath );
		} else if( FLAGS.exactMatch( line ) ) {
			this->parseFlags_( FLAGS.cap( 1 ), trackNO );
		} else if( INDEX.exactMatch( line ) ) {
			this->parseIndex_( INDEX.cap( 1 ), INDEX.cap( 3 ), INDEX.cap( 4 ), INDEX.cap( 5 ), INDEX.cap( 6 ), trackNO );
		} else if( COMMENT.exactMatch( line ) ) {
			this->parseComment_( COMMENT.cap( 1 ), COMMENT.cap( 2 ), trackNO );
		} else if( TRACK.exactMatch( line ) ) {
			++trackNO;
			this->parseTrack_( TRACK.cap( 1 ), currentFile, TRACK.cap( 2 ) );
		} else {
			this->parseGarbage_( line, trackNO );
		}
		line = sin.readLine();
	}

	// set track album
	std::for_each( this->trackBegin(), this->trackEnd(), setAlbum( this->get( "title" ).toString() ) );

	this->setMedia( QUrl::fromLocalFile( currentFile.first ) );
}

void CUESheet::parseSingle_( const QString & c, const QString & s, int trackNO ) {
	QString content = stripQuote( s );
	if( c == "CATALOG" ) {
		if( trackNO == -1 ) {
			this->set( "catalog", content );
		}
	} else if( c == "CDTEXTFILE" ) {
		if( trackNO == -1 ) {
			this->set( "cd_text_file", content );
		}
	} else if( c == "ISRC" ) {
		if( trackNO != -1 ) {
			(*this)[trackNO]->set( "isrc", content );
		}
	} else if( c == "PERFORMER" ) {
		if( trackNO == -1 ) {
			this->set( "artist", content );
		} else {
			(*this)[trackNO]->set( "artist", content );
		}
	} else if( c == "SONGWRITER" ) {
		if( trackNO == -1 ) {
			this->set( "song_writer", content );
		} else {
			(*this)[trackNO]->set( "song_writer", content );
		}
	} else if( c == "TITLE" ) {
		if( trackNO == -1 ) {
			this->set( "title", content );
		} else {
			(*this)[trackNO]->set( "title", content );
		}
	}
}

std::pair< QString, QString > CUESheet::parseFile_( const QString & fileName, const QString & type, const QString & dirPath ) {
	QString filePath = text::joinPath( dirPath, stripQuote( fileName ) );
	return std::make_pair( filePath, type );
}

void CUESheet::parseFlags_( const QString & flag, int /*trackNO*/ ) {
	QSet< QString > flags( this->get( "flags" ).value< QSet< QString > >() );
	flags.insert( flag );
	this->set( "flags", QVariant::fromValue< QSet< QString > >( flags ) );
}

void CUESheet::parseIndex_( const QString & type, const QString & num, const QString & m, const QString & s, const QString & f, int trackNO ) {
	Timestamp tmp( m.toInt(), s.toShort(), f.toShort() * 10 );

	if( type == "INDEX" ) {
		short int n = num.toShort();
		switch( n ) {
		case 1:
			// track start time
			(*this)[trackNO]->set( "start_time", QVariant::fromValue( tmp ) );
			if( trackNO > 0 && (*this)[trackNO-1]->get( "duration" ).value< Timestamp >().isZero() ) {
				(*this)[trackNO-1]->set( "duration", QVariant::fromValue( (*this)[trackNO]->get( "start_time" ).value< Timestamp >() - (*this)[trackNO-1]->get( "start_time" ).value< Timestamp >() ) );
			}
			break;
		case 0:
			// prevous track end time
			if( trackNO > 0 ) {
				(*this)[trackNO-1]->set( "duration", QVariant::fromValue( tmp - (*this)[trackNO-1]->get( "start_time" ).value< Timestamp >() ) );
			}
			break;
		default:
			// other values implies error
			throw error::ParsingError( "Index value error!" );
		}
	} else if( type == "PREGAP" ) {
		(*this)[trackNO]->set( "pregap", QVariant::fromValue( tmp ) );
	} else if( type == "POSTGAP" ) {
		(*this)[trackNO]->set( "postgap", QVariant::fromValue( tmp ) );
	}
}

void CUESheet::parseComment_( const QString & key, const QString & value, int trackNO ) {
	if( trackNO == -1 ) {
		QStringList comments( this->get( "comments" ).toStringList() );
		comments.append( QString( "%1:%2;" ).arg( key ).arg( value ) );
		this->set( "comments", comments );
	} else {
		QStringList comments( (*this)[trackNO]->get( "comments" ).toStringList() );
		comments.append( QString( "%1:%2;" ).arg( key ).arg( value ) );
		(*this)[trackNO]->set( "comments", comments );
	}
}

void CUESheet::parseTrack_( const QString & num, const std::pair< QString, QString > & audioData, const QString & type ) {
	TrackSP track( new Track );
	track->set( "index", num.toShort() );
	track->set( "file_type", audioData.second );
	track->set( "data_type", type );
	track->setURI( audioData.first );
	this->pushBack( track );
}

void CUESheet::parseGarbage_( const QString & line, int trackNO ) {
	if( trackNO == -1 ) {
		QStringList garbage( this->get( "garbage" ).toStringList() );
		garbage.append( line );
		this->set( "garbage", garbage );
	} else {
		QStringList garbage = (*this)[trackNO]->get( "garbage" ).toStringList();
		garbage.append( line );
		(*this)[trackNO]->set( "garbage", garbage );
	}
}
