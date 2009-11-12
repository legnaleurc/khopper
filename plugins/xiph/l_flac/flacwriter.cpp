/**
 * @file flacwriter.cpp
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
#include "flacwriter.hpp"

#include "util/text.hpp"
#include "util/error.hpp"

#include <QtDebug>

khopper::codec::FlacWriter * createFlacWriter() {
	return new khopper::codec::FlacWriter;
}

namespace {

	static inline FILE * fileHelper( const std::wstring & filePath ) {
#ifdef Q_OS_WIN32
		FILE * fout = NULL;
		errno_t ret = _wfopen_s( &fout, filePath.c_str(), L"wb" );
		if( ret != 0 ) {
			return NULL;
		}
		return fout;
#else
		return fopen( khopper::text::toUtf8( filePath ).c_str(), "wb" );
#endif
	}

}

namespace khopper {

	namespace codec {

		FlacWriter::FlacWriter():
		AbstractWriter(),
		pFE_( FLAC__stream_encoder_new(), FLAC__stream_encoder_delete ),
		metadataOwner_(),
		ogg_( false ) {
			if( !this->pFE_ ) {
				throw error::SystemError( "memory allocation error" );
			}
		}

		FlacWriter::~FlacWriter() {
		}

		void FlacWriter::setupEncoder() {
			FLAC__bool ok = true;
			ok &= FLAC__stream_encoder_set_channels( this->pFE_.get(), this->getChannels() );
			// TODO: forcing sample format S16LE, but may cause other problems.
			ok &= FLAC__stream_encoder_set_bits_per_sample( this->pFE_.get(), 16 );
			ok &= FLAC__stream_encoder_set_sample_rate( this->pFE_.get(), this->getSampleRate() );
			if( this->ogg_ ) {
				ok &= FLAC__stream_encoder_set_ogg_serial_number( this->pFE_.get(), 0xcafebabeL );
			}
			ok &= FLAC__stream_encoder_set_compression_level( this->pFE_.get(), 5 );
			ok &= FLAC__stream_encoder_set_verify( this->pFE_.get(), true );
			// FLAC__stream_encoder_set_total_samples_estimate()
			if( !ok ) {
				throw error::CodecError( "encoder parameter error" );
			}
			this->getSampleBuffer().resize( 1024 * 16 * this->getChannels() );
		}

		void FlacWriter::setupMuxer() {
			FLAC__bool ok = true;
			std::vector< FLAC__StreamMetadata * > metadata;

			FLAC__StreamMetadata * tmp = FLAC__metadata_object_new( FLAC__METADATA_TYPE_VORBIS_COMMENT );
			if( tmp == NULL ) {
				throw error::SystemError( "memory allocation error" );
			}
			FLAC__StreamMetadata_VorbisComment_Entry entry;
			ok &= FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair( &entry, "TITLE", this->getTitle().c_str() );
			ok &= FLAC__metadata_object_vorbiscomment_append_comment( tmp, entry, false );
			ok &= FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair( &entry, "ALBUM", this->getAlbum().c_str() );
			ok &= FLAC__metadata_object_vorbiscomment_append_comment( tmp, entry, false );
			ok &= FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair( &entry, "ARTIST", this->getArtist().c_str() );
			ok &= FLAC__metadata_object_vorbiscomment_append_comment( tmp, entry, false );
			if( !ok ) {
				throw error::CodecError( "encoder vorbis comment error" );
			}
			this->metadataOwner_.push_back( std::tr1::shared_ptr< FLAC__StreamMetadata >( tmp, FLAC__metadata_object_delete ) );
			metadata.push_back( tmp );

			tmp = FLAC__metadata_object_new( FLAC__METADATA_TYPE_SEEKTABLE );
			if( tmp == NULL ) {
				throw error::SystemError( "memory allocation error" );
			}
			// FIXME: dirty hack
			FLAC__metadata_object_seektable_template_append_spaced_points_by_samples( tmp, this->getSampleRate(), this->getSampleRate() * 7200 );
			this->metadataOwner_.push_back( std::tr1::shared_ptr< FLAC__StreamMetadata >( tmp, FLAC__metadata_object_delete ) );
			metadata.push_back( tmp );

//			tmp = FLAC__metadata_object_new( FLAC__METADATA_TYPE_PADDING );
//			if( tmp == NULL ) {
//				// error
//				qDebug( "metadata error" );
//			}
//			tmp->length = 2048;
//			this->metadataOwner_.push_back( std::tr1::shared_ptr< FLAC__StreamMetadata >( tmp, FLAC__metadata_object_delete ) );
//			this->metadata_.push_back( tmp );

			ok &= FLAC__stream_encoder_set_metadata( this->pFE_.get(), &metadata[0], metadata.size() );
			if( !ok ) {
				throw error::CodecError( "encoder metadata error" );
			}
		}

		void FlacWriter::openResource() {
			FLAC__StreamEncoderInitStatus init_status;
			if( this->ogg_ ) {
				init_status = FLAC__stream_encoder_init_ogg_FILE(
					this->pFE_.get(),
					fileHelper( this->getFilePath() ),
					progressCallback_,
					this
				);
			} else {
				init_status = FLAC__stream_encoder_init_FILE(
					this->pFE_.get(),
					fileHelper( this->getFilePath() ),
					progressCallback_,
					this
				);
			}
			if( init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK ) {
				throw error::CodecError( FLAC__StreamEncoderInitStatusString[init_status] );
			}
		}

		void FlacWriter::writeHeader() {
		}

		void FlacWriter::writeFrame( const char * sample, std::size_t nSample ) {
			// TODO: assumed that sample format is S16LE, please fix the interface later
			const int32_t buf_size = nSample * sizeof( char ) / sizeof( int16_t );
			// TODO: big or little endian
			const int16_t * buffer_ = static_cast< const int16_t * >( static_cast< const void * >( sample ) );
			std::vector< int32_t > buffer( buf_size );
			for( int i = 0; i < buf_size; ++i ) {
				buffer[i] = buffer_[i];
			}

			FLAC__bool ok = FLAC__stream_encoder_process_interleaved( this->pFE_.get(), &buffer[0], buf_size / this->getChannels() );
			if( !ok ) {
				throw error::CodecError( FLAC__StreamEncoderStateString[FLAC__stream_encoder_get_state( this->pFE_.get() )] );
			}
		}

		void FlacWriter::closeResource() {
			FLAC__bool ok = FLAC__stream_encoder_finish( this->pFE_.get() );
			if( !ok ) {
				// nothrow
				qDebug() << FLAC__StreamEncoderStateString[FLAC__stream_encoder_get_state( this->pFE_.get() )];
			}
			this->metadataOwner_.clear();
			this->ogg_ = false;
		}

		void FlacWriter::progressCallback_(
			const FLAC__StreamEncoder * /*encoder*/,
			FLAC__uint64 bytes_written,
			FLAC__uint64 samples_written,
			unsigned frames_written,
			unsigned total_frames_estimate,
			void * /*client_data*/ ) {
			qDebug() << bytes_written << samples_written << frames_written << total_frames_estimate;
		}

	}

}
