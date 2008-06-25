/**
 * @file type.hpp
 */
#ifndef UTIL_HPP
#define UTIL_HPP

#include "input/input.hpp"
#include "output/output.hpp"

#include <loki/Factory.h>
#include <loki/Singleton.h>
#include <loki/AssocVector.h>

/**
 * @brief Namespace of this program
 */
namespace Khopper {
	
	/**
	 * @brief The input audio format Object Factory for Converter module
	 */
	typedef Loki::SingletonHolder< Loki::Factory< Input, std::string > > InputFactory;
	
	/**
	 * @brief The output audio format Object Factory for Converter module
	 */
	typedef Loki::SingletonHolder< Loki::Factory< Output, std::string > > OutputFactory;
	
	/**
	 * @brief The input and output Object Factory for user interface
	 */
	typedef Loki::SingletonHolder< std::pair< Loki::AssocVector< std::string, std::string >, Loki::AssocVector< std::string, std::string > > > IOTypes;
	
	/**
	 * @brief The convience alias for take input format list
	 */
	typedef IOTypes::ObjectType::first_type InputList;
	
	/**
	 * @brief The convience alias for take output format list
	 */
	typedef IOTypes::ObjectType::second_type OutputList;
	
}

#endif
