/**
 * @file mp3plugin.cpp
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
#include "mp3plugin.hpp"

#include <QtCore/QtPlugin>

#include "khopper/writerpanelcontext.hpp"
#include "mp3panel.hpp"

Q_EXPORT_PLUGIN2( KHOPPER_PLUGIN_ID, khopper::plugin::Mp3Plugin )

using khopper::plugin::Mp3Plugin;
using khopper::widget::Mp3Panel;
using khopper::widget::WriterPanelContext;

Mp3Plugin::Mp3Plugin():
AbstractPlugin(),
panel_( new Mp3Panel ) {
	this->setID( KHOPPER_STRINGIZE(KHOPPER_PLUGIN_ID) );
	this->setVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
}

void Mp3Plugin::doInstall() {
	WriterPanelContext::instance().install( this->panel_.get() );
}

void Mp3Plugin::doUninstall() {
	WriterPanelContext::instance().uninstall( this->panel_.get() );
}
