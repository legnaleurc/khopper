/**
 * @file linux.cpp
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
#include "os.hpp"
#include "tr1.hpp"

#include <QString>

namespace Khopper {

	namespace os {

		std::wstring join( const std::wstring & front, const std::wstring & back ) {
			// kill all tail '/' in front
			// kill all lead '/' in back
			// return front + '/' + back
			using namespace std::tr1;
			const wregex fp( L"(.*)/*" );
			wsmatch fr;
			const wregex bp( L"/*(.*)" );
			wsmatch br;

			if( regex_match( front, fr, fp ) && regex_match( back, br, bp ) ) {
				return fr[1].str() + L"/" + br[1].str();
			} else {
				return L"";
			}
		}

		std::wstring decodeString( const std::string & local ) {
			return QString::fromStdString( local ).toStdWString();
		}

		std::string encodeString( const std::wstring & unicode ) {
			return QString::fromStdWString( unicode ).toLocal8Bit().constData();
		}

	}

}