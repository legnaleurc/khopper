/**
 * @file os.hpp
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
#ifndef KHOPPER_OS_HPP
#define KHOPPER_OS_HPP

#include <string>

/**
 * @namespace Khopper
 */
namespace Khopper {

	/**
	 * @brief Operating System dependant system code
	 */
	namespace os {

		/**
		 * @brief Join paths to one path
		 * @param [in] front front part
		 * @param [in] back back part
		 * @return Complete path
		 */
		std::wstring join( const std::wstring & front, const std::wstring & back );

		/**
		 * @brief Decode local 8-bit string to Unicode
		 * @param local Local 8-bit string
		 * @return Unicode string
		 *
		 * On POSIX, @p unicode is UCS4, on Windows(R), @p unicode is UTF-16.\n
		 * The decoded string will use system locale.
		 */
		std::wstring decodeString( const std::string & local );

		/**
		 * @brief Encode Unicode string to local 8-bits string
		 * @param unicode Unicode string
		 * @param codec Encoding identifier, 0 = locale, 1 = UTF-8
		 * @return Depends on @p codec
		 *
		 * On POSIX, @p unicode is UCS4, on Windows(R), @p unicode is UTF-16.\n
		 * The encoded string will use system locale.
		 */
		std::string encodeString( const std::wstring & unicode, int codec = 0 );

	}

}

#endif
