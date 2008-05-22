#include "MainWindow.hpp"

namespace Khopper {
	
	MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags flags ) : QMainWindow( parent, flags ) {
		// Setting central widget
		QWidget * central = new QWidget( this );
		central->setLayout( new QVBoxLayout( central ) );
		setCentralWidget( central );
		
		// Add song list
		_songList_ = new QTableView( central );
		central->layout()->addWidget( _songList_ );
		// Set model
		_songList_->setModel( new QStandardItemModel( _songList_ ) );
		// Set header of model
		_setLabel_();
		// Set selection behavior
		_songList_->setSelectionBehavior( QAbstractItemView::SelectRows );
		
		// Setting file select
		_outputTypes_ = new QComboBox( central );
		central->layout()->addWidget( _outputTypes_ );
	}
	
	void MainWindow::_setLabel_() {
		QStandardItemModel * model = qobject_cast< QStandardItemModel * >( _songList_->model() );
		QStringList headers;
		
		for( int i = 0; i < 9; ++i ) {
			headers << HEADERS[i];
		}
		
		model->setHorizontalHeaderLabels( headers );
	}
	
	void MainWindow::setSongList( const std::vector< std::vector< std::string > > & list ) {
		QStandardItemModel * model = qobject_cast< QStandardItemModel * >( _songList_->model() );
		QStandardItem * item;
		
		for( std::size_t row = 0; row < list.size(); ++row ) {
			for( int col = 0; col < 9; ++col ) {
				item = new QStandardItem( list[row][col].c_str() );
				model->setItem( row, col, item );
			}
		}
	}
	
}
