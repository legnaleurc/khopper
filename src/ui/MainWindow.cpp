#include "MainWindow.hpp"

namespace Khopper {
	
	MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags flags ) : QMainWindow( parent, flags ) {
		// setTitle
		setWindowTitle( tr( "Khopper" ) );
		
		// Setting menu bar
		setMenuBar( _setMenu_() );
		
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
		connect( _action_, SIGNAL( clicked() ), this, SLOT( _fire_() ) );
		bottom->layout()->addWidget( _action_ );
	}
	
	QMenuBar * MainWindow::_setMenu_() {
		QMenuBar * menuBar = new QMenuBar( this );
		
		// setting file menu
		QMenu * file = new QMenu( tr( "&File" ), menuBar );
		
		QAction * open = new QAction( tr( "&Open..." ), file );
		open->setShortcut( tr( "Ctrl+O" ) );
		connect( open, SIGNAL( triggered() ), this, SLOT( openFile() ) );
		file->addAction( open );
		
		// add file menu to menu bar
		menuBar->addMenu( file );
		// file menu done
		
		return menuBar;
	}
	
	void MainWindow::_setLabel_() {
		QStandardItemModel * model = qobject_cast< QStandardItemModel * >( _songList_->model() );
		QStringList headers;
		
		for( int i = 0; CUESheet::Track::Header[i] != NULL; ++i ) {
			headers << CUESheet::Track::Header[i];
		}
		
		model->setHorizontalHeaderLabels( headers );
	}
	
	void MainWindow::_setOutputTypeList_() {
		const OutputTypes::ObjectType & tm = OutputTypes::Instance();
		for( OutputTypes::ObjectType::const_iterator it = tm.begin(); it != tm.end(); ++it ) {
			_outputTypes_->addItem( it->second.c_str(), QVariant( it->first.c_str() ) );
		}
	}
	
	// TODO Link converter
	void MainWindow::_fire_() {
		// create output format object
		QString test = _outputTypes_->itemData( _outputTypes_->currentIndex() ).toString();
		Output * output = OutputFactory::Instance().CreateObject( test.toStdString() );
		
		// get select list
		QModelIndexList selected = _songList_->selectionModel()->selectedRows();
		
		std::vector< int > index( selected.size() );
		for( int i = 0; i < selected.size(); ++i ) {
			index[i] = selected[i].row();
		}
		
		// TODO call converter
		Converter conv( NULL, output );
	}
	
	// TODO Call CUE parser
	void MainWindow::openFile() {
		QString fileName = QFileDialog::getOpenFileName( this, tr( "Open CUE sheet" ), QDir::homePath(), "*.cue" );
		CUESheet songlist( fileName.toStdString() );
		setSongList( songlist.getTrackInfo() );
	}
	
	void MainWindow::setSongList( const std::vector< CUESheet::FieldType > & list ) {
		QStandardItemModel * model = qobject_cast< QStandardItemModel * >( _songList_->model() );
		QStandardItem * item;
		
		for( std::size_t row = 0; row < list.size(); ++row ) {
			for( int col = 0; col < CUESheet::Track::SIZE; ++col ) {
				item = new QStandardItem( list[row][col].c_str() );
				model->setItem( row, col, item );
			}
		}
	}
	
}
