/**
 * @file wfile.cpp
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
#ifndef _WIN32
#error This file must compile on Windows only!
#endif

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avstring.h>
}

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>

#include <cwchar>
#include <cerrno>
#include <cassert>

namespace {

	static int wfile_open( URLContext * h, const char * filename, int flags ) {
		int ret = av_strstart( filename, "wfile://", &filename );
		if( ret < 0 ) {
			return AVERROR(ENOENT);
		}

		/// 4096 should be enough for a path name
		wchar_t wfilename[_MAX_PATH];
		ret = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, filename, -1, wfilename, sizeof(wfilename) / sizeof(*wfilename) );
		if( ret <= 0 ) {
			return AVERROR(ENOENT);
		}

		int access;
		if( flags & URL_RDWR ) {
			access = _O_CREAT | _O_TRUNC | _O_RDWR;
		} else if (flags & URL_WRONLY) {
			access = _O_CREAT | _O_TRUNC | _O_WRONLY;
		} else {
			access = _O_RDONLY;
		}
#ifdef O_BINARY
		access |= O_BINARY;
#endif
		int fd;
		ret = _wsopen_s( &fd, wfilename, access, _SH_DENYNO, _S_IREAD | _S_IWRITE );
		if( fd < 0 ) {
			const int ret = AVERROR(ENOENT);
			assert( ret < 0 );
			return ret;
		}
		h->priv_data = ( void * )( size_t )fd;
		return 0;
	}

	static int wfile_read( URLContext * h, unsigned char * buf, int size ) {
		int fd = (size_t)h->priv_data;
		int nBytes = _read(fd, buf, size);
		return nBytes;
	}

	static int wfile_write( URLContext * h, unsigned char * buf, int size ) {
		int fd = ( size_t )h->priv_data;
		int nBytes = _write( fd, buf, size );
		return nBytes;
	}

	static int64_t wfile_seek( URLContext * h, int64_t pos, int whence ) {
		//assert(whence == SEEK_SET || whence == SEEK_CUR || whence == SEEK_END);
		const int fd = ( size_t )h->priv_data;
		const __int64 nBytes = _lseeki64( fd, pos, whence );
		return nBytes;
	}

	static int wfile_close(URLContext *h) {
		int fd = ( size_t )h->priv_data;
		if( fd >= 0 ) {
			return _close(fd);
		}
		return 0;
	}

	URLProtocol wfile_protocol = {
		"wfile",
		wfile_open,
		wfile_read,
		wfile_write,
		wfile_seek,
		wfile_close
	};

	static inline int initialize() {
		static int initialized = 0;
		if( !initialized ) {
			initialized = 1;
			return av_register_protocol( &wfile_protocol );
		} else {
			return 0;
		}
	}

	static const int REGISTERED_ = initialize();

}
