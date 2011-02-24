/**
 * @file cuesheetparser.hpp
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
#ifndef KHOPPER_ALBUM_CUESHEETPARSER_HPP
#define KHOPPER_ALBUM_CUESHEETPARSER_HPP

#include "cuesheet.hpp"
#include "cuesheettrack.hpp"

#include "khopper/error.hpp"
#include "khopper/playlist.hpp"
#include "khopper/track.hpp"

#include <QtCore/QDir>

#include <vector>
#include <string>

namespace khopper {

	namespace album {

		/**
		 * @brief The CUE parser
		 */
		class CueSheetParser {
		public:
			static PlayList load( const QString & content, const QDir & dir );

		private:
			/**
			 * @brief Give a CUE Sheet content and create this object
			 * @param [in] content CHESheet content
			 * @param [in] dirPath where the CUE sheet comes from
			 * @throws ParsingError Invalid CUE format
			 * @throws CodecError Decode media error
			 * @throws IOError File not openable
			 */
			CueSheetParser( const QString & content, const QDir & dir );

			void parseCue_( QString, const QDir & );
			void parseSingle_( const QString &, const QString & );
			void parseFile_( const QString &, const QString &, const QDir & );
			void parseFlags_( const QString & );
			void parseIndex_( const QString &, const QString &, const QString &, const QString &, const QString & );
			void parseComment_( const QString &, const QString & );
			void parseTrack_( const QString &, const QString & );
			void parseGarbage_( const QString & );

			void updateLastTrack_();

			/**
			 * @brief Set media file
			 * @param [in] filePath Audio path
			 * @throws CodecError Decode media error
			 * @throws IOError File not openable
			 */
			//void setMedia_( const QUrl & uri );

			PlayList playList_;
			std::shared_ptr< CueSheet > album_;
			QUrl currentFileURI_;
			QString currentFileType_;
			std::shared_ptr< CueSheetTrack > previousTrack_;
			std::shared_ptr< CueSheetTrack > currentTrack_;
			unsigned int trackIndex_;
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
