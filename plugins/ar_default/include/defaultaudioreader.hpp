/**
 * @file defaultaudioreader.hpp
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
#ifndef KHOPPER_CODEC_DEFAULTAUDIOREADER_HPP
#define KHOPPER_CODEC_DEFAULTAUDIOREADER_HPP

#include "abstractaudioreader.hpp"

struct AVFormatContext;
struct AVCodecContext;
struct AVPacket;

namespace khopper {

	namespace codec {

		/**
		 * @brief Default audio reader
		 * @sa DefaultAudioWriter
		 *
		 * This class provides a default audio reader implementation.
		 */
		class DefaultAudioReader : public AbstractAudioReader {
		public:
			/**
			 * @brief Default constructor
			 */
			DefaultAudioReader();
			/**
			 * @brief Virtual destructor
			 */
			virtual ~DefaultAudioReader();

		private:
			virtual void openResource_();
			virtual void closeResource_();
			virtual void setupDemuxer_();
			virtual void setupDecoder_();
			virtual void readHeader_();
			virtual ByteArray readFrame_( double &, bool & );
			virtual bool seek_( double );

			std::tr1::shared_ptr< AVFormatContext > pFormatContext_;
			std::tr1::shared_ptr< AVCodecContext > pCodecContext_;
			std::tr1::shared_ptr< AVPacket > pPacket_;
			double timeBase_;
		};

	}

	namespace plugin {

		class DefaultAudioReaderCreator : public QObject, public AudioReaderCreator {
			Q_OBJECT
			Q_INTERFACES( khopper::plugin::AudioReaderCreator )

		public:
			virtual codec::AudioReaderSP create() const;
		};

	}

}

#endif
