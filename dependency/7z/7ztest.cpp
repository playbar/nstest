// 7ztest.cpp
#include <stdio.h>
#include "7zwrapper.h"

#define TESTUNIT LZMA_HDR_LEN

void main(int argc, char * argv[])
{
	if (argc != 2)
	{
		printf("usage: cmd file\n");
		return;
	}

	FILE * fp = fopen(argv[1], "rb");
	if (fp)
	{
		Byte src[TESTUNIT+1];
		Byte dest[TESTUNIT+1];

		C7ZDecoder dec;

		if (fread(src, 1, LZMA_HDR_LEN, fp) == LZMA_HDR_LEN)
		{
			if (dec.Begin(src, LZMA_HDR_LEN) == SZ_OK)
			{
				SizeT srcLen;
				while ((srcLen=fread(src, 1, TESTUNIT, fp)) > 0)
				{
					SizeT srcPos = 0;
					while (srcPos < srcLen)
					{
						SizeT srcProcessed = srcLen - srcPos; // len of bytes to decode
						SizeT destProcessed = TESTUNIT; // output buffer size
						SRes res = dec.Decode(src+srcPos, &srcProcessed, dest, &destProcessed);
						if (res != SZ_OK)
							break;

						dest[destProcessed] = 0; // length of output is destProcessed
						printf("%s", dest); // do some work
						srcPos += srcProcessed;
					}
				}
			}
		}
		fclose(fp);
	}
}