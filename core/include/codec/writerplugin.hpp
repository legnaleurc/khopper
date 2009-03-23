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

#include "plugin_base.hpp"
#include "abstractwriter.hpp"

#include <loki/Factory.h>
#include <loki/Singleton.h>

namespace khopper {

	namespace plugin {

		class WriterCreator {
		public:
			virtual codec::WriterSP create() const = 0;
		};

		/**
		 * @brief The audio writer factory
		 * @ingroup Codecs
		 */
		typedef Loki::SingletonHolder<
			Loki::Factory<
				WriterCreator,
				std::string
			>,
			Loki::CreateUsingNew,
			Loki::LongevityLifetime::DieAsSmallObjectChild
		> WriterFactory;


		bool registerWriter( const std::string & key, const std::string & plugin );
		codec::WriterSP createWriter( const std::string & key );

	}

}

Q_DECLARE_INTERFACE( khopper::plugin::WriterCreator, "org.FoolproofProject.Khopper.Plugin.Writer/0.2" )

#endif
