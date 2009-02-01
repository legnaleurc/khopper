/**
 * @file cuesheet.hpp
 * @author Wei-Cheng Pan
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

		/// Album catalog
		std::wstring catalog;
		/// CD Text File
		std::wstring cdTextFile;
		/// Album Comments
		std::map< std::wstring, std::wstring > comments;
		/// Garbage information
		std::vector< std::wstring > garbage;
		/// Album performer
		std::wstring performer;
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
