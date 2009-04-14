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
#include <QLabel>

namespace khopper {

	namespace widget {

		TextCodec::TextCodec( QWidget * parent, Qt::WindowFlags f ):
		QDialog( parent, f ),
		encoded_(),
		decoded_(),
		codecs_( new QComboBox( this ) ),
		contents_( new QLabel( this ) ) {
			setWindowTitle( tr( "Choose Encoding" ) );

			this->codecs_->addItem( "UTF-8", QTextCodec::codecForName( "UTF-8" )->mibEnum() );
			this->codecs_->addItem( "Shift-JIS", QTextCodec::codecForName( "Shift-JIS" )->mibEnum() );
			this->codecs_->addItem( "Big5", QTextCodec::codecForName( "Big5" )->mibEnum() );
			connect( this->codecs_, SIGNAL( currentIndexChanged( int ) ), this, SLOT( update( int ) ) );

			this->contents_->setFixedSize( 200, 200 );

			QDialogButtonBox * buttons = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this );
			connect( buttons, SIGNAL( accepted() ), this, SLOT( accept() ) );
			connect( buttons, SIGNAL( rejected() ), this, SLOT( reject() ) );

			QVBoxLayout * box = new QVBoxLayout( this );
			this->setLayout( box );
			box->addWidget( this->codecs_ );
			box->addWidget( buttons );
			box->addWidget( this->contents_ );
		}

		void TextCodec::setEncoded( const QByteArray & encoded ) {
			this->encoded_ = encoded;
			update( this->codecs_->currentIndex() );
		}

		QString TextCodec::getDecoded() const {
			return this->decoded_;
		}

		void TextCodec::update( int index ) {
			int mib = this->codecs_->itemData( index ).toInt();
			QTextStream ts( &this->encoded_ );
			ts.setCodec( QTextCodec::codecForMib( mib ) );
			this->decoded_ = ts.readAll();

			this->contents_->setText( this->decoded_ );
		}

	}

}
