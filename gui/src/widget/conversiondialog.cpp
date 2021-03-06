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
#include "progressviewer.hpp"
#include "converter.hpp"
#include "pathcompletermodel.hpp"

#include "khopper/panel.hpp"
#include "khopper/writerpanelcontext.hpp"

#include <QtCore/QSettings>
#include <QtGui/QCompleter>
#include <QtGui/QDesktopServices>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include <algorithm>
#include <set>

using khopper::album::TrackCSP;
using khopper::album::TrackSP;
using khopper::album::PlayList;
using khopper::codec::Writer;
using khopper::codec::WriterSP;
using khopper::utility::Converter;
using khopper::utility::PathCompleterModel;
using khopper::widget::Panel;
using khopper::widget::ConversionDialog;
using khopper::widget::WriterPanelContext;

ConversionDialog::ConversionDialog( QWidget * parent ):
QDialog( parent ),
ui_( new Ui::ConversionDialog ),
progress_( new ProgressViewer( this ) ),
table_() {
	this->ui_->setupUi( this );
	this->ui_->outputPath->setText( QDesktopServices::storageLocation( QDesktopServices::MusicLocation ) );

	QSettings setting;
	// Output name template
	setting.beginGroup( "preference" );
	this->ui_->tpl->setText( setting.value( "filename", "%2i_%t" ).toString() );
	setting.endGroup();

	this->connect( this->ui_->browse, SIGNAL( clicked() ), SLOT( changeOutputPath_() ) );

	this->connect( &WriterPanelContext::instance(), SIGNAL( installed( khopper::widget::Panel * ) ), SLOT( addPanel( khopper::widget::Panel * ) ) );
	this->connect( &WriterPanelContext::instance(), SIGNAL( uninstalled( khopper::widget::Panel * ) ), SLOT( removePanel( khopper::widget::Panel * ) ) );

	QCompleter * completer = new QCompleter( this );
	PathCompleterModel * model = new PathCompleterModel( this );
	model->setRootPath( this->ui_->outputPath->text() );
	completer->setModel( model );
	this->ui_->outputPath->setCompleter( completer );
}

void ConversionDialog::convert( const PlayList & tracks ) {
	int result = this->exec();
	if( result == QDialog::Rejected ) {
		return;
	}

	Panel * panel = this->getCurrent();
	Writer::Creator wc( panel->getWriterCreator() );
	std::set< QString > failureDirs;

	QList< Converter * > tasks;
	std::for_each( tracks.begin(), tracks.end(), [&]( album::TrackSP track ) {
		QDir outDir( this->getOutputDir_( track ) );
		if( !outDir.exists() ) {
			failureDirs.insert( outDir.path() );
			return;
		}
		QString path = QString( "%1/%2.%3" ).arg( outDir.path() ).arg( this->getOutputName_( track ) ).arg( panel->getSuffix() );
		Converter * task = new Converter( track, wc, path );
		QObject::connect( task, SIGNAL( errorOccured( const QString &, const QString & ) ), this, SIGNAL( errorOccured( const QString &, const QString & ) ) );
		tasks.push_back( task );
	} );

	if( !failureDirs.empty() ) {
		QString allPath;
		std::for_each( failureDirs.begin(), failureDirs.end(), [&]( const QString & path ) {
			allPath += path + "\n";
		} );
		QMessageBox::warning( this, QObject::tr( "Directory not exist" ), QObject::tr( "The following directories are not exist:\n%1" ).arg( allPath ) );
	}

	if( !tasks.isEmpty() ) {
		this->progress_->start( tasks );
	}
}

Panel * ConversionDialog::getCurrent() const {
	return this->table_.find( this->ui_->tabWidget->currentIndex() )->second;
}

void ConversionDialog::addPanel( Panel * panel ) {
	this->table_.insert( std::make_pair( this->ui_->tabWidget->addTab( panel, panel->getTitle() ), panel ) );
}

void ConversionDialog::removePanel( Panel * panel ) {
	int index = this->ui_->tabWidget->indexOf( panel );
	this->table_.erase( index );
	this->ui_->tabWidget->removeTab( index );
}

QDir ConversionDialog::getOutputDir_( TrackSP track ) {
	if( this->ui_->useSource->isChecked() && track->getURI().scheme() == "file" ) {
		return QFileInfo( track->getURI().toLocalFile() ).dir();
	}
	return QDir( this->ui_->outputPath->text() );
}

QString ConversionDialog::getOutputName_( TrackSP track ) {
	QString tmp = this->ui_->tpl->text();
	tmp.replace( "%t", track->getTitle() );
	tmp.replace( "%a", track->getArtist() );
	QRegExp pattern( "%(\\d*)i" );
	int offset = 0;
	while( ( offset = pattern.indexIn( tmp, offset ) ) >= 0 ) {
		QString newString( QString( "%1" ).arg( QString::number( track->getIndex() ), pattern.cap( 1 ).toInt(), '0' ) );
		tmp.replace( offset, pattern.matchedLength(), newString );
		offset += newString.length() - pattern.matchedLength();
	}
	tmp.replace( "%%", "%" );
	tmp.replace( "/", "_" );
	tmp.replace( "\\", "_" );
	return tmp;
}

void ConversionDialog::changeOutputPath_() {
	QString outDir = QFileDialog::getExistingDirectory( this, tr( "Target Directory" ), QDir::homePath() );
	if( !outDir.isEmpty() ) {
		this->ui_->outputPath->setText( outDir );
	}
}
