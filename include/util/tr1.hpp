#ifndef KHOPPER_UTIL_TR1_HPP
#define KHOPPER_UTIL_TR1_HPP

#ifdef _HAS_TR1
#include <memory>
#include <regex>
#else
#include <tr1/memory>
#include <tr1/regex>
#endif

#include <vector>

namespace Khopper {

	typedef std::vector< char > ByteArray;

}

#endif
