/**
 * @file mainwindow.hpp
 */
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QPointer>

QT_BEGIN_NAMESPACE
class QComboBox;
class QPushButton;
class QProgressDialog;
class QMessageBox;
class QString;
class QStandardItemModel;
QT_END_NAMESPACE

/**
 * @namespace Khopper
 */
namespace Khopper {

	class CUESheet;
	class ConverterThread;
	class SongListView;

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
			 * @brief add the song list
			 * @param list Song list, generate by cue/cuesheet.hpp
			 */
			void addSongList( const CUESheet & list );
			/**
			 * @brief open file
			 * @param fileName file name
			 */
			void open( const QString & fileName );
		
		private:
			QPointer< SongListView > songListView_;
			QPointer< QStandardItemModel > songListModel_;
			QPointer< QComboBox > outputTypes_;
			QPointer< QPushButton > action_;
			QPointer< QProgressDialog > progress_;
			QPointer< ConverterThread > cvt_;
			QPointer< QMessageBox > about_;
			
			void initMenuBar_();
			void initOutputTypeList_();
			void initHeader();
		
		private slots:
			void fire_();
			void showAboutQt_();
			void showAbout_();
			void setAbout_();
			void runTimeError_( const QString & );
		
	};
	
}

#endif
