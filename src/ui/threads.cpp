#include "threads.hpp"
#include "track.hpp"
#include "converter.hpp"
#include "decoder.hpp"
#include "os.hpp"
#include <QFileInfo>

namespace Khopper {
	
	ConverterThread::ConverterThread( QObject * parent ) : QThread( parent ) {
	}
	
	void ConverterThread::setOutput( EncoderSP output, const QString & outDir ) {
		converter_.reset( new Converter( output, outDir.toUtf8().constData() ) );
	}
	
	void ConverterThread::setTracks( const std::vector< Track > & tracks ) {
		tracks_ = tracks;
	}
	
	void ConverterThread::run() {
		// convert
		try {
			for( std::size_t i = 0; i < tracks_.size(); ++i ) {
				DecoderSP input( DecoderFactory::Instance().CreateObject( tracks_[i].getAudioData().getFormat().toUtf8().constData() ) );
				converter_->perform( tracks_[i], input );
				emit step( i );
			}
		} catch( std::exception & e ) {
			emit error( tr( e.what() ) );
		}
	}
	
}
