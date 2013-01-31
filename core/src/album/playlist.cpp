/**
 * @file playlist.cpp
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
#include "playlist.hpp"
#include "runtimeerror.hpp"

#include <QtCore/QSet>

#include "album.hpp"
#include "core/applicationprivate.hpp"
#include "runtimeerror.hpp"

using khopper::album::PlayList;
using khopper::album::AlbumSP;
using khopper::album::AlbumWP;
using khopper::album::TrackSP;
using khopper::error::RunTimeError;
using namespace khopper::plugin;

class PlayList::Private {
public:
	void addAlbum( AlbumWP album );
	void removeAlbum( AlbumWP album );

	std::map< AlbumSP, unsigned long > albums;
	TrackList tracks;
};

void PlayList::Private::addAlbum( AlbumWP album ) {
	auto tmp = album.lock();
	if( !tmp ) {
		return;
	}
	auto it = this->albums.find( tmp );
	if( it != this->albums.end() ) {
		++it->second;
		return;
	}
	this->albums.insert( std::make_pair( tmp, 1UL ) );
}

void PlayList::Private::removeAlbum( AlbumWP album ) {
	auto tmp = album.lock();
	if( !tmp ) {
		return;
	}
	auto it = this->albums.find( tmp );
	if( it == this->albums.end() ) {
		return;
	}
	if( it->second > 1UL ) {
		--it->second;
	} else {
		this->albums.erase( it );
	}
}

PlayList PlayList::fromURI( const QUrl & uri ) {
	auto creator = ApplicationPrivate::self->playlistFactory.getCreator( uri );
	if( !creator ) {
		throw RunTimeError( QObject::tr( "this playlist is not supported" ), __FILE__, __LINE__ );
	}
	return creator( uri );
}

PlayList::PlayList(): p_( new Private ) {
}

PlayList::PlayList( PlayList && that ): p_( std::move( that.p_ ) ) {
}

void PlayList::append( const PlayList & that ) {
	this->p_->tracks.append( that.p_->tracks );
	for( auto it = that.p_->tracks.begin(); it != that.p_->tracks.end(); ++it ) {
		this->p_->addAlbum( ( *it )->getAlbum() );
	}
}

PlayList::const_iterator PlayList::begin() const {
	return this->p_->tracks.begin();
}

PlayList::iterator PlayList::begin() {
	return this->p_->tracks.begin();
}

bool PlayList::empty() const {
	return this->p_->tracks.empty();
}

PlayList::const_iterator PlayList::end() const {
	return this->p_->tracks.end();
}

PlayList::iterator PlayList::end() {
	return this->p_->tracks.end();
}

PlayList::iterator PlayList::erase( iterator pos ) {
	auto track = *pos;
	auto it = this->p_->tracks.erase( pos );
	this->p_->removeAlbum( track->getAlbum() );
	return it;
}

PlayList::const_reference PlayList::front() const {
	return this->p_->tracks.front();
}

PlayList::reference PlayList::front() {
	return this->p_->tracks.front();
}

void PlayList::push_back( const_reference track ) {
	this->p_->tracks.push_back( track );
	this->p_->addAlbum( track->getAlbum() );
}

PlayList::size_type PlayList::size() const {
	return this->p_->tracks.size();
}

PlayList::const_reference PlayList::operator []( size_type index ) const {
	return this->p_->tracks[index];
}

PlayList::reference PlayList::operator []( size_type index ) {
	return this->p_->tracks[index];
}

bool khopper::plugin::registerPlayList( const QString & id, PlayListVerifier v, PlayListCreator c ) {
	return ApplicationPrivate::self->playlistFactory.registerProduct( id, v, c );
}

bool khopper::plugin::unregisterPlayList( const QString & id ) {
	return ApplicationPrivate::self->playlistFactory.unregisterProduct( id );
}
