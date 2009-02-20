/**
 * @file defaultaudiowriter.cpp
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
#include "defaultaudiowriter.hpp"
#include "error.hpp"

extern "C" {
#include <avcodec.h>
#include <avformat.h>
}

namespace {

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

}

namespace Khopper {

	namespace codec {

		DefaultAudioWriter::DefaultAudioWriter():
		samples_(),
		pFormatContext_(),
		pStream_( NULL ) {
		}

		DefaultAudioWriter::~DefaultAudioWriter() {
			if( this->isOpen() ) {
				this->close();
			}
		}

		void DefaultAudioWriter::setupMuxer_() {
			AVOutputFormat * pOF = guess_format( NULL, this->getFilePath().c_str(), NULL );
			if( pOF == NULL ) {
				throw Error< Codec >( "Can not recognize output format" );
			}

			this->pFormatContext_.reset( av_alloc_format_context(), ::fc_helper );
			if( !this->pFormatContext_ ) {
				throw Error< System >( "Memory allocation error" );
			}
			this->pFormatContext_->oformat = pOF;

			std::strncpy( this->pFormatContext_->filename, this->getFilePath().c_str(), sizeof( this->pFormatContext_->filename ) );
		}

		void DefaultAudioWriter::setupEncoder_() {
			AVOutputFormat * pOF = this->pFormatContext_->oformat;
			if( pOF->audio_codec == CODEC_ID_NONE ) {
				throw Error< Codec >( "Can not setup encoder" );
			}

			this->pStream_ = av_new_stream( this->pFormatContext_.get(), 1 );
			if( !this->pStream_ ) {
				throw Error< Codec >( "Can not create stream" );
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
				pCC->global_quality = this->pStream_->quality = FF_QP2LAMBDA * this->getQuality();
			}
			if( av_set_parameters( this->pFormatContext_.get(), NULL ) < 0 ) {
				throw Error< Codec >( "Set parameters failed" );
			}
			// NOTE: set complete

			AVCodec * pC = avcodec_find_encoder( pCC->codec_id );
			if( !pC ) {
				throw Error< Codec >( "Find no encoder" );
			}

			if( avcodec_open( pCC, pC ) < 0 ) {
				throw Error< Codec >( "Can not open encoder" );
			}

			this->getSampleBuffer().resize( pCC->frame_size * sizeof( short ) * pCC->channels );
		}

		void DefaultAudioWriter::openResouse_() {
			AVOutputFormat * pOF = this->pFormatContext_->oformat;
			if( !( pOF->flags & AVFMT_NOFILE ) ) {
				if( url_fopen( &this->pFormatContext_->pb, this->getFilePath().c_str(), URL_WRONLY ) < 0 ) {
					throw Error< IO >( std::string( "Can not open file: `" ) + this->getFilePath() + "\'" );
				}
			}
		}

		void DefaultAudioWriter::closeResouse_() {
			av_write_trailer( this->pFormatContext_.get() );
			this->pFormatContext_.reset();
		}

		void DefaultAudioWriter::writeHeader_() {
			std::strncpy( this->pFormatContext_->title, this->getTitle().c_str(), sizeof( this->pFormatContext_->title ) );
			std::strncpy( this->pFormatContext_->author, this->getArtist().c_str(), sizeof( this->pFormatContext_->author ) );
			std::strncpy( this->pFormatContext_->album, this->getAlbum().c_str(), sizeof( this->pFormatContext_->album ) );

			av_write_header( this->pFormatContext_.get() );
		}

		void DefaultAudioWriter::writeFrame_( const char * sample, std::size_t /*nSamples*/ ) {
			AVCodecContext * pCC = this->pStream_->codec;

			uint8_t audio_outbuf[FF_MIN_BUFFER_SIZE];

			AVPacket pkt;
			av_init_packet( &pkt );
			pkt.data = audio_outbuf;
			pkt.stream_index = this->pStream_->index;
			pkt.flags |= PKT_FLAG_KEY;

			// casting hack
			const void * samples = sample;
			pkt.size = avcodec_encode_audio( pCC, audio_outbuf, sizeof( audio_outbuf ), static_cast< const short * >( samples ) );

			if( pCC->coded_frame->pts != AV_NOPTS_VALUE ) {
				pkt.pts = av_rescale_q( pCC->coded_frame->pts, pCC->time_base, this->pStream_->time_base );
			}

			if( av_write_frame( this->pFormatContext_.get(), &pkt ) != 0 ) {
				throw Error< Codec >( "Can not write frame" );
			}
		}

	}

}