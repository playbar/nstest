// gzbuffer.h

#ifndef _GZBUFFER_H
#define _GZBUFFER_H

class CGzipException
{
	char errmsg[1]; // TODO
};

#define GZIPBUF

class CFileAlikeBuffer
{
private:

	enum { DEFBUFSIZE = 1024 };

	bool useuserbuf;
	char * buf;
	int maxsize; // max possible buffer size
	int bufsize; // buffer size
	int datalen;  // data size
	int datapos; // file pointer
	
public:
//#if (__CORE_VERSION__>=0x02070200)
	char* LockBuffer(int nSize);
//#endif
	CFileAlikeBuffer(int maxsize, char * userbuf = 0, int userdatalen = 0);
	~CFileAlikeBuffer();
	int Read(void * buffer, int count);
	int Write(void * buffer, int count);
	long Seek(long offset, int origin);
	int Length();
	int Tell();
	char * GetBuffer();
};

typedef CFileAlikeBuffer * GZBUF;

#endif // _GZBUFFER_H



