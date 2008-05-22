#ifndef CONVERT_HPP
#define CONVERT_HPP

#include <sstream>

#include <cstdlib>

namespace Khopper {

	template< class Input, class Output >
	class Convert : public Input, public Output {
		public:
			void setCUESheet( const std::string & cuesheet ) {
				_sheet_ = cuesheet;
			}
			
			void perfrom() const {
				std::ostringstream sout( cmd );
				
				sout << " -f \"" << _sheet_ << "\"";
				sout << " -i \"" << Input::getOption() << "\"";
				sout << " -o \"" << Output::getOption() << "\"";
				
				if( std::system( sout.c_str() ) != 0 ) {
					throw Error;
				}
			}
		private:
			static const char * const cmd = "shnsplit";
			std::string _sheet_;
	};

}

#endif
