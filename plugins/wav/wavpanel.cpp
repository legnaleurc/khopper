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
#include "ui_wavpanel.h"
#include "wavplugin.hpp"

#include "khopper/defaultwriter.hpp"
#include "khopper/text.hpp"
#include "khopper/application.hpp"

#include <QtCore/QVariant>
#include <QtPlugin>

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::WAVPlugin )

using namespace khopper::plugin;
using namespace khopper::widget;
using khopper::codec::WriterSP;
using khopper::codec::DefaultWriter;

WAVPlugin::WAVPlugin():
AbstractPlugin(),
panel_( new WAVPanel ) {
	this->setID( KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID) );
	this->setVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
}

WAVPlugin::~WAVPlugin() {
	delete this->panel_;
}

void WAVPlugin::doInstall( const QFileInfo & fileInfo ) {
	KHOPPER_APPLICATION->addPanel( this->panel_ );
}

void WAVPlugin::doUninstall() {
	KHOPPER_APPLICATION->removePanel( this->panel_ );
}

WAVPanel::WAVPanel():
AbstractPanel(),
ui_( new Ui::WAVPanel ) {
	this->setTitle( "Wav" );
	this->setSuffix( "wav" );

	this->ui_->setupUi( this );

	this->ui_->sampleRate->addItem( "44100 Hz", QVariant( 44100 ) );
	this->ui_->sampleRate->addItem( "48000 Hz", QVariant( 48000 ) );

	this->ui_->channels->addItem( "Mono", QVariant( 1 ) );
	this->ui_->channels->addItem( "Streao", QVariant( 2 ) );
	this->ui_->channels->setCurrentIndex( 1 );
}

WAVPanel::~WAVPanel() {
	delete this->ui_;
}

WriterSP WAVPanel::getWriter() const {
	WriterSP encoder( new DefaultWriter );

	encoder->setSampleRate( this->ui_->sampleRate->itemData( this->ui_->sampleRate->currentIndex() ).toInt() );
	encoder->setChannels( this->ui_->channels->itemData( this->ui_->channels->currentIndex() ).toInt() );

	return encoder;
}
