/**
 * @file abstractwriter.cpp
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
#include "abstractwriterprivate.hpp"

#include <cassert>

using namespace khopper::codec;

AbstractWriter::AbstractWriter( const QUrl & uri ):
p_( new AbstractWriterPrivate( uri ) ) {
}

AbstractWriter::~AbstractWriter() {
}

bool AbstractWriter::open( OpenMode /*mode*/ ) {
	if( this->isOpen() ) {
		this->close();
	}

	bool opened = this->QIODevice::open( WriteOnly );
	this->doOpen();

	return opened;
}

void AbstractWriter::close() {
	try {
		this->flush();
		this->doClose();
	} catch( ... ) {
		// TODO: log an error
		assert( !"a plugin can not clean up its own mess ..." );
	}
	// FIXME: know what should be reset
//			this->album_.clear();
//			this->artist_.clear();.
//			this->bitRate_ = -1;
//			this->channelLayout_ = LayoutNative;
//			this->channels_ = -1;
//			this->uri_.clear();
//			this->sampleFormat_ = SF_NONE;
//			this->sampleRate_ = -1;
//			this->title_.clear();
	this->QIODevice::close();
}

qint64 AbstractWriter::readData( char * /*data*/, qint64 /*maxlen*/ ) {
	return -1;
}

qint64 AbstractWriter::writeData( const char * data, qint64 len ) {
	this->writeFrame( QByteArray( data, len ) );
	return len;
}

void AbstractWriter::flush() {
	this->writeData( NULL, 0 );
}

const QUrl & AbstractWriter::getURI() const {
	return this->p_->uri;
}

unsigned int AbstractWriter::getBitRate() const {
	return this->p_->bitRate;
}

const QByteArray & AbstractWriter::getAlbum() const {
	return this->p_->album;
}

const QByteArray & AbstractWriter::getArtist() const {
	return this->p_->artist;
}

const QByteArray & AbstractWriter::getTitle() const {
	return this->p_->title;
}

void AbstractWriter::setBitRate( unsigned int bitRate ) {
	this->p_->bitRate = bitRate;
}

void AbstractWriter::setAlbum( const QByteArray & album ) {
	this->p_->album = album;
}

void AbstractWriter::setArtist( const QByteArray & artist ) {
	this->p_->artist = artist;
}

void AbstractWriter::setTitle( const QByteArray & title ) {
	this->p_->title = title;
}

void AbstractWriter::setChannelLayout( ChannelLayout channelLayout ) {
	this->p_->channelLayout = channelLayout;
}

const AudioFormat & AbstractWriter::getAudioFormat() const {
	return this->p_->format;
}

void AbstractWriter::setAudioFormat( const AudioFormat & format ) {
	this->p_->format = format;
}

AbstractWriter::AbstractWriterPrivate::AbstractWriterPrivate( const QUrl & uri ):
album(),
artist(),
bitRate( 0 ),
channelLayout( LayoutNative ),
format(),
uri( uri ),
title() {
}
