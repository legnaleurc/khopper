#ifndef KHOPPER_INFO_TRACK_HPP
#define KHOPPER_INFO_TRACK_HPP

#include "index.hpp"
#include "encoder.hpp"
#include "decoder.hpp"

#include <string>
#include <vector>
#include <map>

namespace Khopper {

	/**
	 * @brief Track information
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

		Track();

		void convert( const std::wstring & targetPath, EncoderSP encoder ) const;

		/**
		 * @brief Dump track information
		 * @return Formated string
		 */
		std::wstring toStdWString() const;

		/**
		 * @brief Headers of fields
		 */
		static const std::vector< std::wstring > Headers;

		std::wstring filePath;
		unsigned short int index;
		Index startTime;
		std::map< std::wstring, std::wstring > comments;
		FileType fileType;
		DataType dataType;
		Index duration;
		Flag flags;
		std::vector< std::wstring > garbage;
		std::wstring isrc;
		std::wstring performer;
		Index postGap;
		Index preGap;
		std::wstring songWriter;
		std::wstring title;

	private:
		DecoderSP decoder_;
	};

}

#endif
