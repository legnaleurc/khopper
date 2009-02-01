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
		 * @brief Decode local 8-bit string to Unicode
		 * @param local Local 8-bit string
		 * @return Unicode string
		 *
		 * On POSIX, @p unicode is UCS4, on Windows(R), @p unicode is UTF-16.\n
		 * The decoded string will use system locale.
		 */
		std::wstring decodeString( const std::string & local );

		/**
		 * @brief Encode Unicode string to local 8-bits string
		 * @param unicode Unicode string
		 * @return Local 8-bits string
		 *
		 * On POSIX, @p unicode is UCS4, on Windows(R), @p unicode is UTF-16.\n
		 * The encoded string will use system locale.
		 */
		std::string encodeString( const std::wstring & unicode );

	}

}

#endif
