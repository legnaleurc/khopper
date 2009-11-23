/**
 * @file timestamp.cpp
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
#include "timestamp.hpp"

#include <sstream>
#include <cmath>

namespace khopper {

	namespace album {

		Timestamp::Timestamp():
		minute_( 0 ),
		second_( 0 ),
		millisecond_( 0 ) {
		}

		Timestamp::Timestamp( int m, short int s, short int ms ):
		minute_( m ),
		second_( s ),
		millisecond_( ms ) {
		}

		bool Timestamp::isZero() const {
			return ( this->minute_ == 0 ) && ( this->second_ == 0 ) && ( this->millisecond_ == 0 );
		}

		Timestamp & Timestamp::operator +=( const Timestamp & that ) {
			this->millisecond_ += that.millisecond_;
			if( this->millisecond_ >= 1000 ) {
				this->millisecond_ -= 1000;
				this->second_ += 1;
			}
			this->second_ += that.second_;
			if( this->second_ >= 60 ) {
				this->second_ -= 60;
				this->minute_ += 1;
			}
			this->minute_ += that.minute_;

			return *this;
		}

		Timestamp & Timestamp::operator -=( const Timestamp & that ) {
			if( this->millisecond_ < that.millisecond_ ) {
				--this->second_;
				this->millisecond_ += 1000 - that.millisecond_;
			} else {
				this->millisecond_ -= that.millisecond_;
			}
			if( this->second_ < that.second_ ) {
				--this->minute_;
				this->second_ += 60 - that.second_;
			} else {
				this->second_ -= that.second_;
			}
			// FIXME: may be negative
			this->minute_ -= that.minute_;

			return *this;
		}

		Timestamp Timestamp::operator -( const Timestamp & that ) const {
			return Timestamp( *this ) -= that;
		}

		int64_t Timestamp::toMillisecond() const {
			return ( static_cast< int64_t >( this->minute_ ) * 60 + this->second_ ) * 1000 + this->millisecond_;
		}

		double Timestamp::toSecond() const {
			return 60 * this->minute_ + this->second_ + this->millisecond_ / 1000.0;
		}

		QString Timestamp::toString() const {
			return QString( "%1:%2.%3" ).arg( this->minute_ ).arg( this->second_, 2L, 10L, QChar( '0' ) ).arg( this->millisecond_, 3L, 10L, QChar( '0' ) );
		}

		Timestamp Timestamp::fromMillisecond( int64_t millisecond ) {
			Timestamp tmp;
			tmp.millisecond_ = millisecond % 1000;
			millisecond /= 1000;
			tmp.second_ = millisecond % 60;
			millisecond /= 60;
			tmp.minute_ = millisecond;
			return tmp;
		}

		Timestamp Timestamp::fromSecond( double second ) {
			return fromMillisecond( static_cast< int >( second * 1000 ) );
		}

	}

}
