/**
 * @file freedb.hpp
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
#ifndef KHOPPER_PLUGIN_FREEDB_HPP
#define KHOPPER_PLUGIN_FREEDB_HPP

#include <QtNetwork/QTcpSocket>
#include <QtCore/QStringList>

#include <vector>
#include <utility>

namespace khopper {
	namespace album {

		struct TrackData {
			QString title;
			QString artist;
			QString ext;
		};
		struct DiscData {
			QString discid;
			QString title;
			QString artist;
			QString year;
			QString genre;
			QString ext;
			QMap< int, TrackData > tracks;
		};

		class FreeDB : public QObject {
			Q_OBJECT
		public:
			FreeDB();
			virtual ~FreeDB();

			bool isConnected() const;
			bool connectToHost( const QString & hostName, quint16 port );
			void disconnectFromHost();
			std::pair< QString, QString > query( unsigned int discid, const QStringList & frames, int nsecs );
			DiscData read( const QString & categ, const QString & discid );

		signals:
			void error( const QString & msg );

		private slots:
			void onError_( QAbstractSocket::SocketError );

		private:
			QStringList sendRequest_( const QByteArray & );
			QStringList getResponse_();

			QTcpSocket * link_;
		};

	}
}

#endif
