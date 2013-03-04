/**
 * @file ffmpegwriter.hpp
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
#ifndef KHOPPER_CODEC_FFMPEGWRITER_HPP
#define KHOPPER_CODEC_FFMPEGWRITER_HPP

#include "khopper/writer.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#ifdef Q_OS_WIN
#include <libavformat/avio.h>
#endif
}

namespace khopper {
namespace codec {

/**
 * @brief Default audio writer
 * @sa DefaultReader
 *
 * This class provides a default audio writer implementation.
 */
class FfmpegWriter: public Writer {
public:
	/**
	 * @brief Constructor
	 */
	explicit FfmpegWriter( const QUrl & uri );

	void setQuality( double quality );

protected:
	/**
	 * @brief setup muxer
	 * @throws CodecError Unknown output format
	 * @throws SystemError Memory allocation failure
	 */
	virtual void setupMuxer();
	/**
	 * @brief setup encoder
	 * @throws CodecError Can not setup encoder
	 */
	virtual void setupEncoder();
	virtual void openResource();
	virtual void closeResource();
	virtual void writeHeader();
	virtual void writeFrame( const char * sample );
	virtual qint64 writeData( const char * data, qint64 len );
	virtual void doOpen();
	virtual void doClose();

private:
#ifdef Q_OS_WIN32
	std::shared_ptr< QIODevice > fio_;
	std::shared_ptr< AVIOContext > pIOContext_;
#endif
	std::shared_ptr< AVFormatContext > pFormatContext_;
	std::shared_ptr< AVCodecContext > pCodecContext_;
	std::shared_ptr< AVFrame > pFrame_;
	AVStream * pStream_;
	QByteArray queue_;
	double quality_;
	int frameSize_;
	int sampleLength_;
};

}
}

#endif
