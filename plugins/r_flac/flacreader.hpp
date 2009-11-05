/**
 * @file flacreader.hpp
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
#ifndef KHOPPER_CODEC_FLACREADER_HPP
#define KHOPPER_CODEC_FLACREADER_HPP

#include "codec/abstractreader.hpp"

#include <FLAC/stream_decoder.h>

namespace khopper {

	namespace codec {

		class FlacReader : public AbstractReader {
		public:
			FlacReader();
			virtual ~FlacReader();

		protected:
			virtual void openResource();
			virtual void closeResource();
			virtual void setupDemuxer();
			virtual void setupDecoder();
			virtual void readHeader();
			virtual ByteArray readFrame( double &, bool & );
			virtual bool seekFrame( double );

		private:
			static void metadataCallback_( const FLAC__StreamDecoder *, const FLAC__StreamMetadata *, void * );
			static FLAC__StreamDecoderWriteStatus writeCallback_( const FLAC__StreamDecoder *, const FLAC__Frame *, const FLAC__int32 * const [], void * );
			static void errorCallback_( const FLAC__StreamDecoder *, FLAC__StreamDecoderErrorStatus, void * );

			void parseVorbisComments_( const FLAC__StreamMetadata_VorbisComment & );

			std::tr1::shared_ptr< FLAC__StreamDecoder > pFD_;
			ByteArray buffer_;
			uint64_t offset_;
			double decodedTime_;
		};

	}

}

#endif
