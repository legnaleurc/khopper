/**
 * @file cuesheetplugin.cpp
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
#include "cuesheetplugin.hpp"
#include "codecselector.hpp"
#include "cuesheetparser.hpp"

#include "khopper/playlist.hpp"

#include <QtCore/QDir>
#include <QtCore/QtPlugin>
#include <QtCore/QtDebug>

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::CueSheetPlugin )

using khopper::album::PlayList;
using khopper::plugin::CueSheetPlugin;

CueSheetPlugin::CueSheetPlugin():
AbstractPlugin() {
	this->setID( KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID) );
	this->setVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
}

void CueSheetPlugin::doInstall() {
	PlayList::install( this->getID(), []( const QUrl & uri )->unsigned int {
		if( uri.scheme() != "file" ) {
			// TODO: network support
			qDebug() << "CueSheetPlugin returned 0" << uri;
			return 0U;
		}
		QFileInfo info( uri.toLocalFile() );
		if( info.suffix().toLower() == "cue" ) {
			qDebug() << "CueSheetPlugin returned 100" << uri;
			return 100U;
		} else if( info.dir().exists( info.baseName() + ".cue" ) ) {
			qDebug() << "CueSheetPlugin returned 200" << uri;
			return 200U;
		} else {
			qDebug() << "CueSheetPlugin returned 0" << uri;
			return 0U;
		}
	}, []( const QUrl & uri )->PlayList {
		QFileInfo info( uri.toLocalFile() );
		QFile fin;
		if( info.suffix().toLower() == "cue" ) {
			fin.setFileName( info.filePath() );
		} else {
			fin.setFileName( info.path() + "/" + info.baseName() + ".cue" );
		}
		fin.open( QIODevice::ReadOnly );
		QByteArray encoded( fin.readAll() );
		fin.close();
		std::pair< bool, QString > decoded = khopper::widget::CodecSelector::selectTextCodec( encoded );
		if( decoded.second.isEmpty() ) {
			return khopper::album::PlayList();
		} else {
			return khopper::album::CueSheetParser::load( decoded.second, info.dir(), decoded.first );
		}
	} );
}

void CueSheetPlugin::doUninstall() {
	PlayList::uninstall( this->getID() );
}
