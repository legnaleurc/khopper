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

#include <loki/Factory.h>
#include <loki/Singleton.h>

namespace khopper {

	namespace plugin {

		class ReaderCreator {
		public:
			virtual codec::ReaderSP create() const = 0;
		};

		/**
		 * @brief The audio reader factory
		 * @ingroup Codecs
		 */
		typedef Loki::SingletonHolder<
			Loki::Factory<
				ReaderCreator,
				std::string
			>,
			Loki::CreateUsingNew,
			Loki::LongevityLifetime::DieAsSmallObjectChild
		> ReaderFactory;

		bool registerReader( const std::string & key, const std::string & plugin );
		codec::ReaderSP createReader( const std::string & key );

	}

}

Q_DECLARE_INTERFACE( khopper::plugin::ReaderCreator, "org.FoolproofProject.Khopper.Plugin.Reader/0.2" )

#endif
