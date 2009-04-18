/**
 * @file wavpanel.cpp
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
#include "wavpanel.hpp"

#include "plugin/abstractwritercreator.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVariant>
#include <QLabel>
#include <QtPlugin>

Q_EXPORT_PLUGIN2( kpp_wav, khopper::plugin::WAVPanel )

namespace khopper {

	namespace plugin {

		WAVPanel::WAVPanel( QWidget * parent, Qt::WindowFlags f ):
		AbstractPanel( parent, f ),
		sampleRate_( new QComboBox( this ) ),
		channels_( new QComboBox( this ) ) {
			QVBoxLayout * vbox = new QVBoxLayout( this );
			this->setLayout( vbox );

			QHBoxLayout * srBox = new QHBoxLayout;
			vbox->addLayout( srBox );
			QLabel * srLabel = new QLabel( tr( "Sample Rate:" ) );
			srBox->addWidget( srLabel );
			this->sampleRate_->addItem( "44100 Hz", QVariant( 44100 ) );
			this->sampleRate_->addItem( "48000 Hz", QVariant( 48000 ) );
			srBox->addWidget( this->sampleRate_ );

			QHBoxLayout * cBox = new QHBoxLayout;
			vbox->addLayout( cBox );
			QLabel * cLabel = new QLabel( tr( "Channel:" ) );
			cBox->addWidget( cLabel );
			this->channels_->addItem( "Mono", QVariant( 1 ) );
			this->channels_->addItem( "Streao", QVariant( 2 ) );
			this->channels_->setCurrentIndex( 1 );
			cBox->addWidget( this->channels_ );
		}

		codec::WriterSP WAVPanel::getWriter() const {
			codec::WriterSP encoder( plugin::createWriter( "wav" ) );

			encoder->setSampleRate( this->sampleRate_->itemData( this->sampleRate_->currentIndex() ).toInt() );
			encoder->setChannels( this->channels_->itemData( this->channels_->currentIndex() ).toInt() );

			return encoder;
		}

		QString WAVPanel::getSuffix() const {
			return "wav";
		}

		QString WAVPanel::getTitle() const {
			return "wav";
		}

	}

}
