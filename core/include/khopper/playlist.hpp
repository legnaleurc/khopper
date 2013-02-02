/**
 * @file playlist.hpp
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
#ifndef KHOPPER_ALBUM_PLAYLIST_HPP
#define KHOPPER_ALBUM_PLAYLIST_HPP

#include <functional>

#include <QtCore/QList>

#include "track.hpp"

namespace khopper {
namespace album {

class KHOPPER_DLL PlayList {
	typedef QList< TrackSP > TrackList;

public:
	typedef TrackList::value_type value_type;
	typedef TrackList::const_iterator const_iterator;
	typedef TrackList::iterator iterator;
	typedef TrackList::const_reference const_reference;
	typedef TrackList::reference reference;
	typedef TrackList::difference_type difference_type;
	typedef TrackList::size_type size_type;

	typedef std::function< unsigned int ( const QUrl & ) > Verifier;
	typedef std::function< PlayList ( const QUrl & ) > Creator;

	static bool install( const QString & id, Verifier v, Creator c );
	static bool uninstall( const QString & id );

	static PlayList fromURI( const QUrl & uri );

	PlayList();
	PlayList( const PlayList & that );
	PlayList & operator =( const PlayList & that );
	PlayList( PlayList && that );
	PlayList & operator =( PlayList && that );

	void append( const PlayList & that );
	const_iterator begin() const;
	iterator begin();
	const_iterator end() const;
	iterator end();
	iterator erase( iterator pos );
	bool empty() const;
	reference front();
	const_reference front() const;
	void push_back( const_reference track );
	size_type size() const;
	reference operator []( size_type index );
	const_reference operator []( size_type index ) const;

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}
}

#endif
