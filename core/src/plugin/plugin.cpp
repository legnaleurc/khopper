/**
* @file plugin.cpp
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
#include "plugin.hpp"

namespace khopper {
namespace plugin {

class Plugin::Private {
public:
	Private();

	bool installed;
	QString id;
	QString version;
	QFileInfo info;
};

}
}

using namespace khopper::plugin;

Plugin::Private::Private():
installed( false ),
id(),
version(),
info() {
}

Plugin::Plugin() : p_( new Private ) {
}

Plugin::~Plugin() {
}

void Plugin::setFileInfo( const QFileInfo & fileInfo ) {
	this->p_->info = fileInfo;
}

const QFileInfo & Plugin::getFileInfo() const {
	return this->p_->info;
}

void Plugin::setID( const QString & id ) {
	this->p_->id = id;
}

void Plugin::setVersion( const QString & version ) {
	this->p_->version = version;
}

const QString & Plugin::getID() const {
	return this->p_->id;
}

const QString & Plugin::getVersion() const {
	return this->p_->version;
}

void Plugin::install() {
	if( this->p_->installed ) {
		return;
	}
	this->doInstall();
	this->p_->installed = true;
}

void Plugin::uninstall() {
	if( !this->p_->installed ) {
		return;
	}
	this->doUninstall();
	this->p_->installed = false;
}

bool Plugin::isInstalled() const {
	return this->p_->installed;
}
