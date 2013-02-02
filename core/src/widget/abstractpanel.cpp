/**
 * @file abstractpanel.cpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include "abstractpanel.hpp"

namespace khopper {
namespace widget {

class AbstractPanel::Private {
public:
	QString title;
	QString suffix;
};

}
}

using khopper::widget::AbstractPanel;

AbstractPanel::AbstractPanel():
QWidget(),
p_( new Private ) {
}

void AbstractPanel::setTitle( const QString & title ) {
	this->p_->title = title;
}

const QString & AbstractPanel::getTitle() const {
	return this->p_->title;
}

void AbstractPanel::setSuffix( const QString & suffix ) {
	this->p_->suffix = suffix;
}

const QString & AbstractPanel::getSuffix() const {
	return this->p_->suffix;
}
