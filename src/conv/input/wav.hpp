#ifndef WAV_HPP
#define WAV_HPP

#include "input.hpp"
#include "type.hpp"

/**
 * @namespace Khopper
 */
namespace Khopper {

	namespace in {
		
		/**
		* @class Wav
		*/
		class Wav : public Input {
			virtual std::string getOption() const;
			virtual QWidget * getUI() const;
		};
	
	}
	
}

#endif
