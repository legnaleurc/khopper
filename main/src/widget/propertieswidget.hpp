/**
 * @file propertieswidget.hpp
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
#ifndef KHOPPER_WIDGET_PROPERTIESWIDGET_HPP
#define KHOPPER_WIDGET_PROPERTIESWIDGET_HPP

#include "track.hpp"

#include <QtGui/QDialog>

class QAbstractButton;
class QDialogButtonBox;
class QGridLayout;
class QLineEdit;

namespace khopper {
	namespace widget {
		class PropertiesWidget : public QDialog {
			Q_OBJECT

		public:
			PropertiesWidget( QWidget * parent );

			using QDialog::exec;
			int exec( const album::TrackList & tracks );

		private slots:
			void perform_( QAbstractButton * );

		private:
			void initFieldLayout_( QGridLayout * );
			void write_();

			QLineEdit * albumInput_;
			QLineEdit * artistInput_;
			QDialogButtonBox * buttonBox_;
			QLineEdit * titleInput_;
		};
	}
}

#endif // KHOPPER_WIDGET_PROPERTIESWIDGET_HPP
