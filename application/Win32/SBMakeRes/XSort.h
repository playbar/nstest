// XSort.h: interface for the XSortObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSORT_H__1632CEBF_FD63_4F7D_A430_24F59765B7B5__INCLUDED_)
#define AFX_XSORT_H__1632CEBF_FD63_4F7D_A430_24F59765B7B5__INCLUDED_

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
#ifdef __APPLE__
		int r=(_ET)((int)iSrt)-(_ET)((int)iDst);
#else
		int r=(_ET)iSrt-(_ET)iDst;
#endif
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
	XBOOL Add(XPCTSTR buf)
	{
		int mi,id;
		id=Index((void*)buf,mi,XSS_NOCASE);
		if(id>=0) return XFALSE;
		m_pStr->InsertAt(mi,buf);
		return XTRUE;
	}
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

#endif // !defined(AFX_XSORT_H__1632CEBF_FD63_4F7D_A430_24F59765B7B5__INCLUDED_)
