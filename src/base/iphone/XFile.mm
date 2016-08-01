// XFile.cpp: implementation of the XFile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XFile.h"
#include "XString.h"
#include "MultiStringUtil.h"
#include "IPhoneDeviceData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern iOSDeviceRelateData g_deviceData;
//#ifdef __IPHONE__
//extern XString8 g_documentDirPath_iPhone;
//extern XString8 g_bundlePath_iPhone;
//#elif defined(__IPAD__)
extern XString8 g_documentDirPath_ios;
extern XString8 g_bundlePath_ios;
//#endif


XFile::XFile()
{
	m_file=XNULL;
}

XFile::~XFile()
{
	Close();
}

void XFile::Close()
{
	if(m_file==XNULL) return;
	::fclose(m_file);
	m_file=XNULL;
}

#ifdef __APPLE__
void translateFileName(XPCTSTR sourceFile, XString8& destFile, XBOOL isResource);
void translateFileName(XPCTSTR sourceFile, XString8& destFile)
{
	const char* p = sourceFile;
	if (!p) return;
    bool isResource = false;
    XString8 disk;
    if ((*(p+1)) && (*(p+1)==':'))
    {
        if ((*p=='c')||(*p=='C'))
            isResource = true;
    }
    if (isResource)
    {
        translateFileName(sourceFile, destFile, XTRUE);
        return;
    }
	while(*p)
	{
		if (*p == ':')
		{
			destFile = p + 1;
			p++;
			break;
		}
		p++;
	};

        if (*p)
        {
//#ifdef __IPHONE__
//            destFile = g_documentDirPath_iPhone + destFile;
//#elif defined(__IPAD__)
            destFile = g_documentDirPath_ios + destFile;
//#endif
        }
        else
        {
//#ifdef __IPHONE__
//            destFile = g_documentDirPath_iPhone + sourceFile;
//#elif defined(__IPAD__)
            destFile = g_documentDirPath_ios + sourceFile;
//#endif
        }
    
	APPLEASSERT(destFile.GetLength() > 0);
	if (destFile.GetData()[destFile.GetLength() - 1] == '/')
	{
		destFile.GetData()[destFile.GetLength() - 1] = 0;
		destFile.RemoveLast();
	}
	char* utf8 = gbkToUtf8(destFile);
	destFile = utf8;
	delete[] utf8;
}

void translateFileName(XPCTSTR sourceFile, XString8& destFile, XBOOL isResource)
{
	const char* p = sourceFile;
	if (!p) return;
	while(*p)
	{
		if (*p == ':')
		{
			destFile = p + 1;
			p++;
			break;
		}
		p++;
	};
	
    if (!isResource)
    {
        if (*p)
        {
            destFile = g_documentDirPath_ios + destFile;
        }
        else
        {
            destFile = g_documentDirPath_ios + sourceFile;
        }
    }
    else
    {
        if (*p)
        {
            destFile = g_bundlePath_ios + destFile;
        }
        else
        {
            destFile = g_bundlePath_ios + sourceFile;
        }
    }
    
	APPLEASSERT(destFile.GetLength() > 0);
	if (destFile.GetData()[destFile.GetLength() - 1] == '/')
	{
		destFile.GetData()[destFile.GetLength() - 1] = 0;
		destFile.RemoveLast();
	}
	char* utf8 = gbkToUtf8(destFile);
	destFile = utf8;
	delete[] utf8;
}

#endif

XBOOL XFile::Open(XPCTSTR strFile, XU8 nStyle, XBOOL isResource)
{
	if(strFile==XNULL||
	   strlen(strFile)==0) return XFALSE;
	XString8 str;
	
#ifdef __APPLE__
	XString8 temp;
	translateFileName(strFile, temp, isResource);
#endif
	
	if(nStyle&XAPPEND)
	{
		str+='a';
		if(nStyle&XREAD) str+='+';
	}
	else if(nStyle&XCREATE)
	{
		str+='w';
		if(nStyle&XREAD) str+='+';
	}
	else if(nStyle&XREAD)
	{
		str+='r';
		if(nStyle&XWRITE) str+='+';
	}
	
	if(nStyle&XBINARY) str+='b';
#ifdef __APPLE__
	m_file=::fopen(temp,str);
#else
	m_file=::fopen(strFile,str);
#endif
	return m_file!=XNULL;
}

