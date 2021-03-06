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

#include "khopper/codecerror.hpp"
#include "khopper/ioerror.hpp"
#include "khopper/systemerror.hpp"

using khopper::codec::FlacWriter;
using khopper::error::CodecError;
using khopper::error::IOError;
using khopper::error::SystemError;

FlacWriter::FlacWriter( const QUrl & uri ):
Writer( uri ),
pFE_( FLAC__stream_encoder_new(), FLAC__stream_encoder_delete ),
metadataOwner_(),
ogg_( false ) {
	if( !this->pFE_ ) {
		throw SystemError( QObject::tr( "memory allocation error" ), __FILE__, __LINE__ );
	}
}

void FlacWriter::setOggMode( bool ogg ) {
	this->ogg_ = ogg;
}

void FlacWriter::doOpen() {
	if( this->getURI().scheme() != "file" ) {
		throw IOError( QObject::tr( "This plugin do not support remote access." ), __FILE__, __LINE__ );
	}
	// open files
	FILE * fout = NULL;
#ifdef Q_OS_WIN32
	errno_t ret = _wfopen_s( &fout, this->getURI().toLocalFile().toStdWString().c_str(), L"wb" );
	if( ret != 0 ) {
		throw IOError( ret );
	}
#else
	fout = fopen( this->getURI().toLocalFile().toStdString().c_str(), "wb" );
	if( fout == NULL ) {
		throw IOError( QObject::tr( "Can not open: %1" ).arg( this->getURI().toString() ), __FILE__, __LINE__ );
	}
#endif

	FLAC__bool ok = true;
	std::vector< FLAC__StreamMetadata * > metadata;

	// setup metadata
	FLAC__StreamMetadata * tmp = FLAC__metadata_object_new( FLAC__METADATA_TYPE_VORBIS_COMMENT );
	if( tmp == NULL ) {
		throw error::SystemError( QObject::tr( "memory allocation error" ), __FILE__, __LINE__ );
	}
	FLAC__StreamMetadata_VorbisComment_Entry entry;
	ok &= FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair( &entry, "TITLE", this->getTitle().constData() );
	ok &= FLAC__metadata_object_vorbiscomment_append_comment( tmp, entry, false );
	ok &= FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair( &entry, "ALBUM", this->getAlbum().constData() );
	ok &= FLAC__metadata_object_vorbiscomment_append_comment( tmp, entry, false );
	ok &= FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair( &entry, "ARTIST", this->getArtist().constData() );
	ok &= FLAC__metadata_object_vorbiscomment_append_comment( tmp, entry, false );
	if( !ok ) {
		throw CodecError( QObject::tr( "encoder vorbis comment error" ), __FILE__, __LINE__ );
	}
	this->metadataOwner_.push_back( std::shared_ptr< FLAC__StreamMetadata >( tmp, FLAC__metadata_object_delete ) );
	metadata.push_back( tmp );

	tmp = FLAC__metadata_object_new( FLAC__METADATA_TYPE_SEEKTABLE );
	if( tmp == NULL ) {
		throw error::SystemError( QObject::tr( "memory allocation error" ), __FILE__, __LINE__ );
	}
	// FIXME: dirty hack, I don't know how to rescale seek table size
	FLAC__metadata_object_seektable_template_append_spaced_points_by_samples( tmp, this->getAudioFormat().frequency(), this->getAudioFormat().frequency() * 7200 );
	this->metadataOwner_.push_back( std::shared_ptr< FLAC__StreamMetadata >( tmp, FLAC__metadata_object_delete ) );
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
		throw CodecError( QObject::tr( "encoder metadata error" ), __FILE__, __LINE__ );
	}

	// setup encoder setting
	ok &= FLAC__stream_encoder_set_channels( this->pFE_.get(), this->getAudioFormat().channels() );
	ok &= FLAC__stream_encoder_set_bits_per_sample( this->pFE_.get(), this->getAudioFormat().sampleSize() );
	ok &= FLAC__stream_encoder_set_sample_rate( this->pFE_.get(), this->getAudioFormat().frequency() );
	// if ogg mode is set
	if( this->ogg_ ) {
		ok &= FLAC__stream_encoder_set_ogg_serial_number( this->pFE_.get(), 0xcafebabeL );
	}
	ok &= FLAC__stream_encoder_set_compression_level( this->pFE_.get(), 5 );
	ok &= FLAC__stream_encoder_set_verify( this->pFE_.get(), true );
	// FLAC__stream_encoder_set_total_samples_estimate()
	if( !ok ) {
		throw CodecError( QObject::tr( "encoder parameter error" ), __FILE__, __LINE__ );
	}

	FLAC__StreamEncoderInitStatus init_status;
	if( this->ogg_ ) {
		init_status = FLAC__stream_encoder_init_ogg_FILE(
			this->pFE_.get(),
			fout,
			progressCallback_,
			this
		);
	} else {
		init_status = FLAC__stream_encoder_init_FILE(
			this->pFE_.get(),
			fout,
			progressCallback_,
			this
		);
	}
	if( init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK ) {
		throw CodecError( FLAC__StreamEncoderInitStatusString[init_status], __FILE__, __LINE__ );
	}
}

qint64 FlacWriter::writeData( const char * data, qint64 len ) {
	if( !data || len <= 0 ) {
		return len;
	}
	const qint64 bufSize = len / ( this->getAudioFormat().sampleSize() / 8 );
	// TODO: big or little endian
	const int16_t * audio = static_cast< const int16_t * >( static_cast< const void * >( data ) );
	std::vector< int32_t > buffer( bufSize );
	for( qint64 i = 0; i < bufSize; ++i ) {
		buffer[i] = audio[i];
	}

	FLAC__bool ok = FLAC__stream_encoder_process_interleaved( this->pFE_.get(), &buffer[0], bufSize / this->getAudioFormat().channels() );
	if( !ok ) {
		throw CodecError( FLAC__StreamEncoderStateString[FLAC__stream_encoder_get_state( this->pFE_.get() )], __FILE__, __LINE__ );
	}

	return len;
}

void FlacWriter::doClose() {
	FLAC__bool ok = FLAC__stream_encoder_finish( this->pFE_.get() );
	if( !ok ) {
		// nothrow
		// qDebug() << FLAC__StreamEncoderStateString[FLAC__stream_encoder_get_state( this->pFE_.get() )];
	}
	this->metadataOwner_.clear();
	this->ogg_ = false;
}

void FlacWriter::progressCallback_(
	const FLAC__StreamEncoder * /*encoder*/,
	FLAC__uint64 /*bytes_written*/,
	FLAC__uint64 /*samples_written*/,
	unsigned /*frames_written*/,
	unsigned /*total_frames_estimate*/,
	void * /*client_data*/ ) {
//			qDebug() << bytes_written << samples_written << frames_written << total_frames_estimate;
}
