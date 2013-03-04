/**
 * @file writer.cpp
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
#include "writer.hpp"

#include <cassert>

#include "baseerror.hpp"

namespace khopper {
namespace codec {
class Writer::Private {
public:
	explicit Private( const QUrl & uri );

	QByteArray album;
	QByteArray artist;
	unsigned int bitRate;
	ChannelLayout channelLayout;
	AudioFormat format;
	QUrl uri;
	QByteArray title;
};
}
}

using khopper::codec::Writer;
using khopper::codec::AudioFormat;
using khopper::codec::ChannelLayout;
using khopper::error::BaseError;

Writer::Writer( const QUrl & uri ):
p_( new Private( uri ) ) {
}

Writer::~Writer() {
}

bool Writer::open( OpenMode /*mode*/ ) {
	if( this->isOpen() ) {
		this->close();
	}

	bool opened = this->QIODevice::open( WriteOnly );
	bool good = false;
	try {
		this->doOpen();
		good = true;
	} catch( BaseError & e ) {
		this->setErrorString( e.getMessage() );
	} catch( std::exception & e ) {
		this->setErrorString( QString::fromAscii( e.what() ) );
	}

	return opened && good;
}

void Writer::close() {
	if( !this->isOpen() ) {
		return;
	}

	try {
		this->doClose();
	} catch( BaseError & e ) {
		this->setErrorString( e.getMessage() );
	} catch( std::exception & e ) {
		this->setErrorString( QString::fromAscii( e.what() ) );
	} catch( ... ) {
		// TODO: log an error
	}
	this->QIODevice::close();
}

qint64 Writer::readData( char * /*data*/, qint64 /*maxlen*/ ) {
	return -1;
}

const QUrl & Writer::getURI() const {
	return this->p_->uri;
}

unsigned int Writer::getBitRate() const {
	return this->p_->bitRate;
}

const QByteArray & Writer::getAlbum() const {
	return this->p_->album;
}

const QByteArray & Writer::getArtist() const {
	return this->p_->artist;
}

const QByteArray & Writer::getTitle() const {
	return this->p_->title;
}

void Writer::setBitRate( unsigned int bitRate ) {
	this->p_->bitRate = bitRate;
}

void Writer::setAlbum( const QByteArray & album ) {
	this->p_->album = album;
}

void Writer::setArtist( const QByteArray & artist ) {
	this->p_->artist = artist;
}

void Writer::setTitle( const QByteArray & title ) {
	this->p_->title = title;
}

void Writer::setChannelLayout( ChannelLayout channelLayout ) {
	this->p_->channelLayout = channelLayout;
}

const AudioFormat & Writer::getAudioFormat() const {
	return this->p_->format;
}

void Writer::setAudioFormat( const AudioFormat & format ) {
	this->p_->format = format;
}

Writer::Private::Private( const QUrl & uri ):
album(),
artist(),
bitRate( 0 ),
channelLayout( LayoutNative ),
format(),
uri( uri ),
title() {
}
