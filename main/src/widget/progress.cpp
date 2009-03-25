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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

namespace khopper {

	namespace widget {

		Progress::Progress( QWidget * parent, Qt::WindowFlags f ):
		QDialog( parent, f ),
		itemName_( new QLabel( this ) ),
		current_( new QLabel( this ) ),
		total_( new QLabel( this ) ),
		prog_( new QProgressBar( this ) ) {
			this->setWindowTitle( tr( "Converting ..." ) );

			QVBoxLayout * mainBox = new QVBoxLayout( this );
			this->setLayout( mainBox );
			mainBox->setAlignment( Qt::AlignCenter );

			mainBox->addWidget( this->itemName_ );
			this->itemName_->setAlignment( Qt::AlignCenter );

			QHBoxLayout * midBox = new QHBoxLayout;
			mainBox->addLayout( midBox );
			midBox->setAlignment( Qt::AlignCenter );

			midBox->addWidget( this->current_ );
			midBox->addWidget( new QLabel( "/", this ) );
			midBox->addWidget( this->total_ );

			mainBox->addWidget( this->prog_ );
			this->prog_->setRange( 0, 0 );

			QPushButton * cancel = new QPushButton( tr( "&Cancel" ), this );
			mainBox->addWidget( cancel );
			connect( cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
		}

		int Progress::getValue() const {
			return this->prog_->value();
		}

		void Progress::setMaximum( int maximum ) {
			this->prog_->setMaximum( maximum );
			this->prog_->setValue( 0 );
		}

		void Progress::setValue( int value ) {
			this->prog_->setValue( value );
		}

		void Progress::setItemName( const QString & name ) {
			this->itemName_->setText( name );
		}

		void Progress::setCurrent( int current ) {
			this->current_->setNum( current );
		}

		void Progress::setTotal( int total ) {
			this->total_->setNum( total );
		}

	}

}
