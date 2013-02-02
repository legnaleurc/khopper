/**
 * @file abstractreader.cpp
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
#include "abstractreader.hpp"

#include <cassert>
#include <cmath>
#include <cstring>

#include "core/factory.hpp"
#include "runtimeerror.hpp"


namespace khopper {
namespace codec {

class AbstractReader::Private {
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
	int64_t msDuration;
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

using khopper::codec::AbstractReader;
using khopper::codec::AudioFormat;
using khopper::codec::ChannelLayout;
using khopper::error::BaseError;
using khopper::error::RunTimeError;

bool khopper::codec::AbstractReader::install( const QString & id, Verifier v, Creator c ) {
	return factory.installCreator( id, v, c );
}

bool khopper::codec::AbstractReader::uninstall( const QString & id ) {
	return factory.uninstallCreator( id );
}

AbstractReader::Creator khopper::codec::AbstractReader::getCreator( const QUrl & uri ) {
	auto creator = factory.getCreator( uri );
	if( !creator ) {
		throw RunTimeError( QObject::tr( "find no suitable codec" ), __FILE__, __LINE__ );
	}
	return creator;
}

AbstractReader::AbstractReader( const QUrl & uri ):
QIODevice(),
p_( new Private( uri ) ) {
}

AbstractReader::~AbstractReader() {
}

bool AbstractReader::open( OpenMode mode ) {
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

void AbstractReader::close() {
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

qint64 AbstractReader::writeData( const char * /*data*/, qint64 /*maxSize*/ ) {
	return -1;
}

const QUrl & AbstractReader::getURI() const {
	return this->p_->uri;
}

void AbstractReader::setDuration( qint64 msDuration ) {
	this->p_->msDuration = msDuration;
}

void AbstractReader::setChannelLayout( ChannelLayout channelLayout ) {
	this->p_->channelLayout = channelLayout;
}

void AbstractReader::setBitRate( unsigned int bitRate ) {
	this->p_->bitRate = bitRate;
}

void AbstractReader::setGenre( const QByteArray & genre ) {
	this->p_->genre = genre;
}

void AbstractReader::setIndex( unsigned int index ) {
	this->p_->index = index;
}

void AbstractReader::setYear( const QString & year ) {
	this->p_->year = year;
}

void AbstractReader::setAlbumTitle( const QByteArray & album ) {
	this->p_->album = album;
}

void AbstractReader::setComment( const QByteArray & comment ) {
	this->p_->comment = comment;
}

void AbstractReader::setCopyright( const QByteArray & copyright ) {
	this->p_->copyright = copyright;
}

void AbstractReader::setArtist( const QByteArray & artist ) {
	this->p_->artist = artist;
}

void AbstractReader::setTitle( const QByteArray & title ) {
	this->p_->title = title;
}

qint64 AbstractReader::getDuration() const {
	return this->p_->msDuration;
}

const QByteArray & AbstractReader::getTitle() const {
	return this->p_->title;
}

unsigned int AbstractReader::getBitRate() const {
	return this->p_->bitRate;
}

const QByteArray & AbstractReader::getArtist() const {
	return this->p_->artist;
}

const QByteArray & AbstractReader::getAlbumTitle() const {
	return this->p_->album;
}

unsigned int AbstractReader::getIndex() const {
	return this->p_->index;
}

ChannelLayout AbstractReader::getChannelLayout() const {
	return this->p_->channelLayout;
}

const QString & AbstractReader::getYear() const {
	return this->p_->year;
}

const QByteArray & AbstractReader::getGenre() const {
	return this->p_->genre;
}

const QByteArray & AbstractReader::getCopyright() const {
	return this->p_->copyright;
}

const QByteArray & AbstractReader::getComment() const {
	return this->p_->comment;
}

const AudioFormat & AbstractReader::getAudioFormat() const {
	return this->p_->format;
}

void AbstractReader::setAudioFormat( const AudioFormat & format ) {
	this->p_->format = format;
	this->p_->format.setCodec( "audio/pcm" );
}

AbstractReader::Private::Private( const QUrl & uri ):
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
