#ifndef ERROR_HPP
#define ERROR_HPP

#include <exception>
#include <string>

namespace Khopper {

	template< typename Type >
	class Error : public std::exception, public Type {
		public:
			Error( const std::string & msg ) throw() : _msg_( msg ) {}
			virtual const char * what() throw() {
				return _msg_.c_str();
			}
			virtual ~Error() throw();
		private:
			std::string _msg_;
	};
	
	class OS {
	};
	
	class Runtime {
	};

}

#endif
