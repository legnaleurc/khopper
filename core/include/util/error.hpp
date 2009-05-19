/**
 * @file error.hpp
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
#ifndef KHOPPER_UTIL_ERROR_HPP
#define KHOPPER_UTIL_ERROR_HPP

#include "util/tr1.hpp"

#include <QString>

#include <exception>
#include <string>

namespace khopper {

	/// Error classes
	namespace error {

		/**
		 * @brief Base class of error types
		 *
		 * This class is the common code of error types,
		 * to avoid duplicate code generation.
		 */
		class KHOPPER_EXPORT BaseError : public std::exception {
		public:
			/**
			 * @brief Construct with an error message
			 */
			explicit BaseError( const char * msg );
			/**
			 * @brief Construct with an error message
			 */
			explicit BaseError( const std::string & msg );
			/**
			 * @brief Construct with an error message
			 */
			explicit BaseError( const std::wstring & msg );
			/**
			 * @brief Construct with an error message
			 */
			explicit BaseError( const QString & msg );
			/**
			 * @brief Virtual destructor
			 */
			virtual ~BaseError() throw();

			/// Get error message
			virtual const char * what() const throw();
			/**
			 * @brief Get the error message
			 */
			const QString & getMessage() const;
		private:
			QString msg_;
		};

		/**
		 * @brief Error class
		 * @tparam Type Custom error extension
		 */
		template< typename Type >
		class KHOPPER_EXPORT Error : public BaseError, public Type {
		public:
			/**
			 * @brief Construct with an error message
			 */
			explicit Error( const char * msg ) : BaseError( msg ) {}
			/**
			 * @brief Construct with an error message
			 */
			explicit Error( const std::string & msg ) : BaseError( msg ) {}
			/**
			 * @brief Construct with an error message
			 */
			explicit Error( const std::wstring & msg ) : BaseError( msg ) {}
			/**
			 * @brief Construct with an error message
			 */
			explicit Error( const QString & msg ) : BaseError( msg ) {}
		};

		/**
		 * @brief Run-time error class
		 */
		class KHOPPER_EXPORT RunTime {};
		/// Error on run-time
		typedef Error< RunTime > RunTimeError;

		/**
		 * @brief System error class
		 */
		class KHOPPER_EXPORT System {};
		/// Error on system
		typedef Error< System > SystemError;

		/**
		 * @brief Input/Ouput error class
		 */
		class KHOPPER_EXPORT IO {};
		/// Error on I/O
		typedef Error< IO > IOError;

	}

}

#endif
