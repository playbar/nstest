#ifndef __XDIBDATA__
#define __XDIBDATA__

#ifdef __REFDIBMODE__

#define _REFBLOCKPIXEL  32
//#define _REFBLOCKHEIGHT 32
#define _REFBLOCKBITS 5
#define _REFBLOCKMASK 0x1f
#define _REFBLOCKSIZE (_REFBLOCKPIXEL*_REFBLOCKPIXEL*4)

#define REF_REFBLOCK(BLOCK) {if(BLOCK&&BLOCK->nType!=_REFBLOCKDATA::_REFEMPTY) BLOCK->nRefCount++;}
#define REF_SETBLOCK(DST,BLOCK) {REF_REFBLOCK(BLOCK);DST=BLOCK;}
#define REF_FREEBLOCK(BLOCK) {if(BLOCK) {XRefDib::FreeBlock(BLOCK);BLOCK=NULL;}}
#define REF_COPYBLOCK(DST,BLOCK) {if(DST!=BLOCK){REF_FREEBLOCK(DST);REF_REFBLOCK(BLOCK);DST=BLOCK;}}

#define REF_REFDIB(DIBDIB) {if(DIBDIB) DIBDIB->nRefCount++;}
#define REF_SETDIB(DST,SRT){REF_REFDIB(SRT);DST=SRT;}
#define REF_FREEDIB(SRT){if(SRT) {XRefDib::FreeDib(SRT);SRT=NULL;}}
#define REF_COPYDIB(DST,SRT){if(DST!=SRT){REF_FREEDIB(DST);REF_REFDIB(SRT);DST=SRT;}}

struct _REFBLOCKDATA
{
	enum
	{
		_REFEMPTY=0,
		_REFSIMPLE,
		_REFDATA
	};
	XU32 nType;
	XU32 nRefCount;
	XU32 nData;
};

struct _REFDIBDATA
{
	int nWidth;
	int nHeight;
	int nBlockCount;
	int nBlockWidth;
	int nBlockHeight;
	int nDataSize;
	XU32 nRefCount;
	_REFBLOCKDATA**pData;
};

struct _RECTBLOCK
{
	int iTop,iLeft,iRight,iBottom;
	int iTopY,iLeftX,iRightX,iBottomY;
};

