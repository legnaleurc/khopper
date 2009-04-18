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
#ifndef KHOPPER_WIDGET_CONVERTER_HPP
#define KHOPPER_WIDGET_CONVERTER_HPP

#include "track.hpp"
#include "codec/abstractwriter.hpp"

#include <QObject>

namespace khopper {

	namespace widget {

		/**
		 * @brief Controller of converting
		 */
		class Converter : public QObject {
			Q_OBJECT

		public:
			/**
			 * @brief Default constructor
			 */
			Converter( QObject * parent = 0 );

			/**
			 * @brief Convert @p track
			 * @param [in] track track to convert
			 * @param [in] targetPath output file path
			 * @param [in] encoder encoder setting
			 */
			void convert( album::TrackCSP track, const QString & targetPath, codec::WriterSP encoder );

		public slots:
			/**
			 * @brief Cancel converting
			 */
			void cancel();

		signals:
			/**
			 * @brief Decoded duration
			 * @param ms Time in second * 10000
			 */
			void decodedTime( int ms ) const;

		private:
			bool canceled_;
		};

	}

}

#endif
