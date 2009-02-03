/**
 * @file textcodec.cpp
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
#include "textcodec.hpp"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QTextCodec>
#include <QTextStream>
#include <QVBoxLayout>

namespace Khopper {

	TextCodec::TextCodec( QWidget *parent, Qt::WindowFlags f ) : QDialog( parent, f ) {
		setWindowTitle( tr( "Choose Encoding" ) );

		codecs_ = new QComboBox( this );
		codecs_->addItem( "UTF-8", QTextCodec::codecForName( "UTF-8" )->mibEnum() );
		codecs_->addItem( "Shift-JIS", QTextCodec::codecForName( "Shift-JIS" )->mibEnum() );
		codecs_->addItem( "Big5", QTextCodec::codecForName( "Big5" )->mibEnum() );
		//codecs_->addItem( "GB18030-0", QTextCodec::codecForName( "GB18030-0" )->mibEnum() );
		connect( codecs_, SIGNAL( currentIndexChanged( int ) ), this, SLOT( update( int ) ) );

		buttons_ = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this );
		connect( buttons_, SIGNAL( accepted() ), this, SLOT( accept() ) );
		connect( buttons_, SIGNAL( rejected() ), this, SLOT( reject() ) );

		QVBoxLayout * box = new QVBoxLayout( this );
		setLayout( box );
		box->addWidget( codecs_ );
		box->addWidget( buttons_ );
	}

	void TextCodec::setEncoded( const QByteArray & encoded ) {
		encoded_ = encoded;
		update( codecs_->currentIndex() );
	}

	QString TextCodec::getDecoded() const {
		return decoded_;
	}

	void TextCodec::update( int index ) {
		int mib = codecs_->itemData( index ).toInt();
		QTextStream ts( &encoded_ );
		ts.setCodec( QTextCodec::codecForMib( mib ) );
		decoded_ = ts.readAll();
	}

}
