// XFile.cpp: implementation of the XFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XFile.h"
#include "XString.h"
#include "XResource.h"
#ifdef _SYMBIAN
#include "ZCFile.h"
#include "ZCConvert.h"
#include "symbiandebug.h"
//#include <libc\sys\reent.h>
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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

XBOOL XFile::Open(XPCTSTR strFile, XU8 nStyle)
{
   if(strFile==XNULL||
	  strlen(strFile)==0) return XFALSE;
   char *fname=NULL;
   fname = (char*)ZCConvert::ConvertGBKtoUTF8((TText8*)strFile);
//   for(int i=0;strFile[i];i++)
//	   {
//	   if(strFile[i] > 127 || strFile[i]<0)
//		   {
//		   fname = (char*)ZCConvert::ConvertGBKtoUTF8((TText8*)strFile);
//		   break;
//		   }
//	   }
   XString8 str;
   
   if(nStyle&XAPPEND)
   {
	   str+='a';
	   if(nStyle&XBINARY) str+='b';
	   if(nStyle&XREAD) str+='+';
   }
   else if(nStyle&XCREATE)
   {
	  str+='w';
	  if(nStyle&XBINARY) str+='b';
	  if(nStyle&XREAD) str+='+';
   }
   else if(nStyle&XREAD)
   {
	   str+='r';
	   if(nStyle&XBINARY) str+='b';
	   if(nStyle&XWRITE) str+='+';
   }
   if(fname)
	   {
	   m_file=fopen(fname,str.GetData());			//�޷����ļ����д���gbk�����ַ���ļ�
	   delete fname;
	   }
   else
	   m_file=fopen(strFile,str.GetData());

   if(!m_file)
	   {
	   SYMBIANDEB1(DefaultLog,"file %s is not open\r\n",strFile);
	   }
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
#ifdef _SYMBIAN
	char *buf = new char[strlen(strFile)+1];
	char *temp = buf;
	strcpy(buf,strFile);
	while(*temp)
		{
		if(*temp == '/') *temp ='\\';
		temp++;
		}
	TText16* testName;
	testName= ZCConvert::ConvertFromUTF8((TText8*)buf);
	//XString16 strTmp(strFile);
	RFs fsession;
//	User::LeaveIfError(fsession.Connect());
	fsession.Connect();
		TPtrC filename(testName);
		XBOOL bOK;
		bOK=fsession.Delete(filename);
	fsession.Close();
	delete testName;
	return bOK;
#elif defined(_WIN32)
	return ::DeleteFileA(strFile);
#endif
}
typedef FILE PFILE;
XBOOL XFileFinder::FindFirst(XPCTSTR strPath)
{
#ifdef _SYMBIAN
	int l=XString8::SafeStrlen(strPath)-1;
	if(l<=0) return XFALSE;	
	char* utf8 = (char*)ZCConvert::ConvertGBKtoUTF8((TText8*)strPath);
	XString16 tempstr;
	tempstr = utf8;
	delete utf8;
	unsigned short *temp = tempstr.GetData();
	for(int i =0 ; i<=l;i++)
			{
			if(temp[i] == '/') 
				temp[i] = '\\';
			}
	TPtrC thePath(temp);
	if(!(strPath[l]=='/'||strPath[l]=='\\'))
	{
		TEntry entry;
		if(!((ZCFile*)m_hFileContain)->ZCGetEntry(thePath,entry))
			{
				{PFILE *fl; 
				fl=fopen(DefaultLog,"a"); 
				if(fl){ fprintf(fl,"file:%s	line:%d function:%s\r\n",__FILE__,__LINE__,__FUNCTION__) ; 
				fprintf(fl,"file %s is not find\r\n",strPath); fclose(fl);}}
			return XFALSE;
			}
		
//		TPtr filename((TUint16*)cFileName,XMAX_PATH);
		XString8 filename;
		filename=entry.iName.Des().PtrZ();
		int len = filename.GetLength();
		memmove(cFileName,filename.GetData(),len+1);
		dwFileAttributes=entry.iAtt;           //��д�ļ�����
		//KEntryAttNormal
			
		nFileSizeLow=entry.iSize;              //��д�ļ���С
			
		TInt64 tempTime=entry.iModified.Int64();       //��д�ļ�ʱ��
		ftLastWriteTime.dwHighDateTime=(tempTime>>32);
		ftLastWriteTime.dwLowDateTime=tempTime;
		ftCreationTime=ftLastWriteTime;
		ftLastAccessTime=ftLastWriteTime;		
		return XTRUE;
	}
	else
		{
		if(m_hDir) delete (CDir*)m_hDir;
		m_hDir=XNULL;
		m_hDir = ((ZCFile*)m_hFileContain)->ZCGetDir(thePath); //ȡ���ļ��б�
		if(m_hDir)
			{
			if(((CDir*)m_hDir)->Count()>0)
				{
				m_nFlag=0;
				TEntry entry = (*((CDir*)m_hDir))[0];
				XString8 filename;									//��д�ļ���
				filename = entry.iName.Des().PtrZ();
				memmove(cFileName,filename.GetData(),filename.GetLength()+1);
				dwFileAttributes=(*((CDir*)m_hDir))[0].iAtt;           //��д�ļ�����
				
				nFileSizeLow=(*((CDir*)m_hDir))[0].iSize;              //��д�ļ���С
				
				TInt64 tempTime=(*((CDir*)m_hDir))[0].iModified.Int64();       //��д�ļ�ʱ��
				ftLastWriteTime.dwHighDateTime=(tempTime>>32);
				ftLastWriteTime.dwLowDateTime=tempTime;
				ftCreationTime=ftLastWriteTime;
				ftLastAccessTime=ftLastWriteTime;
				return XTRUE;
				}
			else
				{
				delete (CDir*)m_hDir;
				m_hDir = XNULL;
				return XFALSE;
				}
			}
		else
			{
			if(m_hDir) delete m_hDir;
			m_hDir = XNULL;
			return XFALSE;
			}
//		return m_hDir!=XNULL;
		}
#elif defined(_WIN32)
    m_hHandle=XNULL;
	int l=XString8::SafeStrlen(strPath)-1;
	if(l<=0) return XFALSE;	
	if(strPath[l]=='/'||strPath[l]=='\\')
	{
		XString8 str=strPath;
		if(strPath[l]=='/')
			str+="/*.*";
		else
			str+="\\*.*";
		m_hHandle=::FindFirstFileA(str,(WIN32_FIND_DATAA*)this);
	}
	else 
		m_hHandle=::FindFirstFileA(strPath,(WIN32_FIND_DATAA*)this);
	if(m_hHandle==INVALID_HANDLE_VALUE) 
		m_hHandle=XNULL;
	return m_hHandle!=XNULL;
#endif
}


