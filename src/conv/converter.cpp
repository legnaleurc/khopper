#include "converter.hpp"

namespace Khopper {
	
	const char * const Converter::COMMAND = "shnsplit";
	
	Converter::Converter( const InputSP & iop, const OutputSP & oop ) : input_( iop ), output_( oop ) {
		os::exists( COMMAND );
	}
	
	std::string Converter::perform( const std::string & audio, const std::string & sheet, const std::vector< int > & songList ) const {
		std::vector< std::string > args;
		
		args.push_back( COMMAND );
		args.push_back( audio );
		args.push_back( "-f" );
		args.push_back( sheet );
		
		if( !songList.empty() ) {
			std::ostringstream sout;
			
			sout << songList[0];
			for( std::size_t i = 1; i < songList.size(); ++i ) {
				sout << ',' << songList[i];
			}
		
			args.push_back( "-x" );
			args.push_back( sout.str() );
		}
		
		if( input_ != NULL && input_->getOption() != "" ) {
			args.push_back( "-i" );
			args.push_back( input_->getOption() );
		}
		
		if( output_ != NULL && output_->getOption() != "" ) {
			args.push_back( "-o" );
			args.push_back( output_->getOption() );
		}
		
		std::pair< std::string, std::string > msg;
		if( os::getResult( msg, args ) != 0 ) {
			throw Error< RunTime >( msg.second );
		} else {
			return msg.first;
		}
	}
	
}
