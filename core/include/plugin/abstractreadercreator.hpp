/**
 * @file abstractreadercreator.hpp
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
#ifndef KHOPPER_PLUGIN_ABSTRACTREADERCREATOR_HPP
#define KHOPPER_PLUGIN_ABSTRACTREADERCREATOR_HPP

#include "plugin/abstractplugin.hpp"
#include "codec/abstractreader.hpp"

#include <QtPlugin>

namespace khopper {

	namespace plugin {

		/**
		 * @ingroup Plugins
		 * @brief Common creator factory
		 * @tparam Product The product type of return
		 * @sa khopper::plugin::AbstractReaderCreator khopper::plugin::AbstractWriterCreator
		 *
		 * You will not use this class directly.
		 * Use khopper::plugin::AbstractReaderCreator or
		 * khopper::plugin::AbstractWriterCreator instead.
		 */
		class KHOPPER_EXPORT AbstractReaderCreator : public AbstractPlugin {
		public:
			AbstractReaderCreator();
			virtual ~AbstractReaderCreator();
			/**
			 * @brief Create the product
			 * @note never null
			 */
			codec::ReaderSP create() const;

		private:
			virtual codec::ReaderSP doCreate() const = 0;
			// no copy
			AbstractReaderCreator( const AbstractReaderCreator & );
			AbstractReaderCreator & operator =( const AbstractReaderCreator & );
		};

		/**
		 * @ingroup Plugins
		 * @brief Register plugin to factory
		 * @param key the key used in program
		 * @param name plugin name
		 * @return if registered in factory
		 */
		KHOPPER_EXPORT bool registerReader( const std::string & key, const std::string & name );
		/**
		 * @ingroup Plugins
		 * @brief Create reader
		 * @param key format key
		 * @return Smart pointer of Product
		 * @throws RunTimeError Can not load any plugin
		 */
		KHOPPER_EXPORT codec::ReaderSP createReader( const std::string & key );

	}

}

Q_DECLARE_INTERFACE( khopper::plugin::AbstractReaderCreator, "org.FoolproofProject.Khopper.Plugin.Reader/0.2" )

#endif
