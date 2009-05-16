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
#include "plugin/abstractreadercreator.hpp"
#include "util/error.hpp"
#include "util/os.hpp"
#include "util/text.hpp"

#include <QTextStream>
#include <QRegExp>
#include <QtDebug>

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
			track->setAlbum( this->title_ );
		}
	private:
		const QString & title_;
	};

	struct setFile {
		setFile( const QString & file ) : file_( file ) {}
		void operator ()( khopper::album::TrackSP track ) {
			track->setFilePath( this->file_ );
		}
	private:
		const QString & file_;
	};

	struct setBCS {
		setBCS( khopper::codec::ReaderCSP decoder ) : decoder_( decoder ) {}
		void operator ()( khopper::album::TrackSP track ) {
			track->setBitRate( this->decoder_->getBitRate() );
			track->setSampleRate( this->decoder_->getSampleRate() );
			track->setChannels( this->decoder_->getChannels() );
		}
	private:
		khopper::codec::ReaderCSP decoder_;
	};

}

namespace khopper {

	namespace album {

		CUESheet::CUESheet() {}

		CUESheet::CUESheet( const QString & content, const QString & dirPath ) {
			this->parseCUE_( content, dirPath );
		}

		void CUESheet::open( const QString & content, const QString & dirPath ) {
			this->parseCUE_( content, dirPath );
		}

