/**
 * @file applicationprivate.hpp
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
#ifndef KHOPPER_APPLICATIONPRIVATE_HPP
#define KHOPPER_APPLICATIONPRIVATE_HPP

#include "abstractreader.hpp"
#include "application.hpp"
#include "factory.hpp"
#include "playlist.hpp"

namespace khopper {

	namespace plugin {
		class PluginManager;
	}

	template< typename KeyType, typename CreatorType >
	class FactoryError {
	public:
		CreatorType onError( const KeyType & /*key*/ ) const {
			return []( const KeyType & /*key*/ )->typename CreatorType::result_type {
				throw error::RunTimeError( "Find no suitable codec." );
			};
		}
	};

	class ApplicationPrivate {
	public:
		static ApplicationPrivate * self;

		ApplicationPrivate();

		std::shared_ptr< plugin::PluginManager > pm;
		Factory< QString, QUrl, codec::ReaderSP, FactoryError > readerFactory;
		Factory< QString, QUrl, album::PlayList, FactoryError > playlistFactory;
	};

}

#endif
