/**
 * @file wavpanel.hpp
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
#ifndef KHOPPER_WIDGET_WAVPANEL_HPP
#define KHOPPER_WIDGET_WAVPANEL_HPP

#include "plugin/abstractpanel.hpp"

#include <QComboBox>

namespace khopper {

	namespace plugin {

		/**
		 * @brief Wav option widget
		 */
		class WAVPanel : public AbstractPanel {
			Q_OBJECT
			Q_INTERFACES( khopper::plugin::AbstractPanel )

		public:
			/**
			 * @brief Default constructor
			 * @param parent Parent widget
			 * @param f Window flags
			 */
			WAVPanel( QWidget * parent = 0, Qt::WindowFlags f = 0 );

			/**
			 * @brief Get encoder setting object
			 * @return Smart pointer contains AbstractWriter
			 */
			virtual codec::WriterSP getWriter() const;
			/**
			 * @brief Get file suffix
			 * @return File extension, without leading '.'
			 */
			virtual QString getSuffix() const;
			/**
			 * @brief Get tab title
			 * @return Title use in tab widget
			 */
			virtual QString getTitle() const;

		private:
			QComboBox * sampleRate_;
			QComboBox * channels_;
		};

	}

}

#endif
