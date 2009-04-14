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

#include "plugin/writerplugin.hpp"

#include <QtPlugin>
#include <QVBoxLayout>
#include <QVariant>

Q_EXPORT_PLUGIN2( kpp_ogg, khopper::plugin::OGGPanel )

namespace khopper {

	namespace plugin {

		OGGPanel::OGGPanel( QWidget * parent, Qt::WindowFlags f ):
		AbstractPanel( parent, f ),
		channels_( new QComboBox( this ) ),
// 		bitRate_( new QComboBox( this ) ),
		sampleRate_( new QComboBox( this ) ) {
			QVBoxLayout * mainBox = new QVBoxLayout( this );
			this->setLayout( mainBox );

			this->channels_->addItem( tr( "Mono" ), QVariant( 1 ) );
			this->channels_->addItem( tr( "Streao" ), QVariant( 2 ) );
			this->channels_->setCurrentIndex( 1 );
			mainBox->addWidget( this->channels_ );

// 			this->bitRate_->addItem( tr( "320 bps" ), QVariant( 320000 ) );
// 			mainBox->addWidget( this->bitRate_ );

			this->sampleRate_->addItem( tr( "44100 Hz" ), QVariant( 44100 ) );
			mainBox->addWidget( this->sampleRate_ );
		}

		codec::WriterSP OGGPanel::getWriter() const {
			codec::WriterSP tmp( plugin::createWriter( "ogg" ) );

			tmp->setChannels( this->channels_->itemData( this->channels_->currentIndex() ).toInt() );
// 			tmp->setBitRate( this->bitRate_->itemData( this->bitRate_->currentIndex() ).toInt() );
			tmp->setSampleRate( this->sampleRate_->itemData( this->sampleRate_->currentIndex() ).toInt() );

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
