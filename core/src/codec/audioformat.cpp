/**
 * @file audioformatprivate.cpp
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
#include "audioformat.hpp"
#include "audioformatprivate.hpp"

using namespace khopper::codec;

AudioFormat::AudioFormatPrivate::AudioFormatPrivate():
byteOrder( AudioFormat::Endian( QSysInfo::ByteOrder ) ),
channels( -1 ),
codec(),
frequency( -1 ),
sampleSize( -1 ),
sampleType( Unknown ) {
}

AudioFormat::AudioFormat() : p_( new AudioFormatPrivate ) {
}

AudioFormat::AudioFormat( const AudioFormat & that ) : p_( new AudioFormatPrivate( *that.p_ ) ) {
}

AudioFormat::Endian AudioFormat::byteOrder() const {
	return this->p_->byteOrder;
}

int AudioFormat::channels() const {
	return this->p_->channels;
}

QString AudioFormat::codec() const {
	return this->p_->codec;
}

int AudioFormat::frequency() const {
	return this->p_->frequency;
}

bool AudioFormat::isValid() const {
	return this->p_->channels != -1 && !this->p_->codec.isEmpty() && this->p_->frequency != -1 && this->p_->sampleSize != -1 && this->p_->sampleType != Unknown;
}

int AudioFormat::sampleSize() const {
	return this->p_->sampleSize;
}

AudioFormat::SampleType AudioFormat::sampleType() const {
	return this->p_->sampleType;
}

void AudioFormat::setByteOrder( Endian byteOrder ) {
	this->p_->byteOrder = byteOrder;
}

void AudioFormat::setChannels( int channels ) {
	this->p_->channels = channels;
}

void AudioFormat::setCodec( const QString & codec ) {
	this->p_->codec = codec;
}

void AudioFormat::setFrequency( int frequency ) {
	this->p_->frequency = frequency;
}

void AudioFormat::setSampleSize( int sampleSize ) {
	this->p_->sampleSize = sampleSize;
}

void AudioFormat::setSampleType( SampleType sampleType ) {
	this->p_->sampleType = sampleType;
}

bool AudioFormat::operator !=( const AudioFormat & that ) const {
	return !( *this == that );
}

AudioFormat & AudioFormat::operator =( const AudioFormat & that ) {
	if( this != &that ) {
		*this->p_ = *that.p_;
	}
	return *this;
}

bool AudioFormat::operator ==( const AudioFormat & that ) const {
	return this->p_->byteOrder == that.p_->byteOrder && this->p_->channels == that.p_->channels && this->p_->codec == that.p_->codec && this->p_->frequency == that.p_->frequency && this->p_->sampleSize == that.p_->sampleSize && this->p_->sampleType == that.p_->sampleType;
}
