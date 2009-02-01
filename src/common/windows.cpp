#include "os.hpp"

#include <QString>

namespace Khopper {

	namespace os {

		std::wstring join( const std::wstring & front, const std::wstring & back ) {
			using namespace std::tr1;
			const wregex fp( L"(.*)\\*" );
			wsmatch fr;
			const wregex bp( L"\\*(.*)" );
			wsmatch br;

			if( regex_match( front, fr, fp ) && regex_match( back, br, bp ) ) {
				return fr[1].str() + L"\\" + br[1].str();
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
