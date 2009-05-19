/**
 * @file tr1.hpp
 * @author Wei-Cheng Pan
 * @attention If you need me, include me FIRST!
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
#ifndef KHOPPER_UTIL_TR1_HPP
#define KHOPPER_UTIL_TR1_HPP

// for stdint.h in C99
#define __STDC_CONSTANT_MACROS

#ifdef _MSC_VER
# define KHOPPER_EXPORT __declspec(dllexport)
# if _MSC_FULL_VER >= 150030729
#  include <memory>
# else
// reserved for older msvc
# endif
# elif defined( __GNUC__ )
# define KHOPPER_EXPORT
# if __GNUC__ >= 4
# include <tr1/memory>
# else
// reserved for older gcc
# endif
#else
// reserved for other compilers
#endif

#endif
