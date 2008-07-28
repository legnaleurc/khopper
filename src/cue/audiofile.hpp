#ifndef AUDIOFILE_HPP
#define AUDIOFILE_HPP

#include <string>

namespace Khopper {
	
	struct AudioFile {
		enum FileType {
			BINARY,
			MOTOROLA,
			AIFF,
			WAVE,
			MP3
		};
		
		AudioFile();
		AudioFile( const std::string & fileName, const std::string & fileType );
		AudioFile( const std::string & fileName, FileType fileType );
		std::string name;
		FileType type;
	};
}

#endif
