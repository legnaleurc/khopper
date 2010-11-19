/**
 * @file uriinputdialog.cpp
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
#include "uriinputdialog.hpp"
#include "ui_uriinputdialog.h"

#include <QtCore/QTextStream>

using namespace khopper::widget;

UriInputDialog::UriInputDialog( QWidget * parent ):
QDialog( parent ),
ui_( new Ui::UriInputDialog ) {
	this->ui_->setupUi( this );
}

void UriInputDialog::setVisible( bool visible ) {
	this->QDialog::setVisible( visible );
	if( visible ) {
		this->ui_->plainTextEdit->clear();
	}
}

QList< QUrl > UriInputDialog::getUriList() const {
	QString plainText( this->ui_->plainTextEdit->toPlainText() );
	QList< QUrl > uris;
	QTextStream sin( &plainText, QIODevice::ReadOnly | QIODevice::Text );
	for( QString line; !sin.atEnd(); ) {
		line = sin.readLine();
		QUrl tmp( QUrl::fromUserInput( line ) );
		if( tmp.isValid() ) {
			uris.push_back( tmp );
		}
	}
	return uris;
}
