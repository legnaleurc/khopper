/**
 * @file wfile.cpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _WIN32
#error This file must compile on Windows only!
#endif

#include "wfile.hpp"

#include <QtCore/QIODevice>

int khopper::libav::read_packet( void * opaque, uint8_t * buf, int buf_size ) {
	QIODevice * device = static_cast< QIODevice * >( opaque );
	return device->read( static_cast< char * >( static_cast< void * >( buf ) ), buf_size );
}

int khopper::libav::write_packet( void * opaque, uint8_t * buf, int buf_size ) {
	QIODevice * device = static_cast< QIODevice * >( opaque );
	return device->write( static_cast< char * >( static_cast< void * >( buf ) ), buf_size );
}

int64_t khopper::libav::seek( void * opaque, int64_t offset, int whence ) {
	QIODevice * device = static_cast< QIODevice * >( opaque );
	switch( whence ) {
	case SEEK_CUR:
		offset += device->pos();
		break;
	case SEEK_END:
		offset = device->size();
	default:
		;
	}
	device->seek( offset );
	return device->pos();
}
