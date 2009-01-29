/**
 * @file os.hpp
 * @author Wei-Cheng Pan
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
		 * @brief Encode Unicode string to local 8-bits string
		 * @return Local 8-bits string
		 */
		std::string encodeString( const std::wstring & unicode );

	}

}

#endif
