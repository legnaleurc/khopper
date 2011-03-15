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
#include "timestampprivate.hpp"

#include <cmath>
#include <sstream>

using namespace khopper::album;

Timestamp::TimestampPrivate::TimestampPrivate( int min, int sec, int ms ):
millisecond( ms ),
minute( min ),
second( sec ) {
}

Timestamp::Timestamp() : p_( new TimestampPrivate( 0, 0, 0 ) ) {
}

Timestamp::Timestamp( int m, int s, int ms ) : p_( new TimestampPrivate( m, s, ms ) ) {
}

Timestamp::Timestamp( const Timestamp & that ) : p_( new TimestampPrivate( *that.p_ ) ) {
}

int Timestamp::getMinute() const {
	return this->p_->minute;
}

int Timestamp::getSecond() const {
	return this->p_->second;
}

int Timestamp::getMillisecond() const {
	return this->p_->millisecond;
}

bool Timestamp::isZero() const {
	return ( this->p_->minute == 0 ) && ( this->p_->second == 0 ) && ( this->p_->millisecond == 0 );
}

bool Timestamp::isValid() const {
	return ( this->p_->minute >= 0 ) && ( this->p_->second >= 0 ) && ( this->p_->millisecond >= 0 );
}

bool Timestamp::operator !=( const Timestamp & that ) const {
	return !( *this == that );
}

Timestamp Timestamp::operator +( const Timestamp & that ) const {
	return Timestamp( *this ) += that;
}

Timestamp & Timestamp::operator +=( const Timestamp & that ) {
	this->p_->millisecond += that.p_->millisecond;
	if( this->p_->millisecond >= 1000 ) {
		this->p_->millisecond -= 1000;
		this->p_->second += 1;
	}
	this->p_->second += that.p_->second;
	if( this->p_->second >= 60 ) {
		this->p_->second -= 60;
		this->p_->minute += 1;
	}
	this->p_->minute += that.p_->minute;

	return *this;
}

Timestamp Timestamp::operator -( const Timestamp & that ) const {
	return Timestamp( *this ) -= that;
}

Timestamp & Timestamp::operator -=( const Timestamp & that ) {
	if( this->p_->millisecond < that.p_->millisecond ) {
		--this->p_->second;
		this->p_->millisecond += 1000 - that.p_->millisecond;
	} else {
		this->p_->millisecond -= that.p_->millisecond;
	}
	if( this->p_->second < that.p_->second ) {
		--this->p_->minute;
		this->p_->second += 60 - that.p_->second;
	} else {
		this->p_->second -= that.p_->second;
	}
	// FIXME: may be negative
	this->p_->minute -= that.p_->minute;

	return *this;
}

bool Timestamp::operator <( const Timestamp & that ) const {
	if( this->p_->minute == that.p_->minute ) {
		if( this->p_->second == that.p_->second ) {
			return this->p_->millisecond < that.p_->millisecond;
		} else {
			return this->p_->second < that.p_->second;
		}
	} else {
		return this->p_->minute < that.p_->minute;
	}
}

bool Timestamp::operator <=( const Timestamp & that ) const {
	return *this < that || *this == that;
}

Timestamp & Timestamp::operator =( const Timestamp & that ) {
	if( this != &that ) {
		*this->p_ = *that.p_;
	}
	return *this;
}

bool Timestamp::operator ==( const Timestamp & that ) const {
	return ( this->p_->minute >= that.p_->minute ) && ( this->p_->second >= that.p_->second ) && ( this->p_->millisecond >= that.p_->millisecond );;
}

bool Timestamp::operator >( const Timestamp & that ) const {
	if( this->p_->minute == that.p_->minute ) {
		if( this->p_->second == that.p_->second ) {
			return this->p_->millisecond > that.p_->millisecond;
		} else {
			return this->p_->second > that.p_->second;
		}
	} else {
		return this->p_->minute > that.p_->minute;
	}
}

bool Timestamp::operator >=( const Timestamp & that ) const {
	return *this > that || *this == that;
}

int64_t Timestamp::toMillisecond() const {
	return ( static_cast< int64_t >( this->p_->minute ) * 60 + this->p_->second ) * 1000 + this->p_->millisecond;
}

double Timestamp::toSecond() const {
	return 60 * this->p_->minute + this->p_->second + this->p_->millisecond / 1000.0;
}

QString Timestamp::toString() const {
	return QString( "%1:%2.%3" ).arg( this->p_->minute ).arg( this->p_->second, 2L, 10L, QChar( '0' ) ).arg( this->p_->millisecond, 3L, 10L, QChar( '0' ) );
}

Timestamp Timestamp::fromMillisecond( int64_t millisecond ) {
	Timestamp tmp;
	tmp.p_->millisecond = millisecond % 1000;
	millisecond /= 1000;
	tmp.p_->second = millisecond % 60;
	millisecond /= 60;
	tmp.p_->minute = millisecond;
	return tmp;
}

Timestamp Timestamp::fromSecond( double second ) {
	return fromMillisecond( static_cast< int >( second * 1000 ) );
}
