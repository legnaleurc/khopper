/**
 * @file abstractaudioreader.hpp
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
#ifndef KHOPPER_CODEC_ABSTRACTAUDIOREADER_HPP
#define KHOPPER_CODEC_ABSTRACTAUDIOREADER_HPP

#include "tr1.hpp"
#include "codec_base.hpp"

#include <loki/Factory.h>
#include <loki/Singleton.h>

#include <string>

namespace Khopper {

	namespace codec {

		class AbstractAudioReader {
		public:
			AbstractAudioReader();
			virtual ~AbstractAudioReader();

			void open( const std::string & filePath );
			bool isOpen() const {
				return this->opening_;
			}
			void close();

			bool seek( double timestamp );
			void setRange( double begin, double end ) {
				this->begin_ = begin;
				this->end_ = end;
			}
			ByteArray read( double & decoded );
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
			 * @return Artist
			 */
			const std::string & getArtist() const {
				return this->artist_;
			}
			/**
			 * @brief Get bit rate
			 * @return Bit rate
			 */
			int getBitRate() const {
				return this->bitRate_;
			}
			/**
			 * @brief Get channels
			 * @return Channels
			 */
			int getChannels() const {
				return this->channels_;
			}
			/**
			 * @brief Get comment
			 * @return Comment
			 */
			const std::string & getComment() const {
				return this->comment_;
			}
			/**
			 * @brief Get copyright
			 * @return Copyright
			 */
			const std::string & getCopyright() const {
				return this->copyright_;
			}
			/**
			 * @brief Get sample duration
			 * @return Duration
			 */
			double getDuration() const {
				return this->duration_;
			}
			/**
			 * @brief Get genre
			 * @return Genre
			 */
			const std::string & getGenre() const {
				return this->genre_;
			}
			/**
			 * @brief Get track index
			 * @return Track index in album
			 */
			int getIndex() const {
				return this->index_;
			}
			/**
			 * @brief Get sample rate
			 * @return Sample rate
			 */
			int getSampleRate() const {
				return this->sampleRate_;
			}
			/**
			 * @brief Get title
			 * @return Title
			 */
			const std::string & getTitle() const {
				return this->title_;
			}
			/**
			 * @brief Get year
			 * @return Year
			 */
			int getYear() const {
				return this->year_;
			}

		protected:
			bool afterBegin( double timestamp ) const;
			bool afterEnd( double timestamp ) const;

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
			void setComment( const std::string & comment ) {
				this->comment_ = comment;
			}
			void setCopyright( const std::string & copyright ) {
				this->copyright_ = copyright;
			}
			void setDuration( double duration ) {
				this->duration_ = duration;
			}
			const std::string & getFilePath() const {
				return this->filePath_;
			}
			void setGenre( const std::string & genre ) {
				this->genre_ = genre;
			}
			void setIndex( int index ) {
				this->index_ = index;
			}
			void setSampleRate( int sampleRate ) {
				this->sampleRate_ = sampleRate;
			}
			void setTitle( const std::string & title ) {
				this->title_ = title;
			}
			void setYear( int year ) {
				this->year_ = year;
			}

		private:
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
		* @brief Decoder smart pointer
		*
		* Use TR1 shared pointer.
		*/
		typedef std::tr1::shared_ptr< AbstractAudioReader > AudioReaderSP;

		/**
		* @brief The decoder Object Factory for Converter module
		*/
		typedef Loki::SingletonHolder< Loki::Factory< AbstractAudioReader, std::string >, Loki::CreateUsingNew, Loki::LongevityLifetime::DieAsSmallObjectChild > AudioReaderFactory;

	}

}

#endif
