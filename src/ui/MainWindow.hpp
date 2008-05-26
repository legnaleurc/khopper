#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "cue.hpp"
#include "util.hpp"

#include <QMainWindow>
#include <QTableView>
#include <QComboBox>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QStringList>
#include <QPushButton>
#include <QVariant>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>

/**
 * @namespace Khopper
 */
namespace Khopper {

	/**
	 * @class MainWindow
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
			void openFile();
			/**
			 * @brief set the song list
			 * @param list Song list, generate by cue/cue.hpp
			 */
			void setSongList( const std::vector< FieldType > & list );
		
		private:
			QTableView * _songList_;
			QComboBox * _outputTypes_;
			QPushButton * _action_;
			
			QMenuBar * _setMenu_();
			void _setLabel_();
			void _setOutputTypeList_();
		
		private slots:
			void _fire_();
		
	};
	
}

#endif
