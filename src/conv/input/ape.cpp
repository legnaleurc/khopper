#include "ape.hpp"

namespace {
	
	Khopper::Input * CreateApe() {
		return new Khopper::in::Ape;
	}
	
	const std::string APE( "in::Ape" );
	
	const bool reg_type = Khopper::IOTypes::Instance().first.insert( Khopper::IOTypes::ObjectType::first_type::value_type( APE, "ape" ) ).second;
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