XBOOL XFileFinder::IsDirectory()
{
#ifdef _SYMBIAN
	/*TText16* tempname;
	tempname=ZCConvert::ConvertToUnicode((TText8*)m_hHandle);
	TPtrC fileName(tempname);
	int err=((ZCFile*)m_hFileContain)->ZCIsDir(fileName);
	delete tempname;
	return err;*/
	/*if(m_hDir==XNULL) return XFALSE;
	CDir* pDir=(CDir*)m_hDir;
	if(m_nFlag>=pDir->Count()) return XFALSE;
	return ((*pDir)[m_nFlag]).IsDir();*/
	return dwFileAttributes&KEntryAttDir;
#elif defined(_WIN32)	
	return (dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY);
#endif
}

XBOOL XFileFinder::FindNext()
{
#ifdef _SYMBIAN
	if(m_hDir==XNULL) return XFALSE;
	m_nFlag++;
	//CDir*pDib=(CDir*)m_hDir;
	if(m_nFlag>=((CDir*)m_hDir)->Count()) return XFALSE;
	TEntry entry = (*((CDir*)m_hDir))[m_nFlag];
	XString8 filename;									//��д�ļ���
	filename = entry.iName.Des().PtrZ();
	memmove(cFileName,filename.GetData(),filename.GetLength()+1);
		
		dwFileAttributes=entry.iAtt;           //��д�ļ�����
		
		nFileSizeLow=entry.iSize;              //��д�ļ���С
		
		TInt64 tempTime=entry.iModified.Int64();       //��д�ļ�ʱ��
		ftLastWriteTime.dwHighDateTime=(tempTime>>32);
		ftLastWriteTime.dwLowDateTime=tempTime;
		ftCreationTime=ftLastWriteTime;
		ftLastAccessTime=ftLastWriteTime;
	return XTRUE;
#elif defined(_WIN32)	
	if(m_hHandle==XNULL) return XFALSE;
//	XBOOL bOK=::FindNextFile(m_hHandle,(WIN32_FIND_DATAA*)this);
	XBOOL bOK=::FindNextFileA(m_hHandle,(WIN32_FIND_DATAA*)this);
	if(m_hHandle==INVALID_HANDLE_VALUE) 
		m_hHandle=XNULL;
	return bOK;
#endif
}

XFileFinder::XFileFinder()
	{
#ifdef WINDOWS
	m_hHandle=0;
#endif
#ifdef _SYMBIAN
	m_hHandle=0;
	m_hFileContain = new ZCFile();
	m_nFlag=0;
	m_hDir=XNULL;
#endif
	}

XFileFinder::~XFileFinder()
{
	Close();
#ifdef _SYMBIAN
	delete (ZCFile*)m_hFileContain;
	delete m_hDir;
#endif	
}

