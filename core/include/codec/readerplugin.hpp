/**
 * @file readerplugin.hpp
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
#ifndef KHOPPER_READERPLUGIN_HPP
#define KHOPPER_READERPLUGIN_HPP

#include "plugin_base.hpp"
#include "abstractreader.hpp"

#include <QtPlugin>

namespace khopper {

	namespace plugin {

		/**
		 * @brief Reader creator interface
		 * @ingroup Plugins
		 */
		typedef Creator< codec::AbstractReader > ReaderCreator;

		/**
		 * @brief Register plugin to factory
		 * @param key the key used in program
		 * @param plugin plugin identifier
		 * @return if registered in factory
		 */
		bool registerReader( const std::string & key, const std::string & plugin );
		/**
		 * @brief Create reader
		 * @param key format key
		 */
		codec::ReaderSP createReader( const std::string & key );

	}

}

Q_DECLARE_INTERFACE( khopper::plugin::ReaderCreator, "org.FoolproofProject.Khopper.Plugin.Reader/0.2" )

#endif
