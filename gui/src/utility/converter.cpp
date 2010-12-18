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

using namespace khopper::utility;
using khopper::album::TrackCSP;
using khopper::codec::WriterSP;
using khopper::codec::ReaderSP;
using khopper::error::BaseError;
using khopper::error::CodecError;

Converter::Converter( TrackCSP track, WriterSP writer ):
QThread( 0 ),
canceled_( false ),
track_( track ),
writer_( writer ) {
	this->writer_->setAlbum( this->track_->getAlbum()->getTitle().toUtf8() );
	this->writer_->setArtist( this->track_->getArtist().toUtf8() );
	this->writer_->setTitle( this->track_->getTitle().toUtf8() );
}

void Converter::run() {
	ReaderSP decoder( this->track_->createReader() );
	try {
		decoder->open( QIODevice::ReadOnly );
	} catch( BaseError & e ) {
		emit this->errorOccured( tr( "Decoder Error" ), e.getMessage() );
		return;
	}

	this->writer_->setAudioFormat( decoder->getAudioFormat() );
	this->writer_->setChannelLayout( decoder->getChannelLayout() );

	try {
		this->writer_->open( QIODevice::WriteOnly );
	} catch( BaseError & e ) {
		emit this->errorOccured( tr( "Encoder Error" ), e.getMessage() );
		return;
	}
	this->canceled_ = false;

	const int sec = decoder->getAudioFormat().frequency() * decoder->getAudioFormat().channels() * decoder->getAudioFormat().sampleSize() / 8;
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
