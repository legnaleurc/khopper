/**
 * @file abstractoption.hpp
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
#ifndef KHOPPER_ABSTRACTOPTION_HPP
#define KHOPPER_ABSTRACTOPTION_HPP

#include "encoder.hpp"

#include <loki/Factory.h>

#include <QWidget>

namespace Khopper {

	/**
	 * @brief Abstract option widget
	 *
	 * All output format option will inherit this class, \n
	 * and must register to factory. \n
	 */
	class AbstractOption : public QWidget {
	public:
		/**
		 * @brief Default constructor
		 * @param parent Parent widget
		 * @param f Window flags
		 */
		AbstractOption( QWidget * parent = 0, Qt::WindowFlags f = 0 );
		/**
		 * @brief Virtual destructor
		 */
		virtual ~AbstractOption();

		/**
		 * @brief Get encoder setting object
		 * @return Smart pointer contains Encoder
		 */
		virtual EncoderSP getEncoder() const = 0;
	};

	/**
	 * @brief AbstractOption factory
	 *
	 * Register by yourself.
	 */
	typedef Loki::SingletonHolder< Loki::Factory< AbstractOption, std::string >, Loki::CreateUsingNew, Loki::LongevityLifetime::DieAsSmallObjectChild > UIFactory;

}

#endif
