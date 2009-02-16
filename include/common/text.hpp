#ifndef KHOPPER_TEXT_HPP
#define KHOPPER_TEXT_HPP

#include <string>

namespace Khopper {

	namespace text {

		std::wstring fromUTF8( const std::string & utf8 );
		std::string toUTF8( const std::wstring & unicode );

		std::wstring fromLocale( const std::string & locale );
		std::string toLocale( const std::wstring & unicode );

	}

}

#endif
