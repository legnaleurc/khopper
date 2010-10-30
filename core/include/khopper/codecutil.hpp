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


#include "tr1.hpp"
#include "error.hpp"

#include <memory>

namespace khopper {

	/**
	 * @ingroup Codecs
	 * @brief Contains Codecs module
	 */
	namespace codec {

		enum Channel {
			FrontLeft          = 0x00000001,
			FrontRight         = 0x00000002,
			FrontCenter        = 0x00000004,
			LowFrequency       = 0x00000008,
			BackLeft           = 0x00000010,
			BackRight          = 0x00000020,
			FrontLeftOfCenter  = 0x00000040,
			FrontRightOfCenter = 0x00000080,
			BackCenter         = 0x00000100,
			SideLeft           = 0x00000200,
			SideRight          = 0x00000400,
			TopCenter          = 0x00000800,
			TopFrontLeft       = 0x00001000,
			TopFrontCenter     = 0x00002000,
			TopFrontRight      = 0x00004000,
			TopBackLeft        = 0x00008000,
			TopBackCenter      = 0x00010000,
			TopBackRight       = 0x00020000,
			StereoLeft         = 0x20000000,
			StereoRight        = 0x40000000,

			Invalid            = 0x00000000
		};

		enum ChannelLayout {
			LayoutMono          = FrontCenter,
			LayoutStereo        = FrontLeft | FrontRight,
			Layout21            = LayoutStereo | BackCenter,
			LayoutSurround      = LayoutStereo | FrontCenter,
			Layout4Point0       = LayoutSurround | BackCenter,
			Layout22            = LayoutStereo | SideLeft | SideRight,
			LayoutQuad          = LayoutStereo | BackLeft | BackRight,
			Layout5Point0       = LayoutSurround | SideLeft | SideRight,
			Layout5Point1       = Layout5Point0 | LowFrequency,
			Layout5Point0Back   = LayoutSurround | BackLeft | BackRight,
			Layout5Point1Back   = Layout5Point0Back | LowFrequency,
			Layout7Point0       = Layout5Point0 | BackLeft | BackRight,
			Layout7Point1       = Layout5Point1 | BackLeft | BackRight,
			Layout7Point1Wide   = Layout5Point1Back | FrontLeftOfCenter | FrontRightOfCenter,
			LayoutStereoDownmix = StereoLeft | StereoRight,

			LayoutNative        = 0x00000000
		};

		class KHOPPER_DLL AudioFormat {
		public:
			enum Endian {
				BigEndian = QSysInfo::BigEndian,
				LittleEndian = QSysInfo::LittleEndian
			};
			enum SampleType {
				Unknown,
				SignedInt,
				UnSignedInt,
				Float
			};

			AudioFormat();
			AudioFormat( const AudioFormat & that );

			Endian byteOrder() const;
			int channels() const;
			QString codec() const;
			int frequency() const;
			bool isValid() const;
			int sampleSize() const;
			SampleType sampleType() const;
			void setByteOrder( Endian byteOrder );
			void setChannels( int channels );
			void setCodec( const QString & codec );
			void setFrequency( int frequency );
			void setSampleSize( int sampleSize );
			void setSampleType( SampleType sampleType );

			bool operator !=( const AudioFormat & that ) const;
			AudioFormat & operator =( const AudioFormat & that );
			bool operator ==( const AudioFormat & that ) const;

		private:
			class AudioFormatPrivate;
			std::shared_ptr< AudioFormatPrivate > p_;
		};

	}

	namespace error {

		/**
		 * @ingroup Codecs
		 * @brief Codec error
		 */
		class Codec {};
		/// Error on encoding or decoding
		typedef Error< Codec > CodecError;

	}

}

#endif
