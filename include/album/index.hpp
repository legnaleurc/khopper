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
#ifndef KHOPPER_INDEX_HPP
#define KHOPPER_INDEX_HPP

#include <string>

namespace Khopper {

	/**
	 * @brief Track index information
	 */
	class Index {
	public:
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
		 * @param [in] f frame
		 *
		 * Initialize to m:s.f.
		 */
		Index( short int m, short int s, short int f );

		/**
		 * @brief Convert from double
		 * @param [in] seconds Time stamp in seconds
		 */
		Index( double seconds );

		/// minite
		unsigned short int minute;
		/// second
		unsigned short int second;
		/// frame
		unsigned short int frame;

		/**
		 * @brief Sub with @p that and assign to self, return self.
		 * @param that sub
		 * @return self
		 */
		const Index & operator -=( const Index & that );
		/**
		 * @brief Sub with @p that and return result.
		 * @param that sub
		 * @return result
		 */
		Index operator -( const Index & that ) const;

		/**
		 * @brief Convert to double
		 * @return seconds in double
		 */
		double toDouble() const;

		/**
		 * @brief Convert to std::wstring
		 * @return mm:ss.ff
		 */
		std::wstring toStdWString() const;
	};

}

#endif
