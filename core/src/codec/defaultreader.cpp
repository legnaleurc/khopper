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
#include "util/text.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <cstdlib>

namespace {

	inline std::string wHelper( const std::wstring & filePath ) {
#ifdef _WIN32
		return std::string( "wfile://" ) + khopper::text::toUtf8( filePath );
#else
		return khopper::text::toUtf8( filePath );
#endif
	}

	inline void p_helper( AVPacket * p ) {
		av_freep( &p );
	}

	inline int64_t toNative( double timestamp ) {
		return timestamp * AV_TIME_BASE;
	}

	inline double toGeneral( int64_t timestamp ) {
		return static_cast< double >( timestamp ) / AV_TIME_BASE;
	}

	inline bool initFFmpeg() {
		av_register_all();
		return true;
	}

	const bool INITIALIZED = initFFmpeg();

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

		DefaultReader::~DefaultReader() {
			if( this->isOpen() ) {
				this->close();
			}
		}

		void DefaultReader::openResource() {
			AVFormatContext * pFC = NULL;
			int ret = av_open_input_file( &pFC, ::wHelper( this->getFilePath() ).c_str(), NULL, 0, NULL );
			if( ret != 0 ) {
				throw error::IOError(
					std::wstring( L"Can not open `" ) + this->getFilePath() + L"\':\n" +
					L"->" + text::toStdWString( strerror( AVUNERROR( ret ) ) )
				);
			}
			this->pFormatContext_.reset( pFC, av_close_input_file );
		}

		void DefaultReader::setupDemuxer() {
			if( av_find_stream_info( this->pFormatContext_.get() ) < 0 ) {
				throw error::CodecError( "Can not find codec info!" );
			}

			if( this->pFormatContext_->duration != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
				this->setDuration( this->pFormatContext_->duration / static_cast< double >( AV_TIME_BASE ) );
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
			this->setTimebase( Rational( this->pStream_->time_base.num, this->pStream_->time_base.den ) );
			this->setBitRate( pCC->bit_rate );
			this->setSampleRate( pCC->sample_rate );
			this->setChannels( pCC->channels );

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

		ByteArray DefaultReader::readFrame( double & duration, bool & stop ) {
			stop = false;
			// static just for speed
			static uint8_t audio_buf[AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2];
			int ret = 0;

			int dp_len, data_size;

			ByteArray data;

			ret = av_read_frame( this->pFormatContext_.get(), this->pPacket_.get() );
			if( ret < 0 ) {
				stop = true;
				return data;
			}
			int64_t curPts = -1;
			int64_t decoded = 0;
			if( this->pPacket_->pts != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
				curPts = av_rescale(
					this->pPacket_->pts,
					AV_TIME_BASE * static_cast< int64_t >( this->pStream_->time_base.num ),
					this->pStream_->time_base.den
				);
			}
#if LIBAVCODEC_VERSION_MAJOR < 53
			uint8_t * audio_pkt_data = this->pPacket_->data;
			int audio_pkt_size = this->pPacket_->size;
			while( audio_pkt_size > 0 ) {
#else
			uint8_t * pktDataBackup = this->pPacket_->data;
			while( this->pPacket_->size > 0 ) {
#endif
				if( this->afterEnd( toGeneral( curPts ) ) ) {
					stop = true;
					break;
				}
				data_size = sizeof( audio_buf );
#if LIBAVCODEC_VERSION_MAJOR < 53
				dp_len = avcodec_decode_audio2(
					this->pCodecContext_.get(),
					static_cast< int16_t * >( static_cast< void * >( audio_buf ) ),
					&data_size,
					audio_pkt_data,
					audio_pkt_size
				);
#else
				dp_len = avcodec_decode_audio3(
					this->pCodecContext_.get(),
					static_cast< int16_t * >( static_cast< void * >( audio_buf ) ),
					&data_size,
					this->pPacket_.get()
				);
#endif
				if( dp_len < 0 ) {
#if LIBAVCODEC_VERSION_MAJOR < 53
					audio_pkt_size = 0;
#endif
					break;
				}
#if LIBAVCODEC_VERSION_MAJOR < 53
				audio_pkt_data += dp_len;
				audio_pkt_size -= dp_len;
#else
				this->pPacket_->data += dp_len;
				this->pPacket_->size -= dp_len;
#endif
				if( data_size <= 0 ) {
					continue;
				}
				int64_t ptsDiff = (static_cast< int64_t >( AV_TIME_BASE )/2 * data_size) / (this->pCodecContext_->sample_rate * this->pCodecContext_->channels);
				if( this->afterBegin( toGeneral( curPts ) ) ) {
					data.insert( data.end(), audio_buf, audio_buf + data_size );
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

			duration = static_cast< double >( decoded ) / AV_TIME_BASE;

			return data;
		}

		bool DefaultReader::seekFrame( double timestamp ) {
			timestamp = av_rescale( timestamp, pStream_->time_base.den, pStream_->time_base.num );
			int succeed = av_seek_frame( this->pFormatContext_.get(), pStream_->index, timestamp, AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD );
			if( succeed >= 0 ) {
				avcodec_flush_buffers( this->pCodecContext_.get() );
			}
			return succeed >= 0;
		}

	}

}
