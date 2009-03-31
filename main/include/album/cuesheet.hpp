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
			 */
			CUESheet( const QString & content, const QString & dirPath = QString() );

			/**
			 * @brief Set CUE sheet by given whole content
			 * @param [in] content CHESheet content
			 * @param [in] dirPath where the CUE sheet comes from
			 */
			void open( const QString & content, const QString & dirPath = QString() );

			const QString & getArtist() const {
				return this->artist_;
			}
			const QString & getCatalog() const {
				return this->catalog_;
			}
			const QString & getCDTextFile() const {
				return this->cdTextFile_;
			}
			const QString & getComment( const QString & key ) const {
				return this->comments_.find( key )->second;
			}
			const std::vector< QString > & getGarbage() const {
				return this->garbage_;
			}
			const QString & getSongWriter() const {
				return this->songWriter_;
			}
			const QString & getTitle() const {
				return this->title_;
			}
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

		/**
		 * @brief Parsing error class
		 */
		class Parsing {
		};

	}

}

#endif
