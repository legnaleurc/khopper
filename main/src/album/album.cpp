/**
 * @file album.cpp
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
#include "album.hpp"

namespace khopper {

	namespace album {

		QVariant Album::get( const QString & key ) const {
			QMap< QString, QVariant >::const_iterator it = this->fields_.find( key );
			if( it == this->fields_.end() ) {
				// no such data, invalid
				return QVariant();
			} else {
				// assume no text codec problem
				return it.value();
			}
		}

		void Album::set( const QString & key, const QVariant & value ) {
			QMap< QString, QVariant >::iterator it = this->fields_.find( key );
			if( it == this->fields_.end() ) {
				this->fields_.insert( key, value );
			} else {
				it.value() = value;
			}
		}

	}

}
