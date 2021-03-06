/**
 * @file oggwriter.hpp
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
#ifndef KHOPPER_CODEC_OGGWRITER_HPP
#define KHOPPER_CODEC_OGGWRITER_HPP

#include <vorbis/vorbisenc.h>

#include "khopper/writer.hpp"

namespace khopper {
namespace codec {

/**
 * @brief Ogg writer
 *
 * This class provides a ogg audio writer implementation.
 */
class OggWriter: public Writer {
public:
	/**
	 * @brief Default constructor
	 */
	explicit OggWriter( const QUrl & uri );

	void setVBRQuality( float quality );

protected:
	virtual void doOpen();
	virtual void doClose();
	virtual qint64 writeData( const char * data, qint64 len );

private:
	void writePage_( const ogg_page & );

	QIODevice * out_;
	std::shared_ptr< vorbis_info > encoder_;
	std::shared_ptr< ogg_stream_state > muxer_;
	std::shared_ptr< vorbis_dsp_state > dsp_;
	std::shared_ptr< vorbis_block > block_;
	std::shared_ptr< vorbis_comment > comments_;
	float quality_;
};

}
}

#endif
