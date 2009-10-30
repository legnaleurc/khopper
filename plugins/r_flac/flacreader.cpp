/**
 * @file flacreader.cpp
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
#include "flacreader.hpp"
#include "flacreadercreator.hpp"

#include "util/text.hpp"

/* register plugin */
namespace {
	bool registered = khopper::plugin::registerReader( "flac", "kpr_flac" );
}
Q_EXPORT_PLUGIN2( kpr_flac, khopper::plugin::FlacReaderCreator )

/* define plugin */
namespace khopper {
	namespace plugin {
		codec::ReaderSP FlacReaderCreator::doCreate() const {
			return codec::ReaderSP( new codec::FlacReader );
		}
	}
}

namespace khopper {

	namespace codec {

		FlacReader::FlacReader():
			AbstractReader(),
			pFD_(),
			buffer_(),
			offset_( 0 ),
			decodedTime_( 0.0 ) {
		}

		FlacReader::~FlacReader() {
		}

		void FlacReader::openResource() {
			this->pFD_.reset( FLAC__stream_decoder_new(), FLAC__stream_decoder_delete );
			if( !this->pFD_ ) {
				throw error::CodecError( "Not enough memory! (from khopper::codec::FlacReader)" );
			}
			if( !FLAC__stream_decoder_set_md5_checking( this->pFD_.get(), true ) ) {
				throw error::CodecError( "Can\'t check md5! (from khopper::codec::FlacReader)" );
			}

			FLAC__StreamDecoderInitStatus initStatus = FLAC__stream_decoder_init_file(
				this->pFD_.get(),
				text::toUtf8( this->getFilePath() ).c_str(),
				writeCallback_,
				metadataCallback_,
				errorCallback_,
				this
			);
			if( initStatus != FLAC__STREAM_DECODER_INIT_STATUS_OK ) {
				throw error::CodecError( std::string( FLAC__StreamDecoderInitStatusString[initStatus] ) + " (from khopper::codec::FlacReader)" );
			}
		}

		void FlacReader::closeResource() {
			this->pFD_.reset();
			this->buffer_.clear();
			this->offset_ = 0;
			this->decodedTime_ = 0.0;
		}

		void FlacReader::setupDemuxer() {
		}

		void FlacReader::setupDecoder() {
		}

		void FlacReader::readHeader() {
			FLAC__bool ok = FLAC__stream_decoder_process_until_end_of_metadata( this->pFD_.get() );
			if( !ok ) {
				throw error::CodecError( "Can\'t read metadata (from khopper::codec::FlacReader)" );
			}
		}

		bool FlacReader::seekFrame( double timestamp ) {
			FLAC__uint64 ts = timestamp * this->getSampleRate();
			FLAC__bool ok = FLAC__stream_decoder_seek_absolute( this->pFD_.get(), ts );
			if( ok ) {
				this->offset_ = ts;
			}
			return ok;
		}

		ByteArray FlacReader::readFrame( double & decoded, bool & stop ) {
			stop = false;
			FLAC__bool ok = FLAC__stream_decoder_process_single( this->pFD_.get() );
			if( !ok ) {
				stop = true;
				return ByteArray();
			} else {
				decoded = this->decodedTime_;
				return this->buffer_;
			}
		}

		void FlacReader::metadataCallback_( const FLAC__StreamDecoder * /*decoder*/, const FLAC__StreamMetadata * metadata, void * client_data ) {
			FlacReader * self = static_cast< FlacReader * >( client_data );
			switch( metadata->type ) {
			case FLAC__METADATA_TYPE_STREAMINFO:
				self->setSampleRate( metadata->data.stream_info.sample_rate );
				self->setChannels( metadata->data.stream_info.channels );
				self->setDuration( metadata->data.stream_info.total_samples / metadata->data.stream_info.sample_rate );
				self->setBitRate( 0 );
				break;
			case FLAC__METADATA_TYPE_PADDING:
				break;
			case FLAC__METADATA_TYPE_APPLICATION:
				break;
			case FLAC__METADATA_TYPE_SEEKTABLE:
				break;
			case FLAC__METADATA_TYPE_VORBIS_COMMENT:
				break;
			case FLAC__METADATA_TYPE_CUESHEET:
				break;
			case FLAC__METADATA_TYPE_PICTURE:
				break;
			case FLAC__METADATA_TYPE_UNDEFINED:
				break;
			default:
				;
			}
		}

		FLAC__StreamDecoderWriteStatus FlacReader::writeCallback_(
			const FLAC__StreamDecoder * /*decoder*/,
			const FLAC__Frame * frame,
			const FLAC__int32 * const buffer[],
			void * client_data ) {
			FlacReader * self = static_cast< FlacReader * >( client_data );
			self->buffer_.clear();

			unsigned int decoded = 0;
			for( unsigned int i = 0; i < frame->header.blocksize; ++i ) {
				double ts = ( self->offset_ + i ) / ( double )frame->header.sample_rate;
				if( self->afterBegin( ts ) ) {
					if( self->afterEnd( ts ) ) {
						return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
					}
					for( unsigned int c = 0; c < frame->header.channels; ++c ) {
						FLAC__int16 d = static_cast< FLAC__int16 >( buffer[c][i] );
						// little endian
						self->buffer_.push_back( d );
						self->buffer_.push_back( d >> 8 );
					}
					++decoded;
				}
			}

			self->offset_ += decoded;
			self->decodedTime_ = decoded / ( double )frame->header.sample_rate;

			return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
		}

		void FlacReader::errorCallback_(const FLAC__StreamDecoder * /*decoder*/, FLAC__StreamDecoderErrorStatus status, void * /*client_data*/) {
			throw error::CodecError( std::string( FLAC__StreamDecoderErrorStatusString[status] ) + " (from khopper::codec::FlacReader)" );
		}

	}

}
