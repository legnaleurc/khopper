/**
 * @file seekslider.hpp
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
#ifndef KHOPPER_WIDGET_SEEKSLIDER_HPP
#define KHOPPER_WIDGET_SEEKSLIDER_HPP

#include <QSlider>
#include <Phonon/MediaObject>

namespace khopper {

	namespace widget {

		/**
		 * @brief Seek slider bar
		 *
		 * I wrote this because Phonon::SeekSlider is not that useful.
		 */
		class SeekSlider : public QWidget {
			Q_OBJECT

		public:
			/**
			 * @brief Constructor
			 * @param media media to be tracking
			 * @param parent parent widget
			 */
			SeekSlider( Phonon::MediaObject * media, QWidget * parent );

			/**
			 * @brief Set media object
			 * @param media media to be tracking
			 */
			void setMediaObject( Phonon::MediaObject * media );
			/**
			 * @brief Set song range
			 * @param begin begin time stamp in media
			 * @param end end time stamp in media
			 */
			void setRange( int begin, int end );

		private slots:
			void changeState_( Phonon::State );
			void seek_( int );
			void tick_( qint64 );

		private:
			Phonon::MediaObject * media_;
			QSlider * slider_;
			bool tracking_;
			qint64 begin_;
			qint64 end_;
			bool ticking_;
		};

	}

}

#endif
