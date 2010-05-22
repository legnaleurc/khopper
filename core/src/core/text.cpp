/**
 * @file text.cpp
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
#include "text.hpp"

#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>

namespace khopper {

	namespace text {

		std::string toUtf8( const std::wstring & unicode ) {
			return QString::fromStdWString( unicode ).toUtf8().constData();
		}
		std::wstring toStdWString( const char * str ) {
			return QString::fromUtf8( str ).toStdWString();
		}


		std::string getSuffix( const QString & filePath ) {
			return QFileInfo( filePath ).suffix().toUtf8().constData();
		}
		std::string getSuffix( const QByteArray & filePath ) {
			return getSuffix( QString::fromLocal8Bit( filePath.constData() ) );
		}

		QString joinPath( const QString & front, const QString & back ) {
			// kill all tail '/' in front
			// kill all lead '/' in back
			// return front + '/' + back
			QRegExp fp( "(.*)/*" );
			QRegExp bp( "/*(.*)" );

			if( fp.exactMatch( front ) && bp.exactMatch( back ) ) {
				return fp.cap( 1 ) + "/" + bp.cap( 1 );
			} else {
				return "";
			}
		}

	}

}
