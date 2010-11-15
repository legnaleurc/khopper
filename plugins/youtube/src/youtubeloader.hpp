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

#include <QtCore/QUrl>
#include <QtGui/QProgressDialog>
#include <QtNetwork/QNetworkAccessManager>

#include <map>

namespace khopper {
	namespace plugin {

		struct VideoParameter {
			QString id;
			QString ticket;
			std::map< QString, QUrl > formatURLs;
		};

		class YouTubeLoader : public QObject {
			Q_OBJECT
		public:
			static QUrl load( const QUrl & url );
			static VideoParameter parse( const QString & content );

			YouTubeLoader();

		signals:
			void finished();

		private slots:
			void download_( QNetworkReply * );
			void parse_( QNetworkReply * );
			void updateProgress_( qint64, qint64 );

		private:
			std::shared_ptr< widget::YouTubeDialog > dialog_;
			QNetworkAccessManager * downloadLink_;
			std::map< QString, std::pair< QString, QString > > formats_;
			QNetworkAccessManager * parseLink_;
			std::shared_ptr< QProgressDialog > progress_;
		};

	}
}

#endif
