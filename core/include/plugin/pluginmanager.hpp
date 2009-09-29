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

#include "plugin/abstractcreator.hpp"
#include "util/error.hpp"

#include <QPluginLoader>
#include <QDir>

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

		namespace private_ {

			class KHOPPER_EXPORT PluginManager {
			public:
				PluginManager();
				void reloadPlugins();
				QObject * getPluginInstance( const QString & name ) const;
				const std::list< AbstractPanel * > & getPanels() const {
					return this->loadedPanels_;
				}

			private:
				std::map< std::string, QObject * > loadedPlugins_;
				std::list< AbstractPanel * > loadedPanels_;
			};

		}

		typedef Loki::SingletonHolder<
			private_::PluginManager,
			Loki::CreateUsingNew,
			Loki::DefaultLifetime,
			Loki::ClassLevelLockable
		> PluginManager;

		namespace private_ {

			/**
			 * @ingroup Plugins
			 * @brief From creator functor
			 * @tparam ProductCreator creator type
			 */
			template< typename ProductCreator >
			class CreatorLoader {
			public:
				/**
				 * @brief Constructor
				 * @param plugin The name of plugin
				 */
				CreatorLoader( const std::string & plugin ) : plugin_( plugin ) {}
				/**
				 * @brief Plugin loader
				 * @return The product creator
				 * @throws Error<RunTime> Can't open plugin or load error
				 *
				 * The returned resource is managed by Qt Plugin System,
				 * so don't worry about it's life time.
				 */
				ProductCreator * operator()() {
					ProductCreator * c = qobject_cast< ProductCreator * >( plugin::PluginManager::Instance().getPluginInstance( this->plugin_.c_str() ) );
					if( !c ) {
						throw error::RunTimeError( "Invalid plugin!" );
					}
					return c;
				}
			private:
				std::string plugin_;
			};

			/**
			 * @ingroup Plugins
			 * @brief Register key and name of plugin
			 * @tparam Product The product of Creator
			 * @tparam CreatorFactory The factory of Creator
			 * @param key the key used in program
			 * @param name plugin name
			 * @return If it registered sucessfully
			 */
			template< typename Product, typename CreatorFactory >
			bool registerProduct( const std::string & key, const std::string & name ) {
				return CreatorFactory::Instance().Register( key, CreatorLoader< AbstractCreator< Product > >( name ) );
			}

			/**
			 * @ingroup Plugins
			 * @brief Create plugin product
			 * @tparam Product The product of Creator
			 * @tparam CreatorFactory The factory of Creator
			 * @param key format key
			 * @return Smart pointer of Product
			 * @throws RunTimeError Can not load any plugin
			 */
			template< typename Product, typename CreatorFactory >
			std::tr1::shared_ptr< Product > createProduct( const std::string & key ) {
				return CreatorFactory::Instance().CreateObject( key )->create();
			}

		}

	}

}

#endif
