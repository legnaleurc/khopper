/**
 * @file MainWindow.hpp
 */
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "cue.hpp"
#include "type.hpp"
#include "converter.hpp"
#include "SongList.hpp"
#include "Threads.hpp"

#include <QMainWindow>
#include <QComboBox>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QStringList>
#include <QPushButton>
#include <QVariant>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>

/**
 * @namespace Khopper
 */
namespace Khopper {

	/**
	 * @class MainWindow
	 * @brief The main window
	 */
	class MainWindow : public QMainWindow {
		
		Q_OBJECT
		
		public:
			/**
			 * @brief Default constructor
			 * @param parent Parent widget
			 * @param flags Widget flag
			 */
			MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
		
		public slots:
			/**
			 * @brief open the file widget
			 */
			void openFileDialog();
			/**
			 * @brief set the song list
			 * @param list Song list, generate by cue/cue.hpp
			 */
			void setSongList( const std::vector< CUESheet::FieldType > & list );
			/**
			 * @brief open file
			 * @param file file name
			 */
			void open( const QString & file );
		
		private:
			SongList * _songList_;
			QComboBox * _outputTypes_;
			QPushButton * _action_;
			std::string _audioPath_;
			std::string _sheetPath_;
			
			QMenuBar * _setMenu_();
			void _setLabel_();
			void _setOutputTypeList_();
		
		private slots:
			void _fire_();
		
	};
	
}

#endif
