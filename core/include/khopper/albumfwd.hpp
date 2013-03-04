/**
 * @file albumfwd.hpp
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
#ifndef KHOPPER_ALBUM_ALBUMFWD_HPP
#define KHOPPER_ALBUM_ALBUMFWD_HPP

#include <memory>

#include <QtCore/QtGlobal>

#include "config.hpp"

namespace khopper {
namespace album {

class Album;
typedef std::shared_ptr< Album > AlbumSP;
typedef std::shared_ptr< const Album > AlbumCSP;
typedef std::weak_ptr< Album > AlbumWP;
typedef std::weak_ptr< const Album > AlbumCWP;

uint KHOPPER_DLL qHash( AlbumCSP key );

class Track;
/**
 * @brief Smart pointer of Track.
 */
typedef std::shared_ptr< Track > TrackSP;
/**
 * @brief Smart pointer of const Track.
 */
typedef std::shared_ptr< const Track > TrackCSP;
typedef std::weak_ptr< Track > TrackWP;
typedef std::weak_ptr< const Track > TrackCWP;

}
}

#endif
