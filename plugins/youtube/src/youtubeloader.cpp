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
#include <QtCore/QtDebug>
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
content_(),
dialog_( new YouTubeDialog ),
downloadURI_(),
formats_(),
fout_(),
link_( new QNetworkAccessManager( this ) ),
needGo_( true ),
progress_( new QProgressDialog() ),
transfer_( NULL ) {
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
}

QUrl YouTubeLoader::load( const QUrl & uri ) {
	YouTubeLoader self;

	self.progress_->setRange( 0, 0 );
	self.transfer_ = self.link_->get( QNetworkRequest( uri ) );
	self.connect( self.transfer_, SIGNAL( readyRead() ), SLOT( read_() ) );
	self.connect( self.transfer_, SIGNAL( downloadProgress( qint64, qint64 ) ), SLOT( updateProgress_( qint64, qint64 ) ) );
	self.connect( self.transfer_, SIGNAL( finished() ), SLOT( finish_() ) );
	QEventLoop wait;
	wait.connect( &self, SIGNAL( finished() ), SLOT( quit() ) );
	wait.exec();

	self.downloadURI_ = self.parse_();
	while( self.needGo_ ) {
		qDebug() << self.downloadURI_;
		self.fout_.setFileName( self.dialog_->getLocalLocation() );
		self.fout_.open( QIODevice::WriteOnly );
		self.progress_->setRange( 0, 0 );
		self.transfer_ = self.link_->get( QNetworkRequest( self.downloadURI_ ) );
		self.connect( self.transfer_, SIGNAL( readyRead() ), SLOT( readAndWrite_() ) );
		self.connect( self.transfer_, SIGNAL( downloadProgress( qint64, qint64 ) ), SLOT( updateProgress_( qint64, qint64 ) ) );
		self.connect( self.transfer_, SIGNAL( finished() ), SLOT( finishDownload_() ) );
		QEventLoop wait2;
		wait2.connect( &self, SIGNAL( finished() ), SLOT( quit() ) );
		wait2.exec();
	}

	return QUrl::fromLocalFile( self.dialog_->getLocalLocation() );
}

void YouTubeLoader::read_() {
	this->content_.append( this->transfer_->readAll() );
}

void YouTubeLoader::updateProgress_( qint64 bytesReceived, qint64 bytesTotal ) {
	this->progress_->setMaximum( bytesTotal );
	this->progress_->setValue( bytesReceived );
}

void YouTubeLoader::finish_() {
	this->transfer_->deleteLater();
	emit this->finished();
}

void YouTubeLoader::finishDownload_() {
	this->fout_.close();
	QUrl redirectURI = this->transfer_->attribute( QNetworkRequest::RedirectionTargetAttribute ).toUrl();
	qDebug() << this->transfer_->attribute( QNetworkRequest::HttpStatusCodeAttribute ) << redirectURI;
	if( redirectURI.isEmpty() || redirectURI == this->downloadURI_ ) {
		this->needGo_ = false;
	}
	this->transfer_->deleteLater();
	emit this->finished();
}

void YouTubeLoader::readAndWrite_() {
	this->fout_.write( this->transfer_->readAll() );
}

QUrl YouTubeLoader::parse_() {
	VideoParameter param( YouTubeLoader::parse( this->content_ ) );

	std::for_each( this->formats_.begin(), this->formats_.end(), [&]( const std::pair< QString, std::pair< QString, QString > > & format ) {
		if( format.first == "5" && ( param.formatURLs.find( "34" ) != param.formatURLs.end() || param.formatURLs.find( "35" ) != param.formatURLs.end() ) ) {
			return;
		}
		if( param.formatURLs.find( format.first ) != param.formatURLs.end() ) {
			this->dialog_->addFormat( format.first );
		}
	} );
	if( this->dialog_->exec() != QDialog::Accepted ) {
		return QUrl();
	}
	QUrl result( param.formatURLs[this->dialog_->getSelectedFormat()] );
	this->dialog_->clearFormat();
	return result;
}
