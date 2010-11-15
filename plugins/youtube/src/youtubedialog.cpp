/**
 * @file youtubedialog.cpp
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
#include "youtubedialog.hpp"
#include "ui_youtubedialog.h"

#include <QtGui/QDesktopServices>
#include <QtGui/QFileDialog>

#include <algorithm>

using namespace khopper::widget;

YouTubeDialog::YouTubeDialog():
QDialog( 0 ),
group_( new QButtonGroup( this ) ),
ui_( new Ui::YouTubeDialog ) {
	this->ui_->setupUi( this );

	this->group_->addButton( this->ui_->radioButton, 5 );
	this->group_->addButton( this->ui_->radioButton_2, 18 );
	this->group_->addButton( this->ui_->radioButton_3, 22 );
	this->group_->addButton( this->ui_->radioButton_4, 34 );
	this->group_->addButton( this->ui_->radioButton_5, 35 );
	this->group_->addButton( this->ui_->radioButton_6, 37 );
	this->group_->addButton( this->ui_->radioButton_7, 38 );
	this->group_->addButton( this->ui_->radioButton_8, 43 );
	this->group_->addButton( this->ui_->radioButton_9, 45 );

	this->clearFormat();

	this->ui_->lineEdit->setText( QDesktopServices::displayName( QDesktopServices::MoviesLocation ) );

	this->connect( this->ui_->pushButton, SIGNAL( clicked() ), SLOT( getLocation_() ) );
}

void YouTubeDialog::addFormat( const QString & format ) {
	this->group_->button( format.toInt() )->setEnabled( true );
}

void YouTubeDialog::clearFormat() {
	QList< QAbstractButton * > buttons( this->group_->buttons() );
	std::for_each( buttons.begin(), buttons.end(), []( QAbstractButton * button ) {
		button->setDisabled( true );
	} );
}

QString YouTubeDialog::getSelectedFormat() const {
	return QString::number( this->group_->checkedId() );
}

QString YouTubeDialog::getLocalLocation() const {
	return this->ui_->lineEdit->text();
}

void YouTubeDialog::getLocation_() {
	QString path = QFileDialog::getSaveFileName( 0, "Save file", this->ui_->lineEdit->text() );
	if( !path.isEmpty() ) {
		this->ui_->lineEdit->setText( path );
	}
}
