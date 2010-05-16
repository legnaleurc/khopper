/**
 * @file codecselector.cpp
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
#include "codecselector.hpp"
#include "ui_codecselector.h"

#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

namespace khopper {

	namespace widget {

		CodecSelector::CodecSelector( QWidget * parent ):
		QDialog( parent ),
		ui_( new Ui::CodecSelector ),
		encoded_(),
		decoded_() {
			this->ui_->setupUi( this );

			this->ui_->codecs->addItem( "UTF-8", QTextCodec::codecForName( "UTF-8" )->mibEnum() );
			this->ui_->codecs->addItem( "Shift-JIS", QTextCodec::codecForName( "Shift-JIS" )->mibEnum() );
			this->ui_->codecs->addItem( "Big5", QTextCodec::codecForName( "Big5" )->mibEnum() );
			connect( this->ui_->codecs, SIGNAL( currentIndexChanged( int ) ), this, SLOT( update_( int ) ) );
		}

		CodecSelector::~CodecSelector() {
			delete this->ui_;
		}

		void CodecSelector::setEncoded( const QByteArray & encoded ) {
			this->encoded_ = encoded;
			this->update_( this->ui_->codecs->currentIndex() );
		}

		QString CodecSelector::getDecoded() const {
			return this->decoded_;
		}

		void CodecSelector::update_( int index ) {
			int mib = this->ui_->codecs->itemData( index ).toInt();
			QTextStream ts( &this->encoded_ );
			ts.setCodec( QTextCodec::codecForMib( mib ) );
			this->decoded_ = ts.readAll();

			this->ui_->contents->setText( this->decoded_ );
		}

	}

}
