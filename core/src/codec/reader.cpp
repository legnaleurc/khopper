/**
 * @file reader.cpp
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
#include "reader.hpp"

#include <cassert>
#include <cmath>
#include <cstring>

#include "core/factory.hpp"
#include "runtimeerror.hpp"


namespace khopper {
namespace codec {

class Reader::Private {
public:
	Private( const QUrl & uri );

	QByteArray album;
	QByteArray artist;
	unsigned int bitRate;
	QByteArray buffer;
	ChannelLayout channelLayout;
	QByteArray comment;
	QByteArray copyright;
	AudioFormat format;
	qint64 msDuration;
	QUrl uri;
	QByteArray genre;
	unsigned int index;
	QByteArray title;
	QString year;
};

template< typename KeyType, typename CreatorType >
class FactoryError {
public:
	CreatorType onError( const KeyType & /*key*/ ) const {
		return nullptr;
	}
};

typedef core::Factory< QString, QUrl, ReaderSP, FactoryError > Factory;

Factory factory;

}
}

using khopper::codec::Reader;
using khopper::codec::AudioFormat;
using khopper::codec::ChannelLayout;
using khopper::error::BaseError;
using khopper::error::RunTimeError;

bool khopper::codec::Reader::install( const QString & id, Verifier v, Creator c ) {
	return factory.installCreator( id, v, c );
}

bool khopper::codec::Reader::uninstall( const QString & id ) {
	return factory.uninstallCreator( id );
}

Reader::Creator khopper::codec::Reader::getCreator( const QUrl & uri ) {
	auto creator = factory.getCreator( uri );
	if( !creator ) {
		throw RunTimeError( QObject::tr( "find no suitable codec" ), __FILE__, __LINE__ );
	}
	return creator;
}

Reader::Reader( const QUrl & uri ):
QIODevice(),
p_( new Private( uri ) ) {
}

Reader::~Reader() {
}

bool Reader::open( OpenMode mode ) {
	if( this->isOpen() ) {
		this->close();
	}

	bool opened = this->QIODevice::open( mode & ~( QIODevice::WriteOnly & QIODevice::Text ) );
	bool good = false;
	try {
		this->doOpen();
		good = true;
	} catch( BaseError & e ) {
		this->setErrorString( e.getMessage() );
		this->close();
	} catch( std::exception & e ) {
		this->setErrorString( QString::fromAscii( e.what() ) );
		this->close();
	}

	return opened && good;
}

void Reader::close() {
	if( !this->isOpen() ) {
		return;
	}

	this->p_.reset( new Private( this->p_->uri ) );

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

qint64 Reader::writeData( const char * /*data*/, qint64 /*maxSize*/ ) {
	return -1;
}

const QUrl & Reader::getURI() const {
	return this->p_->uri;
}

void Reader::setDuration( qint64 msDuration ) {
	this->p_->msDuration = msDuration;
}

void Reader::setChannelLayout( ChannelLayout channelLayout ) {
	this->p_->channelLayout = channelLayout;
}

void Reader::setBitRate( unsigned int bitRate ) {
	this->p_->bitRate = bitRate;
}

void Reader::setGenre( const QByteArray & genre ) {
	this->p_->genre = genre;
}

void Reader::setIndex( unsigned int index ) {
	this->p_->index = index;
}

void Reader::setYear( const QString & year ) {
	this->p_->year = year;
}

void Reader::setAlbumTitle( const QByteArray & album ) {
	this->p_->album = album;
}

void Reader::setComment( const QByteArray & comment ) {
	this->p_->comment = comment;
}

void Reader::setCopyright( const QByteArray & copyright ) {
	this->p_->copyright = copyright;
}

void Reader::setArtist( const QByteArray & artist ) {
	this->p_->artist = artist;
}

void Reader::setTitle( const QByteArray & title ) {
	this->p_->title = title;
}

qint64 Reader::getDuration() const {
	return this->p_->msDuration;
}

const QByteArray & Reader::getTitle() const {
	return this->p_->title;
}

unsigned int Reader::getBitRate() const {
	return this->p_->bitRate;
}

const QByteArray & Reader::getArtist() const {
	return this->p_->artist;
}

const QByteArray & Reader::getAlbumTitle() const {
	return this->p_->album;
}

unsigned int Reader::getIndex() const {
	return this->p_->index;
}

ChannelLayout Reader::getChannelLayout() const {
	return this->p_->channelLayout;
}

const QString & Reader::getYear() const {
	return this->p_->year;
}

const QByteArray & Reader::getGenre() const {
	return this->p_->genre;
}

const QByteArray & Reader::getCopyright() const {
	return this->p_->copyright;
}

const QByteArray & Reader::getComment() const {
	return this->p_->comment;
}

const AudioFormat & Reader::getAudioFormat() const {
	return this->p_->format;
}

void Reader::setAudioFormat( const AudioFormat & format ) {
	this->p_->format = format;
	this->p_->format.setCodec( "audio/pcm" );
}

Reader::Private::Private( const QUrl & uri ):
album(),
artist(),
bitRate( 0 ),
buffer(),
channelLayout( LayoutNative ),
comment(),
copyright(),
format(),
msDuration( 0 ),
uri( uri ),
genre(),
index( 0 ),
title(),
year() {
}
