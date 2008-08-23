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
			 * @param [in] iop Input option
			 * @param [in] oop Output option
			 */
			Converter( OutputSP oop );

			/**
			 * @brief Convert action
			 * @param [in] track Track information
			 * @param [in] path Audio file path
			 * @return message
			 * @throw Error<Runtime>
			 */
			std::string perform( const Track & track, InputSP iop ) const;
		private:
			static const char * const COMMAND;
			
			OutputSP output_;
			
			// Protection
			Converter( const Converter & );
			Converter & operator =( const Converter & );
	};

}

#endif
