/**
 * @file youtubereader.hpp
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
#ifndef KHOPPER_CODEC_YOUTUBEREADER_HPP
#define KHOPPER_CODEC_YOUTUBEREADER_HPP

#include "khopper/abstractreader.hpp"

namespace khopper {
	namespace codec {

		class YouTubeReader : public AbstractReader {
		public:
			explicit YouTubeReader( const QUrl & uri );

			virtual bool atEnd() const;
			virtual bool seek( qint64 pos );
			virtual qint64 size() const;

		protected:
			virtual void doOpen();
			virtual void doClose();
			virtual qint64 readData( char * data, qint64 maxSize );

		private:
			ReaderSP client_;
		};

	}
}

#endif
