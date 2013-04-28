/**
 * @file wavwrapper.cpp
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
#include "wavwrapper.hpp"

#include <cstring>

#include "khopper/codecerror.hpp"

using khopper::utility::WavWrapper;
using khopper::codec::ReaderSP;
using khopper::error::CodecError;

WavWrapper::WavWrapper( ReaderSP reader ):
QIODevice(),
reader_( reader ),
header_( "RIFF____WAVEfmt ____________________data____", 44 ) {
}

bool WavWrapper::atEnd() const {
	return this->QIODevice::atEnd() || this->reader_->atEnd();
}

void WavWrapper::close() {
	this->QIODevice::close();
	this->reader_->close();
}

bool WavWrapper::isSequential() const {
	return this->reader_->isSequential();
}

bool WavWrapper::open( OpenMode mode ) {
	bool ret = this->QIODevice::open( mode );
	ret &= this->reader_->open( mode );
	const codec::AudioFormat & format( this->reader_->getAudioFormat() );

	qint32 dword;
	qint16 word;
	dword = this->reader_->size() + 36;
	std::memcpy( this->header_.data() + 4, &dword, sizeof( dword ) );
	dword = 16;
	std::memcpy( this->header_.data() + 16, &dword, sizeof( dword ) );
	word = 1;
	std::memcpy( this->header_.data() + 20, &word, sizeof( word ) );
	word = format.channels();
	std::memcpy( this->header_.data() + 22, &word, sizeof( word ) );
	dword = format.frequency();
	std::memcpy( this->header_.data() + 24, &dword, sizeof( dword ) );
	dword = format.frequency() * format.channels() * format.sampleSize() / 8;
	std::memcpy( this->header_.data() + 28, &dword, sizeof( dword ) );
	word = format.channels() * format.sampleSize() / 8;
	std::memcpy( this->header_.data() + 32, &word, sizeof( word ) );
	word = format.sampleSize();
	std::memcpy( this->header_.data() + 34, &word, sizeof( word ) );
	dword = this->reader_->size();
	std::memcpy( this->header_.data() + 40, &dword, sizeof( dword ) );

	return ret;
}

bool WavWrapper::seek( qint64 pos ) {
	bool ret = this->QIODevice::seek( pos );
	if( pos < 44 ) {
		ret &= this->reader_->seek( 0LL );
	} else {
		ret &= this->reader_->seek( pos - 44LL );
	}
	return ret;
}

qint64 WavWrapper::size() const {
	return this->reader_->size() + 44LL;
}

qint64 WavWrapper::readData( char * data, qint64 maxlen ) {
	if( this->reader_->atEnd() && !this->QIODevice::atEnd() ) {
		// NOTE hack for PCM fragment
		const qint64 len = std::min( maxlen, this->size() - this->pos() );
		if( len < 0LL ) {
			return -1LL;
		}
		std::fill( data, data + len, '\0' );
		return len;
	} else if( this->pos() < 44LL ) {
		const qint64 len = 44LL - this->pos();
		std::memcpy( data, this->header_.constData() + this->pos(), len );
		try {
			return this->reader_->read( data + len, maxlen - len ) + len;
		} catch( CodecError & e ) {
			this->setErrorString( e.getMessage() );
			return -1;
		}
	} else {
		try {
			return this->reader_->read( data, maxlen );
		} catch( CodecError & e ) {
			this->setErrorString( e.getMessage() );
			return -1;
		}
	}
}

qint64 WavWrapper::writeData( const char * /*data*/, qint64 /*maxlen*/ ) {
	return -1;
}
