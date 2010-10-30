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
#include "khopper/text.hpp"

#include <QtCore/QDir>
#include <QtCore/QtPlugin>

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::CueSheetPlugin )

namespace {

	unsigned int verifier( const QUrl & uri ) {
		if( uri.scheme() != "file" ) {
			// TODO: network support
			return 0;
		}
		QFileInfo info( uri.toLocalFile() );
		if( info.suffix().toLower() == "cue" ) {
			return 100;
		} else if( info.dir().exists( info.baseName() + ".cue" ) ) {
			return 200;
		} else {
			return 0;
		}
	}

	khopper::album::PlayList creator( const QUrl & uri ) {
		QFileInfo info( uri.toLocalFile() );
		QFile fin;
		if( info.suffix().toLower() == "cue" ) {
			fin.setFileName( info.filePath() );
		} else {
			fin.setFileName( info.path() + "/" + info.baseName() + ".cue" );
		}
		fin.open( QIODevice::ReadOnly );
		QString content = khopper::widget::CodecSelector::selectTextCodec( fin.readAll() );
		fin.close();
		return khopper::album::CueSheetParser::load( content, info.dir() );
	}

}

using namespace khopper::plugin;
using khopper::album::PlayList;

CueSheetPlugin::CueSheetPlugin():
AbstractPlugin() {
	this->setID( KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID) );
	this->setVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
}

void CueSheetPlugin::doInstall() {
	registerPlayList( verifier, creator );
}

void CueSheetPlugin::doUninstall() {
	unregisterPlayList( verifier );
}
