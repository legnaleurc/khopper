/**
 * @file abstractaudioreader.cpp
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
#include "abstractaudioreader.hpp"
#include "error.hpp"

namespace khopper {

	namespace codec {

		AbstractAudioReader::AbstractAudioReader():
		album_(),
		artist_(),
		begin_( -1.0 ),
		bitRate_( -1 ),
		channels_( -1 ),
		comment_(),
		copyright_(),
		duration_( -1.0 ),
		end_( -1.0 ),
		filePath_(),
		genre_(),
		hasNext_( false ),
		index_( -1 ),
		opening_( false ),
		sampleRate_( -1 ),
		title_(),
		year_( -1 ) {
		}

		AbstractAudioReader::~AbstractAudioReader() {
		}

		void AbstractAudioReader::open( const std::string & filePath ) {
			if( this->opening_ ) {
				this->close();
			}
			this->filePath_ = filePath;

			this->openResource_();
			this->setupDemuxer_();
			this->setupDecoder_();
			this->readHeader_();

			this->opening_ = true;
			this->hasNext_ = true;
		}

		void AbstractAudioReader::close() {
			this->album_.clear();
			this->artist_.clear();
			this->begin_ = -1.0;
			this->bitRate_ = -1;
			this->channels_ = -1;
			this->comment_.clear();
			this->copyright_.clear();
			this->duration_ = -1.0;
			this->filePath_.clear();
			this->end_ = -1.0;
			this->genre_.clear();
			this->index_ = -1;
			this->sampleRate_ = -1;
			this->title_.clear();
			this->year_ = -1;

			this->closeResource_();

			this->hasNext_ = false;
			this->opening_ = false;
		}

		ByteArray AbstractAudioReader::read( double & decoded ) {
			decoded = 0.0;
			if( !this->opening_ || !this->hasNext_ ) {
				return ByteArray();
			}

			bool stop = false;
			ByteArray data( this->readFrame_( decoded, stop ) );
			if( data.empty() || stop ) {
				this->hasNext_ = false;
			}

			return data;
		}

		bool AbstractAudioReader::seek( double timestamp ) {
			bool succeed = this->seek_( timestamp );
			if( succeed ) {
				this->hasNext_ = true;
			}
			return succeed;
		}

		bool AbstractAudioReader::afterBegin( double timestamp ) const {
			if( this->begin_ < 0 ) {
				return true;
			}
			return timestamp >= this->begin_;
		}

		bool AbstractAudioReader::afterEnd( double timestamp ) const {
			if( this->end_ < 0 ) {
				return false;
			}
			return timestamp >= this->end_;
		}

	}

	namespace plugin {

		bool registerReader( const std::string & key, const std::string & plugin ) {
			return AudioReaderFactory::Instance().Register( key, CreatorLoader< AudioReaderCreator >( plugin ) );
		}

		codec::AudioReaderSP createReader( const std::string & key ) {
			AudioReaderCreator * tmp = NULL;
			try {
				tmp = AudioReaderFactory::Instance().CreateObject( key );
			} catch( std::exception & e ) {
				tmp = AudioReaderFactory::Instance().CreateObject( "*" );
			}
			return tmp->create();
		}

	}

}
