#include "index.hpp"

namespace Khopper {

	Index::Index() : minute( 0 ), second( 0 ), frame( 0 ) {
	}

	Index::Index( short int m, short int s, short int f ):
	minute( m ),
	second( s ),
	frame( f ) {
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
		this->minute -= that.second;

		return *this;
	}

	Index Index::operator -( const Index & that ) const {
		return Index( *this ) -= that;
	}

	double Index::toDouble() const {
		return 60 * minute + second + frame / 100.0;
	}

	std::wstring Index::toStdWString() const {
		return L"";
	}

}
