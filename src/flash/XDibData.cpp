#include "StdAfxflash.h"
#include <assert.h>
#include "XDibData.h"
#include "XXVar.h"

#ifdef __REFDIBMODE__

XFastAlloc* XRefDib::pSimpleAlloc=NULL;
XFastAlloc* XRefDib::pDataAlloc=NULL;
XFastAlloc* XRefDib::pDibAlloc=NULL;

static _REFBLOCKDATA _emptyRefBlock={_REFBLOCKDATA::_REFEMPTY,0x7fffffff,0};
//static _REFBLOCKDATA _blackRefBlock={_REFBLOCKDATA::_REFSIMPLE,0x7fffffff,0xff000000};
//static _REFBLOCKDATA _whiteRefBlock={_REFBLOCKDATA::_REFSIMPLE,0x7fffffff,0xffffffff};

XBOOL XRefDib::InitRefLib()
{
	if(pSimpleAlloc!=NULL) return XFALSE;
	pSimpleAlloc=new XFastAlloc(sizeof(_REFBLOCKDATA),128);
	pDataAlloc=new XFastAlloc(sizeof(_REFBLOCKDATA)+_REFBLOCKSIZE,32);
	pDibAlloc=new XFastAlloc(sizeof(_REFDIBDATA),32);
	if(pDataAlloc==NULL||
	   pSimpleAlloc==NULL||
	   pDataAlloc==NULL)
	{
		ReleaseRefLib();
		return XFALSE;
	}
	return XTRUE;
}
XBOOL XRefDib::ReleaseRefLib()
{
	if(pSimpleAlloc!=NULL)
	{
		pSimpleAlloc->ReleaseData();
		pSimpleAlloc=NULL;
	}
	if(pDataAlloc!=NULL)
	{
		pDataAlloc->ReleaseData();
		pDataAlloc=NULL;
	}
	if(pDibAlloc!=NULL)
	{
		pDibAlloc->ReleaseData();
		pDibAlloc=NULL;
	}
	return XTRUE;
}

_REFBLOCKDATA* XRefDib::CreateColorBlock(XU32 nColor)
{
	assert(pSimpleAlloc!=NULL);
	if(nColor==0)
		return &_emptyRefBlock;
	_REFBLOCKDATA*pBlock=(_REFBLOCKDATA*)pSimpleAlloc->Alloc();
	if(pBlock)
	{
		pBlock->nType=_REFBLOCKDATA::_REFSIMPLE;
		pBlock->nRefCount=0;
		pBlock->nData=nColor;
	}
	return pBlock;
}
_REFBLOCKDATA* XRefDib::CreateDataBlock()
{
	assert(pDataAlloc!=NULL);
	_REFBLOCKDATA*pBlock=(_REFBLOCKDATA*)pDataAlloc->Alloc();
	if(pBlock)
	{
		pBlock->nType=_REFBLOCKDATA::_REFDATA;
		pBlock->nRefCount=0;
		pBlock->nData=(XU32)(pBlock+1);
	}
	return pBlock;
}

void XRefDib::FreeBlock(_REFBLOCKDATA*pBlock)
{
	if(!pBlock) return;
	if(pBlock->nType==_REFBLOCKDATA::_REFEMPTY)
		return;
	if(pBlock->nRefCount>0)
		pBlock->nRefCount--;
	if(!pBlock->nRefCount)
	{
		if(pBlock->nType==_REFBLOCKDATA::_REFDATA)
			pDataAlloc->Free(pBlock);
		else
			pSimpleAlloc->Free(pBlock);
		pBlock->nType=_REFBLOCKDATA::_REFEMPTY;		
	}
}

