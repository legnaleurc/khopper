/**
 * @file abstractwritercreator.hpp
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
#ifndef KHOPPER_PLUGIN_ABSTRACTWRITERCREATOR_HPP
#define KHOPPER_PLUGIN_ABSTRACTWRITERCREATOR_HPP

#include "abstractcreator.hpp"
#include "codec/abstractwriter.hpp"

#include <QtPlugin>

namespace khopper {

	namespace plugin {

		/**
		 * @ingroup Plugins
		 * @brief Wirter creator interface
		 * @sa private_::AbstractCreator
		 */
		typedef private_::AbstractCreator< codec::AbstractWriter > AbstractWriterCreator;

		/**
		 * @ingroup Plugins
		 * @brief Register plugin to WriterFactory
		 * @param key the key used in program
		 * @param name plugin name
		 * @return if registered in factory
		 */
		KHOPPER_EXPORT bool registerWriter( const std::string & key, const std::string & name );
		/**
		 * @ingroup Plugins
		 * @brief Create writer plugin
		 * @param key format key
		 */
		KHOPPER_EXPORT codec::WriterSP createWriter( const std::string & key );

	}

}

Q_DECLARE_INTERFACE( khopper::plugin::AbstractWriterCreator, "org.FoolproofProject.Khopper.Plugin.Writer/0.2" )

#endif
