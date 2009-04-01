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

#include <QRegExp>
#include <QSettings>
#include <QGroupBox>
#include <QVBoxLayout>

namespace khopper {

	namespace widget {

		Preference::Preference( QWidget * parent ):
		QDialog( parent ),
		fnTpl_( new QLineEdit( this ) ),
		buttons_( new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Apply | QDialogButtonBox::Cancel, Qt::Horizontal, this ) ) {
			QSettings setting;
			setting.beginGroup( "preference" );

			this->setWindowTitle( tr( "Preference" ) );

			QVBoxLayout * mainBox = new QVBoxLayout( this );
			this->setLayout( mainBox );

			QGroupBox * fnGroup = new QGroupBox( tr( "File name template" ), this );
			mainBox->addWidget( fnGroup );

			QVBoxLayout * fnBox = new QVBoxLayout( fnGroup );
			fnGroup->setLayout( fnBox );

			// Output name template
			this->fnTpl_->setText( setting.value( "filename", "%2i_%t" ).toString() );
			this->fnTpl_->setToolTip( tr(
				"Keywords:\n"
				"%t: title\n"
				"%a: artist\n"
				"%Ni: index of track, N means width\n"
				"%%: \'%\' literal\n"
				"Extensions will automaticaly append."
			) );
			fnBox->addWidget( this->fnTpl_ );

			mainBox->addWidget( this->buttons_ );
			connect( this->buttons_, SIGNAL( clicked( QAbstractButton * ) ), this, SLOT( perform_( QAbstractButton * ) ) );

			setting.endGroup();
		}

		boost::format Preference::getTemplate() const {
			QString tmp = this->fnTpl_->text();
			tmp.replace( "%t", "%1%" );
			tmp.replace( "%a", "%2%" );
			tmp.replace( QRegExp( "%(\\d*)i" ), "%|3$0\\1|" );
			tmp.replace( "%%", "%" );
			return boost::format( tmp.toUtf8().constData() );
		}

		void Preference::perform_( QAbstractButton * button ) {
			switch( this->buttons_->buttonRole( button ) ) {
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

			setting.setValue( "filename", this->fnTpl_->text() );

			setting.endGroup();
		}

		void Preference::revert_() {
			QSettings setting;
			setting.beginGroup( "preference" );

			this->fnTpl_->setText( setting.value( "filename", "%2i_%t" ).toString() );

			setting.endGroup();
		}

	}

}
