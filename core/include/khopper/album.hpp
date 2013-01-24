/**
 * @file album.hpp
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
#ifndef KHOPPER_ALBUM_ALBUM_HPP
#define KHOPPER_ALBUM_ALBUM_HPP

#include "config.hpp"
#include "albumfwd.hpp"

#include <QtCore/QString>

#include <memory>

namespace khopper {
namespace album {

class KHOPPER_DLL Album {
public:
	Album();
	virtual ~Album();

	const QString & getArtist() const;
	const QString & getTitle() const;

	void setArtist( const QString & artist );
	void setSongWriter( const QString & songWriter );
	void setTitle( const QString & title );

private:
	struct AlbumPrivate;
	std::shared_ptr< AlbumPrivate > p_;
};

}
}

#endif
