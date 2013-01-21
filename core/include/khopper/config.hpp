/**
 * @file config.hpp
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
#ifndef KHOPPER_UTIL_CONFIG_HPP
#define KHOPPER_UTIL_CONFIG_HPP

// Microsoft Visual C++ settings
#ifdef _MSC_VER
// for dll symbol export/import
# ifdef KHOPPER_LIBRARY
#  define KHOPPER_DLL __declspec(dllexport)
# else
#  define KHOPPER_DLL __declspec(dllimport)
# endif
// GNU GCC settings
#elif defined( __GNUC__ )
// need not dll symbol trick
# define KHOPPER_DLL
// supports C++0x since GCC 4.5
#else
// FIXME: reserved for other compilers
#endif

#define KHOPPER_STRINGIZE(token) _KHOPPER_STRINGIZE(token)
#define _KHOPPER_STRINGIZE(token) #token

#endif
