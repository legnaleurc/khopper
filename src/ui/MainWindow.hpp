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
			QPointer< SongList > songList_;
			QPointer< QComboBox > outputTypes_;
			QPointer< QPushButton > action_;
			std::string audioPath_;
			std::string sheetPath_;
			QPointer< QProgressDialog > progress_;
			QPointer< QTimer > pdTimer_;
			QPointer< ConverterThread > cvt_;
			QPointer< QMessageBox > about_;
			
			QPointer< QMenuBar > setMenu_();
			void setLabel_();
			void setOutputTypeList_();
		
		private slots:
			void fire_();
			void stepProgress_();
			void showAboutQt_();
			void showAbout_();
			void setAbout_();
			void runTimeError_( const QString & );
		
	};
	
}

#endif
