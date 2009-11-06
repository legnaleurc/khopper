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
#include "flacwritercreator.hpp"

#include "util/text.hpp"

/* register plugin */
namespace {
	static const bool registered = khopper::plugin::registerWriter( "flac", "kpw_flac" );
}
Q_EXPORT_PLUGIN2( kpw_flac, khopper::plugin::FlacWriterCreator )

/* define plugin */
namespace khopper {
	namespace plugin {
		codec::WriterSP FlacWriterCreator::doCreate() const {
			return codec::WriterSP( new codec::FlacWriter );
		}
	}
}

namespace {
	FILE * fileHelper( const std::wstring & filePath ) {
#ifdef Q_OS_WIN32
		FILE * fin = NULL;
		errno_t ret = _wfopen_s( &fin, filePath.c_str(), L"rb" );
		if( ret != 0 ) {
			return NULL;
		}
		return fin;
#else
		return fopen( khopper::text::toUtf8( filePath ).c_str(), "rb" );
#endif
	}
}

namespace khopper {

	namespace codec {

		FlacWriter::FlacWriter():
		AbstractWriter(),
		pFE_( FLAC__stream_encoder_new(), FLAC__stream_encoder_delete ),
		metadataOwner_(),
		metadata_() {
			if( !this->pFE_ ) {
				// error
			}
		}

		void FlacWriter::setupEncoder() {
			FLAC__stream_encoder_set_channels( this->pFE_.get(), this->getChannels() );
			FLAC__stream_encoder_set_bits_per_sample( this->pFE_.get(), 16 );
			FLAC__stream_encoder_set_sample_rate( this->pFE_.get(), this->getSampleRate() );
			// FLAC__stream_encoder_set_total_samples_estimate()
		}

		void FlacWriter::setupMuxer() {
			FLAC__StreamMetadata * tmp = FLAC__metadata_object_new( FLAC__METADATA_TYPE_VORBIS_COMMENT );
			FLAC__StreamMetadata_VorbisComment_Entry entry;
			FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair( &entry, "TITLE", this->getTitle().c_str() );
			FLAC__metadata_object_vorbiscomment_append_comment( tmp, entry, false );
			FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair( &entry, "ALBUM", this->getAlbum().c_str() );
			FLAC__metadata_object_vorbiscomment_append_comment( tmp, entry, false );
			FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair( &entry, "ARTIST", this->getArtist().c_str() );
			FLAC__metadata_object_vorbiscomment_append_comment( tmp, entry, false );
			this->metadataOwner_.push_back( std::tr1::shared_ptr< FLAC__StreamMetadata >( tmp, FLAC__metadata_object_delete ) );
			this->metadata_.push_back( tmp );

			tmp = FLAC__metadata_object_new( FLAC__METADATA_TYPE_SEEKTABLE );
			this->metadataOwner_.push_back( std::tr1::shared_ptr< FLAC__StreamMetadata >( tmp, FLAC__metadata_object_delete ) );
			this->metadata_.push_back( tmp );
		}

	}

}
