#include "cue/audiofile.hpp"
#include <sstream>

namespace Khopper {
	
	AudioFile::AudioFile( const std::string & fileName, const std::string & fileType ) : name_( fileName ) {
		if( fileType == "BINARY" ) {
			type_ = BINARY;
		} else if( fileType == "MOTOROLA" ) {
			type_ = MOTOROLA;
		} else if( fileType == "AIFF" ) {
			type_ = AIFF;
		} else if( fileType == "WAVE" ) {
			type_ = WAVE;
		} else if( fileType == "MP3" ) {
			type_ = MP3;
		} else {
			type_ = BINARY;
		}
	}
	
	AudioFile::AudioFile() {
	}
	
	AudioFile::AudioFile( const std::string & fileName, FileType fileType ) : name_( fileName ), type_( fileType ) {
	}

	const std::string & AudioFile::getFileName() const {
		return name_;
	}
	
	std::string AudioFile::toString() const {
		std::ostringstream sout;
		sout << "File Name:\t" << name_ << "\n";
		sout << "FileType:\t" << type_ << "\n";
		return sout.str();
	}
	
}
