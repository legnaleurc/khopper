/**
 * @file singleplugin.cpp
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
#include "singleplugin.hpp"

#include <QtCore/QDir>
#include <QtCore/QtPlugin>

#include <fileref.h>
#include <tag.h>

#include "khopper/abstractreader.hpp"
#include "khopper/album.hpp"
#include "khopper/playlist.hpp"
#include "khopper/error.hpp"
#include "khopper/timestamp.hpp"

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::SinglePlugin )

using namespace khopper::plugin;
using khopper::album::PlayList;
using khopper::album::Album;
using khopper::album::Track;
using khopper::album::Timestamp;
using khopper::error::CodecError;

SinglePlugin::SinglePlugin():
AbstractPlugin() {
	this->setID( KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID) );
	this->setVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
}

void SinglePlugin::doInstall() {
	registerPlayList( this->getID(), []( const QUrl & /*uri*/ )->unsigned int {
		return 100;
	}, []( const QUrl & uri )->PlayList {
		auto rc = getReaderCreator( uri );
		auto reader = rc( uri );
		auto album = Album::create();
		auto track = Track::create( uri, rc );

		reader->open( QIODevice::ReadOnly );
		if( !reader->isOpen() ) {
			throw CodecError( QObject::tr( "Can not open `%1\' (%2)" ).arg( uri.toString() ).arg( Q_FUNC_INFO ) );
		}
		track->setBitRate( reader->getBitRate() );
		track->setDuration( Timestamp::fromMillisecond( reader->getDuration() ) );
		track->setAudioFormat( reader->getAudioFormat() );

		// FIXME: text codec
		if( uri.scheme() == "file" ) {
			TagLib::FileRef fin( uri.toLocalFile().toUtf8().constData() );
			if( !fin.isNull() ) {
				if( !fin.tag()->album().isNull() ) {
					album->setTitle( TStringToQString( fin.tag()->album() ) );
				}
				if( !fin.tag()->artist().isNull() ) {
					track->setArtist( QByteArray( fin.tag()->artist().toCString( true ) ) );
				}
				if( fin.tag()->track() != 0 ) {
					track->setIndex( fin.tag()->track() );
				}
				if( !fin.tag()->title().isNull() ) {
					track->setTitle( QByteArray( fin.tag()->title().toCString( true ) ) );
				}
			}
		}

		if( album->getTitle().isEmpty() ) {
			album->setTitle( reader->getAlbumTitle() );
		}
		if( track->getArtist().isEmpty() ) {
			track->setArtist( reader->getArtist() );
		}
		if( track->getIndex() <= 0 ) {
			track->setIndex( reader->getIndex() );
		}
		if( track->getTitle().isEmpty() ) {
			if( reader->getTitle().isEmpty() ) {
				// FIXME may not a local file
				QFileInfo info( reader->getURI().toLocalFile() );
				track->setTitle( info.baseName().toUtf8() );
			} else {
				track->setTitle( reader->getTitle() );
			}
		}

		reader->close();

		album->addTrack( track );

		PlayList tmp;
		tmp.push_back( track );
		return tmp;
	} );
}

void SinglePlugin::doUninstall() {
	unregisterPlayList( this->getID() );
}
