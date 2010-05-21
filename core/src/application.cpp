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
#include "application.hpp"
#include "applicationprivate.hpp"
#include "plugin/pluginmanager.hpp"

using namespace khopper;
using khopper::plugin::AbstractPanel;
using khopper::plugin::PluginManager;

Application::Application( int & argc, char * * argv ):
QApplication( argc, argv ),
p_( new ApplicationPrivate ) {
}

Application::~Application() {
	delete this->p_;
}

void Application::addPanel( AbstractPanel * panel ) {
	emit this->panelAdded( panel );
}

void Application::removePanel( AbstractPanel * panel ) {
	emit this->panelRemoved( panel );
}

void Application::reloadPlugins() {
	this->p_->reloadPlugins();
}

void ApplicationPrivate::reloadPlugins() {
	PluginManager::Instance().reloadPlugins();
}
