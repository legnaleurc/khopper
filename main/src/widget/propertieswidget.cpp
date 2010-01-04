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

#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>

namespace khopper {
	namespace widget {

		PropertiesWidget::PropertiesWidget( QWidget * parent ):
		QDialog( parent ),
		albumInput_( new QLineEdit( this ) ),
		artistInput_( new QLineEdit( this ) ),
		buttonBox_( new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel, Qt::Horizontal, this ) ),
		titleInput_( new QLineEdit( this ) ) {
			QVBoxLayout * mainLayout = new QVBoxLayout( this );
			this->setLayout( mainLayout );

			QGridLayout * contentLayout = new QGridLayout;
			mainLayout->addLayout( contentLayout );

			// properties layout
			this->initFieldLayout_( contentLayout );

			// setup dialog button box
			QObject::connect( this->buttonBox_, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( perform_( QAbstractButton * ) ) );
			mainLayout->addWidget( this->buttonBox_ );
		}

		int PropertiesWidget::exec( const album::TrackList & tracks ) {
			if( tracks.size() == 1 ) {
				// exactally one track
				album::TrackSP track = tracks[0];
				this->titleInput_->setText( track->get( "title" ).toString() );
				this->artistInput_->setText( track->get( "artist" ).toString() );
				this->albumInput_->setText( track->get( "album" ).toString() );
			} else {
				// no need to edit title
				this->titleInput_->setText( "" );
				this->titleInput_->setEnabled( false );

				// find common string
				album::TrackSP track = tracks[0];
				bool allGreen = true;
				QString tmp = track->get( "artist" ).toString();
				for( std::size_t i = 1; i < tracks.size(); ++i ) {
					if( tmp != tracks[i]->get( "artist" ).toString() ) {
						allGreen = false;
						break;
					}
				}
				this->artistInput_->setText( allGreen ? tmp : "" );

				allGreen = true;
				tmp = track->get( "album" ).toString();
				for( std::size_t i = 1; i < tracks.size(); ++i ) {
					if( tmp != tracks[i]->get( "album" ).toString() ) {
						allGreen = false;
						break;
					}
				}
				this->albumInput_->setText( allGreen ? tmp : "" );
			}
			return this->exec();
		}

		void PropertiesWidget::perform_( QAbstractButton * button ) {
			switch( this->buttonBox_->buttonRole( button ) ) {
				case QDialogButtonBox::AcceptRole:
					this->write_();
					this->accept();
					break;
				case QDialogButtonBox::ApplyRole:
					this->write_();
					break;
				case QDialogButtonBox::RejectRole:
					this->reject();
					break;
				default:
					;
			}
			if( !this->titleInput_->isEnabled() ) {
				this->titleInput_->setEnabled( true );
			}
		}

		void PropertiesWidget::write_() {
		}

		void PropertiesWidget::initFieldLayout_( QGridLayout * layout ) {
			QLabel * titleLabel = new QLabel( tr( "Title" ), this );
			layout->addWidget( titleLabel, 0, 0 );
			layout->addWidget( this->titleInput_, 0, 1 );

			QLabel * artistLabel = new QLabel( tr( "Artist" ), this );
			layout->addWidget( artistLabel, 1, 0 );
			layout->addWidget( this->artistInput_, 1, 1 );

			QLabel * albumLabel = new QLabel( tr( "Album" ), this );
			layout->addWidget( albumLabel, 2, 0 );
			layout->addWidget( this->albumInput_, 2, 1 );
		}

	}
}
