/**
 * @file timestamp.hpp
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
#ifndef KHOPPER_ALBUM_TIMESTAMP_HPP
#define KHOPPER_ALBUM_TIMESTAMP_HPP

#include "util/tr1.hpp"

#include <string>

#include <QMetaType>

namespace khopper {

	namespace album {

		/**
		 * @brief Track index information
		 */
		class Timestamp {
		public:
			/**
			 * @brief Default constructor
			 *
			 * Initialize to 0:0.0.
			 */
			Timestamp();
			/**
			 * @brief Custom constructor
			 * @param [in] m minute
			 * @param [in] s second
			 * @param [in] ms millisecond
			 *
			 * Initialize to m:s.ms.
			 */
			Timestamp( int m, short int s, short int ms );

			/// if equals to zero
			bool isZero() const;
			int getMinute() const {
				return this->minute_;
			}
			short getSecond() const {
				return this->second_;
			}
			short getMillisecond() const {
				return this->millisecond_;
			}

			Timestamp & operator +=( const Timestamp & that );
			/**
			 * @brief Subtract assign operator
			 * @param that Subtrahend
			 * @return self
			 */
			Timestamp & operator -=( const Timestamp & that );
			/**
			 * @brief Subtract operator
			 * @param that Subtrahend
			 * @return result
			 */
			Timestamp operator -( const Timestamp & that ) const;

			/**
			 * @brief Convert to millisecond
			 */
			int64_t toMillisecond() const;
			/**
			 * @brief Convert to second
			 */
			double toSecond() const;
			QString toString() const;

			/**
			 * @brief Convert from int
			 * @param [in] millisecond Time stamp in millisecond
			 */
			static Timestamp fromMillisecond( int64_t millisecond );
			/**
			 * @brief Convert from double
			 * @param [in] second Time stamp in second
			 */
			static Timestamp fromSecond( double second );

		private:
			int minute_;
			short int second_;
			short int millisecond_;
		};

	}

}

Q_DECLARE_METATYPE(khopper::album::Timestamp)

#endif
