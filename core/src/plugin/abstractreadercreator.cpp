/**
 * @file abstractreadercreator.cpp
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
#include "plugin/abstractreadercreator.hpp"
#include "plugin/plugincontext.hpp"
#include "codec/defaultreader.hpp"

#ifndef LOKI_CLASS_LEVEL_THREADING
# define LOKI_CLASS_LEVEL_THREADING
#endif

#include <loki/Factory.h>
#include <loki/Singleton.h>

namespace khopper {

	namespace plugin {

		/**
		 * @brief The audio reader factory
		 * @ingroup Plugins
		 */
		typedef Loki::SingletonHolder<
			Loki::Factory<
				AbstractReaderCreator,
				std::string
			>,
			Loki::CreateUsingNew,
			Loki::LongevityLifetime::DieAsSmallObjectChild,
			Loki::ClassLevelLockable
		> ReaderFactory;

		bool registerReader( const std::string & key, const std::string & name ) {
			return private_::registerProduct< codec::AbstractReader, ReaderFactory >( key, name );
		}

		codec::ReaderSP createReader( const std::string & key ) {
			try {
				return private_::createProduct< codec::AbstractReader, ReaderFactory >( key );
			} catch( std::exception & ) {
				return codec::ReaderSP( new codec::DefaultReader );
			}
		}

	}

}
