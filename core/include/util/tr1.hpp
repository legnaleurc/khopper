/**
 * @file tr1.hpp
 * @author Wei-Cheng Pan
 * @attention If you need me, include me FIRST!
 *
 * This File is used to provide TR1 and C99 compatibility for
 * different compilers.
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
#ifndef KHOPPER_UTIL_TR1_HPP
#define KHOPPER_UTIL_TR1_HPP

// for stdint.h in C99
#define __STDC_CONSTANT_MACROS

#ifdef __GNUC__
#include <tr1/memory>
#else
#include <memory>
#endif

#ifdef _MSC_VER
#define KHOPPER_DLL_EXPORT __declspec(dllexport)
#else
#define KHOPPER_DLL_EXPORT
#endif

#endif