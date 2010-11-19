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

		struct VideoParameter {
			QString title;
			QString id;
			QString ticket;
			std::map< QString, QUrl > formatURLs;
		};

		class YouTubeLoader : public QObject {
			Q_OBJECT
		public:
			typedef QHash< QString, QString > FormatTable;
			typedef QHash< QNetworkReply::NetworkError, QString > ErrorTable;

			static ErrorTable & errorString();
			static FormatTable & formats();
			static QString getFormat( const QUrl & uri );
			static QUrl load( const QUrl & url );
			static VideoParameter parse( const QString & content );

			YouTubeLoader();

		signals:
			void finished();
			void errorOccured( const QString & title, const QString & message );

		private slots:
			void finish_();
			void finishDownload_();
			void onError_( QNetworkReply::NetworkError );
			void read_();
			void readAndWrite_();
			void updateProgress_( qint64, qint64 );

		private:
			QByteArray content_;
			std::shared_ptr< widget::YouTubeDialog > dialog_;
			QUrl downloadURI_;
			QFile fout_;
			QNetworkAccessManager * link_;
			bool needGo_;
			std::shared_ptr< QProgressDialog > progress_;
			QNetworkReply * transfer_;
		};

	}
}

#endif
