#include "cue/audiofile.hpp"

namespace Khopper {
	
	AudioFile::AudioFile( const std::string & fileName, const std::string & fileType ) : name( fileName ) {
		if( fileType == "BINARY" ) {
			type = BINARY;
		} else if( fileType == "MOTOROLA" ) {
			type = MOTOROLA;
		} else if( fileType == "AIFF" ) {
			type = AIFF;
		} else if( fileType == "WAVE" ) {
			type = WAVE;
		} else if( fileType == "MP3" ) {
			type = MP3;
		} else {
			type = BINARY;
		}
	}
	
	AudioFile::AudioFile() {
	}
	
	AudioFile::AudioFile( const std::string & fileName, FileType fileType ) : name( fileName ), type( fileType ) {
	}
	
}
