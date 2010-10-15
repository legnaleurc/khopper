/**
 * @file track.hpp
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
#ifndef KHOPPER_ALBUM_TRACK_HPP
#define KHOPPER_ALBUM_TRACK_HPP

#include "timestamp.hpp"
#include "album.hpp"
#include "abstractreader.hpp"

#include <QtCore/QByteArray>
#include <QtCore/QTextCodec>
#include <QtCore/QUrl>

namespace khopper {

	namespace album {

		/**
		 * @brief Provides track information.
		 *
		 * This class store data as encoded string, and ALWAYS
		 * gives you decoded strings. Please use setTextCodec to
		 * change the decoder.
		 */
		class KHOPPER_DLL Track {
		public:
			/**
			 * @brief Default constructor.
			 */
			explicit Track( const QUrl & uri );
			virtual ~Track();

			AlbumSP getAlbum() const;
			QString getArtist() const;
			const QAudioFormat & getAudioFormat() const;
			unsigned int getBitRate() const;
			const Timestamp & getDuration() const;
			unsigned int getIndex() const;
			virtual codec::ReaderSP createReader() const;
			QString getTitle() const;
			/**
			 * @brief Get url.
			 * @sa setFilePath(const QString &) setFilePath(const QByteArray &)
			 */
			const QUrl & getURI() const;

			void setAlbum( AlbumSP album );
			void setArtist( const QByteArray & artist );
			void setArtist( const QString & artist );
			void setAudioFormat( const QAudioFormat & format );
			void setBitRate( unsigned int bitRate );
			void setChannels( unsigned int channels );
			void setDuration( const Timestamp & duration );
			void setIndex( unsigned int index );
			void setSampleRate( unsigned int sampleRate );
			void setSongWriter( const QByteArray & songWriter );
			void setSongWriter( const QString & songWriter );
			/**
			 * @brief Set metadata text codec.
			 */
			void setTextCodec( QTextCodec * textCodec );
			void setTitle( const QByteArray & title );
			void setTitle( const QString & title );
			/**
			 * @brief Set url.
			 * @sa getFilePath() setFilePath(const QByteArray &)
			 */
			void setURI( const QUrl & uri );

		private:
			struct TrackPrivate;
			std::shared_ptr< TrackPrivate > p_;
		};

		/**
		 * @brief Smart pointer of Track.
		 */
		typedef std::shared_ptr< Track > TrackSP;
		/**
		 * @brief Smart pointer of const Track.
		 */
		typedef std::shared_ptr< const Track > TrackCSP;

	}

}

uint KHOPPER_DLL qHash( khopper::album::TrackCSP key );

#endif
