/**
 * @file mp3writer.cpp
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
#include "mp3writer.hpp"

#include "util/text.hpp"

#include <id3v2tag.h>

namespace {

	static inline FILE * fileHelper( const std::wstring & utf8Path ) {
#ifdef Q_OS_WIN32
		FILE * fout = NULL;
		errno_t ret = _wfopen_s( &fout, utf8Path.c_str(), L"wb" );
		if( ret != 0 ) {
			return NULL;
		}
		return fout;
#else
		return fopen( khopper::text::toUtf8( utf8Path ).c_str(), "wb" );
#endif
	}

}

namespace khopper {

	namespace codec {

		Mp3Writer::Mp3Writer():
		AbstractWriter(),
		gfp_(),
		quality_( -1 ),
		id3v2Offset_( -1L ) {
		}

		Mp3Writer::~Mp3Writer() {
		}

		void Mp3Writer::setupMuxer() {
			this->gfp_.reset( lame_init(), lame_close );
		}

		void Mp3Writer::openResource() {
			this->fout_.reset( fileHelper( this->getFilePath() ), fclose );
		}

		void Mp3Writer::setupEncoder() {
			lame_set_num_channels( this->gfp_.get(), this->getChannels() );
			lame_set_in_samplerate( this->gfp_.get(), this->getSampleRate() );
			if( this->quality_ < 0 ) {
				lame_set_brate( this->gfp_.get(), this->getBitRate() / 1000 );
				lame_set_bWriteVbrTag( this->gfp_.get(), 0 );
			} else {
				lame_set_VBR( this->gfp_.get(), vbr_default );
				lame_set_VBR_q( this->gfp_.get(), this->quality_ );
				lame_set_bWriteVbrTag( this->gfp_.get(), 1 );
			}
			lame_set_mode( this->gfp_.get(), JOINT_STEREO );
			lame_set_quality( this->gfp_.get(), 2 );
		}

		void Mp3Writer::writeHeader() {
			lame_set_write_id3tag_automatic( this->gfp_.get(), 0 );

			TagLib::ID3v2::Tag tag;
			tag.setTitle( TagLib::String( this->getTitle(), TagLib::String::UTF8 ) );
			tag.setArtist( TagLib::String( this->getArtist(), TagLib::String::UTF8 ) );
			tag.setAlbum( TagLib::String( this->getAlbum(), TagLib::String::UTF8 ) );

			TagLib::ByteVector id3v2 = tag.render();
			qDebug( "ID3v2 length: %d", id3v2.size() );
			this->id3v2Offset_ = id3v2.size();
			fwrite( id3v2.data(), sizeof( id3v2[0] ), id3v2.size(), this->fout_.get() );

			int ret = lame_init_params( this->gfp_.get() );
			if( ret < 0 ) {
				qDebug( "lame param error" );
			}
			this->getSampleBuffer().resize( lame_get_framesize( this->gfp_.get() ) * 4 );
		}

		void Mp3Writer::writeFrame( const char * samples, std::size_t nSamples ) {
			int ret = 0;
			short int * audio = static_cast< short int * >( const_cast< void * >( static_cast< const void * >( samples ) ) );
			int nsamples = nSamples / sizeof( short int ) / this->getChannels();
			std::vector< unsigned char > buffer( 1.25 * nsamples + 7200 );

			if( this->getChannels() == 1 ) {
				ret = lame_encode_buffer(
					this->gfp_.get(),
					audio,
					audio,
					nsamples,
					&buffer[0],
					buffer.size()
				);
			} else {
				ret = lame_encode_buffer_interleaved(
					this->gfp_.get(),
					audio,
					nsamples,
					&buffer[0],
					buffer.size()
				);
			}

			if( ret < 0 ) {
				qDebug( "lame encode error: %d", ret );
			}
			fwrite( &buffer[0], sizeof( char ), ret, this->fout_.get() );
		}

		void Mp3Writer::closeResource() {
			std::vector< unsigned char > buffer( 7200 );
			int ret = lame_encode_flush(
				this->gfp_.get(),
				&buffer[0],
				buffer.size()
			);
			if( ret < 0 ) {
				qDebug( "lame flush error" );
			}
			fwrite( &buffer[0], sizeof( char ), ret, this->fout_.get() );

			ret = lame_get_lametag_frame( this->gfp_.get(), &buffer[0], buffer.size() );
			if( ret > 0 ) {
				fseek( this->fout_.get(), this->id3v2Offset_, SEEK_SET );
				fwrite( &buffer[0], sizeof( char ), ret, this->fout_.get() );
			}

			this->id3v2Offset_ = -1L;
			this->quality_ = -1;
			this->gfp_.reset();
			this->fout_.reset();
		}

	}

}
