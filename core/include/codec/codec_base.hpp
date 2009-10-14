/**
 * @file codec_base.hpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KHOPPER_CODEC_BASE_HPP
#define KHOPPER_CODEC_BASE_HPP

/// @defgroup Codecs De/Muxers and De/Encoders
// @{
// @}


#include "util/tr1.hpp"
#include "util/error.hpp"

#include <deque>

namespace khopper {

	/**
	 * @ingroup Codecs
	 * @brief Contains Codecs module
	 */
	namespace codec {

		/**
		 * @ingroup Codecs
		 * @brief Used for storing raw binary data
		 */
		typedef std::deque< char > ByteArray;

		/**
		 * @ingroup Codecs
		 * @brief Simple structure to represent rational number
		 */
		struct Rational {
			/**
			 * @brief Default constructor
			 *
			 * Default value is 0/1.
			 */
			Rational() : num( 0 ), den( 1 ) {}
			/**
			 * @brief Constructor
			 *
			 * The value is @p n/@p d.
			 */
			Rational( int n, int d ) : num( n ), den( d ) {}
			/// numerator
			int num;
			/// denominator
			int den;
		};

	}

	namespace error {

		/**
		 * @ingroup Codecs
		 * @brief Codec error
		 */
		class KHOPPER_EXPORT Codec {};
		/// Error on encoding or decoding
		typedef Error< Codec > CodecError;

	}

}

#endif
