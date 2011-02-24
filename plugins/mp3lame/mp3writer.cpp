/**
 * @file mp3writer.cpp
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
#include "mp3writer.hpp"

#include "khopper/error.hpp"

#include <id3v2tag.h>

#include <QtCore/QtDebug>

using namespace khopper::codec;
using khopper::error::IOError;

Mp3Writer::Mp3Writer( const QUrl & uri ):
AbstractWriter( uri ),
gfp_(),
quality_( -1 ),
id3v2Offset_( -1L ) {
}

void Mp3Writer::doOpen() {
	// no remote support
	if( this->getURI().scheme() != "file" ) {
		throw IOError( tr( "This plugin do not support remote writing." ) );
	}
	qDebug() << "Mp3Writer: uri" << this->getURI() << "local" << this->getURI().toLocalFile();

	// open file
	FILE * fout = NULL;
	int ret = 0;
#ifdef Q_OS_WIN32
	ret = _wfopen_s( &fout, this->getURI().toLocalFile().toStdWString().c_str(), L"wb" );
	if( ret != 0 ) {
		throw khopper::error::IOError( ret );
	}
#else
	fout = fopen( this->getURI().toLocalFile().toStdString().c_str(), "wb" );
	if( fout == NULL ) {
		throw IOError( QString( "Can not open: %1" ).arg( this->getURI().toString() ) );
	}
#endif

	this->fout_.reset( fout, fclose );

	// lame encoder setting
	this->gfp_.reset( lame_init(), lame_close );
	lame_set_num_channels( this->gfp_.get(), this->getAudioFormat().channels() );
	lame_set_in_samplerate( this->gfp_.get(), this->getAudioFormat().frequency() );
	if( this->quality_ < 0 ) {
		lame_set_brate( this->gfp_.get(), this->getBitRate() / 1000 );
		lame_set_bWriteVbrTag( this->gfp_.get(), 0 );
	} else {
		lame_set_VBR( this->gfp_.get(), vbr_default );
		lame_set_VBR_q( this->gfp_.get(), this->quality_ );
		lame_set_bWriteVbrTag( this->gfp_.get(), 1 );
	}
	lame_set_mode( this->gfp_.get(), JOINT_STEREO );
	lame_set_quality( this->gfp_.get(), 2 );

	// ID3v2 tag setting
	lame_set_write_id3tag_automatic( this->gfp_.get(), 0 );

	TagLib::ID3v2::Tag tag;
	tag.setTitle( TagLib::String( this->getTitle().constData(), TagLib::String::UTF8 ) );
	tag.setArtist( TagLib::String( this->getArtist().constData(), TagLib::String::UTF8 ) );
	tag.setAlbum( TagLib::String( this->getAlbum().constData(), TagLib::String::UTF8 ) );

	TagLib::ByteVector id3v2 = tag.render();
	qDebug( "ID3v2 length: %d", id3v2.size() );
	this->id3v2Offset_ = id3v2.size();
	fwrite( id3v2.data(), sizeof( id3v2[0] ), id3v2.size(), this->fout_.get() );

	// initialize all parameters
	ret = lame_init_params( this->gfp_.get() );
	if( ret < 0 ) {
		qDebug( "lame param error" );
	}
}

void Mp3Writer::writeFrame( const QByteArray & sample ) {
	if( !sample.isEmpty() ) {
		short int * audio = static_cast< short int * >( const_cast< void * >( static_cast< const void * >( sample.data() ) ) );
		// FIXME: watch out! should check sample format;
		const int nSamples = sample.size() / sizeof( short int ) / this->getAudioFormat().channels();
		std::vector< unsigned char > buffer( 1.25 * nSamples + 7200 );

		int ret = 0;
		if( this->getAudioFormat().channels() == 1 ) {
			ret = lame_encode_buffer(
				this->gfp_.get(),
				audio,
				audio,
				nSamples,
				&buffer[0],
				buffer.size()
			);
		} else {
			ret = lame_encode_buffer_interleaved(
				this->gfp_.get(),
				audio,
				nSamples,
				&buffer[0],
				buffer.size()
			);
		}

		if( ret < 0 ) {
			qDebug( "lame encode error: %d", ret );
		}
		fwrite( &buffer[0], sizeof( buffer[0] ), ret, this->fout_.get() );
	} else {
		unsigned char buffer[7200];
		int ret = lame_encode_flush_nogap(
			this->gfp_.get(),
			buffer,
			7200
		);
		fwrite( buffer, sizeof( buffer[0] ), ret, this->fout_.get() );
	}
}

void Mp3Writer::doClose() {
	std::vector< unsigned char > buffer( 7200 );
	int ret = lame_encode_flush(
		this->gfp_.get(),
		&buffer[0],
		buffer.size()
	);
	if( ret < 0 ) {
		qDebug( "lame flush error" );
	}
	fwrite( &buffer[0], sizeof( char ), ret, this->fout_.get() );

	ret = lame_get_lametag_frame( this->gfp_.get(), &buffer[0], buffer.size() );
	if( ret > 0 ) {
		fseek( this->fout_.get(), this->id3v2Offset_, SEEK_SET );
		fwrite( &buffer[0], sizeof( char ), ret, this->fout_.get() );
	}

	this->id3v2Offset_ = -1L;
	this->quality_ = -1;
	this->gfp_.reset();
	this->fout_.reset();
}
