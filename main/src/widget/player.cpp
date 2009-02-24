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

namespace Khopper {

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
		connect( stop, SIGNAL( clicked() ), this->player_, SLOT( stop() ) );
		playerBox->addWidget( stop );

		playerBox->addWidget( this->seeker_ );
		playerBox->addWidget( this->volume_ );

		connect( this->songList_, SIGNAL( dropFile( const QStringList & ) ), this, SIGNAL( dropFile( const QStringList & ) ) );
		connect( this->songList_, SIGNAL( requireConvert() ), this, SIGNAL( requireConvert() ) );
		mainBox->addWidget( this->songList_ );
	}

	std::vector< TrackSP > Player::getSelectedTracks() const {
		return this->songList_->getSelectedTracks();
	}

	const std::vector< TrackSP > & Player::getTracks() const {
		return this->songList_->getTracks();
	}

	void Player::appendTracks( const std::vector< TrackSP > & tracks ) {
		this->songList_->appendTracks( tracks );
	}

	void Player::play_() {
		const std::vector< TrackSP > & tracks( this->songList_->getTracks() );
		if( !tracks.empty() ) {
			std::vector< TrackSP > selected( this->songList_->getSelectedTracks() );
			if( selected.empty() ) {
				this->player_->setCurrentSource( QString::fromStdWString( tracks[0]->filePath ) );
			} else {
				this->player_->setCurrentSource( QString::fromStdWString( selected[0]->filePath ) );
			}
			this->player_->play();
		}
	}

	void Player::playOrPause_() {
		if( this->player_->state() != Phonon::PlayingState ) {
			this->ppb_->setText( tr( "Pause" ) );
			this->play_();
		} else {
			this->ppb_->setText( tr( "Play" ) );
			this->player_->pause();
		}
	}

}
