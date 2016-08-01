#ifndef __XSTRING_H__
#define __XSTRING_H__

class XStream;
#ifdef _SYMBIAN
#include <string.h>
#include "string_comp.h"
#endif
#ifdef __APPLE__
	#include <string.h>
	#include "string_comp.h"
#endif

#ifdef __ANDROID__
	#include <string.h>
	#include "string_comp.h"
	#include <wchar.h>
#endif

#if 0
#define LOGWHERESTR()  LOGWHERESTR()
#else
#define LOGWHERESTR()
#endif

#include "XVector.h"
#include "GGType.h"	// Added by ClassView

#define XCRLF "\r\n"
#define XCR	   '\r'
#define XCT	   '\n'
#define XCTAB	'\t'
#define XDIVDIV "//"
#define XMMINUS "--"
#define XSPACE ' '
#define XQUEST '?'
#define XCOLON ':'
#define XSPLIT ';'
#define XEQU   '='
#define XPLUS  '+'
#define XMINUS '-'
#define XDIV   '/'
#define XSP	   '\\'
#define XSPSP  "\\"
#define XQO	   '"'
#define XSQO   '\''
#define XMULTI '*'
#define XLA	   '>'
#define XLE	   '<'
#define XAT	   '@'
#define XAND   '&'
#define XEX    '!'
#define XSIGN  '#'
#define XZERO  '0'
#define XPEC   '%'
#define XSTRUE  "true"
#define XSFALSE "false"
#define XON		"on"
#define XOFF	"off"
#define XFD		":/"
#define XID		"://"
#define XPT		'.'
#define XSPT	"."
#define XPTPT	".."
#define XDOLLAR '$'
#define XQOL	'('
#define XQOR	')'

enum {XCHARSET_BIG5=0,
	  XCHARSET_ISO_10646_UCS2=1,
      XCHARSET_ISO_8859_1,
	  XCHARSET_ISO_8859_2,
	  XCHARSET_ISO_8859_3,
	  XCHARSET_ISO_8859_4,
	  XCHARSET_ISO_8859_5,
	  XCHARSET_ISO_8859_6,
	  XCHARSET_ISO_8859_7,
	  XCHARSET_ISO_8859_8,
	  XCHARSET_ISO_8859_9,
	  XCHARSET_SHIFT_JIS,
	  XCHARSET_US_ASCII,
	  XCHARSET_UTF8,
	  XCHARSET_GB2312,
	  XCHARSET_GBK,
	  XCHARSET_GB2314
};

#define MAXFORMATSIZE 256
/////////////////////////////////////////////////////////
// 8位编码的字符串支持类
/*struct _XChunkBlock;

class XChunkAlloc {
private:
	int itemSize;
	int blockN;					// the number of items to allocate in a block
	long tag;
	XU16 blockId;				// the priority of the next block created
	BOOL noGarbageCollection;
	
	_XChunkBlock* firstBlock;		// The list of chunk blocks
	_XChunkBlock* lastBlock;

	_XChunkBlock* firstFree;		// The lowest priority block that has free items

	#ifdef DEBUG
	int maxUsed;
	int nBlocks;
	int nAlloc;
	#endif
	
private:
	_XChunkBlock* CreateChunk();
	void FreeChunk(_XChunkBlock* b);
	
public:
	XChunkAlloc(int size, int n = 64, BOOL suppressGC = false, long tg = 0);
	~XChunkAlloc();

	void FreeEmpties();	// deallocate any empty chunks
	
	void* Alloc();
	void Free(void*);
};

typedef XChunkAlloc ChunkAlloc;

inline void Spin() {}
#ifdef DEBUG
	#define Dbg( x )	x
#else
	#define Dbg( x )
#endif */
#define STRING_REF	  0
#define STRING_ATTACH -1
#define STRING_CONST  -2
#define STRING_EMPTY  -3
/*struct XSTRINGDATA
{
	int    nRefs;
	XU32   nLength,nMaxLength;	
	XPTSTR strData;
	XPTSTR data()           // TCHAR* to managed data
		{ return (XPTSTR)(this+1); }
};

const XU32 _XEmptyRef[]={STRING_EMPTY,0,0,0};

class _XEXT_CLASS XStringRef
{
protected:
	static XChunkAlloc alloc64;
	static XChunkAlloc alloc128;
	static XChunkAlloc alloc256;
	static XChunkAlloc alloc512;
	XPTSTR m_pData;
	inline XSTRINGDATA* GetData(){return ((XSTRINGDATA*)m_pData)-1;}
	inline void Release(XSTRINGDATA*pData)
	{
		switch(pData->nRefs)
		{
		case STRING_ATTACH:
			 delete pData->strData;
			 delete pData;
			 break;
		case STRING_CONST:
			 delete pData;
			 break;
		case STRING_EMPTY:
			 break;
		default:
			 if(pData->nRefs)
				 pData->nRefs--;
			 else
				 delete pData;
		}
	}
public:
	XStringRef()
	{ m_pData=(XPCTSTR)(_XEmptyRef+sizeof(XSTRINGDATA));}
	~XStringRef()
	{
	   Release();
	}
	inline void SetLength(XU32 l)
	{
		l=XMAX((l<<1)+1,32);
		XSTRINGDATA*pData=GetData();
		if(pData->nMaxLength>=l) return;
///////////////////////////////////////////////////////
// Alloc
		XSTRINGDATA*pNew=new
	}
}; */

