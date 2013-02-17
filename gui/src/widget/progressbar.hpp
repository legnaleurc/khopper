/**
 * @file progressbar.hpp
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
#ifndef KHOPPER_WIDGET_PROGRESSBAR_HPP
#define KHOPPER_WIDGET_PROGRESSBAR_HPP

#include <QtGui/QWidget>

#include "khopper/track.hpp"
#include "khopper/writer.hpp"

namespace Ui {
class ProgressBar;
}

namespace khopper {

namespace utility {
class Converter;
}

namespace widget {

class ProgressBar : public QWidget {
	Q_OBJECT
public:
	explicit ProgressBar( QWidget * parent );

	void cancel();
	void start( utility::Converter * task );

signals:
	void finished();

private slots:
	void increase_( qint64 );

private:
	utility::Converter * task_;
	std::shared_ptr< Ui::ProgressBar > ui_;
};

}

}

#endif
