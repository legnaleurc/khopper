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

#include "util/text.hpp"
#include "application.hpp"

#include <QtPlugin>

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::XiphPlugin )

namespace {
	khopper::codec::AbstractReader * creator() {
		return new khopper::codec::FlacReader;
	}
}

using namespace khopper::plugin;
using khopper::widget::FlacPanel;
using khopper::widget::OggPanel;

XiphPlugin::XiphPlugin():
AbstractPlugin(),
flacPanel_( new FlacPanel ),
oggPanel_( new OggPanel ) {
	this->setID( KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID) );
	this->setVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
}

XiphPlugin::~XiphPlugin() {
	delete this->flacPanel_;
	delete this->oggPanel_;
}

void XiphPlugin::doInstall( const QFileInfo & fileInfo ) {
	KHOPPER_APPLICATION->addPanel( this->flacPanel_ );
	KHOPPER_APPLICATION->addPanel( this->oggPanel_ );
	khopper::plugin::registerReader( "flac", creator );
}

void XiphPlugin::doUninstall() {
	KHOPPER_APPLICATION->removePanel( this->flacPanel_ );
	KHOPPER_APPLICATION->removePanel( this->oggPanel_ );
}
