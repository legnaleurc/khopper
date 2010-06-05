/**
 * @file progress.hpp
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
#ifndef KHOPPER_WIDGET_PROGRESS_HPP
#define KHOPPER_WIDGET_PROGRESS_HPP

#include <QtGui/QWidget>

namespace Ui {
	class Progress;
}

namespace khopper {

	namespace widget {

		/**
		 * @brief Show the progress
		 */
		class Progress : public QWidget {
			Q_OBJECT

		public:
			/**
			 * @brief Pass arguments to QDialog
			 */
			explicit Progress( QWidget * parent );

		public slots:
			void increase( qint64 value );
			/**
			 * @brief Set maximum progress
			 */
			void setMaximum( qint64 maximum );
			/**
			 * @brief Set current item name
			 */
			void setItemName( const QString & name );
			/**
			 * @brief Set current task count
			 */
			void setCurrent( int current );
			/**
			 * @brief Set total tasks
			 */
			void setTotal( int total );

		signals:
			void canceled();

		private:
			void updateIndex_();

			std::tr1::shared_ptr< Ui::Progress > ui_;
			int current_;
			int total_;
		};

	}

}

#endif
