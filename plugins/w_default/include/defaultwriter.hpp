/**
 * @file defaultwriter.hpp
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

#include "writerplugin.hpp"

struct AVFormatContext;
struct AVOutputFormat;
struct AVStream;

namespace khopper {

	namespace codec {

		/**
		 * @brief Default audio writer
		 * @sa DefaultReader
		 *
		 * This class provides a default audio writer implementation.
		 */
		class DefaultWriter : public AbstractWriter {
		public:
			/**
			 * @brief Default constructor
			 */
			DefaultWriter();
			/**
			 * @brief Virtual destructor
			 */
			virtual ~DefaultWriter();

		private:
			virtual void setupMuxer_();
			virtual void setupEncoder_();
			virtual void openResouse_();
			virtual void closeResouse_();
			virtual void writeHeader_();
			virtual void writeFrame_( const char *, std::size_t );

			std::tr1::shared_ptr< AVFormatContext > pFormatContext_;
			AVStream * pStream_;
		};

	}

	namespace plugin {

		/// Default writer creator
		class DefaultWriterCreator : public QObject, public WriterCreator {
			Q_OBJECT
			Q_INTERFACES( khopper::plugin::WriterCreator )

		private:
			/// Creates writer object
			virtual codec::AbstractWriter * create_() const;
		};

	}

}

#endif
