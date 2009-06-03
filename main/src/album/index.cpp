/**
 * @file index.cpp
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
#include "index.hpp"

#include <boost/format.hpp>

#include <sstream>
#include <cmath>

namespace khopper {

	namespace album {

		Index::Index():
		minute( 0 ),
		second( 0 ),
		millisecond( 0 ) {
		}

		Index::Index( short int m, short int s, short int ms ):
		minute( m ),
		second( s ),
		millisecond( ms ) {
		}

		bool Index::isZero() const {
			return ( this->minute == 0 ) && ( this->second == 0 ) && ( this->millisecond == 0 );
		}

		Index & Index::operator -=( const Index & that ) {
			if( this->millisecond < that.millisecond ) {
				--this->second;
				this->millisecond += 1000 - that.millisecond;
			} else {
				this->millisecond -= that.millisecond;
			}
			if( this->second < that.second ) {
				--this->minute;
				this->second += 60 - that.second;
			} else {
				this->second -= that.second;
			}
			// FIXME: may be negative
			this->minute -= that.minute;

			return *this;
		}

		Index Index::operator -( const Index & that ) const {
			return Index( *this ) -= that;
		}

		int Index::toMillisecond() const {
			return ( this->minute * 60 + this->second ) * 1000 + this->millisecond;
		}

		double Index::toSecond() const {
			return 60 * minute + second + millisecond / 1000.0;
		}

		std::wstring Index::toStdWString() const {
			boost::wformat tpl( L"%1%:%|2$02|.%|3$03|" );
			return ( tpl % this->minute % this->second % this->millisecond ).str();
		}

		Index Index::fromMillisecond( int millisecond ) {
			Index tmp;
			tmp.millisecond = millisecond % 1000;
			millisecond /= 1000;
			tmp.second = millisecond % 60;
			millisecond /= 60;
			tmp.minute = millisecond;
			return tmp;
		}

		Index Index::fromSecond( double second ) {
			return fromMillisecond( static_cast< int >( second * 1000 ) );
		}

	}

}
