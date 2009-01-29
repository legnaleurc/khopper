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
