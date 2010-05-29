/**
 * @file converterthread.cpp
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
#include "converterthread.hpp"

#include <QtCore/QFileInfo>

using namespace khopper::widget;
using khopper::album::PlayList;
using khopper::codec::WriterSP;

ConverterThread::ConverterThread( QObject * parent ):
QThread( parent ),
encoder_(),
tracks_(),
paths_(),
canceled_( false ),
converter_( this ) {
	connect( &this->converter_, SIGNAL( decodedTime( qint64 ) ), this, SIGNAL( step( qint64 ) ) );
	connect( this, SIGNAL( canceled() ), &this->converter_, SLOT( cancel() ) );
}

void ConverterThread::setOutput( WriterSP output, const QList< QString > & paths ) {
	this->encoder_ = output;
	this->paths_ = paths;
}

void ConverterThread::setTracks( const PlayList & tracks ) {
	this->tracks_ = tracks;
}

void ConverterThread::cancel() {
	this->canceled_ = true;
	emit canceled();
}

void ConverterThread::run() {
	try {
		for( int i = 0; i < this->tracks_.size(); ++i ) {
			this->encoder_->setTitle( this->tracks_[i]->getTitle().toUtf8().constData() );
			this->encoder_->setArtist( this->tracks_[i]->getArtist().toUtf8().constData() );
			this->encoder_->setAlbum( this->tracks_[i]->getAlbum()->getTitle().toUtf8().constData() );
			emit taskName( this->tracks_[i]->getTitle() );
			emit taskGoal( this->tracks_[i]->getDuration().toMillisecond() );
			emit currentTask( i + 1 );

			this->converter_.convert( this->tracks_[i], this->paths_[i], this->encoder_ );
			if( this->canceled_ ) {
				this->canceled_ = false;
				break;
			}
		}
	} catch( error::BaseError & e ) {
		emit errorOccured( tr( "Error on converting!" ), e.getMessage() );
	} catch( std::exception & e ) {
		emit errorOccured( tr( "Error on converting!" ), e.what() );
	}
}
