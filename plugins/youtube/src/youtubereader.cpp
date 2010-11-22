/**
 * @file youtubereader.cpp
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
#include "youtubereader.hpp"
#include "youtubeloader.hpp"

#include <QtCore/QEventLoop>

#include <cstring>

using namespace khopper::codec;
using khopper::error::BaseError;
using khopper::plugin::YouTubeLoader;
using khopper::plugin::VideoParameter;

YouTubeReader::YouTubeReader( const QUrl & uri ):
AbstractReader( uri ),
buffer_(),
link_( NULL ),
linker_( new QNetworkAccessManager( this ) ),
lock_() {
}

void YouTubeReader::onError_( QNetworkReply::NetworkError /*code*/ ) {
}

void YouTubeReader::read_() {
	this->lock_.lock();
	this->buffer_.append( this->link_->readAll() );
	this->lock_.unlock();
}

void YouTubeReader::finish_() {
	this->link_->deleteLater();
	QUrl uri( this->link_->attribute( QNetworkRequest::RedirectionTargetAttribute ).toUrl() );
	if( !uri.isEmpty() && uri != this->link_->url() ) {
		this->link_ = this->linker_->get( QNetworkRequest( uri ) );
		this->connect( this->link_, SIGNAL( error( QNetworkReply::NetworkError ) ), SLOT( onError_( QNetworkReply::NetworkError ) ) );
		this->connect( this->link_, SIGNAL( readyRead() ), SLOT( read_() ) );
		this->connect( this->link_, SIGNAL( finished() ), SLOT( finish_() ) );
	}
	emit this->finished();
}

bool YouTubeReader::isSequential() const {
	return true;
}

void YouTubeReader::doOpen() {
	this->buffer_.clear();
	this->link_ = this->linker_->get( QNetworkRequest( this->getURI() ) );
	this->connect( this->link_, SIGNAL( error( QNetworkReply::NetworkError ) ), SLOT( onError_( QNetworkReply::NetworkError ) ) );
	this->connect( this->link_, SIGNAL( readyRead() ), SLOT( read_() ) );
	this->connect( this->link_, SIGNAL( finished() ), SLOT( finish_() ) );
}

void YouTubeReader::doClose() {
	this->buffer_.clear();
}

qint64 YouTubeReader::readData( char * data, qint64 maxSize ) {
	this->lock_.lock();
	maxSize = qMin( static_cast< qint64 >( this->buffer_.size() ), maxSize );
	std::memcpy( data, this->buffer_, maxSize );
	this->buffer_.remove( 0, maxSize );
	this->lock_.unlock();
	return maxSize;
}
