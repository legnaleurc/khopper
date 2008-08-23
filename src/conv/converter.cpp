#include "conv/converter.hpp"
#include "util/os.hpp"
#include "cue/track.hpp"
#include "util/error.hpp"

#include <vector>

namespace Khopper {
	
	const char * const Converter::COMMAND = "shnsplit";
	
	Converter::Converter( OutputSP oop ) : output_( oop ) {
		os::exists( COMMAND );
	}
	
	std::string Converter::perform( const Track & track, InputSP iop ) const {
		std::vector< std::string > args;
		
		args.push_back( COMMAND );
		args.push_back( track.getAudioData().getFilePath() );
		args.push_back( "-x" );
		args.push_back( "2" );
		args.push_back( "-O" );
		args.push_back( "always" );
		
		if( iop != NULL && iop->getOption() != "" ) {
			args.push_back( "-i" );
			args.push_back( iop->getOption() );
		}
		
		if( output_ != NULL && output_->getOption() != "" ) {
			args.push_back( "-o" );
			args.push_back( output_->getOption() );
		}
		
		std::pair< std::string, std::string > msg;
		try {
			if( os::getResult( msg, args, track.getIndicesString() ) != 0 ) {
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