XBOOL XFile::Open(XPCTSTR strFile, XU8 nStyle)
{
	if(strFile==XNULL||
	   strlen(strFile)==0) return XFALSE;
	XString8 str;
	
#ifdef __APPLE__
	XString8 temp;
	translateFileName(strFile, temp);
#endif
	
	if(nStyle&XAPPEND)
	{
		str+='a';
		if(nStyle&XREAD) str+='+';
	}
	else if(nStyle&XCREATE)
	{
		str+='w';
		if(nStyle&XREAD) str+='+';
	}
	else if(nStyle&XREAD)
	{
		str+='r';
		if(nStyle&XWRITE) str+='+';
	}
	
	if(nStyle&XBINARY) str+='b';
#ifdef __APPLE__
	m_file=::fopen(temp,str);
#else
	m_file=::fopen(strFile,str);
#endif
	return m_file!=XNULL;
}

XBOOL XFile::Seek(int nSize, XU8 nType)
{
	if(m_file==XNULL) return XFALSE;
	switch(nType)
	{
		default:nType=SEEK_CUR;break;
		case XSEEK_SET:nType=SEEK_SET;break;
		case XSEEK_CUR:nType=SEEK_CUR;break;
		case XSEEK_END:nType=SEEK_END;break;
	}
	return ::fseek(m_file,nSize,nType)==0;
}

int XFile::Tell()
{
	if(m_file==XNULL) return 0;
	return ::ftell(m_file);
}

int XFile::GetLength()
{
	if(m_file==XNULL) return 0;
	int v=Tell();
	Seek(0,XSEEK_END);
	int l=Tell();
	Seek(v,XSEEK_SET);
	return l;
}

int XFile::Write(const void *pData, int nSize)
{
	if(m_file==XNULL) return 0;
	return ::fwrite(pData,1,nSize,m_file);
}

int XFile::Read(void *pData, int nSize)
{
	if(m_file==XNULL) return 0;
	return ::fread(pData,1,nSize,m_file);
}

XBOOL XFile::RemoveFile(XPCTSTR strFile)
{
	XString8 temp;
	translateFileName(strFile, temp);
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSString *str =[[NSString alloc] initWithCString:temp.GetData() encoding:NSUTF8StringEncoding];

	NSError *error = nil;
	if (![fileManager removeItemAtPath:str error:&error]) {
		
		[str release];
//		NSLog(@"remove file Error: filepath = %@, reason = %@", str, error);
		return false;
	}
	[str release];
	return true;
}

void XFileFinder::clearTempFileList()
{
	for (int i = 0; i < m_tempFilelist.GetSize(); ++i)
	{
		delete[] m_tempFilelist[i];
	}
	m_tempFilelist.RemoveAll();
	m_tempFileCursor = 0;
}

XBOOL XFileFinder::FindPath(XPCTSTR strPath)
{
	clearTempFileList();

	XString8 temp;
	translateFileName(strPath, temp);
	
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSString *str = [[NSString alloc] initWithCString:temp.GetData() encoding:NSUTF8StringEncoding];
	
	if (![fileManager fileExistsAtPath:str])
	{
		clearTempFileList();
		[str release];
		m_hHandle = 0;
		return XFALSE;
	}
	NSError *error = nil;
	
	// judge whether this is directory.
	NSDictionary* dict = [fileManager attributesOfItemAtPath:str error:&error];
	[str release];
	if (dict == nil)
	{
//		NSLog(@"find path %@", error);
		m_hHandle = (XHANDLE)0;
		return false;
	}
	else
	{
		if ([dict fileType] == NSFileTypeDirectory)
			m_isDirectory = true;
		else 
			m_isDirectory = false;
		nFileSizeHigh = 0;
		nFileSizeLow = [dict fileSize];
		strcpy(cFileName, strPath);

		NSDate* date = [dict fileCreationDate];
		XU64 interval = [date timeIntervalSince1970] * 1000;;
		ftCreationTime.dwHighDateTime = interval >> 32;
		ftCreationTime.dwLowDateTime = interval & 0xffffffff;
		interval = [[dict fileModificationDate] timeIntervalSince1970] * 1000;
		ftLastAccessTime.dwHighDateTime = interval >> 32;
		ftLastAccessTime.dwLowDateTime = interval & 0xffffffff;
		ftLastWriteTime = ftLastAccessTime;
	}
	m_hHandle = (XHANDLE)1;
	return true;
}

