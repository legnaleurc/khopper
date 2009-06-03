/**
 * @file abstractwriter.hpp
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
#ifndef KHOPPER_CODEC_ABSTRACTWRITER_HPP
#define KHOPPER_CODEC_ABSTRACTWRITER_HPP

#include "codec_base.hpp"

#include <string>
#include <vector>

namespace khopper {

	namespace codec {

		/**
		 * @ingroup Codecs
		 * @brief Audio writer interface
		 * @sa AbstractReader
		 */
		class KHOPPER_EXPORT AbstractWriter {
		public:
			/**
			 * @brief Default constructor
			 */
			AbstractWriter();
			/**
			 * @brief Virtual destructor
			 */
			virtual ~AbstractWriter();

			void open( const std::string & filePath );
			/**
			 * @brief Open file
			 * @param filePath
			 *
			 * AbstractWriter do not handle path encoding,
			 * so you should help yourself.
			 */
			void open( const std::wstring & filePath );
			/// Check if audio writer is opening
			bool isOpen() const {
				return this->opening_;
			}
			/**
			 * @brief Close file
			 * @note No fail!
			 */
			void close();

			/**
			 * @brief Write @p data to encoder
			 * @param [in] data Raw binary data
			 */
			void write( const ByteArray & data );
			/**
			 * @brief Write unwrited data in buffer.
			 */
			void flush();

			/**
			 * @brief Set album
			 * @sa getAlbum
			 */
			void setAlbum( const std::string & album ) {
				this->album_ = album;
			}
			/**
			 * @brief Set artist
			 * @sa getArtist
			 */
			void setArtist( const std::string & artist ) {
				this->artist_ = artist;
			}
			/**
			 * @brief Set bitRate
			 * @sa getBitRate
			 */
			void setBitRate( int bitRate ) {
				this->bitRate_ = bitRate;
			}
			/**
			 * @brief Set channels
			 * @sa getChannels
			 */
			void setChannels( int channels ) {
				this->channels_ = channels;
			}
			/**
			 * @brief Set quality
			 * @sa getQuality
			 */
			void setQuality( double quality ) {
				this->quality_ = quality;
			}
			/**
			 * @brief Set sample rate
			 * @sa getSampleRate
			 */
			void setSampleRate( int sampleRate ) {
				this->sampleRate_ = sampleRate;
			}
			void setTimebase( const Rational & timebase ) {
				this->timebase_ = timebase;
			}
			/**
			 * @brief Set title
			 * @sa getTitle
			 */
			void setTitle( const std::string & title ) {
				this->title_ = title;
			}

		protected:
			/**
			 * @brief Get album
			 * @sa setAlbum
			 */
			const std::string & getAlbum() const {
				return this->album_;
			}
			/**
			 * @brief Get artist
			 * @sa setArtist
			 */
			const std::string & getArtist() const {
				return this->artist_;
			}
			/**
			 * @brief Get bit rate
			 * @sa setBitRate
			 */
			int getBitRate() const {
				return this->bitRate_;
			}
			/**
			 * @brief Get channels
			 * @sa setChannels
			 */
			int getChannels() const {
				return this->channels_;
			}
			/**
			 * @brief Get file path
			 * @sa setFilePath
			 */
			const std::wstring & getFilePath() const {
				return this->filePath_;
			}
			/**
			 * @brief Get quality
			 * @sa setQuality
			 */
			double getQuality() const {
				return this->quality_;
			}
			/**
			 * @brief Get sampling buffer
			 */
			std::vector< char > & getSampleBuffer() {
				return this->sampleBuffer_;
			}
			/**
			 * @brief Get sample rate
			 * @sa setSampleRate
			 */
			int getSampleRate() const {
				return this->sampleRate_;
			}
			const Rational & getTimebase() const {
				return this->timebase_;
			}
			/**
			 * @brief Get title
			 * @sa setTitle
			 */
			const std::string & getTitle() const {
				return this->title_;
			}

			/// Setup muxer
			virtual void setupMuxer() = 0;
			/// Setup encoder
			virtual void setupEncoder() = 0;
			/// Open resource
			virtual void openResource() = 0;
			/**
			 * @brief Close resource
			 * @note If an exception was thrown, program will terminate immediately
			 */
			virtual void closeResource() = 0;
			/// Write header
			virtual void writeHeader() = 0;
			/**
			 * @brief Write a frame
			 * @param sample sample data
			 * @param nSample sample size
			 */
			virtual void writeFrame( const char * sample, std::size_t nSample ) = 0;

		private:
			// prevent copying
			AbstractWriter( const AbstractWriter & );
			AbstractWriter & operator =( const AbstractWriter & );

			std::string album_;
			std::string artist_;
			int bitRate_;
			int channels_;
			std::wstring filePath_;
			bool opening_;
			double quality_;
			std::vector< char > sampleBuffer_;
			ByteArray sampleQueue_;
			int sampleRate_;
			Rational timebase_;
			std::string title_;
		};

		/**
		 * @ingroup Codecs
		 * @brief AbstractWriter smart pointer
		 * @sa AbstractWriter WriterCSP
		 *
		 * Use TR1 shared pointer.
		 */
		typedef std::tr1::shared_ptr< AbstractWriter > WriterSP;
		/**
		 * @ingroup Codecs
		 * @brief AbstractWriter const smart pointer
		 * @sa AbstractWriter WriterSP
		 *
		 * Use TR1 shared pointer.
		 */
		typedef std::tr1::shared_ptr< const AbstractWriter > WriterCSP;

	}

}

#endif
