// XFile.cpp: implementation of the XFile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XFile.h"
#include "XString.h"
#include "XResource.h"
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "MultiStringUtil.h"


#if 0
#define LOGWHEREXFILE()  LOGWHERE()
#else
#define LOGWHEREXFILE()
#endif

XFile::XFile()
{
	LOGWHEREXFILE();
   m_file=XNULL;
}

XFile::~XFile()
{
	LOGWHEREXFILE();
   Close();
}

void XFile::Close()
{
	LOGWHEREXFILE();
   if(m_file==XNULL)
	   return;
   ::fclose(m_file);
   m_file=XNULL;
}
void translateFileName( XPCTSTR sourceFile, char* destFile)
{
	LOGWHEREXFILE();
	if( sourceFile == NULL || destFile == NULL )
		return;
	char* p = strstr(sourceFile, ":");
	if (!p)
	{
		p = (char*)sourceFile;
	}
	else
	{
		p = p + 1;
	}
//	strcpy( destFile, p );

	// zhengjl 2011.09.10
	char* utf8 = gbkToUtf8(p);
	strcpy( destFile, utf8 );
	delete[] utf8;


}

XBOOL XFile::Open(XPCTSTR strFile, XU8 nStyle)
{
	LOGWHEREXFILE();
   if(strFile==XNULL || strlen(strFile)==0)
	   return XFALSE;

   	XString8 str;
   	char temp[256] ={0};
	translateFileName(strFile, temp);

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
   m_file=fopen(temp,str.GetData());

   return m_file!=XNULL;
}

XBOOL XFile::Seek(int nSize, XU8 nType)
{
	LOGWHEREXFILE();
   if(m_file==XNULL) return XFALSE;
   switch(nType)
   {
   default:
	   nType=SEEK_CUR;
	   break;
   case XSEEK_SET:
	   nType=SEEK_SET;
	   break;
   case XSEEK_CUR:
	   nType=SEEK_CUR;
	   break;
   case XSEEK_END:
	   nType=SEEK_END;
	   break;
   }
   return ::fseek(m_file,nSize,nType)==0;
}

int XFile::Tell()
{
	LOGWHEREXFILE();
	if(m_file==XNULL)
		return 0;
	return ::ftell(m_file);
}

int XFile::GetLength()
{
	LOGWHEREXFILE();
	if(m_file==XNULL)
		return 0;
	int v=Tell();
	Seek(0,XSEEK_END);
	int l=Tell();
	Seek(v,XSEEK_SET);
	return l;
}

int XFile::Write(const void *pData, int nSize)
{
	LOGWHEREXFILE();
	if(m_file==XNULL)
		return 0;
	return ::fwrite(pData,1,nSize,m_file);
}

int XFile::Read(void *pData, int nSize)
{
	LOGWHEREXFILE();
	if(m_file==XNULL)
		return 0;
	return ::fread(pData,1,nSize,m_file);
}

XBOOL XFile::RemoveFile(XPCTSTR strFile)
{
	LOGWHEREXFILE();
	if(strFile==XNULL || strlen(strFile)==0)
		return XFALSE;
//	char temp[256] ={0};
//	translateFileName(strFile, temp);
//	return (0 == remove( temp ));
	return (0 == remove( strFile ));
}

XBOOL XFileFinder::FindFirst(XPCTSTR strPath)
{
	LOGWHEREXFILE();
	if(strPath==XNULL || strlen(strPath)==0)
		return XFALSE;
	struct dirent **namelist;
	char temp[256] ={0};
	translateFileName(strPath, temp);
	struct stat state;
	stat(temp, &state);

	m_isDirectory = S_ISDIR( state.st_mode);

	if (m_isDirectory)
	{
		if ((num_entries = scandir(temp, &namelist, NULL, NULL)) < 0)
		{
			m_isDirectory = false;
			return XFALSE;
		}
		else
		{
			m_hHandle = namelist;
			m_tempFileCursor = 0;
			m_parentPath = temp;
			return XTRUE;
		}
	}

	if (S_ISREG( state.st_mode))
	{
		strcpy(cFileName, strPath);
		nFileSizeHigh = 0;
		nFileSizeLow = state.st_size;

		XU64 interval = state.st_mtime * 1000;
		ftCreationTime.dwHighDateTime = interval >> 32;
		ftCreationTime.dwLowDateTime = interval & 0xffffffff;

		interval = state.st_atime * 1000;
		ftLastAccessTime.dwHighDateTime = interval >> 32;
		ftLastAccessTime.dwLowDateTime = interval & 0xffffffff;

		ftLastWriteTime = ftLastAccessTime;
		return XTRUE;
	}
	return false;
}

