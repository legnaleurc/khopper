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
#include "youtubeplugin.hpp"
#include "khopper/application.hpp"
#include "khopper/error.hpp"

#include <QtCore/QDir>
#include <QtCore/QEventLoop>
#include <QtCore/QHashIterator>
#include <QtCore/QtDebug>
#include <QtNetwork/QNetworkRequest>

#include <algorithm>

using namespace khopper::plugin;
using khopper::error::RunTimeError;

YouTubeLoader::ErrorTable & YouTubeLoader::errorString() {
	static YouTubeLoader::ErrorTable errorString_;
	return errorString_;
}

YouTubeLoader::FormatTable & YouTubeLoader::formats() {
	static YouTubeLoader::FormatTable formats_;
	return formats_;
}

YouTubeLoader::YouTubeLoader( const QUrl & uri, YouTubePlugin * parent ):
content_(),
curParam_(),
downloadURI_(),
fout_(),
link_( NULL ),
linker_( new QNetworkAccessManager( this ) ),
needGo_( true ),
origURI_( uri ),
parent_( parent ) {
	this->connect( khopper::pApp(), SIGNAL( errorOccured( const QString &, const QString & ) ), SIGNAL( errorOccured( const QString &, const QString & ) ) );
}

void YouTubeLoader::parseHeader( bool display ) {
	this->curParam_ = YouTubeLoader::parseHeader_( this->getHeader_( display ) );
}

QString YouTubeLoader::getTitle() const {
	return this->curParam_.title;
}

QUrl YouTubeLoader::getRealURI( const QString & format ) const {
	std::map< QString, QUrl >::const_iterator it( this->curParam_.formatURLs.find( format ) );
	if( it != this->curParam_.formatURLs.end() ) {
		return it->second;
	} else {
		return QUrl();
	}
}

QString YouTubeLoader::getHeader_( bool display ) {
	if( display ) {
		this->parent_->getProgress()->setValue( 0 );
		this->parent_->getProgress()->setLabelText( tr( "Downloading content" ) );
		this->parent_->getProgress()->show();
	}
	this->link_ = this->linker_->get( QNetworkRequest( this->origURI_ ) );
	this->connect( this->link_, SIGNAL( error( QNetworkReply::NetworkError ) ), SLOT( onError_( QNetworkReply::NetworkError ) ) );
	if( display ) {
		this->connect( this->link_, SIGNAL( readyRead() ), SLOT( read_() ) );
		this->connect( this->link_, SIGNAL( downloadProgress( qint64, qint64 ) ), SLOT( updateProgress_( qint64, qint64 ) ) );
		this->connect( this->link_, SIGNAL( finished() ), SLOT( finish_() ) );
	} else {
		this->connect( this->linker_, SIGNAL( finished( QNetworkReply * ) ), SLOT( finish_( QNetworkReply * ) ) );
	}
	QEventLoop wait;
	wait.connect( this, SIGNAL( finished() ), SLOT( quit() ) );
	wait.exec();

	return QString::fromUtf8( this->content_ );
}

QString YouTubeLoader::selectFormat() {
	for( QHashIterator< YouTubeLoader::FormatTable::key_type, YouTubeLoader::FormatTable::mapped_type > it( YouTubeLoader::formats() ); it.hasNext(); ) {
		it.next();
		if( it.key() == "5" && ( this->curParam_.formatURLs.find( "34" ) != this->curParam_.formatURLs.end() || this->curParam_.formatURLs.find( "35" ) != this->curParam_.formatURLs.end() ) ) {
			continue;
		}
		if( this->curParam_.formatURLs.find( it.key() ) != this->curParam_.formatURLs.end() ) {
			this->parent_->getDialog()->addFormat( it.key() );
		}
	}
	if( this->parent_->getDialog()->exec() != QDialog::Accepted ) {
		// FIXME
		return QString();
	}
	return this->parent_->getDialog()->getSelectedFormat();
}

YouTubeLoader::Parameter YouTubeLoader::parseHeader_( const QString & header ) {
	YouTubeLoader::Parameter param;

	QRegExp pattern( "\\&amp;video_id=([^(\\&|$)]*)" );
	pattern.indexIn( header, 0 );
	param.id = pattern.cap( 1 );

	pattern.setPattern( "\\&amp;t=([^(\\&|$)]*)" );
	pattern.indexIn( header, 0 );
	param.ticket = pattern.cap( 1 );

	pattern.setPattern( "\\&amp;fmt_url_map=([^(\\&|$)]*)" );
	pattern.indexIn( header, 0 );
	QString videoFormats( pattern.cap( 1 ) );

	pattern.setPattern( "<title>\\s*YouTube\\s*\\-\\s+(.*)</title>" );
	pattern.setMinimal( true );
	qDebug() << "pattern" << pattern.isValid();
	pattern.indexIn( header, 0 );
	qDebug() << pattern.capturedTexts();
	param.title = pattern.cap( 1 ).trimmed().replace( QDir::separator(), '_' );
	qDebug() << param.title;

	QStringList videoFormatsGroup( videoFormats.split( "%2C" ) );
	std::for_each( videoFormatsGroup.begin(), videoFormatsGroup.end(), [&param]( const QString & format ) {
		QStringList pair( format.split( "%7C" ) );
		if( pair.size() < 2 ) {
			throw RunTimeError( QObject::tr( "Failed to fetch video from YouTube" ) );
		}
		param.formatURLs.insert( std::make_pair( pair[0], QUrl::fromEncoded( QUrl::fromPercentEncoding( pair[1].toUtf8() ).toUtf8() ) ) );
	} );

	return param;
}

