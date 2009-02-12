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
#include "progress.hpp"
#include "os.hpp"
#include "cuesheet.hpp"
#include "track.hpp"
#include "encoder.hpp"
#include "abstractoption.hpp"
#include "error.hpp"

#include <boost/format.hpp>

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
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QtDebug>

namespace {

	QString applyFormat( QString tpl, Khopper::TrackCSP track ) {
		std::wstring result = ( boost::wformat( tpl.toStdWString() ) % track->title % track->artist % track->index ).str();
		return QString::fromStdWString( result );
	}

}

namespace Khopper {

	MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags flags ):
	QMainWindow( parent, flags ),
	codec_( new TextCodec( this ) ),
	songList_( new SongList( this ) ),
	optionWindow_( new QDialog( this ) ),
	optionTabs_( new QTabWidget( this ) ),
	outputPath_( new QLineEdit( QDir::homePath(), this ) ),
	useSourcePath_( new QCheckBox( tr( "Same as source directories" ), this ) ),
	fileNameTemplate_( new QLineEdit( tr( "%t" ), this ) ),
	progress_( new Progress( this ) ),
	cvt_( new ConverterThread( this ) ),
	preference_( new QWidget( this, Qt::Dialog ) ),
	about_( new QWidget( this, Qt::Dialog ) ) {
		// Setting menu bar
		this->initMenuBar_();
		// Setting about widget
		this->initAbout_();
		// Setting preference widget
		this->initPreference_();
		// Setting output format select
		this->initOptionWindow_();

		// Setting central widget
		QWidget * central = new QWidget( this );
		QVBoxLayout * mainBox = new QVBoxLayout( central );
		central->setLayout( mainBox );
		this->setCentralWidget( central );

		// Add song list
		mainBox->addWidget( this->songList_ );
		connect( this->songList_, SIGNAL( dropFile( const QString & ) ), this, SLOT( open( const QString & ) ) );
		connect( this->songList_, SIGNAL( requireConvert() ), this, SLOT( fire_() ) );

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

		// Progress dialog
		progress_->setWindowModality( Qt::WindowModal );

		// Converter thread
		connect( this->cvt_, SIGNAL( taskName( const QString & ) ), this->progress_, SLOT( setItemName( const QString & ) ) );
		connect( this->cvt_, SIGNAL( taskGoal( int ) ), this->progress_, SLOT( setMaximum( int ) ) );
		connect( this->cvt_, SIGNAL( currentTask( int ) ), this->progress_, SLOT( setCurrent( int ) ) );
		connect( this->cvt_, SIGNAL( step( int ) ), this, SLOT( incProgress_( int ) ) );
		connect( this->cvt_, SIGNAL( finished() ), this->progress_, SLOT( accept() ) );
		connect( this->cvt_, SIGNAL( error( const QString &, const QString & ) ), this, SLOT( showErrorMessage_( const QString &, const QString & ) ) );
		// NOTE: works, but danger
		connect( this->progress_, SIGNAL( rejected() ), this->cvt_, SLOT( cancel() ) );
	}

	void MainWindow::changeOutputPath_() {
		QString outDir = QFileDialog::getExistingDirectory( this, tr( "Target Directory" ), QDir::homePath() );
		if( !outDir.isEmpty() ) {
			this->outputPath_->setText( outDir );
		}
	}

	void MainWindow::incProgress_( int diff ) {
		this->progress_->setValue( this->progress_->getValue() + diff );
	}

	void MainWindow::initMenuBar_() {
		QMenuBar * menuBar = new QMenuBar( this );

		// setting file menu
		QMenu * file = new QMenu( tr( "&File" ), menuBar );

		QAction * open = new QAction( tr( "&Open album" ), this );
		open->setShortcut( tr( "Ctrl+O" ) );
		connect( open, SIGNAL( triggered() ), this, SLOT( showOpenFileDialog() ) );
		file->addAction( open );

		// add file menu to menu bar
		menuBar->addMenu( file );
		// file menu done

		// setting tool menu
		QMenu * tool = new QMenu( tr( "&Tool" ), menuBar );
		menuBar->addMenu( tool );

		QAction * preference = new QAction( tr( "&Preference" ), this );
		tool->addAction( preference );
		connect( preference, SIGNAL( triggered() ), this->preference_, SLOT( show() ) );
		// tool menu done

		// setting help menu
		QMenu * help = new QMenu( tr( "&Help" ), menuBar );

		QAction * about = new QAction( tr( "&About Khopper" ), this );
		connect( about, SIGNAL( triggered() ), this->about_, SLOT( show() ) );
		help->addAction( about );

		QAction * aboutQt = new QAction( tr( "About &Qt" ), this );
		connect( aboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
		help->addAction( aboutQt );

		// add help menu to menu bar
		menuBar->addMenu( help );
		// help menu done

		setMenuBar( menuBar );
	}

	void MainWindow::initOptionWindow_() {
		this->optionWindow_->setWindowTitle( tr( "Output format setting" ) );
		this->optionWindow_->resize( 320, 240 );
		QVBoxLayout * mainBox = new QVBoxLayout( this->optionWindow_ );
		this->optionWindow_->setLayout( mainBox );

		mainBox->addWidget( this->optionTabs_ );
		// Take out the output types
		const EncoderList::ObjectType & tm = EncoderList::Instance();
		for( EncoderList::ObjectType::const_iterator it = tm.begin(); it != tm.end(); ++it ) {
			// it->first is object factory key, it->second is display name
			this->optionTabs_->addTab( UIFactory::Instance().CreateObject( it->first ), QString::fromStdString( it->second ) );
		}

		QDialogButtonBox * buttons = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this );
		mainBox->addWidget( buttons );
		connect( buttons, SIGNAL( accepted() ), this->optionWindow_, SLOT( accept() ) );
		connect( buttons, SIGNAL( rejected() ), this->optionWindow_, SLOT( reject() ) );
	}

	QString MainWindow::parseTemplate_() const {
		QString tmp = this->fileNameTemplate_->text();
		tmp.replace( "%t", "%1%" );
		tmp.replace( "%a", "%2%" );
		tmp.replace( QRegExp( "%(\\d*)i" ), "%|3$0\\1|" );
		tmp.replace( "%%", "%" );
		return tmp;
	}

	QString MainWindow::getOutDir_( TrackSP track ) const {
		if( this->useSourcePath_->isChecked() ) {
			return QFileInfo( QString::fromStdWString( track->filePath ) ).absolutePath();
		} else {
			return this->outputPath_->text();
		}
	}

	void MainWindow::fire_() {
		// get selected list
		std::vector< TrackSP > tracks = this->songList_->getSelectedTracks();
		if( tracks.empty() ) {
			this->showErrorMessage_( tr( "Run-time error!" ), tr( "No track selected." ) );
			return;
		}

		// get option widget
		if( this->optionWindow_->exec() ) {
			AbstractOption * option = qobject_cast< AbstractOption * >( this->optionTabs_->currentWidget() );
			if( !option ) {
				this->showErrorMessage_( tr( "Run-time error!" ), tr( "Bad output plugin" ) );
				return;
			}

			try {
				// create encoder object
				EncoderSP output( option->getEncoder() );

				// generate output paths
				QStringList outputPaths;
				QString tpl = this->parseTemplate_();
				for( std::size_t i = 0; i < tracks.size(); ++i ) {
					outputPaths.push_back( this->getOutDir_( tracks[i] ) + "/" + ::applyFormat( tpl, tracks[i] ) + "." + option->getSuffix() );
				}

				// set progress bar
				this->progress_->setTotal( tracks.size() );
				// set output information
				this->cvt_->setOutput( output, outputPaths );
				this->cvt_->setTracks( tracks );
				this->cvt_->start();
				this->progress_->exec();
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

			std::vector< TrackSP > tracks;

			if( fI.suffix() == "cue" ) {
				QFile fin( filePath );
				if( !fin.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
					this->showErrorMessage_( tr( "File open error!" ), tr( "Could not open the file: `" ) + filePath + "\'" );
				}

				QByteArray raw_input = fin.readAll();
				fin.close();

				this->codec_->setEncoded( raw_input );

				if( this->codec_->exec() ) {
					try {
						tracks = CUESheet( codec_->getDecoded().toStdWString(), fI.absolutePath().toStdWString() ).tracks;
					} catch( std::exception & e ) {
						this->showErrorMessage_( tr( "Error on parsing CUE Sheet!" ), tr( e.what() ) );
					}
				}
			} else {
				this->showErrorMessage_( tr( "Run-time error!" ), tr( "Not implemented." ) );
			}

			this->songList_->appendTracks( tracks );
		}
	}

	void MainWindow::initPreference_() {
		this->preference_->setWindowTitle( tr( "Preference" ) );

		QVBoxLayout * mainBox = new QVBoxLayout( this->preference_ );
		this->preference_->setLayout( mainBox );

		QGroupBox * fnGroup = new QGroupBox( tr( "File name template" ), this );
		mainBox->addWidget( fnGroup );

		QVBoxLayout * fnBox = new QVBoxLayout( fnGroup );
		fnGroup->setLayout( fnBox );

		// Output name template
		this->fileNameTemplate_->setToolTip( tr(
			"Keywords:\n"
			"%t: title\n"
			"%a: author\n"
			"%%: \'%\' literal\n"
			"Extensions will automaticaly append."
		) );
		fnBox->addWidget( this->fileNameTemplate_ );

		QDialogButtonBox * buttons = new QDialogButtonBox( QDialogButtonBox::Ok, Qt::Horizontal, this );
		mainBox->addWidget( buttons );
		connect( buttons, SIGNAL( accepted() ), this->preference_, SLOT( close() ) );
	}

	void MainWindow::initAbout_() {
		this->about_->setWindowTitle( tr( "About Khopper" ) );
		this->about_->resize( 320, 240 );

		QVBoxLayout * vbl = new QVBoxLayout( this->about_ );
		this->about_->setLayout( vbl );

		QTabWidget * tw = new QTabWidget( this->about_ );
		vbl->addWidget( tw );

		QLabel * about = new QLabel( tr(
			"An audio converter<br/>"
			"(C) 2008<br/>"
			"Present by Wei-Cheng Pan<br/>"
			"<a href=\"http://legnaleurc.blogspot.com/search/label/Khopper\">Home Page</a><br/>"
			"This program is a part of FoolproofProject<br/>"
			"E-Mail: <a href=\"mailto:legnaleurc@gmail.com\">legnaleurc@gmail.com</a><br/>"
		), tw );
		about->setTextFormat( Qt::RichText );
		tw->addTab( about, tr( "&About" ) );
	}

	void MainWindow::showErrorMessage_( const QString & title, const QString & msg ) {
		QMessageBox::critical( this, title, msg );
	}

}
