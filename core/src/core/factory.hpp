/**
 * @file factory.hpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KHOPPER_FACTORY_HPP
#define KHOPPER_FACTORY_HPP

#include <algorithm>
#include <functional>
#include <map>

namespace khopper {

template< typename IDType, typename KeyType, typename ProductType, template< typename, typename > class ErrorPolicy >
class Factory : private ErrorPolicy< KeyType, std::function< ProductType ( const KeyType & ) > > {
private:
	typedef std::function< int ( const KeyType & ) > VerifierType;
	typedef std::function< ProductType ( const KeyType & ) > CreatorType;
	typedef std::map< IDType, std::pair< VerifierType, CreatorType > > TableType;

public:
	bool registerProduct( const IDType & id, VerifierType verifier, CreatorType creator ) {
		return this->t_.insert( std::make_pair( id, std::make_pair( verifier, creator ) ) ).second;
	}

	bool unregisterProduct( const IDType & id ) {
		return this->t_.erase( id ) == 1;
	}

	CreatorType getCreator( const KeyType & key ) const {
		CreatorType maxCreator;
		int maxScore = 0;
		std::for_each( this->t_.begin(), this->t_.end(), [&]( const typename TableType::value_type & p ) {
			int score = p.second.first( key );
			if( score > maxScore ) {
				maxScore = score;
				maxCreator = p.second.second;
			}
		} );
		if( maxScore <= 0 ) {
			return this->onError( key );
		}
		return maxCreator;
	}

private:
	TableType t_;
};

}

#endif