struct XSTRINGDATA
{
	int    nRefs;
	XU32   nLength,nMaxLength;	
	//XPTSTR strData;
	XPTSTR data()           // TCHAR* to managed data
		{ return (XPTSTR)(this+1); }
};

class _XEXT_CLASS XString8:public XVector<XTCHAR>
{
public:
	static double ToFloat(XPCTSTR buf,int nSize)
	{
		LOGWHERESTR();
	 if(!buf||buf[0]==0) return 0;
	 if(buf[1]=='x'||
	   buf[2]=='X')
	   return (double)ToInt(buf,nSize,16,XFALSE);
	 return XATOF(buf);
	}
	static XINT ToInt(XPCTSTR buf,int nSize,int nDec,XBOOL bPec);
	static void FormatV(XPTSTR buffer,int v,XBOOL bHex);
	static void FormatV(XPTSTR buffer,float v);
	XString8(XPCTSTR pData,XINT nSize=0)
	{
		LOGWHERESTR();
	   SetString(pData,nSize);
	}
	XString8(XTCHAR c)
	{
		LOGWHERESTR();
		SetSize(2);
		m_pData[0]=c;
	}
	XString8(const XString8&src)
	{
		LOGWHERESTR();
		*this=src;
	}
	XString8() {Init();}
	~XString8() {}
	XString8(XPCWSTR pData)
	{	
		LOGWHERESTR();
		*this=pData;
	}
public:	
	void SetString(XPCTSTR pData,int nSize);	
	XString8 operator+(XPCTSTR pData)
	{
		LOGWHERESTR();
	   XString8 str=*this;
	   str+=pData;
	   return str;
	}
	XUINT GetLength()const
	{return m_nSize-1;}
	XBOOL IsEmpty()const
	{return GetLength()==0;}
	void operator=(const XString8&src)
	{
		LOGWHERESTR();
		SetSize(src.m_nSize);
		XCOPY(m_pData,src.m_pData,src.m_nSize);
	}
	void operator=(XPCTSTR pData)
	{
		LOGWHERESTR();
		if(pData==XNULL)
			Empty();
		else
		{
			XUINT nSize=SafeStrlen(pData)+1;
			SetSize(nSize);
			if(pData!=XNULL)
				XCOPY(m_pData,pData,nSize);
		}
	}
	void AddWChar(XWCHAR c);//,XU8 nCharset)
	XBOOL ConvertFrom(XPCTSTR pStr,XU8 nCharset,int len=0);

