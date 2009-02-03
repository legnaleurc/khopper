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

#include <sstream>
#include <iomanip>
#include <cmath>

namespace Khopper {

	Index::Index() : minute( 0 ), second( 0 ), frame( 0 ) {
	}

	Index::Index( short int m, short int s, short int f ):
	minute( m ),
	second( s ),
	frame( f ) {
	}

	Index::Index( double d ) {
		this->minute = std::floor( d / 60 );
		d -= this->minute * 60;
		this->second = std::floor( d );
		d -= this->second;
		this->frame = std::floor( d * 100 );
	}

	const Index & Index::operator -=( const Index & that ) {
		if( this->frame < that.frame ) {
			--this->second;
			this->frame += 100 - that.frame;
		} else {
			this->frame -= that.frame;
		}
		if( this->second < that.second ) {
			--this->minute;
			this->second += 60 - that.second;
		} else {
			this->second -= that.second;
		}
		this->minute -= that.minute;

		return *this;
	}

	Index Index::operator -( const Index & that ) const {
		return Index( *this ) -= that;
	}

	double Index::toDouble() const {
		return 60 * minute + second + frame / 100.0;
	}

	std::wstring Index::toStdWString() const {
		std::wostringstream sout;
		sout << this->minute << L':';
		sout << std::setfill( L'0' ) << std::setw( 2 ) << this->second;
		sout << L'.' << std::setfill( L'0' ) << std::setw( 2 ) << this->frame;
		return sout.str();
	}

}
