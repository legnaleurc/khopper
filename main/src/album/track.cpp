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
#include "readerplugin.hpp"
#include "text.hpp"
#include "error.hpp"

namespace khopper {

	namespace album {

		Track::Track():
		album(),
		artist(),
		bitRate( 0 ),
		channels( 0 ),
		comments(),
		dataType( AUDIO ),
		duration(),
		filePath(),
		fileType( BINARY ),
		flags( NONE ),
		garbage(),
		index( 0 ),
		isrc(),
		postGap(),
		preGap(),
		sampleRate(),
		songWriter(),
		startTime(),
		textCodec( QTextCodec::codecForName( "UTF-8" ) ),
		title() {
		}

		void Track::load( const QByteArray & filePath ) {
			this->filePath = filePath;

			codec::ReaderSP decoder( plugin::createReader( text::getSuffix( filePath ) ) );
			decoder->open( text::toLocale( this->filePath ) );
			if( decoder->isOpen() ) {
				this->album = decoder->getAlbum();
				this->artist = decoder->getArtist() );
				this->bitRate = decoder->getBitRate();
				this->channels = decoder->getChannels();
				this->duration = decoder->getDuration();
				this->sampleRate = decoder->getSampleRate();
				this->title = decoder->getTitle();

				decoder->close();
			} else {
				throw Error< codec::Codec >( "Can not open file!" );
			}
		}

}
