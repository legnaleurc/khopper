/**
 * @file preference.cpp
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
#include "preference.hpp"
#include "ui_preference.h"

#include <QtCore/QRegExp>
#include <QtCore/QSettings>
#include <QtGui/QApplication>
#include <QtGui/QFontDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

namespace {

	inline QString fontTemplate( const QFont & font ) {
		static const QString tpl = "%1 %2pt";
		return tpl.arg( font.family() ).arg( font.pointSize() );
	}

}

namespace khopper {

	namespace widget {

		Preference::Preference( QWidget * parent ):
		QDialog( parent ),
		ui_( new Ui::Preference ),
		currentFont_() {
			this->ui_->setupUi( this );
			QSettings setting;

			setting.beginGroup( "preference" );
			if( setting.contains( "font" ) && this->currentFont_.fromString( setting.value( "font" ).toString() ) ) {
				qApp->setFont( this->currentFont_ );
			} else {
				this->currentFont_.fromString( qApp->font().toString() );
			}
			setting.endGroup();

			this->ui_->fontName->setText( fontTemplate( this->currentFont_ ) );

			connect( this->ui_->browse, SIGNAL( clicked() ), this, SLOT( changeFont_() ) );

			// Output name template
			setting.beginGroup( "preference" );
			this->ui_->tpl->setText( setting.value( "filename", "%2i_%t" ).toString() );
			setting.endGroup();

			connect( this->ui_->buttonBox, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( perform_( QAbstractButton * ) ) );
		}

		Preference::~Preference() {
			delete this->ui_;
		}

		boost::format Preference::getTemplate() const {
			QString tmp = this->ui_->tpl->text();
			tmp.replace( "%t", "%1%" );
			tmp.replace( "%a", "%2%" );
			tmp.replace( QRegExp( "%(\\d*)i" ), "%|3$0\\1|" );
			tmp.replace( "%%", "%" );
			return boost::format( tmp.toUtf8().constData() );
		}

		void Preference::changeFont_() {
			bool ok = false;
			QFont font = QFontDialog::getFont( &ok, this->currentFont_, this );
			if( ok ) {
				this->currentFont_ = font;
			}
			this->ui_->fontName->setText( fontTemplate( this->currentFont_ ) );
		}

		void Preference::perform_( QAbstractButton * button ) {
			switch( this->ui_->buttonBox->buttonRole( button ) ) {
			case QDialogButtonBox::AcceptRole:
				this->apply_();
				this->accept();
				break;
			case QDialogButtonBox::RejectRole:
				this->revert_();
				this->reject();
				break;
			case QDialogButtonBox::ApplyRole:
				this->apply_();
				break;
			default:
				;
			};
		}

		void Preference::apply_() {
			QSettings setting;
			setting.beginGroup( "preference" );

			setting.setValue( "font", this->currentFont_.toString() );
			qApp->setFont( this->currentFont_ );

			setting.setValue( "filename", this->ui_->tpl->text() );

			setting.endGroup();
		}

		void Preference::revert_() {
			QSettings setting;
			setting.beginGroup( "preference" );

			this->currentFont_.fromString( setting.value( "font", qApp->font().toString() ).toString() );
			this->ui_->fontName->setText( fontTemplate( this->currentFont_ ) );

			this->ui_->tpl->setText( setting.value( "filename", "%2i_%t" ).toString() );

			setting.endGroup();
		}

	}

}