XBOOL XFileFinder::FindFirst(XPCTSTR strPath)
{
	clearTempFileList();
	XString8 temp;
	translateFileName(strPath, temp);
	
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSString *str = [[NSString alloc] initWithCString:temp.GetData() encoding:NSUTF8StringEncoding];
	
	if (![fileManager fileExistsAtPath:str])
	{
//		NSLog(@"%@", str);
		clearTempFileList();
		[str release];
		m_hHandle = 0;
		return XFALSE;
	}
	NSError *error = nil;
	
	// judage whether this is directory.
	NSDictionary* dict = [fileManager attributesOfItemAtPath:str error:&error];
	if (dict == nil)
	{
		[str release];
//		NSLog(@"find first%@", error);
		m_hHandle = (XHANDLE)0;
		return false;
	}
	else
	{
		if ([dict fileType] == NSFileTypeDirectory)
			m_isDirectory = true;
		else 
			m_isDirectory = false;
		nFileSizeHigh = 0;
		nFileSizeLow = [dict fileSize];
		strcpy(cFileName, strPath);
		NSDate* date = [dict fileCreationDate];
		XU64 interval = [date timeIntervalSince1970] * 1000;;
		ftCreationTime.dwHighDateTime = interval >> 32;
		ftCreationTime.dwLowDateTime = interval & 0xffffffff;
		interval = [[dict fileModificationDate] timeIntervalSince1970] * 1000;
		ftLastAccessTime.dwHighDateTime = interval >> 32;
		ftLastAccessTime.dwLowDateTime = interval & 0xffffffff;
		ftLastWriteTime = ftLastAccessTime;
	}
	
	
	
	if (m_isDirectory)
	{
		m_parentPath = strPath;
		NSArray* filelist = [fileManager contentsOfDirectoryAtPath:str error:&error];
		if (filelist == nil)
		{
			[str release];
//			NSLog(@"find first content of directoryError %@", error);
			m_hHandle = (XHANDLE)0;
			return XFALSE;
		}
		NSEnumerator *enumerator = [filelist objectEnumerator];
		NSString* anObject;
		
		while (anObject = [enumerator nextObject]) {
			int bufferSize = [anObject maximumLengthOfBytesUsingEncoding:NSUTF8StringEncoding];
			char* buffer = new char[bufferSize];
			[anObject getCString:buffer maxLength:bufferSize encoding:NSUTF8StringEncoding];
			
			m_tempFilelist.Add(utf8ToGbk(buffer));
			delete[] buffer;

		}
		[str release];
		return FindNext();
	}
	
	[str release];
	m_hHandle = (XHANDLE)1;
	return true;
}

XBOOL XFileFinder::FindNext()
{
	if (m_tempFileCursor < m_tempFilelist.GetSize())
	{
		NSFileManager *fileManager = [NSFileManager defaultManager];
		
		XString8 temp;
		XString8 noprefixpath = m_parentPath + "/" + m_tempFilelist[m_tempFileCursor];
		translateFileName(noprefixpath, temp);
		
		NSString* str = [[NSString alloc] initWithCString:temp.GetData() encoding:NSUTF8StringEncoding];
		m_tempFileCursor++;
		NSError* error;
		NSDictionary* dict = [fileManager attributesOfItemAtPath:str error:&error];
		if (dict == nil)
		{
			
//			NSLog(@"find ext attribute found no dict path = %@, %@", str, error);
			[str release];
			m_hHandle = 0;
			return false;
		}
		else
		{
			if ([dict fileType] == NSFileTypeDirectory)
				m_isDirectory = true;
			else 
				m_isDirectory = false;
			nFileSizeHigh = 0;
			nFileSizeLow = [dict fileSize];
			strcpy(cFileName, m_tempFilelist[m_tempFileCursor - 1]);
			NSDate* date = [dict fileCreationDate];
			XU64 interval = [date timeIntervalSince1970] * 1000;;
			ftCreationTime.dwHighDateTime = interval >> 32;
			ftCreationTime.dwLowDateTime = interval & 0xffffffff;
			interval = [[dict fileModificationDate] timeIntervalSince1970] * 1000;
			ftLastAccessTime.dwHighDateTime = interval >> 32;
			ftLastAccessTime.dwLowDateTime = interval & 0xffffffff;
			ftLastWriteTime = ftLastAccessTime;
			[str release];
			m_hHandle = (XHANDLE)1;
			return true;
		}
	}
	else
	{
		m_hHandle = 0;
		return false;
	}
}

