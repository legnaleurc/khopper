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

namespace Khopper {
	
	class MainWindow : public QMainWindow {
		
		public:
			MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
			void setSongList( const std::vector< FieldType > & list );
		
		private:
			QTableView * _songList_;
			QComboBox * _outputTypes_;
			QPushButton * _action_;
			
			void _setLabel_();
			void _setOutputTypeList_();
		
	};
	
}

#endif
