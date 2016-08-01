#ifndef _GZIPAPIS_H
#define _GZIPAPIS_H

#include "gzbuffer.h"

#ifdef GZIPBUF
extern int gzip_compress(GZBUF bin, GZBUF bout);
extern int gzip_decompress(GZBUF bin, GZBUF bout);
extern int gzip_compress_buffer(char * inbuf, int inlen, char * outbuf, int outlen);
extern int gzip_decompress_buffer(char * inbuf, int inlen, char * outbuf, int outlen);
#endif // GZIPBUF

#endif // _GZIPAPIS_H

