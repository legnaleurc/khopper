/**
 * @file abstractpanel.hpp
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
#ifndef KHOPPER_WIDGET_ABSTRACTPANEL_HPP
#define KHOPPER_WIDGET_ABSTRACTPANEL_HPP

#include "plugin/abstractwriter.hpp"

#include <QWidget>

namespace khopper {

	namespace widget {

		/**
		 * @brief Abstract panel widget
		 *
		 * All output option panel will inherit this class, \n
		 * and must register to factory. \n
		 */
		class AbstractPanel : public QWidget {
			Q_OBJECT

		public:
			/**
			 * @brief Default constructor
			 * @param parent Parent widget
			 * @param f Window flags
			 */
			AbstractPanel( QWidget * parent = 0, Qt::WindowFlags f = 0 );

			/**
			 * @brief Get encoder setting object
			 * @return Smart pointer contains AbstractWriter
			 */
			virtual codec::WriterSP getWriter() const = 0;
			/**
			 * @brief Get file suffix
			 * @return File extension, without leading '.'
			 */
			virtual QString getSuffix() const = 0;
			/**
			 * @brief Get tab title
			 * @return Title use in tab widget
			 */
			virtual QString getTitle() const = 0;
		};

	}

}

Q_DECLARE_INTERFACE( khopper::widget::AbstractPanel, "org.FoolproofProject.Khopper.Plugin.Panel/0.2" )

#endif