XFileFinder::~XFileFinder()
{
	Close();
}

void XFileFinder::Close()
{
	m_hHandle=XNULL;
	clearTempFileList();
	
}

XBOOL XFileFinder::IsDirectory()
{
	return m_isDirectory;
}

int XFileFinder::GetType(XString8 &strFile)
{
	int nType=0;
	XFileFinder find;
	if(!find.FindFirst(strFile))
		nType=0;
	else
	{
		if(find.IsDirectory()) nType=2;
		else nType=1;
	}
	find.Close();
	return nType;
}

void XFileFinder::GetDriverList(XString8Array &drivers, XString8Array &names)
{
	drivers.RemoveAll();
	names.RemoveAll();	
}

XBOOL XFile::CreateFolder(XPCTSTR strPath)
{
	XString8 temp;
	translateFileName(strPath, temp);
	
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSString *str =[[NSString alloc] initWithCString:temp.GetData() encoding:NSUTF8StringEncoding];
	NSError *error = nil;

	if (![fileManager createDirectoryAtPath:str withIntermediateDirectories:YES attributes:nil error:&error])
	{
		[str release];
//		NSLog(@"create directory Error: path = %@%@", str, error);
		return XFALSE;
	}
	[str release];
	return XTRUE;

}

XBOOL XFileFinder::GetFileTime(XPCTSTR strFile, XTime &time, XU8 ts)
{
    if(!FindFirst(strFile)) return XFALSE;
	switch(ts)
	{
		default:
		case XTM_CREATE:
			time.SetTime(ftCreationTime);break;
		case XTM_ACCESS:
			time.SetTime(ftLastAccessTime);break;
		case XTM_MODIFY:
			time.SetTime(ftLastWriteTime);break;
	}
	return XTRUE;
}

int XFile::WriteString(XString8 &str)
{
	if(m_file==XNULL) return 0;
	XU32 nSize=str.GetLength();
	int s=sizeof(nSize);
	Write(&nSize,sizeof(nSize));
	if(nSize>0)
		s+=Write(str.GetData(),nSize);
	return s;
}

int XFile::ReadString(XString8 &str)
{
	if(m_file==XNULL) return XFALSE;
	int nSize;
	int s=0;
	s+=Read(&nSize,sizeof(nSize));
	str.SetSize(nSize+1,XFALSE);
	s+=Read(str.GetData(),nSize);
	return s;
}

XBOOL XFile::RemoveFolder(XPCTSTR strFile)
{
	XString8 temp;
	translateFileName(strFile, temp);
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSString *str =[[NSString alloc] initWithCString:temp.GetData() encoding:NSUTF8StringEncoding];
	//	NSURL *url = [[NSURL alloc] initWithString: str];
	NSError *error = nil;
	if (![fileManager removeItemAtPath:str error:&error]) {
		[str release];
//		NSLog(@"remove folder Error: %@", error);
		return false;
	}
	[str release];
	return true;
}