class XRefDib
{
protected:
	//int m_nWidth;
	//int m_nHeight;
	//int m_nBlockCount;
	//int m_nBlockWidth;
	//int m_nBlockHeight;
	//int m_nDataSize;
	//_REFBLOCKDATA**m_pData;
	_REFDIBDATA*m_pDib;
protected:
	static XFastAlloc* pSimpleAlloc;
	static XFastAlloc* pDataAlloc;
	static XFastAlloc* pDibAlloc;
	static XBOOL InitRefLib();
	static XBOOL ReleaseRefLib();
	static _REFBLOCKDATA* CreateColorBlock(XU32 nColor);
	static _REFBLOCKDATA* CreateDataBlock();
	static _REFDIBDATA*   CreateDibData(int w,int h,XU32 nColor=0);
	static void FreeBlock(_REFBLOCKDATA*pBlock);
	static void FreeDib(_REFDIBDATA*pData);
	inline static XBOOL IsEmpty(_REFBLOCKDATA*pBlock)
	{
		return pBlock==NULL||pBlock->nType==_REFBLOCKDATA::_REFEMPTY;
	}
	static inline void CopyBlock(_REFBLOCKDATA*pDst,_REFBLOCKDATA*pSrt)
	{
		assert(pDst->nType==pSrt->nType);
		switch(pDst->nType)
		{
		case _REFBLOCKDATA::_REFDATA:
			 XCOPY((void*)pDst->nData,(void*)pSrt->nData,_REFBLOCKSIZE);
			 break;
		default:
			 pDst->nData=pSrt->nData;
			 break;
		}
	}
public:
	XRefDib();
	~XRefDib();
	inline bool IsEmpty()
	{
		return m_pDib==NULL;
	}
	void Attach(_REFDIBDATA*pDib)
	{
		REF_COPYDIB(m_pDib,pDib);
	}
	inline void Release()
	{
		REF_FREEDIB(m_pDib);
	}
	inline int Width()
	{
		if(!m_pDib) return 0;
		return m_pDib->nWidth;
	}
	inline int Height()
	{
		if(!m_pDib) return 0;
		return m_pDib->nHeight;
	}
	_REFDIBDATA* GetData(){return m_pDib;}
	inline XBOOL Create(int w,int h,XU32 nColor=0)
	{
		_REFDIBDATA*pDib=XRefDib::CreateDibData(w,h,nColor);
		REF_SETDIB(m_pDib,pDib);
	}
	void Clear();
	void Clear(XU32 nColor);
	inline XBOOL FormatRect(const XRECT&rect,_RECTBLOCK&blk)
	{
		int l=rect.left;
		int t=rect.top;
		int r=rect.right;
		int b=rect.bottom;
		if(l<0)
		{
			r+=l;
			l=0;
		}
		if(t<0)
		{
			b+=t;
			t=0;
		}
		if(r>Width())
			r=Width();
		if(b>Height())
			b=Height();
		if(r<=l||b<=t) return XFALSE;
		blk.iLeft=l>>_REFBLOCKBITS;
		blk.iTop=t>>_REFBLOCKBITS;
		blk.iRight=r>>_REFBLOCKBITS;
		blk.iBottom=b>>_REFBLOCKBITS;
		blk.iLeftX=l&_REFBLOCKMASK;
		blk.iTopY=t&_REFBLOCKMASK;
		blk.iRightX=r&_REFBLOCKMASK;
		blk.iBottomY=b&_REFBLOCKMASK;
		if(blk.iLeftX)
			blk.iLeftX=_REFBLOCKPIXEL-blk.iLeftX;
		if(blk.iTopY)
			blk.iTopY=_REFBLOCKPIXEL-blk.iTopY;
		return XTRUE;
	}
	inline _REFBLOCKDATA* GetBlock(int& x,int& y)
	{
		if(!m_pDib) return NULL;
		int ix=x>>_REFBLOCKBITS;
		x&=_REFBLOCKMASK;
		int iy=y>>_REFBLOCKBITS;
		y&=_REFBLOCKMASK;
		int ic=iy*m_pDib->nBlockWidth+ix;
		assert(ic<m_pDib->nBlockCount);
		return m_pDib->pData[ic];
	}
	enum
	{
		_BLKTOPLEFT,_BLKTOP,_BLKTOPRIGHT,
		_BLKLEFT,		    _BLKRIGHT,
		_BLKBOTTOMLEFT,_BLKBOTTOM,_BLKBOTTOMRIGHT,
	};
	/*inline bool GetRefRect(XRECT&rect,const _RECTBLOCK&blk,int ix,int iy)
	{
		
	}*/
	inline void GetRefRect(XRECT&rect,const _RECTBLOCK&blk,int nType)
	{
		switch(nType)
		{
		case _BLKTOPLEFT:
			 rect.top=blk.iTopY;
			 rect.left=blk.iLeftX;
			 if(blk.iLeft==blk.iRight)
			 	 rect.right=blk.iRightX;
			 else
				 rect.right=_REFBLOCKPIXEL;
			 if(blk.iTop==blk.iBottom)
				 rect.bottom=blk.iBottomY;
			 else
				 rect.bottom=_REFBLOCKPIXEL;
			 break;
		case _BLKTOP:
			 rect.left=0;
			 rect.right=_REFBLOCKPIXEL;
			 rect.top=blk.iTopY;
			 if(blk.iTop==blk.iBottom)
				 rect.bottom=blk.iBottomY;
			 else
				 rect.bottom=_REFBLOCKPIXEL;
			 break;
		case _BLKTOPRIGHT:
			 rect.left=0;
			 rect.right=blk.iRightX;
			 rect.top=blk.iTopY;
			 if(blk.iTop==blk.iBottom)
				 rect.bottom=blk.iBottomY;
			 else
				 rect.bottom=_REFBLOCKPIXEL;
			 break;
		case _BLKLEFT:
			 rect.left=blk.iLeftX;
			 if(blk.iLeft==blk.iRight)
			 	 rect.right=blk.iRightX;
			 else
				 rect.right=_REFBLOCKPIXEL;
			 rect.top=0;
			 rect.bottom=_REFBLOCKPIXEL;
			 break;
		case _BLKRIGHT:
			 rect.left=0;
			 rect.right=blk.iRightX;
			 rect.top=0;
			 rect.bottom=_REFBLOCKPIXEL;
			 break;
		case _BLKBOTTOMLEFT:
			 rect.left=blk.iLeftX;
			 rect.top=0;
			 if(blk.iLeft==blk.iRight)
			 	 rect.right=blk.iRightX;
			 else
				 rect.right=_REFBLOCKPIXEL;
			 rect.bottom=blk.iBottomY;
			 break;
		case _BLKBOTTOM:
			 rect.left=0;
			 rect.right=_REFBLOCKPIXEL;
			 rect.top=0;
			 rect.bottom=blk.iBottomY;
			 break;
		case _BLKBOTTOMRIGHT:
			 rect.left=0;
			 rect.top=0;
			 rect.right=blk.iRightX;
			 rect.bottom=blk.iBottomY;
			 break;
		}
	}
	void FillRect(const XRECT&rect,XU32 nColor);
	/*inline XBOOL LockWrite(_REFBLOCKDATA*&pBlock,XU32 nType)
	{
		switch(nType)
		{
		case _REFBLOCKDATA::_REFSIMPLE:
			 if(!pBlock||pBlock->nType!=nType||pBlock->nRefCount>1)
			 {
				 _REFBLOCKDATA*pNewBlock=CreateColorBlock(pBlock?pBlock->nData:0xff000000);
				 REF_SETBLOCK(pBlock,pNewBlock);
			 }
			 break;
		}
	}*/
};

#endif
#endif