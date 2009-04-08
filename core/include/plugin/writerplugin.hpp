/**
 * @file writerplugin.hpp
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
#ifndef KHOPPER_WRITERPLUGIN_HPP
#define KHOPPER_WRITERPLUGIN_HPP

#include "creator.hpp"
#include "plugin/abstractwriter.hpp"

#include <QtPlugin>

namespace khopper {

	namespace plugin {

		/**
		 * @brief Wirter creator interface
		 */
		typedef Creator< codec::AbstractWriter > WriterCreator;

		/**
		 * @brief Register plugin to factory
		 * @param key the key used in program
		 * @param plugin plugin identifier
		 * @return if registered in factory
		 */
		bool registerWriter( const std::string & key, const std::string & plugin );
		/**
		 * @brief Create writer
		 * @param key format key
		 */
		codec::WriterSP createWriter( const std::string & key );

	}

}

Q_DECLARE_INTERFACE( khopper::plugin::WriterCreator, "org.FoolproofProject.Khopper.Plugin.Writer/0.2" )

#endif
