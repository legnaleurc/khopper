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

#include "util/tr1.hpp"
#include "index.hpp"

#include <QByteArray>
#include <QTextCodec>
#include <QMap>
#include <QVariant>
#include <QUrl>

#include <vector>
#include <map>

namespace khopper {

	namespace album {

		/**
		 * @brief Provides track information.
		 *
		 * This class store data as encoded string, and ALWAYS
		 * gives you decoded strings. Please use setTextCodec to
		 * change the decoder.
		 */
		class Track {
		public:
			/**
			 * @brief Type of audio file
			 */
			enum FileType {
				/// Binary
				BINARY,
				/// Motorola
				MOTOROLA,
				/// AIFF
				AIFF,
				/// wave
				WAVE,
				/// Mpeg 1 Layer 3
				MP3
			};

			/// Track type
			enum DataType {
				/// AUDIO
				AUDIO,
				/// CDG
				CDG,
				/// MODE1/2048
				MODE1_2048,
				/// MODE1/2352
				MODE1_2352,
				/// MODE1/2336
				MODE2_2336,
				/// MODE1/2352
				MODE2_2352,
				/// CDI/2336
				CDI_2336,
				/// CDI/2352
				CDI_2352
			};

			/// Track flag
			enum Flag {
				/// NONE
				NONE = 0x00000,
				/// DCP
				DCP  = 0x00001,
				/// CH4
				CH4  = 0x00010,
				/// PRE
				PRE  = 0x00100,
				/// SCMS
				SCMS = 0x01000,
				/// DATA
				DATA = 0x10000
			};

			/**
			 * @brief Default constructor
			 */
			Track();

			/**
			 * @brief Load track from @p filePath
			 */
			void load( const QUrl & uri );

			/**
			 * @brief Get file path
			 * @sa setFilePath(const QString &) setFilePath(const QByteArray &)
			 *
			 * Encoded string in locale.
			 */
			const QUrl & getURI() const {
				return this->uri_;
			}
			/**
			 * @brief Set file path
			 * @sa getFilePath() setFilePath(const QByteArray &)
			 */
			void setURI( const QUrl & uri ) {
				this->uri_ = uri;
			}
			/**
			 * @brief Set text codec
			 */
			void setTextCodec( QTextCodec * textCodec ) {
				if( textCodec ) {
					this->textCodec_ = textCodec;
				}
			}

			void set( const QString & key, const std::string & value );
			void set( const QString & key, const QString & value );
			void set( const QString & key, const QVariant & value );

			QVariant get( const QString & key ) const;

		private:
			QMap< QString, QVariant > fields_;
			QUrl uri_;
			QTextCodec * textCodec_;
		};

		/**
		 * @brief Smart pointer of Track
		 */
		typedef std::tr1::shared_ptr< Track > TrackSP;
		/**
		 * @brief Smart pointer of const Track
		 */
		typedef std::tr1::shared_ptr< const Track > TrackCSP;

	}

}

#endif
