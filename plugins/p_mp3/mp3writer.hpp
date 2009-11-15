/**
 * @file mp3writer.hpp
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
#ifndef KHOPPER_CODEC_MP3WRITER_HPP
#define KHOPPER_CODEC_MP3WRITER_HPP

#include "codec/abstractwriter.hpp"

#include <lame/lame.h>

namespace khopper {

	namespace codec {

		class Mp3Writer : public AbstractWriter {
		public:
			Mp3Writer();
			virtual ~Mp3Writer();

			void setVBRQuality( int quality ) {
				this->quality_ = quality;
			}

		protected:
			virtual void openResource();
			virtual void closeResource();
			virtual void setupMuxer();
			virtual void setupEncoder();
			virtual void writeHeader();
			virtual void writeFrame( const char * samples, std::size_t nSamples );

		private:
			std::tr1::shared_ptr< FILE > fout_;
			std::tr1::shared_ptr< lame_global_flags > gfp_;
			int quality_;
			long id3v2Offset_;
		};

	}

}

#endif
