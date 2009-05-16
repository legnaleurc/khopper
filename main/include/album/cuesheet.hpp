/**
 * @file cuesheet.hpp
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
#ifndef KHOPPER_ALBUM_CUESHEET_HPP
#define KHOPPER_ALBUM_CUESHEET_HPP

#include "track.hpp"

#include "util/error.hpp"

#include <vector>
#include <string>

namespace khopper {

	namespace album {

		/**
		 * @brief The CUE parser
		 */
		class CUESheet {
		public:
			/**
			 * @brief Default constructor
			 */
			CUESheet();

			/**
			 * @brief Give a CUE Sheet content and create this object
			 * @param [in] content CHESheet content
			 * @param [in] dirPath where the CUE sheet comes from
			 * @throws ParsingError Invalid CUE format
			 * @throws CodecError Decode media error
			 * @throws IOError File not openable
			 */
			explicit CUESheet( const QString & content, const QString & dirPath = QString() );

			/**
			 * @brief Set CUE sheet by given whole content
			 * @param [in] content CHESheet content
			 * @param [in] dirPath where the CUE sheet comes from
			 * @throws ParsingError Invalid CUE format
			 * @throws CodecError Decode media error
			 * @throws IOError File not openable
			 */
			void open( const QString & content, const QString & dirPath = QString() );

			/**
			 * @brief Set media file
			 * @param [in] filePath Audio path
			 * @throws CodecError Decode media error
			 * @throws IOError File not openable
			 */
			void setMedia( const QString & filePath );

			/// Get artist
			const QString & getArtist() const {
				return this->artist_;
			}
			/// Get catalog
			const QString & getCatalog() const {
				return this->catalog_;
			}
			/// Get CD Text File
			const QString & getCDTextFile() const {
				return this->cdTextFile_;
			}
			/// Get comment by key
			const QString & getComment( const QString & key ) const {
				return this->comments_.find( key )->second;
			}
			/// Get garbage
			const std::vector< QString > & getGarbage() const {
				return this->garbage_;
			}
			/// Get song writer
			const QString & getSongWriter() const {
				return this->songWriter_;
			}
			/// Get album title
			const QString & getTitle() const {
				return this->title_;
			}
			/// Get tracks
			const std::vector< TrackSP > & getTracks() const {
				return this->tracks_;
			}

		private:
			QString artist_;
			QString catalog_;
			QString cdTextFile_;
			std::map< QString, QString > comments_;
			std::vector< QString > garbage_;
			QString songWriter_;
			QString title_;
			std::vector< TrackSP > tracks_;

			void parseCUE_( QString, const QString & );
			void parseSingle_( const QString &, const QString &, int );
			std::pair< QString, Track::FileType > parseFile_( const QString &, const QString &, const QString & );
			void parseFlags_( const QString &, int );
			void parseIndex_( const QString &, const QString &, const QString &, const QString &, const QString &, int );
			void parseComment_( const QString &, const QString &, int );
			void parseTrack_( const QString &, const std::pair< QString, Track::FileType > &, const QString & );
			void parseGarbage_( const QString &, int );
		};

	}

	namespace error {

		/// Parsing error policy
		class Parsing {};
		/// Error on parsing
		typedef Error< Parsing > ParsingError;

	}

}

#endif
