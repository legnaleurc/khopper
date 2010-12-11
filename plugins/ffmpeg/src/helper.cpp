/**
 * @file helper.cpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper FFmpeg Plug-in.
 *
 * This plug-in is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This plug-in is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include "helper.hpp"

QByteArray khopper::ffmpeg::fromURI( const QUrl & uri ) {
		QByteArray tmp( uri.toString().toUtf8() );
#ifdef Q_OS_WIN32
		if( uri.scheme() == "file" ) {
			tmp.prepend( 'w' );
		}
#endif
		return tmp;
}
