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
#include "albumprivate.hpp"

using namespace khopper::album;

Album::Album() : p_( new AlbumPrivate ) {
}

Album::~Album() {
}

const QString & Album::getTitle() const {
	return this->p_->title;
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
