/**
 * @file ape.hpp
 */
#ifndef APE_HPP
#define APE_HPP

#include "input.hpp"

namespace Khopper {

	namespace in {
		
		/**
		 * @brief APE input
		 */
		class Ape : public Input {
			virtual std::string getOption() const;
			
			virtual QWidget * getUI() const;
			
			virtual ~Ape();
		};
	
	}
	
}

#endif
