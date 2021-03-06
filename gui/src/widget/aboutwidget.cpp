/**
 * @file aboutwidget.cpp
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
#include "aboutwidget.hpp"
#include "ui_aboutwidget.h"

using namespace khopper::widget;

AboutWidget::AboutWidget( QWidget * parent ):
QWidget( parent, Qt::Dialog ),
ui_( new Ui::AboutWidget ){
	this->ui_->setupUi( this );

	this->ui_->logo->setPixmap( QPixmap( ":/share/pixmap/logo.png" ).scaled( 60, 60 ) );
	this->ui_->version->setText( tr(
		"<h1>%1</h1>"
		"Version %2<br/>"
		"Part of %3<br/>"
	).arg( qApp->applicationName() ).arg( qApp->applicationVersion() ).arg( qApp->organizationName() ) );
}
