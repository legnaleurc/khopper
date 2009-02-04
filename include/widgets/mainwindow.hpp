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
#ifndef KHOPPER_MAINWINDOW_HPP
#define KHOPPER_MAINWINDOW_HPP

#include "track.hpp"

#include <QMainWindow>
#include <QStandardItemModel>
#include <QComboBox>
#include <QPushButton>
#include <QProgressDialog>

/**
 * @namespace Khopper
 */
namespace Khopper {

	class ConverterThread;
	class SongListView;
	class TextCodec;

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
		void showOpenFileDialog();
		/**
		 * @brief Add songs the song list
		 * @param [in] tracks Tracks
		 */
		void addSongList( const std::vector< TrackSP > & tracks );
		/**
		 * @brief Open file
		 * @param [in] filePath file path
		 */
		void open( const QString & filePath );

	private:
		TextCodec * codec_;
		SongListView * songListView_;
		QStandardItemModel * songListModel_;
		QTabWidget * optionTabs_;
		QPushButton * action_;
		QProgressDialog * progress_;
		ConverterThread * cvt_;
		QWidget * about_;

		void initMenuBar_();
		void initOptionTabs_();
		void initHeader_();
		void initAbout_();

	private slots:
		void fire_();
		void showErrorMessage_( const QString &, const QString & );
		void incProgress_( int );
		void delSongList_();
	};

}

#endif
