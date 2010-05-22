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
#include "abstractplugin.hpp"
#include "abstractpluginprivate.hpp"

using namespace khopper::plugin;

AbstractPlugin::AbstractPlugin() : p_( new AbstractPluginPrivate ) {
}

AbstractPlugin::~AbstractPlugin() {
	delete this->p_;
}

void AbstractPlugin::setID( const QString & id ) {
	this->p_->id = id;
}

void AbstractPlugin::setVersion( const QString & version ) {
	this->p_->version = version;
}

const QString & AbstractPlugin::getID() const {
	return this->p_->id;
}

const QString & AbstractPlugin::getVersion() const {
	return this->p_->version;
}

void AbstractPlugin::install( const QFileInfo & fileInfo ) {
	if( this->p_->installed ) {
		return;
	}
	this->doInstall( fileInfo );
	this->p_->installed = true;
}

void AbstractPlugin::uninstall() {
	if( !this->p_->installed ) {
		return;
	}
	this->doUninstall();
	this->p_->installed = false;
}

bool AbstractPlugin::isInstalled() const {
	return this->p_->installed;
}

AbstractPluginPrivate::AbstractPluginPrivate():
installed( false ),
id(),
version() {
}
