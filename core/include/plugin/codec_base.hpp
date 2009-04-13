/**
 * @file codec_base.hpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * Khopper is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Khopper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KHOPPER_CODEC_BASE_HPP
#define KHOPPER_CODEC_BASE_HPP

#include "common/error.hpp"

#include <deque>

namespace khopper {

	/// @defgroup Codecs De/Muxers and De/Encoders
	// @{
	// @}
	/**
	 * @brief Contains Codecs module
	 * @ingroup Codecs
	 */
	namespace codec {

		/**
		 * @brief Used for storing raw binary data
		 * @ingroup Codecs
		 */
		typedef std::deque< char > ByteArray;

	}

	namespace error {

		/**
		 * @brief Codec error
		 * @ingroup Codecs
		 */
		class Codec {};
		/// Error on encoding or decoding
		typedef Error< Codec > CodecError;

	}

}

#endif
