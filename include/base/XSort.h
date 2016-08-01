// XSort.h: interface for the XSortObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSORT_H__1632CEBF_FD63_4F7D_A430_24F59765B7B5__INCLUDED_)
#define AFX_XSORT_H__1632CEBF_FD63_4F7D_A430_24F59765B7B5__INCLUDED_

#include "GGType.h"	// Added by ClassView
#include "XString.h"
#ifdef _SYMBIAN
#include "string_comp.h"
#endif
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum {XSS_NORMAL,XSS_FIRST,XSS_NOCASE};
class _XEXT_CLASS XSort
{
public:
	virtual ~XSort(){};
public:
		XINT Index(void* iSrt,XU8 nArg=XSS_NORMAL)
		{
			XINT mi;
			return Index(iSrt,mi,nArg);
		}
		XINT Index(void* iSrt,XINT&iMatch,XU8 nArg=XSS_NORMAL);

protected:
		static XINT CompareInt(int iSrt,int iDst)
		{
			if(iSrt>iDst) return 1;
			else if(iSrt<iDst) return -1;
			return 0;
		}
		static XINT CompareDouble(double iSrt,double iDst)
		{
			if(iSrt>iDst) return 1;
			else if(iSrt<iDst) return -1;
			return 0;
		}
		static XINT CompareFloat(float iSrt,float iDst)
		{
			if(iSrt>iDst) return 1;
			else if(iSrt<iDst) return -1;
			return 0;
		}
		static XINT CompareUInt(XU32 iSrt,XU32 iDst)
		{
			if(iSrt>iDst) return 1;
			else if(iSrt<iDst) return -1;
			return 0;
		}
		static XINT CompareString8(XPCTSTR str,XPCTSTR strValue,XU8 nMode);
		static XINT CompareString16(XPCWSTR str,XPCWSTR strValue,XU8 nMode);
protected:
		virtual XINT  Compare(void* iSrt,void* iDst,XU8 nArg)=0;
		virtual XU32  GetCount()=0;
		virtual void* GetData(XU32 iData)=0;
};


/*class _XEXT_CLASS XSortObj  
{
public:
	
public:
	XINT Index(XPCTSTR strKey,XINT&nMatch,XU8 nMode=XSS_NOCASE);
	XINT IndexW(XPCWSTR strKey,XINT&nMatch,XU8 nMode=XSS_NOCASE);
	XINT Index(XPCTSTR strKey,XU8 nMode=XSS_NOCASE)
	{
		int mi=0;
		return Index(strKey,mi,nMode);
	}
	XINT IndexF(float v,XINT&nMatch);
	static XINT Compare(XPCTSTR str,XPCTSTR strValue,XU8 nMode)
	{
		XString8 strKey(str,STRING_ATTACH);
		XINT rs=0;
		if(strValue==XNULL) rs=1;
		else
		{
			switch(nMode)
			{
			case XSS_FIRST:rs=strKey[0]-strValue[0];break;
			case XSS_NOCASE:rs=strKey.Compare(strValue,XTRUE);break;
			default:
			case XSS_NORMAL:rs=strKey.Compare(strValue,XFALSE);
			}
		}
		strKey.EmptyNoFree();
		return rs;
	}
	static XINT CompareW(XPCWSTR str,XPCWSTR strValue,XU8 nMode)
	{
		XString16 strKey(str,STRING_ATTACH);
		XINT rs=0;
		if(strValue==XNULL) rs=1;
		else
		{
			switch(nMode)
			{
			case XSS_FIRST:rs=str[0]-strValue[0];break;
			case XSS_NOCASE:rs=strKey.Compare(strValue,XTRUE);break;
			default:
			case XSS_NORMAL:rs=strKey.Compare(strValue,XFALSE);
			}
		}
		strKey.EmptyNoFree();
		return rs;
	}
	XPCTSTR operator[](XU32 nID){return GetData(nID);}
	XINT Index(XINT nData,XINT&nMatch);
	XINT Index(XINT nData)
	{
		int mi=0;
		return Index(nData,mi);
	}	  
	XINT IndexF(float fData)
	{
	    int mi=0;
		return IndexF(fData,mi);
	}
	XINT IndexW(XPCWSTR str)
	{
		int mi=0;
		return IndexW(str,mi);
	}
protected:
	XSortObj();
	virtual XPCTSTR GetData(XU32 nID){return XNULL;};
	virtual XINT	GetIData(XU32 nID){return 0;};
	virtual XU32	GetCount(){return 0;};
	virtual float   GetFData(XU32 nID){return 0;}
	virtual XPCWSTR GetWData(XU32 nID){return XNULL;}
	//XU32 m_nCount;
};*/

