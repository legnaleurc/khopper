/**
 * @file defaultreader.cpp
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
#include "tr1.hpp"
#include "defaultreader.hpp"

extern "C" {
#include <avcodec.h>
#include <avformat.h>
}

#include <QtPlugin>

namespace {

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

	const bool INITIALIZED = khopper::plugin::registerReader( "*", "krp_default" ) && initFFmpeg();

}

Q_EXPORT_PLUGIN2( krp_default, khopper::plugin::DefaultReaderCreator )

namespace khopper {

	namespace codec {

		DefaultReader::DefaultReader():
		pFormatContext_(),
		pCodecContext_(),
		pPacket_( static_cast< AVPacket * >( av_malloc( sizeof( AVPacket ) ) ), ::p_helper ),
		timeBase_( 0.0 ) {
			av_init_packet( this->pPacket_.get() );
		}

		DefaultReader::~DefaultReader() {
			if( this->isOpen() ) {
				this->close();
			}
		}

		void DefaultReader::openResource_() {
			AVFormatContext * pFC = NULL;
			if( av_open_input_file( &pFC, this->getFilePath().c_str(), NULL, 0, NULL ) != 0 ) {
				throw Error< IO >( std::string( "Can not open `" ) + this->getFilePath() + "\'" );
			}
			this->pFormatContext_.reset( pFC, av_close_input_file );
		}

		void DefaultReader::setupDemuxer_() {
			if( av_find_stream_info( this->pFormatContext_.get() ) < 0 ) {
				throw Error< Codec >( "Can not find codec info!" );
			}

			if( this->pFormatContext_->duration != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
				this->setDuration( this->pFormatContext_->duration / static_cast< double >( AV_TIME_BASE ) );
			} else {
				throw Error< Codec >( "Can not get duration!" );
			}
		}

		void DefaultReader::setupDecoder_() {
			int a_stream = -1;
			for( std::size_t i = 0 ; i < this->pFormatContext_->nb_streams; ++i ) {
				if( this->pFormatContext_->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO ) {
					a_stream = i;
					break;
				}
			}
			if( a_stream == -1 ) {
				throw Error< Codec >( "Find no audio stream!" );
			}
			AVCodecContext * pCC = this->pFormatContext_->streams[a_stream]->codec;
			// getting codec information
			this->timeBase_ = av_q2d( this->pFormatContext_->streams[a_stream]->time_base );
			this->setBitRate( pCC->bit_rate );
			this->setSampleRate( pCC->sample_rate );
			this->setChannels( pCC->channels );

			AVCodec * pC = avcodec_find_decoder( pCC->codec_id );
			if( pC == NULL ) {
				throw Error< Codec >( "Find no decoder!" );
			}

			if( avcodec_open( pCC, pC ) < 0 ) {
				throw Error< Codec >( "Can not open decoder." );
			}
			this->pCodecContext_.reset( pCC, avcodec_close );
		}

		void DefaultReader::readHeader_() {
			this->setTitle( this->pFormatContext_->title );
			this->setArtist( this->pFormatContext_->author );
			this->setCopyright( this->pFormatContext_->copyright );
			this->setComment( this->pFormatContext_->comment );
			this->setAlbum( this->pFormatContext_->album );
			this->setYear( this->pFormatContext_->year );
			this->setIndex( this->pFormatContext_->track );
			this->setGenre( this->pFormatContext_->genre );
		}

		void DefaultReader::closeResource_() {
			// clear native information
			this->timeBase_ = 0.0;
			// free the members in packet, not itself
			av_free_packet( this->pPacket_.get() );
			av_init_packet( this->pPacket_.get() );
			this->pCodecContext_.reset();
			this->pFormatContext_.reset();
		}

		ByteArray DefaultReader::readFrame_( double & duration, bool & stop ) {
			stop = false;
			// static just for speed
			static uint8_t audio_buf[AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2];
			void * casting_hack = audio_buf;
			uint8_t * audio_pkt_data = NULL;
			int audio_pkt_size = 0;

			int dp_len, data_size;

			ByteArray data;

			if( av_read_frame( this->pFormatContext_.get(), this->pPacket_.get() ) >= 0 ) {
				audio_pkt_data = this->pPacket_->data;
				audio_pkt_size = this->pPacket_->size;
				int64_t curPts = -1;
				int64_t decoded = 0;
				if( this->pPacket_->pts != static_cast< int64_t >( AV_NOPTS_VALUE ) ) {
					curPts = av_rescale( this->pPacket_->pts, AV_TIME_BASE * static_cast< int64_t >( this->pFormatContext_->streams[0]->time_base.num ), this->pFormatContext_->streams[0]->time_base.den );
				}
				while( audio_pkt_size > 0 ) {
					if( this->afterEnd( ::toGeneral( curPts ) ) ) {
						stop = true;
						break;
					}
					data_size = sizeof( audio_buf );
					dp_len = avcodec_decode_audio2( this->pCodecContext_.get(), static_cast< int16_t * >( casting_hack ), &data_size, audio_pkt_data, audio_pkt_size );
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
					if( this->afterBegin( ::toGeneral( curPts ) ) ) {
						data.insert( data.end(), audio_buf, audio_buf + data_size );
						decoded += ptsDiff;
					}
					curPts += ptsDiff;
				}
				if( this->pPacket_->data ) {
					av_free_packet( this->pPacket_.get() );
				}

				duration = static_cast< double >( decoded ) / AV_TIME_BASE;
			}

			return data;
		}

		bool DefaultReader::seek_( double timestamp ) {
			bool succeed = av_seek_frame( this->pFormatContext_.get(), -1, ::toNative( timestamp ), AVSEEK_FLAG_BACKWARD ) >= 0;
			if( succeed ) {
				avcodec_flush_buffers( this->pCodecContext_.get() );
			}
			return succeed;
		}

	}

	namespace plugin {

		codec::ReaderSP DefaultReaderCreator::create() const {
			return codec::ReaderSP( new codec::DefaultReader );
		}

	}

}
