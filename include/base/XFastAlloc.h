// XFastAlloc.h: interface for the XFastAlloc class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __XFASTALLOC_H__
#define __XFASTALLOC_H__

#include "GGType.h"
#ifdef _DEBUG
	#define _MemAssert MemAssert
	#define _FreeAssert FreeAssert
#else
	#define _MemAssert(ARG)
	#define _FreeAssert(ARG)
#endif
class XFastAlloc  
{
public:
	struct XBLOCK
	{
	//	XBLOCK*pPrev;
		XBLOCK*pNext;
	};
	struct XDATA
	{
		XDATA*pNext;
	};
	int m_nAllocs;
public:
    
    XFastAlloc(int nBlockSize,int nInitBlock);
	virtual ~XFastAlloc();
    
	void ReleaseData();
	void* Alloc();
     void Free(void *pData);
    
#ifdef _DEBUG
	void MemAssert(void*pData);
	void FreeAssert(void*pData);
#endif

    
public:
    int m_nBlockSize;
    int m_nMemSize;
    int m_nInitCount;
	XDATA* m_pData;
	XBLOCK* m_pFreeList;
    
protected:
	void AllocData();
};

#endif // __XFASTALLOC_H__
