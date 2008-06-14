#ifndef APE_HPP
#define APE_HPP

#include "input.hpp"
#include "type.hpp"

/**
 * @namespace Khopper
 */
namespace Khopper {

	namespace in {
		
		/**
		* @class Ape
		*/
		class Ape : public Input {
			virtual std::string getOption() const;
			virtual QWidget * getUI() const;
		};
	
	}
	
}

#endif
