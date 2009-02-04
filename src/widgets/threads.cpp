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
#include "track.hpp"
#include "decoder.hpp"
#include "os.hpp"
#include <QFileInfo>

namespace Khopper {

	ConverterThread::ConverterThread( QObject * parent ) : QThread( parent ) {
	}

	void ConverterThread::setOutput( EncoderSP output, const QString & outDir ) {
		this->encoder_ = output;
		this->outDir_ = outDir;
	}

	void ConverterThread::setTracks( const std::vector< TrackSP > & tracks ) {
		this->tracks_ = tracks;
		for( std::size_t i = 0; i < tracks.size(); ++i ) {
			connect( tracks[i].get(), SIGNAL( decodedTime( int ) ), this, SIGNAL( step( int ) ) );
		}
	}

	void ConverterThread::run() {
		// TODO: damn hack!
		QString nameHack( "%1/%2.mp3" );
		// convert
		try {
			for( std::size_t i = 0; i < this->tracks_.size(); ++i ) {
				this->encoder_->setTitle( this->tracks_[i]->title );
				this->encoder_->setAuthor( this->tracks_[i]->performer );
				this->tracks_[i]->convert( nameHack.arg( this->outDir_ ).arg( QString::fromStdWString( tracks_[i]->title ) ).toStdWString(), this->encoder_ );
// 				emit step( i + 1 );
			}
		} catch( std::exception & e ) {
			emit error( tr( "Error on converting!" ), tr( e.what() ) );
		}
	}

}