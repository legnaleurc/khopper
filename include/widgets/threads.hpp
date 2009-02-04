/**
 * @file threads.hpp
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
#ifndef KHOPPER_THREADS_HPP
#define KHOPPER_THREADS_HPP

#include "track.hpp"
#include "encoder.hpp"

#include <QThread>
#include <QStringList>

namespace Khopper {

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
		void setOutput( EncoderSP output, const QStringList & paths );

		/**
		 * @brief Set tracks to convert
		 * @param [in] tracks Tracks information
		 */
		void setTracks( const std::vector< TrackSP > & tracks );

	signals:
		/**
		 * @brief Send error message
		 * @param title Message box title
		 * @param errMsg Error message
		 */
		void error( const QString & title, const QString & errMsg );

		/**
		 * @brief Progress notifyer
		 * @param duration Convert duration
		 */
		void step( int duration );

	protected:
		/**
		 * @brief Action function
		 */
		virtual void run();

	private:
		EncoderSP encoder_;
		std::vector< TrackSP > tracks_;
		QStringList paths_;
	};

}

#endif
