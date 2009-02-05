/**
 * @file mainwindow.cpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * Khopper is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Khopper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
#include <QPushButton>
#include <QtDebug>

// for Track to QVariant
Q_DECLARE_METATYPE( Khopper::TrackSP )

namespace {

	inline bool indexRowCompD( const QModelIndex & l, const QModelIndex & r ) {
		return l.row() > r.row();
	}

}

namespace Khopper {

	MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags flags ):
	QMainWindow( parent, flags ),
	codec_( new TextCodec( this ) ),
	songListView_( new SongListView( this ) ),
	songListModel_( new QStandardItemModel( this->songListView_ ) ),
	optionTabs_( new QTabWidget( this ) ),
	outputPath_( new QLineEdit( QDir::homePath(), this ) ),
	useSourcePath_( new QCheckBox( tr( "Use source path" ), this ) ),
	fileNameTemplate_( new QLineEdit( tr( "%t" ), this ) ),
	progress_( new QProgressDialog( tr( "Converting..." ), tr( "Cancel" ), 0, 0, this ) ),
	cvt_( new ConverterThread( this ) ),
	about_( new QWidget( this, Qt::Dialog ) ) {
		// Setting about widget
		this->initAbout_();
		// Setting menu bar
		this->initMenuBar_();

		// Setting central widget
		QWidget * central = new QWidget( this );
		QVBoxLayout * mainBox = new QVBoxLayout( central );
		central->setLayout( mainBox );
		this->setCentralWidget( central );

		// Add song list
		mainBox->addWidget( this->songListView_ );
		this->initHeader_();
		// Set model
		this->songListView_->setModel( this->songListModel_ );
		connect( this->songListView_, SIGNAL( openFile( const QString & ) ), this, SLOT( open( const QString & ) ) );
		QAction * delSong = new QAction( this->songListView_ );
		delSong->setShortcut( QKeySequence::Delete );
		this->songListView_->addAction( delSong );
		connect( delSong, SIGNAL( triggered() ), this, SLOT( delSongList_() ) );

		// Setting output format select
		mainBox->addWidget( this->optionTabs_ );
		this->initOptionTabs_();

		QHBoxLayout * pathBox = new QHBoxLayout();
		mainBox->addLayout( pathBox );

		// Output path setting
		QLabel * outputLabel = new QLabel( tr( "Output to:" ), this );
		pathBox->addWidget( outputLabel );
		pathBox->addWidget( this->outputPath_ );
		QPushButton * changePath = new QPushButton( tr( "..." ), this );
		pathBox->addWidget( changePath );
		connect( changePath, SIGNAL( clicked() ), this, SLOT( changeOutputPath_() ) );
		pathBox->addWidget( this->useSourcePath_ );

		this->useSourcePath_->setChecked( true );
		this->outputPath_->setEnabled( false );
		connect( this->useSourcePath_, SIGNAL( toggled( bool ) ), this->outputPath_, SLOT( setDisabled( bool ) ) );
		connect( changePath, SIGNAL( clicked( bool ) ), this->useSourcePath_, SLOT( setChecked( bool ) ) );

		// Set bottom layout
		QHBoxLayout * bottomBox = new QHBoxLayout();
		mainBox->addLayout( bottomBox );

		// Output name template
		QLabel * tpLabel = new QLabel( tr( "File Name:" ), this );
		bottomBox->addWidget( tpLabel );
		this->fileNameTemplate_->setToolTip( tr(
			"Keywords:\n"
			"%t: title\n"
			"%a: author\n"
			"%%: \'%\' literal\n"
			"Extensions will automaticaly append."
		) );
		bottomBox->addWidget( this->fileNameTemplate_ );

		// Action button
		QPushButton * action = new QPushButton( tr( "Fire!" ), this );
		connect( action, SIGNAL( clicked() ), this, SLOT( fire_() ) );
		bottomBox->addWidget( action );

		// Progress dialog
		progress_->setWindowModality( Qt::WindowModal );
		progress_->setMinimumDuration( 0 );

		// Converter thread
		connect( cvt_, SIGNAL( finished() ), progress_, SLOT( cancel() ) );
		connect( cvt_, SIGNAL( step( int ) ), this, SLOT( incProgress_( int ) ) );
		connect( cvt_, SIGNAL( error( const QString &, const QString & ) ), this, SLOT( showErrorMessage_( const QString &, const QString & ) ) );
		// NOTE: works, but danger
		connect( progress_, SIGNAL( canceled() ), cvt_, SLOT( terminate() ) );
	}

	void MainWindow::changeOutputPath_() {
		QString outDir = QFileDialog::getExistingDirectory( this, tr( "Target Directory" ), QDir::homePath() );
		if( !outDir.isEmpty() ) {
			this->outputPath_->setText( outDir );
		}
	}

	void MainWindow::delSongList_() {
		QModelIndexList selected = this->songListView_->selectionModel()->selectedRows();
		std::sort( selected.begin(), selected.end(), ::indexRowCompD );
		foreach( QModelIndex index, selected ) {
			this->songListModel_->removeRow( index.row() );
		}
		this->songListView_->selectionModel()->clear();
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

	QString MainWindow::applyTemplate_( TrackSP track ) const {
		QString tmp = this->fileNameTemplate_->text();
		tmp.replace( "%t", QString::fromStdWString( track->title ) );
		tmp.replace( "%a", QString::fromStdWString( track->performer ) );
		tmp.replace( "%%", "%" );
		return tmp;
	}

	void MainWindow::fire_() {
		// get selected list
		QModelIndexList selected = this->songListView_->selectionModel()->selectedRows( 0 );
		if( selected.isEmpty() ) {
			this->showErrorMessage_( tr( "Run-time error!" ), tr( "No track selected." ) );
			return;
		}
		// get option widget
		AbstractOption * option = qobject_cast< AbstractOption * >( this->optionTabs_->currentWidget() );

		QString outDir = this->outputPath_->text();

		if( !option ) {
			this->showErrorMessage_( tr( "Run-time error!" ), tr( "Bad output plugin" ) );
		} else if( outDir.isEmpty() ) {
			this->showErrorMessage_( tr( "Run-time error!" ), tr( "Bad output path" ) );
		} else {
			try {
				// create encoder object
				EncoderSP output( option->getEncoder() );

				// put selected list
				std::vector< TrackSP > tracks( selected.size() );
				QStringList outputPaths;
				int decodeTimes = 0;
				QString ext = this->optionTabs_->tabText( this->optionTabs_->currentIndex() );
				for( int i = 0; i < selected.size(); ++i ) {
					tracks[i] = selected[i].data( Qt::UserRole ).value< TrackSP >();
					outputPaths.push_back( outDir + "/" + this->applyTemplate_( tracks[i] ) + "." + ext );
					decodeTimes += static_cast< int >( tracks[i]->duration.toDouble() * 100 );
				}

				// set progress bar
				progress_->setRange( 0, decodeTimes );
				// set output information
				cvt_->setOutput( output, outputPaths );
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
			QFileInfo fI( filePath );

			if( this->useSourcePath_->isChecked() ) {
				this->outputPath_->setText( fI.absolutePath() );
			}

			std::vector< TrackSP > tracks;

			if( fI.suffix() == "cue" ) {
				QFile fin( filePath );
				if( !fin.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
					this->showErrorMessage_( tr( "File open error!" ), tr( "Could not open the file: `" ) + filePath + "\'" );
				}

				QByteArray raw_input = fin.readAll();
				fin.close();

				codec_->setEncoded( raw_input );

				if( codec_->exec() ) {
					try {
						tracks = CUESheet( codec_->getDecoded().toStdWString(), fI.absolutePath().toStdWString() ).tracks;
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
			"(C) 2008 ~ 2009<br/>"
			"Present by Wei-Cheng Pan<br/>"
			"E-Mail: <a href=\"mailto:legnaleurc@gmail.com\">legnaleurc@gmail.com</a><br/>"
			"<a href=\"http://legnaleurc.blogspot.com/search/label/Khopper\">Home Page</a>"
		), tw );
		about->setTextFormat( Qt::RichText );
		tw->addTab( about, tr( "&About" ) );
	}

	void MainWindow::showErrorMessage_( const QString & title, const QString & msg ) {
		QMessageBox::critical( this, title, msg );
	}

}
