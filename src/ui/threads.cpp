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
	
	void ConverterThread::setOutput( const OutputSP & output ) {
		converter_.reset( new Converter( output ) );
	}
	
	void ConverterThread::run() {
		// convert
		try {
			for( std::size_t i = 0; i < tracks_.size(); ++i ) {
				InputSP input = InputFactory::Instance().CreateObject( tracks_[i].getAudioData().getFormat() );
				converter_->perform( tracks_[i], input );
				emit step( i );
			}
		} catch( std::exception & e ) {
			emit error( tr( e.what() ) );
		}
	}
	
}
