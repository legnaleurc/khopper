/**
 * @file youtubedialog.hpp
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
#ifndef KHOPPER_WIDGET_YOUTUBEDIALOG_HPP
#define KHOPPER_WIDGET_YOUTUBEDIALOG_HPP

#include <QtCore/QUrl>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>

#include <map>
#include <memory>

namespace Ui {
	class YouTubeDialog;
}

namespace khopper {
	namespace widget {

		class YouTubeDialog : public QDialog {
			Q_OBJECT
		public:
			YouTubeDialog();

			void addFormat( const QString & format );
			void clearFormat();
			QString getSelectedFormat() const;
			QString getLocalLocation() const;

		private slots:
			void getLocation_();

		private:
			QButtonGroup * group_;
			std::shared_ptr< Ui::YouTubeDialog > ui_;
		};

	}
}

#endif
