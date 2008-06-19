#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include "os.hpp"
#include "input.hpp"
#include "output.hpp"

#include <vector>
#include <string>
#include <sstream>

#include <cstdlib>

/**
 * @namespace Khopper
 */
namespace Khopper {

	/**
	 * @class Converter
	 */
	class Converter {
		public:
			/**
			 * @brief You can only use this version
			 * @param [in] iop Input option
			 * @param [in] oop Output option
			 */
			Converter( Input * iop, Output * oop );
			
			/**
			 * @brief Convert action
			 * @param [in] audio Audio file path
			 * @param [in] sheet CUE sheet file path
			 * @param [in] songList the tracks to be convert
			 * @return message
			 * @throw Error<Runtime>
			 */
			std::string perform( const std::string & audio, const std::string & sheet, const std::vector< int > & songList ) const;
			
			virtual ~Converter();
		private:
			static const char * const COMMAND;
			
			Input * _input_;
			Output * _output_;
			
			// Protection
			Converter();
			Converter( const Converter & );
			Converter & operator =( const Converter & );
	};

}

#endif
