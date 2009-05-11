#include "seekslider.hpp"

#include <QHBoxLayout>

namespace khopper {

	namespace widget {

		SeekSlider::SeekSlider( Phonon::MediaObject * media, QWidget * parent ):
		QWidget( parent ),
		media_( NULL ),
		slider_( new QSlider( Qt::Horizontal, parent ) ),
		tracking_( true ) {
			QHBoxLayout * layout = new QHBoxLayout( this );
			this->setLayout( layout );

			layout->addWidget( this->slider_ );
			this->slider_->setDisabled( true );
			this->slider_->setPageStep( 5000 );
			this->slider_->setSingleStep( 500 );
			connect( this->slider_, SIGNAL( valueChanged( int ) ), this, SLOT( seek_( int ) ) );

			setMediaObject( media );
		}

		void SeekSlider::setMediaObject( Phonon::MediaObject * media ) {
			if( this->media_ ) {
				disconnect( this->media_, 0, this, 0 );
			}
			this->media_ = media;

			if( this->media_ ) {
				connect( this->media_, SIGNAL( stateChanged( Phonon::State, Phonon::State ) ), this, SLOT( changeState_( Phonon::State ) ) );
				connect( this->media_, SIGNAL( totalTimeChanged( qint64 ) ), this, SLOT( length_( qint64 ) ) );
				connect( this->media_, SIGNAL( tick( qint64 ) ), this, SLOT( tick_( qint64 ) ) );
				
			} else {
				this->changeState_( Phonon::StoppedState );
				this->seekableChanged_( false );
			}
		}

		void SeekSlider::setRealRange( double begin, double end ) {
			this->begin_ = begin * 1000;
			this->end_ = end * 1000;
			this->setRange( 0, ( this->end_ - this->begin_ ) / 1000 );
		}

		void SeekSlider::changeState_( Phonon::State newState ) {
			switch( newState ) {
			case PlayingState:
				this->play_();
				break;
			default:
				break;
			}
		}

		void SeekSlider::tick_( qint64 time ) {
			this.setValue( ( time - this->begin_ ) / 1000 );
		}

		void SeekSlider::play_() {
			this->slider_->setEnabled( true );
		}

	}

}
