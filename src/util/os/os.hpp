/**
 * @file os.hpp
 */
#ifndef OS_HPP
#define OS_HPP

#include "error.hpp"

#include <string>
#include <utility>
#include <vector>

#include <cerrno>

/**
 * @namespace Khopper
 */
namespace Khopper {

	/**
	 * @struct os
	 * @brief OS specific system code
	 */
	struct os {
		/**
		 * @brief Like system + popen
		 * @param [out] msg If you get any message, goes here. first is stdout, second is stderr
		 * @param [in] args Arguments of program
		 * @return child Process exit state
		 */
		static int getResult( std::pair< std::string, std::string > & msg, const std::vector< std::string > & args );
				
		/**
		 * @brief Test if program exists
		 * @param [in] exe program name
		 * @throw Error<Runtime> if not found
		 */
		static void exists( const std::string & exe );
	};
	
}

#endif
