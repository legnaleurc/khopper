/**
 * @file abstractreaderprivate.hpp
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
#ifndef KHOPPER_CODEC_ABSTRACTREADERPRIVATE_HPP
#define KHOPPER_CODEC_ABSTRACTREADERPRIVATE_HPP

#include "abstractreader.hpp"

#include <loki/Singleton.h>

#include <list>

namespace khopper {

	namespace codec {

		struct AbstractReader::AbstractReaderPrivate {
			AbstractReaderPrivate( const QUrl & uri );

			QByteArray album;
			QByteArray artist;
			unsigned int bitRate;
			QDataStream buffer;
			ChannelLayout channelLayout;
			unsigned int channels;
			QByteArray comment;
			QByteArray copyright;
			int64_t msDuration;
			QUrl uri;
			QByteArray genre;
			unsigned int index;
			SampleFormat sampleFormat;
			unsigned int sampleRate;
			QByteArray title;
			QString year;
		};

	}

	namespace plugin {

		struct ReaderFactoryPrivate {
			typedef std::pair< ReaderVerifier, ReaderCreator > Pair;
			std::list< Pair > l;
		};

		typedef Loki::SingletonHolder<
			ReaderFactoryPrivate
		> ReaderFactory;

	}

}

#endif
