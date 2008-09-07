/**
 * @file mainwindow.hpp
 */
#ifndef KHOPPER_UI_MAINWINDOW_HPP
#define KHOPPER_UI_MAINWINDOW_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QComboBox;
class QPushButton;
class QProgressDialog;
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
			 * @param parent Parent widget
			 * @param flags Widget flag
			 */
			MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
		
		public slots:
			/**
			 * @brief Open the file widget
			 */
			void openFileDialog();
			/**
			 * @brief Add songs the song list
			 * @param sheet CUE Sheet object
			 */
			void addSongList( const CUESheet & sheet );
			/**
			 * @brief Open file
			 * @param fileName file name
			 */
			void open( const QString & fileName );
		
		private:
			TextCodec * codec_;
			SongListView * songListView_;
			QStandardItemModel * songListModel_;
			QComboBox * outputTypes_;
			QPushButton * action_;
			QProgressDialog * progress_;
			ConverterThread * cvt_;
			QWidget * about_;
			
			void initMenuBar_();
			void initOutputTypeList_();
			void initHeader_();
			void initAbout_();
		
		private slots:
			void fire_();
			void runTimeError_( const QString & );
		
	};
	
}

#endif
