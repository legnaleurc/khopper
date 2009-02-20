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
#ifndef KHOPPER_TRACK_HPP
#define KHOPPER_TRACK_HPP

#include "tr1.hpp"
#include "index.hpp"

#include <string>
#include <vector>
#include <map>

namespace Khopper {

	/**
	 * @brief Track information
	 *
	 * This module uses std::wstring to store unicode string.\n
	 * UTF-16 on Windows(R), UCS4 on POSIX.
	 */
	struct Track {
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
		void load( const std::wstring & filePath );

		/**
		 * @brief Dump track information
		 * @return Formated string
		 */
		std::wstring toStdWString() const;

		/**
		 * @brief Headers of fields
		 */
		static const std::vector< std::wstring > Headers;

		/// Album title
		std::wstring album;
		/// Track artist
		std::wstring artist;
		/// Bit Rate
		int bitRate;
		/// Channels
		int channels;
		/// Track comments
		std::map< std::wstring, std::wstring > comments;
		/// Audio data type
		DataType dataType;
		/// Track duration
		Index duration;
		/// Path which is this track refers to.
		std::wstring filePath;
		/// Audio container type
		FileType fileType;
		/// Extra flags
		Flag flags;
		/// Garbage information
		std::vector< std::wstring > garbage;
		/// Index at album
		short int index;
		/// ISRC
		std::wstring isrc;
		/// Gap to previous track (?)
		Index postGap;
		/// Gap to next track (?)
		Index preGap;
		/// Sample Rate
		int sampleRate;
		/// Song writer
		std::wstring songWriter;
		/// Start time in that file
		Index startTime;
		/// Track title
		std::wstring title;
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

#endif
