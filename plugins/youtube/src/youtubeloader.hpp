/**
 * @file youtubeloader.hpp
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
#ifndef KHOPPER_PLUGIN_YOUTUBELOADER_HPP
#define KHOPPER_PLUGIN_YOUTUBELOADER_HPP

#include "youtubedialog.hpp"

#include <QtCore/QFile>
#include <QtCore/QHash>
#include <QtCore/QUrl>
#include <QtGui/QProgressDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

namespace khopper {
	namespace plugin {

		class YouTubePlugin;

		class YouTubeLoader : public QObject {
			Q_OBJECT
		public:
			typedef QHash< QString, QString > FormatTable;
			typedef QHash< QNetworkReply::NetworkError, QString > ErrorTable;

			static ErrorTable & errorString();
			static FormatTable & formats();
//			static Parameter getParameter( const QUrl & uri, bool display );
//			static QString selectFormat( const QUrl & uri );
//			static QUrl download( const QUrl & url );
//			static QUrl getRealUri( const QUrl & uri, const QString & format );

			YouTubeLoader( const QUrl & uri, YouTubePlugin * parent );

			void parseHeader( bool display );
			QString getTitle() const;
			QString selectFormat();
			QUrl getRealURI( const QString & format ) const;

		signals:
			void finished();
			void errorOccured( const QString & title, const QString & message );

		private slots:
			void finish_();
			void finish_( QNetworkReply * );
			void finishDownload_();
			void onError_( QNetworkReply::NetworkError );
			void read_();
			void readAndWrite_();
			void updateProgress_( qint64, qint64 );

		private:
			struct Parameter {
				QString title;
				QString id;
				QString ticket;
				std::map< QString, QUrl > formatURLs;
			};

			static Parameter parseHeader_( const QString & header );
			QString getHeader_( bool display );

			QByteArray content_;
			Parameter curParam_;
			QUrl downloadURI_;
			QFile fout_;
			QNetworkReply * link_;
			QNetworkAccessManager * linker_;
			bool needGo_;
			QUrl origURI_;
			YouTubePlugin * parent_;
		};

	}
}

#endif
