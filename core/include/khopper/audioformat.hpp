/**
 * @file audioformat.hpp
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
#ifndef KHOPPER_CODEC_AUDIOFORMAT_HPP
#define KHOPPER_CODEC_AUDIOFORMAT_HPP

#include <memory>

#include <QtCore/QSysInfo>

#include "config.hpp"
#include "codecfwd.hpp"

namespace khopper {
namespace codec {

class KHOPPER_DLL AudioFormat {
public:
	enum Endian {
		BigEndian = QSysInfo::BigEndian,
		LittleEndian = QSysInfo::LittleEndian
	};
	enum SampleType {
		Unknown,
		SignedInt,
		UnSignedInt,
		Float
	};

	AudioFormat();
	AudioFormat( const AudioFormat & that );

	Endian byteOrder() const;
	int channels() const;
	QString codec() const;
	int frequency() const;
	bool isValid() const;
	int sampleSize() const;
	SampleType sampleType() const;
	void setByteOrder( Endian byteOrder );
	void setChannels( int channels );
	void setCodec( const QString & codec );
	void setFrequency( int frequency );
	void setSampleSize( int sampleSize );
	void setSampleType( SampleType sampleType );

	bool operator !=( const AudioFormat & that ) const;
	AudioFormat & operator =( const AudioFormat & that );
	bool operator ==( const AudioFormat & that ) const;

private:
	class Private;
	std::shared_ptr< Private > p_;
};

}
}

#endif
