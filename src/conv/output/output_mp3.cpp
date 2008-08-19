#include "mp3.hpp"
#include "type.hpp"
#include "os.hpp"

namespace {

	Khopper::Output * CreateMp3() {
		return new Khopper::out::Mp3;
	}
	
	const char * const MP3 = "mp3";
	
	const bool reg_type = Khopper::IOTypes::Instance().second.insert( Khopper::OutputList::value_type( MP3, "mp3" ) ).second;
	const bool reg_fac = Khopper::OutputFactory::Instance().Register( MP3, CreateMp3 );

}

namespace Khopper {
	
	namespace out {
		
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
