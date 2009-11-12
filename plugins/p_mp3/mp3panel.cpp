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
#include "mp3writer.hpp"

#include "plugin/abstractwritercreator.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QVariant>
#include <QLabel>
#include <QtDebug>
#include <QtPlugin>

Q_EXPORT_PLUGIN2( kpp_mp3, khopper::plugin::MP3Panel )

namespace khopper {

	namespace plugin {

		MP3Panel::MP3Panel( QWidget * parent, Qt::WindowFlags f ):
		AbstractPanel( parent, f ),
		brChoise_( new QButtonGroup( this ) ),
		bitRate_( new QComboBox( this ) ),
		level_( new QComboBox( this ) ),
		sampleRate_( new QComboBox( this ) ),
		channels_( new QComboBox( this ) ) {
			QVBoxLayout * vbox = new QVBoxLayout( this );
			this->setLayout( vbox );

			QGroupBox * brGroup = new QGroupBox( tr( "Bit Rate" ), this );
			vbox->addWidget( brGroup );
			QVBoxLayout * brBox = new QVBoxLayout( brGroup );
			brGroup->setLayout( brBox );

			QHBoxLayout * cbrBox = new QHBoxLayout;
			brBox->addLayout( cbrBox );
			QRadioButton * cbr = new QRadioButton( tr( "CBR" ), this );
			cbrBox->addWidget( cbr );
			this->bitRate_->addItem( "64", QVariant( 64000 ) );
			this->bitRate_->addItem( "128", QVariant( 128000 ) );
			this->bitRate_->addItem( "192", QVariant( 192000 ) );
			this->bitRate_->addItem( "256", QVariant( 256000 ) );
			this->bitRate_->addItem( "320", QVariant( 320000 ) );
			this->bitRate_->setCurrentIndex( 4 );
			cbrBox->addWidget( this->bitRate_ );

			QHBoxLayout * vbrBox = new QHBoxLayout;
			brBox->addLayout( vbrBox );
			QRadioButton * vbr = new QRadioButton( tr( "VBR" ), this );
			vbrBox->addWidget( vbr );
			this->level_->addItem( "0 (Highest)", QVariant( 0 ) );
			for( int i = 1; i < 9; ++i ) {
				this->level_->addItem( QString::number( i ), QVariant( i ) );
			}
			this->level_->addItem( "9 (Lowest)", QVariant( 9 ) );
			vbrBox->addWidget( this->level_ );

			this->brChoise_->addButton( cbr );
			this->brChoise_->setId( cbr, 0 );
			this->brChoise_->addButton( vbr );
			this->brChoise_->setId( vbr, 1 );
			connect( cbr, SIGNAL( toggled( bool ) ), this->bitRate_, SLOT( setEnabled( bool ) ) );
			connect( vbr, SIGNAL( toggled( bool ) ), this->level_, SLOT( setEnabled( bool ) ) );
			cbr->setChecked( true );
			this->level_->setEnabled( false );

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

		codec::WriterSP MP3Panel::getWriter() const {
			codec::Mp3Writer * encoder = new codec::Mp3Writer;
//			codec::WriterSP encoder( plugin::createWriter( "mp3" ) );

			switch( this->brChoise_->checkedId() ) {
			case 0:
				encoder->setBitRate( this->bitRate_->itemData( this->bitRate_->currentIndex() ).toInt() );
				break;
			case 1:
				encoder->setVBRQuality( this->level_->itemData( this->level_->currentIndex() ).toInt() );
				break;
			default:
				;
			}
			encoder->setSampleRate( this->sampleRate_->itemData( this->sampleRate_->currentIndex() ).toInt() );
			encoder->setChannels( this->channels_->itemData( this->channels_->currentIndex() ).toInt() );

			return codec::WriterSP( encoder );
		}

		QString MP3Panel::getSuffix() const {
			return "mp3";
		}

		QString MP3Panel::getTitle() const {
			return "mp3";
		}

	}

}
