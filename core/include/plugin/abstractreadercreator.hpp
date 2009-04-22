/**
 * @file abstractreadercreator.hpp
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
		 */
		typedef AbstractCreator< codec::AbstractReader > AbstractReaderCreator;

		/**
		 * @ingroup Plugins
		 * @brief Register plugin to factory
		 * @param key the key used in program
		 * @param name plugin name
		 * @return if registered in factory
		 */
		KHOPPER_DLL_EXPORT bool registerReader( const std::string & key, const std::string & name );
		/**
		 * @ingroup Plugins
		 * @brief Create reader
		 * @param key format key
		 */
		KHOPPER_DLL_EXPORT codec::ReaderSP createReader( const std::string & key );

	}

}

Q_DECLARE_INTERFACE( khopper::plugin::AbstractReaderCreator, "org.FoolproofProject.Khopper.Plugin.Reader/0.2" )

#endif
