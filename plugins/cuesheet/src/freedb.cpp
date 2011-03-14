/**
 * @file freedb.cpp
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
#include "freedb.hpp"

#include <QtCore/QtDebug>
#include <QtCore/QRegExp>

using namespace khopper::album;

FreeDB::FreeDB( unsigned int discid, const QStringList & frames, int nsecs ):
QObject( 0 ),
link_( new QTcpSocket( this ) ),
discid_( discid ),
frames_( frames ),
nsecs_( nsecs ) {
	this->connect( this->link_, SIGNAL( connected() ), SLOT( onConnected_() ) );
	this->connect( this->link_, SIGNAL( error( QAbstractSocket::SocketError ) ), SLOT( onError_( QAbstractSocket::SocketError ) ) );
}

void FreeDB::start() {
	this->link_->connectToHost( "freedb.freedb.org", 8880 );
}

void FreeDB::onConnected_() {
	qDebug() << "connected to host";
	ResponseType msg( this->getResponse_() );
	foreach( QByteArray m, msg ) {
		qDebug() << m;
	}

	msg = this->sendRequest_( "proto 6\n" );
	foreach( QByteArray m, msg ) {
		qDebug() << m;
	}

	msg = this->sendRequest_( "cddb hello \"legnaleurc\" \"foolproofproject.org\" \"Khopper\" \"0.3\"\n" );
	foreach( QByteArray m, msg ) {
		qDebug() << m;
	}
	
	QString tmp( "cddb query \"%1\" \"%2\" %3 \"%4\"\n" );
	msg = this->sendRequest_( tmp.arg( this->discid_, 8, 16, QChar( '0' ) ).arg( this->frames_.size() ).arg( this->frames_.join( " " ) ).arg( this->nsecs_ ).toUtf8() );
	foreach( QByteArray m, msg ) {
		qDebug() << m;
	}
	QRegExp pattern( "(\\d\\d\\d) (\\w+) ([\\w\\d]+) (.+)" );
	if( pattern.exactMatch( QString::fromUtf8( msg[0] ) ) && pattern.cap( 1 ) == "200" ) {
		tmp = "cddb read \"%1\" \"%2\"\n";
		msg = this->sendRequest_( tmp.arg( pattern.cap( 2 ) ).arg( pattern.cap( 3 ) ).toUtf8(), true );
		foreach( QByteArray m, msg ) {
			qDebug() << m;
		}
	} else {
		qDebug() << "not match";
	}

	msg = this->sendRequest_( "quit\n" );
	foreach( QByteArray m, msg ) {
		qDebug() << m;
	}
	this->link_->disconnect();
	emit this->finished();
}

void FreeDB::onError_( QAbstractSocket::SocketError socketError ) {
	qDebug() << socketError;
	qDebug() << this->link_->errorString();
}

FreeDB::ResponseType FreeDB::sendRequest_( const QByteArray & cmd, bool dot ) {
	this->link_->write( cmd );
	if( !dot ) {
		return this->getResponse_();
	} else {
		ResponseType result( this->getResponse_() );
		while( result.back() != ".\r\n" ) {
			ResponseType tmp( this->getResponse_() );
			unsigned int i = 0;
			if( !result.back().endsWith( "\r\n" ) ) {
				result.back().append( tmp[i] );
				++i;
			}
			for( ; i < tmp.size(); ++i ) {
				result.push_back( tmp[i] );
			}
		}
		return result;
	}
}

FreeDB::ResponseType FreeDB::getResponse_() {
	if( !this->link_->waitForReadyRead() ) {
		qDebug() << "TimeOut";
		return ResponseType();
	}
	ResponseType msg;
	while( !this->link_->atEnd() ) {
		msg.push_back( this->link_->readLine() );
	}
	return msg;
}
