/**
 * @file plugin_impl.hpp
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
#ifndef KHOPPER_PLUGIN_IMPL_HPP
#define KHOPPER_PLUGIN_IMPL_HPP

#include "plugin/plugin_base.hpp"
#include "common/os.hpp"
#include "common/error.hpp"

#include <QPluginLoader>

#include <string>

namespace khopper {

	namespace plugin {

		/**
		 * @brief From creator functor
		 * @tparam ProductCreator creator type
		 * @ingroup Plugins
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
				plugin::PluginContext pc;
				ProductCreator * c = qobject_cast< ProductCreator * >( pc.load( this->plugin_.c_str() ) );
				if( !c ) {
					throw error::RunTimeError( "Invalid plugin!" );
				}
				return c;
			}
		private:
			std::string plugin_;
		};

		/**
		 * @brief Register key and name of plugin
		 * @tparam Product The product of Creator
		 * @tparam CreatorFactory The factory of Creator
		 * @return If it registered sucessfully
		 */
		template< typename Product, typename CreatorFactory >
		bool registerProduct( const std::string & key, const std::string & plugin ) {
			return CreatorFactory::Instance().Register( key, CreatorLoader< Creator< Product > >( plugin ) );
		}

		/**
		 * @brief Create plugin product
		 * @tparam Product The product of Creator
		 * @tparam CreatorFactory The factory of Creator
		 * @return Smart pointer of Product
		 * @throws RunTimeError Can not load any plugin
		 */
		template< typename Product, typename CreatorFactory >
		std::tr1::shared_ptr< Product > createProduct( const std::string & key ) {
			Creator< Product > * tmp = NULL;
			try {
				tmp = CreatorFactory::Instance().CreateObject( key );
			} catch( std::exception & e ) {
				try {
					tmp = CreatorFactory::Instance().CreateObject( "*" );
				} catch( std::exception & e ) {
					throw error::RunTimeError( "Found no plugin!" );
				}
			}
			return tmp->create();
		}

	}

}

#endif
