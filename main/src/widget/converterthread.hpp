/**
 * @file converterthread.hpp
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
#ifndef KHOPPER_WIDGET_CONVERTERTHREAD_HPP
#define KHOPPER_WIDGET_CONVERTERTHREAD_HPP

#include "track.hpp"
#include "converter.hpp"
#include "codec/abstractwriter.hpp"

#include <QtCore/QStringList>
#include <QtCore/QThread>

namespace khopper {

	namespace widget {

		/**
		 * @brief Thread of converter
		 */
		class ConverterThread : public QThread {
			Q_OBJECT

		public:
			/**
			 * @brief Default constructor
			 * @param [in] parent parent object
			 */
			ConverterThread( QObject * parent = 0 );

			/**
			 * @brief Set output option
			 * @param [in] output Encoder object
			 * @param [in] paths Output file paths
			 */
			void setOutput( codec::WriterSP output, const QList< QString > & paths );

			/**
			 * @brief Set tracks to convert
			 * @param [in] tracks Tracks information
			 */
			void setTracks( const album::TrackList & tracks );

		public slots:
			/**
			 * @brief Cancel converting
			 */
			void cancel();

		signals:
			/**
			 * @brief Emited when convertion is canceled.
			 */
			void canceled();
			/**
			 * @brief Send error message
			 * @param title Message box title
			 * @param errMsg Error message
			 */
			void errorOccured( const QString & title, const QString & message );

			/**
			 * @brief Send task name
			 */
			void taskName( const QString & Title );
			/**
			 * @brief Send task goal
			 */
			void taskGoal( qint64 msDuration );
			/**
			 * @brief Send current task count
			 */
			void currentTask( int number );
			/**
			 * @brief Progress notifyer
			 * @param duration Convert duration
			 */
			void step( qint64 msDuration );

		protected:
			/**
			 * @brief Action function
			 */
			virtual void run();

		private:
			codec::WriterSP encoder_;
			album::TrackList tracks_;
			QList< QString > paths_;
			bool canceled_;
			Converter converter_;
		};

	}

}

#endif
