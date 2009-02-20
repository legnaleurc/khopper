/**
 * @file defaultaudiowriter.hpp
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
#ifndef KHOPPER_CODEC_DEFAULTAUDIOWRITER_HPP
#define KHOPPER_CODEC_DEFAULTAUDIOWRITER_HPP

#include "abstractaudiowriter.hpp"

struct AVFormatContext;
struct AVOutputFormat;
struct AVStream;

namespace Khopper {

	namespace codec {

		/**
		 * @brief Encoder interface
		 * @sa Decoder
		 */
		class DefaultAudioWriter : public AbstractAudioWriter {
		public:
			/**
			 * @brief Default constructor
			 */
			DefaultAudioWriter();
			virtual ~DefaultAudioWriter();

			/**
			 * @brief Write @p data to encoder
			 * @param [in] data Raw binary data
			 */
			void write( const ByteArray & data );
			/**
			 * @brief Write unwrited data in buffer.
			 */
			void flush();

		private:
			void writeFrame_( short * );

			virtual void setupMuxer_();
			virtual void setupEncoder_();
			virtual void openResouse_();
			virtual void closeResouse_();
			virtual void writeHeader_();
			virtual void writeFrame_( const char *, std::size_t );

			ByteArray buffer_;
			std::vector< char > samples_;
			std::tr1::shared_ptr< AVFormatContext > pFormatContext_;
			AVStream * pStream_;
		};

	}

}

#endif
