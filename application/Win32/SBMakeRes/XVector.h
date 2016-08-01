// XVector.h: interface for the XVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XVECTOR_H__0829E61E_AF2C_494D_A614_B4D3B1984A27__INCLUDED_)
#define AFX_XVECTOR_H__0829E61E_AF2C_494D_A614_B4D3B1984A27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <ctype.h>

#include "GGType.h"
#include <string.h>
#define XCOPY memcpy		 //ƒ⁄¥ÊøΩ±¥∫Ø ˝
#define XSET  memset		 //ƒ⁄¥Ê…Ë÷√∫Ø ˝
#define XMOVE memmove		 //ƒ⁄¥Ê“∆∂Ø∫Ø ˝
#define XCMP  memcmp		 //ƒ⁄¥Ê±»Ωœ∫Ø ˝
#define XSTRLEN strlen		 //8Œª◊÷∑˚¥Æ≥§∂»º∆À„∫Ø ˝
#define XISALPHA isalpha	 //ºÏ≤È“ª∏ˆ8Œª◊÷∑˚¥Æ «∑Ò◊÷ƒ∏
#define FREEBLOCK 256
#define XATOF	atof		 //◊™ªªµΩ∏°µ„ ˝◊÷(8Œª)
#define XSPRINTF sprintf	 //∏Ò ΩªØ ‰≥ˆ
#ifdef __APPLE__
#define XWCSLEN gg_wcslen
#else
#define XWCSLEN	wcslen		 //16位字符串长度计算函数
#endif
#define XSTRCMP strcmp		 //8Œª◊÷∑˚¥Æ±»Ωœ
#define XTOLOWER tolower	 //◊™ªªµΩ–°–¥(8Œª)
#define XTOUPPER toupper	 //◊™ªªµΩ¥Û–¥–¥(8Œª)
#define XSTRCHR strchr		 //≤È’“◊÷∑˚
#define XSTRSTR strstr		 //≤È’“¥Æ
//////////////////////////////////////////////////
//  ˝◊Èƒ£∞Â£¨÷ªø…“‘ø…”√”⁄ª˘±æ ˝æ›¿‡–Õ 


template <class _XT> class _XEXT_CLASS XVector
{
protected:
	_XT *m_pData;	 // ˝æ›µÿ÷∑
	XU32 m_nSize,    // ˝◊Èµƒ µº ≥§∂»
		 m_nMaxSize; // ˝◊È◊Ó¥Ûƒ⁄¥Ê≥§∂»
public:
	typedef XVector<_XT> MyVector;
	XVector()
	{	//ƒ¨»œππ‘Ï∫Ø ˝
		m_nSize=m_nMaxSize=0;
		m_pData=XNULL;
	}
	XVector(XUINT nSize)
	{	//÷∏∂® ˝◊È≥§∂»
		m_nSize=m_nMaxSize=0;
		m_pData=XNULL;
		SetSize(nSize,XFALSE);
	}
	~XVector()
	{
		if(m_pData!=XNULL) delete[] m_pData;
		m_pData=XNULL;
	}
	XVector(const MyVector&src)
	{   //¥”¡ÌÕ‚“ª∏ˆ ˝◊Èππ‘Ï
		m_nSize=m_nMaxSize=0;
		m_pData=XNULL;
		Copy(src);
	}
public:
	XBOOL EnumAdd(const _XT&src);
	void EnumAdd(const _XT&key,const _XT&val);
	XBOOL operator==(const MyVector&src)
	{
		if(m_nSize!=src.m_nSize) return XFALSE;
		if(m_nSize>0)
			return XCMP(m_pData,src.m_pData,m_nSize)==0;
		return XTRUE;
	}
	void operator=(const MyVector&src)
	{	//∏¥÷∆
		Copy(src);
	}
	//ªÒµ√ ˝◊È≥§∂»
	XUINT GetSize() const
	{return m_nSize;}
	//ªÒµ√ ˝◊È∂‘”¶µƒƒ⁄¥Ê◊÷Ω⁄≥§∂»
	XUINT GetMenSize() const
	{return m_nSize*sizeof(_XT);}
//	_XT& operator[](XINT nIndex){return m_pData[nIndex];}
//	const _XT& operator[] (XINT nIndex) const {return m_pData[nIndex];}
	//ªÒµ√÷∏∂®Œª÷√µƒ‘™Àÿ
	const _XT& GetAt(XINT nIndex)const
	{return m_pData[nIndex];}
	//ªÒµ√ ˝æ›µÿ÷∑
	_XT* GetData()const
	{return m_pData;}
	operator _XT*()const
	{return m_pData;}
	//////////////////////////////////////////////////////
	//…Ë÷√–¬µƒ ˝æ›≥§∂»
	//nNewSize(IN) –¬µƒ ˝◊È≥§∂»
	//bExtern(IN)  «∑Ò‘§¡Ùø’º‰
	void SetSize(XUINT nNewSize,XBOOL bExtern=XTRUE);
	//////////////////////////////////////////////////////
	// ÃÌº”“ª∏ˆ‘™ÀÿµΩ ˝◊ÈŒ≤≤ø
	// v(IN) “™ÃÌº”µƒ‘™Àÿ
	void Add(const _XT&v)
	{
		SetSize(m_nSize+1);
		m_pData[m_nSize-1]=v;

	}
	///////////////////////////////////////////////////////////
	// ÃÌº”¡ÌÕ‚“ª∏ˆ ˝◊Èƒ⁄»›µΩ±æ È◊ÈµƒŒ≤≤ø
	// src(IN) “™ÃÌº”µƒ ˝◊È
	XUINT Append(const MyVector& src);
	//////////////////////////////////////////////////////////////
	// Ω´¡ÌÕ‚“ª∏ˆ ˝◊Èµƒƒ⁄»›∏¥÷∆µΩ±æ ˝◊È
	// src(IN) “™∏¥÷∆ƒ⁄»›µƒ ˝◊È
	void Copy(const MyVector& src)
	{
		SetSize(src.m_nSize,XFALSE);
		XCOPY(m_pData, src.m_pData, src.m_nSize * sizeof(_XT));
	}
	
    /////////////////////////////////////////////////////////////
	//  ˝∑≈∂‡”‡µƒƒ⁄»›ø’º‰
	void FreeExtra();
	//////////////////////////////////////////////////////////////////
	// ÷ÿ∏¥≤Â»Î“ª∏ˆ‘™ÀÿµΩ÷∏∂®Œª÷√
	void InsertAt(XUINT nIndex,const _XT&newElement, XUINT nCount=1);
	_XT RemoveLast()
	{
		if(m_nSize>0)
			m_nSize--;
		return m_pData[m_nSize];
	}
	_XT GetFirst()
	{
		if(m_nSize<=0) return XNULL;
		return m_pData[0];
	}
	_XT GetLast()
	{
		if(m_nSize<=0) return XNULL;
		return m_pData[m_nSize-1];
	}
	////////////////////////////////////////////////////////////
	// …æ≥˝÷∏∂®Œª÷√µƒ∂‡∏ˆ‘™Àÿ
	// nIndex(IN) ø™ º…æ≥˝µƒŒª÷√
	// nCount(IN) “™…æ≥˝µƒ‘™Àÿ∏ˆ ˝
	void RemoveAt(XUINT nIndex, XUINT nCount=1);
	void FreeAll();
	//«Âø’»´≤ø ˝æ›
	void RemoveAll(XBOOL bClear=XTRUE)
	{
		m_nSize=0;
		if(bClear)
			FreeExtra();
	}
	////////////////////////////////////////////////////////////
	// Ω´¡ÌÕ‚“ª∏ˆ ˝◊Èµƒƒ⁄»›≤Â»ÎµΩ±æ ˝◊Èµƒ÷∏∂®Œª÷√
	// nIndex(IN) “™≤Â»ÎµƒŒª÷√
	// pNewArray(IN) “™≤Â»Îµƒ ˝◊È
	void InsertAt(XUINT nIndex, MyVector&pNewArray);
	//////////////////////////////////////////////////////////
	// ÃÌº”“ª∂Œ ˝æ›µΩ ˝◊ÈµƒŒ≤≤ø
	// pData(IN) “™ÃÌº”µƒ ˝æ›µÿ÷∑
	// nSize(IN) “™ÃÌº”µƒ ˝æ›≥§∂»
	XUINT Append(const _XT* pData,XUINT nSize);
	/////////////////////////////////////////////////////////////
	// ÃÌº”“ª∂Œ ˝æ›µΩ÷∏∂®µƒŒª÷√
	// nIndex(IN) “™ÃÌº”µƒŒª÷√
	// pData(IN) “™ÃÌº” ˝æ›µƒµÿ÷∑
	// nSize(IN) “™ÃÌº” ˝æ›≥§∂»
	XUINT InsertAt(XUINT nIndex,const _XT* pData,XUINT nSize);
/*	const _XT* DataTo() const
	{
		_XT* p=m_pData;
		m_pData=NULL;
		m_nSize=m_nMaxSize=0;
		return p;
	}*/
	_XT* DataTo()
	{
		_XT* p=m_pData;
		m_pData=XNULL;
		m_nSize=m_nMaxSize=0;
		return p;
	}
	void DataFrom(MyVector&src);
	void DataFrom(_XT* pData,XU32 nSize)
	{
		if(m_pData!=XNULL) delete m_pData;
		m_pData=pData;
		m_nMaxSize=m_nSize=nSize;
	}

protected:
	XUINT CalcSize(XUINT nSize,XBOOL bExtern);
};

