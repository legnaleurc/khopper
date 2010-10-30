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

#include "codecutil.hpp"

#include <QtCore/QIODevice>
#include <QtCore/QUrl>

#include <memory>

namespace khopper {

	namespace codec {

		/**
		 * @ingroup Codecs
		 * @brief Audio writer interface
		 * @sa AbstractReader
		 */
		class KHOPPER_DLL AbstractWriter : public QIODevice {
		public:
			/**
			 * @brief Virtual destructor
			 */
			virtual ~AbstractWriter();

			/**
			 * @brief Open file
			 * @param filePath
			 *
			 * AbstractWriter do not handle path encoding,
			 * so you should help yourself.
			 */
			virtual bool open( OpenMode mode );
			/**
			 * @brief Close file
			 * @note No fail!
			 */
			virtual void close();

			/**
			 * @brief Write @p data to encoder
			 * @param [in] data Raw binary data
			 */
			//void write( const ByteArray & data = ByteArray() );
			/**
			 * @brief Write unwrited data in buffer.
			 */
			void flush();

			/**
			 * @brief Set album
			 * @sa getAlbum
			 */
			void setAlbum( const QByteArray & album );
			/**
			 * @brief Set artist
			 * @sa getArtist
			 */
			void setArtist( const QByteArray & artist );
			void setAudioFormat( const AudioFormat & format );
			/**
			 * @brief Set bitRate
			 * @sa getBitRate
			 */
			void setBitRate( unsigned int bitRate );
			void setChannelLayout( ChannelLayout channelLayout );
			/**
			 * @brief Set title
			 * @sa getTitle
			 */
			void setTitle( const QByteArray & title );

		protected:
			/**
			 * @brief Default constructor
			 */
			explicit AbstractWriter( const QUrl & uri );

			/**
			 * @brief Get album
			 * @sa setAlbum
			 */
			const QByteArray & getAlbum() const;
			/**
			 * @brief Get artist
			 * @sa setArtist
			 */
			const QByteArray & getArtist() const;
			const AudioFormat & getAudioFormat() const;
			/**
			 * @brief Get bit rate
			 * @sa setBitRate
			 */
			unsigned int getBitRate() const;
			ChannelLayout getChannelLayout() const;
			/**
			 * @brief Get file path
			 * @sa setFilePath
			 */
			const QUrl & getURI() const;
			/**
			 * @brief Get title
			 * @sa setTitle
			 */
			const QByteArray & getTitle() const;

			virtual qint64 readData( char * data, qint64 maxlen );
			virtual qint64 writeData( const char * data, qint64 len );

			virtual void doOpen() = 0;
			virtual void doClose() = 0;
			virtual void writeFrame( const QByteArray & sample ) = 0;

		private:
			// prevent copying
			AbstractWriter( const AbstractWriter & );
			AbstractWriter & operator =( const AbstractWriter & );

			struct AbstractWriterPrivate;
			std::shared_ptr< AbstractWriterPrivate > p_;
		};

		/**
		 * @ingroup Codecs
		 * @brief AbstractWriter smart pointer
		 * @sa AbstractWriter WriterCSP
		 *
		 * Use TR1 shared pointer.
		 */
		typedef std::shared_ptr< AbstractWriter > WriterSP;
		/**
		 * @ingroup Codecs
		 * @brief AbstractWriter const smart pointer
		 * @sa AbstractWriter WriterSP
		 *
		 * Use TR1 shared pointer.
		 */
		typedef std::shared_ptr< const AbstractWriter > WriterCSP;

	}

}

#endif
