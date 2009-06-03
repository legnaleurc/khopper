/**
 * @file converter.cpp
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
#include "converter.hpp"
#include "plugin/abstractreadercreator.hpp"
#include "util/error.hpp"
#include "util/text.hpp"

namespace khopper {

	namespace widget {

		Converter::Converter( QObject * parent ):
		QObject( parent ),
		canceled_( false ) {
		}

		void Converter::convert( album::TrackCSP track, const QString & targetPath, codec::WriterSP encoder ) {
			codec::ReaderSP decoder( plugin::createReader( text::getSuffix( track->getFilePath() ) ) );
			decoder->open( track->getFilePath().toStdWString() );
			encoder->setTimebase( decoder->getTimebase() );
			encoder->open( targetPath.toStdWString() );
			this->canceled_ = false;

			if( !decoder->isOpen() || !encoder->isOpen() ) {
				throw error::RunTimeError( "Can not open decoder or encoder!" );
			}

			double begin = track->getStartTime().toSecond();
			double end = begin + track->getDuration().toSecond();
			decoder->setRange( begin, end );
			if( !decoder->seek( begin ) ) {
				throw error::CodecError( "Invalid start point" );
			}

			double decoded;
			while( decoder->hasNext() ) {
				if( this->canceled_ ) {
					break;
				}
				encoder->write( decoder->read( decoded ) );
				emit this->decodedTime( static_cast< int >( decoded * 1000 ) );
			}

			encoder->close();
			decoder->close();
		}

		void Converter::cancel() {
			this->canceled_ = true;
		}

	}

}
