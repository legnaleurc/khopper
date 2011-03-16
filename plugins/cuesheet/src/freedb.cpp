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

#include "khopper/application.hpp"
#include "khopper/error.hpp"

#include <QtCore/QtDebug>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>

namespace {
	void disconnectAndWarn( QTcpSocket * socket ) {
		if( socket->state() != QAbstractSocket::UnconnectedState && socket->state() != QAbstractSocket::ClosingState ) {
			socket->disconnectFromHost();
		}
		if( !socket->waitForDisconnected() ) {
			qWarning() << "disconnect from" << socket->peerName() << "failed";
		}
	}
}

using namespace khopper::album;
using khopper::error::IOError;
using khopper::error::RunTimeError;

FreeDB::FreeDB():
link_( new QTcpSocket, []( QTcpSocket * socket ) {
	disconnectAndWarn( socket );
	delete socket;
} ) {
}

FreeDB::~FreeDB() {
	if( this->isConnected() ) {
		this->disconnectFromHost();
	}
}

bool FreeDB::isConnected() const {
	return this->link_->state() == QAbstractSocket::ConnectedState;
}

void FreeDB::connectToHost( const QString & hostName, quint16 port ) {
	this->link_->connectToHost( hostName, port );
	if( !this->link_->waitForConnected() ) {
		throw IOError( this->link_->errorString() );
	}
	QStringList msg( this->getResponse_() );
	if( msg.empty() || msg.startsWith( "43" ) ) {
		disconnectAndWarn( this->link_.get() );
		return;
	}

	// set protocol level
	msg = this->sendRequest_( "proto 6" );
	// handshake
	QString tmp( "cddb hello \"legnaleurc\" \"foolproofproject.org\" \"%1\" \"%2\"" );
	msg = this->sendRequest_( tmp.arg( QCoreApplication::applicationName() ).arg( QCoreApplication::applicationVersion() ).toUtf8() );
}

void FreeDB::disconnectFromHost() {
	this->sendRequest_( "quit" );
	disconnectAndWarn( this->link_.get() );
}

std::pair< QString, QString > FreeDB::query( unsigned int discid, const QStringList & frames, int nsecs ) {
	QString tmp( "cddb query \"%1\" \"%2\" %3 \"%4\"" );
	QStringList msg( this->sendRequest_( tmp.arg( discid, 8, 16, QChar( '0' ) ).arg( frames.size() ).arg( frames.join( " " ) ).arg( nsecs ).toUtf8() ) );

	QRegExp pattern( "\\d\\d\\d (\\w+) ([\\w\\d]+) .+" );
	if( msg.startsWith( "200" ) && pattern.exactMatch( msg[0] ) ) {
		return std::make_pair( pattern.cap( 1 ), pattern.cap( 2 ) );
	} else if( msg.startsWith( "210" ) ) {
		// code 210, found exact matches, choose first one
		pattern.setPattern( "(\\w+) ([\\w\\d]+) .*" );
		qDebug() << msg[1];
		if( pattern.exactMatch( msg[1] ) ) {
			qDebug() << pattern.capturedTexts();
			return std::make_pair( pattern.cap( 1 ), pattern.cap( 2 ) );
		}
	}
	throw RunTimeError( "find no exact match from FreeDB" );
}

DiscData FreeDB::read( const QString & categ, const QString & discid ) {
	QString tmp( "cddb read \"%1\" \"%2\"" );
	QStringList msg( this->sendRequest_( tmp.arg( categ ).arg( discid ).toUtf8() ) );
	if( !msg[0].startsWith( "210" ) ) {
		throw RunTimeError( "unexcepted error" );
	}
	DiscData data;

	QRegExp pattern( "([A-Z]+)(\\d*)=(.*)" );
	for( int i = 1; i < msg.size(); ++i ) {
		if( !pattern.exactMatch( msg[i] ) ) {
			continue;
		}
		if( pattern.cap( 1 ) == "DISCID" ) {
			// i know the discid
		} else if( pattern.cap( 1 ) == "DTITLE" ) {
			QRegExp tmp( "(.+) / (.+)" );
			if( tmp.exactMatch( pattern.cap( 3 ) ) ) {
				data.artist = tmp.cap( 1 );
				data.title = tmp.cap( 2 );
			} else {
				data.title = pattern.cap( 3 );
			}
		} else if( pattern.cap( 1 ) == "DYEAR" ) {
			data.year = pattern.cap( 3 );
		} else if( pattern.cap( 1 ) == "DGENRE" ) {
			data.genre = pattern.cap( 3 );
		} else if( pattern.cap( 1 ) == "TTITLE" ) {
			int j = pattern.cap( 2 ).toInt();
			QMap< int, TrackData >::iterator it = data.tracks.find( j );
			if( it == data.tracks.end() ) {
				it = data.tracks.insert( j, TrackData() );
			}
			QRegExp tmp( "(.+) / (.+)" );
			if( tmp.exactMatch( pattern.cap( 3 ) ) ) {
				it->artist = tmp.cap( 1 );
				it->title = tmp.cap( 2 );
			} else {
				it->title = pattern.cap( 3 );
			}
		} else if( pattern.cap( 1 ) == "EXTD" ) {
			data.ext += pattern.cap( 3 );
		} else if( pattern.cap( 1 ) == "EXTT" ) {
			int j = pattern.cap( 2 ).toInt();
			QMap< int, TrackData >::iterator it = data.tracks.find( j );
			if( it == data.tracks.end() ) {
				it = data.tracks.insert( j, TrackData() );
			}
			it->ext += pattern.cap( 3 );
		} else if( pattern.cap( 1 ) == "PLAYORDER" ) {
			// not important
		} else {
			// should never happen
		}
	}
	return data;
}

QStringList FreeDB::sendRequest_( const QByteArray & cmd ) {
	qDebug() << cmd;
	this->link_->write( cmd + '\n' );

	return this->getResponse_();
}

QStringList FreeDB::getResponse_() {
	if( !this->link_->waitForReadyRead() ) {
		throw IOError( this->link_->errorString() );
	}

	QByteArray response( this->link_->readLine() );
	if( response[1] == '1' ) {
		response.append( this->link_->readAll() );
		// NOTE: works, should be more graceful
		while( !( response.endsWith( "\r.\r" ) || response.endsWith( "\n.\n" ) || response.endsWith( "\r\n.\r\n" ) ) ) {
			if( !this->link_->waitForReadyRead() ) {
				throw IOError( this->link_->errorString() );
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
	qDebug() << message;
	return message;
}
