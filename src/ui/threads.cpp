#include "threads.hpp"
#include "track.hpp"
#include "decoder.hpp"
#include "os.hpp"
#include <QFileInfo>

namespace Khopper {

	ConverterThread::ConverterThread( QObject * parent ) : QThread( parent ) {
	}

	void ConverterThread::setOutput( EncoderSP output, const QString & outDir ) {
		this->encoder_ = output;
		this->outDir_ = outDir;
	}

	void ConverterThread::setTracks( const std::vector< Track > & tracks ) {
		this->tracks_ = tracks;
	}

	void ConverterThread::run() {
		// convert
		try {
			for( std::size_t i = 0; i < tracks_.size(); ++i ) {
				this->tracks_[i].convert( this->outDir_.toStdWString(), this->encoder_ );
				emit step( i );
			}
		} catch( std::exception & e ) {
			emit error( tr( e.what() ) );
		}
	}

}
