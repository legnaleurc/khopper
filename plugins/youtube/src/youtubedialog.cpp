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

#include <algorithm>

using namespace khopper::widget;

YouTubeDialog::YouTubeDialog():
QDialog( 0 ),
buttons_(),
ui_( new Ui::YouTubeDialog ) {
	this->ui_->setupUi( this );

	this->buttons_.insert( std::make_pair( "5", this->ui_->radioButton ) );
	this->buttons_.insert( std::make_pair( "18", this->ui_->radioButton_2 ) );
	this->buttons_.insert( std::make_pair( "22", this->ui_->radioButton_3 ) );
	this->buttons_.insert( std::make_pair( "34", this->ui_->radioButton_4 ) );
	this->buttons_.insert( std::make_pair( "35", this->ui_->radioButton_5 ) );
	this->buttons_.insert( std::make_pair( "37", this->ui_->radioButton_6 ) );
	this->buttons_.insert( std::make_pair( "38", this->ui_->radioButton_7 ) );
	this->buttons_.insert( std::make_pair( "43", this->ui_->radioButton_8 ) );
	this->buttons_.insert( std::make_pair( "45", this->ui_->radioButton_9 ) );

	this->clearFormat();
}

void YouTubeDialog::addFormat( const QString & format ) {
	this->buttons_[format]->setEnabled( true );
}

void YouTubeDialog::clearFormat() {
	std::for_each( this->buttons_.begin(), this->buttons_.end(), []( std::pair< QString, QRadioButton * > p ) {
		p.second->setDisabled( true );
	} );
}
