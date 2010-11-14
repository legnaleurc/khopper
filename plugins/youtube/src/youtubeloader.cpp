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

#include <QtWebKit/QWebFrame>

#include <algorithm>

using namespace khopper::plugin;
using khopper::widget::YouTubeDialog;

YouTubeLoader::YouTubeLoader():
dialog_( new YouTubeDialog ),
formats_(),
page_() {
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
	QObject::connect( &this->page_, SIGNAL( loadFinished( bool ) ), this, SLOT( parse_( bool ) ) );
}

QUrl YouTubeLoader::operator ()( const QUrl & uri ) {
	this->page_.mainFrame()->load( uri );
	return QUrl();
}

void YouTubeLoader::parse_( bool /*finished*/ ) {
	QString content( this->page_.mainFrame()->toHtml() );

	QRegExp pattern( "\\&video_id=([^(\\&|$)]*)" );
	pattern.indexIn( content, 0 );
	QString videoID( pattern.cap( 1 ) );

	pattern.setPattern( "\\&t=([^(\\&|$)]*)" );
	pattern.indexIn( content, 0 );
	QString videoTicket( pattern.cap( 1 ) );

	pattern.setPattern( "\\&fmt_url_map=([^(\\&|$)]*)" );
	pattern.indexIn( content, 0 );
	QString videoFormats( pattern.cap( 1 ) );

	QString videoTitle( this->page_.mainFrame()->title() );

	QMap< QString, QUrl > videoURL;
	QStringList videoFormatsGroup( videoFormats.split( "%2C" ) );
	std::for_each( videoFormatsGroup.begin(), videoFormatsGroup.end(), [&]( const QString & format ) {
		QStringList pair( format.split( "%7C" ) );
		videoURL.insert( pair[0], QUrl::fromEncoded( pair[1].toUtf8() ) );
	} );

	std::for_each( this->formats_.begin(), this->formats_.end(), [&]( const std::pair< QString, std::pair< QString, QString > > & format ) {
		if( format.first == "5" && ( videoURL.contains( "34" ) || videoURL.contains( "35" ) ) ) {
			return;
		}
		if( videoURL.contains( format.first ) ) {
			this->dialog_->addFormat( format.first );
		}
	} );
	if( this->dialog_->exec() != QDialog::Accepted ) {
		return;
	}
	this->dialog_->clearFormat();
}
