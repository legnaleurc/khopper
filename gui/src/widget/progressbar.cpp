/**
 * @file progressbar.cpp
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
#include "progressbar.hpp"

#include <QtCore/QThreadPool>

#include "ui_progressbar.h"
#include "progressviewer.hpp"
#include "converter.hpp"

using khopper::widget::ProgressBar;
using khopper::utility::Converter;

ProgressBar::ProgressBar( QWidget * parent ):
QWidget( parent ),
task_( NULL ),
ui_( new Ui::ProgressBar ) {
	this->ui_->setupUi( this );
}

void ProgressBar::cancel() {
	if( this->task_ ) {
		this->task_->cancel();
	}
}

void ProgressBar::start( Converter * task ) {
	this->task_ = task;

	task->connect( this->ui_->pushButton, SIGNAL( clicked() ), SLOT( cancel() ) );
	this->connect( task, SIGNAL( decodedTime( qint64 ) ), SLOT( increase_( qint64 ) ) );

	this->ui_->label->setText( task->getTitle() );
	this->ui_->progressBar->setRange( 0, task->getMaximumValue() );
	this->ui_->progressBar->setValue( 0 );

	QThreadPool::globalInstance()->start( this->task_ );
	this->connect( task, SIGNAL( finished() ), SIGNAL( finished() ) );
}

void ProgressBar::increase_( qint64 value ) {
	this->ui_->progressBar->setValue( this->ui_->progressBar->value() + value );
}
