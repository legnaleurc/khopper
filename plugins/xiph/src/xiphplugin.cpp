/**
 * @file xiphplugin.cpp
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
#include "xiphplugin.hpp"
#include "flac/flacpanel.hpp"
#include "flac/flacreader.hpp"
#include "ogg/oggpanel.hpp"

#include "khopper/writerpanelcontext.hpp"

#include <QtCore/QtPlugin>

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::XiphPlugin )

using khopper::codec::AbstractReader;
using khopper::plugin::XiphPlugin;
using khopper::widget::FlacPanel;
using khopper::widget::OggPanel;
using khopper::widget::WriterPanelContext;

XiphPlugin::XiphPlugin():
AbstractPlugin(),
flacPanel_( new FlacPanel ),
oggPanel_( new OggPanel ) {
	this->setID( KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID) );
	this->setVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
}

void XiphPlugin::doInstall() {
	WriterPanelContext::instance().install( this->flacPanel_.get() );
	WriterPanelContext::instance().install( this->oggPanel_.get() );
	AbstractReader::install( this->getID(), []( const QUrl & uri )->unsigned int {
		// FIXME
		QFileInfo info( uri.toLocalFile() );
		if( info.suffix().toLower() == "flac" ) {
			return 200;
		}
		return 0;
	}, []( const QUrl & uri )->khopper::codec::ReaderSP {
		return khopper::codec::ReaderSP( new khopper::codec::FlacReader( uri ) );
	} );
}

void XiphPlugin::doUninstall() {
	AbstractReader::uninstall( this->getID() );
	WriterPanelContext::instance().uninstall( this->oggPanel_.get() );
	WriterPanelContext::instance().uninstall( this->flacPanel_.get() );
}
