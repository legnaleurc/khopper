/**
 * @file abstractcreator.hpp
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
#ifndef KHOPPER_PLUGIN_ABSTRATORCREATOR_HPP
#define KHOPPER_PLUGIN_ABSTRATORCREATOR_HPP

/**
 * @defgroup Plugins Plugin System
 *
 * This is Plugin System module, based on
 * <a href="http://doc.trolltech.com/4.5/plugins-howto.html">
 * Qt Plugin System</a>.
 * This module allows you to extend converting formats, decoders,
 * and encoders dynamicly.
 *
 * <h2>How to write plugins</h2>
 *
 * <h3>Output formats</h3>
 * Inherit khopper::plugin::AbstractPanel and override all pure virtual
 * functions. Please fallow the instructions in the
 * <a href="http://doc.trolltech.com/4.5/plugins-howto.html">
 * Qt Plugin System</a> page.
 *
 * <h3>Decoders</h3>
 * Inherit khopper::plugin::ReaderCreator, and override
 * khopper::plugin::ReaderCreator::create_, make it return the decoder which
 * you desired. \n
 * Then call khopper::plugin::registerReader in global scope to register plugin
 * to main program. Here is an example:
 * <pre>
 * const bool REGISTERED = khopper::plugin::registerReader( "mp3", "krp_mp3" );
 * </pre>
 * Which "mp3" means the key which khopper::plugin::createReader() will use.
 * "krp_mp3" is the name of plugin namely the plugin project target name.
 *
 * <h3>Encoder</h3>
 * Inherit khopper::plugin::WriterCreator, and override
 * khopper::plugin::WriterCreator::create_, make it return the encoder which
 * you desired. \n
 * Then call khopper::plugin::registerWriter in global scope to register plugin
 * to main program. Here is an example:
 * <pre>
 * const bool REGISTERED = khopper::plugin::registerWriter( "mp3", "krp_mp3" );
 * </pre>
 * Which "mp3" means the key which khopper::plugin::createWriter() will use.
 * "krp_mp3" is the name of plugin namely the plugin project target name.
 */
// @{
// @}

#include "util/tr1.hpp"

#include <cassert>

namespace khopper {

	/**
	 * @ingroup Plugins
	 * @brief Contains plugin utility
	 */
	namespace plugin {

		namespace private_ {

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
			template< typename Product >
			class KHOPPER_EXPORT AbstractCreator {
			public:
				/// Default constructor
				AbstractCreator() {}
				/// Destructor
				virtual ~AbstractCreator() {}
				/**
				 * @brief Create the product
				 * @note never null
				 */
				std::tr1::shared_ptr< Product > create() const {
					std::tr1::shared_ptr< Product > pointer;
					try {
						pointer = this->doCreate();
					} catch( ... ) {
						// FIXME: compile error, don't know why
						// assert( !"memory allocation failed" );
						pointer.reset();
					}
					assert( pointer || !"got null pointer" );
					return pointer;
				}
			private:
				virtual std::tr1::shared_ptr< Product > doCreate() const = 0;

				// no copy
				AbstractCreator( const AbstractCreator & );
				AbstractCreator & operator =( const AbstractCreator & );
			};

		}

	}

}

#endif
