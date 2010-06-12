/**
 * @file wavwrapper.hpp
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
#ifndef KHOPPER_WIDGET_WAVWRAPPER_HPP
#define KHOPPER_WIDGET_WAVWRAPPER_HPP

#include <QIODevice>

#include "khopper/abstractreader.hpp"

namespace khopper {
	namespace widget {

		class WavWrapper : public QIODevice {
		public:
			explicit WavWrapper( codec::ReaderSP reader );

			virtual bool atEnd() const;
			virtual bool open( OpenMode mode );
			virtual void close();
			virtual bool seek( qint64 pos );
			virtual qint64 size() const;

		protected:
			virtual qint64 readData( char * data, qint64 maxlen );
			virtual qint64 writeData( const char * data, qint64 maxlen );

		private:
			codec::ReaderSP reader_;
			QByteArray header_;
		};

	}
}

#endif