class _XEXT_CLASS XSortConstStr:public XSort
{
public:
	XSortConstStr(XPCTSTR pData[],XU32 nCount)
	{
		m_nCount=nCount;
		m_pData=pData;
	}
	static XINT Find(XPCTSTR strKey,XPCTSTR pData[],XU32 nCount,XU8 nMode=XSS_NOCASE)
	{
		XSortConstStr sort(pData,nCount);
		int mi=0;
		return sort.Index((void*)strKey,mi,nMode);
	}
	XU32 GetCount(){return m_nCount;}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}
	void* GetData(XU32 nID)
	{
		if(nID>=GetCount()) return XNULL;
		return (void*)m_pData[nID];
	}
	XU32 m_nCount;
	XPCTSTR* m_pData;
};

class _XEXT_CLASS XSortConstStr16:public XSort
{
public:	
	XSortConstStr16(XPCWSTR pData[],XU32 nCount)
	{
		m_nCount=nCount;
		m_pData=pData;
	}
	static XINT Find(XPCWSTR strKey,XPCWSTR pData[],XU32 nCount,XU8 nMode=XSS_NOCASE)
	{
		XSortConstStr16 sort(pData,nCount);
		int mi=0;
		return sort.Index((void*)strKey,mi,nMode);
	}
	XU32 GetCount(){return m_nCount;}
protected:
	XINT Compare(void*pSrt,void*pDst,XU8 nArg)
	{	return CompareString16((XPCWSTR)pSrt,(XPCWSTR)pDst,nArg);	}
	void* GetData(XU32 nID)
	{
		if(nID>=GetCount()) return XNULL;
		return (void*)m_pData[nID];
	}
	XU32 m_nCount;
	XPCWSTR* m_pData;
};


class _XEXT_CLASS XSortConstInt:public XSort
{
public:
	XSortConstInt(XINT pData[],XU32 nCount)
	{
		m_nCount=nCount;
		m_pData=pData;
	}
	void* GetData(XU32 nID)
	{
		if(nID>=m_nCount) return 0;
		return (void*)m_pData[nID];
	}
	XU32 GetCount(){return m_nCount;}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8)
	{	return CompareInt((int)iSrt,(int)iDst);	}
	XINT* m_pData;
	XU32  m_nCount;
};

class _XEXT_CLASS XSortConstU16:public XSort
{
public:
	XSortConstU16(XU16 pData[],XU32 nCount)
	{
		m_nCount=nCount;
		m_pData=pData;
	}
	void* GetData(XU32 nID)
	{
		if(nID>=m_nCount) return 0;
		return (void*)m_pData[nID];
	}
	XU32 GetCount(){return m_nCount;}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8)
	{	return CompareInt((int)iSrt,(int)iDst);	}
	XU16* m_pData;
	XU32  m_nCount;
};

class _XEXT_CLASS XSortConstU8:public XSort
{
public:
	XSortConstU8(XU8 pData[],XU32 nCount)
	{
		m_nCount=nCount;
		m_pData=pData;
	}
	void* GetData(XU32 nID)
	{
		if(nID>=m_nCount) return 0;
		return (void*)m_pData[nID];
	}
	XU32 GetCount(){return m_nCount;}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8)
	{	return CompareInt((int)iSrt,(int)iDst);	}
	XU8* m_pData;
	XU32  m_nCount;
};

class _XEXT_CLASS XSortConstFloat:public XSort
{
public:
	XSortConstFloat(float pData[],XU32 nCount)
	{
		m_nCount=nCount;
		m_pData=pData;
	}
	void* GetData(XU32 nID)
	{
		if(nID>=m_nCount) return 0;
		return (void*)&m_pData[nID];
	}
	XU32 GetCount(){return m_nCount;}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8)
	{	return CompareFloat(*(float*)iSrt,*(float*)iDst);	}
	float* m_pData;
	XU32  m_nCount;
};


