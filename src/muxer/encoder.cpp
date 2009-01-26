#include "encoder.hpp"
#include "os.hpp"

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
	filePath_(),
	samples_(),
	pFormatContext_(),
	pStream_( NULL ),
	bitRate_( -1 ),
	sampleRate_( -1 ),
	channels_( -1 ){
	}

	Encoder::~Encoder() {
		if( this->opening_ ) {
			this->close();
		}
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

	void Encoder::open( const std::wstring & filePath ) {
		this->filePath_ = os::encodeString( filePath );
		AVOutputFormat * pOF = this->guessFormat();
		if( pOF == NULL ) {
			return;
		}

		this->pFormatContext_.reset( av_alloc_format_context(), ::fc_helper );
		if( !this->pFormatContext_ ) {
			return;
		}
		this->pFormatContext_->oformat = pOF;
		strncpy( this->pFormatContext_->filename, this->filePath_.c_str(), sizeof( this->pFormatContext_->filename ) );

		AVCodecContext * pCC = NULL;

		if( pOF->audio_codec != CODEC_ID_NONE ) {
			this->pStream_ = av_new_stream( this->pFormatContext_.get(), 1 );
			if( !pStream_ ) {
				return;
			}

			pCC = this->pStream_->codec;
			pCC->codec_id = pOF->audio_codec;
			pCC->codec_type = CODEC_TYPE_AUDIO;

			// setting special parameters
			pCC->bit_rate = this->bitRate_;
			pCC->sample_rate = this->sampleRate_;
			pCC->channels = this->channels_;
		}

		if( av_set_parameters( this->pFormatContext_.get(), NULL ) < 0 ) {
			return;
		}

		// NOTE: set complete

		this->openAudio_();

		if( !( pOF->flags & AVFMT_NOFILE ) ) {
			if( url_fopen( &this->pFormatContext_->pb, this->filePath_.c_str(), URL_WRONLY ) < 0 ) {
				return;
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

		this->opening_ = false;
	}

	void Encoder::write( const ByteArray & data ) {
		this->buffer_.insert( this->buffer_.end(), data.begin(), data.end() );
		if( buffer_.size() < samples_.size() ) {
			return;
		}

		while( this->buffer_.size() >= samples_.size() ) {
			std::copy( this->buffer_.begin(), this->buffer_.begin() + this->samples_.size(), this->samples_.begin() );
			this->buffer_.erase( this->buffer_.begin(), this->buffer_.begin() + samples_.size() );

			this->writeFrame_( reinterpret_cast< short * >( &this->samples_[0] ) );
// 			pkt.size = avcodec_encode_audio( pCC, audio_outbuf, audio_outbuf_size, reinterpret_cast< short * >( &samples_[0] ) );
// 
// 			if( pCC->coded_frame->pts != AV_NOPTS_VALUE ) {
// 				pkt.pts = av_rescale_q( pCC->coded_frame->pts, pCC->time_base, this->pStream_->time_base );
// 			}
// 
// 			if( av_write_frame( this->pFormatContext_.get(), &pkt ) != 0 ) {
// 				return;
// 			}
		}
	}

	void Encoder::flush() {
		if( this->opening_ && !this->buffer_.empty() ) {
			this->writeFrame_( reinterpret_cast< short * >( &this->buffer_[0] ) );
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
			return;
		}
	}

	bool Encoder::openAudio_() {
		AVCodecContext * pCC = this->pStream_->codec;

		AVCodec * pC = avcodec_find_encoder( pCC->codec_id );
		if( !pC ) {
			return false;
		}

		if( avcodec_open( pCC, pC ) < 0 ) {
			return false;
		}

		return true;
	}

	QWidget * Encoder::getUI() const {
		return NULL;
	}

}
