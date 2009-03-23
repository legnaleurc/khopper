/**
 * @file codec_base.hpp
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
#ifndef KHOPPER_CODEC_BASE_HPP
#define KHOPPER_CODEC_BASE_HPP

#include "os.hpp"
#include "error.hpp"

#include <QPluginLoader>

#include <deque>
#include <string>

namespace khopper {

	/// @defgroup Codecs De/Muxers and De/Encoders
	// @{
	// @}
	/**
	 * @brief Contains Codecs module
	 * @ingroup Codecs
	 */
	namespace codec {

		/**
		 * @brief Used for storing raw binary data
		 * @ingroup Codecs
		 */
		typedef std::deque< char > ByteArray;

		/**
		 * @brief Codec error
		 * @ingroup Codecs
		 */
		class Codec {
		};

	}

	namespace plugin {

		template< typename Creator >
		class CreatorLoader {
		public:
			CreatorLoader( const std::string & plugin ) : plugin_( plugin ) {}
			Creator * operator()() {
				plugin::PluginContext pc;
				Creator * c = qobject_cast< Creator * >( pc.load( this->plugin_.c_str() ) );
				if( !c ) {
					throw Error< RunTime >( "Invalid plugin!" );
				}
				return c;
			}
		private:
			std::string plugin_;
		};

	}

}

#endif
