/**
 * @file error.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KHOPPER_UTIL_ERROR_HPP
#define KHOPPER_UTIL_ERROR_HPP

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
		 */
		Error( const std::string & msg ) throw() : msg_( msg ) {}
		/**
		 * @brief Get the error message
		 * @return Error message
		 */
		virtual const char * what() const throw() {
			return msg_.c_str();
		}
		/**
		 * @brief Dummy destructor
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

	/**
	 * @brief Dummy error class
	 */
	class Parsing {
	};

	/**
	 * @brief Dummy error class
	 */
	class System {
	};

}

#endif
