#include "Threads.hpp"

namespace Khopper {
	
	ConverterThread::ConverterThread( QObject * parent ) : QThread( parent ) {
	}
	
	void ConverterThread::setSheet( const CUESheet & sheet ) {
		sheet_ = sheet;
	}
	
	void ConverterThread::setIndex( const std::vector< int > & index ) {
		index_ = index;
	}
	
	void ConverterThread::setOutput( const OutputSP & output ) {
		output_ = output;
	}
	
	void ConverterThread::run() {
		// convert
		try {
			for( std::size_t i = 0; i < index_.size(); ++i ) {
				InputSP input = InputFactory::Instance().CreateObject( QFileInfo( QString::fromStdString( sheet_[index_[i]].dataFile.name ) ).suffix().toStdString() );
				Converter conv( input, output_ );
				conv.perform( sheet_[index_[i]], sheet_.getPath() );
				emit step( i );
			}
		} catch( const Error< RunTime > & e ) {
			emit error( tr( e.what() ) );
		}
	}
	
}
