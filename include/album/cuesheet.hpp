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
#ifndef KHOPPER_CUESHEET_HPP
#define KHOPPER_CUESHEET_HPP

#include "track.hpp"

#include <vector>
#include <string>

namespace Khopper {

	/**
	 * @brief The CUE parser
	 *
	 * This module uses std::wstring to store unicode string.\n
	 * UTF-16 on Windows(R), UCS4 on POSIX.
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
		CUESheet( const std::wstring & content, const std::wstring & dirPath = std::wstring() );

		/**
		 * @brief Set CUE sheet by given whole content
		 * @param [in] content CHESheet content
		 * @param [in] dirPath where the CUE sheet comes from
		 */
		void open( const std::wstring & content, const std::wstring & dirPath = std::wstring() );

		/**
		 * @brief Dump CUE Sheet to string
		 * @return Human readable information
		 */
		std::wstring toStdWString() const;

		/// Album performer
		std::wstring artist;
		/// Album catalog
		std::wstring catalog;
		/// CD Text File
		std::wstring cdTextFile;
		/// Album Comments
		std::map< std::wstring, std::wstring > comments;
		/// Garbage information
		std::vector< std::wstring > garbage;
		/// Album writer
		std::wstring songWriter;
		/// Album title
		std::wstring title;
		/// Album tracks
		std::vector< TrackSP > tracks;

	private:
		void parseCUE_( const std::wstring &, const std::wstring & );
		void parseSingle_( const std::wstring &, const std::wstring &, int );
		std::pair< std::wstring, Track::FileType > parseFile_( const std::wstring &, const std::wstring &, const std::wstring & );
		void parseFlags_( const std::wstring &, int );
		void parseIndex_( const std::wstring &, const std::wstring &, const std::wstring &, const std::wstring &, const std::wstring &, int );
		void parseComment_( const std::wstring &, const std::wstring &, int );
		void parseTrack_( const std::wstring &, const std::pair< std::wstring, Track::FileType > &, const std::wstring & );
		void parseGarbage_( const std::wstring &, int );
	};

}

#endif
