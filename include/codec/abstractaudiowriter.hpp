/**
 * @file abstractaudiowriter.hpp
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
#ifndef KHOPPER_CODEC_ABSTRACTAUDIOWRITER_HPP
#define KHOPPER_CODEC_ABSTRACTAUDIOWRITER_HPP

#include "tr1.hpp"
#include "codec_base.hpp"

#include <loki/Singleton.h>
#include <loki/AssocVector.h>

#include <string>
#include <vector>

namespace Khopper {

	namespace codec {

		class AbstractAudioWriter {
		public:
			AbstractAudioWriter();
			virtual ~AbstractAudioWriter();

			void open( const std::string & );
			bool isOpen() const {
				return this->opening_;
			}
			void close();

			void write( const ByteArray & data );
			void flush();

			void setAlbum( const std::string & album ) {
				this->album_ = album;
			}
			void setArtist( const std::string & artist ) {
				this->artist_ = artist;
			}
			void setBitRate( int bitRate ) {
				this->bitRate_ = bitRate;
			}
			void setChannels( int channels ) {
				this->channels_ = channels;
			}
			void setQuality( double quality ) {
				this->quality_ = quality;
			}
			void setSampleRate( int sampleRate ) {
				this->sampleRate_ = sampleRate;
			}
			void setTitle( const std::string & title ) {
				this->title_ = title;
			}

		protected:
			const std::string & getAlbum() const {
				return this->album_;
			}
			const std::string & getArtist() const {
				return this->artist_;
			}
			int getBitRate() const {
				return this->bitRate_;
			}
			int getChannels() const {
				return this->channels_;
			}
			const std::string & getFilePath() const {
				return this->filePath_;
			}
			double getQuality() const {
				return this->quality_;
			}
			std::vector< char > & getSampleBuffer() {
				return this->sampleBuffer_;
			}
			int getSampleRate() const {
				return this->sampleRate_;
			}
			const std::string & getTitle() const {
				return this->title_;
			}

		private:
			// prevent copying
			AbstractAudioWriter( const AbstractAudioWriter & );
			AbstractAudioWriter & operator =( const AbstractAudioWriter & );

			virtual void setupMuxer_() = 0;
			virtual void setupEncoder_() = 0;
			virtual void openResouse_() = 0;
			virtual void closeResouse_() = 0;
			virtual void writeHeader_() = 0;
			virtual void writeFrame_( const char *, std::size_t ) = 0;

			std::string album_;
			std::string artist_;
			int bitRate_;
			int channels_;
			std::string filePath_;
			bool opening_;
			double quality_;
			std::vector< char > sampleBuffer_;
			ByteArray sampleQueue_;
			int sampleRate_;
			std::string title_;
		};

		/**
		 * @brief Encoder smart pointer
		 *
		 * Use TR1 shared pointer.
		 */
		typedef std::tr1::shared_ptr< AbstractAudioWriter > AudioWriterSP;

		/**
		 * @brief Encoder type list
		 *
		 * Key is identifier, value is display name on tab.
		 */
		typedef Loki::SingletonHolder< Loki::AssocVector< std::string, std::string > > AudioWriterList;

	}

}

#endif
