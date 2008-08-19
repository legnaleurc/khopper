/**
 * @file output.hpp
 */
#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <string>
#include <QWidget>

namespace Khopper {

	/**
	 * @brief Output file format interface
	 */
	class Output {
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
			virtual ~Output() = 0;
	};
	
	/**
	 * @namespace Khopper::out
	 * @brief Namespace of output file
	 *
	 * Avoid conflict with input format.
	 */
	
}

#endif
