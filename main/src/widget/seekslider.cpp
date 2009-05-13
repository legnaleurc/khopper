/**
 * @file seekslider.cpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * Khopper is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Khopper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
				connect( this->media_, SIGNAL( tick( qint64 ) ), this, SLOT( tick_( qint64 ) ) );
			} else {
				this->changeState_( Phonon::StoppedState );
			}
		}

		void SeekSlider::setRange( double begin, double end ) {
			this->begin_ = begin * 1000;
			this->end_ = end * 1000;
			this->slider_->setRange( 0, this->end_ - this->begin_ );
		}

		void SeekSlider::changeState_( Phonon::State newState ) {
			switch( newState ) {
			case Phonon::PlayingState:
				this->play_();
				break;
			case Phonon::StoppedState:
				this->slider_->setValue( 0 );
				this->slider_->setDisabled( true );
				break;
			default:
				break;
			}
		}

		void SeekSlider::seek_( int time ) {
			this->media_->seek( time );
		}

		void SeekSlider::tick_( qint64 time ) {
			this->slider_->setValue( time - this->begin_ );
		}

		void SeekSlider::play_() {
			this->slider_->setEnabled( true );
		}

	}

}
