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
#include "khopper/text.hpp"

#include <cstdio>
#include <cstdlib>

namespace {

	static inline FILE * fileHelper( const QUrl & uri ) {
#ifdef Q_OS_WIN32
		FILE * fout = NULL;
		errno_t ret = _wfopen_s( &fout, uri.toLocalFile().toStdWString().c_str(), L"wb" );
		if( ret != 0 ) {
			return NULL;
		}
		return fout;
#else
		return fopen( uri.toLocalFile().toStdString().c_str(), "wb" );
#endif
	}

	static inline void vorbisInfoHelper( vorbis_info * vi ) {
		vorbis_info_clear( vi );
		std::free( vi );
	}

	static inline void vorbisDSPHelper( vorbis_dsp_state * vd ) {
		vorbis_dsp_clear( vd );
		std::free( vd );
	}

	static inline void vorbisBlockHelper( vorbis_block * vb ) {
		vorbis_block_clear( vb );
		std::free( vb );
	}

	static inline void vorbisCommentHelper( vorbis_comment * vc ) {
		vorbis_comment_clear( vc );
		std::free( vc );
	}

}

using namespace khopper::codec;

OggWriter::OggWriter( const QUrl & uri ):
AbstractWriter( uri ),
fout_(),
encoder_(),
muxer_(),
dsp_(),
block_(),
comments_(),
quality_( -1.f ) {
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
	this->encoder_.reset( vi, vorbisInfoHelper );
	vorbis_info_init( vi );
	int ret = vorbis_encode_init_vbr( vi, this->getAudioFormat().channels(), this->getAudioFormat().frequency(), this->quality_ );
	if( ret != 0 ) {
		throw error::CodecError( "Vorbis initialization error" );
	}

	// setup vorbis dsp state
	vorbis_dsp_state * vd = static_cast< vorbis_dsp_state * >( std::malloc( sizeof( vorbis_dsp_state ) ) );
	this->dsp_.reset( vd, vorbisDSPHelper );
	vorbis_analysis_init( vd, vi );

	// setup vorbis block
	vorbis_block * vb = static_cast< vorbis_block * >( std::malloc( sizeof( vorbis_block ) ) );
	this->block_.reset( vb, vorbisBlockHelper );
	vorbis_block_init( vd, vb );

	// write vorbis comment
	vorbis_comment * vc = static_cast< vorbis_comment * >( std::malloc( sizeof( vorbis_comment ) ) );
	this->comments_.reset( vc, vorbisCommentHelper );
	vorbis_comment_init( vc );
	// NOTE: dirty hack for the stupid prototype of vorbis_comment_add_tag.
	vorbis_comment_add_tag( vc, const_cast< char * >( "TITLE" ), this->getTitle().data() );
	vorbis_comment_add_tag( vc, const_cast< char * >( "ALBUM" ), this->getAlbum().data() );
	vorbis_comment_add_tag( vc, const_cast< char * >( "ARTIST" ), this->getArtist().data() );

	ogg_packet header;
	ogg_packet header_common;
	ogg_packet header_code;

	vorbis_analysis_headerout( this->dsp_.get(), vc, &header, &header_common, &header_code );
	ogg_stream_packetin( this->muxer_.get(), &header );
	ogg_stream_packetin( this->muxer_.get(), &header_common );
	ogg_stream_packetin( this->muxer_.get(), &header_code );

	// open file
	FILE * fout = fileHelper( this->getURI() );
	if( fout == NULL ) {
		throw error::IOError( QString( "Can not open: %1" ).arg( this->getURI().toString() ) );
	}
	this->fout_.reset( fout, std::fclose );

	// write header
	ogg_page og;
	for(;;) {
		int ret = ogg_stream_flush( this->muxer_.get(), &og );
		if( ret == 0 ) {
			break;
		}
		std::fwrite( og.header, 1, og.header_len, this->fout_.get() );
		std::fwrite( og.body, 1, og.body_len, this->fout_.get() );
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
			std::fwrite( og.header, 1, og.header_len, this->fout_.get() );
			std::fwrite( og.body, 1, og.body_len, this->fout_.get() );
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
			std::fwrite( og.header, 1, og.header_len, this->fout_.get() );
			std::fwrite( og.body, 1, og.body_len, this->fout_.get() );
		}
	}

	this->quality_ = -1.f;
	this->comments_.reset();
	this->block_.reset();
	this->dsp_.reset();
	this->muxer_.reset();
	this->encoder_.reset();
	this->fout_.reset();
}
