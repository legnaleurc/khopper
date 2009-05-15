/**
 * @file threads.cpp
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
#include "threads.hpp"

#include <QFileInfo>

namespace khopper {

	namespace widget {

		ConverterThread::ConverterThread( QObject * parent ):
		QThread( parent ),
		encoder_(),
		tracks_(),
		paths_(),
		canceled_( false ),
		converter_( this ) {
			connect( &this->converter_, SIGNAL( decodedTime( int ) ), this, SIGNAL( step( int ) ) );
			connect( this, SIGNAL( canceled() ), &this->converter_, SLOT( cancel() ) );
		}

		void ConverterThread::setOutput( codec::WriterSP output, const QStringList & paths ) {
			this->encoder_ = output;
			this->paths_ = paths;
		}

		void ConverterThread::setTracks( const std::vector< album::TrackSP > & tracks ) {
			this->tracks_ = tracks;
		}

		void ConverterThread::cancel() {
			this->canceled_ = true;
			emit canceled();
		}

		void ConverterThread::run() {
			try {
				for( std::size_t i = 0; i < this->tracks_.size(); ++i ) {
					this->encoder_->setTitle( this->tracks_[i]->getTitle().toUtf8().constData() );
					this->encoder_->setArtist( this->tracks_[i]->getArtist().toUtf8().constData() );
					this->encoder_->setAlbum( this->tracks_[i]->getAlbum().toUtf8().constData() );
					emit taskName( this->tracks_[i]->getTitle() );
					emit taskGoal( this->tracks_[i]->getDuration().toMillisecond() );
					emit currentTask( i + 1 );

					this->converter_.convert( this->tracks_[i], this->paths_[i], this->encoder_ );
					if( this->canceled_ ) {
						this->canceled_ = false;
						break;
					}
				}
			} catch( std::exception & e ) {
				emit error( tr( "Error on converting!" ), trUtf8( e.what() ) );
			}
		}

	}

}
