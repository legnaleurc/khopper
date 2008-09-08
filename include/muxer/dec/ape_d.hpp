/**
 * @file ape_d.hpp
 */
#ifndef KHOPPER_MUXER_DECODER_APE_HPP
#define KHOPPER_MUXER_DECODER_APE_HPP

#include "decoder.hpp"

namespace Khopper {

	namespace dec {
		
		/**
		 * @brief APE decoder
		 */
		class Ape : public Decoder {
			virtual std::string getOption() const;
			
			virtual QWidget * getUI() const;
			
			virtual ~Ape();
		};
	
	}
	
}

#endif
