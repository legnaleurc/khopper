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

#include <cmath>
#include <sstream>

#include "runtimeerror.hpp"

namespace khopper {
namespace album {

class Timestamp::Private {
public:
	Private( int min, int second, int ms );

	bool operator ==( const Private & that ) const;

	int millisecond;
	int minute;
	int second;
};

}
}

using khopper::album::Timestamp;
using khopper::error::RunTimeError;

Timestamp::Private::Private( int min, int sec, int ms ):
millisecond( ms ),
minute( min ),
second( sec ) {
}

bool Timestamp::Private::operator==( const Private & that ) const {
	return this->minute == that.minute && this->second == that.second && this->millisecond == that.millisecond;
}

Timestamp::Timestamp(): p_() {
}

Timestamp::Timestamp( int m, int s, int ms ): p_( new Private( m, s, ms ) ) {
}

Timestamp::Timestamp( const Timestamp & that ): p_() {
	if( that.p_ ) {
		this->p_.reset( new Private( *that.p_ ) );
	}
}

int Timestamp::getMinute() const {
	if( !this->isValid() ) {
		throw RunTimeError( "invalid timestamp", __FILE__, __LINE__ );
	}
	return this->p_->minute;
}

int Timestamp::getSecond() const {
	if( !this->isValid() ) {
		throw RunTimeError( "invalid timestamp", __FILE__, __LINE__ );
	}
	return this->p_->second;
}

int Timestamp::getMillisecond() const {
	if( !this->isValid() ) {
		throw RunTimeError( "invalid timestamp", __FILE__, __LINE__ );
	}
	return this->p_->millisecond;
}

bool Timestamp::isZero() const {
	if( !this->isValid() ) {
		return false;
	}
	return ( this->p_->minute == 0 ) && ( this->p_->second == 0 ) && ( this->p_->millisecond == 0 );
}

bool Timestamp::isValid() const {
	return this->p_.use_count() >= 1;
}

bool Timestamp::operator !=( const Timestamp & that ) const {
	return !( *this == that );
}

Timestamp Timestamp::operator +( const Timestamp & that ) const {
	return Timestamp( *this ) += that;
}

Timestamp & Timestamp::operator +=( const Timestamp & that ) {
	if( !this->isValid() || !that.isValid() ) {
		throw RunTimeError( "invalid timestamp", __FILE__, __LINE__ );
	}
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
	if( !this->isValid() || !that.isValid() ) {
		throw RunTimeError( "invalid timestamp", __FILE__, __LINE__ );
	}
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
	if( !this->isValid() || !that.isValid() ) {
		throw RunTimeError( "invalid timestamp", __FILE__, __LINE__ );
	}
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
	if( !this->isValid() || !that.isValid() ) {
		throw RunTimeError( "invalid timestamp", __FILE__, __LINE__ );
	}
	return *this < that || *this == that;
}

Timestamp & Timestamp::operator =( const Timestamp & that ) {
	if( this != &that ) {
		if( this->isValid() && that.isValid() ) {
			*this->p_ = *that.p_;
		} else if( this->isValid() ) {
			this->p_.reset();
		} else {
			this->p_.reset( new Private( *that.p_ ) );
		}
	}
	return *this;
}

bool Timestamp::operator ==( const Timestamp & that ) const {
	if( this->isValid() && that.isValid() ) {
		return *this->p_ == *that.p_;
	}
	return this->p_ == that.p_;
}

bool Timestamp::operator >( const Timestamp & that ) const {
	if( !this->isValid() || !that.isValid() ) {
		throw RunTimeError( "invalid timestamp", __FILE__, __LINE__ );
	}
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
	if( !this->isValid() ) {
		throw RunTimeError( "invalid timestamp", __FILE__, __LINE__ );
	}
	return ( static_cast< int64_t >( this->p_->minute ) * 60 + this->p_->second ) * 1000 + this->p_->millisecond;
}

double Timestamp::toSecond() const {
	if( !this->isValid() ) {
		throw RunTimeError( "invalid timestamp", __FILE__, __LINE__ );
	}
	return 60 * this->p_->minute + this->p_->second + this->p_->millisecond / 1000.0;
}

QString Timestamp::toString() const {
	if( !this->isValid() ) {
		throw RunTimeError( "invalid timestamp", __FILE__, __LINE__ );
	}
	return QString( "%1:%2.%3" ).arg( this->p_->minute ).arg( this->p_->second, 2L, 10L, QChar( '0' ) ).arg( this->p_->millisecond, 3L, 10L, QChar( '0' ) );
}

Timestamp Timestamp::fromMillisecond( int64_t millisecond ) {
	auto ms = millisecond % 1000;
	millisecond /= 1000;
	auto sec = millisecond % 60;
	millisecond /= 60;
	auto min = millisecond;
	return Timestamp( min, sec, ms );
}

Timestamp Timestamp::fromSecond( double second ) {
	return fromMillisecond( static_cast< int >( second * 1000 ) );
}
