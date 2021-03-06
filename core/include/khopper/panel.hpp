/**
 * @file panel.hpp
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
#ifndef KHOPPER_WIDGET_PANEL_HPP
#define KHOPPER_WIDGET_PANEL_HPP

#include <QtGui/QWidget>

#include "writer.hpp"


namespace khopper {
namespace widget {

/**
 * @brief Abstract panel widget
 *
 * All output option panel will inherit this class, \n
 * and must register to factory. \n
 */
class KHOPPER_DLL Panel: public QWidget {
public:
	/**
	 * @brief Default constructor
	 * @param parent Parent widget
	 * @param f Window flags
	 */
	Panel();

	/**
	 * @brief Get encoder setting object
	 * @return Smart pointer contains AbstractWriter
	 */
	virtual codec::Writer::Creator getWriterCreator() const = 0;
	/**
	 * @brief Get file suffix
	 * @return File extension, without leading '.'
	 */
	const QString & getSuffix() const;
	/**
	 * @brief Get tab title
	 * @return Title use in tab widget
	 */
	const QString & getTitle() const;

protected:
	void setSuffix( const QString & suffix );
	void setTitle( const QString & title );

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}
}

#endif
