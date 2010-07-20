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

#include "khopper/text.hpp"

#include <QtCore/QMultiMap>
#include <QtCore/QStringList>
#include <QtDebug>

#include <cstring>

namespace {

	static inline FILE * fileHelper( const QUrl & uri ) {
		// FIXME: not always local file
#ifdef Q_OS_WIN32
		FILE * fin = NULL;
		errno_t ret = _wfopen_s( &fin, uri.toLocalFile().toStdWString().c_str(), L"rb" );
		if( ret != 0 ) {
			return NULL;
		}
		return fin;
#else
		return fopen( uri.toLocalFile().toStdString().c_str(), "rb" );
#endif
	}
}

using namespace khopper::codec;
using khopper::error::CodecError;

FlacReader::FlacReader( const QUrl & uri ):
AbstractReader( uri ),
pFD_( FLAC__stream_decoder_new(), FLAC__stream_decoder_delete ),
buffer_(),
offset_( 0 ) {
	if( !this->pFD_ ) {
		throw CodecError( "Not enough memory! (from khopper::codec::FlacReader)" );
	}
}

bool FlacReader::atEnd() const {
	return FLAC__stream_decoder_get_state( this->pFD_.get() ) == FLAC__STREAM_DECODER_END_OF_STREAM;
}

void FlacReader::doOpen() {
	if( !FLAC__stream_decoder_set_md5_checking( this->pFD_.get(), true ) ) {
		throw CodecError( "Can\'t check md5! (from khopper::codec::FlacReader)" );
	}
	if( !FLAC__stream_decoder_set_metadata_respond_all( this->pFD_.get() ) ) {
		throw CodecError( "Can\'t retrive all metadata! (from khopper::codec::FlacReader)" );
	}

	FLAC__StreamDecoderInitStatus initStatus = FLAC__stream_decoder_init_FILE(
		this->pFD_.get(),
		fileHelper( this->getURI() ),
		writeCallback_,
		metadataCallback_,
		errorCallback_,
		this
	);
	if( initStatus != FLAC__STREAM_DECODER_INIT_STATUS_OK ) {
		throw CodecError( std::string( FLAC__StreamDecoderInitStatusString[initStatus] ) + " (from khopper::codec::FlacReader)" );
	}

	FLAC__bool ok = FLAC__stream_decoder_process_until_end_of_metadata( this->pFD_.get() );
	if( !ok ) {
		throw CodecError( "Can\'t read metadata (from khopper::codec::FlacReader)" );
	}
}

void FlacReader::doClose() {
	FLAC__stream_decoder_finish( this->pFD_.get() );
	this->buffer_.clear();
	//this->offset_ = 0;
}

bool FlacReader::seek( qint64 pos ) {
	bool ret = this->AbstractReader::seek( pos );
	FLAC__bool ok = FLAC__stream_decoder_seek_absolute( this->pFD_.get(), pos );
	if( ok && ret ) {
		//this->offset_ = pos;
		this->buffer_.clear();
	}
	return ok && ret;
}

qint64 FlacReader::size() const {
	return FLAC__stream_decoder_get_total_samples( this->pFD_.get() ) * this->getAudioFormat().channels() * this->getAudioFormat().sampleSize() / 8;
}

qint64 FlacReader::readData( char * data, qint64 maxSize ) {
	while( !this->atEnd() && this->buffer_.size() < maxSize ) {
		FLAC__bool ok = FLAC__stream_decoder_process_single( this->pFD_.get() );
		if( !ok ) {
			throw CodecError( QString( "%1 %2" ).arg( FLAC__StreamDecoderErrorStatusString[FLAC__stream_decoder_get_state( this->pFD_.get() )] ).arg( "(from khopper::codec::FlacReader)" ) );
		}
	}
	maxSize = qMin( maxSize, static_cast< qint64 >( this->buffer_.size() ) );
	std::memcpy( data, this->buffer_, maxSize );
	this->buffer_.remove( 0, maxSize );
	return maxSize;
}

void FlacReader::parseVorbisComments_( const FLAC__StreamMetadata_VorbisComment & comments ) {
	QMultiMap< QString, QString > tags;
	for( unsigned int i = 0; i < comments.num_comments; ++i ) {
		QStringList uc = QString::fromUtf8( static_cast< const char * >( static_cast< const void * >( comments.comments[i].entry ) ) ).split( '=' );
		tags.insert( uc[0].toUpper(), uc[1] );
	}
	for( QMultiMap< QString, QString >::const_iterator it = tags.begin(); it != tags.end(); ++it ) {
		if( it.key() == "TITLE" ) {
			this->setTitle( QStringList( tags.values( it.key() ) ).join( " & " ).toUtf8().constData() );
		} else if( it.key() == "ALBUM" ) {
			this->setAlbumTitle( QStringList( tags.values( it.key() ) ).join( " & " ).toUtf8().constData() );
		} else if( it.key() == "TRACKNUMBER" ) {
			this->setIndex( it.value().toInt() );
		} else if( it.key() == "ARTIST" ) {
			this->setArtist( QStringList( tags.values( it.key() ) ).join( " & " ).toUtf8().constData() );
		} else if( it.key() == "PERFORMER" ) {
			this->setArtist( QStringList( tags.values( it.key() ) ).join( " & " ).toUtf8().constData() );
		} else if( it.key() == "GENRE" ) {
			this->setGenre( QStringList( tags.values( it.key() ) ).join( " & " ).toUtf8().constData() );
		} else if( it.key() == "DATE" ) {
			this->setYear( it.value() );
		} else {
			qDebug() << "readed `" << it.key() << "\' but ignored";
		}
	}
}

