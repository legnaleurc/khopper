/**
 * @file converter.hpp
 */
#ifndef KHOPPER_MUXER_CONVERTER_HPP
#define KHOPPER_MUXER_CONVERTER_HPP

#include "decoder.hpp"
#include "encoder.hpp"

#include <QString>

namespace Khopper {

	class Track;

	/**
	 * @brief The audio converter
	 */
	class Converter {
		public:
			/**
			 * @brief Default constructor
			 */
			Converter();

			/**
			 * @brief You can only use this version
			 * @param [in] oop Encoder option
			 * @param [in] outDir Output directory
			 */
			Converter( EncoderSP oop, const QString & outDir );

			/**
			 * @brief Convert action
			 * @param [in] track Track information
			 * @param [in] iop Decoder option
			 * @return message
			 * @throw Error<Runtime> Basic
			 */
			QString perform( const Track & track, DecoderSP iop ) const;
		private:
			EncoderSP output_;
			QString outDir_;
			
			// Protection
			Converter( const Converter & );
			Converter & operator =( const Converter & );
	};

}

#endif
