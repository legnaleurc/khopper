extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avstring.h>
}

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>

#include <wchar.h>
#include <errno.h>
#include <assert.h>

static int wfile_open( URLContext * h, const char * filename, int flags ) {
	int err;
	err = av_strstart( filename, "wfile://", &filename );

	/// 4096 should be enough for a path name
	wchar_t wfilename[4096];
	int nChars = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, filename, -1, wfilename, sizeof(wfilename) / sizeof(*wfilename) );

	if( nChars <= 0 ) {
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
	err = _wsopen_s( &fd, wfilename, access, _SH_DENYNO, _S_IREAD | _S_IWRITE );
	if( fd < 0 ) {
		const int err = AVERROR(ENOENT);
		assert( err < 0 );
		return err;
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

#if LIBAVFORMAT_VERSION_MAJOR >= 52
static int64_t wfile_seek( URLContext * h, int64_t pos, int whence ) {
#else
static offset_t wfile_seek( URLContext *h, offset_t pos, int whence ) {
#endif
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
		return register_protocol( &wfile_protocol );
	} else {
		return 0;
	}
}

static const int REGISTERED_ = initialize();
