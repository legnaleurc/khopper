/**
 * @file defaultwriter.cpp
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
#include "common/tr1.hpp"
#include "common/error.hpp"
#include "common/text.hpp"
#include "defaultwriter.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <cstring>

namespace {

	inline std::string wHelper( const std::wstring & filePath ) {
#ifdef _WIN32
		return std::string( "wfile://" ) + khopper::text::toUtf8( filePath );
#else
		return khopper::text::toUtf8( filePath );
#endif
	}

	inline void fc_helper( AVFormatContext * oc ) {
		for( std::size_t i = 0; i < oc->nb_streams; ++i ) {
			if( oc->streams[i] ) {
				avcodec_close( oc->streams[i]->codec );
			}
			av_freep( &oc->streams[i]->codec );
			av_freep( &oc->streams[i] );
		}

		AVOutputFormat * pOF = oc->oformat;
		if( !( pOF->flags & AVFMT_NOFILE ) ) {
			url_fclose( oc->pb );
		}

		av_freep( &oc );
	}

	inline bool initFFmpeg() {
		av_register_all();
		return true;
	}

	const bool INITIALIZED = initFFmpeg();

}

namespace khopper {

	namespace codec {

		DefaultWriter::DefaultWriter():
		pFormatContext_(),
		pStream_( NULL ) {
		}

		DefaultWriter::~DefaultWriter() {
			if( this->isOpen() ) {
				this->close();
			}
		}

		void DefaultWriter::setupMuxer_() {
			AVOutputFormat * pOF = guess_format( NULL, ::wHelper( this->getFilePath() ).c_str(), NULL );
			if( pOF == NULL ) {
				throw error::CodecError( "Can not recognize output format" );
			}

#if LIBAVFORMAT_VERSION_MAJOR < 53
			this->pFormatContext_.reset( av_alloc_format_context(), ::fc_helper );
#else
			this->pFormatContext_.reset( avformat_alloc_context(), ::fc_helper );
#endif
			if( !this->pFormatContext_ ) {
				throw error::SystemError( "Memory allocation error" );
			}
			this->pFormatContext_->oformat = pOF;

			std::strncpy( this->pFormatContext_->filename, text::toUtf8( this->getFilePath() ).c_str(), sizeof( this->pFormatContext_->filename ) );
		}

		void DefaultWriter::setupEncoder_() {
			AVOutputFormat * pOF = this->pFormatContext_->oformat;
			if( pOF->audio_codec == CODEC_ID_NONE ) {
				throw error::CodecError( "Can not setup encoder" );
			}

			this->pStream_ = av_new_stream( this->pFormatContext_.get(), 1 );
			if( !this->pStream_ ) {
				throw error::CodecError( "Can not create stream" );
			}

			AVCodecContext * pCC = this->pStream_->codec;
			pCC->codec_id = pOF->audio_codec;
			pCC->codec_type = CODEC_TYPE_AUDIO;

			// setting special parameters
			pCC->bit_rate = this->getBitRate();
			pCC->sample_rate = this->getSampleRate();
			pCC->channels = this->getChannels();
			if( this->getQuality() > -1 ) {
				pCC->flags |= CODEC_FLAG_QSCALE;
				this->pStream_->quality = static_cast< float >( FF_QP2LAMBDA * this->getQuality() );
				pCC->global_quality = static_cast< int >( this->pStream_->quality );
			}
			if( av_set_parameters( this->pFormatContext_.get(), NULL ) < 0 ) {
				throw error::CodecError( "Set parameters failed" );
			}
			// NOTE: set complete

			AVCodec * pC = avcodec_find_encoder( pCC->codec_id );
			if( !pC ) {
				throw error::CodecError( "Find no encoder" );
			}

			if( avcodec_open( pCC, pC ) < 0 ) {
				throw error::CodecError( "Can not open encoder" );
			}

			this->getSampleBuffer().resize( pCC->frame_size * sizeof( short ) * pCC->channels );
		}

		void DefaultWriter::openResource_() {
			AVOutputFormat * pOF = this->pFormatContext_->oformat;
			if( !( pOF->flags & AVFMT_NOFILE ) ) {
				if( url_fopen( &this->pFormatContext_->pb, ::wHelper( this->getFilePath() ).c_str(), URL_WRONLY ) < 0 ) {
					throw error::IOError( std::string( "Can not open file: `" ) + text::toUtf8( this->getFilePath() ) + "\'" );
				}
			}
		}

		void DefaultWriter::closeResource_() {
			av_write_trailer( this->pFormatContext_.get() );
			this->pFormatContext_.reset();
		}

		void DefaultWriter::writeHeader_() {
			std::strncpy( this->pFormatContext_->title, this->getTitle().c_str(), sizeof( this->pFormatContext_->title ) );
			std::strncpy( this->pFormatContext_->author, this->getArtist().c_str(), sizeof( this->pFormatContext_->author ) );
			std::strncpy( this->pFormatContext_->album, this->getAlbum().c_str(), sizeof( this->pFormatContext_->album ) );

			av_write_header( this->pFormatContext_.get() );
		}

		void DefaultWriter::writeFrame_( const char * sample, std::size_t /*nSamples*/ ) {
			AVCodecContext * pCC = this->pStream_->codec;

			static uint8_t audio_outbuf[FF_MIN_BUFFER_SIZE * 4];

			AVPacket pkt;
			av_init_packet( &pkt );
			pkt.data = audio_outbuf;
			pkt.stream_index = this->pStream_->index;
			pkt.flags |= PKT_FLAG_KEY;

			pkt.size = avcodec_encode_audio( pCC, audio_outbuf, sizeof( audio_outbuf ), static_cast< const short * >( static_cast< const void * >( sample ) ) );

			if( pCC->coded_frame->pts != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
				pkt.pts = av_rescale_q( pCC->coded_frame->pts, pCC->time_base, this->pStream_->time_base );
			}

			if( av_write_frame( this->pFormatContext_.get(), &pkt ) != 0 ) {
				throw error::CodecError( "Can not write frame" );
			}
		}

	}

}
