/**
 * @file mp3_e.hpp
 */
#ifndef KHOPPER_MUXER_ENCODER_MP3_E_HPP
#define KHOPPER_MUXER_ENCODER_MP3_E_HPP

#include "encoder.hpp"

namespace Khopper {
	
	namespace enc {
		
		/**
		 * @brief MP3 encoder
		 */
		class Mp3 : public Encoder {
			virtual std::string getOption() const;
			
			virtual QWidget * getUI() const;
			
			virtual ~Mp3();
		};
		
	}
	
}

#endif
