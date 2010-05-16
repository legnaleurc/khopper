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

#include "plugin/abstractpanel.hpp"
#include "track.hpp"

#include <boost/format.hpp>

#include <QtGui/QDialog>
#include <QtGui/QTabWidget>

#include <map>

namespace Ui {
	class ConversionDialog;
}

namespace khopper {

	namespace widget {

		class Progress;
		class ConverterThread;

		/// Output format option widget
		class ConversionDialog : public QDialog {
			Q_OBJECT
		public:
			/// Default constructor
			explicit ConversionDialog( QWidget * parent );
			virtual ~ConversionDialog();

			/// Get current panel
			plugin::AbstractPanel * getCurrent() const;
			void convert( const album::TrackList & tracks, const boost::format & tpl );

		public slots:
			void addPanel( khopper::plugin::AbstractPanel * panel );
			void removePanel( khopper::plugin::AbstractPanel * panel );

		signals:
			void errorOccured( const QString & title, const QString & message );

		private slots:
			void changeOutputPath_();

		private:
			QString getOutputPath_( album::TrackSP );

			Ui::ConversionDialog * ui_;
			Progress * progress_;
			ConverterThread * thread_;
			std::map< int ,plugin::AbstractPanel * > table_;
		};

	}

}

#endif