void FlacReader::metadataCallback_( const FLAC__StreamDecoder * /*decoder*/, const FLAC__StreamMetadata * metadata, void * client_data ) {
	FlacReader * self = static_cast< FlacReader * >( client_data );
	QAudioFormat format;
	switch( metadata->type ) {
	case FLAC__METADATA_TYPE_STREAMINFO:
		qDebug( "FLAC__METADATA_TYPE_STREAMINFO" );
		self->setDuration( metadata->data.stream_info.total_samples * 1000 / metadata->data.stream_info.sample_rate );
		self->setBitRate( 0 );
		format.setFrequency( metadata->data.stream_info.sample_rate );
		format.setChannels( metadata->data.stream_info.channels );
		format.setSampleType( QAudioFormat::SignedInt );
		format.setSampleSize( metadata->data.stream_info.bits_per_sample );
		self->setAudioFormat( format );
		break;
	case FLAC__METADATA_TYPE_PADDING:
		qDebug( "FLAC__METADATA_TYPE_PADDING" );
		qDebug() << metadata->length;
		break;
	case FLAC__METADATA_TYPE_APPLICATION:
		qDebug( "FLAC__METADATA_TYPE_APPLICATION" );
		break;
	case FLAC__METADATA_TYPE_SEEKTABLE:
		qDebug( "FLAC__METADATA_TYPE_SEEKTABLE" );
//#ifndef QT_NO_DEBUG_OUTPUT
//				for( uint64_t i = 0; i < metadata->data.seek_table.num_points; ++i ) {
//					FLAC__StreamMetadata_SeekPoint * sp = metadata->data.seek_table.points + i;
//					qDebug() << sp->sample_number << sp->stream_offset << sp->frame_samples;
//				}
//#endif
		break;
	case FLAC__METADATA_TYPE_VORBIS_COMMENT:
		self->parseVorbisComments_( metadata->data.vorbis_comment );
		break;
	case FLAC__METADATA_TYPE_CUESHEET:
		qDebug( "FLAC__METADATA_TYPE_CUESHEET" );
		break;
	case FLAC__METADATA_TYPE_PICTURE:
		qDebug( "FLAC__METADATA_TYPE_PICTURE" );
		break;
	case FLAC__METADATA_TYPE_UNDEFINED:
		qDebug( "FLAC__METADATA_TYPE_UNDEFINED" );
		break;
	default:
		qDebug( "METADATA IS WRONG" );
	}
}

FLAC__StreamDecoderWriteStatus FlacReader::writeCallback_(
	const FLAC__StreamDecoder * /*decoder*/,
	const FLAC__Frame * frame,
	const FLAC__int32 * const buffer[],
	void * client_data ) {
	FlacReader * self = static_cast< FlacReader * >( client_data );

	for( unsigned int i = 0; i < frame->header.blocksize; ++i ) {
		for( unsigned int c = 0; c < frame->header.channels; ++c ) {
			const uint8_t * tmp = static_cast< const uint8_t * >( static_cast< const void * >( &buffer[c][i] ) );
			switch( frame->header.bits_per_sample ) {
			case 8:
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
				self->buffer_.push_back( tmp[0] );
#else
				self->buffer_.push_back( tmp[3] );
#endif
				break;
			case 16:
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
				self->buffer_.push_back( tmp[0] );
				self->buffer_.push_back( tmp[1] );
#else
				self->buffer_.push_back( tmp[2] );
				self->buffer_.push_back( tmp[3] );
#endif
				break;
			case 32:
				self->buffer_.push_back( tmp[0] );
				self->buffer_.push_back( tmp[1] );
				self->buffer_.push_back( tmp[2] );
				self->buffer_.push_back( tmp[3] );
				break;
			default:
				throw CodecError( "Unsupported sample resolution (from khopper::codec::FlacReader)" );
			}
		}
	}

	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

void FlacReader::errorCallback_(const FLAC__StreamDecoder * /*decoder*/, FLAC__StreamDecoderErrorStatus status, void * /*client_data*/) {
	throw CodecError( std::string( FLAC__StreamDecoderErrorStatusString[status] ) + " (from khopper::codec::FlacReader)" );
}
