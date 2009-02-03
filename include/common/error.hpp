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
	 * @brief Prototype of all errors
	 * @tparam Type Custom error extension
	 */
	template< typename Type >
	class Error : public std::exception, public Type {
	public:
		/**
		 * @brief Constructor that set error message
		 * @param [in] msg Error message
		 */
		Error( const std::string & msg ) throw() : msg_( msg ) {}
		/**
		 * @brief Get the error message
		 * @return Error message
		 */
		virtual const char * what() const throw() {
			return msg_.c_str();
		}
		/**
		 * @brief Dummy destructor
		 */
		virtual ~Error() throw() {}
	private:
		std::string msg_;
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
	class Parsing {
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