		void CUESheet::setMedia( const QString & filePath ) {
			std::for_each( this->tracks_.begin(), this->tracks_.end(), ::setFile( filePath ) );

			// get the total length, because cue sheet don't provide it
			codec::ReaderSP decoder( plugin::createReader( text::getSuffix( filePath ) ) );
			// NOTE: may throw exception
			decoder->open( filePath.toStdWString() );
			if( decoder->isOpen() ) {
				// set bit rate, channels, sample rate
				std::for_each( this->tracks_.begin(), this->tracks_.end(), ::setBCS( decoder ) );

				TrackSP last( this->tracks_.back() );
				last->setDuration( Index::fromSecond( decoder->getDuration() ) - last->getStartTime() );

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
			std::pair< QString, Track::FileType > currentFile;
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
			std::for_each( this->tracks_.begin(), this->tracks_.end(), ::setAlbum( this->title_ ) );

			this->setMedia( currentFile.first );
		}

		void CUESheet::parseSingle_( const QString & c, const QString & s, int trackNO ) {
			QString content = stripQuote( s );
			if( c == "CATALOG" ) {
				if( trackNO == -1 ) {
					this->catalog_ = content;
				}
			} else if( c == "CDTEXTFILE" ) {
				if( trackNO == -1 ) {
					this->cdTextFile_ = content;
				}
			} else if( c == "ISRC" ) {
				if( trackNO != -1 ) {
					this->tracks_[trackNO]->setISRC( content );
				}
			} else if( c == "PERFORMER" ) {
				if( trackNO == -1 ) {
					this->artist_ = content;
				} else {
					this->tracks_[trackNO]->setArtist( content );
				}
			} else if( c == "SONGWRITER" ) {
				if( trackNO == -1 ) {
					this->songWriter_ = content;
				} else {
					this->tracks_[trackNO]->setSongWriter( content );
				}
			} else if( c == "TITLE" ) {
				if( trackNO == -1 ) {
					this->title_ = content;
				} else {
					this->tracks_[trackNO]->setTitle( content );
				}
			}
		}

		std::pair< QString, Track::FileType > CUESheet::parseFile_( const QString & fileName, const QString & type, const QString & dirPath ) {
			QString filePath = os::join( dirPath, stripQuote( fileName ) );
			if( type == "BINARY" ) {
				return std::make_pair( filePath, Track::BINARY );
			} else if( type == "MOTOROLA" ) {
				return std::make_pair( filePath, Track::MOTOROLA );
			} else if( type == "AIFF" ) {
				return std::make_pair( filePath, Track::AIFF );
			} else if( type == "WAVE" ) {
				return std::make_pair( filePath, Track::WAVE );
			} else if( type == "MP3" ) {
				return std::make_pair( filePath, Track::MP3 );
			} else {
				return std::make_pair( filePath, Track::BINARY );
			}
		}

		void CUESheet::parseFlags_( const QString & flag, int trackNO ) {
			if( trackNO != -1 ) {
				if( flag == "DATA" ) {
					this->tracks_[trackNO]->addFlag( Track::DATA );
				} else if( flag == "DCP" ) {
					this->tracks_[trackNO]->addFlag( Track::DCP );
				} else if( flag == "4CH" ) {
					this->tracks_[trackNO]->addFlag( Track::CH4 );
				} else if( flag == "PRE" ) {
					this->tracks_[trackNO]->addFlag( Track::PRE );
				} else if( flag == "SCMS" ) {
					this->tracks_[trackNO]->addFlag( Track::SCMS );
				}
			}
		}

		void CUESheet::parseIndex_( const QString & type, const QString & num, const QString & m, const QString & s, const QString & f, int trackNO ) {
			Index tmp( m.toShort(), s.toShort(), f.toShort() * 10 );

			if( type == "INDEX" ) {
				short int n = num.toShort();
				switch( n ) {
				case 1:
					// track start time
					this->tracks_[trackNO]->setStartTime( tmp );
					if( trackNO > 0 && this->tracks_[trackNO-1]->getDuration().isZero() ) {
						this->tracks_[trackNO-1]->setDuration( this->tracks_[trackNO]->getStartTime() - this->tracks_[trackNO-1]->getStartTime() );
					}
					break;
				case 0:
					// prevous track end time
					if( trackNO > 0 ) {
						this->tracks_[trackNO-1]->setDuration( tmp - this->tracks_[trackNO-1]->getStartTime() );
					}
					break;
				default:
					// other values implies error
					throw error::ParsingError( "Index value error!" );
				}
			} else if( type == "PREGAP" ) {
				this->tracks_[trackNO]->setPreGap( tmp );
			} else if( type == "POSTGAP" ) {
				this->tracks_[trackNO]->setPostGap( tmp );
			}
		}

		void CUESheet::parseComment_( const QString & key, const QString & value, int trackNO ) {
			if( trackNO == -1 ) {
				this->comments_.insert( std::make_pair( key, value ) );
			} else {
				this->tracks_[trackNO]->addComment( key, value );
			}
		}

		void CUESheet::parseTrack_( const QString & num, const std::pair< QString, Track::FileType > & audioData, const QString & type ) {
			TrackSP track( new Track );
			track->setIndex( num.toShort() );
			track->setFilePath( audioData.first );
			if( type == "AUDIO" ) {
				track->setDataType( Track::AUDIO );
			} else if( type == "CDG" ) {
				track->setDataType( Track::CDG );
			} else if( type == "MODE1/2048" ) {
				track->setDataType( Track::MODE1_2048 );
			} else if( type == "MODE1/2352" ) {
				track->setDataType( Track::MODE1_2352 );
			} else if( type == "MODE2/2336" ) {
				track->setDataType( Track::MODE2_2336 );
			} else if( type == "MODE2/2352" ) {
				track->setDataType( Track::MODE2_2352 );
			} else if( type == "CDI/2336" ) {
				track->setDataType( Track::CDI_2336 );
			} else if( type == "CDI/2352" ) {
				track->setDataType( Track::CDI_2352 );
			} else {
				track->setDataType( Track::AUDIO );
			}
			this->tracks_.push_back( track );
		}

		void CUESheet::parseGarbage_( const QString & line, int trackNO ) {
			if( trackNO == -1 ) {
				this->garbage_.push_back( line );
			} else {
				this->tracks_[trackNO]->addGarbage( line );
			}
		}

	}

}
