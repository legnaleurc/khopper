#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#ifdef __linux__
#include "os/linux.hpp"
#endif

#include "util/base_converter.hpp"

/**
 * @namespace Khopper
 */
namespace Khopper {
	
#ifdef __linux__
	typedef base_converter< Linux > Converter;
#endif
	
}

#endif
