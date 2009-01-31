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

	void ConverterThread::setTracks( const std::vector< TrackSP > & tracks ) {
		this->tracks_ = tracks;
		for( std::size_t i = 0; i < tracks.size(); ++i ) {
			connect( tracks[i].get(), SIGNAL( decodedTime( int ) ), this, SIGNAL( step( int ) ) );
		}
	}

	void ConverterThread::run() {
		// TODO: damn hack!
		QString nameHack( "%1/%2.mp3" );
		// convert
		try {
			for( std::size_t i = 0; i < this->tracks_.size(); ++i ) {
				this->encoder_->setTitle( this->tracks_[i]->title );
				this->encoder_->setAuthor( this->tracks_[i]->performer );
				this->tracks_[i]->convert( nameHack.arg( this->outDir_ ).arg( QString::fromStdWString( tracks_[i]->title ) ).toStdWString(), this->encoder_ );
// 				emit step( i + 1 );
			}
		} catch( std::exception & e ) {
			emit error( tr( e.what() ) );
		}
	}

}
