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
#include "khopper/application.hpp"

#include <QtCore/QEventLoop>
#include <QtCore/QHashIterator>
#include <QtCore/QtDebug>
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

	pattern.setPattern( "<title>\\s*YouTube\\s*\\-\\s+(.*)</title>" );
	pattern.setMinimal( true );
	qDebug() << "pattern" << pattern.isValid();
	pattern.indexIn( content, 0 );
	param.title = pattern.cap( 1 ).trimmed();
	qDebug() << pattern.capturedTexts();

	QStringList videoFormatsGroup( videoFormats.split( "%2C" ) );
	std::for_each( videoFormatsGroup.begin(), videoFormatsGroup.end(), [&]( const QString & format ) {
		QStringList pair( format.split( "%7C" ) );
		param.formatURLs.insert( std::make_pair( pair[0], QUrl::fromEncoded( QUrl::fromPercentEncoding( pair[1].toUtf8() ).toUtf8() ) ) );
	} );

	return param;
}

YouTubeLoader::ErrorTable & YouTubeLoader::errorString() {
	static YouTubeLoader::ErrorTable errorString_;
	return errorString_;
}

YouTubeLoader::FormatTable & YouTubeLoader::formats() {
	static YouTubeLoader::FormatTable formats_;
	return formats_;
}

YouTubeLoader::YouTubeLoader():
content_(),
dialog_( new YouTubeDialog ),
downloadURI_(),
fout_(),
link_( new QNetworkAccessManager( this ) ),
needGo_( true ),
progress_( new QProgressDialog() ),
transfer_( NULL ) {
	this->progress_->setLabelText( tr( "Downloading progress" ) );
	this->connect( KHOPPER_APPLICATION, SIGNAL( errorOccured( const QString &, const QString & ) ), SIGNAL( errorOccured( const QString &, const QString & ) ) );
}

QUrl YouTubeLoader::load( const QUrl & uri ) {
	YouTubeLoader self;

	self.progress_->setValue( 0 );
	self.progress_->show();
	self.transfer_ = self.link_->get( QNetworkRequest( uri ) );
	self.connect( self.transfer_, SIGNAL( error( QNetworkReply::NetworkError ) ), SLOT( onError_( QNetworkReply::NetworkError ) ) );
	self.connect( self.transfer_, SIGNAL( readyRead() ), SLOT( read_() ) );
	self.connect( self.transfer_, SIGNAL( downloadProgress( qint64, qint64 ) ), SLOT( updateProgress_( qint64, qint64 ) ) );
	self.connect( self.transfer_, SIGNAL( finished() ), SLOT( finish_() ) );
	QEventLoop wait;
	wait.connect( &self, SIGNAL( finished() ), SLOT( quit() ) );
	wait.exec();

	VideoParameter param( YouTubeLoader::parse( self.content_ ) );

	for( QHashIterator< YouTubeLoader::FormatTable::key_type, YouTubeLoader::FormatTable::mapped_type > it( YouTubeLoader::formats() ); it.hasNext(); ) {
		it.next();
		if( it.key() == "5" && ( param.formatURLs.find( "34" ) != param.formatURLs.end() || param.formatURLs.find( "35" ) != param.formatURLs.end() ) ) {
			continue;
		}
		if( param.formatURLs.find( it.key() ) != param.formatURLs.end() ) {
			self.dialog_->addFormat( it.key() );
		}
	}
	if( self.dialog_->exec() != QDialog::Accepted ) {
		// FIXME
		return QUrl();
	}
	self.downloadURI_ = param.formatURLs[self.dialog_->getSelectedFormat()];
	QString ext( YouTubeLoader::formats()[self.dialog_->getSelectedFormat()] );
	self.dialog_->clearFormat();

	QString path( self.dialog_->getLocalLocation() + "/" + param.title + "." + ext );
	qDebug() << param.title << path;
	while( self.needGo_ ) {
		qDebug() << self.downloadURI_;
		qDebug() << self.downloadURI_.host() << self.downloadURI_.queryItems();
		self.fout_.setFileName( path );
		self.fout_.open( QIODevice::WriteOnly );
		self.progress_->setValue( 0 );
		self.progress_->show();
		self.transfer_ = self.link_->get( QNetworkRequest( self.downloadURI_ ) );
		self.connect( self.transfer_, SIGNAL( error( QNetworkReply::NetworkError ) ), SLOT( onError_( QNetworkReply::NetworkError ) ) );
		self.connect( self.transfer_, SIGNAL( readyRead() ), SLOT( readAndWrite_() ) );
		self.connect( self.transfer_, SIGNAL( downloadProgress( qint64, qint64 ) ), SLOT( updateProgress_( qint64, qint64 ) ) );
		self.connect( self.transfer_, SIGNAL( finished() ), SLOT( finishDownload_() ) );
		wait.exec();
	}

	return QUrl::fromLocalFile( path );
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
	this->progress_->hide();
	emit this->finished();
}

void YouTubeLoader::finishDownload_() {
	this->progress_->hide();
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

void YouTubeLoader::onError_( QNetworkReply::NetworkError code ) {
	emit this->errorOccured( tr( "Network Error" ), YouTubeLoader::errorString()[code] );
}
