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
#include "ui_mainwindow.h"
#include "aboutwidget.hpp"
#include "codecselector.hpp"
#include "conversiondialog.hpp"
#include "cuesheet.hpp"
#include "player.hpp"
#include "preference.hpp"

#include "util/error.hpp"
#include "plugin/abstractpanel.hpp"
#include "plugin/pluginmanager.hpp"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtDebug>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

namespace khopper {

	namespace widget {

		MainWindow::MainWindow():
		QMainWindow( 0, 0 ),
		ui_( new Ui::MainWindow ),
		codec_( new CodecSelector( this ) ),
		conversion_( new ConversionDialog( this ) ),
		preference_( new Preference( this ) ),
		about_( new AboutWidget( this ) ),
		lastOpenedDir_( QDir::homePath() ) {
			this->ui_->setupUi( this );
			// Setting menu bar
			this->initMenuBar_();

			connect( this->ui_->player, SIGNAL( fileDropped( const QList< QUrl > & ) ), this, SLOT( open( const QList< QUrl > & ) ) );
			connect( this->ui_->player, SIGNAL( requireConvert( const album::TrackList & ) ), this, SLOT( fire_( const album::TrackList & ) ) );
			connect( this->ui_->player, SIGNAL( error( const QString &, const QString & ) ), this, SLOT( showErrorMessage_( const QString &, const QString & ) ) );

			connect( &plugin::PluginManager::Instance(), SIGNAL( panelAdded( khopper::plugin::AbstractPanel * ) ), this->conversion_, SLOT( addPanel( khopper::plugin::AbstractPanel * ) ) );
			connect( &plugin::PluginManager::Instance(), SIGNAL( panelRemoved( khopper::plugin::AbstractPanel * ) ), this->conversion_, SLOT( removePanel( khopper::plugin::AbstractPanel * ) ) );
		}

		MainWindow::~MainWindow() {
			delete this->ui_;
		}

		void MainWindow::reloadPlugins() {
			plugin::PluginManager::Instance().reloadPlugins();
		}

		void MainWindow::initMenuBar_() {
			connect( this->ui_->action_Open, SIGNAL( triggered() ), this, SLOT( showOpenFilesDialog() ) );
			connect( this->ui_->action_Preference, SIGNAL( triggered() ), this->preference_, SLOT( exec() ) );
			connect( this->ui_->actionAbout_Khopper, SIGNAL( triggered() ), this->about_, SLOT( show() ) );
			connect( this->ui_->actionAbout_Qt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
		}

		void MainWindow::fire_( const album::TrackList & tracks ) {
			if( tracks.empty() ) {
				this->showErrorMessage_( tr( "Run-time error!" ), tr( "No track selected." ) );
				return;
			}

			// get option widget
			this->conversion_->convert( tracks, this->preference_->getTemplate() );
		}

		void MainWindow::showOpenFilesDialog() {
			QStringList filePaths = QFileDialog::getOpenFileNames( this, tr( "Open audio" ), this->lastOpenedDir_ );
			if( filePaths.isEmpty() ) {
				return;
			}
			this->lastOpenedDir_ = QFileInfo( filePaths[0] ).absolutePath();

			QList< QUrl > tmp;
			foreach( QString path, filePaths ) {
				tmp.push_back( QUrl::fromLocalFile( path ) );
			}
			this->open( tmp );
		}

		void MainWindow::open( const QList< QUrl > & uris ) {
			if( uris.isEmpty() ) {
				return;
			}

			album::TrackList tracks;

			foreach( QUrl uri, uris ) {
				// TODO: handle remote resource
				if( uri.isEmpty() || uri.scheme() != "file" ) {
					continue;
				}

				QFileInfo fI( uri.toLocalFile() );

				if( fI.suffix() == "cue" ) {
					// if cue sheet opened
					QFile fin( fI.absoluteFilePath() );
					if( !fin.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
						this->showErrorMessage_(
							tr( "File open error!" ),
							tr( "Could not open the file: `%1\'" ).arg( fI.absoluteFilePath() )
						);
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
							int ret = QMessageBox::warning(
								this,
								tr( "Can not decode media" ),
								tr( "I can not open the media, please select another file." ),
								QMessageBox::Ok,
								QMessageBox::Cancel
							);
							while( ret == QMessageBox::Ok ) {
								QString filePath = QFileDialog::getOpenFileName(
									this,
									tr( "Open audio" ),
									this->lastOpenedDir_
								);
								if( filePath.isEmpty() ) {
									break;
								} else {
									this->lastOpenedDir_ = QFileInfo( filePath ).absolutePath();
								}
								try {
									qDebug() << filePath;
									sheet.setMedia( QUrl::fromLocalFile( filePath ) );
									break;
								} catch( std::exception & ) {
									ret = QMessageBox::warning(
										this,
										tr( "Can not decode media" ),
										tr( "I can not open the media, please select another file." ),
										QMessageBox::Ok,
										QMessageBox::Cancel
									);
								}
							}
						} catch( std::exception & e ) {
							this->showErrorMessage_( tr( "Unknow error" ), trUtf8( e.what() ) );
						}
						tracks.insert( tracks.end(), sheet.trackBegin(), sheet.trackEnd() );
					}
				} else {
					// other, single media
					album::TrackSP track( new album::Track );

					try {
						track->load( uri );
						tracks.push_back( track );
					} catch( std::exception & e ) {
						this->showErrorMessage_( tr( "Can not decode this file!" ), trUtf8( e.what() ) );
					}
				}
			}

			this->ui_->player->appendTracks( tracks );
		}

		void MainWindow::showErrorMessage_( const QString & title, const QString & msg ) {
			QMessageBox::critical( this, title, msg );
		}

	}

}
