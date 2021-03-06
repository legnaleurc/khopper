/**
 * @file rangedreader.hpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * This plug-in is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This plug-in is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KHOPPER_CODEC_RANGEDREADER_HPP
#define KHOPPER_CODEC_RANGEDREADER_HPP

#include "khopper/reader.hpp"

namespace khopper {
namespace codec {

class RangedReader: public Reader {
public:
	RangedReader( const QUrl & uri, qint64 msBegin, qint64 msDuration );

	virtual bool atEnd() const;
	bool seek( qint64 pos );
	virtual qint64 size() const;

protected:
	virtual void doOpen();
	virtual void doClose();
	virtual qint64 readData( char * data, qint64 maxSize );

private:
	ReaderSP client_;
	qint64 msBegin_;
	qint64 msDuration_;
};

}
}

#endif
