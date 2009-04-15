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
#include "songlist.hpp"

#include <QHBoxLayout>

namespace khopper {

	namespace widget {

		Player::Player( QWidget * parent, Qt::WindowFlags f ):
		QWidget( parent, f ),
		player_( new Phonon::MediaObject( this ) ),
		seeker_( new Phonon::SeekSlider( this->player_, this ) ),
		volume_( new Phonon::VolumeSlider( this ) ),
		ppb_( new QPushButton( tr( "Play" ), this ) ),
		songList_( new SongList( this ) ) {
			// Set main layout
			QVBoxLayout * mainBox = new QVBoxLayout( this );
			this->setLayout( mainBox );

			// Set player
			Phonon::AudioOutput * ao = new Phonon::AudioOutput( Phonon::MusicCategory, this );
			Phonon::createPath( this->player_, ao );
			this->volume_->setAudioOutput( ao );

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

			connect( this->songList_, SIGNAL( dropFile( const QStringList & ) ), this, SIGNAL( dropFile( const QStringList & ) ) );
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

		bool Player::play_() {
			const std::vector< album::TrackSP > & tracks( this->songList_->getTracks() );

			if( tracks.empty() ) {
				return false;
			} else {
				std::vector< album::TrackSP > selected( this->songList_->getSelectedTracks() );
				if( selected.empty() ) {
					this->player_->setCurrentSource( tracks[0]->getFilePath() );
				} else {
					this->player_->setCurrentSource( selected[0]->getFilePath() );
				}

				this->player_->play();
				if( this->player_->state() == Phonon::ErrorState ) {
					return false;
				}
			}

			return true;
		}

		void Player::stop_() {
			if( this->player_->state() != Phonon::StoppedState ) {
				this->ppb_->setText( tr( "Play" ) );
				this->player_->stop();
			}
		}

		void Player::playOrPause_() {
			if( this->player_->state() != Phonon::PlayingState ) {
				if( this->play_() ) {
					this->ppb_->setText( tr( "Pause" ) );
				}
			} else {
				this->ppb_->setText( tr( "Play" ) );
				this->player_->pause();
			}
		}

	}

}
