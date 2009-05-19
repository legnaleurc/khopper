/**
 * @file abstractreadercreator.hpp
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
#ifndef KHOPPER_PLUGIN_ABSTRACTREADERCREATOR_HPP
#define KHOPPER_PLUGIN_ABSTRACTREADERCREATOR_HPP

#include "abstractcreator.hpp"
#include "codec/abstractreader.hpp"

#include <QtPlugin>

namespace khopper {

	namespace plugin {

		/**
		 * @ingroup Plugins
		 * @brief Reader creator interface
		 * @sa private_::AbstractCreator
		 */
		typedef private_::AbstractCreator< codec::AbstractReader > AbstractReaderCreator;

		/**
		 * @ingroup Plugins
		 * @brief Register plugin to factory
		 * @param key the key used in program
		 * @param name plugin name
		 * @return if registered in factory
		 */
		KHOPPER_EXPORT bool registerReader( const std::string & key, const std::string & name );
		/**
		 * @ingroup Plugins
		 * @brief Create reader
		 * @param key format key
		 */
		KHOPPER_EXPORT codec::ReaderSP createReader( const std::string & key );

	}

}

Q_DECLARE_INTERFACE( khopper::plugin::AbstractReaderCreator, "org.FoolproofProject.Khopper.Plugin.Reader/0.2" )

#endif
