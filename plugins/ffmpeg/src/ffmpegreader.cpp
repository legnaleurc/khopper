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
#ifdef Q_OS_WIN
#include "wfile.hpp"
#endif

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <QtCore/QFile>
#include <QtCore/QtDebug>

#include <cstdlib>
#include <cstring>

namespace {

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
using khopper::ffmpeg::read_packet;
using khopper::ffmpeg::write_packet;
using khopper::ffmpeg::seek;

FfmpegReader::FfmpegReader( const QUrl & uri ):
AbstractReader( uri ),
#ifdef Q_OS_WIN32
fio_(),
pIOContext_(),
#endif
pFormatContext_(),
pCodecContext_(),
pPacket_( static_cast< AVPacket * >( av_malloc( sizeof( AVPacket ) ) ), []( AVPacket * p ) {
	av_freep( &p );
} ),
pStream_( NULL ),
curPos_( 0LL ),
eof_( true ),
buffer_() {
	av_init_packet( this->pPacket_.get() );
}

bool FfmpegReader::atEnd() const {
	return this->eof_;
}

qint64 FfmpegReader::size() const {
	if( !this->isOpen() ) {
		return 0;
	}
	// size = duration * AVStream::time_base * frequency * channels * sampleSize
	return av_rescale( this->pStream_->duration, this->pStream_->time_base.num* this->getAudioFormat().frequency() * this->getAudioFormat().channels() * this->getAudioFormat().sampleSize() / 8, this->pStream_->time_base.den );
}

void FfmpegReader::doOpen() {
	this->openResource_();
	this->setupDemuxer_();
	this->setupDecoder_();
	this->readHeader_();
	this->eof_ = false;
}

void FfmpegReader::doClose() {
	this->eof_ = true;
	this->closeResource_();
}

void FfmpegReader::openResource_() {
	AVFormatContext * pFC = NULL;
#ifdef Q_OS_WIN
	if( this->getURI().scheme() == "file" ) {
		this->fio_.reset( new QFile( this->getURI().toLocalFile() ) );
		if( !this->fio_->open( QIODevice::ReadOnly ) ) {
			throw IOError( this->fio_->errorString() );
		}
		pFC = avformat_alloc_context();
		const int SIZE = 4 * 4096 * sizeof( unsigned char );
		unsigned char * buffer = static_cast< unsigned char * >( av_malloc( SIZE ) );
		pFC->pb = avio_alloc_context( buffer, SIZE, 0, this->fio_.get(), read_packet, write_packet, ::seek );
		this->pIOContext_.reset( pFC->pb, []( AVIOContext * pb ) {
			av_free( pb->buffer );
			av_free( pb );
		} );
	}
#endif
	int ret = avformat_open_input( &pFC, this->getURI().toString().toUtf8().constData(), NULL, NULL );
	if( ret != 0 ) {
		throw IOError( AVUNERROR( ret ) );
	}
	this->pFormatContext_.reset( pFC, av_close_input_file );
}

void FfmpegReader::setupDemuxer_() {
	if( av_find_stream_info( this->pFormatContext_.get() ) < 0 ) {
		throw CodecError( tr( "FfmpegReader: Can not find codec info!" ) );
	}

	if( this->pFormatContext_->duration != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
		this->setDuration( toMS( this->pFormatContext_->duration ) );
	} else {
		throw CodecError( tr( "FfmpegReader: Can not get duration!" ) );
	}
}

void FfmpegReader::setupDecoder_() {
	int a_stream = -1;
	for( std::size_t i = 0 ; i < this->pFormatContext_->nb_streams; ++i ) {
		if( this->pFormatContext_->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO ) {
			a_stream = i;
			break;
		}
	}
	if( a_stream == -1 ) {
		throw CodecError( "FfmpegReader: Find no audio stream!" );
	}
	this->pStream_ = this->pFormatContext_->streams[a_stream];
	AVCodecContext * pCC = this->pStream_->codec;
	// getting codec information
	this->setBitRate( pCC->bit_rate );
	AudioFormat format;
	format.setFrequency( pCC->sample_rate );
	format.setChannels( pCC->channels );
	switch( pCC->channels ) {
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
		format.setSampleType( AudioFormat::UnSignedInt );
		format.setSampleSize( 8 );
		break;
	case SAMPLE_FMT_S16:
		format.setSampleType( AudioFormat::SignedInt );
		format.setSampleSize( 16 );
		break;
	case SAMPLE_FMT_S32:
		format.setSampleType( AudioFormat::SignedInt );
		format.setSampleSize( 32 );
		break;
	case SAMPLE_FMT_FLT:
		format.setSampleType( AudioFormat::Float );
		format.setSampleSize( 32 );
		break;
	case SAMPLE_FMT_DBL:
		format.setSampleType( AudioFormat::Float );
		format.setSampleSize( 64 );
		break;
	default:
		;
	}
	this->setAudioFormat( format );

	AVCodec * pC = avcodec_find_decoder( pCC->codec_id );
	if( pC == NULL ) {
		throw error::CodecError( tr( "FfmpegReader: Find no decoder!" ) );
	}

	if( avcodec_open( pCC, pC ) < 0 ) {
		throw error::CodecError( tr( "FfmpegReader: Can not open decoder." ) );
	}
	this->pCodecContext_.reset( pCC, avcodec_close );
}

void FfmpegReader::readHeader_() {
	if( this->pStream_->metadata ) {
		this->readMetadata_( this->pStream_->metadata );
	} else {
		this->readMetadata_( this->pFormatContext_->metadata );
	}
}

void FfmpegReader::readMetadata_( AVDictionary * metadata ) {
	AVDictionaryEntry * mt = NULL;
	if( ( mt = av_dict_get( metadata, "title", NULL, 0 ) ) ) {
		this->setTitle( mt->value );
	}
	if( ( mt = av_dict_get( metadata, "artist", NULL, 0 ) ) ) {
		this->setArtist( mt->value );
	}
	if( ( mt = av_dict_get( metadata, "copyright", NULL, 0 ) ) ) {
		this->setCopyright( mt->value );
	}
	if( ( mt = av_dict_get( metadata, "comment", NULL, 0 ) ) ) {
		this->setComment( mt->value );
	}
	if( ( mt = av_dict_get( metadata, "album", NULL, 0 ) ) ) {
		this->setAlbumTitle( mt->value );
	}
	if( ( mt = av_dict_get( metadata, "year", NULL, 0 ) ) ) {
		this->setYear( mt->value );
	}
	if( ( mt = av_dict_get( metadata, "track", NULL, 0 ) ) ) {
		this->setIndex( std::atoi( mt->value ) );
	}
	if( ( mt = av_dict_get( metadata, "genre", NULL, 0 ) ) ) {
		this->setGenre( mt->value );
	}
}

void FfmpegReader::closeResource_() {
	// clear native information
	this->curPos_ = 0LL;
	this->pStream_ = NULL;
	// free the members in packet, not itself
	av_free_packet( this->pPacket_.get() );
	av_init_packet( this->pPacket_.get() );
	this->pCodecContext_.reset();
	this->pFormatContext_.reset();
#ifdef Q_OS_WIN32
	this->pIOContext_.reset();
	this->fio_.reset();
#endif
}

qint64 FfmpegReader::readData( char * data, qint64 maxSize ) {
	while( !this->atEnd() && this->buffer_.size() < maxSize ) {
		this->buffer_.append( this->readFrame_() );
	}
	maxSize = qMin( static_cast< qint64 >( this->buffer_.size() ), maxSize );
	std::memcpy( data, this->buffer_, maxSize );
	this->buffer_.remove( 0, maxSize );
	return maxSize;
}

QByteArray FfmpegReader::readFrame_() {
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
	//if( this->pPacket_->pts != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
	//	// rescale presentation timestamp
	//	this->msCurrent_ = AV_TIME_BASE * av_rescale(
	//		this->pPacket_->pts,
	//		this->pStream_->time_base.num,
	//		this->pStream_->time_base.den
	//	);
	//}
	// backup buffer pointer
	uint8_t * pktDataBackup = this->pPacket_->data;
	while( this->pPacket_->size > 0 ) {
		//if( this->afterEnd( toMS( curPts ) ) ) {
		//	stop = true;
		//	break;
		//}
		int sampleByteLength = sizeof( sampleBuffer );
		int decodedPacketSize = avcodec_decode_audio3(
			this->pCodecContext_.get(),
			sampleBuffer,
			&sampleByteLength,
			this->pPacket_.get()
		);
		if( decodedPacketSize < 0 ) {
			break;
		}
		this->pPacket_->data += decodedPacketSize;
		this->pPacket_->size -= decodedPacketSize;
		if( sampleByteLength <= 0 ) {
			continue;
		}
		// decoded time: decoded size in byte / sizeof int16_t * AV_TIME_BASE / ( sample rate * channels )
		int64_t ptsDiff = ( static_cast< int64_t >( AV_TIME_BASE ) * ( sampleByteLength / sizeof( int16_t ) ) ) / ( this->getAudioFormat().frequency() * this->getAudioFormat().channels() );
		//if( this->afterBegin( toMS( curPts ) ) ) {
			const char * tmp = static_cast< char * >( static_cast< void * >( sampleBuffer ) );
			frame.append( tmp, sampleByteLength );
			decoded += ptsDiff;
		//}
		//curPts += ptsDiff;
	}
	if( pktDataBackup ) {
		this->pPacket_->data = pktDataBackup;
		av_free_packet( this->pPacket_.get() );
	}

	this->curPos_ += toMS( decoded );

	return frame;
}

bool FfmpegReader::seek( qint64 pos ) {
	bool succeed = this->AbstractReader::seek( pos );
	// internal position = pos / frequency / channels / samplesize/ AVStream::time_base
	int64_t internalPos = av_rescale( pos, this->pStream_->time_base.den, this->pStream_->time_base.num * this->getAudioFormat().frequency() * this->getAudioFormat().channels() * this->getAudioFormat().sampleSize() / 8 );
	int ret = av_seek_frame( this->pFormatContext_.get(), this->pStream_->index, internalPos, AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD );
	if( ret >= 0 ) {
		avcodec_flush_buffers( this->pCodecContext_.get() );
		this->buffer_.clear();
		this->curPos_ = pos;
		//this->msCurrent_ = msPos;
		//if( this->pStream_->cur_pkt.pts != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
		//	this->msCurrent_ = av_rescale( this->pStream_->cur_pkt.pts, this->pStream_->time_base.num * 1000, this->pStream_->time_base.den );
		//}
	}
	return succeed && ret >= 0;
}
