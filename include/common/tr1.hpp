/**
 * @file tr1.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_TR1_HPP
#define KHOPPER_TR1_HPP

#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>

#include <vector>

/// dirty hack
#ifndef INT64_C
# if __WORDSIZE == 64
#  define INT64_C(c)	c ## L
# else
#  define INT64_C(c)	c ## LL
# endif
#endif

namespace std {

	namespace tr1 = boost;

}

namespace Khopper {

	/**
	 * @brief Used for storing raw binary data
	 */
	typedef std::vector< char > ByteArray;

}

#endif
