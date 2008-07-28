#ifndef TRACK_HPP
#define TRACK_HPP

#include "audiofile.hpp"

#include <set>
#include <vector>
#include <string>
#include <map>

namespace Khopper {

	struct Index {
		Index() {}
		Index( unsigned short int m, unsigned short int s, unsigned short int f ) : minute( m ), second( s ), frame( f ) {}
		unsigned short int minute;
		unsigned short int second;
		unsigned short int frame;
	};
	
	struct Track {
		enum DataType {
			AUDIO,
			CDG,
			MODE1_2048,
			MODE1_2352,
			MODE2_2336,
			MODE2_2352,
			CDI_2336,
			CDI_2352
		};
		
		enum Flag {
			DCP,
			CH4,
			PRE,
			SCMS,
			DATA,
			NONE
		};
		
		Track( unsigned short int trackNO, const std::string & type );
		Track( unsigned short int trackNO, DataType type );
		
		void addFlag( const std::string & f );
		void addFlag( Flag f );
		
		std::map< std::string, std::string > comments;
		AudioFile dataFile;
		std::vector< std::string > garbage;
		Index indices[2];
		std::string isrc;
		std::string performer;
		Index postGap;
		Index preGap;
		std::string songWriter;
		std::string title;
		std::set< Flag > flags;
		DataType dataType;
		unsigned short int number;
	};

}

#endif