template<class _XT,class _ET> class _XEXT_CLASS XSortArray:public XSort
{
public:
	XBOOL Add(_ET data,XU8 bOnly)
	{
		int mi;
		int id=Index((void*)data,mi);
		if(id>=0&&bOnly) return XFALSE;

		m_pData->InsertAt(mi,data);
		return XTRUE;

	}
	XINT Find(_ET data)
	{
		int mi;
		return Index((void*)data,mi);
	}
	XBOOL Remove(_ET data)
	{
		int mi;
		int id=Index((void*)data,mi);
		if(id>=0)
		{
			m_pData->RemoveAt(id);
			return XTRUE;
		}
		return XFALSE;
	}
	XSortArray(_XT data)
	{
		m_pData=data;
	}
	void* GetData(XU32 nID)
	{
		if(nID>=m_pData->GetSize()) return 0;
		return (void*)(*m_pData)[nID];
	}
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		int r=(_ET)((int)iSrt)-(_ET)((int)iDst);
		if(r>0) return 1;
		else if(r<0) return -1;
		return 0;
	}
	XU32 GetCount(){return m_pData->GetSize();}
protected:
	_XT m_pData;
};

class XSortTable:public XSort
{
public:
	virtual ~XSortTable(){}
	XINT ValueOf(XU32 iKey)
	{
		int id=Index((void*)iKey);
		if(id<0) return -1;
		return m_tables[(id<<1)+1];
	}
	void Add(XU32 iKey,XU32 iValue);
	void RemoveAll(){m_tables.RemoveAll();}
protected:
	void* GetData(XU32 id)
	{
		return (void*)m_tables[id>>1];
	}
	XINT Compare(void*iSrt,void*iDst,XU8)
	{
		return CompareUInt((XU32)iSrt,(XU32)iDst);
	}
	XU32 GetCount()
	{
		return m_tables.GetSize()>>1;
	}
	XU32Array m_tables;
};

typedef XSortArray<XU8Array*,XU8> XSortU8;
typedef XSortArray<XU16Array*,XU16> XSortU16;
typedef XSortArray<XU32Array*,XU32> XSortU32;

class _XEXT_CLASS XSortString8:public XSort
{
public:
	//XINT GetIData(XU32){return 0;}
	XSortString8(XString8Array&list,XU8 nGroup=1)
	{
		m_pStr=&list;
		m_nGroup=nGroup;		
	}
	XU32 GetCount(){return m_pStr->GetSize()/m_nGroup;}
	void* GetData(XU32 nID)
	{	if(nID>=m_pStr->GetSize()) return XNULL;
		return (void*)(*m_pStr)[nID*m_nGroup];
	}
	static XBOOL SSetValue(XString8Array&list,XPCTSTR strKey,XPCTSTR strValue);
	static XBOOL SSetValue(XString8Array&list,XPCTSTR strKey);
	static XBOOL ESetValue(XString8Array&list,XPCTSTR strKey,XPCTSTR strValue);
	static XBOOL ESetValue(XString8Array&list,XPCTSTR strValue);
protected:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}
	XString8Array* m_pStr;
	XU8	m_nGroup;
};
#ifdef _ANALY
struct _MEMINFO
{
	//XString8 strName;
	char*  strName;
	XU32	 nCount;
	XU32	 nTotalSize;
	int		 nCountInc;
	int		 nSizeInc;
	int		 nData[4];
};

