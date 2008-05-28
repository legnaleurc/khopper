#include "converter.hpp"

namespace Khopper {
	
	const char * const Converter::Command = "shnsplit";
	
	Converter::Converter( const std::string & audioFile, const std::string & cuesheet, const std::string & iop, const std::string & oop ) : _audio_( audioFile ), _sheet_( cuesheet ), _inputOption_( iop ), _outputOption_( oop ) {
	}
	
	std::string Converter::perfrom( const std::vector< int > & songList ) const {
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
		if( os::exeRes( msg, args ) != 0 ) {
			throw Error< Runtime >( msg.second );
		} else {
			return msg.first;
		}
	}
	
}
