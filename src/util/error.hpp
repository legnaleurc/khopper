#ifndef ERROR_HPP
#define ERROR_HPP

#include <exception>
#include <string>

/**
 * @namespace Khopper
 */
namespace Khopper {

	/**
	 * @class Error
	 * @brief Prototype of all errors
	 */
	template< typename Type >
	class Error : public std::exception, public Type {
		public:
			/**
			 * @brief Constructor that set error message
			 * @param [in] msg Error message
			 * @throw NEVER
			 */
			Error( const std::string & msg ) throw() : _msg_( msg ) {}
			/**
			 * @brief Get the error message
			 * @return Error message
			 * @throw NEVER
			 */
			virtual const char * what() throw() {
				return _msg_.c_str();
			}
			/**
			 * @brief Dummy destructor
			 * @throw NEVER
			 */
			virtual ~Error() throw() {}
		private:
			std::string _msg_;
	};
	
	/**
	 * @class OS
	 * @brief Dummy error class
	 */
	class OS {
	};
	
	/**
	 * @class Runtime
	 * @brief Dummy error class
	 */
	class Runtime {
	};

}

#endif
