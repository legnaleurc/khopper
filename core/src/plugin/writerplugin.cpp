/**
 * @file writerplugin.cpp
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
#include "plugin/writerplugin.hpp"
#include "plugin_impl.hpp"

#include <loki/Factory.h>
#include <loki/Singleton.h>

namespace khopper {

	namespace plugin {

		/**
		 * @brief The audio writer factory
		 * @ingroup Plugins
		 */
		typedef Loki::SingletonHolder<
			Loki::Factory<
				WriterCreator,
				std::string
			>,
			Loki::CreateUsingNew,
			Loki::LongevityLifetime::DieAsSmallObjectChild,
			Loki::ClassLevelLockable
		> WriterFactory;

		bool registerWriter( const std::string & key, const std::string & name ) {
			return registerProduct< codec::AbstractWriter, WriterFactory >( key, name );
		}

		codec::WriterSP createWriter( const std::string & key ) {
			return createProduct< codec::AbstractWriter, WriterFactory >( key );
		}

	}

}
