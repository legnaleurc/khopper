#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include "os.hpp"

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
			 * @param [in] audioFile Audio file path
			 * @param [in] cuesheet CUE sheet file path
			 * @param [in] iop Input option
			 * @param [in] oop Output option
			 */
			Converter( const std::string & audioFile, const std::string & cuesheet, const std::string & iop, const std::string & oop );
			
			/**
			 * @brief Convert action
			 * @param songList the tracks to be convert
			 * @return message
			 * @throw Error<Runtime>
			 */
			std::string perfrom( const std::vector< int > & songList ) const;
		private:
			static const char * const Command;
			
			std::string _audio_;
			std::string _sheet_;
			std::string _inputOption_;
			std::string _outputOption_;
			
			// Protection
			Converter();
	};

}

#endif
