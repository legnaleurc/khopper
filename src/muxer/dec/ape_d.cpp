#include "ape_d.hpp"
#include "os.hpp"

namespace {
	
	Khopper::Decoder * CreateApe() {
		return new Khopper::dec::Ape;
	}
	
	const char * const APE = "ape";
	
	const bool reg_fac = Khopper::DecoderFactory::Instance().Register( APE, CreateApe );
	
}

namespace Khopper {
	
	namespace dec {
		
		std::string Ape::getOption() const {
			os::exists( "mac" );
			return "";
		}
		
		QWidget * Ape::getUI() const {
			return NULL;
		}
		
		Ape::~Ape() {
		}
		
	}
	
}
