#ifndef UTIL_HPP
#define UTIL_HPP

#include "input/input.hpp"
#include "output/output.hpp"

#include <loki/Factory.h>
#include <loki/Singleton.h>
#include <loki/AssocVector.h>

/**
 * @namespace Khopper
 */
namespace Khopper {
	
	typedef Loki::SingletonHolder< Loki::Factory< Input, std::string > > InputFactory;
	typedef Loki::SingletonHolder< Loki::Factory< Output, std::string > > OutputFactory;
	typedef Loki::SingletonHolder< std::pair< Loki::AssocVector< std::string, std::string >, Loki::AssocVector< std::string, std::string > > > IOTypes;
	typedef IOTypes::ObjectType::first_type InputList;
	typedef IOTypes::ObjectType::second_type OutputList;
	
}

#endif
