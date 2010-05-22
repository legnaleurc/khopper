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
#include "abstractwriter.hpp"

#include <QtDebug>

#include <cassert>

namespace khopper {

	namespace codec {

		AbstractWriter::AbstractWriter():
		album_(),
		artist_(),
		bitRate_( -1 ),
		channelLayout_( LayoutNative ),
		channels_( -1 ),
		uri_(),
		opening_( false ),
		sampleFormat_( SF_NONE ),
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
			// FIXME: know what should be reset
//			this->album_.clear();
//			this->artist_.clear();.
//			this->bitRate_ = -1;
//			this->channelLayout_ = LayoutNative;
//			this->channels_ = -1;
//			this->uri_.clear();
//			this->sampleFormat_ = SF_NONE;
//			this->sampleRate_ = -1;
//			this->title_.clear();
			this->opening_ = false;
		}

		void AbstractWriter::write( const ByteArray & data ) {
			if( !this->isOpen() ) {
				return;
			}
			this->writeFrame( data );
		}

		void AbstractWriter::flush() {
			this->write();
		}

	}

}
