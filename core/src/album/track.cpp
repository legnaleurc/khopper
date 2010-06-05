/**
 * @file track.cpp
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
#include "trackprivate.hpp"
#include "error.hpp"

#include <QHash>

using namespace khopper::album;
using khopper::codec::ReaderSP;
using khopper::error::RunTimeError;
using khopper::error::CodecError;

Track::Track( ReaderSP reader ):
p_( new TrackPrivate ) {
	if( !reader ) {
		throw RunTimeError( "Invalid reader" );
	}

	reader->open( QIODevice::ReadOnly );
	if( !reader->isOpen() ) {
		throw CodecError( QObject::tr( "Can not open file!" ) );
	}

	this->setAlbum( AlbumSP( new Album ) );
	// FIXME: text codec
	this->getAlbum()->setTitle( reader->getAlbum() );
	this->setArtist( reader->getArtist() );
	this->setBitRate( reader->getBitRate() );
	this->setChannels( reader->getChannels() );
	this->setDuration( Timestamp::fromMillisecond( reader->getDuration() ) );
	this->setIndex( reader->getIndex() );
	this->setSampleRate( reader->getSampleRate() );
	this->setTitle( reader->getTitle() );
	this->setURI( reader->getURI() );

	reader->close();

	this->p_->reader = reader;
}

Track::~Track() {
}

AlbumSP Track::getAlbum() const {
	return this->p_->album;
}

QString Track::getArtist() const {
	return this->p_->textCodec->toUnicode( this->p_->artist );
}

unsigned int Track::getBitRate() const {
	return this->p_->bitRate;
}

unsigned int Track::getChannels() const {
	return this->p_->channels;
}

const Timestamp & Track::getDuration() const {
	return this->p_->duration;
}

unsigned int Track::getIndex() const {
	return this->p_->index;
}

ReaderSP Track::getReader() const {
	return this->p_->reader;
}

unsigned int Track::getSampleRate() const {
	return this->p_->sampleRate;
}

const Timestamp & Track::getStartTime() const {
	return this->p_->startTime;
}

QString Track::getTitle() const {
	return this->p_->textCodec->toUnicode( this->p_->title );
}

const QUrl & Track::getURI() const {
	return this->p_->uri;
}

void Track::setAlbum( AlbumSP album ) {
	this->p_->album = album;
}

void Track::setArtist( const QByteArray & artist ) {
	this->p_->artist = artist;
}

void Track::setArtist( const QString & artist ) {
	this->p_->artist = this->p_->textCodec->fromUnicode( artist );
}

void Track::setBitRate( unsigned int bitRate ) {
	this->p_->bitRate = bitRate;
}

void Track::setChannels( unsigned int channels ) {
	this->p_->channels = channels;
}

void Track::setDuration( const Timestamp & duration ) {
	this->p_->duration = duration;
}

void Track::setIndex( unsigned int index ) {
	this->p_->index = index;
}

void Track::setSampleRate( unsigned int sampleRate ) {
	this->p_->sampleRate = sampleRate;
}

void Track::setSongWriter( const QByteArray & songWriter ) {
	this->p_->songWriter = songWriter;
}

void Track::setSongWriter( const QString & songWriter ) {
	this->p_->songWriter = this->p_->textCodec->fromUnicode( songWriter );
}

void Track::setStartTime( const Timestamp & startTime ) {
	this->p_->startTime = startTime;
}

void Track::setTextCodec( QTextCodec * textCodec ) {
	if( textCodec ) {
		this->p_->textCodec = textCodec;
	}
}

void Track::setTitle( const QByteArray & title ) {
	this->p_->title = title;
}

void Track::setTitle( const QString & title ) {
	this->p_->title = this->p_->textCodec->fromUnicode( title );
}

void Track::setURI( const QUrl & uri ) {
	this->p_->uri = uri;
}

Track::TrackPrivate::TrackPrivate():
album(),
artist(),
bitRate( 0 ),
channels( 0 ),
duration(),
reader(),
sampleRate( 0 ),
songWriter(),
title(),
textCodec( QTextCodec::codecForName( "UTF-8" ) ),
uri() {
}

uint qHash( TrackCSP key ) {
	return qHash( key.get() );
}
