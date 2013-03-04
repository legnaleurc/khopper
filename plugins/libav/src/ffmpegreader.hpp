/**
 * @file ffmpegreader.hpp
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
#ifndef KHOPPER_CODEC_FFMPEGREADER_HPP
#define KHOPPER_CODEC_FFMPEGREADER_HPP

#include "khopper/reader.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#ifdef Q_OS_WIN
#include <libavformat/avio.h>
#endif
#include <libavresample/avresample.h>
}

namespace khopper {
namespace codec {

/**
 * @brief Default audio reader
 * @sa DefaultWriter
 *
 * This class provides a default audio reader implementation.
 */
class FfmpegReader : public Reader {
public:
	/**
	 * @brief Default constructor
	 */
	explicit FfmpegReader( const QUrl & uri );

	virtual bool atEnd() const;
	virtual qint64 pos( qint64 msec ) const;
	virtual bool seek( qint64 pos );
	virtual qint64 size() const;

protected:
	virtual void doOpen();
	virtual void doClose();
	virtual qint64 readData( char * data, qint64 maxSize );

private:
	void openResource_();
	void closeResource_();
	void setupDemuxer_();
	void setupDecoder_();
	void readHeader_();
	QByteArray readFrame_();
	void readMetadata_( AVDictionary * );

#ifdef Q_OS_WIN
	std::shared_ptr< QIODevice > fio_;
	std::shared_ptr< AVIOContext > pIOContext_;
#endif
	std::shared_ptr< AVFormatContext > pFormatContext_;
	std::shared_ptr< AVCodecContext > pCodecContext_;
	std::shared_ptr< AVFrame > pFrame_;
	std::shared_ptr< AVAudioResampleContext > pArContext_;
	AVPacket packet_;
	AVStream * pStream_;
	qint64 curPos_;
	bool eof_;
	QByteArray buffer_;
};

}
}

#endif
