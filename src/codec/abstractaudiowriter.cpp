/**
 * @file abstractaudiowriter.cpp
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
 #include "abstractaudiowriter.hpp"

namespace Khopper {

	namespace codec {

		AbstractAudioWriter::AbstractAudioWriter():
		album_(),
		artist_(),
		bitRate_( -1 ),
		channels_( -1 ),
		filePath_(),
		opening_( false ),
		quality_( -1.0 ),
		sampleBuffer_(),
		sampleQueue_(),
		sampleRate_( -1 ),
		title_() {
		}

		AbstractAudioWriter::~AbstractAudioWriter() {
		}

		void AbstractAudioWriter::open( const std::string & filePath ) {
			this->filePath_ = filePath;
			if( this->isOpen() ) {
				this->close();
			}

			this->setupMuxer_();
			this->setupEncoder_();
			this->openResouse_();
			this->writeHeader_();

			this->opening_ = true;
		}

		void AbstractAudioWriter::close() {
			this->flush();
			this->closeResouse_();
			this->opening_ = false;
		}

		void AbstractAudioWriter::write( const ByteArray & data ) {
			this->sampleQueue_.insert( this->sampleQueue_.end(), data.begin(), data.end() );

			while( this->sampleQueue_.size() >= this->sampleBuffer_.size() ) {
				ByteArray::iterator copyEnd = this->sampleQueue_.begin() + this->sampleBuffer_.size();
				std::copy( this->sampleQueue_.begin(), copyEnd, this->sampleBuffer_.begin() );
				this->sampleQueue_.erase( this->sampleQueue_.begin(), copyEnd );

				this->writeFrame_( &this->sampleBuffer_[0], this->sampleBuffer_.size() );
			}
		}

		void AbstractAudioWriter::flush() {
			if( this->opening_ && !this->sampleQueue_.empty() ) {
				std::copy( this->sampleQueue_.begin(), this->sampleQueue_.end(), this->sampleBuffer_.begin() );
				this->writeFrame_( &this->sampleBuffer_[0], this->sampleBuffer_.size() );
				this->sampleQueue_.clear();
			}
		}

	}

}