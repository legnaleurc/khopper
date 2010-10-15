/**
 * @file ffmpegplugin.cpp
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
#include "ffmpegplugin.hpp"
#include "wavpanel.hpp"
#include "ffmpegreader.hpp"
#ifdef _WIN32
#include "wfile.hpp"
#endif

#include "khopper/text.hpp"
#include "khopper/application.hpp"

extern "C" {
#include <libavformat/avformat.h>
}

#include <QtPlugin>

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::FfmpegPlugin )

namespace {

	unsigned int verifier( const QUrl & uri ) {
		if( guess_format( NULL, uri.toLocalFile().toStdString().c_str(), NULL ) != NULL ) {
			return 100;
		}
		return 0;
	}

	khopper::codec::ReaderSP creator( const QUrl & uri ) {
		return khopper::codec::ReaderSP( new khopper::codec::FfmpegReader( uri ) );
	}

}

using namespace khopper::plugin;
using khopper::widget::WavPanel;
using khopper::plugin::registerReader;
using khopper::plugin::unregisterReader;

FfmpegPlugin::FfmpegPlugin():
AbstractPlugin(),
panel_( new WavPanel ) {
	this->setID( KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID) );
	this->setVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
}

void FfmpegPlugin::doInstall() {
	KHOPPER_APPLICATION->addPanel( this->panel_.get() );
	av_register_all();
#ifdef _WIN32
	av_register_protocol( &khopper::codec::wfileProtocol );
#endif
	registerReader( this->getID(), verifier, creator );
}

void FfmpegPlugin::doUninstall() {
	KHOPPER_APPLICATION->removePanel( this->panel_.get() );
	unregisterReader( this->getID() );
}
