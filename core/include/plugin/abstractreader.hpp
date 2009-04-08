/**
 * @file abstractreader.hpp
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
#ifndef KHOPPER_CODEC_ABSTRACTREADER_HPP
#define KHOPPER_CODEC_ABSTRACTREADER_HPP

#include "common/tr1.hpp"
#include "codec_base.hpp"

#include <string>

namespace khopper {

	namespace codec {

		/**
		 * @brief Audio reader interface
		 * @ingroup Codecs
		 * @sa AbstractWriter
		 */
		class AbstractReader {
		public:
			/**
			 * @brief Default constructor
			 */
			AbstractReader();
			/**
			 * @brief Virtual destructor
			 */
			virtual ~AbstractReader();

			/**
			 * @brief Open file
			 * @param [in] filePath
			 *
			 * AbstractReader do not handle path encoding,
			 * so you should help yourself.
			 */
			void open( const std::string & filePath );
			/// Check if audio reader is opening
			bool isOpen() const {
				return this->opening_;
			}
			/**
			 * @brief Close file
			 * @note No fail!
			 */
			void close();

			/**
			 * @brief Seek to @p timestamp
			 * @return succeed of failed
			 */
			bool seek( double timestamp );
			/**
			 * @brief Set decode range
			 */
			void setRange( double begin, double end ) {
				this->begin_ = begin;
				this->end_ = end;
			}
			/**
			 * @brief Read frame
			 * @param [out] decoded
			 * @return raw data
			 */
			ByteArray read( double & decoded );
			/**
			 * @brief Checks if there is any data to read
			 */
			bool hasNext() const {
				return this->hasNext_;
			}

			/**
			 * @brief Get album title
			 * @return Album title
			 */
			const std::string & getAlbum() const {
				return this->album_;
			}
			/**
			 * @brief Get author
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
			 * @brief Get comment
			 * @sa setComment
			 */
			const std::string & getComment() const {
				return this->comment_;
			}
			/**
			 * @brief Get copyright
			 * @sa setCopyright
			 */
			const std::string & getCopyright() const {
				return this->copyright_;
			}
			/**
			 * @brief Get sample duration
			 * @sa setDuration
			 */
			double getDuration() const {
				return this->duration_;
			}
			/**
			 * @brief Get genre
			 * @sa setGenre
			 */
			const std::string & getGenre() const {
				return this->genre_;
			}
			/**
			 * @brief Get track index
			 * @sa setIndex
			 */
			int getIndex() const {
				return this->index_;
			}
			/**
			 * @brief Get sample rate
			 * @sa setSampleRate
			 */
			int getSampleRate() const {
				return this->sampleRate_;
			}
			/**
			 * @brief Get title
			 * @sa setTitle
			 */
			const std::string & getTitle() const {
				return this->title_;
			}
			/**
			 * @brief Get year
			 * @sa setYear
			 */
			int getYear() const {
				return this->year_;
			}

		protected:
			/// If @p timestamp is after decoding begin timestamp
			bool afterBegin( double timestamp ) const;
			/// If @p timestamp is after decoding end timestamp
			bool afterEnd( double timestamp ) const;

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
			 * @brief Set bit rate
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
			 * @brief Set comment
			 * @sa getComment
			 */
			void setComment( const std::string & comment ) {
				this->comment_ = comment;
			}
			/**
			 * @brief Set copyright
			 * @sa getCopyright
			 */
			void setCopyright( const std::string & copyright ) {
				this->copyright_ = copyright;
			}
			/**
			 * @brief Set duration
			 * @sa getDuration
			 */
			void setDuration( double duration ) {
				this->duration_ = duration;
			}
			/**
			 * @brief Get file path
			 */
			const std::string & getFilePath() const {
				return this->filePath_;
			}
			/**
			 * @brief Set genre
			 * @sa getGenre
			 */
			void setGenre( const std::string & genre ) {
				this->genre_ = genre;
			}
			/**
			 * @brief Set track index
			 * @sa getIndex
			 */
			void setIndex( int index ) {
				this->index_ = index;
			}
			/**
			 * @brief Set sample rate
			 * @sa getSampleRate
			 */
			void setSampleRate( int sampleRate ) {
				this->sampleRate_ = sampleRate;
			}
			/**
			 * @brief Set track title
			 * @sa getTitle
			 */
			void setTitle( const std::string & title ) {
				this->title_ = title;
			}
			/**
			 * @brief Set year
			 * @sa getYear
			 */
			void setYear( int year ) {
				this->year_ = year;
			}

		private:
			// prevent copying
			AbstractReader( const AbstractReader & );
			AbstractReader & operator =( const AbstractReader & );

			virtual void openResource_() = 0;
			virtual void closeResource_() = 0;
			virtual void setupDemuxer_() = 0;
			virtual void setupDecoder_() = 0;
			virtual void readHeader_() = 0;
			virtual ByteArray readFrame_( double &, bool & ) = 0;
			virtual bool seek_( double ) = 0;

			std::string album_;
			std::string artist_;
			double begin_;
			int bitRate_;
			int channels_;
			std::string comment_;
			std::string copyright_;
			double duration_;
			double end_;
			std::string filePath_;
			std::string genre_;
			bool hasNext_;
			int index_;
			bool opening_;
			int sampleRate_;
			std::string title_;
			int year_;
		};

		/**
		 * @brief AbstractReader smart pointer
		 * @ingroup Codecs
		 * @sa AbstractReader ReaderCSP
		 *
		 * Use TR1 shared pointer.
		 */
		typedef std::tr1::shared_ptr< AbstractReader > ReaderSP;
		/**
		 * @brief AbstractReader const smart pointer
		 * @ingroup Codecs
		 * @sa AbstractReader ReaderSP
		 *
		 * Use TR1 shared pointer.
		 */
		typedef std::tr1::shared_ptr< const AbstractReader > ReaderCSP;

	}

}

#endif
