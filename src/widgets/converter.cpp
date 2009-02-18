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
#include "decoder.hpp"
#include "error.hpp"
#include "text.hpp"

namespace Khopper {

	Converter::Converter( QObject * parent ):
	QObject( parent ),
	canceled_( false ) {
	}

	void Converter::convert( TrackSP track, const std::wstring & targetPath, codec::AudioWriterSP encoder ) {
		codec::DecoderSP decoder( new codec::Decoder );
		decoder->open( track->filePath );
		encoder->open( text::toLocale( targetPath ) );
		this->canceled_ = false;

		if( !decoder->is_open() || !encoder->isOpen() ) {
			throw Error< RunTime >( "Can not open decoder or encoder!" );
		}

		double begin = track->startTime.toDouble();
		double end = begin + track->duration.toDouble();
		decoder->setRange( begin, end );

		double decoded;
		while( decoder->hasNext() ) {
			if( this->canceled_ ) {
				break;
			}
			encoder->write( decoder->read( decoded ) );
			emit this->decodedTime( static_cast< int >( decoded * 10000 ) );
		}

		encoder->close();
		decoder->close();
	}

	void Converter::cancel() {
		this->canceled_ = true;
	}

}
