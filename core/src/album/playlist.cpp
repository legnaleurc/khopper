/**
 * @file playlist.cpp
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
#include "core/applicationprivate.hpp"

#include <QtCore/QFile>
//#include <QtXml/QXmlStreamWriter>

#include <algorithm>

using namespace khopper::album;
using namespace khopper::plugin;

PlayList khopper::album::createPlayList( const QUrl & uri ) {
	return ApplicationPrivate::self->playlistFactory.createProduct( uri );
}

void khopper::album::exportPlayList( const PlayList & playList, const QUrl & fileURI ) {
	// TODO: find a robust implementation
	// FIXME: local file only
	//QString filePath = fileURI.toLocalFile();
	//QFile fout( filePath );
	//fout.open( QIODevice::WriteOnly );
	//QXmlStreamWriter xout( &fout );

	//xout.writeStartDocument();

	//std::for_each( playList.begin(), playList.end(), [&xout]( TrackCSP track ) {
	//	xout.writeStartElement( "item" );
	//	xout.writeAttribute( "uri", track->getURI().toString() );
	//	xout.writeEndElement();
	//} );

	//xout.writeEndDocument();

	//fout.close();
}

bool khopper::plugin::registerPlayList( const QString & id, PlayListVerifier v, PlayListCreator c ) {
	return ApplicationPrivate::self->playlistFactory.registerProduct( id, v, c );
}

bool khopper::plugin::unregisterPlayList( const QString & id ) {
	return ApplicationPrivate::self->playlistFactory.unregisterProduct( id );
}
