#include "ape.hpp"

namespace {
	
	Khopper::Input * CreateApe() {
		return new Khopper::in::Ape;
	}
	
	const std::string APE( "in::Ape" );
	
	const bool reg_type = Khopper::InputTypes::Instance().insert( Khopper::InputTypes::ObjectType::value_type( APE, "ape" ) ).second;
	const bool reg_fac = Khopper::InputFactory::Instance().Register( APE, CreateApe );
	
}

namespace Khopper {
	
	namespace in {
		
		std::string Ape::getOption() const {
			return "";
		}
		
		QWidget * Ape::getUI() const {
			return NULL;
		}
		
	}
	
}
