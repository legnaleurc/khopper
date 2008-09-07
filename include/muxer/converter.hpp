/**
 * @file converter.hpp
 */
#ifndef KHOPPER_MUXER_CONVERTER_HPP
#define KHOPPER_MUXER_CONVERTER_HPP

#include "decoder.hpp"
#include "encoder.hpp"

#include <string>

namespace Khopper {

	class Track;

	/**
	 * @brief The audio converter
	 */
	class Converter {
		public:
			/**
			 * @brief You can only use this version
			 * @param [in] oop Encoder option
			 * @param [in] outDir Output directory
			 */
			Converter( EncoderSP oop, const std::string & outDir );

			/**
			 * @brief Convert action
			 * @param [in] track Track information
			 * @param [in] iop Decoder option
			 * @return message
			 * @throw Error<Runtime> Basic
			 */
			std::string perform( const Track & track, DecoderSP iop ) const;
		private:
			static const char * const COMMAND;
			
			EncoderSP output_;
			std::string outDir_;
			
			// Protection
			Converter( const Converter & );
			Converter & operator =( const Converter & );
	};

}

#endif
