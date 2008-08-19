#include "ape.hpp"
#include "type.hpp"
#include "os.hpp"

namespace {
	
	Khopper::Input * CreateApe() {
		return new Khopper::in::Ape;
	}
	
	const char * const APE = "ape";
	
	const bool reg_type = Khopper::IOTypes::Instance().first.insert( Khopper::InputList::value_type( APE, "ape" ) ).second;
	const bool reg_fac = Khopper::InputFactory::Instance().Register( APE, CreateApe );
	
}

namespace Khopper {
	
	namespace in {
		
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
