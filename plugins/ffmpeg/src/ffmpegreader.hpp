/**
 * @file ffmpegreader.hpp
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
#ifndef KHOPPER_CODEC_FFMPEGREADER_HPP
#define KHOPPER_CODEC_FFMPEGREADER_HPP

#include "khopper/abstractreader.hpp"

struct AVFormatContext;
struct AVCodecContext;
struct AVPacket;
struct AVStream;

namespace khopper {

	namespace codec {

		/**
		 * @brief Default audio reader
		 * @sa DefaultWriter
		 *
		 * This class provides a default audio reader implementation.
		 */
		class FfmpegReader : public AbstractReader {
		public:
			/**
			 * @brief Default constructor
			 */
			explicit FfmpegReader( const QUrl & uri );

			virtual bool atEnd() const;
			virtual qint64 pos() const;

		protected:
			/**
			 * @brief Open resource
			 * @throws IOError Can not open file
			 */
			virtual void openResource();
			/**
			 * @brief Close resource
			 */
			virtual void closeResource();
			/**
			 * @brief Setup demuxer
			 * @throws CodecError Can not find codec information.
			 * @throws CodecError Can not find duration.
			 */
			virtual void setupDemuxer();
			/**
			 * @brief Setup decoder
			 * @throws CodecError Can not find audio stream.
			 * @throws CodecError Can not find decoder.
			 */
			virtual void setupDecoder();
			/// Read header
			virtual void readHeader();
			virtual QByteArray readFrame();
			virtual bool seekFrame( qint64 msPos );
			virtual void doOpen();
			virtual void doClose();

		private:
			std::tr1::shared_ptr< AVFormatContext > pFormatContext_;
			std::tr1::shared_ptr< AVCodecContext > pCodecContext_;
			std::tr1::shared_ptr< AVPacket > pPacket_;
			AVStream * pStream_;
			int64_t msCurrent_;
			bool eof_;
		};

	}

}

#endif