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

#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>

namespace khopper {

	namespace widget {

		class ConverterThread;
		class Progress;
		class Player;
		class TextCodec;
		class Preference;
		class OutputOption;

		/**
		 * @class MainWindow
		 * @brief The main window
		 */
		class MainWindow : public QMainWindow {
			Q_OBJECT

		public:
			/**
			 * @brief Default constructor
			 * @param [in] parent Parent widget
			 * @param [in] flags Widget flag
			 */
			MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );

		public slots:
			/**
			 * @brief Open the file widget
			 */
			void showOpenFilesDialog();
			/**
			 * @brief Open files
			 * @param [in] filePaths file path
			 */
			void open( const QStringList & filePaths );

		private:
			TextCodec * codec_;
			Player * player_;
			OutputOption * optionWindow_;
			QLineEdit * outputPath_;
			QCheckBox * useSourcePath_;
			Progress * progress_;
			ConverterThread * cvt_;
			Preference * preference_;
			QWidget * about_;

			void initMenuBar_();
			void initOptionWindow_();
			void initAbout_();

		private slots:
			void fire_();
			void showErrorMessage_( const QString &, const QString & );
			void incProgress_( int );
			void changeOutputPath_();
			QString getOutDir_( album::TrackSP ) const;
		};

	}

}

#endif
