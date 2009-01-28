/**
 * @file mainwindow.hpp
 * @author Wei-Cheng Pan
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
	class Track;

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
			void addSongList( const std::vector< Track > & tracks );
			/**
			 * @brief Open file
			 * @param [in] filePath file path
			 */
			void open( const QString & filePath );
		
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
