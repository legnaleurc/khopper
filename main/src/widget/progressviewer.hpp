/**
 * @file progressviewer.hpp
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
#ifndef KHOPPER_WIDGET_PROGRESSVIEWER_HPP
#define KHOPPER_WIDGET_PROGRESSVIEWER_HPP

#include "khopper/playlist.hpp"
#include "khopper/abstractwriter.hpp"

#include <QtCore/QList>
#include <QtCore/QQueue>
#include <QtGui/QWidget>

namespace Ui {
	class ProgressViewer;
}

namespace khopper {
	namespace widget {

		class ProgressBar;
		class Converter;

		class ProgressViewer : public QWidget {
			Q_OBJECT
		public:
			explicit ProgressViewer( QWidget * parent );

			void start( const QList< Converter * > & tasks );

		private slots:
			void cancel_();
			void dispatch_();

		private:
			void dispatch_( ProgressBar * );

			int rc_;
			QList< ProgressBar * > lp_;
			QList< Converter * > tasks_;
			std::shared_ptr< Ui::ProgressViewer > ui_;
		};

	}
}

#endif
