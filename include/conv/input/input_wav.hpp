/**
 * @file input_wav.hpp
 */
#ifndef WAV_HPP
#define WAV_HPP

#include "input.hpp"

namespace Khopper {

	namespace in {
		
		/**
		 * @brief WAV input
		 */
		class Wav : public Input {
			virtual std::string getOption() const;
			
			virtual QWidget * getUI() const;
			
			virtual ~Wav();
		};
	
	}
	
}

#endif
