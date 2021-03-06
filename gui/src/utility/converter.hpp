/**
 * @file converter.hpp
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
#ifndef KHOPPER_UTILITY_CONVERTER_HPP
#define KHOPPER_UTILITY_CONVERTER_HPP

#include <QtCore/QRunnable>

#include "khopper/writer.hpp"
#include "khopper/track.hpp"

namespace khopper {
namespace utility {

/**
 * @brief Controller of converting
 */
class Converter : public QObject, public QRunnable {
	Q_OBJECT

public:
	/**
	 * @brief Constructor
	 */
	Converter( album::TrackCSP track, codec::Writer::Creator creator, const QString & path );

	virtual void run();

	QString getTitle() const;
	qint64 getMaximumValue() const;

public slots:
	/**
	 * @brief Cancel converting
	 */
	void cancel();

signals:
	/**
	 * @brief Decoded duration
	 * @param ms Time in second * 1000
	 */
	void decodedTime( qint64 ms );
	void errorOccured( const QString & title, const QString & message );
	void finished();

private:
	QUrl getUniqueURI_() const;

	bool canceled_;
	codec::Writer::Creator creator_;
	album::TrackCSP track_;
	QString path_;
};

}
}

#endif
