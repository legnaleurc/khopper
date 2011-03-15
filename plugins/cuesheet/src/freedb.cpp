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

#include "khopper/error.hpp"

#include <QtCore/QtDebug>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>

using namespace khopper::album;
using khopper::error::RunTimeError;

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
	qDebug() << this->getResponse_();

	QStringList msg = this->sendRequest_( "proto 6\n" );
	qDebug() << msg;

	msg = this->sendRequest_( "cddb hello \"legnaleurc\" \"foolproofproject.org\" \"Khopper\" \"0.3\"\n" );
	qDebug() << msg;
	
	QString tmp( "cddb query \"%1\" \"%2\" %3 \"%4\"\n" );
	msg = this->sendRequest_( tmp.arg( this->discid_, 8, 16, QChar( '0' ) ).arg( this->frames_.size() ).arg( this->frames_.join( " " ) ).arg( this->nsecs_ ).toUtf8() );
	QRegExp pattern( "(\\d\\d\\d) (\\w+) ([\\w\\d]+) (.+)" );
	if( pattern.exactMatch( msg[0] ) && pattern.cap( 1 ) == "200" ) {
		tmp = "cddb read \"%1\" \"%2\"\n";
		msg = this->sendRequest_( tmp.arg( pattern.cap( 2 ) ).arg( pattern.cap( 3 ) ).toUtf8() );
		qDebug() << msg;
	} else {
		throw RunTimeError( QObject::tr( "no exact match" ) );
	}

	msg = this->sendRequest_( "quit\n" );
	qDebug() << msg;
	this->link_->disconnect();
	emit this->finished();
}

void FreeDB::onError_( QAbstractSocket::SocketError socketError ) {
	qDebug() << socketError;
	qDebug() << this->link_->errorString();
}

QStringList FreeDB::sendRequest_( const QByteArray & cmd ) {
	this->link_->write( cmd );

	if( !this->link_->waitForReadyRead() ) {
		throw RunTimeError( this->link_->errorString() );
	}
	QByteArray response( this->link_->readLine() );
	if( response[1] == '1' ) {
		response.append( this->link_->readAll() );
		// NOTE: works, should be more graceful
		while( !( response.endsWith( "\r.\r" ) || response.endsWith( "\n.\n" ) || response.endsWith( "\r\n.\r\n" ) ) ) {
			if( !this->link_->waitForReadyRead() ) {
				throw RunTimeError( this->link_->errorString() );
			}
			response.append( this->link_->readAll() );
		}
	}

	QTextStream sin( response );
	sin.setCodec( QTextCodec::codecForName( "UTF-8" ) );
	QStringList message;
	while( !sin.atEnd() ) {
		message.push_back( sin.readLine() );
	}
	return message;
}

QByteArray FreeDB::getResponse_() {
	if( !this->link_->waitForReadyRead() ) {
		throw RunTimeError( this->link_->errorString() );
	}
	return this->link_->readAll();
}
