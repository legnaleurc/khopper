/**
 * @file abstractpanel.hpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KHOPPER_WIDGET_ABSTRACTPANEL_HPP
#define KHOPPER_WIDGET_ABSTRACTPANEL_HPP

#include "codec/abstractwriter.hpp"

#include <QWidget>

namespace khopper {

	namespace plugin {

		/**
		 * @ingroup Plugins
		 * @brief Abstract panel widget
		 *
		 * All output option panel will inherit this class, \n
		 * and must register to factory. \n
		 */
		class KHOPPER_EXPORT AbstractPanel : public QWidget {
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

Q_DECLARE_INTERFACE( khopper::plugin::AbstractPanel, "org.FoolproofProject.Khopper.Plugin.Panel/0.2" )

#endif
