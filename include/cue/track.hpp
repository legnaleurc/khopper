#ifndef TRACK_HPP
#define TRACK_HPP

#include "audiofile.hpp"

#include <set>
#include <vector>
#include <string>
#include <map>

namespace Khopper {

	/**
	 * @brief Track index information
	 */
	struct Index {
		/**
		 * @brief Default constructor
		 * 
		 * Initialize to 0:0:0.
		 */
		Index() : minute( 0 ), second( 0 ), frame( 0 ) {}
		/**
		 * @brief Custom constructor
		 * @param [in] m minute
		 * @param [in] s second
		 * @param [in] f frame
		 * 
		 * Initialize to m:s:f.
		 */
		Index( unsigned short int m, unsigned short int s, unsigned short int f ) : minute( m ), second( s ), frame( f ) {}
		/// minite
		unsigned short int minute;
		/// second
		unsigned short int second;
		/// frame
		unsigned short int frame;
	};
	
	/**
	 * @brief Track information
	 */
	struct Track {
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
			/// DCP
			DCP,
			/// CH4
			CH4,
			/// PRE
			PRE,
			/// SCMS
			SCMS,
			/// DATA
			DATA,
			/// NONE
			NONE
		};

		/**
		 * @brief Initialize track type form string
		 * @param [in] trackNO Track number
		 * @param [in] type Track type (written in string)
		 * 
		 * The string will be parsing to DataType.
		 */
		Track( unsigned short int trackNO, const std::string & type );
		
		/**
		 * @brief Initialize track type form DataType
		 * @param [in] trackNO Track number
		 * @param [in] type Track type
		 */
		Track( unsigned short int trackNO, DataType type );

		/**
		 * @brief Add flag information form string
		 * @param [in] f Flag string
		 */
		void addFlag( const std::string & f );
		
		/**
		 * @brief Add flag information
		 * @param [in] f Flag enum
		 */
		void addFlag( Flag f );

		/// Comment map
		std::map< std::string, std::string > comments;
		/// The audio data of this track
		AudioFile dataFile;
		/// Garbage information
		std::vector< std::string > garbage;
		/// Indices (start to end)
		Index indices[2];
		/// ISRC
		std::string isrc;
		/// Performer
		std::string performer;
		/// PostGap index
		Index postGap;
		/// PreGap index
		Index preGap;
		/// SongWriter
		std::string songWriter;
		/// Track title
		std::string title;
		/// Flags
		std::set< Flag > flags;
		/// Audio type
		DataType dataType;
		/// Track number
		unsigned short int number;
	};

}

#endif
