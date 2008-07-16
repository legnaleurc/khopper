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
#include <QProgressDialog>
#include <QTimer>
#include <QPointer>

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
			QPointer< SongList > _songList_;
			QPointer< QComboBox > _outputTypes_;
			QPointer< QPushButton > _action_;
			std::string _audioPath_;
			std::string _sheetPath_;
			QPointer< QProgressDialog > _progress_;
			QPointer< QTimer > _pdTimer_;
			QPointer< ConverterThread > _cvt_;
			
			QPointer< QMenuBar > _setMenu_();
			void _setLabel_();
			void _setOutputTypeList_();
		
		private slots:
			void _fire_();
			void _stepProgress_();
		
	};
	
}

#endif