//QUrl YouTubeLoader::getRealUri( const QUrl & uri, const QString & format ) {
//	YouTubeLoader self( uri );

//	VideoParameter param( self.parseHeader( self.getHeader( false ) ) );

//	return param.formatURLs[format];
//}

//QUrl YouTubeLoader::download( const QUrl & uri ) {
//	YouTubeLoader self( uri );

//	VideoParameter param( self.parseHeader( self.getHeader( true ) ) );

//	for( QHashIterator< YouTubeLoader::FormatTable::key_type, YouTubeLoader::FormatTable::mapped_type > it( YouTubeLoader::formats() ); it.hasNext(); ) {
//		it.next();
//		if( it.key() == "5" && ( param.formatURLs.find( "34" ) != param.formatURLs.end() || param.formatURLs.find( "35" ) != param.formatURLs.end() ) ) {
//			continue;
//		}
//		if( param.formatURLs.find( it.key() ) != param.formatURLs.end() ) {
//			self.dialog_->addFormat( it.key() );
//		}
//	}
//	if( self.dialog_->exec() != QDialog::Accepted ) {
//		// FIXME
//		return QUrl();
//	}

//	self.downloadURI_ = param.formatURLs[self.dialog_->getSelectedFormat()];
//	QString ext( YouTubeLoader::formats()[self.dialog_->getSelectedFormat()] );
//	self.dialog_->clearFormat();

//	QString path( self.dialog_->getLocalLocation() + "/" + param.title + "." + ext );
//	qDebug() << param.title << path;
//	while( self.needGo_ ) {
//		qDebug() << self.downloadURI_;
//		qDebug() << self.downloadURI_.host() << self.downloadURI_.queryItems();
//		self.fout_.setFileName( path );
//		self.fout_.open( QIODevice::WriteOnly );
//		self.progress_->setValue( 0 );
//		self.progress_->show();
//		self.transfer_ = self.link_->get( QNetworkRequest( self.downloadURI_ ) );
//		self.connect( self.transfer_, SIGNAL( error( QNetworkReply::NetworkError ) ), SLOT( onError_( QNetworkReply::NetworkError ) ) );
//		self.connect( self.transfer_, SIGNAL( readyRead() ), SLOT( readAndWrite_() ) );
//		self.connect( self.transfer_, SIGNAL( downloadProgress( qint64, qint64 ) ), SLOT( updateProgress_( qint64, qint64 ) ) );
//		self.connect( self.transfer_, SIGNAL( finished() ), SLOT( finishDownload_() ) );
//		wait.exec();
//	}

//	return QUrl::fromLocalFile( path );
//}

void YouTubeLoader::read_() {
	this->content_.append( this->link_->readAll() );
}

void YouTubeLoader::updateProgress_( qint64 bytesReceived, qint64 bytesTotal ) {
	this->parent_->getProgress()->setMaximum( bytesTotal );
	this->parent_->getProgress()->setValue( bytesReceived );
}

void YouTubeLoader::finish_() {
	this->link_->deleteLater();
	this->parent_->getProgress()->hide();
	emit this->finished();
}

void YouTubeLoader::finishDownload_() {
	this->parent_->getProgress()->hide();
	this->fout_.close();
	QUrl redirectURI = this->link_->attribute( QNetworkRequest::RedirectionTargetAttribute ).toUrl();
	qDebug() << this->link_->attribute( QNetworkRequest::HttpStatusCodeAttribute ) << redirectURI;
	if( redirectURI.isEmpty() || redirectURI == this->downloadURI_ ) {
		this->needGo_ = false;
	}
	this->link_->deleteLater();
	emit this->finished();
}

void YouTubeLoader::readAndWrite_() {
	this->fout_.write( this->link_->readAll() );
}

void YouTubeLoader::onError_( QNetworkReply::NetworkError code ) {
	emit this->errorOccured( tr( "Network Error" ), YouTubeLoader::errorString()[code] );
}

void YouTubeLoader::finish_( QNetworkReply * reply ) {
	this->content_ = reply->readAll();
	reply->deleteLater();
	emit this->finished();
}
