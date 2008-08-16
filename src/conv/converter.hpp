/**
 * @file converter.hpp
 */
#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include "os.hpp"
#include "input.hpp"
#include "output.hpp"
#include "type.hpp"
#include "track.hpp"

#include <boost/lexical_cast.hpp>

#include <vector>
#include <string>
#include <sstream>

#include <cstdlib>

namespace Khopper {

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
			Converter( const InputSP & iop, const OutputSP & oop );
			
			/**
			 * @brief Convert action
			 * @param [in] track Track information
			 * @param [in] sheet CUE sheet file path
			 * @return message
			 * @throw Error<Runtime>
			 */
			std::string perform( const Track & track, const std::string & sheet ) const;
		private:
			static const char * const COMMAND;
			
			InputSP input_;
			OutputSP output_;
			
			// Protection
			Converter( const Converter & );
			Converter & operator =( const Converter & );
	};

}

#endif
