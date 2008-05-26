#ifndef CONVERT_HPP
#define CONVERT_HPP

#include <vector>
#include <string>

#include <cstdlib>

/**
 * @namespace Khopper
 */
namespace Khopper {

	/**
	 * @class base_converter
	 */
	template< class OSPolicy >
	class base_converter : public OSPolicy {
		public:
			/**
			 * @brief You can only use this version
			 * @param [in] audioFile Audio file path
			 * @param [in] cuesheet CUE sheet file path
			 * @param [in] iop Input option
			 * @param [in] oop Output option
			 */
			base_converter( const std::string & audioFile, const std::string & cuesheet, const std::string & iop, const std::string & oop ) : _audio_( audioFile ), _sheet_( cuesheet ), _inputOption_( iop ), _outputOption_( oop ) {}
			
			/**
			 * @brief Convert action
			 * @param songList the tracks to be convert
			 * @return message
			 * @throw Error<Runtime>
			 */
			std::string perfrom( const std::vector< int > & songList ) const {
				std::vector< std::string > args;
				
				args.push_back( Command );
				args.push_back( _audio_ );
				args.push_back( "-f" );
				args.push_back( _sheet_ );
				
				if( !songList.empty() ) {
					std::ostringstream sout;
					
					sout << songList[0];
					for( std::size_t i = 1; i < songList.size(); ++i ) {
						sout << ',' << songList[i];
					}
					
					args.push_back( "-x" );
					args.push_back( sout.str() );
				}
				
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
