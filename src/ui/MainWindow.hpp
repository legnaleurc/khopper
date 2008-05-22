#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTableView>
#include <QComboBox>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QStringList>

#include "cue.hpp"

namespace Khopper {
	
	class MainWindow : public QMainWindow {
		
		public:
			MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
			void setSongList( const std::vector< std::vector< std::string > > & list );
		
		private:
			QTableView * _songList_;
			QComboBox * _outputTypes_;
			
			void _setLabel_();
		
	};
	
}

#endif
