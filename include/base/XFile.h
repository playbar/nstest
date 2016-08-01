// XFile.h: interface for the XFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFILE_H__239D4EFB_4AEC_4D15_8374_5FFF8852DAC6__INCLUDED_)
#define AFX_XFILE_H__239D4EFB_4AEC_4D15_8374_5FFF8852DAC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GGType.h"
#include "XTime.h"
#include "XString.h"
#ifdef _SYMBIAN
	#include <f32file.h>
	#include <stdio.h>
#endif
#ifdef __APPLE__
#include <stdio.h>
#endif
#define XMAX_PATH 260
#define XDIRECTORY            0x00000010  

class XFileFinder 
{
public:
	enum {NONE,FILE,DIR};
    XU32 dwFileAttributes;
    XFILETIME ftCreationTime;
    XFILETIME ftLastAccessTime;
    XFILETIME ftLastWriteTime;
    XU32 nFileSizeHigh;
    XU32 nFileSizeLow;
    XU32 dwReserved0;
    XU32 dwReserved1;
    XTCHAR  cFileName[ XMAX_PATH ];
    XTCHAR  cAlternateFileName[ 14 ];
    XU32 dwFileType;
    XU32 dwCreatorType;
    XU16  wFinderFlags;
public:
	static void GetFileName(XString8&strFile,XString8&strName,XBOOL bExt=XFALSE);
	enum {XNONE=0,XFILE=1,XFOLDER=2};
	static int GetType(XString8&strFile);
	XBOOL GetFileTime(XPCTSTR strFile,XTime&time,XU8 ts=XTM_CREATE);
	enum {XTM_CREATE,XTM_ACCESS,XTM_MODIFY};
	
	static void GetDriverList(XString8Array&drivers,XString8Array&names);
	XBOOL IsDirectory();
	void Close();
	XBOOL FindNext();
	XBOOL FindFirst(XPCTSTR strPath);

#ifdef __APPLE__
	XBOOL FindPath(XPCTSTR strPath);
#endif

#ifdef __ANDROID__
	XBOOL FindPath(XPCTSTR strPath);
#endif

#ifdef _SYMBIAN
	XFileFinder();
#else
	XFileFinder()
	{
		m_hHandle=0;
	}
#endif
	~XFileFinder();

	XBOOL IsValid(){return m_hHandle!=XNULL;}

protected:
	XHANDLE m_hHandle;
#ifdef _SYMBIAN
	XHANDLE m_hDir;                  //�ļ��б�
	XINT    m_nFlag;                 
	XHANDLE m_hFileContain;          //�ļ��������Ự
#endif	
	
#ifdef __APPLE__
	XVector<char*> m_tempFilelist;
	int m_tempFileCursor;
	XString8 m_parentPath; // only valid if m_tempFilelist not empty.
	bool m_isDirectory;
	void clearTempFileList();
#endif

#ifdef __ANDROID__
	XVector<char*> m_tempFilelist;
	int m_tempFileCursor;
	int num_entries;	// added by zhengjl 2011.07
	XString8 m_parentPath; // only valid if m_tempFilelist not empty.
	bool m_isDirectory;
	void clearTempFileList();
#endif
	
};

class _XEXT_CLASS XFile  
{
public:
	enum {XREAD=0x01,
		  XWRITE=0x02,
		  XAPPEND=0x04,
		  XCREATE=0x10,
		  XBINARY=0x08};
	enum {XSEEK_SET,XSEEK_END,XSEEK_CUR};
	
public:
	
	static XBOOL LoadText(XPCTSTR strFile,XString8&strTxt);
	static XBOOL RemoveFolder(XPCTSTR strFile);
	int ReadString(XString8&str);
	int WriteString(XString8&str);
	static XBOOL CreateFolder(XPCTSTR strPath);
	//XBOOL GetFileTime(XPCTSTR strFile,XTime&time,XU8=XTM_CREATE);
	static XBOOL RemoveFile(XPCTSTR strFile);
	int Read(void*pData,int nSize);
	int Write(const void*pData,int nSize);
	int GetLength();
	int Tell();
	XBOOL Seek(int nSize,XU8 nType);
#ifdef __APPLE__
    XBOOL Open(XPCTSTR strFile,XU8 nStyle, XBOOL isResource);
#endif
    XBOOL Open(XPCTSTR strFile,XU8 nStyle);

	void Close();
	XFile();
	virtual ~XFile();
	XBOOL IsValid(){return m_file!=XNULL;}

	FILE* getFileHandle()
	{
		return m_file;
	}
	
protected:
	FILE* m_file;
};

