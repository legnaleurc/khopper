#ifndef LINUX_HPP
#define LINUX_HPP

#include "error.hpp"

#include <sys/wait.h>

#include <string>
#include <utility>
#include <vector>

#include <cerrno>

/**
 * @namespace Khopper
 */
namespace Khopper {

	/**
	 * @struct Linux
	 * @brief Linux specific system code
	 */
	struct Linux {
		/**
		 * @brief Like system + popen
		 * @param [out] msg If you get any message, goes here. first is stdout, second is stderr
		 * @param [in] args Arguments of program
		 * @return child Process exit state
		 */
		int exeRes( std::pair< std::string, std::string > & msg, const std::vector< std::string > & args );
	};
	
}

#endif
