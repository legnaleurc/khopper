/**
 * @file propertieswidget.cpp
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
#include "propertieswidget.hpp"

#include <QtGui/QVBoxLayout>

namespace khopper {
	namespace widget {

		PropertiesWidget::PropertiesWidget( QWidget * parent ):
		QDialog( parent ),
		buttonBox_( new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel, Qt::Horizontal, this ) ) {
			QVBoxLayout * mainLayout = new QVBoxLayout( this );
			this->setLayout( mainLayout );

			// setup dialog button box
			QObject::connect( this->buttonBox_, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( perform_( QAbstractButton * ) ) );
			mainLayout->addWidget( this->buttonBox_ );
		}

		int PropertiesWidget::exec( const album::TrackList & tracks ) {
			if( tracks.size() == 1 ) {
				// exactally one track
			} else {
				// multiple editing
			}
			return this->exec();
		}

		void PropertiesWidget::perform_( QAbstractButton * button ) {
			switch( this->buttonBox_->buttonRole( button ) ) {
				case QDialogButtonBox::AcceptRole:
					this->accept();
					break;
				case QDialogButtonBox::ApplyRole:
					break;
				case QDialogButtonBox::RejectRole:
					this->reject();
					break;
				default:
					;
			}
		}

	}
}
