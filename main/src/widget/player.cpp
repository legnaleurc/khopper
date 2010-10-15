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
//#include <QtCore/QModelIndex>
#include <QtDebug>
#include <Phonon/BackendCapabilities>

namespace {

	static inline QString fromTimestamp( const khopper::album::Timestamp & ts ) {
		return QString( "%1:%2" ).arg( ts.getMinute() ).arg( ts.getSecond(), 2L, 10L, QChar( '0' ) );
	}

	static inline bool decRowComp( const QModelIndex & l, const QModelIndex & r ) {
		return l.row() > r.row();
	}

	static inline bool incRowComp( const QModelIndex & l, const QModelIndex & r ) {
		return l.row() < r.row();
	}

}

using namespace khopper::widget;
using khopper::album::PlayList;
using khopper::album::TrackSP;

Player::Player( QWidget * parent ):
QWidget( parent, 0 ),
currentTrack_(),
duration_(),
model_( new PlayListModel( this ) ),
playable_( true ),
player_( new Phonon::MediaObject( this ) ),
prop_( new PropertyDialog( this ) ),
ui_( new Ui::Player ) {
	this->ui_->setupUi( this );

	this->ui_->playListView->setModel( this->model_ );
	connect( this->ui_->playListView, SIGNAL( fileDropped( const QList< QUrl > & ) ), this, SIGNAL( fileDropped( const QList< QUrl > & ) ) );
	connect( this->ui_->playListView, SIGNAL( requireConvert() ), this, SLOT( convertHelper_() ) );
	connect( this->ui_->playListView, SIGNAL( errorOccured( const QString &, const QString & ) ), this, SIGNAL( errorOccured( const QString &, const QString & ) ) );
	connect( this->ui_->playListView, SIGNAL( requireProperty( const QModelIndex & ) ), this, SLOT( propertyHelper_( const QModelIndex & ) ) );

	if( !Phonon::BackendCapabilities::isMimeTypeAvailable( "audio/pcm" ) ) {
		this->playable_ = false;
		this->ui_->next->setEnabled( false );
		this->ui_->playOrPause->setEnabled( false );
		this->ui_->previous->setEnabled( false );
		this->ui_->seeker->setEnabled( false );
		this->ui_->stop->setEnabled( false );
		this->ui_->volume->setEnabled( false );
		return;
	}

	// Set player
	Phonon::AudioOutput * ao = new Phonon::AudioOutput( Phonon::MusicCategory, this );
	Phonon::createPath( this->player_, ao );
	this->ui_->seeker->setMediaObject( this->player_ );
	this->ui_->volume->setAudioOutput( ao );
	connect( this->player_, SIGNAL( stateChanged( Phonon::State, Phonon::State ) ), this, SLOT( handleState_( Phonon::State, Phonon::State ) ) );
	//connect( this->player_, SIGNAL( tick( qint64 ) ), this, SLOT( tick_( qint64 ) ) );

	connect( this->ui_->playOrPause, SIGNAL( clicked() ), this, SLOT( playOrPause_() ) );
	connect( this->ui_->stop, SIGNAL( clicked() ), this, SLOT( stop_() ) );

	connect( this->ui_->playListView, SIGNAL( requirePlay() ), this, SLOT( play_() ) );
}

void Player::setQueue_( const PlayList & tracks ) {
	if( tracks.empty() ) {
		return;
	}
	QList< Phonon::MediaSource > queue;
	foreach( TrackSP track, tracks ) {
		queue.push_back( new WavWrapper( track->createReader() ) );
		queue.back().setAutoDelete( true );
	}
	this->player_->setCurrentSource( queue.first() );
	this->player_->setQueue( queue.mid( 1 ) );
}

PlayList Player::getSelectedTracks() const {
	QModelIndexList selected( this->ui_->playListView->selectionModel()->selectedRows() );
	qSort( selected.begin(), selected.end(), incRowComp );
	PlayList playList;
	foreach( const QModelIndex & i, selected ) {
		playList.push_back( this->model_->getPlayList()[i.row()] );
	}
	return playList;
}

const PlayList & Player::getTracks() const {
	return this->model_->getPlayList();
}

void Player::append( const PlayList & playList ) {
	this->model_->append( playList );
	this->ui_->playListView->resizeRowsToContents();
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

		//this->duration_ = this->currentTrack_->getDuration();
		// set time display
		//this->currentTimeStamp_ = album::Timestamp::fromMillisecond( 0 );
		//this->ui_->passedTime->setText( fromTimestamp( this->currentTimeStamp_ ) );
		//this->ui_->remainTime->setText( fromTimestamp( this->duration_ ) );
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

void Player::propertyHelper_( const QModelIndex & index ) {
	this->prop_->exec( this->model_->getPlayList()[index.row()] );
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
		emit this->errorOccured( tr( "Player error" ), this->player_->errorString() );
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

//void Player::tick_( qint64 time ) {
//	this->currentTimeStamp_ = album::Timestamp::fromMillisecond( time - this->currentBeginTime_ );
//	this->ui_->passedTime->setText( fromTimestamp( this->currentTimeStamp_ ) );
//	this->ui_->remainTime->setText( fromTimestamp( this->duration_ - this->currentTimeStamp_ ) );
////			qDebug() << time;
//	if( time >= this->currentEndTime_ ) {
//		this->stop_();
//	}
//}

//void Player::updateTimestamp_( int ms ) {
//	this->currentTimeStamp_ = album::Timestamp::fromMillisecond( this->currentBeginTime_ + ms );
//	this->ui_->passedTime->setText( fromTimestamp( this->currentTimeStamp_ ) );
//	this->ui_->remainTime->setText( fromTimestamp( this->duration_ - this->currentTimeStamp_ ) );
//}
