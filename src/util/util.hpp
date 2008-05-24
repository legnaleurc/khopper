#ifndef UTIL_HPP
#define UTIL_HPP

#include "output/output.hpp"

#include <loki/Factory.h>
#include <loki/Singleton.h>
#include <loki/AssocVector.h>

namespace Khopper {
	
	typedef Loki::SingletonHolder< Loki::Factory< Output, std::string > > OutputFactory;
	typedef Loki::SingletonHolder< Loki::AssocVector< std::string, std::string > > OutputTypes;
	
}

#endif
