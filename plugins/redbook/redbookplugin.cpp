/**
 * @file redbookplugin.cpp
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
#include "redbookplugin.hpp"
#include "redbookreader.hpp"

#include "khopper/application.hpp"

#include <QtCore/QtPlugin>
#include <QtCore/QtDebug>

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::RedbookPlugin )

using namespace khopper::plugin;
using khopper::plugin::registerReader;

RedbookPlugin::RedbookPlugin():
AbstractPlugin() {
	this->setID( KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID) );
	this->setVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
}

RedbookPlugin::~RedbookPlugin() {
}

void RedbookPlugin::doInstall() {
	registerReader( this->getID(), []( const QUrl & uri ) {
		QFileInfo info( uri.toLocalFile() );
		if( info.suffix().toLower() == "bin" ) {
			qDebug() << "returned 200 (" << Q_FUNC_INFO << ")";
			return 200;
		}
		return 0;
	}, []( const QUrl & uri ) {
		return khopper::codec::ReaderSP( new khopper::codec::RedbookReader( uri ) );
	} );
}

void RedbookPlugin::doUninstall() {
	unregisterReader( this->getID() );
}
