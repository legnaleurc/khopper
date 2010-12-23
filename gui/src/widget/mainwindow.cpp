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
#include "conversiondialog.hpp"
#include "player.hpp"
#include "pluginviewer.hpp"
#include "preference.hpp"
#include "uriinputdialog.hpp"

#include "khopper/application.hpp"
#include "khopper/error.hpp"
#include "khopper/playlist.hpp"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtGui/QDesktopServices>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>

#include <algorithm>

using namespace khopper::widget;
using khopper::album::createPlayList;
using khopper::album::PlayList;
using khopper::error::BaseError;

MainWindow::MainWindow():
QMainWindow( 0, 0 ),
about_( new AboutWidget( this ) ),
conversion_( new ConversionDialog( this ) ),
lastOpenedDir_( QDesktopServices::storageLocation( QDesktopServices::MusicLocation ) ),
plugins_( new PluginViewer( this ) ),
preference_( new Preference( this ) ),
ui_( new Ui::MainWindow ),
uriInput_( new UriInputDialog( this ) ) {
	this->ui_->setupUi( this );
	// Setting menu bar
	this->initMenuBar_();

	QObject::connect( khopper::pApp(), SIGNAL( errorOccured( const QString &, const QString & ) ), this, SLOT( showErrorMessage_( const QString &, const QString & ) ) );

	QObject::connect( this->ui_->player, SIGNAL( fileDropped( const QList< QUrl > & ) ), this, SLOT( open( const QList< QUrl > & ) ) );
	QObject::connect( this->ui_->player, SIGNAL( requireConvert( const khopper::album::PlayList & ) ), this, SLOT( fire_( const khopper::album::PlayList & ) ) );
	QObject::connect( this->ui_->player, SIGNAL( errorOccured( const QString &, const QString & ) ), this, SLOT( showErrorMessage_( const QString &, const QString & ) ) );

	QObject::connect( this->conversion_, SIGNAL( errorOccured( const QString &, const QString & ) ), this, SLOT( showErrorMessage_( const QString &, const QString & ) ) );

	if( !this->ui_->player->isPlayable() ) {
		this->showErrorMessage_( tr( "Backend Capability Error" ), tr( "Phonon Backend is down." ) );;
	}
}

void MainWindow::initMenuBar_() {
	QObject::connect( this->ui_->action_Open, SIGNAL( triggered() ), this, SLOT( showOpenFilesDialog() ) );
	QObject::connect( this->ui_->actionPlugins_Manager, SIGNAL( triggered() ), this->plugins_, SLOT( show() ) );
	QObject::connect( this->ui_->action_Load_By_Url, SIGNAL( triggered() ), this, SLOT( showUriInputDialog() ) );
	QObject::connect( this->ui_->action_Preference, SIGNAL( triggered() ), this->preference_, SLOT( exec() ) );
	QObject::connect( this->ui_->actionAbout_Khopper, SIGNAL( triggered() ), this->about_, SLOT( show() ) );
	QObject::connect( this->ui_->actionAbout_Qt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
}

void MainWindow::fire_( const PlayList & tracks ) {
	if( tracks.empty() ) {
		this->showErrorMessage_( tr( "Run-time error!" ), tr( "No track selected." ) );
		return;
	}

	// get option widget
	this->conversion_->convert( tracks );
}

void MainWindow::showOpenFilesDialog() {
	QStringList filePaths = QFileDialog::getOpenFileNames( this, tr( "Open audio" ), this->lastOpenedDir_ );
	if( filePaths.isEmpty() ) {
		return;
	}
	this->lastOpenedDir_ = QFileInfo( filePaths[0] ).absolutePath();

	QList< QUrl > tmp;
	std::for_each( filePaths.begin(), filePaths.end(), [&tmp]( const QString & path ) {
		tmp.push_back( QUrl::fromLocalFile( path ) );
	} );
	this->open( tmp );
}

void MainWindow::open( const QList< QUrl > & uris ) {
	if( uris.isEmpty() ) {
		return;
	}

	PlayList tracks;

	std::for_each( uris.begin(), uris.end(), [&]( const QUrl & uri ) {
		if( uri.isEmpty() ) {
			return;
		}

		try {
			tracks.append( createPlayList( uri ) );
		} catch( BaseError & e ) {
			this->showErrorMessage_( tr( "Can not decode this file!" ), e.getMessage() );
		}
	} );

	this->ui_->player->append( tracks );
}

void MainWindow::showErrorMessage_( const QString & title, const QString & msg ) {
	QMessageBox::critical( this, title, msg );
}

void MainWindow::showUriInputDialog() {
	if( this->uriInput_->exec() != QDialog::Accepted ) {
		return;
	}
	this->open( this->uriInput_->getUriList() );
}
