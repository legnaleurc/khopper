/**
 * @file text.cpp
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
#include "common/text.hpp"

#include <QFileInfo>

namespace khopper {

	namespace text {

		std::wstring fromUTF8( const std::string & utf8 ) {
			return QString::fromUtf8( utf8.c_str() ).toStdWString();
		}

		std::string toUTF8( const std::wstring & unicode ) {
			return QString::fromStdWString( unicode ).toUtf8().constData();
		}

		std::wstring fromLocale( const std::string & locale ) {
			return QString::fromLocal8Bit( locale.c_str() ).toStdWString();
		}
		std::string toLocale( const QString & unicode ) {
			return unicode.toLocal8Bit().constData();
		}

		std::string getSuffix( const QString & filePath ) {
			return QFileInfo( filePath ).suffix().toUtf8().constData();
		}
		std::string getSuffix( const QByteArray & filePath ) {
			return getSuffix( QString::fromLocal8Bit( filePath.constData() ) );
		}

	}

}
