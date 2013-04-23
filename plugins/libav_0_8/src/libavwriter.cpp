/**
 * @file libavwriter.cpp
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
#include "libavwriter.hpp"

extern "C" {
#include <libavutil/mathematics.h>
}

#include <QtCore/QFile>

#include "khopper/codecerror.hpp"
#include "khopper/ioerror.hpp"
#ifdef Q_OS_WIN32
#include "wfile.hpp"
#endif

using khopper::error::CodecError;
using khopper::error::IOError;

namespace {
static const double QSCALE_NONE = -99999.;
}

using khopper::codec::LibavWriter;
#ifdef Q_OS_WIN32
using khopper::libav::read_packet;
using khopper::libav::write_packet;
using khopper::libav::seek;
#endif

LibavWriter::LibavWriter( const QUrl & uri ):
Writer( uri ),
#ifdef Q_OS_WIN32
fio_(),
pIOContext_(),
#endif
pFormatContext_(),
pCodecContext_(),
pFrame_( avcodec_alloc_frame(), []( AVFrame * p )->void {
	av_freep( &p );
} ),
pStream_( NULL ),
queue_(),
quality_( QSCALE_NONE ),
frameSize_( -1 ),
sampleLength_( -1 ) {
}

void LibavWriter::doOpen() {
	this->setupMuxer();
	this->setupEncoder();
	this->openResource();
	this->writeHeader();
}

void LibavWriter::doClose() {
	this->closeResource();
}

void LibavWriter::setupMuxer() {
	auto pOF = av_guess_format( NULL, this->getURI().toString().toUtf8().constData(), NULL );
	if( !pOF ) {
		throw CodecError( QObject::tr( "could not recognize output format" ), __FILE__, __LINE__ );
	}
	this->pFormatContext_.reset( avformat_alloc_context(), []( AVFormatContext * oc )->void {
#ifndef Q_OS_WIN32
		AVOutputFormat * pOF = oc->oformat;
		if( !( pOF->flags & AVFMT_NOFILE ) && oc->pb ) {
			avio_close( oc->pb );
		}
#endif
		avformat_free_context( oc );
	} );
	this->pFormatContext_->oformat = pOF;
	qstrncpy( this->pFormatContext_->filename, this->getURI().toString().toStdString().c_str(), sizeof( this->pFormatContext_->filename ) );
}

void LibavWriter::setupEncoder() {
	AVOutputFormat * pOF = this->pFormatContext_->oformat;
	if( pOF->audio_codec == CODEC_ID_NONE ) {
		throw CodecError( QObject::tr( "Can not setup encoder" ), __FILE__, __LINE__ );
	}

	AVCodec * pC = avcodec_find_encoder( pOF->audio_codec );
	if( !pC ) {
		throw CodecError( QObject::tr( "Find no encoder" ), __FILE__, __LINE__ );
	}

	this->pStream_ = avformat_new_stream( this->pFormatContext_.get(), pC );
	if( !this->pStream_ ) {
		throw CodecError( QObject::tr( "Can not create stream" ), __FILE__, __LINE__ );
	}
	this->pStream_->id = this->pFormatContext_->nb_streams - 1;

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

	// FIXME: find best sample format?
	pCC->sample_fmt = AV_SAMPLE_FMT_NONE;
	for( auto fmt = pC->sample_fmts; *fmt != AV_SAMPLE_FMT_NONE; ++fmt ) {
		if( !av_sample_fmt_is_planar( *fmt ) && av_get_bytes_per_sample( *fmt ) == 2 ) {
			pCC->sample_fmt = *fmt;
			break;
		}
	}
	if( pCC->sample_fmt == AV_SAMPLE_FMT_NONE ) {
		throw CodecError( QObject::tr( "this codec does not support PCM S16LE" ), __FILE__, __LINE__ );
	}

	int ret = avcodec_open2( pCC, pC, NULL );
	if( ret < 0 ) {
		throw CodecError( AVUNERROR( ret ), __FILE__, __LINE__ );
	}

	if( pC->capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE ) {
		this->frameSize_ = FF_MIN_BUFFER_SIZE;
	} else {
		this->frameSize_ = pCC->frame_size;
	}

	this->sampleLength_ = this->frameSize_ * av_get_bytes_per_sample( pCC->sample_fmt ) * pCC->channels;
}

void LibavWriter::openResource() {
	AVOutputFormat * pOF = this->pFormatContext_->oformat;
	if( pOF->flags & AVFMT_NOFILE ) {
		return;
	}

#ifndef Q_OS_WIN
	if( avio_open( &this->pFormatContext_->pb, this->getURI().toString().toUtf8().constData(), AVIO_FLAG_WRITE ) < 0 ) {
		throw IOError( QObject::tr( "Can not open file: `%1\'" ).arg( this->getURI().toString() ), __FILE__, __LINE__ );
	}
#else
	// TODO only support local file
	this->fio_.reset( new QFile( this->getURI().toLocalFile() ) );
	if( !this->fio_->open( QIODevice::ReadWrite ) ) {
		throw IOError( this->fio_->errorString(), __FILE__, __LINE__ );
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

void LibavWriter::closeResource() {
	// flush remaining samples
	this->writeData( nullptr, 0 );
	av_write_trailer( this->pFormatContext_.get() );
	this->queue_.clear();
	this->pStream_ = nullptr;
	this->pFrame_.reset();
	this->pCodecContext_.reset();
	this->pFormatContext_.reset();
#ifdef Q_OS_WIN32
	this->pIOContext_.reset();
	this->fio_->close();
	this->fio_.reset();
#endif
}

void LibavWriter::writeHeader() {
	av_dict_set( &this->pFormatContext_->metadata, "title", this->getTitle().constData(), 0 );
	av_dict_set( &this->pFormatContext_->metadata, "artist", this->getArtist().constData(), 0 );
	av_dict_set( &this->pFormatContext_->metadata, "album", this->getAlbum().constData(), 0 );

	if( avformat_write_header( this->pFormatContext_.get(), NULL ) < 0 ) {
		throw CodecError( QObject::tr( "Can not write header" ), __FILE__, __LINE__ );
	}
}

qint64 LibavWriter::writeData( const char * data, qint64 len ) {
	if( data && len > 0LL ) {
		// put samples into queue
		this->queue_.append( data, len );
		// encode if possible
		while( this->queue_.size() >= this->sampleLength_ ) {
			auto tmp = this->queue_.left( this->sampleLength_ );
			this->writeFrame( tmp.constData() );
			this->queue_.remove( 0, this->sampleLength_ );
		}
	} else {
		if( !this->queue_.isEmpty() ) {
			this->writeFrame( this->queue_.constData() );
			this->queue_.clear();
		}
	}
	return len;
}

void LibavWriter::writeFrame( const char * sample ) {
	AVCodecContext * pCC = this->pStream_->codec;
	this->pFrame_->nb_samples = this->frameSize_;
	auto tmp = static_cast< const uint8_t * >( static_cast< const void * >( sample ) );
	int ret = avcodec_fill_audio_frame( this->pFrame_.get(), pCC->channels, pCC->sample_fmt, tmp, this->sampleLength_, 1 );
	if( ret < 0 ) {
		throw CodecError( AVUNERROR( ret ), __FILE__, __LINE__ );
	}

	AVPacket pkt;
	av_init_packet( &pkt );
	pkt.data = nullptr;
	pkt.size = 0;
	int got_frame = 0;
	ret = avcodec_encode_audio2( pCC, &pkt, this->pFrame_.get(), &got_frame );
	if( ret < 0 ) {
		throw CodecError( AVUNERROR( ret ), __FILE__, __LINE__ );
	}
	if( !got_frame ) {
		return;
	}

	pkt.stream_index = this->pStream_->index;

	if( pCC->coded_frame->pts != AV_NOPTS_VALUE ) {
		pkt.pts = av_rescale_q( pCC->coded_frame->pts, pCC->time_base, this->pStream_->time_base );
	}

	ret = av_interleaved_write_frame( this->pFormatContext_.get(), &pkt );
	if( ret != 0 ) {
		throw CodecError( QObject::tr( "Can not write frame" ), __FILE__, __LINE__ );
	}
}
