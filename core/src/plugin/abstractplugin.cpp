/**
* @file abstractplugin.cpp
* @author Wei-Cheng Pan
*
* Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
*
* This file is part of Khopper.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, see <http://www.gnu.org/licenses/>.
*/
#include "plugin/abstractplugin.hpp"

using namespace khopper::plugin;

AbstractPlugin::AbstractPlugin() : installed_( false ) {
}

AbstractPlugin::~AbstractPlugin() {
}

void AbstractPlugin::install( const QFileInfo & fileInfo ) {
	if( this->installed_ ) {
		return;
	}
	this->doInstall( fileInfo );
	this->installed_ = true;
}

void AbstractPlugin::uninstall() {
	if( !this->installed_ ) {
		return;
	}
	this->doUninstall();
	this->installed_ = false;
}
