/**
 * @file oggwriter.hpp
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
#ifndef KHOPPER_CODEC_OGGWRITER_HPP
#define KHOPPER_CODEC_OGGWRITER_HPP

#include "codec/abstractwriter.hpp"

#include <vorbis/vorbisenc.h>

namespace khopper {

	namespace codec {

		/**
		 * @brief Ogg writer
		 *
		 * This class provides a ogg audio writer implementation.
		 */
		class OggWriter : public AbstractWriter {
		public:
			/**
			 * @brief Default constructor
			 */
			OggWriter();
			virtual ~OggWriter();

			void setVBRQuality( float quality ) {
				this->quality_ = quality;
			}

		protected:
			virtual void openResource();
			virtual void closeResource();
			virtual void setupMuxer();
			virtual void setupEncoder();
			virtual void writeHeader();
			virtual void writeFrame( const char *, std::size_t );

		private:
			std::tr1::shared_ptr< FILE > fout_;
			std::tr1::shared_ptr< vorbis_info > encoder_;
			std::tr1::shared_ptr< ogg_stream_state > muxer_;
			std::tr1::shared_ptr< vorbis_dsp_state > dsp_;
			std::tr1::shared_ptr< vorbis_block > block_;
			std::tr1::shared_ptr< vorbis_comment > comments_;
			float quality_;
		};

	}

}

#endif