XBOOL XFile::LoadText(XPCTSTR strFile, XString8 &strTxt)
{
	XFile file;
	if(!file.Open(strFile,XFile::XREAD)) 
	{
//		printf("no this file = %s\n", strFile);
		return XFALSE;
	}
	int l=file.GetLength();
	if(l<=0) {file.Close();return XFALSE;}
	strTxt.SetSize(l+1,XFALSE);
	file.Read(strTxt.GetData(),l);
	file.Close();
	return XTRUE;
}
///////////////////////////////////////////////////////////////
void MD5::Transform(MD5State& stat, char buffer[], int shift)
{
    int a = stat.state[0];
    int b = stat.state[1];
    int c = stat.state[2];
    int d = stat.state[3];
	int x[16];
    Decode(buffer, 64, shift,x);
    a = FF(a, b, c, d, x[0], 7, 0xd76aa478);
    d = FF(d, a, b, c, x[1], 12, 0xe8c7b756);
    c = FF(c, d, a, b, x[2], 17, 0x242070db);
    b = FF(b, c, d, a, x[3], 22, 0xc1bdceee);
    a = FF(a, b, c, d, x[4], 7, 0xf57c0faf);
    d = FF(d, a, b, c, x[5], 12, 0x4787c62a);
    c = FF(c, d, a, b, x[6], 17, 0xa8304613);
    b = FF(b, c, d, a, x[7], 22, 0xfd469501);
    a = FF(a, b, c, d, x[8], 7, 0x698098d8);
    d = FF(d, a, b, c, x[9], 12, 0x8b44f7af);
    c = FF(c, d, a, b, x[10], 17, -42063);
    b = FF(b, c, d, a, x[11], 22, 0x895cd7be);
    a = FF(a, b, c, d, x[12], 7, 0x6b901122);
    d = FF(d, a, b, c, x[13], 12, 0xfd987193);
    c = FF(c, d, a, b, x[14], 17, 0xa679438e);
    b = FF(b, c, d, a, x[15], 22, 0x49b40821);
    a = GG(a, b, c, d, x[1], 5, 0xf61e2562);
    d = GG(d, a, b, c, x[6], 9, 0xc040b340);
    c = GG(c, d, a, b, x[11], 14, 0x265e5a51);
    b = GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);
    a = GG(a, b, c, d, x[5], 5, 0xd62f105d);
    d = GG(d, a, b, c, x[10], 9, 0x2441453);
    c = GG(c, d, a, b, x[15], 14, 0xd8a1e681);
    b = GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);
    a = GG(a, b, c, d, x[9], 5, 0x21e1cde6);
    d = GG(d, a, b, c, x[14], 9, 0xc33707d6);
    c = GG(c, d, a, b, x[3], 14, 0xf4d50d87);
    b = GG(b, c, d, a, x[8], 20, 0x455a14ed);
    a = GG(a, b, c, d, x[13], 5, 0xa9e3e905);
    d = GG(d, a, b, c, x[2], 9, 0xfcefa3f8);
    c = GG(c, d, a, b, x[7], 14, 0x676f02d9);
    b = GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);
    a = HH(a, b, c, d, x[5], 4, 0xfffa3942);
    d = HH(d, a, b, c, x[8], 11, 0x8771f681);
    c = HH(c, d, a, b, x[11], 16, 0x6d9d6122);
    b = HH(b, c, d, a, x[14], 23, 0xfde5380c);
    a = HH(a, b, c, d, x[1], 4, 0xa4beea44);
    d = HH(d, a, b, c, x[4], 11, 0x4bdecfa9);
    c = HH(c, d, a, b, x[7], 16, 0xf6bb4b60);
    b = HH(b, c, d, a, x[10], 23, 0xbebfbc70);
    a = HH(a, b, c, d, x[13], 4, 0x289b7ec6);
    d = HH(d, a, b, c, x[0], 11, 0xeaa127fa);
    c = HH(c, d, a, b, x[3], 16, 0xd4ef3085);
    b = HH(b, c, d, a, x[6], 23, 0x4881d05);
    a = HH(a, b, c, d, x[9], 4, 0xd9d4d039);
    d = HH(d, a, b, c, x[12], 11, 0xe6db99e5);
    c = HH(c, d, a, b, x[15], 16, 0x1fa27cf8);
    b = HH(b, c, d, a, x[2], 23, 0xc4ac5665);
    a = II(a, b, c, d, x[0], 6, 0xf4292244);
    d = II(d, a, b, c, x[7], 10, 0x432aff97);
    c = II(c, d, a, b, x[14], 15, 0xab9423a7);
    b = II(b, c, d, a, x[5], 21, 0xfc93a039);
    a = II(a, b, c, d, x[12], 6, 0x655b59c3);
    d = II(d, a, b, c, x[3], 10, 0x8f0ccc92);
    c = II(c, d, a, b, x[10], 15, 0xffeff47d);
    b = II(b, c, d, a, x[1], 21, 0x85845dd1);
    a = II(a, b, c, d, x[8], 6, 0x6fa87e4f);
    d = II(d, a, b, c, x[15], 10, 0xfe2ce6e0);
    c = II(c, d, a, b, x[6], 15, 0xa3014314);
    b = II(b, c, d, a, x[13], 21, 0x4e0811a1);
    a = II(a, b, c, d, x[4], 6, 0xf7537e82);
    d = II(d, a, b, c, x[11], 10, 0xbd3af235);
    c = II(c, d, a, b, x[2], 15, 0x2ad7d2bb);
    b = II(b, c, d, a, x[9], 21, 0xeb86d391);
    stat.state[0] += a;
    stat.state[1] += b;
    stat.state[2] += c;
    stat.state[3] += d;
}
void MD5::Encode(int input[], int len,char*out)
{
    int j;
    int i = j = 0;
    for(; j < len; j += 4)
    {
        out[j] = (char)(input[i] & 0xff);
        out[j + 1] = (char)(AbsRight(input[i],8) & 0xff);
        out[j + 2] = (char)(AbsRight(input[i],16) & 0xff);
        out[j + 3] = (char)(AbsRight(input[i],24) & 0xff);
        i++;
    }
}

