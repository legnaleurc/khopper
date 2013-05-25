/**
 * @file libavplugin.cpp
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
#include "libavplugin.hpp"

#include <cassert>

#include <QtCore/QtDebug>
#include <QtCore/QtPlugin>
#include <QtCore/QMutex>

extern "C" {
#include <libavformat/avformat.h>
}

#include "khopper/writerpanelcontext.hpp"
#include "wavpanel.hpp"
#include "libavreader.hpp"
#ifdef Q_OS_WIN32
#include "wfile.hpp"
#endif

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::LibavPlugin )

namespace {

int locker( void ** mutex, AVLockOp op ) {
	if( op == AV_LOCK_CREATE ) {
		*mutex = new QMutex;
	} else if( op == AV_LOCK_OBTAIN ) {
		auto m = static_cast< QMutex * >( *mutex );
		m->lock();
	} else if( op == AV_LOCK_RELEASE ) {
		auto m = static_cast< QMutex * >( *mutex );
		m->unlock();
	} else if( op == AV_LOCK_DESTROY ) {
		auto m = static_cast< QMutex * >( *mutex );
		delete m;
		*mutex = nullptr;
	} else {
		assert( !"undefined AVLockOp" );
	}
	return 0;
}

}

using khopper::codec::Reader;
#ifdef Q_OS_WIN32
using khopper::libav::read_packet;
using khopper::libav::write_packet;
using khopper::libav::seek;
#endif
using khopper::plugin::LibavPlugin;
using khopper::widget::WriterPanelContext;
using khopper::widget::WavPanel;

LibavPlugin::LibavPlugin():
Plugin(),
panel_( new WavPanel ) {
	this->setID( KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID) );
	this->setVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
}

void LibavPlugin::doInstall() {
	WriterPanelContext::instance().install( this->panel_.get() );

	av_register_all();
	avformat_network_init();
	av_lockmgr_register( locker );

	Reader::install( this->getID(), []( const QUrl & uri )->unsigned int {
		AVFormatContext * pFC = NULL;
#ifdef Q_OS_WIN32
		QFile opaque;
		std::shared_ptr< AVIOContext > pb;
		if( uri.scheme() == "file" ) {
			opaque.setFileName( uri.toLocalFile() );
			if( !opaque.open( QIODevice::ReadOnly ) ) {
				return 0;
			}
			// NOTE will free by avformat_close_input
			pFC = avformat_alloc_context();
			const int SIZE = 4 * 1024 * sizeof( unsigned char );
			unsigned char * buffer = static_cast< unsigned char * >( av_malloc( SIZE ) );
			pFC->pb = avio_alloc_context( buffer, SIZE, 0, &opaque, read_packet, write_packet, seek );
			pb.reset( pFC->pb, [&opaque]( AVIOContext * pb ) {
				opaque.close();
				av_free( pb->buffer );
				av_free( pb );
			} );
		}
#endif
		if( avformat_open_input( &pFC, uri.toString().toUtf8().constData(), NULL, NULL ) == 0 ) {
			int ret = avformat_find_stream_info( pFC, NULL );
			avformat_close_input( &pFC );
			if( ret < 0 ) {
				char msg[1024];
				av_strerror( ret, msg, sizeof( msg ) );
				qDebug() << msg;
				return 0;
			}
			qDebug() << "returned 100 (from LibavReader)";
			return 100;
		}
		return 0;
	}, []( const QUrl & uri )->khopper::codec::ReaderSP {
		return khopper::codec::ReaderSP( new khopper::codec::LibavReader( uri ) );
	} );
}

void LibavPlugin::doUninstall() {
	Reader::uninstall( this->getID() );
	WriterPanelContext::instance().uninstall( this->panel_.get() );

	avformat_network_deinit();
}