class XSortMemName:public XSort
{
public:
	XSortMemName()
	{		
	}
	~XSortMemName()
	{
		Release();
	}
	void Release()
	{
		int i,nc=m_data.GetSize();
		for(i=0;i<nc;i++)
		{
			delete m_data[i]->strName;
			delete m_data[i];
		}
		m_data.RemoveAll();
	}
	void Add(XPCTSTR name,int nSize,int nCount=1)
	{
		if(name==NULL) return;
		int mi=0;
		int si=Index((void*)name,mi);
		_MEMINFO*pInfo=NULL;
		if(si<0)
		{
			pInfo = new _MEMINFO();
			int l = strlen(name)+1;
			pInfo->strName = new char[l];
			memcpy(pInfo->strName,name,l);
			//pInfo->strName = name;
			pInfo->nCount = nCount;
			pInfo->nTotalSize = nSize;
			pInfo->nCountInc = 0;
			pInfo->nSizeInc = 0;
			pInfo->nData[0] = 0;
			pInfo->nData[1] = 0;
			pInfo->nData[2] = 0;
			pInfo->nData[3] = 0;
			m_data.InsertAt(mi,pInfo);
		}
		else
		{
			pInfo = GetAt(si);
			pInfo->nCount+=nCount;
			pInfo->nTotalSize += nSize;
		}
	}

	XU32 GetCount(){return m_data.GetSize();}
	_MEMINFO* GetAt(XINT i)
	{
		return m_data[i];
	}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}
	void* GetData(XU32 nID)
	{
		if(nID>=GetCount()) return XNULL;
		return (void*)m_data[nID]->strName;
	}
	XVector<_MEMINFO*> m_data;
	
};

class XSortMemCount:public XSort
{
public:
	XSortMemCount()
	{		
	}
	~XSortMemCount()
	{		
	}
	
	void Add(_MEMINFO*pInfo)
	{
		int mi=0;
		int si=Index((void*)pInfo->nCount,mi);
		m_data.InsertAt(mi,pInfo);	
	}

	XU32 GetCount(){return m_data.GetSize();}
	_MEMINFO* GetAt(XINT i)
	{
		return m_data[i];
	}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		XU32 i1=(XU32)iSrt;
		XU32 i2=(XU32)iDst;
		if(i1>i2) return 1;
		else if(i1<i2) return -1;
		return 0;
		//return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}
	void* GetData(XU32 nID)
	{
		if(nID>=GetCount()) return XNULL;
		return (void*)m_data[nID]->nCount;
	}
	XVector<_MEMINFO*> m_data;
	
};

class XSortMemSize:public XSort
{
public:
	XSortMemSize()
	{		
	}
	~XSortMemSize()
	{		
	}
	
	void Add(_MEMINFO*pInfo)
	{
		int mi=0;
		int si=Index((void*)pInfo->nTotalSize,mi);
		m_data.InsertAt(mi,pInfo);	
	}

	XU32 GetCount(){return m_data.GetSize();}
	_MEMINFO* GetAt(XINT i)
	{
		return m_data[i];
	}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		XU32 i1=(XU32)iSrt;
		XU32 i2=(XU32)iDst;
		if(i1>i2) return 1;
		else if(i1<i2) return -1;
		return 0;
		//return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}
	void* GetData(XU32 nID)
	{
		if(nID>=GetCount()) return XNULL;
		return (void*)m_data[nID]->nTotalSize;
	}
	XVector<_MEMINFO*> m_data;
	
};


class XSortMemSizeInc:public XSort
{
public:
	XSortMemSizeInc()
	{		
	}
	~XSortMemSizeInc()
	{		
	}
	
	void Add(_MEMINFO*pInfo)
	{
		int mi=0;
		int si=Index((void*)pInfo->nSizeInc,mi);
		m_data.InsertAt(mi,pInfo);	
	}

	XU32 GetCount(){return m_data.GetSize();}
	_MEMINFO* GetAt(XINT i)
	{
		return m_data[i];
	}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		int i1=(int)iSrt;
		int i2=(int)iDst;
		if(i1>i2) return 1;
		else if(i1<i2) return -1;
		return 0;
		//return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}
	void* GetData(XU32 nID)
	{
		if(nID>=GetCount()) return XNULL;
		return (void*)m_data[nID]->nSizeInc;
	}
	XVector<_MEMINFO*> m_data;
	
};

class XSortMemCountInc:public XSort
{
public:
	XSortMemCountInc()
	{		
	}
	~XSortMemCountInc()
	{		
	}
	
	void Add(_MEMINFO*pInfo)
	{
		int mi=0;
		int si=Index((void*)pInfo->nCountInc,mi);
		m_data.InsertAt(mi,pInfo);	
	}

