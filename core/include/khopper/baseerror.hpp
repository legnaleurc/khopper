/**
 * @file baseerror.hpp
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
#ifndef KHOPPER_CORE_BASEERROR_HPP
#define KHOPPER_CORE_BASEERROR_HPP

#include <exception>
#include <string>

#include <QtCore/QString>

#include "config.hpp"

namespace khopper {
/// Error classes
namespace error {

/**
 * @brief Base class of error types
 *
 * This class is the common code of error types,
 * to avoid duplicate code generation.
 */
class KHOPPER_DLL BaseError: public std::exception {
public:
	/**
	 * @brief Construct with an error code
	 */
	BaseError( int code, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	BaseError( const char * msg, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	BaseError( const std::string & msg, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	BaseError( const wchar_t * msg, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	BaseError( const std::wstring & msg, const char * file, long line );
	/**
	 * @brief Construct with an error message
	 */
	BaseError( const QString & msg, const char * file, long line );

	/// Get error message
	virtual const char * what() const throw();

	const QString & getFile() const;
	long getLine() const;
	/**
	 * @brief Get the error message
	 */
	const QString & getMessage() const;
private:
	class Private;
	std::shared_ptr< Private > p_;
};

}
}

#endif
