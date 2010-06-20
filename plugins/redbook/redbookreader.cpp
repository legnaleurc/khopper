/**
 * @file redbookreader.cpp
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
#include "redbookreader.hpp"

using namespace khopper::codec;

RedbookReader::RedbookReader( const QUrl & uri ):
AbstractReader( uri ),
file_( uri.toLocalFile() ) {
	QAudioFormat format;
	format.setByteOrder( QAudioFormat::LittleEndian );
	format.setChannels( 2 );
	format.setFrequency( 44100 );
	format.setSampleSize( 2 );
	format.setSampleType( QAudioFormat::SignedInt );
	this->setAudioFormat( format );
}

bool RedbookReader::atEnd() const {
	return this->file_.atEnd();
}

bool RedbookReader::seek( qint64 pos ) {
	bool ret = this->AbstractReader::seek( pos );
	ret &= this->file_.seek( pos );
	return ret;
}

qint64 RedbookReader::size() const {
	return this->file_.size();
}

void RedbookReader::doOpen() {
	this->file_.open( QIODevice::ReadOnly );
	this->setDuration( this->file_.size() * 1000 / 44100 / 2 / 2 );
}

void khopper::codec::RedbookReader::doClose() {
	this->file_.close();
}

qint64 RedbookReader::readData( char * data, qint64 maxSize ) {
	return this->file_.read( data, maxSize );
}