	void operator=(XPCWSTR pData);
	void operator +=(XTCHAR c)
	{
		LOGWHERESTR();
		m_pData[m_nSize-1]=c;
		SetSize(m_nSize+1);
	}
	void operator +=(XPCTSTR pData);
	void AddString(XPCTSTR pData,XUINT nSize=0);
	void Insert(XUINT nIndex,XPCTSTR pData)
	{
		InsertAt(nIndex,pData,SafeStrlen(pData));
	}
	void Insert(XUINT nIndex,XTCHAR c)
	{
		InsertAt(nIndex,c,1);
	}
	void operator+=(XString8&src)
	{
		*this+=src.m_pData;
	}
	void operator+=(XPCWSTR pData)
	{
		if(pData==XNULL) return;
		*this+=XString8(pData);
	}
	void operator+=(XINT v)
	{
		LOGWHERESTR();
		XString8 str;
		str.FromInt(v);
		*this+=str;
	}				 
	//static int Compare(XPCTSTR pSrt,XPCTSTR pDst,int nLen=0,XU8 bLowcase=0);
	//int Compare(XPCTSTR pStr,XBOOL bLowcase=XFALSE,int nLen=0);
	static int Compare(XPCTSTR pSrt,XPCTSTR pDst,int nLen=0,XU8 bLowcase=0)
	{
		LOGWHERESTR();
		if(pSrt==pDst) return 0;
		if(pSrt==XNULL)
		{
			//if(pDst==XNULL) return 0;
			return -1;
		}
		else if(pDst==XNULL) return 1;
		if(nLen>0)
			return bLowcase?strnicmp(pSrt,pDst,nLen):strncmp(pSrt,pDst,nLen);
		else return bLowcase?stricmp(pSrt,pDst):strcmp(pSrt,pDst);
	}
	int Compare(XPCTSTR pStr,XBOOL bLowcase=XFALSE,int nLen=0)
	{
		LOGWHERESTR();
		if(pStr==XNULL) return 1;
		if(nLen>0)
			return bLowcase?strnicmp(m_pData,pStr,nLen):strncmp(m_pData,pStr,nLen);
		else return bLowcase?stricmp(m_pData,pStr):strcmp(m_pData,pStr);
	}

	XBOOL operator>=(XPCTSTR pData)const
	{
		LOGWHERESTR();
		if(pData==XNULL) return XTRUE;
		return strcmp(m_pData,pData)>=0;
	}
	XBOOL operator >(XPCTSTR pData)const
	{
		LOGWHERESTR();
		if(pData==XNULL) return XTRUE;
		return strcmp(m_pData,pData)>0;
	}
	XINT operator-(XPCTSTR pData)const
	{
		LOGWHERESTR();
		return strcmp(m_pData,pData);
	}

	XBOOL operator<=(XPCTSTR pData)const
	{
		LOGWHERESTR();
		if(pData==XNULL) return XFALSE;
		return strcmp(m_pData,pData)<0;
	}
	XBOOL operator <(XPCTSTR pData)const
	{
		LOGWHERESTR();
		if(pData==XNULL) return XFALSE;
		return strcmp(m_pData,pData)<0;
	}

	XBOOL operator==(XPCTSTR pData)const
	{
		LOGWHERESTR();
		if(pData==XNULL) return IsEmpty();
		return strcmp(m_pData,pData)==0;
	}
	/*XBOOL operator==(XPTSTR pData)const
	{
		if(pData==XNULL) return IsEmpty();
		return strcmp(m_pData,pData)==0;
	}*/
	XBOOL operator==(const XString8&src)const
	{
		LOGWHERESTR();
		return strcmp(m_pData,src.m_pData)==0;
	}
	XBOOL operator!=(const XString8&src)const
	{
		LOGWHERESTR();
		return strcmp(m_pData,src.m_pData)!=0;
	}
	XBOOL operator!=(XPCTSTR pData)const
	{
		LOGWHERESTR();
		if(pData==XNULL) return !IsEmpty();
		return strcmp(m_pData,pData)!=0;
	}
	int ReverseFind(XTCHAR c)const
	{
		LOGWHERESTR();
	    XPCTSTR p=strrchr(m_pData,c);
		if(p==NULL) return -1;
		return p-m_pData;
	}

	int Find(XTCHAR c,XUINT nBegin=0);
	int Find(XPCTSTR pData,XUINT nBegin=0);
	
	void TrimLeft(char c=0);
	void TrimRight(char c=0);	
	void Delete(int id,int nCount=1);
	void Empty()
	{	Init();}
	enum {XLS_FULL,XLS_FIRST,XLS_NOCASE};
	XU8 IsNum();
	int IndexOf(XPCTSTR p[],XU32 nSize,int nDefault=-1,XU8 nType=XLS_NOCASE);
	void EmptyNoFree(){m_nSize=0;m_pData=XNULL;}
public:
	XBOOL ConvertFrom(XU8 nCharset);
	void ConvertToGB();
	void FromLogic(XBOOL v)
	{
		LOGWHERESTR();
		*this=v?XSTRUE:XSFALSE;
	}
	void FromInt(XINT v,XBOOL bHex=XFALSE);
	
