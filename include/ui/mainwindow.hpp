/**
 * @file mainwindow.hpp
 */
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "cue/cuesheet.hpp"

#include <QMainWindow>
#include <QPointer>

QT_BEGIN_NAMESPACE
class QComboBox;
class QPushButton;
class QProgressDialog;
class QMessageBox;
class QString;
class QMenuBar;
QT_END_NAMESPACE

/**
 * @namespace Khopper
 */
namespace Khopper {

	class ConverterThread;
	class SongList;

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
			void setSongList( const CUESheet & list );
			/**
			 * @brief open file
			 * @param file file name
			 */
			void open( const QString & file );
		
		private:
			QPointer< SongList > songList_;
			QPointer< QComboBox > outputTypes_;
			QPointer< QPushButton > action_;
			QPointer< QProgressDialog > progress_;
			QPointer< ConverterThread > cvt_;
			QPointer< QMessageBox > about_;
			CUESheet sheet_;
			
			QPointer< QMenuBar > setMenu_();
			void setLabel_();
			void setOutputTypeList_();
		
		private slots:
			void fire_();
			void showAboutQt_();
			void showAbout_();
			void setAbout_();
			void runTimeError_( const QString & );
		
	};
	
}

#endif
