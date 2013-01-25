/**
 * @file album.cpp
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
#include "album_p.hpp"
#include "track.hpp"

using khopper::album::Album;
using khopper::album::AlbumCSP;
using khopper::album::AlbumSP;
using khopper::album::TrackSP;

AlbumSP Album::create() {
	return AlbumSP( new Album );
}

Album::Album() : p_( new Private ) {
}

Album::~Album() {
}

const QString & Album::getArtist() const {
	return this->p_->artist;
}

const QString & Album::getSongWriter() const {
	return this->p_->songWriter;
}

const QString & Album::getTitle() const {
	return this->p_->title;
}

const QList< TrackSP > & Album::getTracks() const {
	return this->p_->tracks;
}

void Album::addTrack( TrackSP track ) {
	this->p_->tracks.push_back( track );
	track->setAlbum( this->shared_from_this() );
}

void Album::setArtist( const QString & artist ) {
	this->p_->artist = artist;
}

void Album::setSongWriter( const QString & songWriter ) {
	this->p_->songWriter = songWriter;
}

void Album::setTitle( const QString & title ) {
	this->p_->title = title;
}

uint khopper::album::qHash( AlbumCSP key ) {
	return ::qHash( key.get() );
}
