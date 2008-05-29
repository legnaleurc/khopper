#ifndef INPUT_HPP
#define INPUT_HPP

#include <QWidget>
#include <string>

/**
 * @namespace Khopper
 */
namespace Khopper {

	/**
	 * @class Input
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
	};
	
}

#endif
