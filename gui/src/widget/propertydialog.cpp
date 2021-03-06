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
#include "khopper/album.hpp"
#include "ui_propertydialog.h"

using namespace khopper::widget;
using khopper::album::TrackSP;

PropertyDialog::PropertyDialog( QWidget * parent ):
QDialog( parent ),
currentTrack_(),
ui_( new Ui::PropertyDialog ) {
	this->ui_->setupUi( this );

	// setup dialog button box
	QObject::connect( this->ui_->buttonBox, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( perform_( QAbstractButton * ) ) );
}

int PropertyDialog::exec( TrackSP track ) {
	this->currentTrack_ = track;
	this->ui_->title->setText( track->getTitle() );
	this->ui_->artist->setText( track->getArtist() );
	auto album = track->getAlbum().lock();
	this->ui_->albumTitle->setText( album->getTitle() );
	this->ui_->bitRate->setText( QString( "%1 bps" ).arg( track->getBitRate() ) );
	this->ui_->channels->setNum( track->getAudioFormat().channels() );
	this->ui_->frequency->setText( QString( "%1 Hz" ).arg( track->getAudioFormat().frequency() ) );
	return this->exec();
}

void PropertyDialog::perform_( QAbstractButton * button ) {
	switch( this->ui_->buttonBox->buttonRole( button ) ) {
		case QDialogButtonBox::AcceptRole:
			this->write_();
			this->currentTrack_.reset();
			this->accept();
			break;
		case QDialogButtonBox::ApplyRole:
			this->write_();
			break;
		case QDialogButtonBox::RejectRole:
			this->reject();
			this->currentTrack_.reset();
			break;
		default:
			;
	}
}

void PropertyDialog::write_() {
	this->currentTrack_->setTitle( this->ui_->title->text() );
	this->currentTrack_->setArtist( this->ui_->artist->text() );
	auto album = this->currentTrack_->getAlbum().lock();
	album->setTitle( this->ui_->albumTitle->text() );

	this->currentTrack_->save();
}
