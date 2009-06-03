/**
 * @file abstractreader.cpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include "codec/abstractreader.hpp"
#include "util/text.hpp"

#include <cassert>

namespace khopper {

	namespace codec {

		AbstractReader::AbstractReader():
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
		timebase_(),
		title_(),
		year_( -1 ) {
		}

		AbstractReader::~AbstractReader() {
		}

		void AbstractReader::open( const std::string & filePath ) {
			this->open( text::toStdWString( filePath.c_str() ) );
		}

		void AbstractReader::open( const std::wstring & filePath ) {
			if( this->opening_ ) {
				this->close();
			}
			this->filePath_ = filePath;

			this->openResource();
			this->setupDemuxer();
			this->setupDecoder();
			this->readHeader();

			this->opening_ = true;
			this->hasNext_ = true;
		}

		void AbstractReader::close() {
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
			this->timebase_ = Rational();
			this->title_.clear();
			this->year_ = -1;

			try {
				this->closeResource();
			} catch( ... ) {
				// TODO: log an error
				assert( !"a plugin can not clean up its own mess ..." );
			}

			this->hasNext_ = false;
			this->opening_ = false;
		}

		ByteArray AbstractReader::read( double & decoded ) {
			decoded = 0.0;
			if( !this->opening_ || !this->hasNext_ ) {
				return ByteArray();
			}

			bool stop = false;
			ByteArray data( this->readFrame( decoded, stop ) );
			if( data.empty() || stop ) {
				this->hasNext_ = false;
			}

			return data;
		}

		bool AbstractReader::seek( double timestamp ) {
			bool succeed = this->seekFrame( timestamp );
			if( succeed ) {
				this->hasNext_ = true;
			}
			return succeed;
		}

		bool AbstractReader::afterBegin( double timestamp ) const {
			if( this->begin_ < 0 ) {
				return true;
			}
			return timestamp >= this->begin_;
		}

		bool AbstractReader::afterEnd( double timestamp ) const {
			if( this->end_ < 0 ) {
				return false;
			}
			return timestamp >= this->end_;
		}

	}

}
