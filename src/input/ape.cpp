#include "ape.hpp"

namespace {
	
	Input * CreateApe() {
		return new Ape;
	}
	
	// ID, encode in base64
	const std::string APE( "QVBFCg==" );
	
	const bool registered = TheFactory::Instance().RegisterShape( APE, CreateApe );
	
}

namespace Khopper {
	
	std::string Ape::getOption() const {
		return "";
	}
	
}
