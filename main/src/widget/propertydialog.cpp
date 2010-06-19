/**
 * @file propertydialog.cpp
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
#include "propertydialog.hpp"
#include "ui_propertydialog.h"

using namespace khopper::widget;
using khopper::album::TrackSP;

PropertyDialog::PropertyDialog( QWidget * parent ):
QDialog( parent ),
ui_( new Ui::PropertyDialog ) {
	this->ui_->setupUi( this );

	// setup dialog button box
	QObject::connect( this->ui_->buttonBox, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( perform_( QAbstractButton * ) ) );
}

int PropertyDialog::exec( TrackSP track ) {
	this->ui_->title->setText( track->getTitle() );
	this->ui_->artist->setText( track->getArtist() );
	this->ui_->albumTitle->setText( track->getAlbum()->getTitle() );
	return this->exec();
}

void PropertyDialog::perform_( QAbstractButton * button ) {
	switch( this->ui_->buttonBox->buttonRole( button ) ) {
		case QDialogButtonBox::AcceptRole:
			this->write_();
			this->accept();
			break;
		case QDialogButtonBox::ApplyRole:
			this->write_();
			break;
		case QDialogButtonBox::RejectRole:
			this->reject();
			break;
		default:
			;
	}
}

void PropertyDialog::write_() {
}
