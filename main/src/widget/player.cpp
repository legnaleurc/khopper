/**
 * @file player.cpp
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
#include "player.hpp"
#include "seekslider.hpp"
#include "songlist.hpp"

#include <QHBoxLayout>
#include <QtDebug>
#include <Phonon/AudioOutput>

namespace khopper {

	namespace widget {

		Player::Player( QWidget * parent, Qt::WindowFlags f ):
		QWidget( parent, f ),
		player_( new Phonon::MediaObject( this ) ),
		seeker_( new SeekSlider( this->player_, this ) ),
		volume_( new Phonon::VolumeSlider( this ) ),
		ppb_( new QPushButton( tr( "Play" ), this ) ),
		songList_( new SongList( this ) ),
		currentTrack_(),
		currentBeginTime_( -1 ),
		currentEndTime_( -1 ),
		starting_( false ) {
			// Set main layout
			QVBoxLayout * mainBox = new QVBoxLayout( this );
			this->setLayout( mainBox );

			// Set player
			Phonon::AudioOutput * ao = new Phonon::AudioOutput( Phonon::MusicCategory, this );
			Phonon::createPath( this->player_, ao );
			this->volume_->setAudioOutput( ao );
			connect( this->player_, SIGNAL( stateChanged( Phonon::State, Phonon::State ) ), this, SLOT( handleState_( Phonon::State, Phonon::State ) ) );
			connect( this->player_, SIGNAL( tick( qint64 ) ), this, SLOT( tick_( qint64 ) ) );

			// Setting player panel
			QHBoxLayout * playerBox = new QHBoxLayout;
			mainBox->addLayout( playerBox );

			connect( this->ppb_, SIGNAL( clicked() ), this, SLOT( playOrPause_() ) );
			playerBox->addWidget( ppb_ );
			QPushButton * stop = new QPushButton( tr( "Stop" ), this );
			connect( stop, SIGNAL( clicked() ), this, SLOT( stop_() ) );
			playerBox->addWidget( stop );

			playerBox->addWidget( this->seeker_ );
			playerBox->addWidget( this->volume_ );

			connect( this->songList_, SIGNAL( fileDropped( const QStringList & ) ), this, SIGNAL( fileDropped( const QStringList & ) ) );
			connect( this->songList_, SIGNAL( requireConvert() ), this, SIGNAL( requireConvert() ) );
			mainBox->addWidget( this->songList_ );
		}

		std::vector< album::TrackSP > Player::getSelectedTracks() const {
			return this->songList_->getSelectedTracks();
		}

		const std::vector< album::TrackSP > & Player::getTracks() const {
			return this->songList_->getTracks();
		}

		void Player::appendTracks( const std::vector< album::TrackSP > & tracks ) {
			this->songList_->appendTracks( tracks );
		}

		void Player::play_() {
			if( this->player_->state() == Phonon::PausedState ) {
				this->player_->play();
				return;
			}

			const std::vector< album::TrackSP > & tracks( this->songList_->getTracks() );

			if( !tracks.empty() ) {
				const std::vector< album::TrackSP > selected( this->songList_->getSelectedTracks() );
				if( selected.empty() ) {
					this->currentTrack_ = tracks[0];
				} else {
					this->currentTrack_ = selected[0];
				}

				this->player_->setCurrentSource( this->currentTrack_->getFilePath() );
				this->currentBeginTime_ = this->currentTrack_->getStartTime().toMillisecond();
				this->currentEndTime_ = this->currentBeginTime_ + this->currentTrack_->getDuration().toMillisecond();
				this->seeker_->setRange( this->currentBeginTime_, this->currentEndTime_ );
				qDebug() << this->currentBeginTime_ << this->currentEndTime_;
				this->starting_ = true;
				this->player_->play();
			}
		}

		void Player::stop_() {
			if( this->player_->state() != Phonon::StoppedState ) {
				this->ppb_->setText( tr( "Play" ) );
				this->player_->stop();
			}
		}

		void Player::playOrPause_() {
			if( this->player_->state() != Phonon::PlayingState ) {
				this->play_();
			} else {
				this->ppb_->setText( tr( "Play" ) );
				this->player_->pause();
			}
		}

		void Player::handleState_( Phonon::State newState, Phonon::State /*oldState*/ ) {
			switch( newState ) {
			case Phonon::PlayingState:
				if( this->starting_ ) {
					this->player_->seek( this->currentBeginTime_ );
					this->starting_ = false;
				}
				this->ppb_->setText( tr( "Pause" ) );
				break;
			case Phonon::StoppedState:
				this->ppb_->setText( tr( "Play" ) );
				break;
			case Phonon::ErrorState:
				emit this->error( tr( "Player error" ), this->player_->errorString() );
				break;
			default:
				;
			}
		}

		void Player::tick_( qint64 time ) {
			qDebug() << time;
			if( time >= this->currentEndTime_ ) {
				this->stop_();
			}
		}

	}

}
