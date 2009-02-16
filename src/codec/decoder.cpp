/**
 * @file decoder.cpp
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
#include "decoder.hpp"
#include "error.hpp"
#include "os.hpp"

extern "C" {
#include <avcodec.h>
#include <avformat.h>
}

#include <QtDebug>

namespace {

	inline void p_helper( AVPacket * p ) {
		av_freep( &p );
	}

}

namespace Khopper {

	Decoder::Decoder():
	opening_( false ),
	begin_( -1 ),
	end_( -1 ),
	hasNext_( false ),
	pFormatContext_(),
	pCodecContext_(),
	pPacket_( static_cast< AVPacket * >( av_malloc( sizeof( AVPacket ) ) ), p_helper ),
	timeBase_( 0.0 ),
	duration_( 0.0 ),
	title_(),
	artist_(),
	copyright_(),
	comment_(),
	album_(),
	year_( 0 ),
	index_( 0 ),
	genre_(),
	bitRate_( 0 ),
	sampleRate_( 0 ),
	channels_( 0 ) {
		av_init_packet( this->pPacket_.get() );
	}

	Decoder::~Decoder() {
		if( this->opening_ ) {
			this->close();
		}
	}

	void Decoder::open( const std::wstring & filePath ) {
		if( this->opening_ ) {
			this->close();
		}
		std::string fP = os::encodeString( filePath );

		AVFormatContext * pFC = NULL;
		if( av_open_input_file( &pFC, fP.c_str(), NULL, 0, NULL ) != 0 ) {
			throw Error< IO >( std::wstring( L"Can not open `" ) + filePath + L"\'!" );
		}
		this->pFormatContext_.reset( pFC, av_close_input_file );
		if( av_find_stream_info( this->pFormatContext_.get() ) < 0 ) {
			throw Error< Codec >( "Can not find codec info!" );
		}
		// NOTE: format recognization is done

		// getting format information
		if( this->pFormatContext_->duration != AV_NOPTS_VALUE ) {
			this->duration_ = this->pFormatContext_->duration / static_cast< double >( AV_TIME_BASE );
		} else {
			throw Error< Codec >( "Can not get duration!" );
		}
		this->title_ = this->pFormatContext_->title;
		this->artist_ = this->pFormatContext_->author;
		this->copyright_ = this->pFormatContext_->copyright;
		this->comment_ = this->pFormatContext_->comment;
		this->album_ = this->pFormatContext_->album;
		this->year_ = this->pFormatContext_->year;
		this->index_ = this->pFormatContext_->track;
		this->genre_ = this->pFormatContext_->genre;

		int a_stream = -1;
		for( std::size_t i = 0 ; i < pFormatContext_->nb_streams; ++i ) {
			if( pFormatContext_->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO ) {
				a_stream = i;
				break;
			}
		}
		if( a_stream == -1 ) {
			throw Error< Codec >( "Find no audio stream!" );
		}
		AVCodecContext * pCC = pFormatContext_->streams[a_stream]->codec;
		// getting codec information
		this->timeBase_ = av_q2d( this->pFormatContext_->streams[a_stream]->time_base );
		this->bitRate_ = pCC->bit_rate;
		this->sampleRate_ = pCC->sample_rate;
		this->channels_ = pCC->channels;

		AVCodec * pC = avcodec_find_decoder( pCC->codec_id );
		if( pC == NULL ) {
			throw Error< Codec >( "Find no decoder!" );
		}

		if( avcodec_open( pCC, pC ) < 0 ) {
			throw Error< Codec >( "Can not open decoder." );
		}
		pCodecContext_.reset( pCC, avcodec_close );

		opening_ = true;
		hasNext_ = true;
	}

	void Decoder::close() {
		// clear format information
		this->duration_ = 0.0;
		this->title_.clear();
		this->artist_.clear();
		this->copyright_.clear();
		this->comment_.clear();
		this->album_.clear();
		this->year_ = 0;
		this->index_ = 0;
		this->genre_.clear();
		this->bitRate_ = 0;
		this->sampleRate_ = 0;
		this->channels_ = 0;

		// clear native information
		this->timeBase_ = 0.0;
		this->begin_ = -1;
		this->end_ = -1;
		this->hasNext_ = false;
		this->opening_ = false;
		// free the members in packet, not itself
		av_free_packet( this->pPacket_.get() );
		av_init_packet( this->pPacket_.get() );
		this->pCodecContext_.reset();
		this->pFormatContext_.reset();
	}

	void Decoder::setRange( double begin, double end ) {
		this->begin_ = begin * AV_TIME_BASE;
		this->end_ = end * AV_TIME_BASE;
		this->seek( begin );
	}

	ByteArray Decoder::read( double & duration ) {
		duration = 0.0;
		if( !this->opening_ || !this->hasNext_ ) {
			return ByteArray();
		}

		// static just for speed
		static uint8_t audio_buf[AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2];
		uint8_t * audio_pkt_data = NULL;
		int audio_pkt_size = 0;

		int dp_len, data_size;

		ByteArray data;

		if( av_read_frame( this->pFormatContext_.get(), this->pPacket_.get() ) >= 0 ) {
			audio_pkt_data = this->pPacket_->data;
			audio_pkt_size = this->pPacket_->size;
			int64_t curPts = -1;
			int64_t decoded = 0;
			if( this->pPacket_->pts != AV_NOPTS_VALUE ) {
				curPts = av_rescale( this->pPacket_->pts, AV_TIME_BASE * static_cast< int64_t >( this->pFormatContext_->streams[0]->time_base.num ), this->pFormatContext_->streams[0]->time_base.den );
			}
			while( audio_pkt_size > 0 ) {
				if( this->afterEnd_( curPts ) ) {
					this->hasNext_ = false;
					break;
				}
				data_size = sizeof( audio_buf );
				dp_len = avcodec_decode_audio2( this->pCodecContext_.get(), reinterpret_cast< int16_t * >( audio_buf ), &data_size, audio_pkt_data, audio_pkt_size );
				if( dp_len < 0 ) {
					audio_pkt_size = 0;
					break;
				}
				audio_pkt_data += dp_len;
				audio_pkt_size -= dp_len;
				if( data_size <= 0 ) {
					continue;
				}
				int64_t ptsDiff = (static_cast< int64_t >( AV_TIME_BASE )/2 * data_size) / (this->pCodecContext_->sample_rate * this->pCodecContext_->channels);
				if( this->afterBegin_( curPts ) ) {
					data.insert( data.end(), audio_buf, audio_buf + data_size );
					decoded += ptsDiff;
				}
				curPts += ptsDiff;
			}
			if( this->pPacket_->data ) {
				av_free_packet( this->pPacket_.get() );
			}

			duration = static_cast< double >( decoded ) / AV_TIME_BASE;
		} else {
			this->hasNext_ = false;
		}

		return data;
	}

	bool Decoder::afterBegin_( int64_t pts ) const {
		if( this->begin_ < 0 ) {
			return true;
		}
		return pts >= this->begin_;
	}

	bool Decoder::afterEnd_( int64_t pts ) const {
		if( this->end_ < 0 ) {
			return false;
		}
		return pts >= this->end_;
	}

	bool Decoder::hasNext() const {
		return this->hasNext_;
	}

	bool Decoder::seek( double timestamp ) {
		bool succeed = av_seek_frame( this->pFormatContext_.get(), -1, timestamp * AV_TIME_BASE, AVSEEK_FLAG_BACKWARD ) >= 0;
		if( succeed ) {
			this->hasNext_ = true;
			avcodec_flush_buffers( this->pCodecContext_.get() );
		}
		return succeed;
	}

}
