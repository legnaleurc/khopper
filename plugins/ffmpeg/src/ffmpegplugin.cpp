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
#include "helper.hpp"
#ifdef _WIN32
#include "wfile.hpp"
#endif

#include "khopper/application.hpp"

extern "C" {
#include <libavformat/avformat.h>
}

#include <QtCore/QtDebug>
#include <QtCore/QtPlugin>

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::FfmpegPlugin )

using namespace khopper::plugin;
using khopper::ffmpeg::fromURI;
using khopper::plugin::registerReader;
using khopper::plugin::unregisterReader;
using khopper::widget::WavPanel;

FfmpegPlugin::FfmpegPlugin():
AbstractPlugin(),
panel_( new WavPanel ) {
	this->setID( KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID) );
	this->setVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
}

void FfmpegPlugin::doInstall() {
	khopper::pApp()->addPanel( this->panel_.get() );
	av_register_all();
#ifdef _WIN32
	av_register_protocol2( &khopper::codec::wfileProtocol, sizeof( khopper::codec::wfileProtocol ) );
#endif
	registerReader( this->getID(), []( const QUrl & uri )->unsigned int {
		qDebug() << "verifing FfmpegReader" << uri << fromURI( uri );
		AVFormatContext * pFC = NULL;
		if( avformat_open_input( &pFC, fromURI( uri ), NULL, NULL ) == 0 ) {
			int ret = av_find_stream_info( pFC );
			av_close_input_file( pFC );
			if( ret < 0 ) {
				char msg[1024];
				av_strerror( ret, msg, sizeof( msg ) );
				qDebug() << msg;
				return 0;
			}
			qDebug() << "returned 100 (from FfmpegReader)";
			return 100;
		}
		return 0;
	}, []( const QUrl & uri )->khopper::codec::ReaderSP {
		return khopper::codec::ReaderSP( new khopper::codec::FfmpegReader( uri ) );
	} );
}

void FfmpegPlugin::doUninstall() {
	khopper::pApp()->removePanel( this->panel_.get() );
	unregisterReader( this->getID() );
}
