/**
 * @file cuesheet.hpp
 */
#ifndef KHOPPER_INFO_CUESHEET_HPP
#define KHOPPER_INFO_CUESHEET_HPP

#include "track.hpp"

#include <vector>
#include <string>

namespace Khopper {

	/**
	 * @brief The CUE parser
	 */
	class CUESheet {
	public:
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

		/**
		 * @brief Headers of fields
		 */
		static const std::vector< std::wstring > Headers;

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

		std::wstring catalog;
		std::wstring cdTextFile;
		std::map< std::wstring, std::wstring > comments;
		std::vector< std::wstring > garbage;
		std::wstring performer;
		std::wstring songWriter;
		std::wstring title;
		std::vector< Track > tracks;

	private:
		void parseCUE_( const std::wstring &, const std::wstring & );
		void parseSingle_( const std::wstring &, const std::wstring &, int );
		std::pair< std::wstring, FileType > parseFile_( const std::wstring &, const std::wstring &, const std::wstring & );
		void parseFlags_( const std::wstring &, int );
		void parseIndex_( const std::wstring &, const std::wstring &, const std::wstring &, const std::wstring &, const std::wstring &, int );
		void parseComment_( const std::wstring &, const std::wstring &, int );
		void parseTrack_( const std::wstring &, const std::pair< std::wstring, FileType > &, const std::wstring & );
		void parseGarbage_( const std::wstring &, int );
	};

}

#endif
