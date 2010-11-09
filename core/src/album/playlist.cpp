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
#include "playlistfactory.hpp"

#include <algorithm>

using namespace khopper::album;
using namespace khopper::plugin;

PlayList khopper::plugin::createPlayList( const QUrl & uri ) {
	const PlayListFactoryPrivate::TableType & m( PlayListFactory::Instance().m );

	if( !m.empty() ) {
		typedef PlayListFactoryPrivate::TableType::value_type Pair;
		return std::max_element( m.begin(), m.end(), [&uri]( const Pair & l, const Pair & r ) {
			return l.second.first( uri ) < r.second.first( uri );
		} )->second.second( uri );
	} else {
		throw khopper::error::SystemError( QObject::tr( "No playlist can be created." ) );
	}
}

bool khopper::plugin::registerPlayList( const QString & id, PlayListVerifier v, PlayListCreator c ) {
	return PlayListFactory::Instance().m.insert( std::make_pair( id, std::make_pair( v, c ) ) ).second;
}

bool khopper::plugin::unregisterPlayList( const QString & id ) {
	PlayListFactoryPrivate::TableType & m( PlayListFactory::Instance().m );
	return m.erase( id ) == 1;
}
