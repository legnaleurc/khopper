/**
 * @file abstractwriter.cpp
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
#include "codec/abstractwriter.hpp"

#include <cassert>
#include <QtDebug>

namespace khopper {

	namespace codec {

		AbstractWriter::AbstractWriter():
		album_(),
		artist_(),
		bitRate_( -1 ),
		channels_( -1 ),
		uri_(),
		opening_( false ),
		quality_( 0.0 ),
		sampleBuffer_(),
		sampleQueue_(),
		sampleRate_( -1 ),
		title_() {
		}

		AbstractWriter::~AbstractWriter() {
		}

		void AbstractWriter::open( const QUrl & uri ) {
			this->uri_ = uri;
			if( this->isOpen() ) {
				this->close();
			}

			this->doOpen();

			this->opening_ = true;
		}

		void AbstractWriter::close() {
			try {
				this->flush();
				this->doClose();
			} catch( ... ) {
				// TODO: log an error
				assert( !"a plugin can not clean up its own mess ..." );
			}
			this->opening_ = false;
		}

		void AbstractWriter::write( const ByteArray & data ) {
			this->sampleQueue_.insert( this->sampleQueue_.end(), data.begin(), data.end() );

			while( this->sampleQueue_.size() >= this->sampleBuffer_.size() ) {
				ByteArray::iterator copyEnd = this->sampleQueue_.begin() + this->sampleBuffer_.size();
				std::copy( this->sampleQueue_.begin(), copyEnd, this->sampleBuffer_.begin() );
				this->sampleQueue_.erase( this->sampleQueue_.begin(), copyEnd );

				this->writeFrame( &this->sampleBuffer_[0], this->sampleBuffer_.size() );
			}
		}

		void AbstractWriter::flush() {
			if( this->opening_ && !this->sampleQueue_.empty() ) {
				std::copy( this->sampleQueue_.begin(), this->sampleQueue_.end(), this->sampleBuffer_.begin() );
				this->writeFrame( &this->sampleBuffer_[0], this->sampleBuffer_.size() );
				this->sampleQueue_.clear();
			}
		}

	}

}
