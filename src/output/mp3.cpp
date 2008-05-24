#include "mp3.hpp"

namespace {
	
	Output * CreateMp3() {
		return new Mp3;
	}
	
	// ID, encode in base64
	const std::string MP3( "TVAzCg==" );
	
	const bool registered = TheFactory::Instance().Register( MP3, CreateMp3 );
	
}

namespace Khopper {
	
	std::string Mp3::getOption() const {
		return "cust ext=mp3 lame --quiet - %f";
	}
	
}
