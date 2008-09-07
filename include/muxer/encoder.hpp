/**
 * @file encoder.hpp
 */
#ifndef KHOPPER_MUXER_ENCODER_HPP
#define KHOPPER_MUXER_ENCODER_HPP

#include "tr1.hpp"

#include <loki/Factory.h>
#include <loki/Singleton.h>
#include <loki/AssocVector.h>

#include <QWidget>

#include <string>

namespace Khopper {

	/**
	 * @brief Encoder interface
	 */
	class Encoder {
		public:
			/**
			 * @brief Get format specified options
			 * @return string option
			 */
			virtual std::string getOption() const = 0;
			
			/**
			 * @brief Get Type GUI
			 * @return GUI object
			 */
			virtual QWidget * getUI() const = 0;
			
			/**
			 * @brief Pure virtual destructor
			 */
			virtual ~Encoder() = 0;
	};

	/**
	 * @brief Encoder smart pointer
	 *
	 * Use TR1 shared pointer.
	 */
	typedef std::tr1::shared_ptr< Encoder > EncoderSP;

	/**
	 * @brief The encoder Object Factory for Converter module
	 */
	typedef Loki::SingletonHolder< Loki::Factory< Encoder, std::string >, Loki::CreateUsingNew, Loki::LongevityLifetime::DieAsSmallObjectChild > EncoderFactory;

	/**
	 * @brief Encoder type list
	 */
	typedef Loki::SingletonHolder< Loki::AssocVector< std::string, std::string > > EncoderList;
	
	/**
	 * @namespace Khopper::enc
	 * @brief Namespace of encoder
	 *
	 * Avoid conflict with decoder.
	 */
	
}

#endif
