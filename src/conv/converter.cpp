#include "converter.hpp"

#include <iostream>

namespace Khopper {
	
	const char * const Converter::Command = "shnsplit";
	
	Converter::Converter( Input * iop, Output * oop ) : _input_( iop ), _output_( oop ) {
	}
	
	Converter::~Converter() {
		if( _input_ != NULL ) {
			delete _input_;
		}
		if( _output_ != NULL ) {
			delete _output_;
		}
	}
	
	std::string Converter::perform( const std::string & audio, const std::string & sheet, const std::vector< int > & songList ) const {
		std::vector< std::string > args;
		
		args.push_back( Command );
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
		
		if( _input_ != NULL && _input_->getOption() != "" ) {
			args.push_back( "-i" );
			args.push_back( _input_->getOption() );
		}
		
		if( _output_ != NULL && _output_->getOption() != "" ) {
			args.push_back( "-o" );
			args.push_back( _output_->getOption() );
		}
		
		std::pair< std::string, std::string > msg;
		if( os::exeRes( msg, args ) != 0 ) {
			throw Error< Runtime >( msg.second );
		} else {
			return msg.first;
		}
	}
	
}
