#include "ape.hpp"

namespace {
	
	Input * CreateApe() {
		return new Ape;
	}
	
	const std::string APE( "ape" );
	
	const bool registered = TheFactory::Instance().RegisterShape( APE, CreateApe );
	
}

namespace Khopper {
	
	std::string Ape::getOption() const {
		return "";
	}
	
}
