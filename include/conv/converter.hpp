/**
 * @file converter.hpp
 */
#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include "types.hpp"

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
			 * @param [in] oop Output option
			 * @param [in] outDir Output directory
			 */
			Converter( OutputSP oop, const std::string & outDir );

			/**
			 * @brief Convert action
			 * @param [in] track Track information
			 * @param [in] iop Input option
			 * @return message
			 * @throw Error<Runtime> Basic
			 */
			std::string perform( const Track & track, InputSP iop ) const;
		private:
			static const char * const COMMAND;
			
			OutputSP output_;
			std::string outDir_;
			
			// Protection
			Converter( const Converter & );
			Converter & operator =( const Converter & );
	};

}

#endif
