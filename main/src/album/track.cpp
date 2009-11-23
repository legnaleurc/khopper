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
		fields_(),
		uri_(),
		textCodec_( QTextCodec::codecForName( "UTF-8" ) ) {
		}

		void Track::set( const QString & key, const std::string & value ) {
			this->set( key, QVariant( QByteArray( value.c_str() ) ) );
		}

		void Track::set( const QString & key, const QString & value ) {
			this->set( key, QVariant( this->textCodec_->fromUnicode( value ) ) );
		}

		void Track::set( const QString & key, const QVariant & value ) {
			QMap< QString, QVariant >::iterator it = this->fields_.find( key );
			if( it == this->fields_.end() ) {
				this->fields_.insert( key, value );
			} else {
				it.value() = value;
			}
		}

		QVariant Track::get( const QString & key ) const {
			QMap< QString, QVariant >::const_iterator it = this->fields_.find( key );
			if( it == this->fields_.end() ) {
				// no such data, invalid
				return QVariant();
			} else {
				if( it.value().canConvert( QVariant::ByteArray ) ) {
					// should decode using textcodec
					return this->textCodec_->toUnicode( it.value().toByteArray() );
				} else {
					return it.value();
				}
			}
		}

		void Track::load( const QUrl & uri ) {
			this->uri_ = uri;

			// FIXME: not always local file
			codec::ReaderSP decoder( plugin::createReader( text::getSuffix( uri.toLocalFile() ) ) );
			decoder->open( uri );
			if( decoder->isOpen() ) {
				this->set( "album", decoder->getAlbum() );
				this->set( "artist", decoder->getArtist() );
				this->set( "bit_rate", decoder->getBitRate() );
				this->set( "channels", decoder->getChannels() );
				this->set( "duration", QVariant::fromValue( Timestamp::fromMillisecond( decoder->getDuration() ) ) );
				this->set( "sample_rate", decoder->getSampleRate() );
				this->set( "title", decoder->getTitle() );

				decoder->close();
			} else {
				throw error::CodecError( "Can not open file!" );
			}
		}

	}

}
