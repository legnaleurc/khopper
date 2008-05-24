#ifndef LINUX_HPP
#define LINUX_HPP

#include "util/error.hpp"

#include <sys/wait.h>

#include <string>
#include <utility>
#include <vector>

#include <cerrno>

namespace Khopper {
	
	struct Linux {
		int exeRes( std::pair< std::string, std::string > & msg, const std::vector< std::string > & args );
	};
	
}

#endif
