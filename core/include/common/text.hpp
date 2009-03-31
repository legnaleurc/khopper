/**
 * @file text.hpp
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
#ifndef KHOPPER_TEXT_HPP
#define KHOPPER_TEXT_HPP

#include <QString>
#include <string>

namespace khopper {

	/**
	 * @brief Text processing
	 */
	namespace text {

		/// @brief Convert @p utf8 to Unicode
		std::wstring fromUTF8( const std::string & utf8 );
		/// @brief Convert @p unicode to UTF-8
		std::string toUTF8( const std::wstring & unicode );

		/// @brief Convert @p locale to Unicode
		std::wstring fromLocale( const std::string & locale );
		/// @brief Convert @p unicode to system locale
		std::string toLocale( const QString & unicode );

		/// @brief Get suffix of a file name
		std::string getSuffix( const QString & filePath );
		std::string getSuffix( const QByteArray & filePath );

	}

}

#endif
