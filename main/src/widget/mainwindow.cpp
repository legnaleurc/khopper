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
#include "player.hpp"
#include "threads.hpp"
#include "textcodec.hpp"
#include "progress.hpp"
#include "cuesheet.hpp"
#include "preference.hpp"
#include "outputoption.hpp"

#include "util/error.hpp"
#include "plugin/abstractpanel.hpp"

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
#include <QtDebug>

namespace {

	inline QString applyFormat( boost::format tpl, khopper::album::TrackCSP track ) {
		return QString::fromUtf8( ( tpl % track->getTitle().toUtf8().constData() % track->getArtist().toUtf8().constData() % track->getIndex() ).str().c_str() );
	}

}

namespace khopper {

	namespace widget {

		MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags flags ):
		QMainWindow( parent, flags ),
		codec_( new TextCodec( this ) ),
		player_( new Player( this ) ),
		optionWindow_( new OutputOption( this ) ),
		outputPath_( new QLineEdit( QDir::homePath(), this ) ),
		useSourcePath_( new QCheckBox( tr( "Same as source directories" ), this ) ),
		progress_( new Progress( this ) ),
		cvt_( new ConverterThread( this ) ),
		preference_( new Preference( this ) ),
		about_( new QWidget( this, Qt::Dialog ) ) {
			// Setting menu bar
			this->initMenuBar_();
			// Setting about widget
			this->initAbout_();

			// Setting central widget
			QWidget * central = new QWidget( this );
			QVBoxLayout * mainBox = new QVBoxLayout( central );
			central->setLayout( mainBox );
			this->setCentralWidget( central );

			// Add song list
			mainBox->addWidget( this->player_ );
			connect( this->player_, SIGNAL( fileDropped( const QStringList & ) ), this, SLOT( open( const QStringList & ) ) );
			connect( this->player_, SIGNAL( requireConvert() ), this, SLOT( fire_() ) );
			connect( this->player_, SIGNAL( error( const QString &, const QString & ) ), this, SLOT( showErrorMessage_( const QString &, const QString & ) ) );

			QHBoxLayout * pathBox = new QHBoxLayout;
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

			QAction * open = new QAction( tr( "&Open files" ), this );
			open->setShortcut( tr( "Ctrl+O" ) );
			connect( open, SIGNAL( triggered() ), this, SLOT( showOpenFilesDialog() ) );
			file->addAction( open );

			// add file menu to menu bar
			menuBar->addMenu( file );
			// file menu done

			// setting tool menu
			QMenu * tool = new QMenu( tr( "&Tools" ), menuBar );
			menuBar->addMenu( tool );

			QAction * preference = new QAction( tr( "&Preference" ), this );
			tool->addAction( preference );
			connect( preference, SIGNAL( triggered() ), this->preference_, SLOT( exec() ) );
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

		QString MainWindow::getOutDir_( album::TrackSP track ) const {
			if( this->useSourcePath_->isChecked() ) {
				return QFileInfo( track->getFilePath() ).absolutePath();
			} else {
				return this->outputPath_->text();
			}
		}

		void MainWindow::fire_() {
			// get selected list
			std::vector< album::TrackSP > tracks = this->player_->getSelectedTracks();
			if( tracks.empty() ) {
				this->showErrorMessage_( tr( "Run-time error!" ), tr( "No track selected." ) );
				return;
			}

			// get option widget
			if( this->optionWindow_->exec() ) {
				plugin::AbstractPanel * option = this->optionWindow_->getCurrent();

				try {
					// generate output paths
					QStringList outputPaths;
					for( std::size_t i = 0; i < tracks.size(); ++i ) {
						outputPaths.push_back( this->getOutDir_( tracks[i] ) + "/" + ::applyFormat( this->preference_->getTemplate(), tracks[i] ) + "." + option->getSuffix() );
					}

					// set progress bar
					this->progress_->setTotal( tracks.size() );
					// set output information
					this->cvt_->setOutput( option->getWriter(), outputPaths );
					this->cvt_->setTracks( tracks );
					this->cvt_->start();
					this->progress_->exec();
				} catch( error::RunTimeError & e ) {
					this->showErrorMessage_( tr( "Run-time error!" ), e.getMessage() );
				} catch( std::exception & e ) {
					this->showErrorMessage_( tr( "Unknown error!" ), trUtf8( e.what() ) );
				}
			}
		}

		void MainWindow::showOpenFilesDialog() {
			QStringList filePaths = QFileDialog::getOpenFileNames( this, tr( "Open audio" ), QDir::homePath() );
			this->open( filePaths );
		}

		void MainWindow::open( const QStringList & filePaths ) {
			std::vector< album::TrackSP > tracks;

			foreach( QString filePath, filePaths ) {
				if( filePath != "" ) {
					QFileInfo fI( filePath );

					if( fI.suffix() == "cue" ) {
						// if cue sheet opened
						QFile fin( filePath );
						if( !fin.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
							this->showErrorMessage_( tr( "File open error!" ), tr( "Could not open the file: `" ) + filePath + "\'" );
						}

						QByteArray raw_input = fin.readAll();
						fin.close();

						this->codec_->setEncoded( raw_input );

						if( this->codec_->exec() ) {
							album::CUESheet sheet;
							try {
								sheet.open( this->codec_->getDecoded(), fI.absolutePath() );
							} catch( error::ParsingError & e ) {
								this->showErrorMessage_( tr( "Error on parsing CUE Sheet!" ), trUtf8( e.what() ) );
							} catch( error::BaseError & ) {
								int ret = QMessageBox::warning( this, tr( "Can not decode media" ), tr( "I can not open the media, please select another file." ), QMessageBox::Ok, QMessageBox::Cancel );
								while( ret == QMessageBox::Ok ) {
									filePath = QFileDialog::getOpenFileName( this, tr( "Open audio" ), QDir::homePath() );
									try {
										sheet.setMedia( filePath );
										break;
									} catch( std::exception & ) {
										ret = QMessageBox::warning( this, tr( "Can not decode media" ), tr( "I can not open the media, please select another file." ), QMessageBox::Ok, QMessageBox::Cancel );
									}
								}
							} catch( std::exception & e ) {
								this->showErrorMessage_( tr( "Unknow error" ), trUtf8( e.what() ) );
							}
							tracks.insert( tracks.end(), sheet.getTracks().begin(), sheet.getTracks().end() );
						}
					} else {
						// other, single media
						album::TrackSP track( new album::Track );

						try {
							track->load( filePath );
							tracks.push_back( track );
						} catch( std::exception & e ) {
							this->showErrorMessage_( tr( "Can not decode this file!" ), trUtf8( e.what() ) );
						}
					}
				}
			}

			this->player_->appendTracks( tracks );
		}

		void MainWindow::initAbout_() {
			this->about_->setWindowTitle( tr( "About Khopper" ) );

			QVBoxLayout * mainBox = new QVBoxLayout( this->about_ );
			this->about_->setLayout( mainBox );

			QHBoxLayout * topBox = new QHBoxLayout;
			mainBox->addLayout( topBox );

			QLabel * logo = new QLabel( this->about_ );
			logo->setPixmap( QPixmap( ":/image/logo.svg" ).scaled( 60, 60 ) );
			topBox->addWidget( logo );

			QLabel * version = new QLabel( this->about_ );
			version->setText( tr(
				"<h1>%1</h1>"
				"Version %2<br/>"
				"Part of %3<br/>"
			).arg( qApp->applicationName() ).arg( qApp->applicationVersion() ).arg( qApp->organizationName() ) );
			topBox->addWidget( version );

			QTabWidget * tw = new QTabWidget( this->about_ );
			mainBox->addWidget( tw );

			QLabel * about = new QLabel( tr(
				"An audio converter<br/>"
				"<br/>"
				"(C) 2008<br/>"
				"<br/>"
				"Present by Wei-Cheng Pan<br/>"
				"<a href=\"http://legnaleurc.blogspot.com/search/label/Khopper\">Home Page</a><br/>"
			), tw );
			about->setTextFormat( Qt::RichText );
			tw->addTab( about, tr( "&About" ) );

			QLabel * authors = new QLabel( this->about_ );
			authors->setText( tr(
				"Wei-Cheng Pan<br/>"
				"&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"http://legnaleurc.blogspot.com\">http://legnaleurc.blogspot.com</a><br/>"
				"&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"mailto:legnaleurc@gmail.com\">legnaleurc@gmail.com</a><br/>"
				"<br/>"
				"Meng-Lin Jhuang<br/>"
				"<br/>"
				"Jheng-Syuan Lin<br/>"
				"<br/>"
				"Zih-Jie Jhou<br/>"
				"<br/>"
				"Wei-Ru Zeng<br/>"
				"<br/>"
			) );
			authors->setTextFormat( Qt::RichText );
			tw->addTab( authors, tr( "A&uthors" ) );

			QLabel * womm = new QLabel( this->about_ );
			womm->setPixmap( QPixmap( ":/image/womm.png" ) );
			womm->setAlignment( Qt::AlignCenter );
			tw->addTab( womm, tr( "&Certification" ) );
		}

		void MainWindow::showErrorMessage_( const QString & title, const QString & msg ) {
			QMessageBox::critical( this, title, msg );
		}

	}

}
