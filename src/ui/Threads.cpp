#include "Threads.hpp"

namespace Khopper {
	
	ConverterThread::ConverterThread( QObject * parent ) : QThread( parent ) {
	}
	
	void ConverterThread::setAudio( const std::string & audio ) {
		audio_ = audio;
	}
	
	void ConverterThread::setSheet( const std::string & sheet ) {
		sheet_ = sheet;
	}
	
	void ConverterThread::setIndex( const std::vector< int > & index ) {
		index_ = index;
	}
	
	void ConverterThread::setInput( const InputSP & input ) {
		input_ = input;
	}
	
	void ConverterThread::setOutput( const OutputSP & output ) {
		output_ = output;
	}
	
	void ConverterThread::run() {
		// convert
		Converter conv( input_, output_ );
		conv.perform( audio_, sheet_, index_ );
	}
	
}
