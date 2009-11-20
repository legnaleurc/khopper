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

#include <vector>

namespace khopper {

	/**
	 * @ingroup Codecs
	 * @brief Contains Codecs module
	 */
	namespace codec {

		enum SampleFormat {
			S8 = 0x0,
			U8 = 0x1,
			S16LE = 0x2,
			S16BE = 0x3,
			U16LE = 0x4,
			U16BE = 0x5,
			S32LE = 0x6,
			S32BE = 0x7,
			U32LE = 0x8,
			U32BE = 0x9,
			FLOAT = 0xA,
			DOUBLE = 0xB,
			NONE = 0xFFFFFFFF
		};
		Q_DECLARE_FLAGS( SampleFormats, SampleFormat )

		/**
		 * @ingroup Codecs
		 * @brief Used for storing raw binary data
		 */
		typedef std::vector< uint8_t > ByteArray;

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
