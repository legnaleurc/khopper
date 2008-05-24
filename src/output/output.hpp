#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <QWidget>
#include <string>

namespace Khopper {
	
	class Output {
		public:
			virtual std::string getOption() const = 0;
			virtual QWidget * getUI() const = 0;
	};
	
}

#endif
