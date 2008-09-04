#include "conv/converter.hpp"
#include "conv/input/input.hpp"
#include "conv/output/output.hpp"
#include "util/os.hpp"
#include "cue/track.hpp"
#include "util/error.hpp"

namespace Khopper {
	
	const char * const Converter::COMMAND = "shnsplit";
	
	Converter::Converter( OutputSP oop, const std::string & outDir ) : output_( oop ), outDir_( outDir ) {
		os::exists( COMMAND );
	}
	
	std::string Converter::perform( const Track & track, InputSP iop ) const {
		std::vector< std::string > args;
		
		args.push_back( COMMAND );
		args.push_back( track.getAudioData().getFilePath().toUtf8().constData() );
		args.push_back( "-x" );
		// Dirty hack
		if( track.getNumber() == 1 ) {
			args.push_back( "1" );
		} else {
			args.push_back( "2" );
		}
		args.push_back( "-t" );
		args.push_back( "%n" );
		args.push_back( "-d" );
		args.push_back( outDir_ );
		args.push_back( "-z" );
		args.push_back( track.getTitle().toUtf8().constData() );
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
			if( os::getResult( msg, args, track.getIndicesString().toUtf8().constData() ) != 0 ) {
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
