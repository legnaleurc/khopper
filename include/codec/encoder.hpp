/**
 * @file encoder.hpp
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
#ifndef KHOPPER_ENCODER_HPP
#define KHOPPER_ENCODER_HPP

#include "tr1.hpp"
#include "codec_base.hpp"

#include <loki/Singleton.h>
#include <loki/AssocVector.h>

#include <string>

struct AVFormatContext;
struct AVOutputFormat;
struct AVStream;

namespace Khopper {

	/**
	 * @brief Encoder interface
	 * @sa Decoder
	 */
	class Encoder {
	public:
		/**
		 * @brief Default constructor
		 */
		Encoder();
		/**
		 * @brief Virtual destructor
		 */
		virtual ~Encoder();

		/**
		 * @brief Open encoder
		 * @param [in] filePath Path of file
		 * @throw Error<System> memory error
		 * @throw Error<Codec> encoding error
		 * @throw Error<IO> file process error
		 */
		void open( const std::wstring & filePath );
		/**
		 * @brief Check if encoder is opening
		 * @return true Is opening
		 * @return false Closed
		 */
		bool is_open() const;
		/**
		 * @brief Close encoder
		 */
		void close();

		/**
		 * @brief Set codec quality (codec dependent)
		 * @param quality Quality
		 * @note Change will ONLY apply on openning.
		 */
		void setQuality( double quality );
		/**
		 * @brief Set bit rate
		 * @param [in] bit_rate Bit rate
		 * @note Change will ONLY apply on openning.
		 */
		void setBitRate( int bit_rate );
		/**
		 * @brief Set sample rate
		 * @param [in] sample_rate Sample rate
		 * @note Change will ONLY apply on openning.
		 */
		void setSampleRate( int sample_rate );
		/**
		 * @brief Set channels
		 * @param [in] channels Channels
		 * @note Change will ONLY apply on openning.
		 */
		void setChannels( int channels );
		/**
		 * @brief Set title
		 * @param [in] title
		 * @note Change will ONLY apply on openning.
		 */
		void setTitle( const std::wstring & title );
		/**
		 * @brief Set artist
		 * @param [in] artist
		 * @note Change will ONLY apply on openning.
		 */
		void setArtist( const std::wstring & artist );
		/**
		 * @brief Set album title
		 * @param [in] album
		 * @note Change will ONLY apply on openning.
		 */
		void setAlbum( const std::wstring & album );

		/**
		 * @brief Write @p data to encoder
		 * @param [in] data Raw binary data
		 */
		void write( const ByteArray & data );
		/**
		 * @brief Write unwrited data in buffer.
		 */
		void flush();

	protected:
		/**
		 * @brief Get file path
		 * @return ENCODED path
		 *
		 * Helps derived classes to do something.
		 */
		std::string getFilePath() const {
			return this->filePath_;
		}
		/**
		 * @brief Get output format setting
		 * @return Format handle
		 *
		 * Override this function to provide your own setting.
		 */
		virtual AVOutputFormat * guessFormat() const;
		/**
		 * @brief Hook function after close file
		 *
		 * Encoder will call this function after close a file.
		 */
		virtual void closeHook() {}

	private:
		void writeFrame_( short * );

		bool opening_;
		ByteArray buffer_;
		std::vector< char > samples_;
		std::tr1::shared_ptr< AVFormatContext > pFormatContext_;
		AVStream * pStream_;

		std::string filePath_;
		double quality_;
		int bitRate_;
		int sampleRate_;
		int channels_;
		std::string title_;
		std::string artist_;
		std::string album_;
	};

	/**
	 * @brief Encoder smart pointer
	 *
	 * Use TR1 shared pointer.
	 */
	typedef std::tr1::shared_ptr< Encoder > EncoderSP;

	/**
	 * @brief Encoder type list
	 *
	 * Key is identifier, value is display name on tab.
	 */
	typedef Loki::SingletonHolder< Loki::AssocVector< std::string, std::string > > EncoderList;

}

#endif
