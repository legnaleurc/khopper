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

#include "util/tr1.hpp"
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
			void load( const QString & filePath );

			/**
			 * @brief Get album
			 * @sa setAlbum(const QString &) setAlbum(const QByteArray &)
			 */
			QString getAlbum() const {
				return this->textCodec_->toUnicode( this->album_ );
			}
			/**
			 * @brief Get artist
			 * @sa setArtist(const QString &) setArtist(const QByteArray &)
			 */
			QString getArtist() const {
				return this->textCodec_->toUnicode( this->artist_ );
			}
			/**
			 * @brief Get bit rate
			 * @sa setBitRate()
			 */
			int getBitRate() const {
				return this->bitRate_;
			}
			/**
			 * @brief Get channels
			 * @sa setChannels()
			 */
			int getChannels() const {
				return this->channels_;
			}
			/**
			 * @brief Get duration
			 * @sa setDuration()
			 */
			const Index & getDuration() const {
				return this->duration_;
			}
			/**
			 * @brief Get file path
			 * @sa setFilePath(const QString &) setFilePath(const QByteArray &)
			 *
			 * Encoded string in locale.
			 */
			const QString & getFilePath() const {
				return this->filePath_;
			}
			/**
			 * @brief Get track index
			 * @sa setIndex()
			 */
			short int getIndex() const {
				return this->index_;
			}
			/**
			 * @brief Get sample rate
			 * @sa setSampleRate()
			 */
			int getSampleRate() const {
				return this->sampleRate_;
			}
			/**
			 * @brief Get start time
			 * @sa setStartTime()
			 */
			const Index & getStartTime() const {
				return this->startTime_;
			}
			/**
			 * @brief Get title
			 * @sa setTitle(const QString &) setTitle(const QByteArray &)
			 */
			QString getTitle() const {
				return this->textCodec_->toUnicode( this->title_ );
			}

			/**
			 * @brief Set album
			 * @sa getAlbum() setAlbum(const QString &)
			 */
			void setAlbum( const QByteArray & album ) {
				this->album_ = album;
			}
			/**
			 * @brief Set album
			 * @sa getAlbum() setAlbum(const QByteArray &)
			 */
			void setAlbum( const QString & album ) {
				this->setAlbum( this->textCodec_->fromUnicode( album ) );
			}
			/**
			 * @brief Set Artist
			 * @sa getArtist() setArtist(const QString &)
			 */
			void setArtist( const QByteArray & artist ) {
				this->artist_ = artist;
			}
			/**
			 * @brief Set Artist
			 * @sa getArtist() setArtist(const QByteArray &)
			 */
			void setArtist( const QString & artist ) {
				this->setArtist( this->textCodec_->fromUnicode( artist ) );
			}
			/**
			 * @brief Set bit rate
			 * @sa getBitRate()
			 */
			void setBitRate( int bitRate ) {
				this->bitRate_ = bitRate;
			}
			/**
			 * @brief Set channels
			 * @sa getChannels()
			 */
			void setChannels( int channels ) {
				this->channels_ = channels;
			}
			/**
			 * @brief Add comment
			 * @param key comment key
			 * @param value comment content
			 * @sa addComment(const QString &, const QString &)
			 */
			bool addComment( const QByteArray & key, const QByteArray & value ) {
				return this->comments_.insert( std::make_pair( key, value ) ).second;
			}
			/**
			 * @brief Add comment
			 * @param key comment key
			 * @param value comment content
			 * @sa addComment(const QByteArray &, const QByteArray &)
			 */
			bool addComment( const QString & key, const QString & value ) {
				return this->addComment( this->textCodec_->fromUnicode( key ), this->textCodec_->fromUnicode( value ) );
			}
			/**
			 * @brief Set data type
			 */
			void setDataType( DataType dataType ) {
				this->dataType_ = dataType;
			}
			/**
			 * @brief Set duration
			 * @sa getDuration()
			 */
			void setDuration( const Index & duration ) {
				this->duration_ = duration;
			}
			/**
			 * @brief Set file path
			 * @sa getFilePath() setFilePath(const QString &)
			 */
			void setFilePath( const QByteArray & filePath ) {
				this->setFilePath( QString::fromLocal8Bit( filePath.constData() ) );
			}
			/**
			 * @brief Set file path
			 * @sa getFilePath() setFilePath(const QByteArray &)
			 */
			void setFilePath( const QString & filePath ) {
				this->filePath_ = filePath;
			}
			/**
			 * @brief Add flag
			 */
			void addFlag( Flag flags ) {
				this->flags_ = static_cast< Flag >( this->flags_ | flags );
			}
			/**
			 * @brief Add garbage information
			 * @sa getGarbage() addGarbage(const QString &)
			 */
			void addGarbage( const QByteArray & garbage ) {
				this->garbage_.push_back( garbage );
			}
			/**
			 * @brief Add garbage information
			 * @sa getGarbage() addGarbage(const QByteArray &)
			 */
			void addGarbage( const QString & garbage ) {
				this->addGarbage( this->textCodec_->fromUnicode( garbage ) );
			}
			/**
			 * @brief Set index
			 * @sa getIndex()
			 */
			void setIndex( short int index ) {
				this->index_ = index;
			}
			/**
			 * @brief Set ISRC
			 * @sa getISRC() setISRC(const QString &)
			 */
			void setISRC( const QByteArray & isrc ) {
				this->isrc_ = isrc;
			}
			/**
			 * @brief Set ISRC
			 * @sa getISRC() setISRC(const QByteArray &)
			 */
			void setISRC( const QString & isrc ) {
				this->setISRC( this->textCodec_->fromUnicode( isrc ) );
			}
			/**
			 * @brief Set postgap
			 */
			void setPostGap( const Index & postGap ) {
				this->postGap_ = postGap;
			}
			/**
			 * @brief Set pregap
			 */
			void setPreGap( const Index & preGap ) {
				this->preGap_ = preGap;
			}
			/**
			 * @brief Set sample rate
			 * @sa getSampleRate()
			 */
			void setSampleRate( int sampleRate ) {
				this->sampleRate_ = sampleRate;
			}
			/**
			 * @brief Set song writer
			 * @sa getSongWriter() setSongWriter(const QString &)
			 */
			void setSongWriter( const QByteArray & songWriter ) {
				this->songWriter_ = songWriter;
			}
			/**
			 * @brief Set song writer
			 * @sa getSongWriter() setSongWriter(const QByteArray &)
			 */
			void setSongWriter( const QString & songWriter ) {
				this->setSongWriter( this->textCodec_->fromUnicode( songWriter ) );
			}
			/**
			 * @brief Set start time
			 * @sa getStartTime()
			 */
			void setStartTime( const Index & startTime ) {
				this->startTime_ = startTime;
			}
			/**
			 * @brief Set text codec
			 */
			void setTextCodec( QTextCodec * textCodec ) {
				if( textCodec ) {
					this->textCodec_ = textCodec;
				}
			}
			/**
			 * @brief Set track title
			 * @sa getTitle() setTitle(const QString &)
			 */
			void setTitle( const QByteArray & title ) {
				this->title_ = title;
			}
			/**
			 * @brief Set track title
			 * @sa getTitle() setTitle(const QByteArray &)
			 */
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
			QString filePath_;
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
