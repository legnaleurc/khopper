/**
 * @file os.hpp
 */
#ifndef KHOPPER_UTIL_OS_HPP
#define KHOPPER_UTIL_OS_HPP

#include "error.hpp"

#include <string>
#include <utility>
#include <vector>

/**
 * @namespace Khopper
 */
namespace Khopper {

	/**
	 * @brief Operating System dependant system code
	 */
	namespace os {
		/**
		 * @brief Like system + popen
		 * @param [out] msg If you get any message, goes here. first is stdout, second is stderr
		 * @param [in] args Arguments of program
		 * @param [in] input String that pipe to stdin
		 * @return child process exit state
		 */
		int getResult( std::pair< std::string, std::string > & msg, const std::vector< std::string > & args, const std::string & input = "" );
		
		/**
		 * @brief Test if program exists
		 * @param [in] exe program name
		 * @throw Error<RunTime> Strong
		 */
		void exists( const std::string & exe );
		
		/**
		 * @brief Join paths to one path
		 * @param [in] front front part
		 * @param [in] back back part
		 * @return Complete path
		 */
		std::string join( const std::string & front, const std::string & back );

		std::wstring join( const std::wstring & front, const std::wstring & back );

		std::string encodeString( const std::wstring & unicode );
	};
	
}

#endif
