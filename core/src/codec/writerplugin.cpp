/**
 * @file writerplugin.cpp
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
#include "writerplugin.hpp"

namespace khopper {

	namespace plugin {

		bool registerWriter( const std::string & key, const std::string & plugin ) {
			return WriterFactory::Instance().Register( key, CreatorLoader< WriterCreator >( plugin ) );
		}

		codec::WriterSP createWriter( const std::string & key ) {
			WriterCreator * tmp = NULL;
			try {
				tmp = WriterFactory::Instance().CreateObject( key );
			} catch( std::exception & e ) {
				tmp = WriterFactory::Instance().CreateObject( "*" );
			}
			return tmp->create();
		}

	}

}
