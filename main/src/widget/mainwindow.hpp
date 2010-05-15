/**
 * @file mainwindow.hpp
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
#ifndef KHOPPER_WIDGET_MAINWINDOW_HPP
#define KHOPPER_WIDGET_MAINWINDOW_HPP

#include "track.hpp"

#include <QtCore/QList>
#include <QtCore/QUrl>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>

namespace Ui {
	class MainWindow;
}

namespace khopper {

	namespace widget {

		class AboutWidget;
		class ConverterThread;
		class Progress;
		class Player;
		class TextCodec;
		class Preference;
		class ConversionDialog;

		/**
		 * @class MainWindow
		 * @brief The main window
		 */
		class MainWindow : public QMainWindow {
			Q_OBJECT

		public:
			/**
			 * @brief Default constructor
			 */
			MainWindow();
			virtual ~MainWindow();

		public slots:
			/**
			 * @brief Open the file widget
			 */
			void showOpenFilesDialog();
			/**
			 * @brief Open files
			 * @param [in] filePaths file path
			 */
			void open( const QList< QUrl > & uris );

		private:
			Ui::MainWindow * ui_;
			TextCodec * codec_;
			ConversionDialog * conversion_;
			Preference * preference_;
			AboutWidget * about_;
			QString lastOpenedDir_;

			void initMenuBar_();
			void initOptionWindow_();

		private slots:
			void fire_( const album::TrackList & );
			void showErrorMessage_( const QString &, const QString & );
		};

	}

}

#endif
