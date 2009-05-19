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
			/**
			 * @brief Virtual destructor
			 */
			virtual ~DefaultWriter();

		protected:
			/// Get format context
			std::tr1::shared_ptr< AVFormatContext > formatContext() {
				return this->pFormatContext_;
			}
			/// if is VBR
			bool isVariable() const;

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
			virtual void writeFrame( const char *, std::size_t );

		private:
			std::tr1::shared_ptr< AVFormatContext > pFormatContext_;
			AVStream * pStream_;
		};

	}

}

#endif
