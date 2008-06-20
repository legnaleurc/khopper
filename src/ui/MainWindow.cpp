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
		_songList_ = new SongList( central );
		central->layout()->addWidget( _songList_ );
		// Set model
		_songList_->setModel( new QStandardItemModel( _songList_ ) );
		// Set header of model
		_setLabel_();
		// Set selection behavior
		_songList_->setSelectionBehavior( QAbstractItemView::SelectRows );
		connect( _songList_, SIGNAL( openFile( const QString & ) ), this, SLOT( open( const QString & ) ) );
		
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
		connect( open, SIGNAL( triggered() ), this, SLOT( openFileDialog() ) );
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
		// Take out the output types
		const OutputList & tm = IOTypes::Instance().second;
		for( OutputList::const_iterator it = tm.begin(); it != tm.end(); ++it ) {
			_outputTypes_->addItem( it->second.c_str(), QVariant( it->first.c_str() ) );
		}
	}
	
	void MainWindow::_fire_() {
		// create output format object
		QString test = _outputTypes_->itemData( _outputTypes_->currentIndex() ).toString();
		Output * output = OutputFactory::Instance().CreateObject( test.toStdString() );
		Input * input = InputFactory::Instance().CreateObject( "in::Ape" );
		
		// get select list
		QModelIndexList selected = _songList_->selectionModel()->selectedRows();
		
		std::vector< int > index( selected.size() );
		for( int i = 0; i < selected.size(); ++i ) {
			// FIXME: dirty hack!
			index[i] = selected[i].row() + 1;
		}
		
		// Create converter
		try {
			Converter conv( input, output );
			conv.perform( _audioPath_, _sheetPath_, index );
		} catch( const Error< RunTime > & e ) {
			QMessageBox::critical( this, tr( "Runtime error!" ), tr( e.what() ) );
		}
	}
	
	void MainWindow::openFileDialog() {
		QString fileName = QFileDialog::getOpenFileName( this, tr( "Open CUE sheet" ), QDir::homePath(), "*.cue" );
		open( fileName );
	}
	
	void MainWindow::open( const QString & file ) {
		if( file != "" ) {
			CUESheet songlist( file.toUtf8().constData() );
			setSongList( songlist.getTrackInfo() );
			_audioPath_ = songlist.getAudioName().first + "/" + songlist.getAudioName().second;
			_sheetPath_ = songlist.getSheetName().first + "/" + songlist.getSheetName().second;
		}
	}
	
	void MainWindow::setSongList( const std::vector< CUESheet::FieldType > & list ) {
		QStandardItemModel * model = qobject_cast< QStandardItemModel * >( _songList_->model() );
		QStandardItem * item;
		
		model->setRowCount( 0 );
		
		for( std::size_t row = 0; row < list.size(); ++row ) {
			for( int col = 0; col < CUESheet::Track::SIZE; ++col ) {
				item = new QStandardItem( QString::fromUtf8( list[row][col].c_str() ) );
				model->setItem( row, col, item );
			}
		}
	}
	
}
