/**
 * @file os.hpp
 */
#ifndef OS_HPP
#define OS_HPP

#include "error.hpp"

#include <string>
#include <utility>
#include <vector>

/**
 * @namespace Khopper
 */
namespace Khopper {

	/**
	 * @namespace os
	 * @brief OS specific system code
	 */
	namespace os {
		/**
		 * @brief Like system + popen
		 * @param [out] msg If you get any message, goes here. first is stdout, second is stderr
		 * @param [in] args Arguments of program
		 * @param [in] input String that pipe to stdin
		 * @return child Process exit state
		 */
		int getResult( std::pair< std::string, std::string > & msg, const std::vector< std::string > & args, const std::string & input = "" );
		
		/**
		 * @brief Test if program exists
		 * @param [in] exe program name
		 * @throw Error<RunTime> Strong
		 */
		void exists( const std::string & exe );

		std::string join( const std::string & front, const std::string & back );
	};
	
}

#endif
