#include "text.hpp"

#include <QString>

namespace Khopper {

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
		std::string toLocale( const std::wstring & unicode ) {
			return QString::fromStdWString( unicode ).toLocal8Bit().constData();
		}

	}

}
