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

#include "khopper/abstractreader.hpp"
#include "khopper/playlist.hpp"
#include "khopper/text.hpp"

#include <QtCore/QDir>
#include <QtCore/QtPlugin>

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::SinglePlugin )

using namespace khopper::plugin;
using khopper::album::PlayList;

SinglePlugin::SinglePlugin():
AbstractPlugin() {
	this->setID( KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID) );
	this->setVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
}

void SinglePlugin::doInstall() {
	registerPlayList( this->getID(), []( const QUrl & uri ) {
		if( uri.scheme() != "file" ) {
			// TODO: network support
			return 0;
		}
		return 100;
	}, []( const QUrl & uri ) {
		khopper::album::TrackSP track( new khopper::album::Track( uri ) );

		khopper::album::PlayList tmp;
		tmp.push_back( track );
		return tmp;
	} );
}

void SinglePlugin::doUninstall() {
	unregisterPlayList( this->getID() );
}
