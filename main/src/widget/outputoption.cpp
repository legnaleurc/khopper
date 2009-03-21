/**
 * @file outputoption.cpp
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
#include "outputoption.hpp"
#include "abstractpanel.hpp"
#include "os.hpp"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPluginLoader>

namespace khopper {

	namespace widget {

		OutputOption::OutputOption( QWidget * parent ):
		QDialog( parent ),
		optionTabs_( new QTabWidget( this ) ),
		table_() {
			this->setWindowTitle( tr( "Output format setting" ) );
			this->resize( 320, 240 );

			QVBoxLayout * mainBox = new QVBoxLayout( this );
			this->setLayout( mainBox );

			mainBox->addWidget( this->optionTabs_ );

			// Take out the output types
			os::PluginContext pc;
			foreach( QString fileName, pc.getDir().entryList( QStringList( "libkpp_*" ), QDir::Files ) ) {
				QPluginLoader piLoader( pc.getDir().absoluteFilePath( fileName ) );
				QObject * plugin = piLoader.instance();
				if( plugin ) {
					AbstractPanel * option = qobject_cast< AbstractPanel * >( plugin );
					if( option ) {
						this->table_.insert( std::make_pair( this->optionTabs_->addTab( option, option->getTitle() ), option ) );
					}
				}
			}

			QDialogButtonBox * buttons = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this );
			mainBox->addWidget( buttons );
			connect( buttons, SIGNAL( accepted() ), this, SLOT( accept() ) );
			connect( buttons, SIGNAL( rejected() ), this, SLOT( reject() ) );
		}

		AbstractPanel * OutputOption::getCurrent() const {
			return this->table_.find( this->optionTabs_->currentIndex() )->second;
		}

	}

}
