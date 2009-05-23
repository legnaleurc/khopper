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
		tracking_( true ),
		begin_( 0 ),
		end_( 0 ),
		ticking_( false ) {
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
				this->slider_->setTickInterval( 500 );
				this->media_->setTickInterval( 500 );
				connect( this->media_, SIGNAL( stateChanged( Phonon::State, Phonon::State ) ), this, SLOT( changeState_( Phonon::State ) ) );
				connect( this->media_, SIGNAL( tick( qint64 ) ), this, SLOT( tick_( qint64 ) ) );
			} else {
				this->changeState_( Phonon::StoppedState );
			}
		}

		void SeekSlider::setRange( int begin, int end ) {
			this->begin_ = begin;
			this->end_ = end;
			this->ticking_ = true;
			this->slider_->setRange( 0, this->end_ - this->begin_ );
			this->ticking_ = false;
		}

		void SeekSlider::changeState_( Phonon::State newState ) {
			if( !this->media_ || ! this->media_->isSeekable() ) {
				this->setDisabled( false );
				return;
			}
			switch( newState ) {
			case Phonon::PlayingState:
				if( this->media_->tickInterval() == 0 ) {
					this->media_->setTickInterval( 350 );
				}
			case Phonon::BufferingState:
			case Phonon::PausedState:
				this->slider_->setEnabled( true );
				break;
			case Phonon::StoppedState:
			case Phonon::LoadingState:
			case Phonon::ErrorState:
				this->slider_->setDisabled( true );
				this->ticking_ = true;
				this->slider_->setValue( 0 );
				this->ticking_ = false;
				break;
			default:
				;
			}
		}

		void SeekSlider::seek_( int time ) {
			if( !this->ticking_ && this->media_ ) {
				this->media_->seek( this->begin_ + time );
			}
		}

		void SeekSlider::tick_( qint64 time ) {
			qDebug() << time - this->begin_;
			this->ticking_ = true;
			this->slider_->setValue( time - this->begin_ );
			this->ticking_ = false;
		}

	}

}
