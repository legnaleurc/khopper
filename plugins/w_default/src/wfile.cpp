extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avstring.h>
}

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <share.h>

#include <wchar.h>
#include <errno.h>
#include <assert.h>

static int wfile_open( URLContext * h, const char * filename, int flags ) {
	int access;
	int fd;

	av_strstart( filename, "ufile:", &filename );

	/// 4096 should be enough for a path name
	wchar_t wfilename[4096];
	int nChars = MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, filename, -1, wfilename, sizeof(wfilename) / sizeof(*wfilename) );

	if( nChars <= 0 ) {
		return AVERROR(ENOENT);
	}

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
	_wsopen_s( &fd, wfilename, _SH_DENYNO, access, 0666 );
	h->priv_data = ( void * )( size_t )fd;
	if( fd < 0 ) {
		const int err = AVERROR(ENOENT);
		assert( err < 0 );
		return err;
	}
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

//int modify_file_url_to_utf8(char* buffer, size_t buffer_size, const char*
//url)
//{
//   strncpy(buffer, "ufile:", buffer_size);
//   strncat(buffer, url, buffer_size);
//   return 0;
//}

//int modify_file_url_to_utf8(char* buffer, size_t buffer_size, const wchar_t*
//url)
//{
//   static const char ufile[] = "ufile:";
//   strncpy(buffer, ufile, buffer_size);
//
//   /// convert Unicode to multi-byte string
//   ERROR_IF(
//       int result = WideCharToMultiByte(
//       CP_UTF8,
//       0,
//       url,
//       -1,
//       buffer + (sizeof(ufile) - 1),
//       buffer_size - sizeof(ufile),
//       NULL,
//       NULL
//       ),
//       result <= 0,
//       return -1
//       );
//
//   return 0;
//}

//this is how to use this

//int main()
//{
// const wchar_t* my_unicode_filename = ...
// char utf8url[1024];
// modify_file_url_to_utf8(utf8url, sizeof(utf8url), my_unicode_filename);
// AVFormatContext* ctx;
// av_open_input_file(&ctx, utf8url, NULL, 0, NULL);
//
//}

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
