/**
 * @file converter.cpp
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
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

#include "converter.hpp"
#include "khopper/album.hpp"
#include "khopper/reader.hpp"
#include "khopper/writer.hpp"
#include "khopper/codecerror.hpp"

using khopper::utility::Converter;
using khopper::album::TrackCSP;
using khopper::codec::Writer;
using khopper::codec::WriterSP;
using khopper::codec::ReaderSP;
using khopper::error::BaseError;
using khopper::error::CodecError;

Converter::Converter( TrackCSP track, Writer::Creator creator, const QString & path ):
QObject( 0 ),
QRunnable(),
canceled_( false ),
creator_( creator ),
track_( track ),
path_( path ) {
}

QUrl Converter::getUniqueURI_() const {
	QFileInfo fi( this->path_ );
	QString baseName( fi.baseName() );
	for( int i = 1; fi.exists(); ++i ) {
		fi.setFile( fi.dir(), QString( "%1_%2.%3" ).arg( baseName ).arg( i ).arg( fi.completeSuffix() ) );
	}
	return QUrl::fromLocalFile( fi.absoluteFilePath() );
}

void Converter::run() {
	ReaderSP decoder;
	try {
		decoder = this->track_->createReader();
	} catch( BaseError & e ) {
		emit this->errorOccured( QObject::tr( "Decoder Error" ), e.getMessage() );
		emit this->finished();
		return;
	}
	bool opened = decoder->open( QIODevice::ReadOnly );
	if( !opened ) {
		emit this->errorOccured( QObject::tr( "Decoder Error" ), decoder->errorString() );
		emit this->finished();
		return;
	}

	WriterSP encoder( this->creator_( this->getUniqueURI_() ) );
	auto album = this->track_->getAlbum().lock();
	encoder->setAlbum( album->getTitle().toUtf8() );
	encoder->setArtist( this->track_->getArtist().toUtf8() );
	encoder->setTitle( this->track_->getTitle().toUtf8() );
	encoder->setAudioFormat( decoder->getAudioFormat() );
	encoder->setChannelLayout( decoder->getChannelLayout() );

	opened = encoder->open( QIODevice::WriteOnly );
	if( !opened ) {
		emit this->errorOccured( tr( "Encoder Error" ), encoder->errorString() );
		emit this->finished();
		return;
	}
	this->canceled_ = false;

	const int sec = decoder->getAudioFormat().frequency() * decoder->getAudioFormat().channels() * decoder->getAudioFormat().sampleSize() / 8;
	std::function< bool () > endCondition( ( decoder->isSequential() ) ? static_cast< std::function< bool () > >( [&]()->bool {
		return decoder->waitForReadyRead( -1 );
	} ) : static_cast< std::function< bool () > >( [&]()->bool {
		return !decoder->atEnd();
	} ) );
	try {
		while( endCondition() ) {
			if( this->canceled_ ) {
				break;
			}
			QByteArray frame( decoder->read( sec ) );
			encoder->write( frame );
			emit this->decodedTime( frame.size() * 1000 / sec );
		}
	} catch( BaseError & e ) {
		emit this->errorOccured( QObject::tr( "Convert Error" ), e.getMessage() );
	} catch( std::exception & e ) {
		emit this->errorOccured( QObject::tr( "Convert Error" ), QString::fromLocal8Bit( e.what() ) );
	}

	encoder->close();
	decoder->close();
	emit this->finished();
}

void Converter::cancel() {
	this->canceled_ = true;
}

qint64 Converter::getMaximumValue() const {
	return this->track_->getDuration().toMillisecond();
}

QString Converter::getTitle() const {
	return this->track_->getTitle();
}
