#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <QWidget>
#include <string>

/**
 * @namespace Khopper
 */
namespace Khopper {

	/**
	 * @class Output
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
	};
	
}

#endif
