/**
 * @file conversiondialog.hpp
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
#ifndef KHOPPER_WIDGET_CONVERSIONDIALOG_HPP
#define KHOPPER_WIDGET_CONVERSIONDIALOG_HPP

#include <map>

#include <QtCore/QDir>
#include <QtGui/QDialog>

#include "khopper/playlist.hpp"

namespace Ui {
class ConversionDialog;
}

namespace khopper {
namespace widget {

class ProgressViewer;
class Panel;

/// Output format option widget
class ConversionDialog : public QDialog {
	Q_OBJECT
public:
	/// Default constructor
	explicit ConversionDialog( QWidget * parent );

	/// Get current panel
	Panel * getCurrent() const;
	void convert( const album::PlayList & tracks );

public slots:
	void addPanel( khopper::widget::Panel * panel );
	void removePanel( khopper::widget::Panel * panel );

signals:
	void errorOccured( const QString & title, const QString & message );

private slots:
	void changeOutputPath_();

private:
	QDir getOutputDir_( album::TrackSP );
	QString getOutputName_( album::TrackSP );

	std::shared_ptr< Ui::ConversionDialog > ui_;
	ProgressViewer * progress_;
	std::map< int, Panel * > table_;
};

}
}

#endif
