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
#include "ui_oggpanel.h"
#include "oggwriter.hpp"
#include "flacwriter.hpp"

#include "khopper/text.hpp"

#include <QtCore/QVariant>

using namespace khopper::widget;
using khopper::codec::WriterSP;
using khopper::codec::FlacWriter;
using khopper::codec::OggWriter;

OggPanel::OggPanel():
AbstractPanel(),
ui_( new Ui::OggPanel ),
choise_( new QButtonGroup( this ) ) {
	this->setTitle( "Ogg Vorbis" );
	this->setSuffix( "ogg" );

	this->ui_->setupUi( this );

	this->choise_->addButton( this->ui_->lossless );
	this->choise_->setId( this->ui_->lossless, 0 );
	this->choise_->addButton( this->ui_->lossy );
	this->choise_->setId( this->ui_->lossy, 1 );

	// set lossy options
	this->ui_->quality->addItem( "10 (Highest)", QVariant( 10 ) );
	for( int i = 9; i >= 0; --i ) {
		this->ui_->quality->addItem( QString::number( i ), QVariant( i ) );
	}
	this->ui_->quality->addItem( "-1 (Lowest)", QVariant( -1 ) );

	this->ui_->channels->addItem( tr( "Mono" ), QVariant( 1 ) );
	this->ui_->channels->addItem( tr( "Streao" ), QVariant( 2 ) );
	this->ui_->channels->setCurrentIndex( 1 );
	this->ui_->sampleRate->addItem( tr( "44100 Hz" ), QVariant( 44100 ) );
	this->ui_->sampleRate->addItem( tr( "48000 Hz" ), QVariant( 48000 ) );
}

WriterSP OggPanel::createWriter( const QUrl & uri ) const {
	WriterSP tmp;
	int id = this->choise_->checkedId();

	if( id == 0 ) {
		FlacWriter * flac = new FlacWriter( uri );
		flac->setOggMode( true );
		tmp.reset( flac );
	} else if( id == 1 ) {
		OggWriter * vorbis = new OggWriter( uri );
		vorbis->setVBRQuality( this->ui_->quality->itemData( this->ui_->quality->currentIndex() ).toInt() / 10.f );
		tmp.reset( vorbis );
	} else {
		qDebug( "%d\n", id );
	}

	tmp->setChannels( this->ui_->channels->itemData( this->ui_->channels->currentIndex() ).toInt() );
	tmp->setSampleRate( this->ui_->sampleRate->itemData( this->ui_->sampleRate->currentIndex() ).toInt() );

	return tmp;
}
