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
#ifndef KHOPPER_UTIL_TEXT_HPP
#define KHOPPER_UTIL_TEXT_HPP

#include "util/tr1.hpp"

#include <QString>
#include <string>

namespace khopper {

	/**
	 * @brief Text processing
	 */
	namespace text {

		/**
		 * @brief Encode unicode string to UTF-8
		 */
		KHOPPER_EXPORT std::string toUtf8( const std::wstring & unicode );
		/**
		 * @brief Convert narrow string to wide string
		 */
		KHOPPER_EXPORT std::wstring toStdWString( const char * str );

		/// @brief Get suffix of a file name
		KHOPPER_EXPORT std::string getSuffix( const QString & filePath );
		/**
		 * @brief Overloaded version for convenience
		 * @sa getSuffix(const QString &)
		 */
		KHOPPER_EXPORT std::string getSuffix( const QByteArray & filePath );

	}

}

#endif
