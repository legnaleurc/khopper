/**
 * @file index.hpp
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
#ifndef KHOPPER_ALBUM_INDEX_HPP
#define KHOPPER_ALBUM_INDEX_HPP

#include <string>

namespace khopper {

	namespace album {

		/**
		 * @brief Track index information
		 */
		struct Index {
			/// minite
			short int minute;
			/// second
			short int second;
			/// millisecond
			short int millisecond;

			/**
			 * @brief Default constructor
			 *
			 * Initialize to 0:0.0.
			 */
			Index();
			/**
			 * @brief Custom constructor
			 * @param [in] m minute
			 * @param [in] s second
			 * @param [in] ms millisecond
			 *
			 * Initialize to m:s.ms.
			 */
			Index( short int m, short int s, short int ms );

			/// if equals to zero
			bool isZero() const;

			/**
			 * @brief Subtract assign operator
			 * @param that Subtrahend
			 * @return self
			 */
			Index & operator -=( const Index & that );
			/**
			 * @brief Subtract operator
			 * @param that Subtrahend
			 * @return result
			 */
			Index operator -( const Index & that ) const;

			/**
			 * @brief Convert to millisecond
			 */
			int toMillisecond() const;
			/**
			 * @brief Convert to second
			 */
			double toSecond() const;
			/**
			 * @brief Convert to std::wstring
			 * @return mm:ss.ff
			 */
			std::wstring toStdWString() const;

			/**
			 * @brief Convert from int
			 * @param [in] millisecond Time stamp in millisecond
			 */
			static Index fromMillisecond( int millisecond );
			/**
			 * @brief Convert from double
			 * @param [in] second Time stamp in second
			 */
			static Index fromSecond( double second );
		};

	}

}

#endif