void MD5::Update(MD5State& stat, char buffer[], int tLen,int offset, int length)
{  
	fin=0;
    if(length - offset > tLen)
        length = tLen - offset;
    int index = AbsRight(stat.count[0],3) & 0x3f;
    if((stat.count[0] += length << 3) < length << 3)
        stat.count[1]++;
    stat.count[1] += AbsRight(length,29);
    int partlen = 64 - index;
    int i;
    if(length >= partlen)
    {
        for(i = 0; i < partlen; i++)
            stat.buffer[i + index] = buffer[i + offset];
		
        Transform(stat, stat.buffer, 0);
        for(i = partlen; i + 63 < length; i += 64)
            Transform(stat, buffer, i);
		
        index = 0;
    } else
    {
        i = 0;
    }
    if(i < length)
    {
        int start = i;
        for(; i < length; i++)
            stat.buffer[(index + i) - start] = buffer[i + offset];
		
    }
}

void MD5::Final(char out[16])
{
	MD5State finals(state);
	if(fin==0)		
    {
		fin=1;
		char bits[8]; 
		Encode(finals.count, 8,bits);
		int index = (finals.count[0] >> 3) & 0x3f;
		int padlen = index >= 56 ? 120 - index : 56 - index;
		Update(finals, padding, 64,0, padlen);
		Update(finals, bits, 8,0, 8);            
	}
    Encode(finals.state, 16,out);
}

void XFileFinder::GetFileName(XString8 &strPath, XString8 &strName, XBOOL bExt)
{
//	int id0=strPath.ReverseFind('\\');
//	if(id0<0) id0=strPath.ReverseFind('/');
//	int idd=strPath.ReverseFind('.');
//	if(idd<0) idd=strPath.GetLength();	
//	if(id0>=0)
//		strPath.Mid(strName,id0+1,idd-id0-1);		
//	else
//		strPath.Left(strName,idd);

	int idd=bExt?strPath.ReverseFind('&'):strPath.ReverseFind('.');
	
	int _id=idd>0?idd-1:strPath.GetLength()-1;
	XBOOL bFind=0;
	while(_id>=0)
	{
		switch(strPath[_id])
		{
			case '\\':
			case '/':
			case '%':
			case '?':
			case '=':
				bFind=XTRUE;
				break;
		}
		if(bFind) break;
		_id--;
	}
	
	
	if(idd<0) idd=strPath.GetLength();
	if(_id>=0)
		strPath.Mid(strName,_id+1,idd-_id-1);
	else
		strPath.Left(strName,idd);
	
}
