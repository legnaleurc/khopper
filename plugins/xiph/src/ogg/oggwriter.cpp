/**
 * @file oggwriter.cpp
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
#include "oggwriter.hpp"

#include "khopper/error.hpp"

#include <QtCore/QFile>

#include <cstdio>
#include <cstdlib>

using namespace khopper::codec;
using khopper::error::IOError;

OggWriter::OggWriter( const QUrl & uri ):
AbstractWriter( uri ),
out_( NULL ),
encoder_(),
muxer_(),
dsp_(),
block_(),
comments_(),
quality_( -1.f ) {
	if( this->getURI().scheme() != "file" ) {
		throw IOError( QObject::tr( "Do not support remote access." ) );
	}
	this->out_ = new QFile( this->getURI().toLocalFile(), this );
}

OggWriter::~OggWriter() {
}

void OggWriter::doOpen() {
	// setup ogg stream
	ogg_stream_state * os = static_cast< ogg_stream_state * >( std::malloc( sizeof( ogg_stream_state ) ) );
	ogg_stream_init( os, 0xcafebabe );
	this->muxer_.reset( os, ogg_stream_destroy );

	// setup vorbis info
	vorbis_info * vi = static_cast< vorbis_info * >( std::malloc( sizeof( vorbis_info ) ) );
	this->encoder_.reset( vi, []( vorbis_info * vi ) {
		vorbis_info_clear( vi );
		std::free( vi );
	} );
	vorbis_info_init( vi );
	int ret = vorbis_encode_init_vbr( vi, this->getAudioFormat().channels(), this->getAudioFormat().frequency(), this->quality_ );
	if( ret != 0 ) {
		throw error::CodecError( "Vorbis initialization error" );
	}

	// setup vorbis dsp state
	vorbis_dsp_state * vd = static_cast< vorbis_dsp_state * >( std::malloc( sizeof( vorbis_dsp_state ) ) );
	this->dsp_.reset( vd, []( vorbis_dsp_state * vd ) {
		vorbis_dsp_clear( vd );
		std::free( vd );
	} );
	vorbis_analysis_init( vd, vi );

	// setup vorbis block
	vorbis_block * vb = static_cast< vorbis_block * >( std::malloc( sizeof( vorbis_block ) ) );
	this->block_.reset( vb, []( vorbis_block * vb ) {
		vorbis_block_clear( vb );
		std::free( vb );
	} );
	vorbis_block_init( vd, vb );

	// write vorbis comment
	vorbis_comment * vc = static_cast< vorbis_comment * >( std::malloc( sizeof( vorbis_comment ) ) );
	this->comments_.reset( vc, []( vorbis_comment * vc ) {
		vorbis_comment_clear( vc );
		std::free( vc );
	} );
	vorbis_comment_init( vc );
	vorbis_comment_add_tag( vc, "TITLE", this->getTitle().constData() );
	vorbis_comment_add_tag( vc, "ALBUM", this->getAlbum().constData() );
	vorbis_comment_add_tag( vc, "ARTIST", this->getArtist().constData() );

	ogg_packet header;
	ogg_packet header_common;
	ogg_packet header_code;

	vorbis_analysis_headerout( this->dsp_.get(), vc, &header, &header_common, &header_code );
	ogg_stream_packetin( this->muxer_.get(), &header );
	ogg_stream_packetin( this->muxer_.get(), &header_common );
	ogg_stream_packetin( this->muxer_.get(), &header_code );

	// open device
	if( !this->out_->open( QIODevice::WriteOnly ) ) {
		throw IOError( QObject::tr( "Can not open %1 : %2" ).arg( this->getURI().toString() ).arg( this->out_->errorString() ) );
	}

	// write header
	ogg_page og;
	for(;;) {
		ret = ogg_stream_flush( this->muxer_.get(), &og );
		if( ret == 0 ) {
			break;
		}
		this->writePage_( og );
	}
}

void OggWriter::writeFrame( const QByteArray & sample ) {
	if( !sample.isEmpty() ) {
		const int nSamples = sample.size() / sizeof( short int ) / this->getAudioFormat().channels();
		const short int * audio = static_cast< const short int * >( static_cast< const void * >( sample.data() ) );
		float * * buffer = vorbis_analysis_buffer( this->dsp_.get(), nSamples );

		if( this->getAudioFormat().channels() == 1 ) {
			for( int l = 0; l < nSamples; ++l ) {
				buffer[0][l] = audio[l] / 32768.f;
			}
		} else {
			for( int l = 0; l < nSamples; ++l ) {
				buffer[0][l] = audio[l*2] / 32768.f;
				buffer[1][l] = audio[l*2+1] / 32768.f;
			}
		}
		vorbis_analysis_wrote( this->dsp_.get(), nSamples );
	}

	while( vorbis_analysis_blockout( this->dsp_.get(), this->block_.get() ) == 1 ) {
		vorbis_analysis( this->block_.get(), NULL );
		vorbis_bitrate_addblock( this->block_.get() );

		ogg_packet op;
		while( vorbis_bitrate_flushpacket( this->dsp_.get(), &op ) ) {
			ogg_stream_packetin( this->muxer_.get(), &op );
		}

		for(;;) {
			ogg_page og;
			int ret = ogg_stream_pageout( this->muxer_.get(), &og );
			if( ret == 0 ) {
				break;
			}
			this->writePage_( og );
		}
	}
}

void OggWriter::doClose() {
	vorbis_analysis_wrote( this->dsp_.get(), 0 );

	while( vorbis_analysis_blockout( this->dsp_.get(), this->block_.get() ) == 1 ) {
		vorbis_analysis( this->block_.get(), NULL );
		vorbis_bitrate_addblock( this->block_.get() );

		ogg_packet op;
		while( vorbis_bitrate_flushpacket( this->dsp_.get(), &op ) ) {
			ogg_stream_packetin( this->muxer_.get(), &op );
		}

		for(;;) {
			ogg_page og;
			int ret = ogg_stream_pageout( this->muxer_.get(), &og );
			if( ret == 0 ) {
				break;
			}
			this->writePage_( og );
		}
	}

	this->quality_ = -1.f;
	this->comments_.reset();
	this->block_.reset();
	this->dsp_.reset();
	this->muxer_.reset();
	this->encoder_.reset();
	this->out_->close();
}

void OggWriter::writePage_( const ogg_page & og ) {
	this->out_->write( static_cast< const char * >( static_cast< const void * >( og.header ) ), og.header_len );
	this->out_->write( static_cast< const char * >( static_cast< const void * >( og.body ) ), og.body_len );
}
