#include "Threads.hpp"

namespace Khopper {
	
	ConverterThread::ConverterThread( const Converter & conv, const std::string & audio, const std::string & sheet, const std::vector< int > & index, QObject * parent ) : QThread( parent ), _conv_( conv ), _audio_( audio ), _sheet_( sheet ), _index_( index ) {
	}
	
	void ConverterThread::run() {
		// convert
		_conv_.perform( _audio_, _sheet_, _index_ );
		exec();
	}
	
	ProgressThread::ProgressThread( QObject * parent ) : QThread( parent ) {
	}
	
	void ProgressThread::run() {
		// show progress
		QProgressDialog progress( tr( "Converting..." ), tr( "Don\'t touch!" ), 0, 0 );
		exec();
	}
	
}
