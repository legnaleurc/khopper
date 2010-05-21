/**
 * @file abstractreadercreator.cpp
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
#include "plugin/abstractreadercreator.hpp"
#include "application.hpp"
#include "codec/defaultreader.hpp"

#ifndef LOKI_CLASS_LEVEL_THREADING
# define LOKI_CLASS_LEVEL_THREADING
#endif

#include <loki/Factory.h>
#include <loki/Singleton.h>

using namespace khopper::plugin;
using khopper::codec::ReaderSP;
using khopper::codec::AbstractReader;
using khopper::codec::DefaultReader;
using khopper::error::RunTimeError;

AbstractReaderCreator::AbstractReaderCreator() {
}

AbstractReaderCreator::~AbstractReaderCreator() {
}

ReaderSP AbstractReaderCreator::create() const {
	ReaderSP pointer;
	try {
		pointer = this->doCreate();
	} catch( ... ) {
		// FIXME: compile error, don't know why
		// assert( !"memory allocation failed" );
		pointer.reset();
	}
	assert( pointer || typeid( *pointer ) == typeid( AbstractReader ) || !"got null pointer" );
	return pointer;
}

/**
 * @brief The audio reader factory
 * @ingroup Plugins
 */
typedef Loki::SingletonHolder<
	Loki::Factory<
		AbstractReaderCreator,
		std::string
	>,
	Loki::CreateUsingNew,
	Loki::LongevityLifetime::DieAsSmallObjectChild,
	Loki::ClassLevelLockable
> ReaderFactory;

/**
 * @ingroup Plugins
 * @brief From creator functor
 * @tparam ProductCreator creator type
 */
class CreatorLoader {
public:
	/**
	 * @brief Constructor
	 * @param pluginID The ID of plugin
	 */
	CreatorLoader( const std::string & pluginID ) : pluginID_( pluginID ) {}
	/**
	 * @brief Plugin loader
	 * @return The product creator
	 * @throws Error<RunTime> Can't open plugin or load error
	 *
	 * The returned resource is managed by Qt Plugin System,
	 * so don't worry about it's life time.
	 */
	AbstractReaderCreator * operator()() {
		AbstractReaderCreator * c = dynamic_cast< AbstractReaderCreator * >( KHOPPER_APPLICATION->getPluginInstance( this->pluginID_.c_str() ) );
		if( !c ) {
			throw RunTimeError( "Invalid plugin!" );
		}
		return c;
	}
private:
	std::string pluginID_;
};

bool khopper::plugin::registerReader( const std::string & key, const std::string & name ) {
	return ReaderFactory::Instance().Register( key, CreatorLoader( name ) );
}

ReaderSP khopper::plugin::createReader( const std::string & key ) {
	try {
		return ReaderFactory::Instance().CreateObject( key )->create();
	} catch( std::exception & ) {
		return ReaderSP( new DefaultReader );
	}
}
