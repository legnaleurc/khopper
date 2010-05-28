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
		bool operator ()( const khopper::album::PlayListFactoryPrivate::Pair & l, const khopper::album::PlayListFactoryPrivate::Pair & r ) {
			return l.first( this->uri ) < r.first( this->uri );
		}
		const QUrl & uri;
	};

}

using namespace khopper::album;

PlayList PlayList::loadFromUri( const QUrl & uri ) {
	const std::list< PlayListFactoryPrivate::Pair > & fList( PlayListFactory::Instance().fList );

	return std::max_element( fList.begin(), fList.end(), Helper( uri ) )->second( uri );
}

void PlayList::registerPlayList( Verifier v, Creator c ) {
	PlayListFactory::Instance().fList.push_back( std::make_pair( v, c ) );
}