_REFDIBDATA*  XRefDib::CreateDibData(int w,int h,XU32 nColor)
{
	assert(pDibAlloc!=NULL);
	_REFDIBDATA*pData=(_REFDIBDATA*)pDibAlloc->Alloc();
	if(!pData) return NULL;
	pData->nWidth=w;
	pData->nHeight=h;
	pData->nBlockWidth=(w+_REFBLOCKPIXEL-1)>>_REFBLOCKBITS;
	pData->nBlockHeight=(h+_REFBLOCKPIXEL-1)>>_REFBLOCKBITS;
	pData->nBlockCount=pData->nBlockWidth*pData->nBlockHeight;
	pData->nDataSize=pData->nBlockCount*4;
	pData->pData=(_REFBLOCKDATA**)XXVar::AllocData(pData->nDataSize);
		//new XU32[m_nBlockCount];
	if(pData->pData==NULL)
	{
		pDibAlloc->Free(pData);
		return NULL;
	}
	int i;
	for(i=0;i<pData->nBlockCount;i++)
	{
		REF_SETBLOCK(pData->pData[i],CreateColorBlock(nColor));
	}
	return pData;
}

void XRefDib::FreeDib(_REFDIBDATA*pData)
{
	if(!pData) return;
	if(pData->nRefCount)
		pData->nRefCount--;
	if(!pData->nRefCount)
	{
		int i;
		for(i=0;i<pData->nBlockCount;i++)
		{
			//FreeBlock(pData->m_pData[i]);
			//pData->m_pData[i]=NULL;
			REF_FREEBLOCK(pData->pData[i]);
		}
		XXVar::FreeData(pData->pData,pData->nDataSize);
		pData->pData=NULL;
		pDibAlloc->Free(pData);
	}
	
}
////////////////////////////////////////////////////////////////////
XRefDib::XRefDib()
{
	m_pDib=NULL;
}
XRefDib::~XRefDib()
{
	Release();
}


void XRefDib::Clear()
{
	if(m_pDib==NULL) return;
	int i,nCount=m_pDib->nBlockCount;
	for(i=0;i<nCount;i++)
	{
		REF_COPYBLOCK(m_pDib->pData[i],(&_emptyRefBlock));
	}
}

void XRefDib::Clear(XU32 nColor)
{
	if(m_pDib==NULL) return;
	int i,nCount=m_pDib->nBlockCount;
	_REFBLOCKDATA*pBlock=CreateColorBlock(nColor);
	for(i=0;i<nCount;i++)
	{
		REF_COPYBLOCK(m_pDib->pData[i],pBlock);
	}
}

void XRefDib::FillRect(const XRECT&rect,XU32 nColor)
{
	if(m_pDib==NULL) return;
	_RECTBLOCK block;
	if(!FormatRect(rect,block)) return;
	int bl=block.iLeft,
		br=block.iRight;
	int bt=block.iTop,
		bb=block.iBottom;
	/*int i;
	if(block.iTopY)
	{
		XRECT dr;
		int r=br,l=bl;
		if(block.iLeftX)
		{
			GetRefRect(dr,block,_BLKTOPLEFT);
			r++;
		}
		if(br>bl&&block.iRightX)
		{
			GetRefRect(dr,block,_BLKTOPRIGHT);
			l--;
		}
		for(i=r;i<l;i++)
		{
			GetRefRect(dr,block,_BLKTOP);
		}
		bt++;
	}
	if(bb>bt&&block.iBottomY)
	{
		XRECT dr;
		int r=br,l=bl;
		if(block.iLeftX)
		{
			GetRefRect(dr,block,_BLKBOTTOMLEFT);
			r++;
		}
		if(br>bl&&block.iRightX)
		{
			GetRefRect(dr,block,_BLKBOTTOMRIGHT);
			l--;
		}
		for(i=r;i<l;i++)
		{
			GetRefRect(dr,block,_BLKBOTTOM);
		}
		bb--;
	}
	if(bb>bt)
	{
		XRECT dr;
		if(block.iLeftX)
		{
			for(i=bt;i<bb;i++)
			{
				GetRefRect(dr,block,_BLKLEFT);
			}
		}
		if(br>bl&&block.iRightX)
		{
			for(i=bt;i<bb;i++)
			{
				GetRefRect(dr,block,_BLKRIGHT);
			}
		}
	}*/
}

#endif