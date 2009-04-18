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

			QRadioButton * lossless = new QRadioButton( tr( "Lossless (FLAC)" ), this );
			this->brGroup_->addButton( lossless );
			codec->addWidget( lossless );

			QHBoxLayout * lossyBox = new QHBoxLayout;
			codec->addLayout( lossyBox );
			QRadioButton * lossy = new QRadioButton( tr( "Lossy (Vorbis)" ), this );
			this->brGroup_->addButton( lossy );
			lossyBox->addWidget( lossy );
			for( int i = 10; i >= -1; --i ) {
				this->quality_->addItem( QString::number( i ), QVariant( i ) );
			}
			lossyBox->addWidget( this->quality_ );

			this->channels_->addItem( tr( "Mono" ), QVariant( 1 ) );
			this->channels_->addItem( tr( "Streao" ), QVariant( 2 ) );
			this->channels_->setCurrentIndex( 1 );
			mainBox->addWidget( this->channels_ );

			this->sampleRate_->addItem( tr( "44100 Hz" ), QVariant( 44100 ) );
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
				;
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
