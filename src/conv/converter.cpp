#include "conv/converter.hpp"
#include "util/os.hpp"
#include "cue/track.hpp"
#include "util/error.hpp"

#include <vector>
#include <boost/lexical_cast.hpp>

namespace Khopper {
	
	const char * const Converter::COMMAND = "shnsplit";
	
	Converter::Converter( InputSP iop, OutputSP oop ) : input_( iop ), output_( oop ) {
		os::exists( COMMAND );
	}
	
	std::string Converter::perform( const Track & track, const std::string & sheet ) const {
		std::vector< std::string > args;
		
		args.push_back( COMMAND );
		args.push_back( track.dataFile.name );
		args.push_back( "-f" );
		args.push_back( sheet );
		args.push_back( "-O" );
		args.push_back( "always" );
		
		args.push_back( "-x" );
		args.push_back( boost::lexical_cast< std::string >( track.number ) );
		
		if( input_ != NULL && input_->getOption() != "" ) {
			args.push_back( "-i" );
			args.push_back( input_->getOption() );
		}
		
		if( output_ != NULL && output_->getOption() != "" ) {
			args.push_back( "-o" );
			args.push_back( output_->getOption() );
		}
		
		std::pair< std::string, std::string > msg;
		try {
			if( os::getResult( msg, args ) != 0 ) {
				// FIXME: Should delete junk
				throw Error< RunTime >( msg.second );
			} else {
				return msg.first;
			}
		} catch ( const Error< OS > & e ) {
			throw Error< RunTime >( e.what() );
		}
	}
	
}
