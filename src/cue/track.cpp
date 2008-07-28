#include "track.hpp"

namespace Khopper {
	
	Track::Track( unsigned short int trackNO, const std::string & type ) : number( trackNO ), preGap( 0, 0, 0 ), postGap( 0, 0, 0 ) {
		if( type == "AUDIO" ) {
			dataType = AUDIO;
		} else if( type == "CDG" ) {
			dataType = CDG;
		} else if( type == "MODE1/2048" ) {
			dataType = MODE1_2048;
		} else if( type == "MODE1/2352" ) {
			dataType = MODE1_2352;
		} else if( type == "MODE2/2336" ) {
			dataType = MODE2_2336;
		} else if( type == "MODE2/2352" ) {
			dataType = MODE2_2352;
		} else if( type == "CDI/2336" ) {
			dataType = CDI_2336;
		} else if( type == "CDI/2352" ) {
			dataType = CDI_2352;
		} else {
			dataType = AUDIO;
		}
	}
	
	Track::Track( unsigned short int trackNO, DataType type ) : number( trackNO ), dataType( type ), preGap( 0, 0, 0 ), postGap( 0, 0, 0 ) {
	}
	
	void Track::addFlag( const std::string & f ) {
		if( f == "DATA" ) {
			addFlag( DATA );
		} else if( f == "DCP" ) {
			addFlag( DCP );
		} else if( f == "4CH" ) {
			addFlag( CH4 );
		} else if( f == "PRE" ) {
			addFlag( PRE );
		} else if( f == "SCMS" ) {
			addFlag( SCMS );
		}
	}
	
	void Track::addFlag( Flag f ) {
		flags.insert( f );
	}
	
}