template<class _XT>
XBOOL XVector<_XT>::EnumAdd(const _XT&src)
{
	for(XU32 i=0;i<m_nSize;i++)
		if(m_pData[i]==src) return XFALSE;
	Add(src);
	return XTRUE;
}
template<class _XT>
void XVector<_XT>::EnumAdd(const _XT&key,const _XT&val)
{
	for(XU32 i=0;i<m_nSize;i+=2)
	{
		if(m_pData[i]==key) 
		{
			m_pData[i+1]=val;
			return;
		}
	}
	Add(key);
	Add(val);
}

///////////////////////////////////////////////////////////
// ÃÌº”¡ÌÕ‚“ª∏ˆ ˝◊Èƒ⁄»›µΩ±æ È◊ÈµƒŒ≤≤ø
// src(IN) “™ÃÌº”µƒ ˝◊È
template<class _XT>
XUINT XVector<_XT>::Append(const MyVector& src)
{
	XUINT nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize,XFALSE);
	XCOPY(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(_XT));
	return nOldSize;
}

/////////////////////////////////////////////////////////////
//  ˝∑≈∂‡”‡µƒƒ⁄»›ø’º‰
template<class _XT>
void XVector<_XT>::FreeExtra()
{
	//return;
	if (m_nSize != m_nMaxSize)
	{	//¥Ê‘⁄∂‡”‡µƒø’º‰
		_XT* pNewData = XNULL;
		if (m_nSize != 0)
		{
			//…Í«Î–¬µƒø’º‰
			pNewData = new _XT[m_nSize];
			//∏¥÷∆‘≠¿¥µƒ ˝æ›
			XCOPY(pNewData, m_pData,m_nSize*sizeof(_XT));
		}
		//…Ë÷√–¬µƒ ˝æ›÷∏œÚ
		delete[] m_pData;
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

////////////////////////////////////////////////////////////
// …æ≥˝÷∏∂®Œª÷√µƒ∂‡∏ˆ‘™Àÿ
// nIndex(IN) ø™ º…æ≥˝µƒŒª÷√
// nCount(IN) “™…æ≥˝µƒ‘™Àÿ∏ˆ ˝
template<class _XT>
void XVector<_XT>::RemoveAt(XUINT nIndex, XUINT nCount)
{
	if(nIndex+nCount>=m_nSize)
		nCount=m_nSize-nIndex;
	if(nCount<=0) return;
	//º∆À„“™“∆∂Øµƒ≥§∂»
	XUINT nMoveCount = m_nSize - (nIndex + nCount);

	if (nMoveCount>0)	//“∆∂Ø ˝æ›
		XMOVE(&m_pData[nIndex], &m_pData[nIndex + nCount],
			nMoveCount * sizeof(_XT));
	//if(nIndex+nCount<m_nSize)
	XSET(m_pData+nIndex+nMoveCount,0,sizeof(_XT)*nCount);
	//for(XINT i=nIndex+nCount;i<m_nSize;i++)
	//	m_pData[i
	m_nSize -= nCount;
}
template<class _XT>
void XVector<_XT>::FreeAll()
{
	for(XU32 i=0;i<m_nSize;i++)
	{
		//if(i==169)
		//	int v=0;
		delete m_pData[i];
	}
	m_nSize=0;
	FreeExtra();
}

////////////////////////////////////////////////////////////
// Ω´¡ÌÕ‚“ª∏ˆ ˝◊Èµƒƒ⁄»›≤Â»ÎµΩ±æ ˝◊Èµƒ÷∏∂®Œª÷√
// nIndex(IN) “™≤Â»ÎµƒŒª÷√
// pNewArray(IN) “™≤Â»Îµƒ ˝◊È
template<class _XT>
void XVector<_XT>::InsertAt(XUINT nIndex, MyVector&pNewArray)
{
	if (pNewArray.GetSize() >0)
	{
		XUINT nOldSize=m_nSize;
		//…Ë÷√≥§∂»
		SetSize(m_nSize+pNewArray.m_nSize);
		//“∆∂Ø ˝æ›ƒ⁄»›
		XMOVE(&m_pData[nIndex+pNewArray.m_nSize],&m_pData[nIndex],
			  (nOldSize-nIndex)*sizeof(_XT));
		//∏¥÷∆ƒ⁄»›
		XCOPY(&m_pData[nIndex],pNewArray.m_pData,pNewArray.m_nSize*sizeof(_XT));
	}
}
//////////////////////////////////////////////////////////
// ÃÌº”“ª∂Œ ˝æ›µΩ ˝◊ÈµƒŒ≤≤ø
// pData(IN) “™ÃÌº”µƒ ˝æ›µÿ÷∑
// nSize(IN) “™ÃÌº”µƒ ˝æ›≥§∂»
template<class _XT>
XUINT XVector<_XT>::Append(const _XT* pData,XUINT nSize)
{
	if(pData==XNULL) return m_nSize;
	XUINT nOldSize = m_nSize;
	//…Ë÷√–¬µƒ≥§∂»
	SetSize(m_nSize + nSize,XFALSE);
	XCOPY(m_pData + nOldSize, pData, nSize * sizeof(_XT));
	return nOldSize;	
}
/////////////////////////////////////////////////////////////
// ÃÌº”“ª∂Œ ˝æ›µΩ÷∏∂®µƒŒª÷√
// nIndex(IN) “™ÃÌº”µƒŒª÷√
// pData(IN) “™ÃÌº” ˝æ›µƒµÿ÷∑
// nSize(IN) “™ÃÌº” ˝æ›≥§∂»
template<class _XT>
XUINT XVector<_XT>::InsertAt(XUINT nIndex,const _XT* pData,XUINT nSize)
{
	if(nIndex>=m_nSize) return Append(pData,nSize);
	int nOldSize=m_nSize;
	SetSize(m_nSize+nSize);
	XMOVE(&m_pData[nIndex+nSize],&m_pData[nIndex],
		  (nOldSize-nIndex)*sizeof(_XT));
	XCOPY(&m_pData[nIndex],pData,nSize*sizeof(_XT));
	return nOldSize;
}
template<class _XT>
void XVector<_XT>::DataFrom(MyVector&src)
{
	if(m_pData!=XNULL) delete m_pData;
	m_pData=src.m_pData;
	m_nMaxSize=src.m_nMaxSize;
	m_nSize=src.m_nSize;
	src.m_pData=XNULL;
	src.m_nSize=0;
	src.m_nMaxSize=0;
}


template<class _XT>
XUINT XVector<_XT>::CalcSize(XUINT nSize,XBOOL bExtern)
	{
		if(!bExtern) return nSize;
		if(nSize==0) return 0;
		else if(nSize<4) nSize=4;
		else if(nSize<64) nSize+=16;
		else if(nSize<256) nSize+=nSize>>1;
		else if(nSize<1024) nSize+=nSize>>2;
		else nSize+=nSize>>4;
		return nSize;
	}

template<class _XT>
void XVector<_XT>::InsertAt(XUINT nIndex,const _XT&newElement, XUINT nCount)
	{
		if(nCount<=0) return;
		if (nIndex >= m_nSize)
		{
			//≤Â»ÎµƒŒª÷√≥¨π˝ ˝◊È≥§∂»£¨Õÿ’π ˝◊È≥§∂»
			SetSize(nIndex + nCount);  // grow so nIndex is valid
		}
		else
		{
			XUINT nOldSize = m_nSize;
			SetSize(m_nSize + nCount);
			//“∆∂Ø ˝æ›ƒ⁄»›£¨Ã⁄≥ˆ“™≤Â»Îµƒø◊º“
			XMOVE(&m_pData[nIndex+nCount], &m_pData[nIndex],
				  (nOldSize-nIndex) * sizeof(_XT));
			XSET(&m_pData[nIndex], 0, nCount * sizeof(_XT));
		}
		//∏¥÷∆ ˝æ›µΩ÷∏∂®Œª÷√
		while (nCount--)
			m_pData[nIndex++] = newElement;

	}

template<class _XT>
void XVector<_XT>::SetSize(XUINT nNewSize,XBOOL bExtern)
{
		if (nNewSize == 0)
		{	//–¬µƒ ˝æ›≥§∂»Œ™¡„,«Â≥˝»´≤øƒ⁄»›£¨≤¢ Õ∑≈ø’º‰
			if(m_pData!=XNULL)
				delete[] m_pData;
			m_pData = XNULL;
			m_nSize = m_nMaxSize = 0;
		}
		else if (m_pData == XNULL)
		{	//‘≠ ˝◊ÈŒ™ø’
			//º∆À„–¬µƒ≥§∂»
			m_nMaxSize=CalcSize(nNewSize,bExtern);
			m_pData = new _XT[m_nMaxSize];
			if(m_pData==XNULL)
			{
				m_nSize=m_nMaxSize=0;
				return;
			}
			XSET(m_pData, 0, m_nMaxSize * sizeof(_XT));
			m_nSize = nNewSize;
		}
		else if (nNewSize <= m_nMaxSize)
		{	//–¬µƒ ˝◊È≥§∂»–°”⁄‘≠¿¥µƒ◊Ó¥Û≥§∂»
			if(nNewSize<m_nSize)
				XSET(m_pData+nNewSize,0,(m_nSize-nNewSize)*sizeof(_XT));
			//÷ÿ–¬…Ë÷√ ˝◊È≥§∂»
			m_nSize = nNewSize;
		}
		else
		{	
			//–¬µƒ≥§∂»≥¨π˝◊Ó¥Û≥§∂»£¨÷ÿ–¬∑÷≈‰ø’º‰
			m_nMaxSize=CalcSize(nNewSize,bExtern);//+nExt;
			_XT* pNewData = new _XT[m_nMaxSize];
			if(pNewData==XNULL)
			{
				m_nSize=m_nMaxSize=0;
				return;
			}
			//∏¥÷∆‘≠¿¥µƒ ˝æ›
			XCOPY(pNewData, m_pData, m_nSize * sizeof(_XT));
			XSET(&pNewData[m_nSize], 0, (m_nMaxSize-m_nSize) * sizeof(_XT));

			//…Ë÷√–¬µƒ ˝æ›÷∏œÚ
			delete[] m_pData;
			m_pData = pNewData;
			m_nSize = nNewSize;
		}
	}

typedef XVector<XU8>  XU8Array;
typedef XVector<XU16> XU16Array;
typedef XVector<XU32> XU32Array;
typedef XVector<XS8>  XS8Array;
typedef XVector<XS16> XS16Array;
typedef XVector<XS32> XS32Array;
typedef XVector<char> XCharArray;
typedef XVector<int>  XIntArray;
//typedef XVector<void*>XVoidArray;

/*class _XEXT_CLASS XVoidArray
{
protected:
	void* *m_pData;	 // ˝æ›µÿ÷∑
	XU32 m_nSize,    // ˝◊Èµƒ µº ≥§∂»
		 m_nMaxSize; // ˝◊È◊Ó¥Ûƒ⁄¥Ê≥§∂»
public:
	//typedef XVector<void*> MyVector;
	XVoidArray()
	{	//ƒ¨»œππ‘Ï∫Ø ˝
		m_nSize=m_nMaxSize=0;
		m_pData=XNULL;
	}
	XVoidArray(XUINT nSize)
	{	//÷∏∂® ˝◊È≥§∂»
		m_nSize=m_nMaxSize=0;
		m_pData=XNULL;
		SetSize(nSize,XFALSE);
	}
	~XVoidArray()
	{
		if(m_pData!=XNULL) delete[] m_pData;
		m_pData=XNULL;
	}
	XVoidArray(const XVoidArray&src)
	{   //¥”¡ÌÕ‚“ª∏ˆ ˝◊Èππ‘Ï
		m_nSize=m_nMaxSize=0;
		m_pData=XNULL;
		Copy(src);
	}
public:
	XBOOL EnumAdd(void* src)
	{
		for(XU32 i=0;i<m_nSize;i++)
			if(m_pData[i]==src) return XFALSE;
		Add(src);
		return XTRUE;
	}
	void EnumAdd(void*key,void*val)
	{
		for(XU32 i=0;i<m_nSize;i+=2)
		{
			if(m_pData[i]==key) 
			{
				m_pData[i+1]=val;
				return;
			}
		}
		Add(key);
		Add(val);
	}
	XBOOL operator==(const XVoidArray&src)
	{
		if(m_nSize!=src.m_nSize) return XFALSE;
		if(m_nSize>0)
			return XCMP(m_pData,src.m_pData,m_nSize)==0;
		return XTRUE;
	}
	void operator=(const XVoidArray&src)
	{	//∏¥÷∆
		Copy(src);
	}
	//ªÒµ√ ˝◊È≥§∂»
	XUINT GetSize() const
	{return m_nSize;}
	//ªÒµ√ ˝◊È∂‘”¶µƒƒ⁄¥Ê◊÷Ω⁄≥§∂»
	XUINT GetMenSize() const
	{return m_nSize*sizeof(void*);}
	//void* operator[](XINT nIndex){return m_pData[nIndex];}
	//ªÒµ√÷∏∂®Œª÷√µƒ‘™Àÿ
	const void*& GetAt(XINT nIndex)const
	{return m_pData[nIndex];}
	//ªÒµ√ ˝æ›µÿ÷∑
	void** GetData()const
	{return m_pData;}
	operator void**()const
	{return m_pData;}
	//////////////////////////////////////////////////////
	//…Ë÷√–¬µƒ ˝æ›≥§∂»
	//nNewSize(IN) –¬µƒ ˝◊È≥§∂»
	//bExtern(IN)  «∑Ò‘§¡Ùø’º‰
	void SetSize(XUINT nNewSize,XBOOL bExtern=XTRUE)
	{
		if (nNewSize == 0)
		{	//–¬µƒ ˝æ›≥§∂»Œ™¡„,«Â≥˝»´≤øƒ⁄»›£¨≤¢ Õ∑≈ø’º‰
			if(m_pData!=XNULL)
				delete[] m_pData;
			m_pData = XNULL;
			m_nSize = m_nMaxSize = 0;
		}
		else if (m_pData == XNULL)
		{	//‘≠ ˝◊ÈŒ™ø’
			//º∆À„–¬µƒ≥§∂»
			m_nMaxSize=CalcSize(nNewSize,bExtern);
			m_pData = new void*[m_nMaxSize];
			if(m_pData==XNULL)
			{
				m_nSize=m_nMaxSize=0;
				return;
			}
			XSET(m_pData, 0, m_nMaxSize * sizeof(void*));
			m_nSize = nNewSize;
		}
		else if (nNewSize <= m_nMaxSize)
		{	//–¬µƒ ˝◊È≥§∂»–°”⁄‘≠¿¥µƒ◊Ó¥Û≥§∂»
			if(nNewSize<m_nSize)
				XSET(m_pData+nNewSize,0,(m_nSize-nNewSize)*sizeof(void*));
			//÷ÿ–¬…Ë÷√ ˝◊È≥§∂»
			m_nSize = nNewSize;
		}
		else
		{	
			//–¬µƒ≥§∂»≥¨π˝◊Ó¥Û≥§∂»£¨÷ÿ–¬∑÷≈‰ø’º‰
			m_nMaxSize=CalcSize(nNewSize,bExtern);//+nExt;
			void** pNewData = new void*[m_nMaxSize];
			if(pNewData==XNULL)
			{
				m_nSize=m_nMaxSize=0;
				return;
			}
			//∏¥÷∆‘≠¿¥µƒ ˝æ›
			XCOPY(pNewData, m_pData, m_nSize * sizeof(void*));
			XSET(&pNewData[m_nSize], 0, (m_nMaxSize-m_nSize) * sizeof(void*));

			//…Ë÷√–¬µƒ ˝æ›÷∏œÚ
			delete[] m_pData;
			m_pData = pNewData;
			m_nSize = nNewSize;
		}
	}
	//////////////////////////////////////////////////////
	// ÃÌº”“ª∏ˆ‘™ÀÿµΩ ˝◊ÈŒ≤≤ø
	// v(IN) “™ÃÌº”µƒ‘™Àÿ
	void Add(void*v)
	{
		SetSize(m_nSize+1);
		m_pData[m_nSize-1]=v;
	}
	///////////////////////////////////////////////////////////
	// ÃÌº”¡ÌÕ‚“ª∏ˆ ˝◊Èƒ⁄»›µΩ±æ È◊ÈµƒŒ≤≤ø
	// src(IN) “™ÃÌº”µƒ ˝◊È
	XUINT Append(const XVoidArray& src)
	{
		XUINT nOldSize = m_nSize;
		SetSize(m_nSize + src.m_nSize,XFALSE);
		XCOPY(m_pData + nOldSize, src.m_pData, src.m_nSize * sizeof(void*));
		return nOldSize;
	}
	//////////////////////////////////////////////////////////////
	// Ω´¡ÌÕ‚“ª∏ˆ ˝◊Èµƒƒ⁄»›∏¥÷∆µΩ±æ ˝◊È
	// src(IN) “™∏¥÷∆ƒ⁄»›µƒ ˝◊È
	void Copy(const XVoidArray& src)
	{
		SetSize(src.m_nSize,XFALSE);
		XCOPY(m_pData, src.m_pData, src.m_nSize * sizeof(void*));
	}
	
    /////////////////////////////////////////////////////////////
	//  ˝∑≈∂‡”‡µƒƒ⁄»›ø’º‰
	void FreeExtra()
	{
		//return;
		if (m_nSize != m_nMaxSize)
		{	//¥Ê‘⁄∂‡”‡µƒø’º‰
			void** pNewData = XNULL;
			if (m_nSize != 0)
			{
				//…Í«Î–¬µƒø’º‰
				pNewData = new void*[m_nSize];
				//∏¥÷∆‘≠¿¥µƒ ˝æ›
				XCOPY(pNewData, m_pData,m_nSize*sizeof(void*));
			}
			//…Ë÷√–¬µƒ ˝æ›÷∏œÚ
			delete[] m_pData;
			m_pData = pNewData;
			m_nMaxSize = m_nSize;
		}
	}
	//////////////////////////////////////////////////////////////////
	// ÷ÿ∏¥≤Â»Î“ª∏ˆ‘™ÀÿµΩ÷∏∂®Œª÷√
	void InsertAt(XUINT nIndex,void*newElement, XUINT nCount=1)
	{
		if(nCount<=0) return;
		if (nIndex >= m_nSize)
		{
			//≤Â»ÎµƒŒª÷√≥¨π˝ ˝◊È≥§∂»£¨Õÿ’π ˝◊È≥§∂»
			SetSize(nIndex + nCount);  // grow so nIndex is valid
		}
		else
		{
			XUINT nOldSize = m_nSize;
			SetSize(m_nSize + nCount);
			//“∆∂Ø ˝æ›ƒ⁄»›£¨Ã⁄≥ˆ“™≤Â»Îµƒø◊º“
			XMOVE(&m_pData[nIndex+nCount], &m_pData[nIndex],
				  (nOldSize-nIndex) * sizeof(void*));
			XSET(&m_pData[nIndex], 0, nCount * sizeof(void*));
		}
		//∏¥÷∆ ˝æ›µΩ÷∏∂®Œª÷√
		while (nCount--)
			m_pData[nIndex++] = newElement;

	}
	void* RemoveLast()
	{
		if(m_nSize>0)
			m_nSize--;
		return m_pData[m_nSize];
	}
	void* GetFirst()
	{
		if(m_nSize<=0) return XNULL;
		return m_pData[0];
	}
	void* GetLast()
	{
		if(m_nSize<=0) return XNULL;
		return m_pData[m_nSize-1];
	}
	////////////////////////////////////////////////////////////
	// …æ≥˝÷∏∂®Œª÷√µƒ∂‡∏ˆ‘™Àÿ
	// nIndex(IN) ø™ º…æ≥˝µƒŒª÷√
	// nCount(IN) “™…æ≥˝µƒ‘™Àÿ∏ˆ ˝
	void RemoveAt(XUINT nIndex, XUINT nCount=1)
	{
		if(nIndex+nCount>=m_nSize)
			nCount=m_nSize-nIndex;
		if(nCount<=0) return;
		//º∆À„“™“∆∂Øµƒ≥§∂»
		XUINT nMoveCount = m_nSize - (nIndex + nCount);

		if (nMoveCount>0)	//“∆∂Ø ˝æ›
			XMOVE(&m_pData[nIndex], &m_pData[nIndex + nCount],
				nMoveCount * sizeof(void*));
		//if(nIndex+nCount<m_nSize)
		XSET(m_pData+nIndex+nMoveCount,0,sizeof(void*)*nCount);
		//for(XINT i=nIndex+nCount;i<m_nSize;i++)
		//	m_pData[i
		m_nSize -= nCount;
	}
	void FreeAll()
	{
		for(XU32 i=0;i<m_nSize;i++)
		{
			//if(i==169)
			//	int v=0;
			delete m_pData[i];
		}
		m_nSize=0;
		FreeExtra();
	}
	//«Âø’»´≤ø ˝æ›
	void RemoveAll(XBOOL bClear=XTRUE)
	{
		m_nSize=0;
		if(bClear)
			FreeExtra();
	}
	////////////////////////////////////////////////////////////
	// Ω´¡ÌÕ‚“ª∏ˆ ˝◊Èµƒƒ⁄»›≤Â»ÎµΩ±æ ˝◊Èµƒ÷∏∂®Œª÷√
	// nIndex(IN) “™≤Â»ÎµƒŒª÷√
	// pNewArray(IN) “™≤Â»Îµƒ ˝◊È
	void InsertAt(XUINT nIndex, XVoidArray&pNewArray)
	{
		if (pNewArray.GetSize() >0)
		{
			XUINT nOldSize=m_nSize;
			//…Ë÷√≥§∂»
			SetSize(m_nSize+pNewArray.m_nSize);
			//“∆∂Ø ˝æ›ƒ⁄»›
			XMOVE(&m_pData[nIndex+pNewArray.m_nSize],&m_pData[nIndex],
				  (nOldSize-nIndex)*sizeof(void*));
			//∏¥÷∆ƒ⁄»›
			XCOPY(&m_pData[nIndex],pNewArray.m_pData,pNewArray.m_nSize*sizeof(void*));
		}
	}
	//////////////////////////////////////////////////////////
	// ÃÌº”“ª∂Œ ˝æ›µΩ ˝◊ÈµƒŒ≤≤ø
	// pData(IN) “™ÃÌº”µƒ ˝æ›µÿ÷∑
	// nSize(IN) “™ÃÌº”µƒ ˝æ›≥§∂»
	XUINT Append(const void** pData,XUINT nSize)
	{
		if(pData==XNULL) return m_nSize;
		XUINT nOldSize = m_nSize;
		//…Ë÷√–¬µƒ≥§∂»
		SetSize(m_nSize + nSize,XFALSE);
		XCOPY(m_pData + nOldSize, pData, nSize * sizeof(void*));
		return nOldSize;	
	}
	/////////////////////////////////////////////////////////////
	// ÃÌº”“ª∂Œ ˝æ›µΩ÷∏∂®µƒŒª÷√
	// nIndex(IN) “™ÃÌº”µƒŒª÷√
	// pData(IN) “™ÃÌº” ˝æ›µƒµÿ÷∑
	// nSize(IN) “™ÃÌº” ˝æ›≥§∂»
	XUINT InsertAt(XUINT nIndex,const void** pData,XUINT nSize)
	{
		if(nIndex>=m_nSize) return Append(pData,nSize);
	   	int nOldSize=m_nSize;
		SetSize(m_nSize+nSize);
		XMOVE(&m_pData[nIndex+nSize],&m_pData[nIndex],
			  (nOldSize-nIndex)*sizeof(void*));
		XCOPY(&m_pData[nIndex],pData,nSize*sizeof(void*));
		return nOldSize;
	}
	void** DataTo()
	{
		void** p=m_pData;
		m_pData=NULL;
		m_nSize=m_nMaxSize=0;
		return p;
	}
	void DataFrom(XVoidArray&src)
	{
		if(m_pData!=NULL) delete m_pData;
		m_pData=src.m_pData;
		m_nMaxSize=src.m_nMaxSize;
		m_nSize=src.m_nSize;
		src.m_pData=XNULL;
		src.m_nSize=0;
		src.m_nMaxSize=0;
	}
	void DataFrom(void** pData,XU32 nSize)
	{
		if(m_pData!=NULL) delete m_pData;
		m_pData=pData;
		m_nMaxSize=m_nSize=nSize;
	}

protected:
	XUINT CalcSize(XUINT nSize,XBOOL bExtern)
	{
		if(!bExtern) return nSize;
		if(nSize==0) return 0;
		else if(nSize<4) nSize=4;
		else if(nSize<64) nSize+=16;
		else if(nSize<256) nSize+=nSize>>1;
		else if(nSize<1024) nSize+=nSize>>2;
		else nSize+=nSize>>4;
		return nSize;
	}
};*/
/*class XVoidArray:public XVector<void*>
{
};*/
 
template <class _XT> class _XEXT_CLASS XArray
{
protected:
	_XT *m_pData;
	XU16 m_nSize,m_nMaxSize;	
public:
	typedef XVector<_XT> MyArray;
	_XT* DataTo()
	{
		_XT* p=m_pData;
		m_pData=XNULL;
		m_nSize=m_nMaxSize=0;
		return p;
	}
	void DataFrom(_XT* pData,XU16 nSize)
	{
		if(m_pData!=XNULL) delete m_pData;
		m_pData=pData;
		m_nMaxSize=m_nSize=nSize;
	}
	void DataFrom(MyArray&src)
	{
		if(m_pData!=XNULL) delete m_pData;
		m_pData=src.m_pData;
		m_nMaxSize=src.m_nMaxSize;
		m_nSize=src.m_nSize;
		src.m_pData=XNULL;
		src.m_nSize=0;
		src.m_nMaxSize=0;
	}
	typedef XArray<_XT> MyVector;
	XArray()
	{	//ƒ¨»œππ‘Ï∫Ø ˝
		m_nSize=m_nMaxSize=0;
		m_pData=XNULL;
	}
	XArray(XUINT nSize)
	{	//÷∆∂®ƒ⁄¥Ê≥§∂»
		SetSize(nSize);
	}
	virtual ~XArray()
	{	
		if(m_pData!=XNULL) delete[] m_pData;
	}
	XArray(const MyVector&src)
	{	//¥”¡ÌÕ‚“ª∏ˆ∂‘œÛ∏¥÷∆
		Copy(src);
	}
public:
	void operator=(const MyVector&src)
	{	Copy(src); }
	//ªÒµ√ ˝◊È≥§∂»
	XUINT GetSize()const
	{return m_nSize;}
	//ªÒµ√÷∏∂®œ¬±Íµƒ∂‘œÛ
	_XT& GetAt(XINT nIndex) {return m_pData[nIndex];}
	//÷ÿ‘ÿ¿‡–Õ◊™ªª∫Ø ˝
	operator _XT*(){return m_pData;}
	//////////////////////////////////////////////////
	// …Ë÷√ ˝◊È≥§∂»
	// nNewSize(IN) –¬µƒ ˝◊È≥§∂»
	// bExtern(IN)	 «∑Ò‘§Õÿ’π≤ø∑÷ƒ⁄¥Ê
	void SetSize(XUINT nNewSize,XBOOL bExtern=XTRUE);
	////////////////////////////////////////////////
	// ÃÌº”“ª∏ˆ‘™ÀÿµΩ ˝◊ÈŒ≤≤ø
	// v(IN) “™ÃÌº”µƒ‘™Àÿ
	void Add(const _XT&v)
	{
		//…Ë÷√–¬µƒ≥§∂»
		SetSize(m_nSize+1);
		//∏¥÷∆ƒ⁄»›
		m_pData[m_nSize-1]=v;
	}
	////////////////////////////////////////////////
	// Ω´∆‰À˚ ˝◊Èµƒƒ⁄»›ÃÌº”µΩ ˝◊ÈŒ≤≤ø
	// src(IN) “™ÃÌº”µƒ ˝◊È
	// return: ‘≠ ˝◊È≥§∂»
	XUINT Append(const MyVector& src)
	{
		XUINT nOldSize = m_nSize;
		//…Ë÷√≥§∂»
		SetSize(m_nSize + src.m_nSize,XFALSE);
		//∏¥÷∆ƒ⁄»›
		for(XUINT i=0;i<src.m_nSize;i++)
			m_pData[nOldSize+i]=src.m_pData[i];
		return nOldSize;
	}
	////////////////////////////////////////////////
	// Ω´¡ÌÕ‚“ª∏ˆ ˝◊Èµƒƒ⁄»›∏¥÷∆µΩ±æ ˝◊È
	void Copy(const MyVector& src)
	{
		SetSize(src.m_nSize,XFALSE);
		for(XUINT i=0;i<src.m_nSize;i++)
			m_pData[i]=src.m_pData[i];
	}
	///////////////////////////////////////////////////
	//  Õ∑≈√ª”– π”√µƒ∂‡”‡ƒ⁄¥Ê
	void FreeExtra()
	{
		//¥Ê‘⁄∂‡”‡ø’º‰
		if (m_nSize != m_nMaxSize)
		{	
			_XT* pNewData = XNULL;
			if (m_nSize != 0)
			{
				//…Í«Î“ªøÈ∏’∫√  ∫œµƒø’º‰
				pNewData = new _XT[m_nSize];
				//∏¥÷∆ƒ⁄»›
				for(XUINT i=0;i<m_nSize;i++)
					pNewData[i]=m_pData[i];
			}
			delete[] m_pData;
			m_pData = pNewData;
			m_nMaxSize = m_nSize;
		}
	}
	/////////////////////////////////////////////////////////////
	// ÷ÿ∏¥≤Â»Î‘™ÀÿµΩ÷∏∂®Œª÷√
	// nIndex(IN) “™ø™ º≤Â»ÎµƒŒª÷√
	// newElement(IN) “™≤Â»Îµƒ‘™Àÿ
	// nCount(IN) “™÷ÿ∏¥≤Â»Îµƒ¥Œ ˝
	void InsertAt(XUINT nIndex,const _XT&newElement, XUINT nCount=1)
	{
		if(nCount<=0) return;
		if (nIndex >= m_nSize)
		{
			//≤Â»ÎµƒŒª÷√≥¨π˝ ˝◊È≥§∂»£¨º”≥§ ˝◊È≥§∂»
			SetSize(nIndex + nCount);
		}
		else
		{
//			XUINT nOldSize = m_nSize;
			SetSize(m_nSize + nCount);  //…Ë÷√–¬µƒ ˝◊È≥§∂»
			//“∆∂Ø ˝◊Èƒ⁄»›,Œ™≤Â»ÎŒª÷√Ã⁄≥ˆø’º‰
			for(XUINT i=m_nSize-1;i>=nIndex+nCount;i--)
				m_pData[i]=m_pData[i-nCount];
		}
		//∏¥÷∆‘™ÀÿµΩ÷∏∂®Œª÷√
		while (nCount--)
			m_pData[nIndex++] = newElement;

	}
	////////////////////////////////////////////////////////
	// …æ≥˝÷∏∂®Œª÷√µƒN∏ˆ‘™Àÿ
	void RemoveAt(XUINT nIndex, XUINT nCount=1)
	{
		if(nCount<=0) return;
		/////////////////////////////////////////
		// “∆∂Ø ˝◊Èƒ⁄»›
		for(XUINT i=nIndex;i<m_nSize;i++)
		{
		  XUINT id=i+nCount;
		  if(id<m_nSize) //”––ßƒ⁄»›
			  m_pData[i]=m_pData[id];
		  //else // ˝◊È≥§∂»÷ÆÕ‚			 
		  //	  m_pData[i]=0;
		}
		m_nSize -= nCount;
	}
	void FreeAll()
	{
		for(XU32 i=0;i<m_nSize;i++)
			delete m_pData[i];
		m_nSize=0;
		FreeExtra();
	}
	///////////////////////////////////////////////
	// «Âø’ ˝◊È£¨≤¢«“ Õ∑≈ƒ⁄¥Ê
	void RemoveAll(XU8 bClear=XTRUE)
	{
		m_nSize=0;
		if(bClear)
			FreeExtra();
	}
	//////////////////////////////////////////////////
	// Ω´¡ÌÕ‚“ª∏ˆ ˝◊È÷–µƒ»´≤ø‘™Àÿ£¨≤Â»ÎµΩ÷∏∂®Œª÷√
	// nIndex(IN) ≤Â»ÎŒª÷√
	// pNewArray(IN) “™≤Â»Îµƒ ˝◊È
	void InsertAt(XUINT nIndex, MyVector&pNewArray)
	{
		if (pNewArray.GetSize() >0)
		{
			XINT i;
			XUINT nOldSize=m_nSize;
			int nCount=pNewArray->GetSize();
			SetSize(m_nSize+pNewArray.m_nSize);
			for(i=m_nSize-1;i>=nIndex+nCount;i--)
				m_pData[i]=m_pData[i-nCount];
			for(i=0;i<pNewArray.m_nSize;i++)
				m_pData[nIndex+i]=pNewArray.m_pData[i];
		}
	}
	
protected:
	///////////////////////////////////////////////
	// º∆À„∫œ  µƒƒ⁄¥Ê≥§∂»
	// nSize(IN)  ˝◊È≥§∂»
	// bExtern(IN)  «∑Ò‘§¡Ùø’º‰
	XUINT CalcSize(XUINT nSize,XBOOL bExtern)
	{
		if(!bExtern) return nSize;
		if(nSize==0) return 0;
		else if(nSize<4) nSize=4;
		else if(nSize<64) nSize+=16;
		else if(nSize<256) nSize+=nSize>>1;
		else if(nSize<1024) nSize+=nSize>>2;
		else nSize+=nSize>>4;
		return nSize;
	}	

};

template<class _XT>
void XArray<_XT>::SetSize(XUINT nNewSize,XBOOL bExtern)
	{
		if (nNewSize == 0)
		{	//–¬µƒ≥§∂»Œ™0£¨…æ≥˝»´≤ø ˝æ›
			if(m_pData!=XNULL) delete[] m_pData;
			m_pData = XNULL;
			m_nSize = m_nMaxSize = 0;
		}
		else if (m_pData == XNULL)
		{	//‘≠¿¥µƒ≥§∂»Œ™0£¨…Í«Î–Ë“™µƒø’º‰
			m_nMaxSize=CalcSize(nNewSize,bExtern);
			m_pData = new _XT[m_nMaxSize];
			//for(XUINT i=0;i<m_nMaxSize;i++)
			//	m_pData[i]=XNULL;
			m_nSize = nNewSize;
		}
		else if (nNewSize <= m_nMaxSize)
		{	//–¬≥§∂»–°”⁄‘≠¿¥µƒ≥§∂»£¨Ωÿ∂œ ˝æ›£¨≤ª Õ∑≈ƒ⁄¥Ê
			//for(XUINT i=nNewSize;i<m_nSize;i++)
			//	m_pData[i]=XNULL;
			m_nSize = nNewSize;
		}
		else
		{	
			//–¬≥§∂»¥Û”⁄‘≠¿¥µƒ≥§∂»£¨…Í«Î–Ë“™µƒø’º‰
			m_nMaxSize=CalcSize(nNewSize,bExtern);//+nExt;
			_XT* pNewData = new _XT[m_nMaxSize];

			//∏¥÷∆ªª¿¥µƒ ˝æ›
			for(XUINT i=0;i<m_nSize;i++)
				pNewData[i]=m_pData[i];
			//«Âø’–¬…Í«Îµƒ ˝æ›
			//for(i=m_nSize;i<m_nMaxSize;i++)
			//	pNewData[i]=0;
			delete[] m_pData;
			m_pData = pNewData;
			m_nSize = nNewSize;
		}
	}


//////////////////////////////////////////////////////////////
// ∂—’ª÷ß≥÷¿‡£¨÷ß≥÷ª˘±æµƒ ˝æ›¿‡–Õ∫Õ¿‡
template <class _XT> class _XEXT_CLASS XStack:public XVector<_XT>
{
public:
	void Push(const _XT&v){Add(v);}
	XBOOL Pop(_XT&v) 
	{
		if(this->m_nSize<=0) return XFALSE;
		this->m_nSize--;
		v=this->m_pData[this->m_nSize];
		return XTRUE;
	}
	_XT Pop()
	{
		if(this->m_nSize<=0) return 0;
		this->m_nSize--;
		return this->m_pData[this->m_nSize];
	}
};

//////////////////////////////////////////////////////////////
// ∂—’ª÷ß≥÷¿‡£¨÷ß≥÷ª˘±æµƒ ˝æ›¿‡–Õ∫Õ¿‡
template <class _XT> class _XEXT_CLASS XStackC:public XArray<_XT>
{
public:
	void Push(const _XT&v){Add(v);}
	XBOOL Pop(_XT&v) 
	{
		if(this->m_nSize<=0) return XFALSE;
		this->m_nSize--;
		v=this->m_pData[this->m_nSize];
		return XTRUE;
	}
	_XT Pop()
	{
		if(this->m_nSize<=0) return 0;
		this->m_nSize--;
		return this->m_pData[this->m_nSize];
	}
};

#define DOMPOS XINT
typedef XVector<XINT> DOMPOSARRAY;

//#define XPOSITION void**
typedef void** XPOSITION;

template <class _XT> 
class _XEXT_CLASS XList
{
	struct XPos
	{
		XPos*pNext;
		_XT  pData;
	};
public:
	XList()
	{
		m_pFirst=XNULL;
	}
	~XList()
	{
		Release();
	}
	void Release();
	XPOSITION GetFirstPosition()
	{
		return (XPOSITION)&m_pFirst;
	}
	XPOSITION AddFirst(_XT data);
	_XT GetFirst()
	{
		if(m_pFirst==XNULL) return XNULL;
		return m_pFirst->pData;
	}
	_XT RemoveFist()
	{
		if(m_pFirst==XNULL) return XNULL;
		m_pFirst=m_pFirst->pNext;
		_XT data=m_pFirst->pData;
		return data;
	}
	XPOSITION GetNext(XPOSITION pos)
	{
		//if(!pos) return XNULL;
		XPos *p=*(XPos**)pos;
		//pos=(XPOSITION)&(p)->pNext;
		return (XPOSITION)&p->pNext;
		//return (p)->pData;
		//return &(*p)->pNext;	
	}
	_XT GetAt(XPOSITION pos)
	{
		//if(!pos) return XNULL;
		XPos *p=*(XPos**)pos;
		return (p)->pData;
	}
	XPOSITION AddAt(XPOSITION pos,_XT data);
	_XT RemoveAt(XPOSITION pos)
	{
		XPos**p=(XPos**)pos;
		_XT data=(*p)->pData;
		*p=(*p)->pNext;
		return data;
		//return 
	}
	void Expand(XVector<_XT> data)
	{
		XPos*p=m_pFirst;
		while(p)
		{
			data.Add(p->pData);
			p=p->pNext;
		}
	}
protected:
	XPos* m_pFirst;
};

#define XRELEASELIST(VV) {XPOSITION pos=VV.GetFirstPosition();while(pos)\
{delete VV.GetAt(pos);pos=VV.GetNext(pos);}VV.Release();}

template <class _XT>
XPOSITION XList<_XT>::AddAt(XPOSITION pos,_XT data)
{
	
	XPos*pNew=new XPos;
	if(!pNew) return XNULL;
	XPos**p=(XPos**)pos;
	pNew->pNext=(*p)->pNext;
	(*p)->pNext=pNew;
	return (XPOSITION)&(*p)->pNext;
}

template <class _XT>
XPOSITION XList<_XT>::AddFirst(_XT data)
{
	XPos*p=new XPos;
	if(!p) return XNULL;
	p->pNext=m_pFirst;
	p->pData=data;
	m_pFirst=p;
	return (XPOSITION)&m_pFirst;
}

template <class _XT>
void XList<_XT>::Release()
{
	XPos*p=m_pFirst;
	while(p)
	{
		XPos*pNext=p->pNext;
		delete p;
		p=pNext;
	}
	m_pFirst=XNULL;
}

template<class _XT>
class XStackL:private XList<_XT>
{
public:
	XBOOL IsEmpty(){return this->m_pFirst==XNULL;}
	_XT Pop()
	{	return this->RemoveFist(); }
	void Push(_XT data)
	{
		this->AddFirst(data);
	}	
};

#define XRELEASESTACK(VV,TYPE) {TYPE*p=VV.Pop();while(p) {delete p;p=VV.Pop();}}


/////////////////////////////////////////////////////
// SWF�������࣬���������λ��ʽ��������
// ����Ӹ����ж�����������FLASH����
class XStream : public XVector<XU8>
{
public:
	void* DataTo()
	{
		XU8*p=m_pData;
		m_pData=XNULL;
		m_nSize=m_nMaxSize=0;
		Reset();
		return p;
	}
	void SetBuffer(int nSize){SetSize(nSize,XFALSE);}
	XStream(XPCVOID pData,int nLength)
	{
		AttachFrom(pData,nLength);
	}
	XU32 GetDataSize(){return writePos-readPos;}
	void AttachFrom(XPCVOID pData, int nLength,XU8 bMem=0);
	
	XU8* ClearNoFree()
	{
		XU8* pData=m_pData;
		m_pData=XNULL;
		m_nMaxSize=m_nSize=0;
		Reset();
		return pData;
	}
	void AttachTo(XU8Array&data)
	{
		data.DataFrom(m_pData,m_nSize);
		m_pData=XNULL;
		m_nSize=m_nMaxSize=0;
		Reset();
	}

	//д���ֽ�
	void WriteByte(XU8 data)
	{
		FlushWriteBits();
		Add(data);
	}
	XU8* ReadAddr(){return m_pData+readPos;}
	XU8* WriteAddr(){return m_pData+readPos;}
	XU8* BufferAddr(){return m_pData;}
	void WriteByte(XS8 data) {WriteData(&data,sizeof(char));}
	void WriteWord(XS16 data){WriteData(&data,sizeof(short));}
	void WriteWord(XU16 data) {WriteData(&data,sizeof(XU16));}
	void WriteDWord(XU32 data){WriteData(&data,sizeof(XU32));}
	void WriteDWord(XS32 data) {WriteData(&data,sizeof(long));}
	void WriteDouble(double data){WriteData(&data,sizeof(double));}
	void WriteDWord64(XS64 data){WriteData(&data,sizeof(XS64));}
	void WriteFloat(float data){WriteData(&data,sizeof(float));}
	void WriteString(XPCTSTR pData)
	{
		if(pData==XNULL) return;
		WriteData(pData,strlen(pData)+1);
	}
	void WriteEU32(XU32 data);
	void WriteDWord24(XU32 data);
	//void WriteFixed16(double data);
	//void WriteFixed32(double data);
	//д�����ݿ鵽��
	void WriteData(XPCVOID pData,int nLength);
	//���ļ��ж������ݵ�������
//	int  ReadFromFile(XFile&file,int nLength);
	XU8 NextByte(){return m_pData[readPos++];}
	void FlushWriteBits()
	{
		if(writeBytePos!=8)
		{
			writePos++;
			readBytePos=8;
		}
	}
	//int GetDataSize(){return writePos-readPos;}
public:
	void WriteBits(XU32 data,XU8 size);
	XPCTSTR ReadString();
	//void AttachTo(XU8Array&data);
	//void AttachData(XPCVOID pData,int nLength,XU8 bMem=0);
	// ��ȡһ���ַ���
//	XBOOL ReadString(XString8&string,int nLen);
	XBOOL Skip(int nLength);
	XBOOL ReadData(void*pData,int nLength);
	// ����ʣ�������λ
	void FlushReadBits()
	{
		if(readBytePos!=8)
		{
			readPos++;
			readBytePos=8;
		}
	}
	// ��黺���������Ƿ�Ϊ��
	XBOOL IsEmpty(){ return writePos<=readPos; }
	//����һλ
	//XU8 Bit(){ return (m_pData[readPos]>>(readBytePos-1))&1;}
	//XU8 ReadBit();
	// ����һ���䳤����
	XBOOL ReadEU32(XU32&data);
	// ����һ��64λ����
	XBOOL ReadDWord64(XU64& data);
	//����һ��������
	XBOOL ReadFixed16(double&data);
	XBOOL ReadFixed32(double&data);
	// ����һ��32λ����
	XBOOL ReadDWord(XU32&data);
	XBOOL ReadDWord24(XU32&data);
	XBOOL ReadDWord(long&data){return ReadDWord((XU32&)data);}
	XBOOL ReadDouble(double&data);
	XBOOL ReadFloat(float&data){return ReadDWord((XU32&)data);}
	XBOOL ReadFloat16(float&data);
	// ����һ��16λ����
	XBOOL ReadWord(XU16&data);
	XBOOL ReadWord(short&data){return ReadWord((XU16&)data);}
	// ����һ��8λ����
	XBOOL ReadByte(XU8&data);
	XBOOL ReadByte(char&data){return ReadByte((XU8&)data);}

	// ����ָ��λ���ı���
	XBOOL ReadBits0(XU32&data,XU8 size);
	XBOOL ReadBits(XU32&data,XU8 size);
	XBOOL ReadBits(XS16&data,XU8 size);
	XBOOL ReadBits(XU8&data,XU8 size)
	{
		//if(size>8) return XFALSE;
		XU32 d32=0;
		if(!ReadBits(d32,size)) return XFALSE;
		data=(XU8)d32;
		return XTRUE;
	}
	XBOOL ReadBits(XU16&data,XU8 size)
	{
		//if(size>8) return XFALSE;
		XU32 d32=0;
		if(!ReadBits(d32,size)) return XFALSE;
		data=(XU16)d32;
		return XTRUE;
	}
	XBOOL ReadBits(XS32&data,XU8 size);	
	void Reset();
	//��ȡ���ݳ���
	XU32 Bytes(){return writePos-readPos;}
	XU32 Bits(){ return ((writePos-readPos)<<3)+(writeBytePos-readBytePos);}

	XStream();
	void RestoreBits(XU8 bits)
	{
		if(bits<=0) return;
		readBytePos+=bits-1;
		readPos-=readBytePos>>3;
		readBytePos=(readBytePos&7)+1;
	}
	void FreeReadSpace()
	{
		if(readPos>FREEBLOCK)
		{
			RemoveAt(0,readPos);
			writePos-=readPos;
			readPos=0;
		}
	}
	virtual ~XStream();
	void SetParam(XU32 p){nParam=p;}
	XU32 GetParam(){return nParam;}
protected:
	
	XU32  readPos,bAttach;			 //��ȡ��ǰ��������λ��
	XU8   readBytePos;			 //��ǰ�ֽڵ�ʣ��λ��
	XU32  writePos;
	XU8   writeBytePos;
protected:
	XU32  nParam;
	////////////////////////////////////////
	// �ͷſռ�
	
	//�������SWFд����
};
#endif // !defined(AFX_XVECTOR_H__0829E61E_AF2C_494D_A614_B4D3B1984A27__INCLUDED_)
