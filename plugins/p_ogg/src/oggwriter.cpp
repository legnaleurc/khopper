/**
 * @file oggwriter.cpp
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
#include "oggwriter.hpp"
#include "util/error.hpp"
#include "util/text.hpp"

extern "C" {
#include <libavformat/avformat.h>
}

namespace khopper {

	namespace codec {

		OGGWriter::OGGWriter() {
		}

		void OGGWriter::setupMuxer() {
			this->DefaultWriter::setupMuxer();
			if( this->isVariable() ) {
				this->formatContext()->oformat->audio_codec = CODEC_ID_VORBIS;
			}
		}

	}

}
