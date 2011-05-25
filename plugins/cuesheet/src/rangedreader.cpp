/**
 * @file rangedreader.cpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * This plug-in is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This plug-in is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include "rangedreader.hpp"

#include <cstring>

namespace {

	inline static qint64 posFromMs( qint64 ms, const khopper::codec::AudioFormat & format ) {
		return ms * format.frequency() * format.channels() * format.sampleSize() / 8 / 1000;
	}

	inline static qint64 msFromPos( qint64 pos, const khopper::codec::AudioFormat & format ) {
		return pos * 8 * 1000 / format.frequency() / format.channels() / format.sampleSize();
	}

}

using namespace khopper::codec;
using khopper::plugin::getReaderCreator;

RangedReader::RangedReader( const QUrl & uri, qint64 msBegin, qint64 msDuration ):
AbstractReader( uri ),
client_(),
msBegin_( msBegin ),
msDuration_( msDuration ) {
	this->client_ = getReaderCreator( uri )( uri );
}

bool RangedReader::atEnd() const {
	return this->client_->atEnd() || this->client_->pos() >= posFromMs( this->msBegin_ + this->msDuration_, this->getAudioFormat() );
}

qint64 RangedReader::size() const {
	return posFromMs( this->msDuration_, this->getAudioFormat() );
}

void RangedReader::doOpen() {
	this->client_->open( ReadOnly );

	this->setAlbumTitle( this->client_->getAlbumTitle() );
	this->setArtist( this->client_->getArtist() );
	this->setAudioFormat( this->client_->getAudioFormat() );
	this->setBitRate( this->client_->getBitRate() );
	this->setChannelLayout( this->client_->getChannelLayout() );
	this->setComment( this->client_->getComment() );
	this->setCopyright( this->client_->getCopyright() );
	this->setDuration( this->msDuration_ );
	this->setGenre( this->client_->getGenre() );
	this->setIndex( this->client_->getIndex() );
	this->setTitle( this->client_->getTitle() );
	this->setYear( this->client_->getYear() );

	this->client_->seek( posFromMs( this->msBegin_, this->getAudioFormat() ) );
}

void RangedReader::doClose() {
	this->client_->close();
}

qint64 RangedReader::readData( char * data, qint64 maxSize ) {
	qint64 frameBegin = this->client_->pos();
	QByteArray frame( this->client_->read( maxSize ) );
	qint64 frameLength = frame.size();
	qint64 begin = posFromMs( this->msBegin_, this->getAudioFormat() );
	qint64 length = posFromMs( this->msDuration_, this->getAudioFormat() );

	qint64 bufferBegin = 0, bufferLength = frameLength;
	
	if( frameBegin < begin && frameBegin + frameLength >= begin ) {
		bufferBegin = ( begin - frameBegin );
	}
	if( frameBegin < begin + length && frameBegin + frameLength >= begin + length ) {
		bufferLength = ( ( frameBegin + frameLength ) - ( begin + length ) );
	}
	std::memcpy( data, frame.constData() + bufferBegin, bufferLength );
	return bufferLength;
}

bool RangedReader::seek( qint64 pos ) {
	qint64 begin = posFromMs( this->msBegin_, this->getAudioFormat() );
	return this->client_->seek( begin + pos );
}
