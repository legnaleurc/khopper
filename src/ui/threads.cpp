#include "ui/threads.hpp"
#include "cue/track.hpp"
#include "conv/converter.hpp"
#include "types.hpp"
#include "util/os.hpp"
#include <QFileInfo>

namespace Khopper {
	
	ConverterThread::ConverterThread( QObject * parent ) : QThread( parent ) {
	}
	
	void ConverterThread::setTracks( const std::vector< Track > & tracks ) {
		tracks_ = tracks;
	}
	
	void ConverterThread::setPaths( const std::vector< QString > & paths ) {
		paths_ = paths;
	}
	
	void ConverterThread::setOutput( const OutputSP & output ) {
		output_ = output;
	}
	
	void ConverterThread::run() {
		// convert
		try {
			for( std::size_t i = 0; i < tracks_.size(); ++i ) {
				InputSP input = InputFactory::Instance().CreateObject( tracks_[i].getAudioData().getFormat() );
				Converter conv( input, output_ );
				conv.perform( tracks_[i] );
				emit step( i );
			}
		} catch( Error< RunTime > & e ) {
			emit error( tr( e.what() ) );
		} catch( Loki::DefaultFactoryError< std::string, Input >::Exception & e ) {
			emit error( tr( e.what() ) );
		}
	}
	
}
