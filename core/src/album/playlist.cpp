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
#include "playlistprivate.hpp"

#include <algorithm>

namespace {

	struct Helper {
		Helper( const QUrl & uri ) : uri( uri ) {
		}
		bool operator ()( const khopper::plugin::PlayListFactoryPrivate::Pair & l, const khopper::plugin::PlayListFactoryPrivate::Pair & r ) {
			return l.first( this->uri ) < r.first( this->uri );
		}
		const QUrl & uri;
	};

}

using namespace khopper::album;
using namespace khopper::plugin;

PlayList khopper::plugin::createPlayList( const QUrl & uri ) {
	const std::list< PlayListFactoryPrivate::Pair > & fList( PlayListFactory::Instance().fList );

	if( !fList.empty() ) {
		return std::max_element( fList.begin(), fList.end(), Helper( uri ) )->second( uri );
	} else {
		throw khopper::error::SystemError( QObject::tr( "No playlist can be created." ) );
	}
}

void khopper::plugin::registerPlayList( PlayListVerifier v, PlayListCreator c ) {
	PlayListFactory::Instance().fList.push_back( std::make_pair( v, c ) );
}

void khopper::plugin::unregisterPlayList( PlayListVerifier v ) {
	std::list< PlayListFactoryPrivate::Pair > & fList( PlayListFactory::Instance().fList );
	for( std::list< PlayListFactoryPrivate::Pair >::iterator it = fList.begin(); it != fList.end(); ++it ) {
		if( it->first == v ) {
			fList.erase( it );
			break;
		}
	}
}
