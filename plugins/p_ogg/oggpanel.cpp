/**
 * @file oggpanel.cpp
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
#include "oggpanel.hpp"

#include "plugin/abstractwritercreator.hpp"
#include "oggwriter.hpp"

#include <QtPlugin>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVariant>
#include <QRadioButton>

Q_EXPORT_PLUGIN2( kpp_ogg, khopper::plugin::OGGPanel )

namespace khopper {

	namespace plugin {

		OGGPanel::OGGPanel( QWidget * parent, Qt::WindowFlags f ):
		AbstractPanel( parent, f ),
		channels_( new QComboBox( this ) ),
		brGroup_( new QButtonGroup( this ) ),
		quality_( new QComboBox( this ) ),
		sampleRate_( new QComboBox( this ) ) {
			QVBoxLayout * mainBox = new QVBoxLayout( this );
			this->setLayout( mainBox );

			QVBoxLayout * codec = new QVBoxLayout;
			mainBox->addLayout( codec );

			// set lossless options
			QRadioButton * lossless = new QRadioButton( tr( "Lossless (FLAC)" ), this );
			this->brGroup_->addButton( lossless );
			this->brGroup_->setId( lossless, 0 );
			codec->addWidget( lossless );

			// set lossy options
			QHBoxLayout * lossyBox = new QHBoxLayout;
			codec->addLayout( lossyBox );
			QRadioButton * lossy = new QRadioButton( tr( "Lossy (Vorbis)" ), this );
			this->brGroup_->addButton( lossy );
			this->brGroup_->setId( lossy, 1 );
			lossyBox->addWidget( lossy );
			this->quality_->addItem( "10 (Highest)", QVariant( 10 ) );
			for( int i = 9; i >= 0; --i ) {
				this->quality_->addItem( QString::number( i ), QVariant( i ) );
			}
			this->quality_->addItem( "-1 (Lowest)", QVariant( -1 ) );
			lossyBox->addWidget( this->quality_ );

			// set mutex
			connect( lossy, SIGNAL( toggled( bool ) ), this->quality_, SLOT( setEnabled( bool ) ) );
			lossless->setChecked( true );
			this->quality_->setDisabled( true );

			this->channels_->addItem( tr( "Mono" ), QVariant( 1 ) );
			this->channels_->addItem( tr( "Streao" ), QVariant( 2 ) );
			this->channels_->setCurrentIndex( 1 );
			mainBox->addWidget( this->channels_ );

			this->sampleRate_->addItem( tr( "44100 Hz" ), QVariant( 44100 ) );
			this->sampleRate_->addItem( tr( "48000 Hz" ), QVariant( 48000 ) );
			mainBox->addWidget( this->sampleRate_ );
		}

		codec::WriterSP OGGPanel::getWriter() const {
			codec::WriterSP tmp( new codec::OGGWriter() );

			tmp->setChannels( this->channels_->itemData( this->channels_->currentIndex() ).toInt() );
			tmp->setSampleRate( this->sampleRate_->itemData( this->sampleRate_->currentIndex() ).toInt() );
			switch( this->brGroup_->checkedId() ) {
			case 0:
				break;
			case 1:
				tmp->setQuality( this->quality_->itemData( this->quality_->currentIndex() ).toInt() );
				break;
			default:
				qDebug( "%d\n", this->brGroup_->checkedId() );
			}

			return tmp;
		}

		QString OGGPanel::getSuffix() const {
			return "ogg";
		}

		QString OGGPanel::getTitle() const {
			return "ogg";
		}

	}

}
