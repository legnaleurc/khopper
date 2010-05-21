/**
 * @file abstractplugin.hpp
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
#ifndef KHOPPER_PLUGIN_ABSTRACTPLUGIN_HPP
#define KHOPPER_PLUGIN_ABSTRACTPLUGIN_HPP

#include "util/tr1.hpp"

#include <QtCore/QFileInfo>
#include <QtCore/QString>

namespace khopper {

	namespace plugin {

		/**
		 * @brief Interface of plugins
		 *
		 * This interface is used to define some properties of plugins.
		 * Clients usally need not to inherit this class, because
		 * AbstractPanel, AbstractReaderCreator and AbstractWriterCreator do so.
		 * Use them instead of this interface.
		 */
		class KHOPPER_DLL AbstractPlugin {
		public:
			AbstractPlugin();
			/// virtual destructor
			virtual ~AbstractPlugin();

			void install( const QFileInfo & fileInfo );
			void uninstall();
			bool isInstalled() const;

			/// get the id of the plugin, must be unique
			virtual QString getID() const = 0;
			/// get version string of the plugin
			virtual QString getVersion() const = 0;

		protected:
			virtual void doInstall( const QFileInfo & fileInfo ) = 0;
			virtual void doUninstall() = 0;

		private:
			bool installed_;
		};

	}

}

#endif
