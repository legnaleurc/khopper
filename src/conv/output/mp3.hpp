#ifndef MP3_HPP
#define MP3_HPP

#include "output.hpp"
#include "type.hpp"

namespace Khopper {
	
	namespace out {
		
		class Mp3 : public Output {
			virtual std::string getOption() const;
			virtual QWidget * getUI() const;
		};
		
	}
	
}

#endif
