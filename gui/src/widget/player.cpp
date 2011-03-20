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
#include "playlistmodel.hpp"
#include "playlistview.hpp"
#include "propertydialog.hpp"
#include "wavwrapper.hpp"

#include "ui_player.h"

#include <Phonon/AudioOutput>
#include <Phonon/BackendCapabilities>

#include <algorithm>

namespace {
	static inline QString fromTimestamp( qint64 ms ) {
		return QString( "%1:%2" ).arg( ms / 1000 / 60 ).arg( ms / 1000 % 60, 2L, 10L, QChar( '0' ) );
	}
}

using namespace khopper::widget;
using khopper::album::PlayList;
using khopper::album::TrackSP;
using khopper::album::Timestamp;
using khopper::utility::PlayListModel;
using khopper::utility::WavWrapper;

Player::Player( QWidget * parent ):
QWidget( parent, 0 ),
currentTrack_(),
duration_( 0 ),
model_( new PlayListModel( this ) ),
playable_( true ),
player_( new Phonon::MediaObject( this ) ),
prop_( new PropertyDialog( this ) ),
ui_( new Ui::Player ) {
	this->ui_->setupUi( this );

	this->ui_->playListView->setModel( this->model_ );
	this->connect( this->ui_->playListView, SIGNAL( fileDropped( const QList< QUrl > & ) ), SIGNAL( fileDropped( const QList< QUrl > & ) ) );
	this->connect( this->ui_->playListView, SIGNAL( requireConvert() ), SLOT( convertHelper_() ) );
	this->connect( this->ui_->playListView, SIGNAL( errorOccured( const QString &, const QString & ) ), SIGNAL( errorOccured( const QString &, const QString & ) ) );
	this->connect( this->ui_->playListView, SIGNAL( requireProperty( const QModelIndex & ) ), SLOT( propertyHelper_( const QModelIndex & ) ) );

	if( Phonon::BackendCapabilities::availableMimeTypes().empty() ) {
		this->playable_ = false;
		this->ui_->playOrPause->setEnabled( false );
		this->ui_->seekerSlider->setEnabled( false );
		this->ui_->stop->setEnabled( false );
		this->ui_->volumeSlider->setEnabled( false );
		return;
	}

	// Set player
	Phonon::AudioOutput * ao = new Phonon::AudioOutput( Phonon::MusicCategory, this );
	Phonon::createPath( this->player_, ao );
	this->ui_->seekerSlider->setMediaObject( this->player_ );
	this->ui_->volumeSlider->setAudioOutput( ao );
	this->connect( this->player_, SIGNAL( stateChanged( Phonon::State, Phonon::State ) ), SLOT( handleState_( Phonon::State, Phonon::State ) ) );
	this->connect( this->player_, SIGNAL( tick( qint64 ) ), SLOT( onTick_( qint64 ) ) );

	this->connect( this->ui_->playOrPause, SIGNAL( clicked() ), SLOT( playOrPause_() ) );
	this->connect( this->ui_->stop, SIGNAL( clicked() ), SLOT( stop_() ) );
	this->connect( ao, SIGNAL( volumeChanged( qreal ) ), SLOT( onVolumeChanged_( qreal ) ) );

	this->connect( this->ui_->playListView, SIGNAL( requirePlay() ), SLOT( play_() ) );
}

void Player::setQueue_( const PlayList & tracks ) {
	if( tracks.empty() ) {
		return;
	}
	this->currentTrack_ = tracks.front();
	this->player_->setCurrentSource( new WavWrapper( this->currentTrack_->createReader() ) );
	this->player_->currentSource().setAutoDelete( true );
}

PlayList Player::getSelectedTracks() const {
	QModelIndexList selected( this->ui_->playListView->selectionModel()->selectedRows() );
	qSort( selected.begin(), selected.end(), []( const QModelIndex & l, const QModelIndex & r ) {
		return l.row() < r.row();
	} );
	PlayList playList;
	std::for_each( selected.begin(), selected.end(), [&]( const QModelIndex & i ) {
		playList.push_back( this->model_->getPlayList()[i.row()] );
	} );
	return playList;
}

const PlayList & Player::getTracks() const {
	return this->model_->getPlayList();
}

void Player::append( const PlayList & playList ) {
	this->model_->append( playList );
}

void Player::play_() {
	if( this->player_->state() == Phonon::PausedState ) {
		this->player_->play();
		return;
	}

	const PlayList & tracks( this->model_->getPlayList() );

	if( !tracks.empty() ) {
		const PlayList selected( this->getSelectedTracks() );
		if( selected.empty() ) {
			this->setQueue_( tracks );
		} else {
			this->setQueue_( selected );
		}

		this->duration_ = this->currentTrack_->getDuration().toMillisecond();
		// set time display
		this->ui_->elapsedTime->setText( fromTimestamp( 0 ) );
		this->ui_->remainTime->setText( fromTimestamp( this->duration_ ) );
		this->player_->play();
	}
}

void Player::stop_() {
	if( this->player_->state() != Phonon::StoppedState ) {
		this->ui_->playOrPause->setText( tr( "Play" ) );
		this->player_->stop();
		this->ui_->elapsedTime->setText( "00:00" );
		this->ui_->remainTime->setText( "00:00" );
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

void Player::propertyHelper_( const QModelIndex & index ) {
	this->prop_->exec( this->model_->getPlayList()[index.row()] );
}

void Player::handleState_( Phonon::State newState, Phonon::State /*oldState*/ ) {
	switch( newState ) {
	case Phonon::PlayingState:
		this->ui_->playOrPause->setText( tr( "Pause" ) );
		break;
	case Phonon::StoppedState:
		this->ui_->playOrPause->setText( tr( "Play" ) );
		break;
	case Phonon::ErrorState:
		emit this->errorOccured( tr( "Player error" ), this->player_->errorString() );
		break;
	case Phonon::LoadingState:
		qDebug() << "phonon loading ...";
		break;
	default:
		;
	}
}

void Player::convertHelper_() {
	emit this->requireConvert( this->getSelectedTracks() );
}

bool Player::isPlayable() const {
	return this->playable_;
}

void Player::onTick_( qint64 time ) {
	this->ui_->elapsedTime->setText( fromTimestamp( time ) );
	this->ui_->remainTime->setText( fromTimestamp( this->duration_ - time ) );
	if( time >= this->duration_ ) {
		this->stop_();
	}
}

void Player::onVolumeChanged_( qreal volume ) {
	this->ui_->volume->setText( QString( "%1" ).arg( static_cast< int >( volume * 100 ), 3, 10, QChar( '0' ) ) );
}
