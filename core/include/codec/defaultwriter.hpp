/**
 * @file defaultwriter.hpp
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
#ifndef KHOPPER_CODEC_DEFAULTWRITER_HPP
#define KHOPPER_CODEC_DEFAULTWRITER_HPP

#include "codec/abstractwriter.hpp"

#include <deque>

struct AVFormatContext;
struct AVStream;

namespace khopper {

	namespace codec {

		/**
		 * @brief Default audio writer
		 * @sa DefaultReader
		 *
		 * This class provides a default audio writer implementation.
		 */
		class KHOPPER_EXPORT DefaultWriter : public AbstractWriter {
		public:
			/**
			 * @brief Default constructor
			 */
			DefaultWriter();

			void setQuality( double quality ) {
				this->quality_ = quality;
			}

		protected:
			/// Get format context
			std::tr1::shared_ptr< AVFormatContext > formatContext() {
				return this->pFormatContext_;
			}

			/**
			 * @brief setup muxer
			 * @throws CodecError Unknown output format
			 * @throws SystemError Memory allocation failure
			 */
			virtual void setupMuxer();
			/**
			 * @brief setup encoder
			 * @throws CodecError Can not setup encoder
			 */
			virtual void setupEncoder();
			virtual void openResource();
			virtual void closeResource();
			virtual void writeHeader();
			virtual void writeFrame( const short * );

		private:
			virtual void doOpen();
			virtual void doClose();
			virtual void writeFrame( const ByteArray & );

			typedef std::deque< ByteArray::value_type > ByteQueue;

			std::tr1::shared_ptr< AVFormatContext > pFormatContext_;
			AVStream * pStream_;
			ByteQueue queue_;
			double quality_;
			std::size_t frameSize_;
		};

	}

}

#endif
