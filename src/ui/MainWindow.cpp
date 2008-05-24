#include "MainWindow.hpp"

namespace Khopper {
	
	MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags flags ) : QMainWindow( parent, flags ) {
		// setTitle
		setWindowTitle( tr( "Khopper" ) );
		
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
		
		// Set bottom layout
		QWidget * bottom = new QWidget( central );
		bottom->setLayout( new QHBoxLayout( bottom ) );
		central->layout()->addWidget( bottom );
		
		// Setting output format select
		_outputTypes_ = new QComboBox( bottom );
		bottom->layout()->addWidget( _outputTypes_ );
		_setOutputTypeList_();
		
		// Action button
		_action_ = new QPushButton( tr( "Fire!" ), bottom );
		bottom->layout()->addWidget( _action_ );
	}
	
	void MainWindow::_setLabel_() {
		QStandardItemModel * model = qobject_cast< QStandardItemModel * >( _songList_->model() );
		QStringList headers;
		
		for( int i = 0; Track::Header[i] != NULL; ++i ) {
			headers << Track::Header[i];
		}
		
		model->setHorizontalHeaderLabels( headers );
	}
	
	void MainWindow::_setOutputTypeList_() {
		const OutputTypes::ObjectType & tm = OutputTypes::Instance();
		for( OutputTypes::ObjectType::const_iterator it = tm.begin(); it != tm.end(); ++it ) {
			_outputTypes_->addItem( it->second.c_str() );
		}
	}
	
	void MainWindow::setSongList( const std::vector< FieldType > & list ) {
		QStandardItemModel * model = qobject_cast< QStandardItemModel * >( _songList_->model() );
		QStandardItem * item;
		
		for( std::size_t row = 0; row < list.size(); ++row ) {
			for( int col = 0; col < Track::SIZE; ++col ) {
				item = new QStandardItem( list[row][col].c_str() );
				model->setItem( row, col, item );
			}
		}
	}
	
}
