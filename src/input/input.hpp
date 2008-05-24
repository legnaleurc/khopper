#ifndef INPUT_HPP
#define INPUT_HPP

namespace Khopper {
	
	class Input {
		
		public:
			virtual std::string getOption() const = 0;
			
			virtual QWidget * getUI() const = 0;
		
	};
	
}

#endif