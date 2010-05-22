/**
 * @file mp3panel.hpp
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
#ifndef KHOPPER_WIDGET_MP3PANEL_HPP
#define KHOPPER_WIDGET_MP3PANEL_HPP

#include "khopper/abstractpanel.hpp"

class QButtonGroup;
namespace Ui {
	class MP3Panel;
}

namespace khopper {
	namespace widget {

		/**
		 * @brief Mp3 option widget
		 */
		class MP3Panel : public AbstractPanel {
		public:
			/**
			 * @brief Default constructor
			 * @param parent Parent widget
			 * @param f Window flags
			 */
			MP3Panel();
			virtual ~MP3Panel();

			/**
			 * @brief Get encoder setting object
			 * @return Smart pointer contains AbstractWriter
			 */
			virtual codec::WriterSP getWriter() const;

		private:
			Ui::MP3Panel * ui_;
			QButtonGroup * choise_;
		};

	}
}

#endif
