/**
 * @file encoder.cpp
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
#include "encoder.hpp"
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

	Encoder::Encoder():
	opening_( false ),
	samples_(),
	pFormatContext_(),
	pStream_( NULL ),
	filePath_(),
	quality_( -1 ),
	bitRate_( -1 ),
	sampleRate_( -1 ),
	channels_( -1 ),
	title_(),
	author_() {
	}

	Encoder::~Encoder() {
		if( this->opening_ ) {
			this->close();
		}
	}

	void Encoder::setQuality( double quality ) {
		this->quality_ = quality;
	}

	void Encoder::setBitRate( int bit_rate ) {
		this->bitRate_ = bit_rate;
	}

	void Encoder::setSampleRate( int sample_rate ) {
		this->sampleRate_ = sample_rate;
	}

	void Encoder::setChannels( int channels ) {
		this->channels_ = channels;
	}

	void Encoder::setTitle( const std::wstring & title ) {
		this->title_ = os::encodeString( title, 1 );
	}

	void Encoder::setAuthor( const std::wstring & author ) {
		this->author_ = os::encodeString( author, 1 );
	}

	void Encoder::open( const std::wstring & filePath ) {
		if( this->opening_ ) {
			this->close();
		}

		this->filePath_ = os::encodeString( filePath );
		AVOutputFormat * pOF = this->guessFormat();
		if( pOF == NULL ) {
			throw Error< Codec >( "Can not recognize output format" );
		}

		this->pFormatContext_.reset( av_alloc_format_context(), ::fc_helper );
		if( !this->pFormatContext_ ) {
			throw Error< System >( "Memory allocation error" );
		}
		this->pFormatContext_->oformat = pOF;
		// setting format information
		std::strncpy( this->pFormatContext_->filename, this->filePath_.c_str(), sizeof( this->pFormatContext_->filename ) );
		std::strncpy( this->pFormatContext_->title, this->title_.c_str(), sizeof( this->pFormatContext_->title ) );
		std::strncpy( this->pFormatContext_->author, this->author_.c_str(), sizeof( this->pFormatContext_->author ) );

		AVCodecContext * pCC = NULL;

		if( pOF->audio_codec != CODEC_ID_NONE ) {
			this->pStream_ = av_new_stream( this->pFormatContext_.get(), 1 );
			if( !this->pStream_ ) {
				throw Error< Codec >( "Can not create stream" );
			}

			pCC = this->pStream_->codec;
			pCC->codec_id = pOF->audio_codec;
			pCC->codec_type = CODEC_TYPE_AUDIO;

			// setting special parameters
			pCC->bit_rate = this->bitRate_;
			pCC->sample_rate = this->sampleRate_;
			pCC->channels = this->channels_;

			if( this->quality_ > -1 ) {
				pCC->flags |= CODEC_FLAG_QSCALE;
				pCC->global_quality = this->pStream_->quality = FF_QP2LAMBDA * this->quality_;
			}
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

		if( !( pOF->flags & AVFMT_NOFILE ) ) {
			if( url_fopen( &this->pFormatContext_->pb, this->filePath_.c_str(), URL_WRONLY ) < 0 ) {
				throw Error< IO >( std::wstring( L"Can not open file: `" ) + filePath + L"\'" );
			}
		}

		av_write_header( this->pFormatContext_.get() );

		samples_.resize( pCC->frame_size * sizeof( short ) * pCC->channels );

		this->opening_ = true;
	}

	bool Encoder::is_open() const {
		return this->opening_;
	}

	void Encoder::close() {
		this->flush();
		av_write_trailer( this->pFormatContext_.get() );
		this->pFormatContext_.reset();

		this->closeHook();

		this->opening_ = false;
	}

	void Encoder::write( const ByteArray & data ) {
		this->buffer_.insert( this->buffer_.end(), data.begin(), data.end() );
		if( buffer_.size() < samples_.size() ) {
			return;
		}

		while( this->buffer_.size() >= samples_.size() ) {
			ByteArray::iterator copyEnd = this->buffer_.begin() + this->samples_.size();
			std::copy( this->buffer_.begin(), copyEnd, this->samples_.begin() );
			this->buffer_.erase( this->buffer_.begin(), copyEnd );

			this->writeFrame_( reinterpret_cast< short * >( &this->samples_[0] ) );
		}
	}

	void Encoder::flush() {
		if( this->opening_ && !this->buffer_.empty() ) {
			std::copy( this->buffer_.begin(), this->buffer_.end(), this->samples_.begin() );
			this->writeFrame_( reinterpret_cast< short * >( &this->samples_[0] ) );
			this->buffer_.clear();
		}
	}

	AVOutputFormat * Encoder::guessFormat() const {
		return guess_format( NULL, this->filePath_.c_str(), NULL );
	}

	void Encoder::writeFrame_( short * samples ) {
		AVCodecContext * pCC = this->pStream_->codec;

		uint8_t audio_outbuf[FF_MIN_BUFFER_SIZE];

		AVPacket pkt;
		av_init_packet( &pkt );
		pkt.data = audio_outbuf;
		pkt.stream_index = this->pStream_->index;
		pkt.flags |= PKT_FLAG_KEY;

		pkt.size = avcodec_encode_audio( pCC, audio_outbuf, sizeof( audio_outbuf ), samples );

		if( pCC->coded_frame->pts != AV_NOPTS_VALUE ) {
			pkt.pts = av_rescale_q( pCC->coded_frame->pts, pCC->time_base, this->pStream_->time_base );
		}

		if( av_write_frame( this->pFormatContext_.get(), &pkt ) != 0 ) {
			throw Error< Codec >( "Can not write frame" );
		}
	}

}
