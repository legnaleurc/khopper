#include "decoder.hpp"
#include "os.hpp"

extern "C" {
#include <avcodec.h>
#include <avformat.h>
}

namespace Khopper {

	Decoder::Decoder():
	opening_( false ),
	hasNext_( false ),
	pFormatContext_(),
	pCodecContext_(),
	pPacket_( static_cast< AVPacket * >( av_malloc( sizeof( AVPacket ) ) ), av_freep ),
	timeBase_( 0.0 ) {
		av_init_packet( this->pPacket_.get() );
	}

	Decoder::~Decoder() {
	}

	void Decoder::open( const std::wstring & filePath ) {
		std::string fP = os::encodeString( filePath );

		AVFormatContext * pFC = NULL;
		if( av_open_input_file( &pFC, fP.c_str(), NULL, 0, NULL ) != 0 ) {
			return;
		}
		pFormatContext_.reset( pFC, av_close_input_file );
		if( av_find_stream_info( pFormatContext_.get() ) < 0 ) {
			return;
		}
		// NOTE: format recognization is done

		int a_stream = -1;
		for( std::size_t i = 0 ; i < pFormatContext_->nb_streams; ++i ) {
			if( pFormatContext_->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO ) {
				a_stream = i;
				break;
			}
		}
		if( a_stream == -1 ) {
			return;
		}
		AVCodecContext * pCC = pFormatContext_->streams[a_stream]->codec;
		this->timeBase_ = av_q2d( this->pFormatContext_->streams[a_stream]->time_base );

		AVCodec * pC = avcodec_find_decoder( pCC->codec_id );
		if( pC == NULL ) {
			return;
		}

		if( avcodec_open( pCC, pC ) < 0 ) {
			return;
		}
		pCodecContext_.reset( pCC, avcodec_close );

		opening_ = true;
		hasNext_ = true;
	}

	void Decoder::close() {
		this->timeBase_ = 0.0;
		this->hasNext_ = false;
		this->opening_ = false;
		// free the members in packet, not itself
		av_free_packet( this->pPacket_.get() );
		av_init_packet( this->pPacket_.get() );
		this->pCodecContext_.reset();
		this->pFormatContext_.reset();
	}

	ByteArray Decoder::next() {
		if( !this->opening_ || !this->hasNext_ ) {
			return ByteArray();
		}

		uint8_t audio_buf[AVCODEC_MAX_AUDIO_FRAME_SIZE*3/2];
		uint8_t * audio_pkt_data = NULL;
		int audio_pkt_size = 0;

		int dp_len, data_size;

		ByteArray data;

		if( av_read_frame( this->pFormatContext_.get(), this->pPacket_.get() ) >= 0 ) {
			audio_pkt_data = this->pPacket_->data;
			audio_pkt_size = this->pPacket_->size;
			while( audio_pkt_size > 0 ) {
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
				data.insert( data.end(), audio_buf, audio_buf + data_size );
			}
			if( this->pPacket_->data ) {
				av_free_packet( this->pPacket_.get() );
			}
		} else {
			this->hasNext_ = false;
		}

		return data;
	}

	bool Decoder::hasNext( double timestamp ) const {
		if( timestamp == 0.0 ) {
			return this->hasNext_;
		} else {
			return this->hasNext_ && ( ( ( this->pPacket_->pts + this->pPacket_->duration ) * this->timeBase_ ) < timestamp );
		}
	}

	bool Decoder::seek( double timestamp ) {
		bool succeed = av_seek_frame( this->pFormatContext_.get(), -1, timestamp * AV_TIME_BASE, AVSEEK_FLAG_BACKWARD ) >= 0;
		if( succeed ) {
			this->hasNext_ = true;
			avcodec_flush_buffers( this->pCodecContext_.get() );
		}
		return succeed;
	}

	QWidget * Decoder::getUI() const {
		return NULL;
	}

}
