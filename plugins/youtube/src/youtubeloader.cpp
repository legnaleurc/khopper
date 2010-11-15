/**
 * @file youtubeloader.cpp
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
#include "youtubeloader.hpp"

#include <QtCore/QEventLoop>
#include <QtCore/QFile>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <algorithm>

using namespace khopper::plugin;
using khopper::widget::YouTubeDialog;

VideoParameter YouTubeLoader::parse( const QString & content ) {
	VideoParameter param;

	QRegExp pattern( "\\&video_id=([^(\\&|$)]*)" );
	pattern.indexIn( content, 0 );
	param.id = pattern.cap( 1 );

	pattern.setPattern( "\\&t=([^(\\&|$)]*)" );
	pattern.indexIn( content, 0 );
	param.ticket = pattern.cap( 1 );

	pattern.setPattern( "\\&fmt_url_map=([^(\\&|$)]*)" );
	pattern.indexIn( content, 0 );
	QString videoFormats( pattern.cap( 1 ) );

	//QString videoTitle( this->page_.mainFrame()->title() );

	QStringList videoFormatsGroup( videoFormats.split( "%2C" ) );
	std::for_each( videoFormatsGroup.begin(), videoFormatsGroup.end(), [&]( const QString & format ) {
		QStringList pair( format.split( "%7C" ) );
		param.formatURLs.insert( std::make_pair( pair[0], QUrl::fromEncoded( pair[1].toUtf8() ) ) );
	} );

	return param;
}

YouTubeLoader::YouTubeLoader():
dialog_( new YouTubeDialog ),
downloadLink_( new QNetworkAccessManager( this ) ),
formats_(),
parseLink_( new QNetworkAccessManager( this ) ),
progress_( new QProgressDialog() ) {
	// '5':'FLV 240p','18':'MP4 360p','22':'MP4 720p (HD)','34':'FLV 360p','35':'FLV 480p','37':'MP4 1080p (HD)','38':'MP4 Original (HD)','43':'WebM 480p','45':'WebM 720p (HD)'
	// '5':'flv','18':'mp4','22':'mp4','34':'flv','35':'flv','37':'mp4','38':'mp4','43':'webm','45':'webm'
	this->formats_.insert( std::make_pair( "5", std::make_pair( "flv", "FLV 240p" ) ) );
	this->formats_.insert( std::make_pair( "18", std::make_pair( "mp4", "MP4 360p" ) ) );
	this->formats_.insert( std::make_pair( "22", std::make_pair( "mp4", "MP4 720p (HD)" ) ) );
	this->formats_.insert( std::make_pair( "34", std::make_pair( "flv", "FLV 360p" ) ) );
	this->formats_.insert( std::make_pair( "35", std::make_pair( "flv", "FLV 480p" ) ) );
	this->formats_.insert( std::make_pair( "37", std::make_pair( "mp4", "MP4 1080p (HD)" ) ) );
	this->formats_.insert( std::make_pair( "38", std::make_pair( "mp4", "MP4 Original (HD)" ) ) );
	this->formats_.insert( std::make_pair( "43", std::make_pair( "webm", "WebM 480p" ) ) );
	this->formats_.insert( std::make_pair( "45", std::make_pair( "webm", "WebM 720p (HD)" ) ) );
	QObject::connect( this->parseLink_, SIGNAL( finished( QNetworkReply * ) ), this, SLOT( parse_( QNetworkReply * ) ) );
	QObject::connect( this->downloadLink_, SIGNAL( finished( QNetworkReply * ) ), this, SLOT( download_( QNetworkReply * ) ) );
}

QUrl YouTubeLoader::load( const QUrl & uri ) {
	YouTubeLoader self;
	self.parseLink_->get( QNetworkRequest( uri ) );
	QEventLoop wait;
	wait.connect( &self, SIGNAL( finished() ), SLOT( quit() ) );
	wait.exec();
	return QUrl::fromLocalFile( self.dialog_->getLocalLocation() );
}

void YouTubeLoader::download_( QNetworkReply * reply ) {
	this->progress_->setRange( 0, 0 );
	QObject::connect( reply, SIGNAL( downloadProgress( qint64, qint64 ) ), this, SLOT( updateProgress_( qint64, qint64 ) ) );
	QFile fout( this->dialog_->getLocalLocation() );
	fout.open( QIODevice::WriteOnly );

	while( !reply->atEnd() ) {
		fout.write( reply->read( reply->readBufferSize() ), reply->readBufferSize() );
	}

	fout.close();

	reply->disconnect( this );
	emit this->finished();
}

void YouTubeLoader::parse_( QNetworkReply * reply ) {
	this->progress_->setRange( 0, 0 );

	QObject::connect( reply, SIGNAL( downloadProgress( qint64, qint64 ) ), this, SLOT( updateProgress_( qint64, qint64 ) ) );

	VideoParameter param( YouTubeLoader::parse( reply->readAll() ) );

	reply->disconnect( this );

	std::for_each( this->formats_.begin(), this->formats_.end(), [&]( const std::pair< QString, std::pair< QString, QString > > & format ) {
		if( format.first == "5" && ( param.formatURLs.find( "34" ) != param.formatURLs.end() || param.formatURLs.find( "35" ) != param.formatURLs.end() ) ) {
			return;
		}
		if( param.formatURLs.find( format.first ) != param.formatURLs.end() ) {
			this->dialog_->addFormat( format.first );
		}
	} );
	if( this->dialog_->exec() != QDialog::Accepted ) {
		return;
	}
	this->downloadLink_->get( QNetworkRequest( param.formatURLs[this->dialog_->getSelectedFormat()] ) );
	this->dialog_->clearFormat();
}

void YouTubeLoader::updateProgress_( qint64 bytesReceived, qint64 bytesTotal ) {
	this->progress_->setMaximum( bytesTotal );
	this->progress_->setValue( bytesReceived );
}