	XU32 GetCount(){return m_data.GetSize();}
	_MEMINFO* GetAt(XINT i)
	{
		return m_data[i];
	}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		int i1=(int)iSrt;
		int i2=(int)iDst;
		if(i1>i2) return 1;
		else if(i1<i2) return -1;
		return 0;
		//return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}
	void* GetData(XU32 nID)
	{
		if(nID>=GetCount()) return XNULL;
		return (void*)m_data[nID]->nCountInc;
	}
	XVector<_MEMINFO*> m_data;
	
};

class XSortMemData:public XSort
{
public:
	XSortMemData(int id)
	{		
		m_nID = id;
	}
	~XSortMemData()
	{		
	}
	
	void Add(_MEMINFO*pInfo)
	{
		int mi=0;
		int si=Index((void*)pInfo->nData[m_nID],mi);
		m_data.InsertAt(mi,pInfo);	
	}

	XU32 GetCount(){return m_data.GetSize();}
	_MEMINFO* GetAt(XINT i)
	{
		return m_data[i];
	}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		int i1=(int)iSrt;
		int i2=(int)iDst;
		if(i1>i2) return 1;
		else if(i1<i2) return -1;
		return 0;
		//return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
	}
	void* GetData(XU32 nID)
	{
		if(nID>=GetCount()) return XNULL;
		return (void*)m_data[nID]->nData[m_nID];
	}
	XVector<_MEMINFO*> m_data;
	int m_nID;
};

#define MAXSEEKSTACK 10240
#define MAXSAVES	 256
class XSeekItem
{
public:
	XSeekItem(char*name)
	{
		m_strSeekName=CopyString(name);
		memset(m_stacks,0,sizeof(m_stacks));
		memset(m_saves,0,sizeof(m_saves));
		m_nStack = 0;
		m_nSaves = 0;
	}
	~XSeekItem()
	{
		if(m_strSeekName)
			delete m_strSeekName;
		int i;
		for(i=0;i<m_nStack;i++)
			delete m_stacks[i];
		m_nStack = 0;
		for(i=0;i<m_nSaves;i++)
			delete m_saves[i];
		m_nSaves = 0;
	}

	bool Match(char* name)
	{
		if(!name) return false;
		if(strcmp(m_strSeekName,name)!=0) return false;
		//if(strcmp(name,m_strSeekName)!=0) return false;
		//Push();
		SaveStack();
		return true;
		//Pop();
	}

	void SaveStack()
	{
		if(m_nStack<=0) return;
		if(m_nSaves>=MAXSAVES) return;
		int i,l = 1;
		for(i=0;i<m_nStack;i++)
		{
			l+=strlen(m_stacks[i]);
			l++;
		}
		char* str=new char[l];
		char* dstr=str;
		for(i=0;i<m_nStack;i++)
		{
			l=strlen(m_stacks[i]);
			memcpy(dstr,m_stacks[i],l);
			dstr+=l;
			*dstr='-';
			dstr++;			
		}
		*dstr=0;
		for(i=0;i<m_nSaves;i++)
		{
			if(strcmp(m_saves[i],str)==0)
			{
				delete str;
				return;
			}
		}
		m_saves[m_nSaves] = str;
		m_nSaves++;
	}

	void Push(char* name)
	{
		if(m_nStack>=MAXSEEKSTACK) return;
		if(!name)
			name="GCObject";
		m_stacks[m_nStack]=CopyString(name);
		m_nStack++;
	}
	void Pop()
	{
		if(m_nStack<=0) return;
		m_nStack--;
		delete m_stacks[m_nStack];
		
	}

public:
	int	  m_nSaves;
	int	  m_nStack;
	char* m_strSeekName;
	char* m_stacks[MAXSEEKSTACK];
	char* m_saves[MAXSAVES];
public:
	char* CopyString(char*str)
	{
		int l = strlen(str)+1;
		char* newStr = new char[l];
		memcpy(newStr,str,l);
		return newStr;
	}
};

#endif
#endif // !defined(AFX_XSORT_H__1632CEBF_FD63_4F7D_A430_24F59765B7B5__INCLUDED_)
