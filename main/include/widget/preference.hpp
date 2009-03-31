/**
 * @file preference.hpp
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
#ifndef KHOPPER_WIDGET_PREFERENCE_HPP
#define KHOPPER_WIDGET_PREFERENCE_HPP

#include <boost/format.hpp>

#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>

namespace khopper {

	namespace widget {

		/**
		 * @brief Preference dialog
		 */
		class Preference : public QDialog {
			Q_OBJECT

		public:
			/// Default constructor
			Preference( QWidget * parent = 0 );

			/**
			 * @brief Get file name template
			 * @return Format string in boost::format
			 */
			boost::format getTemplate() const;

		private slots:
			void perform_( QAbstractButton * );

		private:
			QLineEdit * fnTpl_;
			QDialogButtonBox * buttons_;

			void apply_();
			void revert_();
		};

	}

}

#endif
