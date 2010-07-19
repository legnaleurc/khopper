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
#include "progressbar.hpp"
#include "converter.hpp"

#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtGui/QVBoxLayout>

namespace {
	static QMutex mutex;
}

using namespace khopper::widget;
using khopper::album::PlayList;
using khopper::codec::WriterSP;

ProgressViewer::ProgressViewer( QWidget * parent ):
QWidget( parent, Qt::Window ),
rc_( 0 ),
lp_(),
tasks_() {
	this->setLayout( new QVBoxLayout );
	for( int i = 0; i < 2; ++i ) {
		this->lp_.append( new ProgressBar( this ) );
		this->layout()->addWidget( this->lp_.last() );
		QObject::connect( this->lp_.last(), SIGNAL( finished() ), this, SLOT( dispatch_() ) );
	}
}

void ProgressViewer::start( const QList< Converter * > & tasks ) {
	this->tasks_ = tasks;
	for( int i = 0; i < this->lp_.size(); ++i ) {
		this->lp_[i]->show();
		this->dispatch_( this->lp_[i] );
	}

	this->show();
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
		++this->rc_;
	} else if( this->rc_ > 1 ) {
		pb->hide();
		--this->rc_;
	} else {
		this->hide();
	}
}
