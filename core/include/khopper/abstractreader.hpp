/**
 * @file abstractreader.hpp
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
#ifndef KHOPPER_CODEC_ABSTRACTREADER_HPP
#define KHOPPER_CODEC_ABSTRACTREADER_HPP

#include "codecutil.hpp"

#include <QtCore/QIODevice>
#include <QtCore/QUrl>
#include <QtMultimedia/QAudioFormat>

#ifndef LOKI_CLASS_LEVEL_THREADING
# define LOKI_CLASS_LEVEL_THREADING
#endif
#include <loki/Functor.h>

namespace khopper {

	namespace codec {

		class AbstractReader;
		/**
		 * @ingroup Codecs
		 * @brief AbstractReader smart pointer
		 * @sa AbstractReader ReaderCSP
		 *
		 * Use TR1 shared pointer.
		 */
		typedef std::tr1::shared_ptr< AbstractReader > ReaderSP;
		/**
		 * @ingroup Codecs
		 * @brief AbstractReader const smart pointer
		 * @sa AbstractReader ReaderSP
		 *
		 * Use TR1 shared pointer.
		 */
		typedef std::tr1::shared_ptr< const AbstractReader > ReaderCSP;

		/**
		 * @ingroup Codecs
		 * @brief Audio reader interface
		 * @sa AbstractWriter
		 */
		class KHOPPER_DLL AbstractReader : public QIODevice {
		public:
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
			virtual bool open( OpenMode mode );
			/**
			 * @brief Close file
			 * @note No fail!
			 */
			virtual void close();

			/**
			 * @brief Seek to @p ms
			 * @return succeed of failed
			 */
			virtual bool seek( qint64 msPos );
			virtual qint64 size() const;
			/**
			 * @brief Set decode range
			 */
			//void setRange( int64_t msBegin, int64_t msEnd );
			/**
			 * @brief Read frame
			 * @param [out] decoded
			 * @return raw data
			 */
//			ByteArray read( int64_t & msDecoded );
			/**
			 * @brief Checks if there is any data to read
			 */
			//bool hasNext() const;

			/**
			 * @brief Get album title
			 * @return Album title
			 */
			const QByteArray & getAlbum() const;
			/**
			 * @brief Get author
			 * @sa setArtist
			 */
			const QByteArray & getArtist() const;
			const QAudioFormat & getAudioFormat() const;
			/**
			 * @brief Get bit rate
			 * @sa setBitRate
			 */
			unsigned int getBitRate() const;
			ChannelLayout getChannelLayout() const;
			/**
			 * @brief Get channels
			 * @sa setChannels
			 */
			//unsigned int getChannels() const;
			/**
			 * @brief Get comment
			 * @sa setComment
			 */
			const QByteArray & getComment() const;
			/**
			 * @brief Get copyright
			 * @sa setCopyright
			 */
			const QByteArray & getCopyright() const;
			/**
			 * @brief Get sample duration in millisecond
			 * @sa setDuration
			 */
			qint64 getDuration() const;
			/**
			 * @brief Get genre
			 * @sa setGenre
			 */
			const QByteArray & getGenre() const;
			/**
			 * @brief Get track index
			 * @sa setIndex
			 */
			unsigned int getIndex() const;
			//SampleFormat getSampleFormat() const;
			/**
			 * @brief Get sample rate
			 * @sa setSampleRate
			 */
			//unsigned int getSampleRate() const;
			/**
			 * @brief Get title
			 * @sa setTitle
			 */
			const QByteArray & getTitle() const;
			/**
			 * @brief Get file path
			 */
			const QUrl & getURI() const;
			/**
			 * @brief Get year
			 * @sa setYear
			 */
			const QString & getYear() const;

		protected:
			/**
			 * @brief Default constructor
			 */
			explicit AbstractReader( const QUrl & uri );

			/// If @p timestamp is after decoding begin timestamp
			//bool afterBegin( int64_t ms ) const;
			/// If @p timestamp is after decoding end timestamp
			//bool afterEnd( int64_t ms ) const;

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
			void setAudioFormat( const QAudioFormat & format );
			/**
			 * @brief Set bit rate
			 * @sa getBitRate
			 */
			void setBitRate( unsigned int bitRate );
			void setChannelLayout( ChannelLayout channelLayout );
			/**
			 * @brief Set channels
			 * @sa getChannels
			 */
			//void setChannels( unsigned int channels );
			/**
			 * @brief Set comment
			 * @sa getComment
			 */
			void setComment( const QByteArray & comment );
			/**
			 * @brief Set copyright
			 * @sa getCopyright
			 */
			void setCopyright( const QByteArray & copyright );
			/**
			 * @brief Set duration in millisecond
			 * @sa getDuration
			 */
			void setDuration( qint64 msDuration );
			/**
			 * @brief Set genre
			 * @sa getGenre
			 */
			void setGenre( const QByteArray & genre );
			/**
			 * @brief Set track index
			 * @sa getIndex
			 */
			void setIndex( unsigned int index );
			//void setSampleFormat( SampleFormat sampleFormat );
			/**
			 * @brief Set sample rate
			 * @sa getSampleRate
			 */
			//void setSampleRate( unsigned int sampleRate );
			/**
			 * @brief Set track title
			 * @sa getTitle
			 */
			void setTitle( const QByteArray & title );
			/**
			 * @brief Set year
			 * @sa getYear
			 */
			void setYear( const QString & year );

			virtual qint64 readData( char * data, qint64 maxSize );
			virtual qint64 writeData( const char * data, qint64 maxSize );

			/// Read one frame
			virtual QByteArray readFrame() = 0;
			/// Seek frame
			virtual bool seekFrame( qint64 msPos ) = 0;
			virtual void doOpen() = 0;
			virtual void doClose() = 0;

		private:
			// prevent copying
			AbstractReader( const AbstractReader & );
			AbstractReader & operator =( const AbstractReader & );

			struct AbstractReaderPrivate;
			std::tr1::shared_ptr< AbstractReaderPrivate > p_;
		};

	}

	namespace plugin {

		typedef Loki::Functor<
			unsigned int,
			LOKI_TYPELIST_1( const QUrl & ),
			Loki::ClassLevelLockable
		> ReaderVerifier;
		typedef Loki::Functor<
			codec::ReaderSP,
			LOKI_TYPELIST_1( const QUrl & ),
			Loki::ClassLevelLockable
		> ReaderCreator;

		/**
		 * @ingroup Plugins
		 * @brief Register plugin to factory
		 * @param key the key used in program
		 * @param name plugin name
		 * @return if registered in factory
		 */
		bool KHOPPER_DLL registerReader( ReaderVerifier v, ReaderCreator c );
		void KHOPPER_DLL unregisterReader( ReaderVerifier v );
		/**
		 * @ingroup Plugins
		 * @brief Create reader
		 * @param key format key
		 * @return Smart pointer of Product
		 * @throws RunTimeError Can not load any plugin
		 */
		codec::ReaderSP KHOPPER_DLL createReader( const QUrl & uri );

	}

}

#endif
