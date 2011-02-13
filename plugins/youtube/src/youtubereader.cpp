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
#include "youtubeplugin.hpp"

using namespace khopper::codec;
using khopper::plugin::getReaderCreator;
using khopper::error::BaseError;
using khopper::plugin::YouTubeLoader;
using khopper::plugin::YouTubePlugin;

YouTubeReader::YouTubeReader( const QUrl & uri, YouTubePlugin * parent ):
AbstractReader( uri ),
client_() {
	YouTubeLoader loader( uri, parent );
	loader.parseHeader( false );
	QString title( loader.getTitle() );
	title.replace( "&lt;", "<" ).replace( "&gt;", ">" ).replace( "&nbsp;", " " ).replace( "&amp;", "&" );
	this->setTitle( title.toUtf8() );
	// Always use MP4, it makes no diff for audio.
	QUrl realURI( loader.getRealURI( "18" ) );
	this->client_ = getReaderCreator( realURI )( realURI );
}

bool YouTubeReader::atEnd() const {
	return this->client_->atEnd();
}

bool YouTubeReader::seek( qint64 pos ) {
	return this->QIODevice::seek( pos ) && this->client_->seek( pos );
}

qint64 YouTubeReader::size() const {
	return this->client_->size();
}

void YouTubeReader::doOpen() {
	this->client_->open( QIODevice::ReadOnly );
	this->setAudioFormat( this->client_->getAudioFormat() );
	this->setDuration( this->client_->getDuration() );
	this->setBitRate( this->client_->getBitRate() );
}

void YouTubeReader::doClose() {
	this->client_->close();
}

qint64 YouTubeReader::readData( char * data, qint64 maxSize ) {
	return this->client_->read( data, maxSize );
}
