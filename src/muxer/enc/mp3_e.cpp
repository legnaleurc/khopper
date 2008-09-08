#include "mp3_e.hpp"
#include "os.hpp"

namespace {

	Khopper::Encoder * CreateMp3() {
		return new Khopper::enc::Mp3;
	}
	
	const char * const MP3 = "mp3";
	
	const bool reg_type = Khopper::EncoderList::Instance().insert( Khopper::EncoderList::ObjectType::value_type( MP3, "mp3" ) ).second;
	const bool reg_fac = Khopper::EncoderFactory::Instance().Register( MP3, CreateMp3 );

}

namespace Khopper {
	
	namespace enc {
		
		std::string Mp3::getOption() const {
			os::exists( "lame" );
			return "cust ext=mp3 lame --quiet - %f";
		}
		
		QWidget * Mp3::getUI() const {
			return NULL;
		}
		
		Mp3::~Mp3() {
		}
		
	}
	
}
