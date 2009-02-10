/**
 * @file error.hpp
 * @author Wei-Cheng Pan
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
#ifndef KHOPPER_ERROR_HPP
#define KHOPPER_ERROR_HPP

#include <exception>
#include <string>

namespace Khopper {

	/**
	 * @brief Base class of error types
	 *
	 * This class is the common code of error types,
	 * to avoid duplicate code generation.
	 */
	class ErrorBase : public std::exception {
	public:
		/**
		 * @brief Construct with an error message
		 * @param [in] msg Error message
		 */
		ErrorBase( const std::string & msg ) throw();
		/**
		 * @brief Construct with an error message
		 * @param [in] msg Error message
		 */
		ErrorBase( const std::wstring & msg ) throw();
		/**
		 * @brief Virtual destructor
		 */
		virtual ~ErrorBase() throw();

		/**
		 * @brief Get the error message
		 * @return Error message
		 */
		virtual const char * what() throw();
	private:
		std::string msg_;
	};

	/**
	 * @brief Error class
	 * @tparam Type Custom error extension
	 */
	template< typename Type >
	class Error : public ErrorBase, public Type {
	public:
		Error( const std::string & msg ) throw() : ErrorBase( msg ) {}
		Error( const std::wstring & msg ) throw() : ErrorBase( msg ) {}
	};
	
	/**
	 * @brief Dummy error class
	 */
	class OS {
	};
	
	/**
	 * @brief Dummy error class
	 */
	class RunTime {
	};

	/**
	 * @brief Dummy error class
	 */
	class System {
	};

	/**
	 * @brief Dummy error class
	 */
	class IO {
	};

}

#endif
