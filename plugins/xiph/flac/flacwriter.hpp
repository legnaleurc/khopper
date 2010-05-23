/**
 * @file flacwriter.hpp
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
#ifndef KHOPPER_CODEC_FLACWRITER_HPP
#define KHOPPER_CODEC_FLACWRITER_HPP

#include "khopper/abstractwriter.hpp"

#include <FLAC/stream_encoder.h>
#include <FLAC/metadata.h>

namespace khopper {

	namespace codec {

		class FlacWriter : public AbstractWriter {
		public:
			FlacWriter();
			virtual ~FlacWriter();

			void setOggMode( bool ogg ) {
				this->ogg_ = ogg;
			}

		private:
			virtual void doOpen();
			virtual void doClose();
			virtual void writeFrame( const ByteArray & );

			std::tr1::shared_ptr< FLAC__StreamEncoder > pFE_;
			std::vector< std::tr1::shared_ptr< FLAC__StreamMetadata > > metadataOwner_;
			bool ogg_;

			static void progressCallback_( const FLAC__StreamEncoder *, FLAC__uint64, FLAC__uint64, unsigned, unsigned, void * );
		};

	}

}

#endif