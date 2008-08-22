#include "cue/audiofile.hpp"
#include <boost/regex.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <sstream>

namespace Khopper {
	
	AudioFile::AudioFile( const std::string & filePath, const std::string & fileType ) : path_( filePath ) {
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
	
	AudioFile::AudioFile( const std::string & filePath, FileType fileType ) : path_( filePath ), type_( fileType ) {
		boost::smatch result;
		if( boost::regex_match( path_, result, boost::regex( "*.\\.(*.)" ) ) ) {
			format_ = result[1];
			boost::algorithm::to_lower( format_ );
		}
	}

	const std::string & AudioFile::getFilePath() const {
		return path_;
	}

	const std::string & AudioFile::getFormat() const {
		return format_;
	}
	
	std::string AudioFile::toString() const {
		std::ostringstream sout;
		sout << "File Path:\t" << path_ << "\n";
		sout << "FileType:\t" << type_ << "\n";
		return sout.str();
	}
	
}
