/**
 * @file youtubereader.hpp
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
#ifndef KHOPPER_CODEC_YOUTUBEREADER_HPP
#define KHOPPER_CODEC_YOUTUBEREADER_HPP

#include "khopper/abstractreader.hpp"

#include <QtCore/QMutex>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

namespace khopper {
	namespace codec {

		class YouTubeReader : public AbstractReader {
			Q_OBJECT
		public:
			explicit YouTubeReader( const QUrl & uri );

			virtual bool isSequential() const;

		signals:
			void finished();

		protected:
			virtual void doOpen();
			virtual void doClose();
			virtual qint64 readData( char * data, qint64 maxSize );

		private slots:
			void onError_( QNetworkReply::NetworkError );
			void read_();
			void finish_();

		private:
			QByteArray buffer_;
			QNetworkReply * link_;
			QNetworkAccessManager * linker_;
			QMutex lock_;
		};

	}
}

#endif
