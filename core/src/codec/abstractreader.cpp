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
#include "defaultreader.hpp"

#ifndef LOKI_CLASS_LEVEL_THREADING
# define LOKI_CLASS_LEVEL_THREADING
#endif

#include <loki/Factory.h>
#include <loki/Singleton.h>

#include <QtDebug>

#include <cassert>

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

	return std::max_element( l.begin(), l.end(), Helper( uri ) )->second( uri );
}

AbstractReader::AbstractReader( const QUrl & uri ):
QIODevice(),
p_( new AbstractReaderPrivate( uri ) ) {
}

AbstractReader::~AbstractReader() {
}

bool AbstractReader::open( OpenMode /*mode*/ ) {
	if( this->isOpen() ) {
		this->close();
	}

	bool opened = this->QIODevice::open( ReadOnly );
	this->doOpen();

	this->p_->hasNext = true;

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

	this->p_->hasNext = false;
}

qint64 AbstractReader::readData( char * data, qint64 maxSize ) {
	//if( !this->p_->hasNext ) {
	//	return 0;
	//}

	//bool stop = false;
	QByteArray frame( this->readFrame() );
	while( /*!stop &&*/ frame.isEmpty() ) {
		frame = this->readFrame();
	}
	//if( stop ) {
	//	this->p_->hasNext = false;
	//}

	this->p_->buffer << frame;
	return this->p_->buffer.readRawData( data, maxSize );
}

qint64 AbstractReader::writeData( const char * /*data*/, qint64 /*maxSize*/ ) {
	return -1;
}

bool AbstractReader::seek( int64_t msPos ) {
	bool succeed = this->QIODevice::seek( msPos );
	succeed &= this->seekFrame( msPos );
	//if( succeed ) {
	//	this->p_->hasNext = true;
	//}
	return succeed;
}

qint64 AbstractReader::size() const {
	return this->isSequential() ? this->bytesAvailable() : this->getDuration();
}

//bool AbstractReader::afterBegin( int64_t ms ) const {
//	if( this->p_->msBegin < 0 ) {
//		return true;
//	}
//	return ms >= this->p_->msBegin;
//}
//
//bool AbstractReader::afterEnd( int64_t ms ) const {
//	if( this->p_->msEnd < 0 ) {
//		return false;
//	}
//	return ms >= this->p_->msEnd;
//}

const QUrl & AbstractReader::getURI() const {
	return this->p_->uri;
}

void AbstractReader::setDuration( qint64 msDuration ) {
	this->p_->msDuration = msDuration;
}

void AbstractReader::setSampleFormat( SampleFormat sampleFormat ) {
	this->p_->sampleFormat = sampleFormat;
}

void AbstractReader::setChannelLayout( ChannelLayout channelLayout ) {
	this->p_->channelLayout = channelLayout;
}

unsigned int AbstractReader::getChannels() const {
	return this->p_->channels;
}

void AbstractReader::setChannels( unsigned int channels ) {
	this->p_->channels = channels;
}

void AbstractReader::setSampleRate( unsigned int sampleRate ) {
	this->p_->sampleRate = sampleRate;
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

void AbstractReader::setAlbum( const QByteArray & album ) {
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

unsigned int AbstractReader::getSampleRate() const {
	return this->p_->sampleRate;
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

const QByteArray & AbstractReader::getAlbum() const {
	return this->p_->album;
}

unsigned int AbstractReader::getIndex() const {
	return this->p_->index;
}

ChannelLayout AbstractReader::getChannelLayout() const {
	return this->p_->channelLayout;
}

SampleFormat AbstractReader::getSampleFormat() const {
	return this->p_->sampleFormat;
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

AbstractReader::AbstractReaderPrivate::AbstractReaderPrivate( const QUrl & uri ):
album(),
artist(),
bitRate( 0 ),
buffer(),
channelLayout( LayoutNative ),
channels( 0 ),
comment(),
copyright(),
msBegin( 0 ),
msDuration( 0 ),
msEnd( 0 ),
uri( uri ),
genre(),
hasNext( false ),
index( 0 ),
sampleFormat( SF_NONE ),
sampleRate( 0 ),
title(),
year() {
}
