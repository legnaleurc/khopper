/**
 * @file track.hpp
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
#ifndef KHOPPER_ALBUM_TRACK_HPP
#define KHOPPER_ALBUM_TRACK_HPP

#include "tr1.hpp"
#include "index.hpp"

#include <QByteArray>
#include <QTextCodec>

#include <vector>
#include <map>

namespace khopper {

	namespace album {

		/**
		 * @brief Track information
		 */
		class Track {
		public:
			/**
			 * @brief Type of audio file
			 */
			enum FileType {
				/// Binary
				BINARY,
				/// Motorola
				MOTOROLA,
				/// AIFF
				AIFF,
				/// wave
				WAVE,
				/// Mpeg 1 Layer 3
				MP3
			};

			/// Track type
			enum DataType {
				/// AUDIO
				AUDIO,
				/// CDG
				CDG,
				/// MODE1/2048
				MODE1_2048,
				/// MODE1/2352
				MODE1_2352,
				/// MODE1/2336
				MODE2_2336,
				/// MODE1/2352
				MODE2_2352,
				/// CDI/2336
				CDI_2336,
				/// CDI/2352
				CDI_2352
			};

			/// Track flag
			enum Flag {
				/// NONE
				NONE = 0x00000,
				/// DCP
				DCP  = 0x00001,
				/// CH4
				CH4  = 0x00010,
				/// PRE
				PRE  = 0x00100,
				/// SCMS
				SCMS = 0x01000,
				/// DATA
				DATA = 0x10000
			};

			/**
			 * @brief Default constructor
			 */
			Track();

			/**
			 * @brief Load track from @p filePath
			 */
			void load( const QByteArray & filePath );

			QString getAlbum() const {
				return this->textCodec_->toUnicode( this->album_ );
			}
			QString getArtist() const {
				return this->textCodec_->toUnicode( this->artist_ );
			}
			int getBitRate() const {
				return this->bitRate_;
			}
			int getChannels() const {
				return this->channels_;
			}
			const Index & getDuration() const {
				return this->duration_;
			}
			const QByteArray & getFilePath() const {
				return this->filePath_;
			}
			short int getIndex() const {
				return this->index_;
			}
			int getSampleRate() const {
				return this->sampleRate_;
			}
			const Index & getStartTime() const {
				return this->startTime_;
			}
			QString getTitle() const {
				return this->textCodec_->toUnicode( this->title_ );
			}

			void setAlbum( const QByteArray & album ) {
				this->album_ = album;
			}
			void setAlbum( const QString & album ) {
				this->setAlbum( this->textCodec_->fromUnicode( album ) );
			}
			void setArtist( const QByteArray & artist ) {
				this->artist_ = artist;
			}
			void setArtist( const QString & artist ) {
				this->setArtist( this->textCodec_->fromUnicode( artist ) );
			}
			void setBitRate( int bitRate ) {
				this->bitRate_ = bitRate;
			}
			void setChannels( int channels ) {
				this->channels_ = channels;
			}
			bool addComment( const QByteArray & key, const QByteArray & value ) {
				return this->comments_.insert( std::make_pair( key, value ) ).second;
			}
			bool addComment( const QString & key, const QString & value ) {
				return this->addComment( this->textCodec_->fromUnicode( key ), this->textCodec_->fromUnicode( value ) );
			}
			void setDataType( DataType dataType ) {
				this->dataType_ = dataType;
			}
			void setDuration( const Index & duration ) {
				this->duration_ = duration;
			}
			void setFilePath( const QByteArray & filePath ) {
				this->filePath_ = filePath;
			}
			void setFilePath( const QString & filePath ) {
				this->setFilePath( filePath.toLocal8Bit() );
			}
			void addFlag( Flag flags ) {
				this->flags_ = static_cast< Flag >( this->flags_ | flags );
			}
			void addGarbage( const QByteArray & garbage ) {
				this->garbage_.push_back( garbage );
			}
			void addGarbage( const QString & garbage ) {
				this->addGarbage( this->textCodec_->fromUnicode( garbage ) );
			}
			void setIndex( short int index ) {
				this->index_ = index;
			}
			void setISRC( const QByteArray & isrc ) {
				this->isrc_ = isrc;
			}
			void setISRC( const QString & isrc ) {
				this->setISRC( this->textCodec_->fromUnicode( isrc ) );
			}
			void setPostGap( const Index & postGap ) {
				this->postGap_ = postGap;
			}
			void setPreGap( const Index & preGap ) {
				this->preGap_ = preGap;
			}
			void setSampleRate( int sampleRate ) {
				this->sampleRate_ = sampleRate;
			}
			void setSongWriter( const QByteArray & songWriter ) {
				this->songWriter_ = songWriter;
			}
			void setSongWriter( const QString & songWriter ) {
				this->setSongWriter( this->textCodec_->fromUnicode( songWriter ) );
			}
			void setStartTime( const Index & startTime ) {
				this->startTime_ = startTime;
			}
			void setTextCodec( QTextCodec * textCodec ) {
				this->textCodec_ = textCodec;
			}
			void setTitle( const QByteArray & title ) {
				this->title_ = title;
			}
			void setTitle( const QString & title ) {
				this->setTitle( this->textCodec_->fromUnicode( title ) );
			}

		private:
			QByteArray album_;
			QByteArray artist_;
			int bitRate_;
			int channels_;
			std::map< QByteArray, QByteArray > comments_;
			DataType dataType_;
			Index duration_;
			QByteArray filePath_;
			FileType fileType_;
			Flag flags_;
			std::vector< QByteArray > garbage_;
			short int index_;
			QByteArray isrc_;
			Index postGap_;
			Index preGap_;
			int sampleRate_;
			QByteArray songWriter_;
			Index startTime_;
			QTextCodec * textCodec_;
			QByteArray title_;
		};

		/**
		 * @brief Smart pointer of Track
		 */
		typedef std::tr1::shared_ptr< Track > TrackSP;
		/**
		 * @brief Smart pointer of const Track
		 */
		typedef std::tr1::shared_ptr< const Track > TrackCSP;

	}

}

#endif
