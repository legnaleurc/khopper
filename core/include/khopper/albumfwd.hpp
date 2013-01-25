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
