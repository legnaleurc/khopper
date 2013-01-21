/**
 * @file ffmpegwriter.cpp
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
#include "ffmpegwriter.hpp"

#include "khopper/error.hpp"
#ifdef Q_OS_WIN32
#include "wfile.hpp"
#endif

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <QtCore/QFile>

#include <cstring>

namespace {
	static const double QSCALE_NONE = -99999.;
	static const int OUTPUT_BUFFER_SIZE = FF_MIN_BUFFER_SIZE * 4;
}

using namespace khopper::codec;
#ifdef Q_OS_WIN32
using khopper::ffmpeg::read_packet;
using khopper::ffmpeg::write_packet;
using khopper::ffmpeg::seek;
#endif

FfmpegWriter::FfmpegWriter( const QUrl & uri ):
AbstractWriter( uri ),
#ifdef Q_OS_WIN32
fio_(),
pIOContext_(),
#endif
pFormatContext_(),
pCodecContext_(),
pStream_( NULL ),
queue_(),
quality_( QSCALE_NONE ),
frameSize_( -1 ) {
}

void FfmpegWriter::doOpen() {
	this->setupMuxer();
	this->setupEncoder();
	this->openResource();
	this->writeHeader();
}

void FfmpegWriter::doClose() {
	this->closeResource();
}

void FfmpegWriter::setupMuxer() {
	AVFormatContext * pFC = NULL;
	int ret = avformat_alloc_output_context2( &pFC, NULL, NULL, this->getURI().toString().toUtf8().constData() );
	if( ret != 0 ) {
		throw error::CodecError( AVUNERROR( ret ) );
	}
	this->pFormatContext_.reset( pFC, []( AVFormatContext * oc ) {
#ifndef Q_OS_WIN32
		AVOutputFormat * pOF = oc->oformat;
		if( !( pOF->flags & AVFMT_NOFILE ) && oc->pb ) {
			avio_close( oc->pb );
		}
#endif

		avformat_free_context( oc );
	} );
}

void FfmpegWriter::setupEncoder() {
	AVOutputFormat * pOF = this->pFormatContext_->oformat;
	if( pOF->audio_codec == CODEC_ID_NONE ) {
		throw error::CodecError( "Can not setup encoder" );
	}

	AVCodec * pC = avcodec_find_encoder( pOF->audio_codec );
	if( !pC ) {
		throw error::CodecError( "Find no encoder" );
	}

	this->pStream_ = avformat_new_stream( this->pFormatContext_.get(), pC );
	if( !this->pStream_ ) {
		throw error::CodecError( "Can not create stream" );
	}

	AVCodecContext * pCC = this->pStream_->codec;
	this->pCodecContext_.reset( pCC, avcodec_close );
	pCC->time_base.num = 1;
	pCC->time_base.den = this->getAudioFormat().frequency();

	// setting special parameters
	pCC->bit_rate = this->getBitRate();
	pCC->sample_rate = this->getAudioFormat().frequency();
	pCC->channels = this->getAudioFormat().channels();
	if( this->quality_ != QSCALE_NONE ) {
		pCC->flags |= CODEC_FLAG_QSCALE;
		pCC->global_quality = static_cast< int >( FF_QP2LAMBDA * this->quality_ );
	}
	// NOTE: set complete

	pCC->sample_fmt = pC->sample_fmts[0];

	if( avcodec_open2( pCC, pC, NULL ) < 0 ) {
		throw error::CodecError( "Can not open encoder" );
	}

	switch( pCC->codec_id ) {
	case CODEC_ID_PCM_S16LE:
	case CODEC_ID_PCM_S16BE:
	case CODEC_ID_PCM_U16LE:
	case CODEC_ID_PCM_U16BE:
	case CODEC_ID_PCM_S8:
	case CODEC_ID_PCM_U8:
	case CODEC_ID_PCM_MULAW:
	case CODEC_ID_PCM_ALAW:
	case CODEC_ID_PCM_S32LE:
	case CODEC_ID_PCM_S32BE:
	case CODEC_ID_PCM_U32LE:
	case CODEC_ID_PCM_U32BE:
	case CODEC_ID_PCM_S24LE:
	case CODEC_ID_PCM_S24BE:
	case CODEC_ID_PCM_U24LE:
	case CODEC_ID_PCM_U24BE:
	case CODEC_ID_PCM_S24DAUD:
	case CODEC_ID_PCM_ZORK:
	case CODEC_ID_PCM_S16LE_PLANAR:
	case CODEC_ID_PCM_DVD:
	case CODEC_ID_PCM_F32BE:
	case CODEC_ID_PCM_F32LE:
	case CODEC_ID_PCM_F64BE:
	case CODEC_ID_PCM_F64LE:
		this->frameSize_ = OUTPUT_BUFFER_SIZE;
		break;
	default:
		this->frameSize_ = pCC->frame_size * sizeof( short ) * pCC->channels;
	}
}

void FfmpegWriter::openResource() {
	AVOutputFormat * pOF = this->pFormatContext_->oformat;
	if( pOF->flags & AVFMT_NOFILE ) {
		return;
	}

#ifndef Q_OS_WIN
	if( avio_open( &this->pFormatContext_->pb, this->getURI().toString().toUtf8().constData(), AVIO_FLAG_WRITE ) < 0 ) {
		throw error::IOError( QString( "Can not open file: `%1\'" ).arg( this->getURI().toString() ) );
	}
#else
	// TODO only support local file
	this->fio_.reset( new QFile( this->getURI().toLocalFile() ) );
	if( !this->fio_->open( QIODevice::ReadWrite ) ) {
		throw error::IOError( this->fio_->errorString() );
	}
	const int SIZE = 4 * 1024 * sizeof( unsigned char );
	unsigned char * buffer = static_cast< unsigned char * >( av_malloc( SIZE ) );
	this->pFormatContext_->pb = avio_alloc_context( buffer, SIZE, 1, this->fio_.get(), read_packet, write_packet, ::seek );
	this->pIOContext_.reset( this->pFormatContext_->pb, []( AVIOContext * pb ) {
		av_free( pb->buffer );
		av_free( pb );
	} );
#endif
}

void FfmpegWriter::closeResource() {
	this->writeFrame( NULL );
	av_write_trailer( this->pFormatContext_.get() );
	this->queue_.clear();
	this->pCodecContext_.reset();
	this->pFormatContext_.reset();
#ifdef Q_OS_WIN32
	this->pIOContext_.reset();
	this->fio_->close();
	this->fio_.reset();
#endif
}

void FfmpegWriter::writeHeader() {
	av_dict_set( &this->pFormatContext_->metadata, "title", this->getTitle().constData(), 0 );
	av_dict_set( &this->pFormatContext_->metadata, "artist", this->getArtist().constData(), 0 );
	av_dict_set( &this->pFormatContext_->metadata, "album", this->getAlbum().constData(), 0 );

	if( avformat_write_header( this->pFormatContext_.get(), NULL ) < 0 ) {
		throw error::CodecError( "Can not write header" );
	}
}

void FfmpegWriter::writeFrame( const QByteArray & sample ) {
	if( !sample.isEmpty() ) {
		// put samples into queue
		this->queue_.append( sample );
		// encode if possible
		while( this->queue_.size() >= this->frameSize_ ) {
			this->writeFrame( static_cast< const short * >( static_cast< const void * >( this->queue_.left( this->frameSize_ ).constData() ) ) );
			this->queue_.remove( 0, this->frameSize_ );
		}
	} else {
		if( !this->queue_.isEmpty() ) {
			this->writeFrame( static_cast< const short * >( static_cast< const void * >( this->queue_.constData() ) ) );
			this->queue_.clear();
		}
	}
}

void FfmpegWriter::writeFrame( const short * sample ) {
	AVCodecContext * pCC = this->pStream_->codec;

	static uint8_t audio_outbuf[OUTPUT_BUFFER_SIZE];

	AVPacket pkt;
	av_init_packet( &pkt );

	pkt.size = avcodec_encode_audio( pCC, audio_outbuf, sizeof( audio_outbuf ), sample );

	pkt.data = audio_outbuf;
	pkt.stream_index = this->pStream_->index;
	pkt.flags |= AV_PKT_FLAG_KEY;

	if( pCC->coded_frame->pts != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
		pkt.pts = av_rescale_q( pCC->coded_frame->pts, pCC->time_base, this->pStream_->time_base );
	}

	if( av_interleaved_write_frame( this->pFormatContext_.get(), &pkt ) != 0 ) {
		throw error::CodecError( "Can not write frame" );
	}
}
