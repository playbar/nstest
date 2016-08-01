#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "gzbuffer.h"

CFileAlikeBuffer::CFileAlikeBuffer(int maxsize, char * userbuf, int userdatalen)
{
//#if (__CORE_VERSION__>=0x02070200)
//#else
//	assert(maxsize > 0);
//#endif
	assert(userdatalen >= 0);

	this->maxsize =  maxsize;
	datapos = 0;
	
	if (userbuf)
	{
		useuserbuf = true;
		buf = userbuf;
		bufsize = maxsize;
		datalen = userdatalen;
	}
//#if (__CORE_VERSION__>=0x02070200)
	else if(maxsize>0)
	{
		useuserbuf = false;
		buf = (char *)malloc(maxsize);
		bufsize = buf ? maxsize : 0;
		datalen = 0;
	}
	else
	{
		useuserbuf = false;
		buf = 0;
		bufsize = 0;
		datalen = 0;
	}
//#else
//	else
//	{
//		useuserbuf = false;
//		buf = (char *)malloc(maxsize);
//		bufsize = buf ? maxsize : 0;
//		datalen = 0;
//	}
//#endif
}

CFileAlikeBuffer::~CFileAlikeBuffer()
{
	if (buf && !useuserbuf)
		free(this->buf);
}

int CFileAlikeBuffer::Length()
{
	if (!buf)
		return -1;

	return datalen;
}

int CFileAlikeBuffer::Tell()
{
	if (!buf)
		return -1;

	return datapos;
}

char * CFileAlikeBuffer::GetBuffer()
{
	 return buf;
}

int CFileAlikeBuffer::Read(void * buffer, int count)
{
	if (!buf)
		return -1;

	if (count < 0)
		return -1;

	int maxread = datalen - datapos;
	if (count > maxread)
		count = maxread;

	memcpy(buffer, buf + datapos, count);
	datapos += count;
	return count;
}
//#if (__CORE_VERSION__>=0x02070200)
char* CFileAlikeBuffer::LockBuffer(int nSize)
{
	if(!buf)
	{
		maxsize=nSize;
		bufsize=nSize;
		buf=(char *)malloc(maxsize);
	}
	else if(datapos+nSize>maxsize)
	{
		maxsize=datapos+nSize*2;
		bufsize=maxsize;
		char*newbuf=(char*)malloc(maxsize);
		if(newbuf)
		{
			memcpy(newbuf,buf,datapos);
		}
		free(buf);
		buf=newbuf;
	}
	return buf;
}
//#endif
int CFileAlikeBuffer::Write(void * buffer, int count)
{
//#if (__CORE_VERSION__>=0x02070200)
	if(!LockBuffer(count)) return -1;
//#else
//	if (!buf)
//		return -1;
//#endif
	if (count < 0)
		return -1;

	// max number of bytes can be writen
	int maxwrite = maxsize - datapos;
	if (count > maxwrite)
		count = maxwrite;

	// ensure buffer availibity
	if (Seek(count, SEEK_CUR) == -1)
		return -1;
	else
		Seek(-count, SEEK_CUR);

	memcpy(buf+datapos, buffer, count);
	datapos += count;
	if (datalen < datapos)
		datalen = datapos;
	return count;
}

long CFileAlikeBuffer::Seek(long offset, int origin)
{
	long newpos;

	switch (origin)
	{
	case SEEK_SET:
		newpos = offset;
		break;

	case SEEK_END:
		newpos = (long)datalen + offset;
		break;

	case SEEK_CUR:
		newpos = (long)datapos + offset;
		break;

	default:
		return -1;
	}

	if (newpos < 0 || newpos > (long)maxsize)
		return -1;

	if (bufsize < newpos)
	{
		if (useuserbuf)
		{
			return -1;
		}
		else
		{
			// readjust buffer, if required
			int newsize = bufsize;
			while (newsize > 0 && newsize < newpos )
			{
				newsize <<= 1;
			}
			if (newsize <= 0) // << may overflow
				return -1;

			if (newsize > maxsize)
				newsize = maxsize;

			if (newsize < newpos)
				return -1;

			char * newbuf = (char *)realloc(buf, newsize);
			if (!newbuf)
				return -1;

			buf = newbuf;
			bufsize = newsize;
		}
	}

	datapos = newpos;
	if (datapos > datalen)
		datalen = datapos;

	return 0;
}
