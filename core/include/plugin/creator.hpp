/**
 * @file creator.hpp
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
#ifndef KHOPPER_CREATOR_HPP
#define KHOPPER_CREATOR_HPP

#include "common/tr1.hpp"

namespace khopper {

	/// @defgroup Plugins Plugin System
	// @{
	// @}
	/**
	 * @brief Contains plugin utility
	 * @ingroup Plugins
	 */
	namespace plugin {

		/**
		 * @brief Common creator factory
		 * @tparam Product The product type of return
		 * @sa ReaderCreator WriterCreator
		 *
		 * You will not use this class directly.
		 */
		template< typename Product >
		class Creator {
		public:
			/// Default constructor
			Creator() {}
			/// Create the product
			std::tr1::shared_ptr< Product > create() const {
				return this->create_();
			}
		private:
			Creator( const Creator & );
			Creator & operator =( const Creator & );
			virtual std::tr1::shared_ptr< Product > create_() const = 0;
		};

	}

}

#endif
