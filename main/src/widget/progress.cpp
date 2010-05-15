/**
 * @file progress.cpp
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
#include "progress.hpp"
#include "ui_progress.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

namespace khopper {

	namespace widget {

		Progress::Progress( QWidget * parent ):
		QWidget( parent, Qt::Dialog ),
		ui_( new Ui::Progress ),
		current_( 0 ),
		total_( 0 ) {
			this->ui_->setupUi( this );

			connect( this->ui_->cancel, SIGNAL( clicked() ), this, SIGNAL( canceled() ) );
		}

		Progress::~Progress() {
			delete this->ui_;
		}

		void Progress::increase( qint64 value ) {
			this->ui_->progressBar->setValue( value + this->ui_->progressBar->value() );
		}

		void Progress::setMaximum( qint64 maximum ) {
			this->ui_->progressBar->setMaximum( maximum );
			this->ui_->progressBar->setValue( 0 );
		}

		void Progress::setItemName( const QString & name ) {
			this->ui_->name->setText( name );
		}

		void Progress::setCurrent( int current ) {
			this->current_ = current;
			this->updateIndex_();
		}

		void Progress::setTotal( int total ) {
			this->total_ = total;
			this->updateIndex_();
		}

		void Progress::updateIndex_() {
			this->ui_->index->setText( QString( "%1/%2" ).arg( this->current_ ).arg( this->total_ ) );
		}

	}

}
