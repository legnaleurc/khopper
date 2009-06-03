/**
 * @file defaultwriter.cpp
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
#include "codec/defaultwriter.hpp"
#include "util/error.hpp"
#include "util/text.hpp"

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
			if( oc->streams[i] && oc->streams[i]->codec->codec ) {
				avcodec_close( oc->streams[i]->codec );
			}
			av_freep( &oc->streams[i]->codec );
			av_freep( &oc->streams[i] );
		}

		AVOutputFormat * pOF = oc->oformat;
		if( !( pOF->flags & AVFMT_NOFILE ) && oc->pb ) {
			url_fclose( oc->pb );
		}

		av_freep( &oc );
	}

	inline bool initFFmpeg() {
		av_register_all();
		return true;
	}

	const bool INITIALIZED = initFFmpeg();
	const double QSCALE_NONE = -99999.;
	const int OUTPUT_BUFFER_SIZE = FF_MIN_BUFFER_SIZE * 4;

}

namespace khopper {

	namespace codec {

		DefaultWriter::DefaultWriter():
		pFormatContext_(),
		pStream_( NULL ) {
			this->setQuality( QSCALE_NONE );
		}

		DefaultWriter::~DefaultWriter() {
			if( this->isOpen() ) {
				this->close();
			}
		}

		bool DefaultWriter::isVariable() const {
			return this->getQuality() != QSCALE_NONE;
		}

		void DefaultWriter::setupMuxer() {
			AVOutputFormat * pOF = guess_format( NULL, ::wHelper( this->getFilePath() ).c_str(), NULL );
			if( pOF == NULL ) {
				throw error::CodecError( "Can not recognize output format" );
			}

			this->pFormatContext_.reset( avformat_alloc_context(), ::fc_helper );
			if( !this->pFormatContext_ ) {
				throw error::SystemError( "Memory allocation error" );
			}
			this->pFormatContext_->oformat = pOF;

			std::strncpy( this->pFormatContext_->filename, text::toUtf8( this->getFilePath() ).c_str(), sizeof( this->pFormatContext_->filename ) );
		}

		void DefaultWriter::setupEncoder() {
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
			pCC->time_base.num = 1;
			pCC->time_base.den = this->getSampleRate();

			// setting special parameters
			pCC->bit_rate = this->getBitRate();
			pCC->sample_rate = this->getSampleRate();
			pCC->channels = this->getChannels();
			if( this->getQuality() != QSCALE_NONE ) {
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

			int bufferSize = 0;
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
				bufferSize = OUTPUT_BUFFER_SIZE;
				break;
			default:
				bufferSize = pCC->frame_size * sizeof( short ) * pCC->channels;
				break;
			}

			this->getSampleBuffer().resize( bufferSize );
		}

		void DefaultWriter::openResource() {
			AVOutputFormat * pOF = this->pFormatContext_->oformat;
			if( !( pOF->flags & AVFMT_NOFILE ) ) {
				if( url_fopen( &this->pFormatContext_->pb, ::wHelper( this->getFilePath() ).c_str(), URL_WRONLY ) < 0 ) {
					throw error::IOError( std::wstring( L"Can not open file: `" ) + this->getFilePath() + L"\'" );
				}
			}
		}

		void DefaultWriter::closeResource() {
			this->writeFrame( NULL, 0 );
			av_write_trailer( this->pFormatContext_.get() );
			this->pFormatContext_.reset();
		}

		void DefaultWriter::writeHeader() {
			av_metadata_set( &this->pFormatContext_->metadata, "title", this->getTitle().c_str() );
			av_metadata_set( &this->pFormatContext_->metadata, "author", this->getArtist().c_str() );
			av_metadata_set( &this->pFormatContext_->metadata, "album", this->getAlbum().c_str() );

			if( av_write_header( this->pFormatContext_.get() ) < 0 ) {
				throw error::CodecError( "Can not write header" );
			}
		}

		void DefaultWriter::writeFrame( const char * sample, std::size_t /*nSamples*/ ) {
			AVCodecContext * pCC = this->pStream_->codec;

			static uint8_t audio_outbuf[OUTPUT_BUFFER_SIZE];

			AVPacket pkt;
			av_init_packet( &pkt );

			pkt.size = avcodec_encode_audio( pCC, audio_outbuf, sizeof( audio_outbuf ), static_cast< const short * >( static_cast< const void * >( sample ) ) );

			pkt.data = audio_outbuf;
			pkt.stream_index = this->pStream_->index;
			pkt.flags |= PKT_FLAG_KEY;

			if( pCC->coded_frame->pts != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
				pkt.pts = av_rescale_q( pCC->coded_frame->pts, pCC->time_base, this->pStream_->time_base );
			}

			if( av_interleaved_write_frame( this->pFormatContext_.get(), &pkt ) != 0 ) {
				throw error::CodecError( "Can not write frame" );
			}
		}

	}

}
