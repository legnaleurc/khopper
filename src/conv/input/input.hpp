/**
 * @file input.hpp
 */
#ifndef INPUT_HPP
#define INPUT_HPP

#include <QWidget>
#include <string>

namespace Khopper {

	/**
	 * @brief Input file format interface
	 */
	class Input {
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
			virtual ~Input() = 0;
	};
	
	/**
	 * @namespace Khopper::in
	 * @brief Namespace of input file
	 *
	 * Avoid conflict with output format.
	 */
	
}

#endif
