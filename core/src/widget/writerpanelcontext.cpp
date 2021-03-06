/**
 * @file writerpanelcontext.cpp
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
#include "writerpanelcontext.hpp"

#include "runtimeerror.hpp"

namespace khopper {
namespace widget {

WriterPanelContext * self = nullptr;

}
}

using khopper::error::RunTimeError;
using khopper::widget::WriterPanelContext;
using khopper::widget::Panel;

WriterPanelContext & WriterPanelContext::instance() {
	if( !self ) {
		throw RunTimeError( QObject::tr( "WriterPanelContext has not been initialized yet" ), __FILE__, __LINE__ );
	}
	return *self;
}

WriterPanelContext::WriterPanelContext() {
	if( self ) {
		throw RunTimeError( QObject::tr( "WriterPanelContext has been initialized" ), __FILE__, __LINE__ );
	}
	self = this;
}

WriterPanelContext::~WriterPanelContext() {
	self = nullptr;
}

void WriterPanelContext::install( Panel * panel ) {
	emit this->installed( panel );
}

void WriterPanelContext::uninstall( Panel * panel ) {
	emit this->uninstalled( panel );
}
