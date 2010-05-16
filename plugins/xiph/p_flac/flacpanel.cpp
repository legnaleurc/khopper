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
#include "flacwriter.hpp"
#include "plugin/pluginmanager.hpp"
#include "util/text.hpp"

#include <QtCore/QLibrary>
#include <QtCore/QVariant>
#include <QtDebug>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>
#include <QtPlugin>

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::FlacPanel )

#ifdef Q_OS_WIN32
static const char * LIBFLAC = KHOPPER_STRINGIZE(KHOPPER_XIPH_LIB);
#else
static const char * LIBFLAC = KHOPPER_STRINGIZE(KHOPPER_XIPH_LIB) ".so.0.2.60";
#endif

namespace khopper {

	namespace plugin {

		FlacPanel::FlacPanel( QWidget * parent, Qt::WindowFlags f ):
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

		QString FlacPanel::getID() const {
			return KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID);
		}

		QString FlacPanel::getVersion() const {
			return KHOPPER_STRINGIZE(KHOPPER_VERSION);
		}

		codec::WriterSP FlacPanel::getWriter() const {
			FlacWriterCreator loader = reinterpret_cast< FlacWriterCreator >( QLibrary::resolve( LIBFLAC, "createFlacWriter" ) );
			if( loader == NULL ) {
				return codec::WriterSP();
			}
			std::tr1::shared_ptr< codec::FlacWriter > encoder( loader() );

			encoder->setSampleRate( this->sampleRate_->itemData( this->sampleRate_->currentIndex() ).toInt() );
			encoder->setChannels( this->channels_->itemData( this->channels_->currentIndex() ).toInt() );

			return encoder;
		}

		QString FlacPanel::getSuffix() const {
			return "flac";
		}

		QString FlacPanel::getTitle() const {
			return "flac";
		}

		void FlacPanel::doInstall(const QFileInfo& fileInfo) {
			PluginManager::Instance().addPanel( this );
		}

		void FlacPanel::doUninstall() {
			PluginManager::Instance().removePanel( this );
		}

	}

}
