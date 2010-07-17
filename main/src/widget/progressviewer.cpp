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

#include <QtCore/QTimer>
#include <QtGui/QVBoxLayout>

using namespace khopper::widget;
using khopper::album::PlayList;
using khopper::codec::WriterSP;

ProgressViewer::ProgressViewer( QWidget * parent ):
QWidget( parent, Qt::Window ),
lp_(),
queue_() {
	this->setLayout( new QVBoxLayout );
	for( int i = 0; i < 2; ++i ) {
		this->lp_.append( new ProgressBar( this ) );
		this->layout()->addWidget( this->lp_.last() );
	}
}

void ProgressViewer::start( const PlayList & tracks, const QList< WriterSP > & encoders ) {
	for( int i = 0; i < tracks.size(); ++i ) {
		this->queue_.enqueue( new Converter( tracks[i], encoders[i], this ) );
	}
	for( int i = 0; i < this->lp_.size(); ++i ) {
		QTimer::singleShot( 0, this->lp_[i], SLOT( start() ) );
	}

	this->show();
}

Converter * ProgressViewer::take() {
	if( !this->queue_.isEmpty() ) {
		return this->queue_.dequeue();
	} else {
		return NULL;
	}
}
