#ifndef LINUX_HPP
#define LINUX_HPP

namespace Khopper {
	
	struct Linux {
		int exeRes( std::pair< std::string, std::string > & msg, const std::vector< std::string > & args );
	};
	
}

#endif