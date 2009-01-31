/**
 * @file track.hpp
 * @author Wei-Cheng Pan
 */

#ifndef KHOPPER_TRACK_HPP
#define KHOPPER_TRACK_HPP

#include "index.hpp"
#include "encoder.hpp"
#include "decoder.hpp"

#include <QObject>

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
	class Track : public QObject {
		Q_OBJECT

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
		virtual ~Track();

		/**
		 * @brief Convert this track
		 * @param [in] targetPath output file path
		 * @param [in] encoder encoder setting
		 */
		virtual void convert( const std::wstring & targetPath, EncoderSP encoder ) const;

		int getDecodeTimes() const;

		/**
		 * @brief Dump track information
		 * @return Formated string
		 */
		std::wstring toStdWString() const;

		/**
		 * @brief Headers of fields
		 */
		static const std::vector< std::wstring > Headers;

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
		unsigned short int index;
		/// ISRC
		std::wstring isrc;
		/// Track performer
		std::wstring performer;
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

	signals:
		void decodeOnce() const;

	private:
		DecoderSP decoder_;
	};

	typedef std::tr1::shared_ptr< Track > TrackSP;

}

#endif