XBOOL XFileFinder::IsDirectory()
{
	LOGWHEREXFILE();
	return m_isDirectory;
}

XBOOL XFileFinder::FindNext()
{
	LOGWHEREXFILE();
	if(m_hHandle==XNULL)
		return XFALSE;

	if (m_tempFileCursor < num_entries)
	{
		struct dirent **namelist = (dirent **)m_hHandle;
		namelist = namelist + m_tempFileCursor;

	    if(strcmp((*namelist)->d_name, "..") == 0 || strcmp((*namelist)->d_name, ".") == 0)
	    {
	    	m_tempFileCursor++;
	    	return FindNext();
	    }

	    strcpy(cFileName, (*namelist)->d_name);
		cFileName[strlen(cFileName)] = '\0';

		char temp[256] ={0};
		XString8 noprefixpath = m_parentPath + "/" + cFileName;
		translateFileName(noprefixpath, temp);

		struct stat state;
		stat(temp, &state);

		m_isDirectory = S_ISDIR( state.st_mode);

		nFileSizeHigh = 0;
		nFileSizeLow = state.st_size;

		XU64 interval = state.st_mtime * 1000;
		ftCreationTime.dwHighDateTime = interval >> 32;
		ftCreationTime.dwLowDateTime = interval & 0xffffffff;

		interval = state.st_atime * 1000;
		ftLastAccessTime.dwHighDateTime = interval >> 32;
		ftLastAccessTime.dwLowDateTime = interval & 0xffffffff;

		ftLastWriteTime = ftLastAccessTime;

    	m_tempFileCursor++;
		return XTRUE;
	}
	else
	{
		return XFALSE;
	}

}


XFileFinder::~XFileFinder()
{
	LOGWHEREXFILE();
	Close();
}

void XFileFinder::Close()
{
	LOGWHEREXFILE();
	if( m_isDirectory && m_hHandle != XNULL )
	{
		struct dirent **namelist = (dirent **)m_hHandle;
		for (int i = 0; i < num_entries; i++)
		{
			 free(*namelist);
			 namelist++;
		}
		m_hHandle=XNULL;
	}
	return;
}

int XFileFinder::GetType(XString8 &strFile)
{
	//LOGWHERE();
	//return XNONE;
	LOGWHEREXFILE();
	int nType=0;
	XFileFinder find;
	if( ! find.FindFirst(strFile))
	{
		nType = XNONE;
	}
	else
	{
		if( find.IsDirectory())
			nType = XFOLDER;
		else
			nType = XFILE;
	}
	find.Close();
	return nType;

}

void XFileFinder::GetDriverList(XString8Array &drivers, XString8Array &names)
{
	LOGWHEREXFILE();
	drivers.RemoveAll();
	names.RemoveAll();
}

int CreateDir(const char *sPathName)
{
	LOGWHEREXFILE();

	if( sPathName == NULL )
		return 0;

	char DirName[256];
	strcpy(DirName, sPathName);
	int i, len = strlen(DirName);;
	if (DirName[len - 1] != '/')
		strcat(DirName, "/");

	len = strlen(DirName);

	for (i = 1; i < len; i++)
	{
		if (DirName[i] == '/')
		{
			DirName[i] = 0;
			if (access(DirName, 0) != 0)
			{
				if (mkdir(DirName, S_IRWXU) != 0)
				{
					return -1;
				}
			}
			DirName[i] = '/';
		}
	}
	return 0;
}

