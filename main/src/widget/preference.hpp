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

#include <QtGui/QAbstractButton>
#include <QtGui/QDialog>

namespace Ui {
	class Preference;
}

namespace khopper {

	namespace widget {

		/**
		 * @brief Preference dialog
		 */
		class Preference : public QDialog {
			Q_OBJECT

		public:
			/// Default constructor
			explicit Preference( QWidget * parent );

		private slots:
			void changeFont_();
			void perform_( QAbstractButton * );

		private:
			std::tr1::shared_ptr< Ui::Preference > ui_;
			QFont currentFont_;

			void apply_();
			void revert_();
		};

	}

}

#endif
