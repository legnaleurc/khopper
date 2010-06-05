/**
 * @file ffmpegreader.cpp
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
#include "ffmpegreader.hpp"

#include "khopper/error.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <cstdlib>

namespace {

	static inline std::string wHelper( const QUrl & uri ) {
		// FIXME: not always local file
		QString tmp( uri.toLocalFile() );
#ifdef Q_OS_WIN32
		tmp.prepend( "wfile://" );
#endif
		return tmp.toStdString();
	}

	static inline void p_helper( AVPacket * p ) {
		av_freep( &p );
	}

	static inline int64_t toNative( int64_t ms ) {
		return av_rescale( ms, AV_TIME_BASE, 1000 );
	}

	static inline int toMS( int64_t timestamp ) {
		return av_rescale( timestamp, 1000, AV_TIME_BASE );
	}

}

using namespace khopper::codec;
using khopper::error::IOError;
using khopper::error::CodecError;

FfmpegReader::FfmpegReader( const QUrl & uri ):
AbstractReader( uri ),
pFormatContext_(),
pCodecContext_(),
pPacket_( static_cast< AVPacket * >( av_malloc( sizeof( AVPacket ) ) ), ::p_helper ),
pStream_( NULL ),
msCurrent_( 0LL ),
eof_( true ) {
	av_init_packet( this->pPacket_.get() );
}

bool FfmpegReader::atEnd() const {
	return this->eof_;
}

qint64 FfmpegReader::pos() const {
	return this->msCurrent_;
}

void FfmpegReader::doOpen() {
	this->openResource();
	this->setupDemuxer();
	this->setupDecoder();
	this->readHeader();
	this->eof_ = false;
}

void FfmpegReader::doClose() {
	this->eof_ = true;
	this->closeResource();
}

void FfmpegReader::openResource() {
	AVFormatContext * pFC = NULL;
	int ret = av_open_input_file( &pFC, wHelper( this->getURI() ).c_str(), NULL, 0, NULL );
	if( ret != 0 ) {
		throw IOError(
			QString(
				"Can not open `%1\':\n"
				"%2"
			).arg( this->getURI().toString() ).arg( strerror( AVUNERROR( ret ) ) )
		);
	}
	this->pFormatContext_.reset( pFC, av_close_input_file );
}

void FfmpegReader::setupDemuxer() {
	if( av_find_stream_info( this->pFormatContext_.get() ) < 0 ) {
		throw CodecError( "Can not find codec info!" );
	}

	if( this->pFormatContext_->duration != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
		this->setDuration( toMS( this->pFormatContext_->duration ) );
	} else {
		throw CodecError( "Can not get duration!" );
	}
}

void FfmpegReader::setupDecoder() {
	int a_stream = -1;
	for( std::size_t i = 0 ; i < this->pFormatContext_->nb_streams; ++i ) {
		if( this->pFormatContext_->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO ) {
			a_stream = i;
			break;
		}
	}
	if( a_stream == -1 ) {
		throw CodecError( "Find no audio stream!" );
	}
	this->pStream_ = this->pFormatContext_->streams[a_stream];
	AVCodecContext * pCC = this->pStream_->codec;
	// getting codec information
	this->setBitRate( pCC->bit_rate );
	this->setSampleRate( pCC->sample_rate );
	this->setChannels( pCC->channels );
	switch( this->getChannels() ) {
	case 1:
		this->setChannelLayout( LayoutMono );
		break;
	case 2:
		this->setChannelLayout( LayoutStereo );
		break;
	default:
		this->setChannelLayout( LayoutNative );
	}
	switch( pCC->sample_fmt ) {
		case SAMPLE_FMT_U8:
			this->setSampleFormat( SF_U8 );
			break;
		case SAMPLE_FMT_S16:
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
			this->setSampleFormat( SF_S16LE );
#else
			this->setSampleFormat( SF_S16BE );
#endif
			break;
		case SAMPLE_FMT_S32:
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
			this->setSampleFormat( SF_S32LE );
#else
			this->setSampleFormat( SF_S32BE );
#endif
			break;
		case SAMPLE_FMT_FLT:
			this->setSampleFormat( SF_FLOAT );
			break;
		case SAMPLE_FMT_DBL:
			this->setSampleFormat( SF_DOUBLE );
			break;
		default:
			this->setSampleFormat( SF_NONE );
	}

	AVCodec * pC = avcodec_find_decoder( pCC->codec_id );
	if( pC == NULL ) {
		throw error::CodecError( "Find no decoder!" );
	}

	if( avcodec_open( pCC, pC ) < 0 ) {
		throw error::CodecError( "Can not open decoder." );
	}
	this->pCodecContext_.reset( pCC, avcodec_close );
}

void FfmpegReader::readHeader() {
	av_metadata_conv( this->pFormatContext_.get(), NULL, this->pFormatContext_->iformat->metadata_conv );
	AVMetadataTag * mt = NULL;
	if( ( mt = av_metadata_get( this->pFormatContext_->metadata, "title", NULL, 0 ) ) ) {
		this->setTitle( mt->value );
	}
	if( ( mt = av_metadata_get( this->pFormatContext_->metadata, "author", NULL, 0 ) ) ) {
		this->setArtist( mt->value );
	}
	if( ( mt = av_metadata_get( this->pFormatContext_->metadata, "copyright", NULL, 0 ) ) ) {
		this->setCopyright( mt->value );
	}
	if( ( mt = av_metadata_get( this->pFormatContext_->metadata, "comment", NULL, 0 ) ) ) {
		this->setComment( mt->value );
	}
	if( ( mt = av_metadata_get( this->pFormatContext_->metadata, "album", NULL, 0 ) ) ) {
		this->setAlbum( mt->value );
	}
	if( ( mt = av_metadata_get( this->pFormatContext_->metadata, "year", NULL, 0 ) ) ) {
		this->setYear( mt->value );
	}
	if( ( mt = av_metadata_get( this->pFormatContext_->metadata, "track", NULL, 0 ) ) ) {
		this->setIndex( std::atoi( mt->value ) );
	}
	if( ( mt = av_metadata_get( this->pFormatContext_->metadata, "genre", NULL, 0 ) ) ) {
		this->setGenre( mt->value );
	}
}

void FfmpegReader::closeResource() {
	// clear native information
	this->msCurrent_ = 0LL;
	this->pStream_ = NULL;
	// free the members in packet, not itself
	av_free_packet( this->pPacket_.get() );
	av_init_packet( this->pPacket_.get() );
	this->pCodecContext_.reset();
	this->pFormatContext_.reset();
}

QByteArray FfmpegReader::readFrame() {
	//stop = false;

	// read a frame
	int ret = av_read_frame( this->pFormatContext_.get(), this->pPacket_.get() );
	if( ret < 0 ) {
		//stop = true;
		this->eof_ = true;
		return QByteArray();
	}

	int16_t sampleBuffer[AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2];
	QByteArray frame;
	// current presentation timestamp: in second * AV_TIME_BASE
	//int64_t curPts = -1;
	// decoded time: in second * AV_TIME_BASE
	int64_t decoded = 0;
	if( this->pPacket_->pts != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
		// rescale presentation timestamp
		this->msCurrent_ = AV_TIME_BASE * av_rescale(
			this->pPacket_->pts,
			this->pStream_->time_base.num,
			this->pStream_->time_base.den
		);
	}
#if LIBAVCODEC_VERSION_MAJOR < 53
	uint8_t * audio_pkt_data = this->pPacket_->data;
	int audio_pkt_size = this->pPacket_->size;
	while( audio_pkt_size > 0 ) {
#else
	// backup buffer pointer
	uint8_t * pktDataBackup = this->pPacket_->data;
	while( this->pPacket_->size > 0 ) {
#endif
		//if( this->afterEnd( toMS( curPts ) ) ) {
		//	stop = true;
		//	break;
		//}
		int sampleByteLength = sizeof( sampleBuffer );
#if LIBAVCODEC_VERSION_MAJOR < 53
		int decodedPacketSize = avcodec_decode_audio2(
			this->pCodecContext_.get(),
			sampleBuffer,
			&sampleByteLength,
			audio_pkt_data,
			audio_pkt_size
		);
#else
		int decodedPacketSize = avcodec_decode_audio3(
			this->pCodecContext_.get(),
			sampleBuffer,
			&sampleByteLength,
			this->pPacket_.get()
		);
#endif
		if( decodedPacketSize < 0 ) {
#if LIBAVCODEC_VERSION_MAJOR < 53
			audio_pkt_size = 0;
#endif
			break;
		}
#if LIBAVCODEC_VERSION_MAJOR < 53
		audio_pkt_data += decodedPacketSize;
		audio_pkt_size -= decodedPacketSize;
#else
		this->pPacket_->data += decodedPacketSize;
		this->pPacket_->size -= decodedPacketSize;
#endif
		if( sampleByteLength <= 0 ) {
			continue;
		}
		// decoded time: decoded size in byte / sizeof int16_t * AV_TIME_BASE / ( sample rate * channels )
		int64_t ptsDiff = ( static_cast< int64_t >( AV_TIME_BASE ) * ( sampleByteLength / sizeof( int16_t ) ) ) / ( this->getSampleRate() * this->getChannels() );
		//if( this->afterBegin( toMS( curPts ) ) ) {
			const int8_t * tmp = static_cast< int8_t * >( static_cast< void * >( sampleBuffer ) );
			frame.append( tmp, sampleByteLength );
			decoded += ptsDiff;
		//}
		//curPts += ptsDiff;
	}
#if LIBAVCODEC_VERSION_MAJOR < 53
	if( this->pPacket_->data ) {
#else
	if( pktDataBackup ) {
		this->pPacket_->data = pktDataBackup;
#endif
		av_free_packet( this->pPacket_.get() );
	}

	this->msCurrent_ += toMS( decoded );

	return frame;
}

bool FfmpegReader::seekFrame( qint64 msPos ) {
	int64_t internalPos = av_rescale( msPos, this->pStream_->time_base.den, this->pStream_->time_base.num * 1000 );
	int succeed = av_seek_frame( this->pFormatContext_.get(), this->pStream_->index, internalPos, AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD );
	if( succeed >= 0 ) {
		avcodec_flush_buffers( this->pCodecContext_.get() );
	}
	this->msCurrent_ = av_rescale( this->pStream_->cur_pkt.pts, this->pStream_->time_base.num * 1000, this->pStream_->time_base.den );
	return succeed >= 0;
}
