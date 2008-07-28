#include "MainWindow.hpp"

#include <iostream>

namespace Khopper {
	
	MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags flags ) : QMainWindow( parent, flags ) {
		// setTitle
		setWindowTitle( tr( "Khopper" ) );
		
		setAbout_();
		
		// Setting menu bar
		setMenuBar( setMenu_() );
		
		// Setting central widget
		QPointer< QWidget > central = new QWidget( this );
		central->setLayout( new QVBoxLayout( central ) );
		setCentralWidget( central );
		
		// Add song list
		songList_ = new SongList( central );
		central->layout()->addWidget( songList_ );
		// Set model
		songList_->setModel( new QStandardItemModel( songList_ ) );
		// Set header of model
		setLabel_();
		// Set selection behavior
		songList_->setSelectionBehavior( QAbstractItemView::SelectRows );
		connect( songList_, SIGNAL( openFile( const QString & ) ), this, SLOT( open( const QString & ) ) );
		
		// Set bottom layout
		QPointer< QWidget > bottom = new QWidget( central );
		bottom->setLayout( new QHBoxLayout( bottom ) );
		central->layout()->addWidget( bottom );
		
		// Setting output format select
		outputTypes_ = new QComboBox( bottom );
		bottom->layout()->addWidget( outputTypes_ );
		setOutputTypeList_();
		
		// Action button
		action_ = new QPushButton( tr( "Fire!" ), bottom );
		connect( action_, SIGNAL( clicked() ), this, SLOT( fire_() ) );
		bottom->layout()->addWidget( action_ );
		
		// Progress dialog
		progress_ = new QProgressDialog( tr( "Converting..." ), tr( "Don\'t touch me!" ), 0, 0, this );
		progress_->setWindowModality( Qt::WindowModal );
		progress_->setMinimumDuration( 0 );
		pdTimer_ = new QTimer( this );
		connect( pdTimer_, SIGNAL( timeout() ), this, SLOT( stepProgress_() ) );
		connect( progress_, SIGNAL( canceled() ), pdTimer_, SLOT( stop() ) );
		
		// Converter thread
		cvt_ = new ConverterThread( this );
		connect( cvt_, SIGNAL( finished() ), progress_, SLOT( cancel() ) );
		connect( cvt_, SIGNAL( error( const QString & ) ), this, SLOT( runTimeError_( const QString & ) ) );
		// FIXME: in fact, this don't work
		connect( progress_, SIGNAL( canceled() ), cvt_, SLOT( terminate() ) );
	}
	
	QPointer< QMenuBar > MainWindow::setMenu_() {
		QPointer< QMenuBar > menuBar = new QMenuBar( this );
		
		// setting file menu
		QPointer< QMenu > file = new QMenu( tr( "&File" ), menuBar );
		
		QPointer< QAction > open = new QAction( tr( "&Open..." ), this );
		open->setShortcut( tr( "Ctrl+O" ) );
		connect( open, SIGNAL( triggered() ), this, SLOT( openFileDialog() ) );
		file->addAction( open );
		
		// add file menu to menu bar
		menuBar->addMenu( file );
		// file menu done
		
		// setting help menu
		QPointer< QMenu > help = new QMenu( tr( "&Help" ), menuBar );
		
		QPointer< QAction > about = new QAction( tr( "&About Khopper" ), this );
		connect( about, SIGNAL( triggered() ), about_, SLOT( exec() ) );
		help->addAction( about );
		
		QPointer< QAction > aboutQt = new QAction( tr( "About &Qt" ), this );
		connect( aboutQt, SIGNAL( triggered() ), this, SLOT( showAboutQt_() ) );
		help->addAction( aboutQt );
		
		// add help menu to menu bar
		menuBar->addMenu( help );
		// help menu done
		
		return menuBar;
	}
	
	void MainWindow::setLabel_() {
		QPointer< QStandardItemModel > model = qobject_cast< QStandardItemModel * >( songList_->model() );
		QStringList headers;
		
		for( std::size_t i = 0; i < CUESheet::headers.size(); ++i ) {
			headers << CUESheet::headers[i].c_str();
		}
		
		model->setHorizontalHeaderLabels( headers );
	}
	
	void MainWindow::setOutputTypeList_() {
		// Take out the output types
		const OutputList & tm = IOTypes::Instance().second;
		for( OutputList::const_iterator it = tm.begin(); it != tm.end(); ++it ) {
			outputTypes_->addItem( it->second.c_str(), QVariant( it->first.c_str() ) );
		}
	}
	
	void MainWindow::fire_() {
		// create output format object
		QString test = outputTypes_->itemData( outputTypes_->currentIndex() ).toString();
		OutputSP output;
		InputSP input;
		
		try {
			output = OutputFactory::Instance().CreateObject( test.toStdString() );
			input = InputFactory::Instance().CreateObject( QFileInfo( QString::fromStdString( audioPath_ ) ).suffix().toStdString() );
			
			// get select list
			QModelIndexList selected = songList_->selectionModel()->selectedRows();
			
			std::vector< int > index( selected.size() );
			for( int i = 0; i < selected.size(); ++i ) {
				// FIXME: dirty hack!
				index[i] = selected[i].row() + 1;
			}
			
			cvt_->setAudio( audioPath_ );
			cvt_->setSheet( sheetPath_ );
			cvt_->setIndex( index );
			cvt_->setInput( input );
			cvt_->setOutput( output );
			cvt_->start();
			pdTimer_->start( 50 );
		} catch( const Error< RunTime > & e ) {
			runTimeError_( tr( e.what() ) );
		} catch( const std::exception & e ) {
			QMessageBox::critical( this, tr( "Unknown error!" ), tr( e.what() ) );
		}
	}
	
	void MainWindow::openFileDialog() {
		QString fileName = QFileDialog::getOpenFileName( this, tr( "Open CUE sheet" ), QDir::homePath(), "*.cue" );
		open( fileName );
	}
	
	void MainWindow::open( const QString & file ) {
		if( file != "" ) {
			CUESheet songlist( file.toUtf8().constData() );
			setSongList( songlist );
// 			audioPath_ = songlist.getAudioName().first + "/" + songlist.getAudioName().second;
// 			sheetPath_ = songlist.getSheetName().first + "/" + songlist.getSheetName().second;
		}
	}
	
	void MainWindow::setSongList( const CUESheet & sheet ) {
		QPointer< QStandardItemModel > model = qobject_cast< QStandardItemModel * >( songList_->model() );
		
		model->setRowCount( 0 );
		
		for( std::size_t row = 0; row < sheet.size(); ++row ) {
			model->setItem( row, 0, new QStandardItem( QString::fromAscii( sheet[row].title.c_str() ) ) );
			model->setItem( row, 1, new QStandardItem( QString::fromAscii( sheet[row].performer.c_str() ) ) );
		}
	}
	
	void MainWindow::stepProgress_() {
		// Don't worry about multithread, I don't care.
		static int i = 1;
		// FIXME
		if( !progress_->wasCanceled() ) {
			progress_->setValue( i );
			( i < 0 ) ? ( i = 0 ) : ( ++i );
		} else {
			pdTimer_->stop();
		}
	}
	
	void MainWindow::showAboutQt_() {
		QMessageBox::aboutQt( this );
	}
	
	void MainWindow::showAbout_() {
		QMessageBox::about( this, tr( "About Khopper" ), tr( "<a href=\"http://legnaleurc.blogspot.com/\">Home Page</a>" ) );
	}
	
	void MainWindow::setAbout_() {
		about_ = new QMessageBox( QMessageBox::Information, tr( "About Khopper" ), tr( "<a href=\"http://legnaleurc.blogspot.com/\">Home Page</a>" ), QMessageBox::Close, this );
	}
	
	void MainWindow::runTimeError_( const QString & msg ) {
		QMessageBox::critical( this, tr( "Run-time error!" ), msg );
	}
	
}
