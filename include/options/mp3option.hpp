/**
 * @file mp3option.hpp
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
#ifndef KHOPPER_MP3OPTION_HPP
#define KHOPPER_MP3OPTION_HPP

#include "abstractoption.hpp"

#include <QButtonGroup>
#include <QComboBox>

namespace Khopper {

	/**
	 * @brief Mp3 option widget
	 */
	class MP3Option : public AbstractOption {
	public:
		/**
		 * @brief Default constructor
		 * @param parent Parent widget
		 * @param f Window flags
		 */
		MP3Option( QWidget * parent = 0, Qt::WindowFlags f = 0 );

		/**
		 * @brief Get Encoder setting
		 * @return Smart pointer to Encoder
		 */
		virtual EncoderSP getEncoder() const;

	private:
		QButtonGroup * brChoise_;
		QComboBox * bitRate_;
		QComboBox * level_;
		QComboBox * sampleRate_;
		QComboBox * channels_;
	};

}

#endif
