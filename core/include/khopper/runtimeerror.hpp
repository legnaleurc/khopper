/**
 * @file runtimeerror.hpp
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
#ifndef KHOPPER_CORE_RUNTIMEERROR_HPP
#define KHOPPER_CORE_RUNTIMEERROR_HPP

#include "baseerror.hpp"

namespace khopper {
namespace error {

/**
 * @brief Run-time error
 */
class KHOPPER_DLL RunTimeError: public BaseError {
public:
	/**
	 * @brief Construct with an error code
	 */
	RunTimeError( int code, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	RunTimeError( const char * msg, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	RunTimeError( const std::string & msg, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	RunTimeError( const wchar_t * msg, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	RunTimeError( const std::wstring & msg, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	RunTimeError( const QString & msg, const char * file, long line );
};

}
}

#endif
