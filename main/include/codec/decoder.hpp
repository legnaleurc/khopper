/**
 * @file decoder.hpp
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
#ifndef KHOPPER_DECODER_HPP
#define KHOPPER_DECODER_HPP

#include "tr1.hpp"
#include "codec_base.hpp"

#include <string>

struct AVFormatContext;
struct AVCodecContext;
struct AVPacket;

namespace Khopper {

	/**
	 * @brief Decoder interface
	 * @sa Encoder
	 *
	 * This module uses std::wstring to store unicode string.\n
	 * UTF-16 on Windows(R), UCS4 on POSIX.
	 */
	class Decoder {
	public:
		/**
		 * @brief Default constructor
		 */
		Decoder();
		/**
		 * @brief Virtual destructor
		 */
		virtual ~Decoder();

		/**
		 * @brief Open file
		 * @param filePath File path
		 * @throw Error<IO> if no file found
		 * @throw Error<Codec> decoding error
		 */
		void open( const std::wstring & filePath );
		/**
		 * @brief Check if file is opening
		 * @retval true opening
		 * @retval false closed
		 */
		bool is_open() const {
			return opening_;
		}
		/**
		 * @brief Close decoder
		 */
		void close();

		/**
		 * @brief Seek time
		 * @param timestamp Seeks time stamp, in seconds
		 * @retval ture Succeed
		 * @retval false Failed
		 */
		bool seek( double timestamp );

		/**
		 * @brief Set decode range
		 * @param begin Begin time stamp in seconds, -1 means begin of file
		 * @param end End time stamp in seconds, -1 means end of file
		 */
		void setRange( double begin, double end );
		/**
		 * @brief Read one frame
		 * @param [out] duration Decoded duration
		 * @return Frame raw binary data
		 */
		ByteArray read( double & duration );
		/**
		 * @brief Check if there is any readable data
		 * @retval true Has data to read
		 * @retval false No data
		 */
		bool hasNext() const;

		/**
		 * @brief Get sample duration
		 * @return Duration
		 */
		double getDuration() const {
			return this->duration_;
		}
		/**
		 * @brief Get title
		 * @return Title
		 */
		const std::wstring & getTitle() const {
			return this->title_;
		}
		/**
		 * @brief Get author
		 * @return Author
		 */
		const std::wstring & getAuthor() const {
			return this->author_;
		}
		/**
		 * @brief Get copyright
		 * @return Copyright
		 */
		const std::wstring & getCopyright() const {
			return this->copyright_;
		}
		/**
		 * @brief Get comment
		 * @return Comment
		 */
		const std::wstring & getComment() const {
			return this->comment_;
		}
		/**
		 * @brief Get album title
		 * @return Album title
		 */
		const std::wstring & getAlbum() const {
			return this->album_;
		}
		/**
		 * @brief Get year
		 * @return Year
		 */
		int getYear() const {
			return this->year_;
		}
		/**
		 * @brief Get track index
		 * @return Track index in album
		 */
		int getIndex() const {
			return this->index_;
		}
		/**
		 * @brief Get genre
		 * @return Genre
		 */
		const std::wstring & getGenre() const {
			return this->genre_;
		}
		/**
		 * @brief Get bit rate
		 * @return Bit rate
		 */
		int getBitRate() const {
			return this->bitRate_;
		}
		/**
		 * @brief Get sample rate
		 * @return Sample rate
		 */
		int getSampleRate() const {
			return this->sampleRate_;
		}
		/**
		 * @brief Get channels
		 * @return Channels
		 */
		int getChannels() const {
			return this->channels_;
		}

	private:
		bool afterBegin_( int64_t ) const;
		bool afterEnd_( int64_t ) const;

		bool opening_;
		int64_t begin_;
		int64_t end_;
		bool hasNext_;
		std::tr1::shared_ptr< AVFormatContext > pFormatContext_;
		std::tr1::shared_ptr< AVCodecContext > pCodecContext_;
		std::tr1::shared_ptr< AVPacket > pPacket_;
		double timeBase_;

		double duration_;
		std::wstring title_;
		std::wstring author_;
		std::wstring copyright_;
		std::wstring comment_;
		std::wstring album_;
		int year_;
		int index_;
		std::wstring genre_;
		int bitRate_;
		int sampleRate_;
		int channels_;
	};

	/**
	 * @brief Decoder smart pointer
	 *
	 * Use TR1 shared pointer.
	 */
	typedef std::tr1::shared_ptr< Decoder > DecoderSP;

}

#endif
