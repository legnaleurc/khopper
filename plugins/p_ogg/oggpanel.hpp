/**
 * @file oggpanel.hpp
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
#ifndef KHOPPER_WIDGET_OGGPANEL_HPP
#define KHOPPER_WIDGET_OGGPANEL_HPP

#include "plugin/abstractpanel.hpp"

#include <QComboBox>
#include <QButtonGroup>

namespace khopper {

	namespace plugin {

		/// Ogg output option
		class OGGPanel : public AbstractPanel {
			Q_OBJECT
			Q_INTERFACES( khopper::plugin::AbstractPanel )

		public:
			/// Default constructor
			OGGPanel( QWidget * parent = 0, Qt::WindowFlags f = 0 );

			/// Get configured Writer instance
			virtual codec::WriterSP getWriter() const;
			/// Get file suffix
			virtual QString getSuffix() const;
			/// Get widget title
			virtual QString getTitle() const;

		private:
			QComboBox * channels_;
			QButtonGroup * brGroup_;
			QComboBox * quality_;
			QComboBox * sampleRate_;
		};

	}

}

#endif
