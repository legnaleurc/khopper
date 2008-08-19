#include "wav.hpp"
#include "type.hpp"

namespace {
	
	Khopper::Input * CreateWav() {
		return new Khopper::in::Wav;
	}
	
	const char * const WAV = "wav";
	
	const bool reg_type = Khopper::IOTypes::Instance().first.insert( Khopper::InputList::value_type( WAV, "wav" ) ).second;
	const bool reg_fac = Khopper::InputFactory::Instance().Register( WAV, CreateWav );
	
}

namespace Khopper {
	
	namespace in {
		
		std::string Wav::getOption() const {
			return "";
		}
		
		QWidget * Wav::getUI() const {
			return NULL;
		}
		
		Wav::~Wav() {
		}
		
	}
	
}
