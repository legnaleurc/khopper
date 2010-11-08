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
// supports C++0x since Visual C++ 10
# if _MSC_VER >= 1600
# ifndef __STDC_CONSTANT_MACROS
#  define __STDC_CONSTANT_MACROS
# endif
# else
#  error Must use Microsoft Visual C++ 10 or newer toolchain.
# endif
// GNU GCC settings
#elif defined( __GNUC__ )
// need not dll symbol trick
# define KHOPPER_DLL
// supports C++0x since GCC 4.5
# if __GNUC__ < 4 || __GNUC_MINOR__ < 5
#  error Must use GNU GCC 4.5 or newer toolchain.
# endif
#else
// FIXME: reserved for other compilers
#endif

#endif
