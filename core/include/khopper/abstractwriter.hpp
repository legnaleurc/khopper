/**
 * @file abstractwriter.hpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KHOPPER_CODEC_ABSTRACTWRITER_HPP
#define KHOPPER_CODEC_ABSTRACTWRITER_HPP

#include <functional>
#include <memory>

#include <QtCore/QIODevice>
#include <QtCore/QUrl>

#include "audioformat.hpp"

namespace khopper {

namespace codec {

/**
 * @ingroup Codecs
 * @brief Audio writer interface
 * @sa AbstractReader
 */
class KHOPPER_DLL AbstractWriter : public QIODevice {
public:
	/**
	 * @brief Virtual destructor
	 */
	virtual ~AbstractWriter();

	/**
	 * @brief Open file
	 * @param filePath
	 *
	 * AbstractWriter do not handle path encoding,
	 * so you should help yourself.
	 */
	virtual bool open( OpenMode mode );
	/**
	 * @brief Close file
	 * @note No fail!
	 */
	virtual void close();

	/**
	 * @brief Set album
	 * @sa getAlbum
	 */
	void setAlbum( const QByteArray & album );
	/**
	 * @brief Set artist
	 * @sa getArtist
	 */
	void setArtist( const QByteArray & artist );
	void setAudioFormat( const AudioFormat & format );
	/**
	 * @brief Set bitRate
	 * @sa getBitRate
	 */
	void setBitRate( unsigned int bitRate );
	void setChannelLayout( ChannelLayout channelLayout );
	/**
	 * @brief Set title
	 * @sa getTitle
	 */
	void setTitle( const QByteArray & title );

protected:
	/**
	 * @brief Constructor
	 *
	 * @param uri where this writer is
	 */
	explicit AbstractWriter( const QUrl & uri );

	/**
	 * @brief Get album
	 * @sa setAlbum
	 */
	const QByteArray & getAlbum() const;
	/**
	 * @brief Get artist
	 * @sa setArtist
	 */
	const QByteArray & getArtist() const;
	const AudioFormat & getAudioFormat() const;
	/**
	 * @brief Get bit rate
	 * @sa setBitRate
	 */
	unsigned int getBitRate() const;
	ChannelLayout getChannelLayout() const;
	/**
	 * @brief Get file path
	 * @sa setFilePath
	 */
	const QUrl & getURI() const;
	/**
	 * @brief Get title
	 * @sa setTitle
	 */
	const QByteArray & getTitle() const;

	virtual qint64 readData( char * data, qint64 maxlen );

	virtual void doOpen() = 0;
	virtual void doClose() = 0;

private:
	// prevent copying
	AbstractWriter( const AbstractWriter & );
	AbstractWriter & operator =( const AbstractWriter & );

	class Private;
	std::shared_ptr< Private > p_;
};

/**
 * @ingroup Codecs
 * @brief AbstractWriter smart pointer
 * @sa AbstractWriter WriterCSP
 */
typedef std::shared_ptr< AbstractWriter > WriterSP;
/**
 * @ingroup Codecs
 * @brief AbstractWriter const smart pointer
 * @sa AbstractWriter WriterSP
 */
typedef std::shared_ptr< const AbstractWriter > WriterCSP;

}

namespace plugin {
typedef std::function< codec::WriterSP ( const QUrl & ) > WriterCreator;
}

}

#endif
