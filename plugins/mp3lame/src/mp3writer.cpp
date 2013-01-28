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

#include <id3v2tag.h>

#include <QtCore/QFile>
#include <QtCore/QtDebug>

#include "khopper/error.hpp"

using namespace khopper::codec;
using khopper::error::IOError;

Mp3Writer::Mp3Writer( const QUrl & uri ):
AbstractWriter( uri ),
out_( NULL ),
gfp_(),
quality_( -1 ),
id3v2Offset_( -1L ) {
	// no remote support
	if( this->getURI().scheme() != "file" ) {
		throw IOError( QObject::tr( "This plugin do not support remote writing." ) );
	}

	// setup file
	this->out_ = new QFile( this->getURI().toLocalFile(), this );
}

void Mp3Writer::setVBRQuality( int quality ) {
	this->quality_ = quality;
}

void Mp3Writer::doOpen() {
	if( !this->out_->open( QIODevice::WriteOnly ) ) {
		throw IOError( QObject::tr( "%1 (%2)" ).arg( this->out_->errorString() ).arg( this->getURI().toString() ) );
	}

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
	this->out_->write( id3v2.data(), id3v2.size() );

	// initialize all parameters
	int ret = lame_init_params( this->gfp_.get() );
	if( ret < 0 ) {
		qDebug( "lame param error" );
	}
}

qint64 Mp3Writer::writeData( const char * data, qint64 len ) {
	if( data && len > 0 ) {
		short int * audio = static_cast< short int * >( const_cast< void * >( static_cast< const void * >( data ) ) );
		// FIXME: watch out! should check sample format;
		const qint64 nSamples = len / sizeof( short int ) / this->getAudioFormat().channels();
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
		this->out_->write( static_cast< char * >( static_cast< void * >( &buffer[0] ) ), ret );
	} else {
		unsigned char buffer[7200];
		int ret = lame_encode_flush_nogap(
			this->gfp_.get(),
			buffer,
			7200
		);
		this->out_->write( static_cast< char * >( static_cast< void * >( &buffer[0] ) ), ret );
	}

	return len;
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
	this->out_->write( static_cast< char * >( static_cast< void * >( &buffer[0] ) ), ret );

	ret = lame_get_lametag_frame( this->gfp_.get(), &buffer[0], buffer.size() );
	if( ret > 0 ) {
		this->out_->seek( this->id3v2Offset_ );
		this->out_->write( static_cast< char * >( static_cast< void * >( &buffer[0] ) ), ret );
	}

	this->id3v2Offset_ = -1L;
	this->quality_ = -1;
	this->gfp_.reset();
	this->out_->close();
}
