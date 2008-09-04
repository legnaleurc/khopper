/**
 * @file types.hpp
 */
#ifndef UTIL_HPP
#define UTIL_HPP

#include <loki/Factory.h>
#include <loki/Singleton.h>
#include <loki/AssocVector.h>

#include <memory>

/**
 * @brief Namespace of this program
 */
namespace Khopper {

	class Input;
	class Output;
	
	/**
	 * @brief Input Type smart pointer
	 * 
	 * Use TR1 shared pointer.
	 */
	typedef std::tr1::shared_ptr< Input > InputSP;
	
	/**
	 * @brief Output Type smart pointer
	 * 
	 * Use TR1 shared pointer.
	 */
	typedef std::tr1::shared_ptr< Output > OutputSP;
	
	/**
	 * @brief The input audio format Object Factory for Converter module
	 */
	typedef Loki::SingletonHolder< Loki::Factory< Input, std::string >, Loki::CreateUsingNew, Loki::LongevityLifetime::DieAsSmallObjectChild > InputFactory;
	
	/**
	 * @brief The output audio format Object Factory for Converter module
	 */
	typedef Loki::SingletonHolder< Loki::Factory< Output, std::string >, Loki::CreateUsingNew, Loki::LongevityLifetime::DieAsSmallObjectChild > OutputFactory;
	
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
