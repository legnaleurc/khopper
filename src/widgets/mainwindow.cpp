#include "mainwindow.hpp"
#include "songlist.hpp"
#include "threads.hpp"
#include "textcodec.hpp"
#include "os.hpp"
#include "cuesheet.hpp"
#include "track.hpp"
#include "encoder.hpp"
#include "abstractoption.hpp"
#include "error.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QLabel>
#include <QTabWidget>
#include <QtDebug>

// for Track to QVariant
Q_DECLARE_METATYPE( Khopper::TrackSP )

namespace Khopper {

	MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags flags ):
	QMainWindow( parent, flags ),
	codec_( new TextCodec( this ) ),
	songListView_( new SongListView( this ) ),
	songListModel_( new QStandardItemModel( songListView_ ) ),
	optionTabs_( new QTabWidget( this ) ),
	action_( new QPushButton( tr( "Fire!" ), this ) ),
	progress_( new QProgressDialog( tr( "Converting..." ), tr( "Don\'t touch me!" ), 0, 0, this ) ),
	cvt_( new ConverterThread( this ) ),
	about_( new QWidget( this, Qt::Dialog ) ) {
		this->initAbout_();

		// Setting menu bar
		this->initMenuBar_();

		// Setting central widget
		QWidget * central = new QWidget( this );
		central->setLayout( new QVBoxLayout( central ) );
		this->setCentralWidget( central );

		// Add song list
		central->layout()->addWidget( songListView_ );
		this->initHeader_();
		// Set model
		this->songListView_->setModel( songListModel_ );
		connect( this->songListView_, SIGNAL( openFile( const QString & ) ), this, SLOT( open( const QString & ) ) );

		// Set bottom layout
		QWidget * bottom = new QWidget( central );
		bottom->setLayout( new QHBoxLayout( bottom ) );
		central->layout()->addWidget( bottom );

		// Setting output format select
		bottom->layout()->addWidget( this->optionTabs_ );
		this->initOptionTabs_();

		// Action button
		connect( action_, SIGNAL( clicked() ), this, SLOT( fire_() ) );
		bottom->layout()->addWidget( action_ );

		// Progress dialog
		progress_->setWindowModality( Qt::WindowModal );
		progress_->setMinimumDuration( 0 );

		// Converter thread
		connect( cvt_, SIGNAL( finished() ), progress_, SLOT( cancel() ) );
		connect( cvt_, SIGNAL( step( int ) ), this, SLOT( incProgress_( int ) ) );
		connect( cvt_, SIGNAL( error( const QString &, const QString & ) ), this, SLOT( showErrorMessage_( const QString &, const QString & ) ) );
		// FIXME: in fact, this don't work
		connect( progress_, SIGNAL( canceled() ), cvt_, SLOT( terminate() ) );
	}

	void MainWindow::incProgress_( int diff ) {
		this->progress_->setValue( this->progress_->value() + diff );
	}

	void MainWindow::initMenuBar_() {
		QMenuBar * menuBar = new QMenuBar( this );

		// setting file menu
		QMenu * file = new QMenu( tr( "&File" ), menuBar );

		QAction * open = new QAction( tr( "&Open..." ), this );
		open->setShortcut( tr( "Ctrl+O" ) );
		connect( open, SIGNAL( triggered() ), this, SLOT( showOpenFileDialog() ) );
		file->addAction( open );

		// add file menu to menu bar
		menuBar->addMenu( file );
		// file menu done

		// setting help menu
		QMenu * help = new QMenu( tr( "&Help" ), menuBar );

		QAction * about = new QAction( tr( "&About Khopper" ), this );
		connect( about, SIGNAL( triggered() ), about_, SLOT( show() ) );
		help->addAction( about );

		QAction * aboutQt = new QAction( tr( "About &Qt" ), this );
		connect( aboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
		help->addAction( aboutQt );

		// add help menu to menu bar
		menuBar->addMenu( help );
		// help menu done

		setMenuBar( menuBar );
	}

	void MainWindow::initOptionTabs_() {
		// Take out the output types
		const EncoderList::ObjectType & tm = EncoderList::Instance();
		for( EncoderList::ObjectType::const_iterator it = tm.begin(); it != tm.end(); ++it ) {
			// it->first is object factory key, it->second is display name
			this->optionTabs_->addTab( UIFactory::Instance().CreateObject( it->first ), QString::fromStdString( it->second ) );
		}
	}

	void MainWindow::initHeader_() {
		QStringList headers;
		for( std::vector< std::wstring >::const_iterator it = Track::Headers.begin(); it != Track::Headers.end(); ++it ) {
			headers.push_back( QString::fromStdWString( *it ) );
		}
		songListModel_->setHorizontalHeaderLabels( headers );
	}

	void MainWindow::fire_() {
		// get saving directory
		QString outDir = QFileDialog::getExistingDirectory( this, tr( "Target Directory" ), QDir::homePath() );
		// TODO: use qobject_cast
		AbstractOption * option = dynamic_cast< AbstractOption * >( this->optionTabs_->currentWidget() );

		if( outDir != "" && option ) {
			try {
				// create encoder object
				EncoderSP output( option->getEncoder() );

				// get selected list
				QModelIndexList selected = this->songListView_->selectionModel()->selectedRows( 0 );

				// put selected list
				std::vector< TrackSP > tracks( selected.size() );
				int decodeTimes = 0;
				for( int i = 0; i < selected.size(); ++i ) {
					tracks[i] = selected[i].data( Qt::UserRole ).value< TrackSP >();
					decodeTimes += static_cast< int >( tracks[i]->duration.toDouble() * 100 );
				}

				// set progress bar
				progress_->setRange( 0, decodeTimes );
				// set output information
				// TODO:
				// not only target directory, but filenames
				// I'll let cvt_ to do this, but this really needs change
				cvt_->setOutput( output, outDir );
				cvt_->setTracks( tracks );
				cvt_->start();
				progress_->show();
			} catch( Error< RunTime > & e ) {
				this->showErrorMessage_( tr( "Run-time error!" ), tr( e.what() ) );
			} catch( std::exception & e ) {
				this->showErrorMessage_( tr( "Unknown error!" ), tr( e.what() ) );
			}
		}
	}

	void MainWindow::showOpenFileDialog() {
		QString filePath = QFileDialog::getOpenFileName( this, tr( "Open CUE sheet" ), QDir::homePath(), "*.cue" );
		this->open( filePath );
	}

	void MainWindow::open( const QString & filePath ) {
		if( filePath != "" ) {
			std::vector< TrackSP > tracks;

			if( QFileInfo( filePath ).suffix() == "cue" ) {
				QFile fin( filePath );
				if( !fin.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
					this->showErrorMessage_( tr( "File open error!" ), tr( "Could not open the file: `" ) + filePath + "\'" );
				}

				QByteArray raw_input = fin.readAll();
				fin.close();

				codec_->setEncoded( raw_input );

				if( codec_->exec() ) {
					try {
						tracks = CUESheet( codec_->getDecoded().toStdWString(), QFileInfo( filePath ).absolutePath().toStdWString() ).tracks;
					} catch( std::exception & e ) {
						this->showErrorMessage_( tr( "Error on parsing CUE Sheet!" ), tr( e.what() ) );
					}
				}
			} else {
				this->showErrorMessage_( tr( "Run-time error!" ), tr( "Not implemented." ) );
			}

			this->addSongList( tracks );
		}
	}

	void MainWindow::addSongList( const std::vector< TrackSP > & tracks ) {
		// get last row number
		int offset = songListModel_->rowCount();
		// add all tracks
		for( std::size_t row = 0; row < tracks.size(); ++row ) {
			this->songListModel_->setItem( row + offset, 0, new QStandardItem( QString::fromStdWString( tracks[row]->title ) ) );
			this->songListModel_->setItem( row + offset, 1, new QStandardItem( QString::fromStdWString( tracks[row]->performer ) ) );
			this->songListModel_->setItem( row + offset, 2, new QStandardItem( QString::fromStdWString( tracks[row]->duration.toStdWString() ) ) );
			this->songListModel_->setItem( row + offset, 3, new QStandardItem( QString::number( tracks[row]->bitRate ) ) );
			this->songListModel_->setItem( row + offset, 4, new QStandardItem( QString::number( tracks[row]->sampleRate ) ) );
			this->songListModel_->setItem( row + offset, 5, new QStandardItem( QString::number( tracks[row]->channels ) ) );

			// add extra information
			QVariant data;
			data.setValue( tracks[row] );
			this->songListModel_->item( row + offset, 0 )->setData( data, Qt::UserRole );
		}
	}

	void MainWindow::initAbout_() {
		about_->setWindowTitle( tr( "About Khopper" ) );
		about_->resize( 320, 240 );
		
		QVBoxLayout * vbl = new QVBoxLayout( about_ );
		about_->setLayout( vbl );

		QTabWidget * tw = new QTabWidget( about_ );
		vbl->addWidget( tw );

		QLabel * about = new QLabel( tr(
			"An audio converter<br/>"
			"(C) 2008 ~<br/>"
			"Present by legnaleurc<br/>"
			"E-Mail: <a href=\"mailto:legnaleurc@gmail.com\">legnaleurc@gmail.com</a><br/>"
			"<a href=\"http://legnaleurc.blogspot.com/\">Home Page</a>"
		), tw );
		about->setTextFormat( Qt::RichText );
		tw->addTab( about, tr( "&About" ) );
	}

	void MainWindow::showErrorMessage_( const QString & title, const QString & msg ) {
		QMessageBox::critical( this, title, msg );
	}

}
