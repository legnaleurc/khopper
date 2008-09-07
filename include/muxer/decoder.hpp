/**
 * @file decoder.hpp
 */
#ifndef KHOPPER_MUXER_DECODER_HPP
#define KHOPPER_MUXER_DECODER_HPP

#include "tr1.hpp"

#include <loki/Factory.h>
#include <loki/Singleton.h>

#include <QWidget>

#include <string>

namespace Khopper {

	/**
	 * @brief Decoder interface
	 */
	class Decoder {
		public:
			/**
			 * @brief Get option information
			 * @return It is a string
			 */
			virtual std::string getOption() const = 0;
			
			/**
			 * @brief Type specified UI
			 * @return A GUI object
			 */
			virtual QWidget * getUI() const = 0;
			
			/**
			 * @brief Pure virtual destructor
			 */
			virtual ~Decoder() = 0;
	};

	/**
	 * @brief Decoder smart pointer
	 *
	 * Use TR1 shared pointer.
	 */
	typedef std::tr1::shared_ptr< Decoder > DecoderSP;

	/**
	 * @brief The decoder Object Factory for Converter module
	 */
	typedef Loki::SingletonHolder< Loki::Factory< Decoder, std::string >, Loki::CreateUsingNew, Loki::LongevityLifetime::DieAsSmallObjectChild > DecoderFactory;

	/**
	 * @namespace Khopper::dec
	 * @brief Namespace of decoder
	 *
	 * Avoid conflict with encoder.
	 */
	
}

#endif
