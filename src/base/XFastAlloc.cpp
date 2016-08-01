// XFastAlloc.cpp: implementation of the XFastAlloc class.
//
//////////////////////////////////////////////////////////////////////
#include <assert.h>
#include "StdAfxGGBase.h"
#include "XFastAlloc.h"
#ifdef __APPLE__
#include <string>
#endif

//函数实现先放到头文件中，放在cpp文件中出现链接错误  modify by hgl
#define _AllocFlag 0xf0781234

XFastAlloc::XFastAlloc(int nBlockSize,int nInitSize)
{
	m_nBlockSize=nBlockSize+sizeof(XBLOCK)+sizeof(XU32);
	m_nMemSize=m_nBlockSize*nInitSize+sizeof(XDATA);
	m_pFreeList=XNULL;
	m_nInitCount=nInitSize;
	m_pData=XNULL;
	m_nAllocs=0;
	AllocData();
}

void* XFastAlloc::Alloc()
{
    if( m_pFreeList == XNULL )
        AllocData();
    if( m_pFreeList == XNULL )
        return XNULL;
    XBLOCK*pBlock=m_pFreeList;
    m_pFreeList=m_pFreeList->pNext;
    _MemAssert(m_pFreeList);
    pBlock->pNext=0;
    m_nAllocs++;
    return (void*)(pBlock+1);
}

void XFastAlloc::Free(void *pData)
{
    XBLOCK*pBlock=((XBLOCK*)pData)-1;
    _FreeAssert(pBlock);
    _MemAssert(pBlock);
    pBlock->pNext=m_pFreeList;
    m_pFreeList=pBlock;
    m_nAllocs--;
   
}

void XFastAlloc::AllocData()
{
	//LOGWHERE();
	XDATA*pData=(XDATA*)new XU8[m_nMemSize];
	if (!pData) return;
	memset(pData,0,m_nMemSize);
	pData->pNext=m_pData;
	m_pData=pData;
	XBLOCK*pBlock=(XBLOCK*)(pData+1);
	int i = 0;
	for(i=0;i<m_nInitCount;i++)
	{
		pBlock->pNext=m_pFreeList;
		*((XU32*)((XU8*)pBlock+m_nBlockSize-sizeof(XU32)))=_AllocFlag;
		m_pFreeList=pBlock;
		pBlock=(XBLOCK*)(((XU8*)pBlock)+m_nBlockSize);
	}
	//LOGWHERE();
	return;
}

XFastAlloc::~XFastAlloc()
{
	ReleaseData();
}


void XFastAlloc::ReleaseData()
{
	//LOGWHERE();
	while(m_pData)
	{
		XU8*pData=(XU8*)m_pData;
		m_pData=m_pData->pNext;
		delete pData;

	}
	m_pFreeList=XNULL;
	m_nAllocs=0;
	//LOGWHERE();
	return;
}

#ifdef _DEBUG


static void __MemAssert(XFastAlloc*f,void*pData)
{
	if(!pData) return;
	bool bSet=false;
	
	XFastAlloc::XDATA*p=f->m_pData;
	while(p)
	{
		if(pData>=p&&pData<p+f->m_nMemSize)
		{
			bSet=true;
			break;
		}
		p=p->pNext;
	}
	if(!bSet)
	{
		LOGWHERE();
		assert(false);
	}
}

void XFastAlloc::MemAssert(void*p)
{
	if(!p) return;
	__MemAssert(this,p);
	XU32*pData=(XU32*)((XU8*)p+m_nBlockSize-sizeof(XU32));
	//pData+=m_nBlockSize;
	if(*pData!=_AllocFlag)
	{
		LOGWHERE();
		assert(false);
	}
}

void XFastAlloc::FreeAssert(void*pData)
{
	if(!pData)
	{
		assert(false);
		return;
	}
}

#endif
