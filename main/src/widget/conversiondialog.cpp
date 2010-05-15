/**
 * @file conversiondialog.cpp
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
#include "conversiondialog.hpp"

#include "plugin/pluginmanager.hpp"

#include "ui_conversiondialog.h"

#include <QtCore/QPluginLoader>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QVBoxLayout>
#include <QtDebug>

namespace khopper {

	namespace widget {

		ConversionDialog::ConversionDialog( QWidget * parent ):
		QDialog( parent ),
		ui_( new Ui::ConversionDialog ),
		table_() {
			this->ui_->setupUi( this );

			foreach( plugin::AbstractPanel * panel, plugin::PluginManager::Instance().getPanels() ) {
				qDebug() << panel;
				this->table_.insert( std::make_pair( this->ui_->tabWidget->addTab( panel, panel->getTitle() ), panel ) );
			}
		}

		ConversionDialog::~ConversionDialog() {
			delete this->ui_;
		}

		plugin::AbstractPanel * ConversionDialog::getCurrent() const {
			return this->table_.find( this->ui_->tabWidget->currentIndex() )->second;
		}

	}

}
