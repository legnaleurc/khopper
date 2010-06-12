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
#include "abstractreaderprivate.hpp"

#ifndef LOKI_CLASS_LEVEL_THREADING
# define LOKI_CLASS_LEVEL_THREADING
#endif

#include <loki/Factory.h>
#include <loki/Singleton.h>

#include <QtDebug>

#include <cassert>
#include <cmath>
#include <cstring>

namespace {

	struct Helper {
		Helper( const QUrl & uri ) : uri( uri ) {
		}
		bool operator ()( const khopper::plugin::ReaderFactoryPrivate::Pair & l, const khopper::plugin::ReaderFactoryPrivate::Pair & r ) {
			return l.first( this->uri ) < r.first( this->uri );
		}
		const QUrl & uri;
	};

}

using namespace khopper::codec;
using namespace khopper::plugin;

bool khopper::plugin::registerReader( ReaderVerifier v, ReaderCreator c ) {
	ReaderFactory::Instance().l.push_back( std::make_pair( v, c ) );
	return true;
}

void khopper::plugin::unregisterReader( ReaderVerifier v ) {
	std::list< ReaderFactoryPrivate::Pair > & l( ReaderFactory::Instance().l );
	for( std::list< ReaderFactoryPrivate::Pair >::iterator it = l.begin(); it != l.end(); ++it ) {
		if( it->first == v ) {
			l.erase( it );
			break;
		}
	}
}

ReaderSP khopper::plugin::createReader( const QUrl & uri ) {
	const std::list< ReaderFactoryPrivate::Pair > & l( ReaderFactory::Instance().l );

	if( !l.empty() ) {
		return std::max_element( l.begin(), l.end(), Helper( uri ) )->second( uri );
	} else {
		throw khopper::error::SystemError( QObject::tr( "No reader can be used." ) );
	}
}

AbstractReader::AbstractReader( const QUrl & uri ):
QIODevice(),
p_( new AbstractReaderPrivate( uri ) ) {
}

AbstractReader::~AbstractReader() {
}

bool AbstractReader::open( OpenMode mode ) {
	if( this->isOpen() ) {
		this->close();
	}

	bool opened = this->QIODevice::open( mode & ~( WriteOnly & Text ) );
	this->doOpen();

	return opened;
}

void AbstractReader::close() {
	this->QIODevice::close();

	this->p_.reset( new AbstractReaderPrivate( this->p_->uri ) );

	try {
		this->doClose();
	} catch( ... ) {
		// TODO: log an error
		assert( !"a plugin can not clean up its own mess ..." );
	}
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

const QAudioFormat & AbstractReader::getAudioFormat() const {
	return this->p_->format;
}

void AbstractReader::setAudioFormat( const QAudioFormat & format ) {
	this->p_->format = format;
	this->p_->format.setCodec( "audio/pcm" );
}

AbstractReader::AbstractReaderPrivate::AbstractReaderPrivate( const QUrl & uri ):
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
