#ifndef CONVERT_HPP
#define CONVERT_HPP

#include <vector>

#include <cstdlib>

namespace Khopper {

	template< class OSPolicy >
	class base_converter : public OSPolicy {
		public:
			base_converter( const std::string & audioFile, const std::string & cuesheet, const std::string & iop, const std::string & oop ) : _audio_( audioFile ), _sheet_( cuesheet ), _inputOption_( iop ), _outputOption_( oop ) {}
			
			std::string perfrom() const {
				std::vector< std::string > args;
				
				args.push_back( Command );
				args.push_back( _audio_ );
				args.push_back( "-f" );
				args.push_back( _sheet_ );
				
				if( _inputOption_ != "" ) {
					args.push_back( "-i" );
					args.push_back( _inputOption_ );
				}
				
				if( _outputOption_ != "" ) {
					args.push_back( "-o" );
					args.push_back( _outputOption_ );
				}
				
				std::pair< std::string, std::string > msg;
				if( OSPolicy::exeRes( msg, args ) != 0 ) {
					throw Error< Runtime >( msg.second );
				} else {
					return msg.first;
				}
			}
		private:
			static const char * const Command = "shnsplit";
			
			std::string _audio_;
			std::string _sheet_;
			std::string _inputOption_;
			std::string _outputOption_;
			
			// Protection
			base_converter();
	};

}

#endif
