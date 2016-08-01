// gziptest.cpp
#include <stdio.h>
#include <memory.h>
#include "gzipapis.h"

#define TESTBUFSIZE 1000000

char buf0[TESTBUFSIZE];
char buf1[TESTBUFSIZE];
char buf2[TESTBUFSIZE];
char buf3[TESTBUFSIZE];
char buft[100]; // tiny buffer

int n0, n1, n2, n3;

int main(int argc, char * argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "usage: gziptest gzipfile\n");
		return 1;
	}

	FILE * fin = fopen(argv[1], "rb");

	if (!fin)
	{
		fprintf(stderr, "fopen error\n");
		return 1;
	}

	int n0 = fread(buf0, 1, sizeof(buf0), fin);
	if (n0 == sizeof(buf0))
	{
		fprintf(stderr, "size of %s too large\n", argv[1]);
		return 1;
	}

	fprintf(stderr , "testing %s\n", argv[1]);

	int loop = 10000;
	for (int i=0; i<loop; ++i)
	{
		fprintf(stderr, "loop %d\n", i+1);
		fprintf(stderr, "gzip_decompress_buffer to tiny buffer\n");
		n1 = gzip_decompress_buffer(buf0, n0, buft, sizeof(buft)); // this call must fail
		if (n1 >= 0)
			break;
		fprintf(stderr, "gzip_decompress_buffer\n");
		n1 = gzip_decompress_buffer(buf0, n0, buf1, sizeof(buf1));
		if (n1 < 0)
			break;
		fprintf(stderr, "gzip_compress_buffer to tiny buffer\n");
		n2 = gzip_compress_buffer(buf1, n1, buft, sizeof(buft)); // this call must fail
		if (n2 >= 0)
			break;
		fprintf(stderr, "gzip_compress_buffer\n");
		n2 = gzip_compress_buffer(buf1, n1, buf2, sizeof(buf2));
		if (n2 < 0)
			break;
		fprintf(stderr, "gzip_decompress_buffer\n");
		n3 = gzip_decompress_buffer(buf2, n2, buf3, sizeof(buf3));

		if (n3 != n1 || memcmp(buf3, buf1, n1) != 0)
		{
			fprintf(stderr, "error: data not match, n1=%d, n3=%d\n", n1, n3);
			break;
		}

		{
			CFileAlikeBuffer gzbuf0(n0, buf0, n0);
			CFileAlikeBuffer gzbuf1(TESTBUFSIZE);
			CFileAlikeBuffer gzbuf2(TESTBUFSIZE);
			CFileAlikeBuffer gzbuf3(TESTBUFSIZE);

			fprintf(stderr, "gzip_decompress\n");
			n1 = gzip_decompress(&gzbuf0, &gzbuf1);
			if (n1 < 0)
				break;
			fprintf(stderr, "gzip_compress\n");
			gzbuf1.Seek(0, SEEK_SET);
			n2 = gzip_compress(&gzbuf1, &gzbuf2);
			if (n2 < 0)
				break;
			fprintf(stderr, "gzip_decompress\n");
			gzbuf2.Seek(0, SEEK_SET);
			n3 = gzip_decompress(&gzbuf2, &gzbuf3);

			if (n3 != n1 || memcmp(gzbuf1.GetBuffer(), gzbuf3.GetBuffer(), n1) != 0)
			{
				fprintf(stderr, "error: data not match, n1=%d, n3=%d\n", n1, n3);
				break;
			}
		}
	} // for
	if (loop == loop)
	{
		fprintf(stderr, "all test passed\n");
		return 0;
	}
	else
	{
		fprintf(stderr, "error\n");
		return 1;
	}
}



