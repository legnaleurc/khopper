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

#include "khopper/abstractreader.hpp"
#include "khopper/abstractwriter.hpp"
#include "khopper/error.hpp"
#include "khopper/text.hpp"

#include <QtDebug>

using namespace khopper::widget;
using khopper::album::TrackCSP;
using khopper::codec::WriterSP;
using khopper::codec::ReaderSP;
using khopper::plugin::createReader;
using khopper::error::RunTimeError;
using khopper::error::CodecError;

Converter::Converter( TrackCSP track, WriterSP writer ):
QThread( 0 ),
canceled_( false ),
track_( track ),
writer_( writer ) {
}

void Converter::run() {
	ReaderSP decoder( this->track_->getReader() );
	decoder->open( QIODevice::ReadOnly );

	this->writer_->setAudioFormat( decoder->getAudioFormat() );
	this->writer_->setChannelLayout( decoder->getChannelLayout() );

	this->writer_->open( QIODevice::WriteOnly );
	this->canceled_ = false;

	if( !decoder->isOpen() || !this->writer_->isOpen() ) {
		throw RunTimeError( "Can not open decoder or encoder!" );
	}

	int sec = decoder->getAudioFormat().frequency() * decoder->getAudioFormat().channels() * decoder->getAudioFormat().sampleSize() / 8;
	while( !decoder->atEnd() ) {
		if( this->canceled_ ) {
			break;
		}
		QByteArray frame( decoder->read( sec ) );
		this->writer_->write( frame );
		emit this->decodedTime( frame.size() * 1000 / sec );
	}

	this->writer_->close();
	decoder->close();
}

void Converter::cancel() {
	this->canceled_ = true;
}

qint64 Converter::getMaximumValue() const {
	return this->track_->getDuration().toMillisecond();
}

QString Converter::getTitle() const {
	return this->track_->getTitle();
}
