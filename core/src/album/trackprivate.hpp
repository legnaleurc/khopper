/**
 * @file trackprivate.hpp
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
#ifndef KHOPPER_ALBUM_TRACKPRIVATE_HPP
#define KHOPPER_ALBUM_TRACKPRIVATE_HPP

#include "track.hpp"

#include <QtCore/QTextCodec>
#include <QtCore/QUrl>

namespace khopper {
	namespace album {

		class Track::Private {
		public:
			Private( const QUrl & uri, plugin::ReaderCreator creator );

			AlbumSP album;
			QByteArray artist;
			unsigned int bitRate;
			plugin::ReaderCreator creator;
			Timestamp duration;
			codec::AudioFormat format;
			unsigned int index;
			QByteArray songWriter;
			QByteArray title;
			QTextCodec * textCodec;
			QUrl uri;
		};

	}
}

#endif
