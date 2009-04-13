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

namespace khopper {

	namespace error {

		/**
		 * @brief Base class of error types
		 *
		 * This class is the common code of error types,
		 * to avoid duplicate code generation.
		 *
		 * Message will encode to UTF-8.
		 */
		class ErrorBase : public std::exception {
		public:
			/**
			 * @brief Construct with an error message
			 * @param [in] msg Error message
			 */
			ErrorBase( const std::string & msg ) throw();
			/**
			 * @brief Virtual destructor
			 */
			virtual ~ErrorBase() throw();

			/**
			 * @brief Get the error message
			 * @return Error message
			 */
			virtual const char * what() const throw();
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
			/**
			 * @brief Construct with an error message
			 * @param [in] msg Error message
			 */
			Error( const std::string & msg ) throw() : ErrorBase( msg ) {}
		};

		/**
		 * @brief Run-time error class
		 */
		class RunTime {};
		/// Error on run-time
		typedef Error< RunTime > RunTimeError;

		/**
		 * @brief System error class
		 */
		class System {};
		/// Error on system
		typedef Error< System > SystemError;

		/**
		 * @brief Input/Ouput error class
		 */
		class IO {};
		/// Error on I/O
		typedef Error< IO > IOError;

	}

}

#endif
