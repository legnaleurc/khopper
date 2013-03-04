/**
 * @file flacwriter.hpp
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
#ifndef KHOPPER_CODEC_FLACWRITER_HPP
#define KHOPPER_CODEC_FLACWRITER_HPP

#include <vector>

#include <FLAC/stream_encoder.h>
#include <FLAC/metadata.h>

#include "khopper/writer.hpp"

namespace khopper {
namespace codec {

class FlacWriter: public Writer {
public:
	explicit FlacWriter( const QUrl & uri );

	void setOggMode( bool ogg );

protected:
	virtual void doOpen();
	virtual void doClose();
	virtual qint64 writeData( const char * data, qint64 len );

private:
	static void progressCallback_( const FLAC__StreamEncoder *, FLAC__uint64, FLAC__uint64, unsigned, unsigned, void * );

	std::shared_ptr< FLAC__StreamEncoder > pFE_;
	std::vector< std::shared_ptr< FLAC__StreamMetadata > > metadataOwner_;
	bool ogg_;
};

}
}

#endif
