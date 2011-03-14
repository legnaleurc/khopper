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

using namespace khopper::album;

FreeDB::FreeDB() : QObject( 0 ), link_( new QTcpSocket( this ) ) {
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

FreeDB::ResponseType FreeDB::sendRequest_( const QByteArray & cmd ) {
	this->link_->write( cmd );
	return this->getResponse_();
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
