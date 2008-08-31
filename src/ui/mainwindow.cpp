#include "ui/mainwindow.hpp"
#include "ui/songlist.hpp"
#include "ui/threads.hpp"
#include "types.hpp"
#include "util/os.hpp"
#include "cue/cuesheet.hpp"
#include "cue/track.hpp"
#include <QtGui>

// for Track to QVariant
Q_DECLARE_METATYPE( Khopper::Track )

namespace Khopper {
	
	MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags flags ) : QMainWindow( parent, flags ) {
		// setTitle
		setWindowTitle( tr( "Khopper" ) );
		
		initAbout_();
		
		// Setting menu bar
		initMenuBar_();
		
		// Setting central widget
		QPointer< QWidget > central = new QWidget( this );
		central->setLayout( new QVBoxLayout( central ) );
		setCentralWidget( central );
		
		// Add song list
		songListView_ = new SongListView( central );
		songListModel_ = new QStandardItemModel( songListView_ );
		central->layout()->addWidget( songListView_ );
		initHeader_();
		// Set model
		songListView_->setModel( songListModel_ );
		connect( songListView_, SIGNAL( openFile( const QString & ) ), this, SLOT( open( const QString & ) ) );
		
		// Set bottom layout
		QPointer< QWidget > bottom = new QWidget( central );
		bottom->setLayout( new QHBoxLayout( bottom ) );
		central->layout()->addWidget( bottom );
		
		// Setting output format select
		outputTypes_ = new QComboBox( bottom );
		bottom->layout()->addWidget( outputTypes_ );
		initOutputTypeList_();
		
		// Action button
		action_ = new QPushButton( tr( "Fire!" ), bottom );
		connect( action_, SIGNAL( clicked() ), this, SLOT( fire_() ) );
		bottom->layout()->addWidget( action_ );
		
		// Progress dialog
		progress_ = new QProgressDialog( tr( "Converting..." ), tr( "Don\'t touch me!" ), 0, 0, this );
		progress_->setWindowModality( Qt::WindowModal );
		progress_->setMinimumDuration( 0 );
		
		// Converter thread
		cvt_ = new ConverterThread( this );
		connect( cvt_, SIGNAL( finished() ), progress_, SLOT( cancel() ) );
		connect( cvt_, SIGNAL( step( int ) ), progress_, SLOT( setValue( int ) ) );
		connect( cvt_, SIGNAL( error( const QString & ) ), this, SLOT( runTimeError_( const QString & ) ) );
		// FIXME: in fact, this don't work
		connect( progress_, SIGNAL( canceled() ), cvt_, SLOT( terminate() ) );
	}
	
	void MainWindow::initMenuBar_() {
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
		connect( about, SIGNAL( triggered() ), about_, SLOT( show() ) );
		help->addAction( about );
		
		QPointer< QAction > aboutQt = new QAction( tr( "About &Qt" ), this );
		connect( aboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
		help->addAction( aboutQt );
		
		// add help menu to menu bar
		menuBar->addMenu( help );
		// help menu done
		
		setMenuBar( menuBar );
	}
	
	void MainWindow::initOutputTypeList_() {
		// Take out the output types
		const OutputList & tm = IOTypes::Instance().second;
		for( OutputList::const_iterator it = tm.begin(); it != tm.end(); ++it ) {
			outputTypes_->addItem( it->second.c_str(), QVariant( it->first.c_str() ) );
		}
	}
	
	void MainWindow::initHeader_() {
		songListModel_->setHorizontalHeaderLabels( Track::headers );
	}
	
	void MainWindow::fire_() {
		QString test = outputTypes_->itemData( outputTypes_->currentIndex() ).toString();
		QString outDir = QFileDialog::getExistingDirectory( this, tr( "Target Directory" ), QDir::homePath() );

		if( outDir != "" ) {
			try {
				// create output format object
				OutputSP output( OutputFactory::Instance().CreateObject( test.toStdString() ) );

				// get select list
				QModelIndexList selected = songListView_->selectionModel()->selectedRows( 0 );

				std::vector< Track > tracks( selected.size() );
				for( int i = 0; i < selected.size(); ++i ) {
					tracks[i] = selected[i].data( Qt::UserRole ).value< Track >();
				}

				progress_->setRange( 0, tracks.size() );
				cvt_->setOutput( output, outDir );
				cvt_->setTracks( tracks );
				cvt_->start();
				progress_->show();
			} catch( Error< RunTime > & e ) {
				runTimeError_( tr( e.what() ) );
			} catch( std::exception & e ) {
				QMessageBox::critical( this, tr( "Unknown error!" ), tr( e.what() ) );
			}
		}
	}
	
	void MainWindow::openFileDialog() {
		QString fileName = QFileDialog::getOpenFileName( this, tr( "Open CUE sheet" ), QDir::homePath(), "*.cue" );
		open( fileName );
	}
	
	void MainWindow::open( const QString & fileName ) {
		if( fileName != "" ) {
			QFile fin( fileName );
			if( !fin.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
				QMessageBox::critical( this, tr( "File open error!" ), tr( "Could not open the file: `" ) + fileName + "\'" );
			}
			// TODO: set codec
			QTextStream ts( &fin );
			QStringList lines;
			QString line( ts.readLine() );
			while( !line.isNull() ) {
				lines.push_back( line );
				line = ts.readLine();
			}
			addSongList( CUESheet( lines, QFileInfo( fileName ).absolutePath() ) );
		}
	}
	
	void MainWindow::addSongList( const CUESheet & sheet ) {
		int offset = songListModel_->rowCount();
		for( std::size_t row = 0; row < sheet.size(); ++row ) {
			songListModel_->setItem( row + offset, 0, new QStandardItem( sheet[row].getTitle() ) );
			songListModel_->setItem( row + offset, 1, new QStandardItem( sheet[row].getPerformer() ) );

			QVariant data;
			data.setValue( sheet[row] );
			songListModel_->item( row + offset, 0 )->setData( data, Qt::UserRole );
		}
	}
	
	void MainWindow::initAbout_() {
		about_ = new QWidget( this, Qt::Dialog );
		about_->setWindowTitle( tr( "About Khopper" ) );
		about_->resize( 320, 240 );
		
		QPointer< QVBoxLayout > vbl = new QVBoxLayout( about_ );
		about_->setLayout( vbl );

		QPointer< QTabWidget > tw = new QTabWidget( about_ );
		vbl->addWidget( tw );

		QPointer< QLabel > about = new QLabel( tr(
			"An audio converter<br/>"
			"(C) 2008 ~<br/>"
			"Present by legnaleurc<br/>"
			"E-Mail: <a href=\"mailto:legnaleurc@gmail.com\">legnaleurc@gmail.com</a><br/>"
			"<a href=\"http://legnaleurc.blogspot.com/\">Home Page</a>"
		), tw );
		about->setTextFormat( Qt::RichText );
		tw->addTab( about, tr( "&About" ) );
	}
	
	void MainWindow::runTimeError_( const QString & msg ) {
		QMessageBox::critical( this, tr( "Run-time error!" ), msg );
	}
	
}
