/**
 * @file mp3.hpp
 */
#ifndef MP3_HPP
#define MP3_HPP

#include "output.hpp"
#include "type.hpp"

namespace Khopper {
	
	namespace out {
		
		/**
		 * @brief MP3 output
		 */
		class Mp3 : public Output {
			virtual std::string getOption() const;
			
			virtual QWidget * getUI() const;
			
			virtual ~Mp3();
		};
		
	}
	
}

#endif
