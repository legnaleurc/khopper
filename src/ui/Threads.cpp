#include "Threads.hpp"

namespace Khopper {
	
	ConverterThread::ConverterThread( QObject * parent ) : QThread( parent ) {
	}
	
	void ConverterThread::setAudio( const std::string & audio ) {
		_audio_ = audio;
	}
	
	void ConverterThread::setSheet( const std::string & sheet ) {
		_sheet_ = sheet;
	}
	
	void ConverterThread::setIndex( const std::vector< int > & index ) {
		_index_ = index;
	}
	
	void ConverterThread::setInput( const InputSP & input ) {
		_input_ = input;
	}
	
	void ConverterThread::setOutput( const OutputSP & output ) {
		_output_ = output;
	}
	
	void ConverterThread::run() {
		// convert
		Converter conv( _input_, _output_ );
		conv.perform( _audio_, _sheet_, _index_ );
	}
	
}