	void FromFloat(XF64 val);

	//void FromFloat(float v);	

	double ToFloat();
	inline XBOOL ToLogic()
	{
		LOGWHERESTR();
		if(IsEmpty()) return XFALSE;
		return XTOLOWER(m_pData[0])==XSTRUE[0];
	}
	XINT ToInt(char nDec=10,XU8 bPec=XFALSE);

	void Left(XString8&str,XU32 nSize)const
	{
		LOGWHERESTR();
		if(nSize<=0) str.Empty();
		else str.SetString(m_pData,nSize);
	}
	void Mid(XString8&str,int nIndex,XU32 nSize=0) const;
	
	void Right(XString8&str,XU32 nSize);
	
	void MakeUpper()
	{
		LOGWHERESTR();
#ifdef _SYMBIAN
		int i,nc=(int)m_nSize-1;
		for(i=0;i<nc;i++)
			{
			if(m_pData[i]>='a' && m_pData[i]<='z')
				m_pData[i]='A'+(m_pData[i]-'a');
			}
#elif defined(_WIN32)
		int i,nc=(int)m_nSize-1;
		for(i=0;i<nc;i++)
			{
			if(m_pData[i]>='a' && m_pData[i]<='z')
				m_pData[i]='A'+(m_pData[i]-'a');
			}
		//strupr(m_pData);
#elif defined(__APPLE__)
		strupr(m_pData);
#elif defined( __ANDROID__ )
		strupr(m_pData);
#endif
	}
	void MakeLower()
	{
		LOGWHERESTR();
#ifdef _WIN32
		int i,nc=(int)m_nSize-1;
		for(i=0;i<nc;i++)
			{
			if(m_pData[i]>='A' && m_pData[i]<='Z')
				m_pData[i]='a'+(m_pData[i]-'A');
			}
		//strlwr(m_pData);
#elif defined(__APPLE__)
		strlwr(m_pData);
#elif defined( __ANDROID__ )
		strlwr(m_pData);
#endif
		//for(XUINT i=0;i<m_nSize-1;i++)
		//	m_pData[i]=XTOLOWER(m_pData[i]);
	}
	void MakeFirstUpper();
protected:
	void Init(){SetSize(1);m_pData[0]=0;}
public:
	void ReadFromStream(XStream*pStream,int nLen);
	void FromFloat(float v);
	inline static XUINT SafeStrlen(XPCTSTR pData)
	{
		LOGWHERESTR();
		if(pData==XNULL) return 0;
		return strlen(pData);
	}
};

//////////////////////////////////////////////////////////
// 16位编码的字符串支持类
class _XEXT_CLASS XString16:public XVector<XWCHAR>
{
public:	
	static int GBToUCS(const char*buf,XU16*dst,int nCount);
	static int UTF8ToUCS(const char*buf,XU16*dst,int nCount);
	void FromLogic(XBOOL v)
	{
		LOGWHERESTR();
		*this=v?XSTRUE:XSFALSE;
	}
	void FromFloat(XF64 v)
	{
		LOGWHERESTR();
		XString8 str;
		str.FromFloat(v);
		*this=str;
	}
	void FromFloat(float v)
	{
		LOGWHERESTR();
		XString8 str;
		str.FromFloat(v);
		*this=str;
	}
	void FromInt(XINT v,XBOOL bHex=XFALSE);
	inline XINT ToInt(char nDec=10,XU8 bPec=XFALSE);

