/**
 * @file mp3panel.cpp
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
#include "mp3panel.hpp"
#include "ui_mp3panel.h"
#include "mp3writer.hpp"

#include <QtCore/QVariant>

using namespace khopper::widget;
using khopper::codec::Writer;
using khopper::codec::WriterSP;
using khopper::codec::Mp3Writer;

Mp3Panel::Mp3Panel():
Panel(),
ui_( new Ui::Mp3Panel ),
choise_( new QButtonGroup( this ) ) {
	this->setTitle( "MPEG Layer 3" );
	this->setSuffix( "mp3" );

	this->ui_->setupUi( this );

	this->ui_->bitRate->addItem( "64", QVariant( 64000 ) );
	this->ui_->bitRate->addItem( "128", QVariant( 128000 ) );
	this->ui_->bitRate->addItem( "192", QVariant( 192000 ) );
	this->ui_->bitRate->addItem( "256", QVariant( 256000 ) );
	this->ui_->bitRate->addItem( "320", QVariant( 320000 ) );
	this->ui_->bitRate->setCurrentIndex( 4 );

	this->ui_->level->addItem( "0 (Highest)", QVariant( 0 ) );
	for( int i = 1; i < 9; ++i ) {
		this->ui_->level->addItem( QString::number( i ), QVariant( i ) );
	}
	this->ui_->level->addItem( "9 (Lowest)", QVariant( 9 ) );

	this->choise_->addButton( this->ui_->cbr );
	this->choise_->setId( this->ui_->cbr, 0 );
	this->choise_->addButton( this->ui_->vbr );
	this->choise_->setId( this->ui_->vbr, 1 );

	this->ui_->sampleRate->addItem( "44100 Hz", QVariant( 44100 ) );
	this->ui_->sampleRate->addItem( "48000 Hz", QVariant( 48000 ) );

	this->ui_->channels->addItem( "Mono", QVariant( 1 ) );
	this->ui_->channels->addItem( "Stereo", QVariant( 2 ) );
	this->ui_->channels->setCurrentIndex( 1 );
}

Writer::Creator Mp3Panel::getWriterCreator() const {
	int id = this->choise_->checkedId();
	int bitRate = this->ui_->bitRate->itemData( this->ui_->bitRate->currentIndex() ).toInt();
	int quality = this->ui_->level->itemData( this->ui_->level->currentIndex() ).toInt();
	// NOTE: workaround for VC10
	return std::bind( std::function< WriterSP ( const QUrl &, int, unsigned int, int ) >( []( const QUrl & uri, int id, unsigned int bitRate, int quality )->WriterSP {
		Mp3Writer * encoder = new Mp3Writer( uri );

		switch( id ) {
		case 0:
			encoder->setBitRate( bitRate );
			break;
		case 1:
			encoder->setVBRQuality( quality );
			break;
		default:
			;
		}
		//if( this->ui_->custom->isChecked() ) {
			//encoder->setSampleRate( this->ui_->sampleRate->itemData( this->ui_->sampleRate->currentIndex() ).toInt() );
			//encoder->setChannels( this->ui_->channels->itemData( this->ui_->channels->currentIndex() ).toInt() );
		//}

		return WriterSP( encoder );
	} ), std::placeholders::_1, id, bitRate, quality );
}
