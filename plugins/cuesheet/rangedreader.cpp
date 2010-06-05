/**
 * @file rangedreader.cpp
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
#include "rangedreader.hpp"

using namespace khopper::codec;
using khopper::plugin::createReader;

RangedReader::RangedReader( const QUrl & uri ):
AbstractReader( uri ),
client_(),
msBegin_( 0 ),
msCurrent_( 0 ),
msDuration_( -1 ) {
	this->client_ = createReader( uri );
}

bool RangedReader::atEnd() const {
	return this->client_->atEnd();
}

qint64 RangedReader::pos() const {
	return this->client_->pos() - this->msBegin_;
}

void RangedReader::setRange( qint64 msBegin, qint64 msDuration ) {
	this->msBegin_ = msBegin;
	this->msDuration_ = msDuration;
	this->setDuration( msDuration );
}

qint64 RangedReader::size() const {
	return this->msDuration_;
}

void RangedReader::doOpen() {
	this->client_->open( ReadOnly );

	this->setAlbum( this->client_->getAlbum() );
	this->setArtist( this->client_->getArtist() );
	this->setBitRate( this->client_->getBitRate() );
	this->setChannelLayout( this->client_->getChannelLayout() );
	this->setChannels( this->client_->getChannels() );
	this->setComment( this->client_->getComment() );
	this->setCopyright( this->client_->getCopyright() );
	this->setDuration( this->msDuration_ );
	this->setGenre( this->client_->getGenre() );
	this->setIndex( this->client_->getIndex() );
	this->setSampleFormat( this->client_->getSampleFormat() );
	this->setTitle( this->client_->getTitle() );
	this->setYear( this->client_->getYear() );

	this->client_->seek( this->msBegin_ );
}

void RangedReader::doClose() {
	this->msBegin_ = 0;
	this->msCurrent_ = 0;
	this->msDuration_ = -1;
	this->client_->close();
}

QByteArray RangedReader::readFrame() {
	QByteArray frame( this->client_->read( this->getSampleRate() * this->getChannels() ) );
	qint64 msDuration = frame.size() * 1000 / this->getSampleRate() / this->getChannels();
	qint64 msBegin = this->pos() - msDuration;

	qint64 begin = 0, duration = -1;
	
	if( msBegin < this->msBegin_ && msBegin + msDuration >= this->msBegin_ ) {
		begin = ( this->msBegin_ - msBegin ) * this->getSampleRate() / 1000 * this->getChannels();
	}
	if( msBegin < this->msBegin_ + this->msDuration_ && msBegin + msDuration >= this->msBegin_ + this->msDuration_ ) {
		duration = ( ( msBegin + msDuration ) - ( this->msBegin_ + this->msDuration_ ) ) * this->getSampleRate() / 1000 * this->getChannels();
	}
	return frame.mid( begin, duration );
}

bool RangedReader::seekFrame( qint64 msPos ) {
	if( msPos >= this->msBegin_ && this->msBegin_ + msPos < this->msDuration_ ) {
		return this->client_->seek( this->msBegin_ + msPos );
	}
	return false;
}
