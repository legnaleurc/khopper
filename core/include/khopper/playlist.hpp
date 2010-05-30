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

#include "track.hpp"

#ifndef LOKI_CLASS_LEVEL_THREADING
# define LOKI_CLASS_LEVEL_THREADING
#endif
#include <loki/Functor.h>

#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtCore/QSet>

namespace khopper {
	namespace album {

		class KHOPPER_DLL PlayList : public QList< TrackSP > {
		public:
			typedef Loki::Functor<
				unsigned int,
				LOKI_TYPELIST_1( const QUrl & ),
				Loki::ClassLevelLockable
			> Verifier;
			typedef Loki::Functor<
				PlayList,
				LOKI_TYPELIST_1( const QUrl & ),
				Loki::ClassLevelLockable
			> Creator;

			static PlayList loadFromUri( const QUrl & uri );
			static void registerPlayList( Verifier v, Creator c );
			static void unregisterPlayList( Verifier v );
		};

	}
}

#endif
