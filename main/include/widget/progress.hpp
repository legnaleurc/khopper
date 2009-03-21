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

#include <QDialog>
#include <QLabel>
#include <QProgressBar>

namespace khopper {

	namespace widget {

		/**
		 * @brief Show the progress
		 */
		class Progress : public QDialog {
			Q_OBJECT

		public:
			/**
			 * @brief Pass arguments to QDialog
			 */
			Progress( QWidget * parent = 0, Qt::WindowFlags f = 0 );

			/**
			 * @brief Get current progress
			 * @sa setValue
			 */
			int getValue() const;

		public slots:
			/**
			 * @brief Set maximum progress
			 */
			void setMaximum( int maximum );
			/**
			 * @brief Set current progress
			 * @sa getValue
			 */
			void setValue( int value );
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

		private:
			QLabel * itemName_;
			QLabel * current_;
			QLabel * total_;
			QProgressBar * prog_;
		};

	}

}

#endif