	XBOOL ToLogic()
	{
		LOGWHERESTR();
		if(IsEmpty()) return XFALSE;
		return XTOLOWER(m_pData[0])==XSTRUE[0];
	}
	double ToFloat();
	XBOOL IsEmpty()const
	{return GetLength()==0;}
	void Left(XString16&str,XU32 nSize)const
	{
		LOGWHERESTR();
		if(nSize<=0) str.Empty();
		else str.SetString(m_pData,nSize);
	}
	void Mid(XString16&str,int nIndex,XU32 nSize=0);
	/*XString8 Mid(int nIndex,XU32 nSize)const
	{
		if(nSize<=0) return XString8("");
		return XString8(m_pData+nIndex,nSize);
	}*/
	void Right(XString16&str,XU32 nSize)const;
	int Find(XWCHAR c,XUINT nBegin=0);	
	int Find(XPCWSTR pData,XUINT nBegin=0);
	void Insert(XUINT nIndex,XPCWSTR pData)
	{
		LOGWHERESTR();
		InsertAt(nIndex,pData,SafeStrlen(pData));
	}
	void Insert(XUINT nIndex,XWCHAR c,int n=1)
	{
	    InsertAt(nIndex,c,n);
	}
	void Empty(){Init();}
	void EmptyNoFree(){m_nSize=0;m_pData=XNULL;}
	XString16(){Init();}
	XString16(XPCWSTR pData,XINT nSize=0)
	{
		SetString(pData,nSize);
	}
	XString16(XWCHAR c)
	{
		LOGWHERESTR();
		SetSize(2);
		m_pData[0]=c;
	}
	void SetString(XPCWSTR pData,XINT nSize=0);

	void MakeUpper()
	{
		LOGWHERESTR();
#ifdef _SYMBIAN
		for(XUINT i=0;i<m_nSize-1;i++)
			m_pData[i]=XTOUPPER(m_pData[i]);
#elif defined(_WIN32)		
		wcsupr(m_pData);
#elif defined(__APPLE__)
		wcsupr(m_pData);
#elif defined( __ANDROID__ )
		wcsupr(m_pData);
#endif
		//for(XUINT i=0;i<m_nSize-1;i++)
		//	m_pData[i]=XTOUPPER(m_pData[i]);
	}
	void MakeLower()
	{
		LOGWHERESTR();
#ifdef _WIN32
		wcslwr(m_pData);
#elif defined(__APPLE__)
		wcslwr(m_pData);
#elif defined( __ANDROID__ )
		wcslwr(m_pData);
#endif
        return;
	}
	void MakeFirstUpper();

	static int Compare(XPCWSTR pSrt,XPCWSTR pStr,XBOOL bLowcase=XFALSE,int nLen=0);
	int Compare(XPCWSTR pStr,XBOOL bLowcase=XFALSE,int nLen=0)const;

	XBOOL ConvertFrom(XPCTSTR pStr,XU8 nCharset);
	void operator=(XPCTSTR pData);
	XBOOL operator==(XPCWSTR pData)const
	{
		LOGWHERESTR();
		if(pData==XNULL)
            return m_nSize<=1;
#ifdef _WIN32
		return wcscmp(m_pData,pData)==0;
#elif defined(__APPLE__)
		return gg_wcscmp(m_pData, pData) == 0;
#elif defined (__ANDROID__ )
		return gg_wcscmp(m_pData, pData) == 0;
#endif
	}
	void operator=(XPCWSTR pData);
	void operator +=(XWCHAR c)
	{
		LOGWHERESTR();
		m_pData[m_nSize-1]=c;
		SetSize(m_nSize+1);
	}
	void operator +=(XPCWSTR pData);
	XString16(XPCTSTR pData)
	{
		*this=pData;
	}
	XU32 GetLength()const
	{return m_nSize-1;}
	void operator=(XString16&src)
	{
		LOGWHERESTR();
		SetSize(src.m_nSize,XFALSE);
		memcpy(m_pData,src.m_pData,m_nSize*sizeof(XWCHAR));
	}
	operator XPCWSTR(){return m_pData;}
protected:
	void Init(){SetSize(1);m_pData[0]=0;}
public:
	void FormatTxt(const XString16&txt,XWCHAR c='%')
	{
		LOGWHERESTR();
		int id=Find(c);
		if(id<0) return;
		RemoveAt(id,1);
		InsertAt(id,txt.GetData(),txt.GetLength());
	}
	XBOOL IsNum();
	static XUINT SafeStrlen(XPCWSTR pData)
	{
		LOGWHERESTR();
		if(pData==XNULL)
            return 0;
#ifdef _WIN32
		return wcslen(pData);
#elif defined(__APPLE__)
		return gg_wcslen(pData);
#elif defined( __ANDROID__ )
		return gg_wcslen( pData );
#else
		return 0;
#endif
	}
};



