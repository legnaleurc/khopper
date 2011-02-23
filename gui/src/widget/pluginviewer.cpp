/**
 * @file pluginviewer.cpp
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
#include "pluginviewer.hpp"
#include "ui_pluginviewer.h"

#include "khopper/application.hpp"
#include "khopper/pluginmodel.hpp"

#include <algorithm>
#include <functional>

using namespace khopper::widget;
using khopper::plugin::PluginModel;

PluginViewer::PluginViewer( QWidget * parent ):
QWidget( parent, Qt::Dialog ),
ui_( new Ui::PluginViewer ) {
	this->ui_->setupUi( this );

	this->ui_->treeView->setModel( khopper::pApp()->getPluginModel() );
	this->connect( this->ui_->loadButton, SIGNAL( clicked() ), SLOT( loadSelected_() ) );
	this->connect( this->ui_->unloadButton, SIGNAL( clicked() ), SLOT( unloadSelected_() ) );
}

void PluginViewer::loadSelected_() {
	QItemSelectionModel * selection = this->ui_->treeView->selectionModel();
	QModelIndexList indices = selection->selectedRows();
	std::for_each( indices.begin(), indices.end(), std::bind( &PluginModel::loadPlugin, khopper::pApp()->getPluginModel(), std::placeholders::_1 ) );
}

void PluginViewer::unloadSelected_() {
	QItemSelectionModel * selection = this->ui_->treeView->selectionModel();
	QModelIndexList indices = selection->selectedRows();
	std::for_each( indices.begin(), indices.end(), std::bind( &PluginModel::unloadPlugin, khopper::pApp()->getPluginModel(), std::placeholders::_1 ) );
}
