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
#include "preference.hpp"

#include "khopper/error.hpp"
#include "khopper/playlist.hpp"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtDebug>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

using namespace khopper::widget;
using khopper::album::PlayList;
using khopper::error::BaseError;

MainWindow::MainWindow():
QMainWindow( 0, 0 ),
ui_( new Ui::MainWindow ),
conversion_( new ConversionDialog( this ) ),
preference_( new Preference( this ) ),
about_( new AboutWidget( this ) ),
lastOpenedDir_( QDir::homePath() ) {
	this->ui_->setupUi( this );
	// Setting menu bar
	this->initMenuBar_();

	QObject::connect( this->ui_->player, SIGNAL( fileDropped( const QList< QUrl > & ) ), this, SLOT( open( const QList< QUrl > & ) ) );
	QObject::connect( this->ui_->player, SIGNAL( requireConvert( const album::TrackList & ) ), this, SLOT( fire_( const album::TrackList & ) ) );
	QObject::connect( this->ui_->player, SIGNAL( error( const QString &, const QString & ) ), this, SLOT( showErrorMessage_( const QString &, const QString & ) ) );

	QObject::connect( this->conversion_, SIGNAL( errorOccured( const QString &, const QString & ) ), this, SLOT( showErrorMessage_( const QString &, const QString & ) ) );
}

void MainWindow::initMenuBar_() {
	QObject::connect( this->ui_->action_Open, SIGNAL( triggered() ), this, SLOT( showOpenFilesDialog() ) );
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

	PlayList tracks;

	foreach( QUrl uri, uris ) {
		if( uri.isEmpty() ) {
			continue;
		}

		try {
			tracks.append( PlayList::loadFromUri( uri ) );
		} catch( BaseError & e ) {
			this->showErrorMessage_( tr( "Can not decode this file!" ), e.getMessage() );
		}
	}

	this->ui_->player->append( tracks );
}

void MainWindow::showErrorMessage_( const QString & title, const QString & msg ) {
	QMessageBox::critical( this, title, msg );
}
