/**
 * @file text.hpp
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
#ifndef KHOPPER_UTIL_TEXT_HPP
#define KHOPPER_UTIL_TEXT_HPP

#include "tr1.hpp"

#include <QtCore/QString>

#include <string>

#define KHOPPER_STRINGIZE(token) _KHOPPER_STRINGIZE(token)
#define _KHOPPER_STRINGIZE(token) #token

namespace khopper {

	/**
	 * @brief Text processing
	 */
	namespace text {

		/**
		 * @brief Encode unicode string to UTF-8
		 */
		KHOPPER_DLL std::string toUtf8( const std::wstring & unicode );
		/**
		 * @brief Convert narrow string to wide string
		 */
		KHOPPER_DLL std::wstring toStdWString( const char * str );

		/// @brief Get suffix of a file name
		KHOPPER_DLL std::string getSuffix( const QString & filePath );
		/**
		 * @brief Overloaded version for convenience
		 * @sa getSuffix(const QString &)
		 */
		KHOPPER_DLL std::string getSuffix( const QByteArray & filePath );

		/**
		 * @brief Join paths to one path
		 * @param [in] front front part
		 * @param [in] back back part
		 * @return Complete path
		 */
		KHOPPER_DLL QString joinPath( const QString & front, const QString & back );

	}

}

#endif