/////////////////////////////////////////////////////////////////
// MD5��������㷨ʵ���࣬������MD5�����
#define BUF_LENGTH 64
class _XEXT_CLASS MD5  
{
	/////////////////////////////////////////////////////////////////
	// MD5��������㷨ʵ���࣬������MD5�����
	class MD5State
	{	
	public:
		int state[4];
		int count[2];
		char buffer[BUF_LENGTH];
		MD5State()
		{   state[0] = 0x67452301;
			state[1] = 0xefcdab89;
			state[2] = 0x98badcfe;
			state[3] = 0x10325476;
			count[0] = count[1] = 0;
			memset(buffer,0,BUF_LENGTH);}
		MD5State(const MD5State& from)
		{   
			int i;
			for(i = 0; i < BUF_LENGTH; i++)
			   buffer[i] = from.buffer[i];
			for( i = 0; i < 4; i++)
				state[i] = from.state[i];
			for( i = 0; i < 2; i++)
				count[i] = from.count[i];	}    
	};
public:
	// ����û����룬�û��������MD5��������
	// strUser:�û����
	// strPasswd:�û�����
	// passwd:��ݼ�����
	void MakeMD5(char*buf,int nLen,char* passwd)
	{
	    Update(buf, nLen,nLen);
		Final(passwd);
		//memcpy(passwd,md5out,16);
	}
 
    MD5()
    {  
		memset(padding,0,64);
		padding[0]=(char)-128;
		fin=0;
    }    
private:
	int AbsRight(int a,int b)
	{
		if(a>0) return a>>b;
		else return ((XU32)a)>>b;
	}
	int rotate_left(int x, int n)
    {
        return (x << n )| (AbsRight(x,(32 - n)));
    }
    int uadd(int a, int b)
    {
        long aa = (long)a & 0xffffffffL;
        long bb = (long)b & 0xffffffffL;
        aa += bb;
        return (int)(aa & 0xffffffffL);
    }
    int uadd(int a, int b, int c)
    {
        return uadd(uadd(a, b), c);
    }
    int uadd(int a, int b, int c, int d)
    {
        return uadd(uadd(a, b, c), d);
    }
    int FF(int a, int b, int c, int d, int x, int s, int ac)
    {
        a = uadd(a, b & c | ~b & d, x, ac);
        return uadd(rotate_left(a, s), b);
    }
    int GG(int a, int b, int c, int d, int x, int s, int ac)
    {
        a = uadd(a, b & d | c & ~d, x, ac);
        return uadd(rotate_left(a, s), b);
    }
    int HH(int a, int b, int c, int d, int x, int s, int ac)
    {
        a = uadd(a, b ^ c ^ d, x, ac);
        return uadd(rotate_left(a, s), b);
    }
    int II(int a, int b, int c, int d, int x, int s, int ac)
    {
        a = uadd(a, c ^ (b | ~d), x, ac);
        return uadd(rotate_left(a, s), b);
    }
    void Decode(char buffer[], int len, int shift,int out[16])
    {        
        int j;
        int i = j = 0;
        for(; j < len; j += 4)
        {
            out[i] = buffer[j + shift] & 0xff | (buffer[j + 1 + shift] & 0xff) << 8 | (buffer[j + 2 + shift] & 0xff) << 16 | (buffer[j + 3 + shift] & 0xff) << 24;
            i++;
        }     
    }
    void Transform(MD5State& stat, char buffer[], int shift);
	void Encode(int input[], int len,char*out);
private:
	void Update(MD5State& stat, char buffer[], int tLen,int offset, int length);

    void Update(char buffer[], int tLen,int offset, int length)
    {
        Update(state, buffer, tLen,offset, length);
    }

    void Update(char buffer[], int tLen,int length)
    {
        Update(state, buffer, tLen,0, length);
    }

    void Update(char buffer[],int tLen)
    {
        Update(buffer, tLen,0, tLen);
    }

    void Update(char b)
    {        
        Update(&b, 1);
    }

    void Update(XString8& s)
    {        
        Update((char*)(XPCTSTR)s, s.GetLength());
    }
	void Final(char out[16]);

    void asHex(char hash[],int nLen,XString8&buf)
    {        		
        for(int i = 0; i < nLen; i++)
        {
			buf+=((hash[i]>>4)&0xf)+'0';
			buf+=((hash[i])&0xf)+'0';
        }        
    }

    void asHex(XString8&buf)
    {
		char out[16];
		Final(out);
        asHex(out,16,buf);
    }

    MD5State state;
	char     fin;
    char     padding[64];

};

#endif // !defined(AFX_XFILE_H__239D4EFB_4AEC_4D15_8374_5FFF8852DAC6__INCLUDED_)
