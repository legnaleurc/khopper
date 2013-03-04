/**
 * @file progressviewer.cpp
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
#include "progressviewer.hpp"

#include <algorithm>

#include <QtCore/QMutexLocker>
#include <QtCore/QThreadPool>

#include "ui_progressviewer.h"
#include "progressbar.hpp"
#include "converter.hpp"

namespace {
	static QMutex mutex;
}

using khopper::album::PlayList;
using khopper::codec::WriterSP;
using khopper::utility::Converter;
using khopper::widget::ProgressViewer;
using khopper::widget::ProgressBar;

ProgressViewer::ProgressViewer( QWidget * parent ):
QWidget( parent, Qt::Window ),
rc_( 0 ),
lp_(),
tasks_(),
ui_( new Ui::ProgressViewer ) {
	this->ui_->setupUi( this );
	QObject::connect( this->ui_->cancel, SIGNAL( clicked() ), this, SLOT( cancel_() ) );
	const auto MAX_TASKS = QThreadPool::globalInstance()->maxThreadCount();

	for( int i = 0; i < MAX_TASKS; ++i ) {
		this->lp_.append( new ProgressBar( this ) );
		this->ui_->layout->addWidget( this->lp_.last() );
		QObject::connect( this->lp_.last(), SIGNAL( finished() ), this, SLOT( dispatch_() ) );
	}
}

void ProgressViewer::start( const QList< Converter * > & tasks ) {
	QMutexLocker locker( &mutex );
	this->tasks_.append( tasks );
	locker.unlock();
	std::for_each( this->lp_.begin(), this->lp_.end(), [&]( ProgressBar * pb ) {
		if( !pb->isVisible() ) {
			pb->show();
			++this->rc_;
			this->dispatch_( pb );
		}
	} );

	this->show();
}

void ProgressViewer::cancel_() {
	QMutexLocker locker( &mutex );

	std::for_each( this->tasks_.begin(), this->tasks_.end(), []( Converter * task ) {
		delete task;
	} );
	this->tasks_.clear();

	std::for_each( this->lp_.begin(), this->lp_.end(), []( ProgressBar * pb ) {
		pb->cancel();
	} );
}

void ProgressViewer::dispatch_() {
	ProgressBar * pb = dynamic_cast< ProgressBar * >( this->sender() );
	if( pb ) {
		this->dispatch_( pb );
	}
}

void ProgressViewer::dispatch_( ProgressBar * pb ) {
	Converter * task = NULL;

	QMutexLocker locker( &mutex );
	if( !this->tasks_.isEmpty() ) {
		task = this->tasks_.front();
		this->tasks_.pop_front();
	}

	if( task ) {
		pb->start( task );
	} else if( this->rc_ > 1 ) {
		pb->hide();
		--this->rc_;
	} else {
		this->hide();
		this->rc_ = 0;
	}
}