template <class _XT> class _XEXT_CLASS XStringArray
	{
	public:
		typedef XStringArray<_XT> MyString;
		~XStringArray(){RemoveAll();}
		XU32 GetSize()const {return m_list.GetSize();}
		_XT& operator[](XU32 nIndex)const
		{ return *m_list[nIndex];}
        
		int Find(XPCTSTR src,XU8 bNoCase=XFALSE)
		{
			for(XU32 i=0;i<m_list.GetSize();i++)
				if(m_list[i]->Compare(src,bNoCase)==0) return i;
			return -1;
		}
		void InsertAt(int id,XPCTSTR src)
		{
			_XT*pStr=new _XT(src);
			m_list.InsertAt(id,pStr);
		}
		void Attach(MyString&src)
		{
			m_list.Append(src.m_pData,this->m_nSize);
			src.m_list.RemoveAll();
		}
		void Add(XPCWSTR str)
		{
			_XT *pStr=new _XT(str);
			m_list.Add(pStr);
		}
		void Add(XPCTSTR str)
		{
			_XT *pStr=new _XT(str);
			m_list.Add(pStr);
		}
		void RemoveAll()
		{	m_list.FreeAll(); }
		void RemoveAt(XU32 nIndex,XU32 nCount=1)
		{
			for(XU32 i=0;i<nCount;i++)
			{ if(nIndex+i<m_list.GetSize())	delete m_list[nIndex+i];}
			m_list.RemoveAt(nIndex,nCount);
		}
		void Append(const MyString&src)
		{
			for(XU32 i=0;i<src.GetSize();i++)
			{
				_XT *p=new _XT(src[i]);
				m_list.Add(p);
			}
		}
		void Copy(const MyString&src)
		{
			RemoveAll();
		    Append(src);
		}
		int SplitString(XString8&str,char c=' ',XBOOL bTrim=XTRUE,int iBegin=0);
		int SplitStringS(XString8&str,char* cl=", ",XBOOL bTrim=XTRUE,int iBegin=0);
        
		void Init(){m_list.Init();}
	protected:
		XVector<_XT*> m_list;
	};

template<class _XT>
int XStringArray<_XT>::SplitString(XString8&str,char c,XBOOL bTrim,int iBegin)
	{
		XString8 strTmp;
		XBOOL bQuo=XFALSE;
		XU32 len=str.GetLength();
		XTCHAR*pData=str;
		for(XUINT id=iBegin;id<len;id++)
		{
			if(c!='%'&&pData[id]==XQO&&c!=XQO&&c!='*')
			{
				bQuo=!bQuo;
				if(!bQuo) 
				{
					Add(strTmp);
					strTmp.Empty();
				}
			}
			else if(bQuo)
				strTmp+=pData[id];
			else if(pData[id]==c)
			{
				if(!strTmp.IsEmpty())
				{
					Add(strTmp);
					strTmp.Empty();
				}
			}
			else 
			{   if(!bTrim||(XU8)pData[id]>XSPACE||!strTmp.IsEmpty())
					strTmp+=pData[id];
			}
		}
		if(!strTmp.IsEmpty())
			Add(strTmp);
		return GetSize();
	}
template<class _XT>
int XStringArray<_XT>::SplitStringS(XString8&str,char* c,XBOOL bTrim,int iBegin)
	{
		XString8 strTmp;
		XBOOL bQuo=XFALSE;
		XU32 len=str.GetLength();
		XTCHAR*pData=str;
		for(XUINT id=iBegin;id<len;id++)
		{
			if(pData[id]==XQO)
			{
				bQuo=!bQuo;
				if(!bQuo) 
				{
					Add(strTmp);
					strTmp.Empty();
				}
			}
			else if(bQuo)
				strTmp+=pData[id];
			else if(::strchr(c,pData[id]))
			{
				if(!strTmp.IsEmpty())
				{
					Add(strTmp);
					strTmp.Empty();
				}
			}
			else 
			{   if(!bTrim||(XU8)pData[id]>XSPACE||!strTmp.IsEmpty())
					strTmp+=pData[id];
			}
		}
		if(!strTmp.IsEmpty())
			Add(strTmp);
		return GetSize();
	}
typedef XStringArray<XString8>XString8Array;
typedef XStringArray<XString16>XString16Array;

#endif // !defined(AFX_XSTRING_H__68FE755C_12A8_4DEB_A5E0_B452F7A9B85E__INCLUDED_)
