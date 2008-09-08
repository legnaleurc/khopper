#include "wav_d.hpp"

namespace {
	
	Khopper::Decoder * CreateWav() {
		return new Khopper::dec::Wav;
	}
	
	const char * const WAV = "wav";
	
	const bool reg_fac = Khopper::DecoderFactory::Instance().Register( WAV, CreateWav );
	
}

namespace Khopper {
	
	namespace dec {
		
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
