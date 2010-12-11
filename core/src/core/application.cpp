/**
 * @file application.cpp
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
#include "applicationprivate.hpp"
#include "plugin/pluginmanager.hpp"

using namespace khopper;
using khopper::widget::AbstractPanel;
using khopper::plugin::AbstractPlugin;
using khopper::plugin::PluginManager;

ApplicationPrivate * ApplicationPrivate::self = NULL;

Application::Application( int & argc, char * * argv ):
QApplication( argc, argv ),
p_( new ApplicationPrivate ) {
	QObject::connect( this->p_->pm.get(), SIGNAL( errorOccured( const QString &, const QString & ) ), this, SIGNAL( errorOccured( const QString &, const QString & ) ) );
}

void Application::addPanel( AbstractPanel * panel ) {
	emit this->panelAdded( panel );
}

void Application::removePanel( AbstractPanel * panel ) {
	emit this->panelRemoved( panel );
}

void Application::reloadPlugins() {
	this->p_->pm->reloadPlugins();
}

AbstractPlugin * Application::getPluginInstance( const QString & pluginID ) const {
	return this->p_->pm->getPluginInstance( pluginID );
}

ApplicationPrivate::ApplicationPrivate():
readerFactory(),
playlistFactory(),
pm( new PluginManager ) {
	ApplicationPrivate::self = this;
}

Application * khopper::pApp() {
	return static_cast< Application * >( QCoreApplication::instance() );
}
