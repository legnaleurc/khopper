/**
 * @file ioerror.hpp
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
#ifndef KHOPPER_CORE_IOERROR_HPP
#define KHOPPER_CORE_IOERROR_HPP

#include "baseerror.hpp"

namespace khopper {
namespace error {

/**
 * @brief I/O error
 */
class KHOPPER_DLL IOError: public BaseError {
public:
	/**
	 * @brief Construct with an error code
	 */
	IOError( int code, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	IOError( const char * msg, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	IOError( const std::string & msg, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	IOError( const wchar_t * msg, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	IOError( const std::wstring & msg, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	IOError( const QString & msg, const char * file, long line );
	virtual ~IOError() throw();
};

}
}

#endif
