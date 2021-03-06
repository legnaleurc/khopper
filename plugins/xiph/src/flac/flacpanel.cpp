/**
 * @file flacpanel.cpp
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
#include "flacpanel.hpp"
#include "ui_flacpanel.h"
#include "flacwriter.hpp"

#include <QtCore/QVariant>

using khopper::widget::FlacPanel;
using khopper::codec::Writer;
using khopper::codec::WriterSP;
using khopper::codec::FlacWriter;

FlacPanel::FlacPanel():
Panel(),
ui_( new Ui::FlacPanel ) {
	this->setTitle( "FLAC" );
	this->setSuffix( "flac" );

	this->ui_->setupUi( this );

	this->ui_->sampleRate->addItem( "44100 Hz", QVariant( 44100 ) );
	this->ui_->sampleRate->addItem( "48000 Hz", QVariant( 48000 ) );

	this->ui_->channels->addItem( "Mono", QVariant( 1 ) );
	this->ui_->channels->addItem( "Stereo", QVariant( 2 ) );
	this->ui_->channels->setCurrentIndex( 1 );
}

Writer::Creator FlacPanel::getWriterCreator() const {
	return []( const QUrl & uri )->WriterSP {
		FlacWriter * encoder = new FlacWriter( uri );

		//if( this->ui_->custom->isChecked() ) {
			//encoder->setSampleRate( this->ui_->sampleRate->itemData( this->ui_->sampleRate->currentIndex() ).toInt() );
			//encoder->setChannels( this->ui_->channels->itemData( this->ui_->channels->currentIndex() ).toInt() );
		//}

		return WriterSP( encoder );
	};
}
