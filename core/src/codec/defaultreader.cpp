/**
 * @file defaultreader.cpp
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
#include "codec/defaultreader.hpp"
#include "util/error.hpp"

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

	static inline bool initFFmpeg() {
		av_register_all();
		return true;
	}

	static const bool INITIALIZED = initFFmpeg();

}

namespace khopper {

	namespace codec {

		DefaultReader::DefaultReader():
		pFormatContext_(),
		pCodecContext_(),
		pPacket_( static_cast< AVPacket * >( av_malloc( sizeof( AVPacket ) ) ), ::p_helper ),
		pStream_( NULL ) {
			av_init_packet( this->pPacket_.get() );
		}

		void DefaultReader::doOpen() {
			this->openResource();
			this->setupDemuxer();
			this->setupDecoder();
			this->readHeader();
		}

		void DefaultReader::doClose() {
			this->closeResource();
		}

		void DefaultReader::openResource() {
			AVFormatContext * pFC = NULL;
			int ret = av_open_input_file( &pFC, wHelper( this->getURI() ).c_str(), NULL, 0, NULL );
			if( ret != 0 ) {
				throw error::IOError(
					QString(
						"Can not open `%1\':\n"
						"%2"
					).arg( this->getURI().toString() ).arg( strerror( AVUNERROR( ret ) ) )
				);
			}
			this->pFormatContext_.reset( pFC, av_close_input_file );
		}

		void DefaultReader::setupDemuxer() {
			if( av_find_stream_info( this->pFormatContext_.get() ) < 0 ) {
				throw error::CodecError( "Can not find codec info!" );
			}

			if( this->pFormatContext_->duration != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
				this->setDuration( toMS( this->pFormatContext_->duration ) );
			} else {
				throw error::CodecError( "Can not get duration!" );
			}
		}

		void DefaultReader::setupDecoder() {
			int a_stream = -1;
			for( std::size_t i = 0 ; i < this->pFormatContext_->nb_streams; ++i ) {
				if( this->pFormatContext_->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO ) {
					a_stream = i;
					break;
				}
			}
			if( a_stream == -1 ) {
				throw error::CodecError( "Find no audio stream!" );
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

		void DefaultReader::readHeader() {
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
				this->setYear( std::atoi( mt->value ) );
			}
			if( ( mt = av_metadata_get( this->pFormatContext_->metadata, "track", NULL, 0 ) ) ) {
				this->setIndex( std::atoi( mt->value ) );
			}
			if( ( mt = av_metadata_get( this->pFormatContext_->metadata, "genre", NULL, 0 ) ) ) {
				this->setGenre( mt->value );
			}
		}

		void DefaultReader::closeResource() {
			// clear native information
			this->pStream_ = NULL;
			// free the members in packet, not itself
			av_free_packet( this->pPacket_.get() );
			av_init_packet( this->pPacket_.get() );
			this->pCodecContext_.reset();
			this->pFormatContext_.reset();
		}

		ByteArray DefaultReader::readFrame( int64_t & duration, bool & stop ) {
			stop = false;
			int16_t sampleBuffer[AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2];

			ByteArray data;

			// read a frame
			int ret = av_read_frame( this->pFormatContext_.get(), this->pPacket_.get() );
			if( ret < 0 ) {
				stop = true;
				return data;
			}
			// current presentation timestamp: in second * AV_TIME_BASE
			int64_t curPts = -1;
			// decoded time: in second * AV_TIME_BASE
			int64_t decoded = 0;
			if( this->pPacket_->pts != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
				// rescale presentation timestamp
				curPts = AV_TIME_BASE * av_rescale(
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
				if( this->afterEnd( toMS( curPts ) ) ) {
					stop = true;
					break;
				}
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
				if( this->afterBegin( toMS( curPts ) ) ) {
					const uint8_t * tmp = static_cast< uint8_t * >( static_cast< void * >( sampleBuffer ) );
					data.insert( data.end(), tmp, tmp + sampleByteLength );
					decoded += ptsDiff;
				}
				curPts += ptsDiff;
			}
#if LIBAVCODEC_VERSION_MAJOR < 53
			if( this->pPacket_->data ) {
#else
			if( pktDataBackup ) {
				this->pPacket_->data = pktDataBackup;
#endif
				av_free_packet( this->pPacket_.get() );
			}

			duration = toMS( decoded );

			return data;
		}

		bool DefaultReader::seekFrame( int64_t ms ) {
			ms = av_rescale( ms, pStream_->time_base.den, pStream_->time_base.num );
			int succeed = av_seek_frame( this->pFormatContext_.get(), pStream_->index, toNative( ms ), AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD );
			if( succeed >= 0 ) {
				avcodec_flush_buffers( this->pCodecContext_.get() );
			}
			return succeed >= 0;
		}

	}

}
