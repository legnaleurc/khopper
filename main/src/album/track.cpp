/**
 * @file track.cpp
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
#include "track.hpp"
#include "plugin/abstractreadercreator.hpp"
#include "util/error.hpp"
#include "util/text.hpp"

namespace khopper {

	namespace album {

		Track::Track():
		album_(),
		artist_(),
		bitRate_( 0 ),
		channels_( 0 ),
		comments_(),
		dataType_( AUDIO ),
		duration_(),
		filePath_(),
		fileType_( BINARY ),
		flags_( NONE ),
		garbage_(),
		index_( 0 ),
		isrc_(),
		postGap_(),
		preGap_(),
		sampleRate_(),
		songWriter_(),
		startTime_(),
		textCodec_( QTextCodec::codecForName( "UTF-8" ) ),
		title_() {
		}

		void Track::load( const QString & filePath ) {
			this->filePath_ = filePath;

			codec::ReaderSP decoder( plugin::createReader( text::getSuffix( filePath ) ) );
			decoder->open( filePath.toStdWString() );
			if( decoder->isOpen() ) {
				this->album_ = decoder->getAlbum().c_str();
				this->artist_ = decoder->getArtist().c_str();
				this->bitRate_ = decoder->getBitRate();
				this->channels_ = decoder->getChannels();
				this->duration_ = Index::fromSecond( decoder->getDuration() );
				this->sampleRate_ = decoder->getSampleRate();
				this->title_ = decoder->getTitle().c_str();

				decoder->close();
			} else {
				throw error::CodecError( "Can not open file!" );
			}
		}

	}

}
