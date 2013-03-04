/**
 * @file wfile.hpp
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
#ifndef KHOPPER_LIBAV_WFILE_HPP
#define KHOPPER_LIBAV_WFILE_HPP

#include <cstdint>

namespace khopper {
namespace libav {

int read_packet( void * opaque, uint8_t * buf, int buf_size );
int write_packet( void * opaque, uint8_t * buf, int buf_size );
int64_t seek( void * opaque, int64_t offset, int whence );

}
}

#endif
