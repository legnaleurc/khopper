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
#include "wavwrapper.hpp"

#include "ui_player.h"

#include <Phonon/AudioOutput>
#include <QtDebug>

namespace {

	static inline QString fromTimestamp( const khopper::album::Timestamp & ts ) {
		return QString( "%1:%2" ).arg( ts.getMinute() ).arg( ts.getSecond(), 2L, 10L, QChar( '0' ) );
	}

}

using namespace khopper::widget;
using khopper::album::PlayList;

Player::Player( QWidget * parent ):
QWidget( parent, 0 ),
ui_( new Ui::Player ),
player_( new Phonon::MediaObject( this ) ),
//currentTimeStamp_(),
duration_(),
currentTrack_()/*,
currentBeginTime_( -1 ),
currentEndTime_( -1 ),
starting_( false )*/ {
	this->ui_->setupUi( this );

	// Set player
	Phonon::AudioOutput * ao = new Phonon::AudioOutput( Phonon::MusicCategory, this );
	Phonon::createPath( this->player_, ao );
	this->ui_->seeker->setMediaObject( this->player_ );
	this->ui_->volume->setAudioOutput( ao );
	connect( this->player_, SIGNAL( stateChanged( Phonon::State, Phonon::State ) ), this, SLOT( handleState_( Phonon::State, Phonon::State ) ) );
	//connect( this->player_, SIGNAL( tick( qint64 ) ), this, SLOT( tick_( qint64 ) ) );

	connect( this->ui_->playOrPause, SIGNAL( clicked() ), this, SLOT( playOrPause_() ) );
	connect( this->ui_->stop, SIGNAL( clicked() ), this, SLOT( stop_() ) );

	//connect( this->ui _->seeker, SIGNAL( dragged( int ) ), this, SLOT( updateTimestamp_( int ) ) );

	connect( this->ui_->songList, SIGNAL( fileDropped( const QList< QUrl > & ) ), this, SIGNAL( fileDropped( const QList< QUrl > & ) ) );
	connect( this->ui_->songList, SIGNAL( requireConvert( const khopper::album::PlayList & ) ), this, SIGNAL( requireConvert( const khopper::album::PlayList & ) ) );
	connect( this->ui_->songList, SIGNAL( requirePlay() ), this, SLOT( play_() ) );
	connect( this->ui_->songList, SIGNAL( error( const QString &, const QString & ) ), this, SIGNAL( error( const QString &, const QString & ) ) );
}

PlayList Player::getSelectedTracks() const {
	return this->ui_->songList->getSelectedTracks();
}

const PlayList & Player::getTracks() const {
	return this->ui_->songList->getTracks();
}

void Player::append( const PlayList & playList ) {
	this->ui_->songList->append( playList );
}

void Player::play_() {
	if( this->player_->state() == Phonon::PausedState ) {
		this->player_->play();
		return;
	}

	const PlayList & tracks( this->ui_->songList->getTracks() );

	if( !tracks.empty() ) {
		const PlayList selected( this->ui_->songList->getSelectedTracks() );
		if( selected.empty() ) {
			this->currentTrack_ = tracks[0];
		} else {
			this->currentTrack_ = selected[0];
		}

		this->player_->setCurrentSource( new WavWrapper( this->currentTrack_->getReader() ) );
		//album::Timestamp startTime = this->currentTrack_->getStartTime();
		//this->duration_ = this->currentTrack_->getDuration();
		//this->currentBeginTime_ = startTime.toMillisecond();
		//this->currentEndTime_ = this->currentBeginTime_ + this->duration_.toMillisecond();
		//this->ui_->seeker->setRange( this->currentBeginTime_, this->currentEndTime_ );
		//qDebug() << this->currentBeginTime_ << this->currentEndTime_;
		// set time display
		//this->currentTimeStamp_ = album::Timestamp::fromMillisecond( 0 );
		//this->ui_->passedTime->setText( fromTimestamp( this->currentTimeStamp_ ) );
		//this->ui_->remainTime->setText( fromTimestamp( this->duration_ ) );
		this->starting_ = true;
		this->player_->play();
	}
}

void Player::stop_() {
	if( this->player_->state() != Phonon::StoppedState ) {
		this->ui_->playOrPause->setText( tr( "Play" ) );
		//this->ui_->passedTime->setText( "00:00" );
		//this->ui_->remainTime->setText( "00:00" );
		this->player_->stop();
	}
}

void Player::playOrPause_() {
	if( this->player_->state() != Phonon::PlayingState ) {
		this->play_();
	} else {
		this->ui_->playOrPause->setText( tr( "Play" ) );
		this->player_->pause();
	}
}

void Player::handleState_( Phonon::State newState, Phonon::State /*oldState*/ ) {
	switch( newState ) {
	case Phonon::PlayingState:
		//if( this->starting_ ) {
			//this->player_->seek( this->currentBeginTime_ );
			//this->starting_ = false;
		//}
		this->ui_->playOrPause->setText( tr( "Pause" ) );
		break;
	case Phonon::StoppedState:
		this->ui_->playOrPause->setText( tr( "Play" ) );
		break;
	case Phonon::ErrorState:
		emit this->error( tr( "Player error" ), this->player_->errorString() );
		break;
	default:
		;
	}
}

//void Player::tick_( qint64 time ) {
//	this->currentTimeStamp_ = album::Timestamp::fromMillisecond( time - this->currentBeginTime_ );
//	this->ui_->passedTime->setText( fromTimestamp( this->currentTimeStamp_ ) );
//	this->ui_->remainTime->setText( fromTimestamp( this->duration_ - this->currentTimeStamp_ ) );
////			qDebug() << time;
//	if( time >= this->currentEndTime_ ) {
//		this->stop_();
//	}
//}
//
//void Player::updateTimestamp_( int ms ) {
//	this->currentTimeStamp_ = album::Timestamp::fromMillisecond( this->currentBeginTime_ + ms );
//	this->ui_->passedTime->setText( fromTimestamp( this->currentTimeStamp_ ) );
//	this->ui_->remainTime->setText( fromTimestamp( this->duration_ - this->currentTimeStamp_ ) );
//}
