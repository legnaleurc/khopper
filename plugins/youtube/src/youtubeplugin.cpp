/**
 * @file youtubeplugin.cpp
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
#include "youtubeplugin.hpp"
#include "youtubeloader.hpp"
#include "youtubereader.hpp"

#include <QtCore/QtPlugin>
#include <QtCore/QtDebug>

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::YouTubePlugin )

using namespace khopper::plugin;
using khopper::plugin::registerReader;
using khopper::plugin::unregisterReader;

YouTubePlugin::YouTubePlugin():
AbstractPlugin() {
	this->setID( KHOPPER_STRINGIZE( KHOPPER_PLUGIN_ID ) );
	this->setVersion( KHOPPER_STRINGIZE( KHOPPER_VERSION ) );
}

void YouTubePlugin::doInstall() {
	YouTubeLoader::FormatTable & formats( YouTubeLoader::formats() );
	// '5':'FLV 240p','18':'MP4 360p','22':'MP4 720p (HD)','34':'FLV 360p','35':'FLV 480p','37':'MP4 1080p (HD)','38':'MP4 Original (HD)','43':'WebM 480p','45':'WebM 720p (HD)'
	// '5':'flv','18':'mp4','22':'mp4','34':'flv','35':'flv','37':'mp4','38':'mp4','43':'webm','45':'webm'
	formats.insert( "5", "flv" );
	formats.insert( "18", "mp4" );
	formats.insert( "22", "mp4" );
	formats.insert( "34", "flv" );
	formats.insert( "35", "flv" );
	formats.insert( "37", "mp4" );
	formats.insert( "38", "mp4" );
	formats.insert( "43", "webm" );
	formats.insert( "45", "webm" );

	YouTubeLoader::ErrorTable & errorString( YouTubeLoader::errorString() );
	errorString.insert( QNetworkReply::NoError, tr( "no error" ) );
	errorString.insert( QNetworkReply::ConnectionRefusedError, tr( "connection refused" ) );
	errorString.insert( QNetworkReply::RemoteHostClosedError, tr( "remote host closed" ) );
	errorString.insert( QNetworkReply::HostNotFoundError, tr( "host not found" ) );
	errorString.insert( QNetworkReply::TimeoutError, tr( "time out" ) );
	errorString.insert( QNetworkReply::OperationCanceledError, tr( "operation canceled" ) );
	errorString.insert( QNetworkReply::SslHandshakeFailedError, tr( "SSL handshake failed" ) );
#if QT_VERSION >= 0x040700
	errorString.insert( QNetworkReply::TemporaryNetworkFailureError, tr( "temporary network failure" ) );
#endif
	errorString.insert( QNetworkReply::ProxyConnectionRefusedError, tr( "proxy connection refused" ) );
	errorString.insert( QNetworkReply::ProxyConnectionClosedError, tr( "proxy connection closed" ) );
	errorString.insert( QNetworkReply::ProxyNotFoundError, tr( "proxy not found" ) );
	errorString.insert( QNetworkReply::ProxyTimeoutError, tr( "proxy timeout" ) );
	errorString.insert( QNetworkReply::ProxyAuthenticationRequiredError, tr( "proxy authentication required" ) );
	errorString.insert( QNetworkReply::ContentAccessDenied, tr( "content access denied" ) );
	errorString.insert( QNetworkReply::ContentOperationNotPermittedError, tr( "content operation not permitted" ) );
	errorString.insert( QNetworkReply::ContentNotFoundError, tr( "content not found" ) );
	errorString.insert( QNetworkReply::AuthenticationRequiredError, tr( "authentication required" ) );
	errorString.insert( QNetworkReply::ContentReSendError, tr( "content re-send" ) );
	errorString.insert( QNetworkReply::ProtocolUnknownError, tr( "protocol unknown" ) );
	errorString.insert( QNetworkReply::ProtocolInvalidOperationError, tr( "protocol invalid operation" ) );
	errorString.insert( QNetworkReply::UnknownNetworkError, tr( "unknown network" ) );
	errorString.insert( QNetworkReply::UnknownProxyError, tr( "unknown proxy" ) );
	errorString.insert( QNetworkReply::UnknownContentError, tr( "unknown content" ) );
	errorString.insert( QNetworkReply::ProtocolFailure, tr( "protocol failure" ) );

	registerReader( this->getID(), []( const QUrl & uri )->int {
		if( uri.scheme() == "http" && uri.host() == "www.youtube.com" && uri.path() == "/watch" ) {
			qDebug() << Q_FUNC_INFO << "returned 200";
			return 200;
		}
		return 0;
	}, []( const QUrl & uri )->khopper::codec::ReaderSP {
		using namespace khopper::codec;
		return ReaderSP( new YouTubeReader( uri ) );
	} );
}

void YouTubePlugin::doUninstall() {
	unregisterReader( this->getID() );
	YouTubeLoader::errorString().clear();
	YouTubeLoader::formats().clear();
}
