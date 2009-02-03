/**
 * @file tr1.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_TR1_HPP
#define KHOPPER_TR1_HPP

#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>

#include <vector>

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
