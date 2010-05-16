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
#include "plugin/pluginmanager.hpp"
#include "progress.hpp"
#include "converterthread.hpp"

#include <QtDebug>
#include <QtGui/QFileDialog>

namespace {
	inline QString applyFormat( boost::format tpl, khopper::album::TrackCSP track ) {
		return QString::fromUtf8( ( tpl % track->get( "title" ).toString().toUtf8().constData() % track->get( "artist" ).toString().toUtf8().constData() % track->get( "index" ).toInt() ).str().c_str() );
	}
}

namespace khopper {

	namespace widget {

		ConversionDialog::ConversionDialog( QWidget * parent ):
		QDialog( parent ),
		ui_( new Ui::ConversionDialog ),
		progress_( new Progress( this ) ),
		thread_( new ConverterThread( this ) ),
		table_() {
			this->ui_->setupUi( this );
			this->ui_->outputPath->setText( QDir::homePath() );

			connect( this->ui_->browse, SIGNAL( clicked() ), this, SLOT( changeOutputPath_() ) );

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

		ConversionDialog::~ConversionDialog() {
			delete this->ui_;
		}

		void ConversionDialog::convert( const album::TrackList & tracks, const boost::format & tpl ) {
			int result = this->exec();
			if( result == QDialog::Rejected ) {
				return;
			}

			plugin::AbstractPanel * panel = this->getCurrent();
			codec::WriterSP encoder( panel->getWriter() );
			if( encoder == NULL ) {
				this->errorOccured( tr( "Plugin Error" ), tr( "Can\'t load codec." ) );
				return;
			}

			QList< QString > outputPaths;
			foreach( album::TrackSP track, tracks ) {
				outputPaths.push_back( QString( "%1/%2.%3" ).arg( this->getOutputPath_( track ) ).arg( applyFormat( tpl, track ) ).arg( panel->getSuffix() ) );
			}

			// set progress bar
			this->progress_->setTotal( tracks.size() );
			// set output information
			this->thread_->setOutput( encoder, outputPaths );
			this->thread_->setTracks( tracks );
			this->thread_->start();
			this->progress_->show();
		}

		plugin::AbstractPanel * ConversionDialog::getCurrent() const {
			return this->table_.find( this->ui_->tabWidget->currentIndex() )->second;
		}

		void ConversionDialog::addPanel( plugin::AbstractPanel * panel ) {
			this->table_.insert( std::make_pair( this->ui_->tabWidget->addTab( panel, panel->getTitle() ), panel ) );
		}

		void ConversionDialog::removePanel( plugin::AbstractPanel * panel ) {
			int index = this->ui_->tabWidget->indexOf( panel );
			this->table_.erase( index );
			this->ui_->tabWidget->removeTab( index );
		}

		QString ConversionDialog::getOutputPath_( album::TrackSP track ) {
			if( this->ui_->useSource->isChecked() && track->getURI().scheme() == "file" ) {
				return track->getURI().toLocalFile();
			}
			return this->ui_->outputPath->text();
		}

		void ConversionDialog::changeOutputPath_() {
			QString outDir = QFileDialog::getExistingDirectory( this, tr( "Target Directory" ), QDir::homePath() );
			if( !outDir.isEmpty() ) {
				this->ui_->outputPath->setText( outDir );
			}
		}

	}

}
