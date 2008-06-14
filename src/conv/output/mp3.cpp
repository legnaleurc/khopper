#include "mp3.hpp"

namespace {

	Khopper::Output * CreateMp3() {
		return new Khopper::out::Mp3;
	}
	
	const std::string MP3( "out::Mp3" );
	
	const bool reg_type = Khopper::IOTypes::Instance().second.insert( Khopper::IOTypes::ObjectType::second_type::value_type( MP3, "mp3" ) ).second;
	const bool reg_fac = Khopper::OutputFactory::Instance().Register( MP3, CreateMp3 );

}

namespace Khopper {
	
	namespace out {
		
		std::string Mp3::getOption() const {
			return "cust ext=mp3 lame --quiet - %f";
		}
		
		QWidget * Mp3::getUI() const {
			return NULL;
		}
		
	}
	
}