void XFileFinder::Close()
{
#ifdef _SYMBIAN
   delete m_hDir;
   m_nFlag=0;
   m_hDir=XNULL;
#elif defined(_WIN32)	
   if(m_hHandle!=XNULL&&m_hHandle!=INVALID_HANDLE_VALUE)
		::FindClose(m_hHandle);
	m_hHandle=XNULL;
#endif
}




int XFileFinder::GetType(XString8 &strFile)
{
	XString8 str=strFile;
	int l=str.GetLength()-1;
	if(strFile[1]!=':') return XNONE;
	if(l<=2) return XFOLDER;
	switch(strFile[l])
	{
	case ':':
		 if(l==1)
			return XFOLDER;
		 else
			return XNONE;
		 break;
	case XSP:
	case XDIV:
		 str[l]=0;
		 break;
	}	
	int nType=0;
	XFileFinder find;
	if(!find.FindFirst(str))
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
#ifdef _SYMBIAN

	char dick[]="C:\\";
	XString8 strName="(C:)";
	TDriveList drivelist;  //���������б�����
	TChar driveLetter;     //�����̷�
	TInt driveNumber=EDriveA;    //���������б������
	ZCFile fsession;
	fsession.ZCGetDriverList(drivelist);
	for(;driveNumber<=EDriveZ;driveNumber++)
	{
		if(drivelist[driveNumber])
		{
			XString8 str;
			drivers.Add(dick);
			switch(driveNumber){
			case EDriveC:
				str=XResource::LoadWString(XIDS_SYSDISK);break;
					//"ϵͳ��"; break;
			case EDriveD:
				str=XResource::LoadWString(XIDS_VIRDISK);break;
					//"������"; break;
			case EDriveZ:
				str=XResource::LoadWString(XIDS_BAKDISK);break;
				//str="������"; break;
			default:
				str=XResource::LoadWString(XIDS_REMOVEBLEDISK);break;
				//str="���ƶ���"; break;
			}
			strName[1]=dick[0];
			str+=strName;
			names.Add(str);
		}
		dick[0]++;
	}
#elif defined(_WIN32)
	int drives=GetLogicalDrives();
	int nums=0;	
	drives>>=2;
	XTCHAR disk[]="C:\\";
	XString8 strName="(C:)";
	while(drives)
	{
		if(drives&0x01)
		{						
//			UINT type=GetDriveType(disk);
			UINT type=GetDriveTypeA(disk);
			XTCHAR buf[64];
			DWORD s;
//			::GetVolumeInformation(disk,buf,64,&s,&s,&s,XNULL,0);
			::GetVolumeInformationA(disk,buf,64,&s,&s,&s,XNULL,0);
			if(type!=DRIVE_CDROM)
			{
				drivers.Add(disk);
				XString8 str;
				if((type==DRIVE_REMOVABLE)&&buf[0]==0)
					str="���ƶ���";	
				else str=buf;
				strName[1]=disk[0];
				str+=strName;
				names.Add(str);
			}
		}
		disk[0]++;
		drives>>=1;
	}
#endif
}

XBOOL XFile::CreateFolder(XPCTSTR strPath)
{
#ifdef _SYMBIAN
	TText16* testName;
	testName= ZCConvert::ConvertToUnicode((TText8*)strPath);
	RFs fsession;
//	User::LeaveIfError(fsession.Connect());
	for(unsigned short* s=testName;*s;s++)
		if(*s=='/') *s='\\';
	TFileName fname;
	fname.Copy(testName);
	delete testName;
	fname.Append(_L("\\a"));
	fsession.Connect();
		XBOOL bOK;
		bOK=fsession.MkDirAll(fname);
	fsession.Close();
	return bOK;
#elif defined(_WIN32)
	return ::CreateDirectoryA(strPath,NULL);
#endif
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
#ifdef _SYMBIAN
	TText16* testName;
	testName= ZCConvert::ConvertToUnicode((TText8*)strFile);
	RFs fsession;
//	User::LeaveIfError(fsession.Connect());
	fsession.Connect();
		TPtrC filename(testName);
		XBOOL bOK;
		bOK=fsession.RmDir(filename);
	fsession.Close();
	delete testName;
#else
	XBOOL bOK=::RemoveDirectoryA(strFile);
#endif
	//int v=GetLastError();
	return bOK;
}

XBOOL XFile::LoadText(XPCTSTR strFile, XString8 &strTxt)
{
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



void XFileFinder::GetFileName(XString8 &strPath, XString8 &strName,XBOOL bExt)
{
	int id=strPath.ReverseFind('\\');
	if(id<0) id=strPath.ReverseFind('/');
	int idd=bExt?strPath.ReverseFind('&'):strPath.ReverseFind('.');
	if(idd<0) idd=strPath.GetLength();	
	if(id>=0)
		strPath.Mid(strName,id+1,idd-id-1);		
	else
		strPath.Left(strName,idd);
	
}
