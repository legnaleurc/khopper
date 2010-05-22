/**
 * @file pluginmanager.hpp
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
#ifndef KHOPPER_PLUGIN_PLUGINMANAGER_HPP
#define KHOPPER_PLUGIN_PLUGINMANAGER_HPP

#include "error.hpp"

#include <QtCore/QDir>

#ifndef LOKI_CLASS_LEVEL_THREADING
# define LOKI_CLASS_LEVEL_THREADING
#endif

#include <loki/Singleton.h>

#include <string>
#include <list>
#include <map>

namespace khopper {

	namespace plugin {

		class AbstractPanel;
		class AbstractPlugin;

		/**
		 * @ingroup Plugins
		 * @brief Private plugin manager
		 */
		class PluginManager {
		public:
			/// default constructor
			PluginManager();

			/**
			 * @brief reload all plugins
			 *
			 * Will discard duplicated plugins.
			 */
			void reloadPlugins();

			const std::list< QDir > & getSearchPaths() const {
				return this->searchPaths_;
			}
			/// get plugin instance by name
			AbstractPlugin * getPluginInstance( const QString & name ) const;

		private:
			QStringList getPluginFiles_() const;

			std::list< QDir > searchPaths_;
			std::map< std::string, AbstractPlugin * > loadedPlugins_;
		};

	}

}

#endif