XBOOL XFile::CreateFolder(XPCTSTR strPath)
{
	LOGWHEREXFILE();
	if(strPath==XNULL || strlen(strPath)==0)
		return XFALSE;
	char temp[256] ={0};
	translateFileName(strPath, temp);
	int ret = CreateDir(temp);
	return ( 0 == ret);
}

XBOOL XFileFinder::GetFileTime(XPCTSTR strFile, XTime &time, XU8 ts)
{
	LOGWHEREXFILE();
	if(strFile==XNULL || strlen(strFile)==0)
		return XFALSE;

    if(!FindFirst(strFile))
    	return XFALSE;
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
	LOGWHEREXFILE();
	if(m_file==XNULL)
		return 0;
	XU32 nSize=str.GetLength();
	int s=sizeof(nSize);
	Write(&nSize,sizeof(nSize));
	if(nSize>0)
		s+=Write(str.GetData(),nSize);
	return s;
}

int XFile::ReadString(XString8 &str)
{
	LOGWHEREXFILE();
   if(m_file==XNULL)
	   return XFALSE;
   int nSize;
   int s=0;
   s+=Read(&nSize,sizeof(nSize));
   str.SetSize(nSize+1,XFALSE);
   s+=Read(str.GetData(),nSize);
   return s;
}

XBOOL XFile::RemoveFolder(XPCTSTR strFile)
{
	LOGWHEREXFILE();
	if(strFile == NULL || strlen(strFile)==0)
		return XFALSE;
//	char temp[256] ={0};
//	translateFileName(strFile, temp);

//    DIR* dp = NULL;
//    DIR* dpin = NULL;
//    char *pathname = (char*)malloc(256);
//    struct dirent* dirp;
////    dp = opendir(temp);
//    dp = opendir(strFile);
//    if(dp == NULL)
//    {
//        return XFALSE;
//    }
//
//    while((dirp = readdir(dp)) != NULL)
//    {
//        if(strcmp(dirp->d_name, "..") == 0 || strcmp(dirp->d_name, ".") == 0)
//            continue;
////        strcpy(pathname, temp);
//        strcpy(pathname, strFile);
//        strcat(pathname, "/");
//        strcat(pathname, dirp->d_name);
//        dpin = opendir(pathname);
//        if(dpin != NULL)
//        	RemoveFolder(pathname);
//        else
//            remove(pathname);
//        strcpy(pathname, "");
//        closedir(dpin);
//        dpin = NULL;
//    }
//    rmdir(temp);
    rmdir(strFile);
//    closedir(dp);
//    free(pathname);
//    pathname = NULL;
//    dirp = NULL;


	return XTRUE;
}

XBOOL XFile::LoadText(XPCTSTR strFile, XString8 &strTxt)
{
	LOGWHEREXFILE();
	if(strFile == NULL || strlen(strFile)==0)
		return XFALSE;
	XFile file;
	if(!file.Open(strFile,XFile::XREAD)) return XFALSE;
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
	LOGWHEREXFILE();
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
	LOGWHEREXFILE();
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
	LOGWHEREXFILE();
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
	LOGWHEREXFILE();
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



void XFileFinder::GetFileName(XString8 &strPath, XString8 &strName,XBOOL bExt)
{
	LOGWHEREXFILE();
//	int id=strPath.ReverseFind('\\');
//	if(id<0) id=strPath.ReverseFind('/');
//	int idd=bExt?strPath.ReverseFind('&'):strPath.ReverseFind('.');
//	if(idd<0) idd=strPath.GetLength();
//	if(id>=0)
//		strPath.Mid(strName,id+1,idd-id-1);
//	else
//		strPath.Left(strName,idd);

	int idd=bExt?strPath.ReverseFind('&'):strPath.ReverseFind('.');

	int id=idd>0?idd-1:strPath.GetLength()-1;
	XBOOL bFind=0;
	while(id>=0)
	{
		switch(strPath[id])
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
		id--;
	}
	if(idd<0) idd=strPath.GetLength();
	if(id>=0)
		strPath.Mid(strName,id+1,idd-id-1);
	else
		strPath.Left(strName,idd);
}
