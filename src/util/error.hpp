/**
 * @file error.hpp
 */
#ifndef ERROR_HPP
#define ERROR_HPP

#include <exception>
#include <string>

namespace Khopper {

	/**
	 * @brief Prototype of all errors
	 * @tparam Type Custom error extension
	 */
	template< typename Type >
	class Error : public std::exception, public Type {
		public:
			/**
			 * @brief Constructor that set error message
			 * @param [in] msg Error message
			 * @throw NEVER
			 */
			Error( const std::string & msg ) throw() : msg_( msg ) {}
			/**
			 * @brief Get the error message
			 * @return Error message
			 * @throw NEVER
			 */
			virtual const char * what() const throw() {
				return msg_.c_str();
			}
			/**
			 * @brief Dummy destructor
			 * @throw NEVER
			 */
			virtual ~Error() throw() {}
		private:
			std::string msg_;
	};
	
	/**
	 * @brief Dummy error class
	 */
	class OS {
	};
	
	/**
	 * @brief Dummy error class
	 */
	class RunTime {
	};
	
	class Parsing {
	};
	
	class System {
	};

}

#endif
