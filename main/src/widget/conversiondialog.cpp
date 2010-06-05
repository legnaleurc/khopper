/**
 * @file conversiondialog.cpp
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
#include "conversiondialog.hpp"
#include "ui_conversiondialog.h"
#include "progress.hpp"
#include "converterthread.hpp"

#include "khopper/abstractpanel.hpp"
#include "khopper/application.hpp"

#include <QtCore/QSettings>
#include <QtDebug>
#include <QtGui/QFileDialog>

#include <boost/format.hpp>

using namespace khopper::widget;
using khopper::album::TrackCSP;
using khopper::album::TrackSP;
using khopper::album::PlayList;
using khopper::codec::WriterSP;

ConversionDialog::ConversionDialog( QWidget * parent ):
QDialog( parent ),
ui_( new Ui::ConversionDialog ),
progress_( new Progress( this ) ),
thread_( new ConverterThread( this ) ),
table_() {
	this->ui_->setupUi( this );
	this->ui_->outputPath->setText( QDir::homePath() );

	QSettings setting;
	// Output name template
	setting.beginGroup( "preference" );
	this->ui_->tpl->setText( setting.value( "filename", "%2i_%t" ).toString() );
	setting.endGroup();

	connect( this->ui_->browse, SIGNAL( clicked() ), this, SLOT( changeOutputPath_() ) );

	connect( KHOPPER_APPLICATION, SIGNAL( panelAdded( khopper::widget::AbstractPanel * ) ), this, SLOT( addPanel( khopper::widget::AbstractPanel * ) ) );
	connect( KHOPPER_APPLICATION, SIGNAL( panelRemoved( khopper::widget::AbstractPanel * ) ), this, SLOT( removePanel( khopper::widget::AbstractPanel * ) ) );

	// Converter thread
	connect( this->thread_, SIGNAL( taskName( const QString & ) ), this->progress_, SLOT( setItemName( const QString & ) ) );
	connect( this->thread_, SIGNAL( taskGoal( qint64 ) ), this->progress_, SLOT( setMaximum( qint64 ) ) );
	connect( this->thread_, SIGNAL( currentTask( int ) ), this->progress_, SLOT( setCurrent( int ) ) );
	connect( this->thread_, SIGNAL( step( qint64 ) ), this->progress_, SLOT( increase( qint64 ) ) );
	connect( this->thread_, SIGNAL( finished() ), this->progress_, SLOT( hide() ) );
	connect( this->thread_, SIGNAL( errorOccured( const QString &, const QString & ) ), this, SIGNAL( errorOccured( const QString &, const QString & ) ) );
	// NOTE: works, but danger
	connect( this->progress_, SIGNAL( canceled() ), this->thread_, SLOT( cancel() ) );
}

void ConversionDialog::convert( const PlayList & tracks ) {
	int result = this->exec();
	if( result == QDialog::Rejected ) {
		return;
	}

	AbstractPanel * panel = this->getCurrent();

	QList< WriterSP > outs;
	foreach( album::TrackSP track, tracks ) {
		outs.push_back( panel->createWriter( QUrl::fromLocalFile( QString( "%1/%2.%3" ).arg( this->getOutputPath_( track ) ).arg( this->getOutputName_( track ) ).arg( panel->getSuffix() ) ) ) );
	}

	// set progress bar
	this->progress_->setTotal( tracks.size() );
	// set output information
	this->thread_->setOutput( outs );
	this->thread_->setTracks( tracks );
	this->thread_->start();
	this->progress_->show();
}

AbstractPanel * ConversionDialog::getCurrent() const {
	return this->table_.find( this->ui_->tabWidget->currentIndex() )->second;
}

void ConversionDialog::addPanel( AbstractPanel * panel ) {
	this->table_.insert( std::make_pair( this->ui_->tabWidget->addTab( panel, panel->getTitle() ), panel ) );
}

void ConversionDialog::removePanel( AbstractPanel * panel ) {
	int index = this->ui_->tabWidget->indexOf( panel );
	this->table_.erase( index );
	this->ui_->tabWidget->removeTab( index );
}

QString ConversionDialog::getOutputPath_( TrackSP track ) {
	if( this->ui_->useSource->isChecked() && track->getURI().scheme() == "file" ) {
		return QFileInfo( track->getURI().toLocalFile() ).path();
	}
	return this->ui_->outputPath->text();
}

QString ConversionDialog::getOutputName_( TrackSP track ) {
	QString tmp = this->ui_->tpl->text();
	tmp.replace( "%t", "%1%" );
	tmp.replace( "%a", "%2%" );
	tmp.replace( QRegExp( "%(\\d*)i" ), "%|3$0\\1|" );
	tmp.replace( "%%", "%" );
	return QString::fromStdString( ( boost::format( tmp.toStdString() ) % track->getTitle().toStdString() % track->getArtist().toStdString() % track->getIndex() ).str() );
}

void ConversionDialog::changeOutputPath_() {
	QString outDir = QFileDialog::getExistingDirectory( this, tr( "Target Directory" ), QDir::homePath() );
	if( !outDir.isEmpty() ) {
		this->ui_->outputPath->setText( outDir );
	}
}
