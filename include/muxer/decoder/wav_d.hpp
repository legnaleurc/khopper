/**
 * @file wav_d.hpp
 */
#ifndef KHOPPER_MUXER_DECODER_WAV_HPP
#define KHOPPER_MUXER_DECODER_WAV_HPP

#include "decoder.hpp"

namespace Khopper {

	namespace dec {
		
		/**
		 * @brief WAV decoder
		 */
		class Wav : public Decoder {
			virtual std::string getOption() const;
			
			virtual QWidget * getUI() const;
			
			virtual ~Wav();
		};
	
	}
	
}

#endif
