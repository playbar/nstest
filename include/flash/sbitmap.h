/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright � Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc. 
****************************************************************************/

#ifndef SBITMAP_INCLUDED
#define SBITMAP_INCLUDED

#include "XDrawDib.h"
class XSWFPlayer;
enum { bm1Bit, bm2Bit, bm4Bit, bm8Bit, bm16Bit, bm32Bit };	// the four bitmap formats we support
#define MAXTEXTURE 4096//2048
class SBitmapCore
{
#ifdef _WINEMU
public:
	static int GetBitmapCount();
	static SBitmapCore* GetBitmapByID(int id);
#endif
public:
	void* m_pCH;
	static XSWFPlayer* m_pPlayer;
	XU32   m_nLockTime;
	//XU32	m_nMaxCacheTime;
	SBitmapCore* m_pNext;
	//void*		 m_pBitmapData;
	//XDrawDib*	 m_pDib;
	// Out bitmap structure
	char* baseAddr,*pAlloc;
	SColorTable* cTab;
	struct _TEXTUREID* bitmapID;
	//int bitmapID;
	int bmFormat;
	//bool isAttach;
	int width, height,width0,height0;
	S32 rowBytes;
	int useCount;
	//int lockCount,useCount;
	int nImgDiv;
	bool bAdded;
	//int nBlendMode;
	int transparent;	// set if this bitmap has any transparent alpha values, this is a hint that should be kept up to date
	XBOOL preAlpha,attachDib;
	void CalcTrans();
	//void SetBlendMode(int nMode);
	/*XU16* lineAlpha;
	void BuildLineAlpha();
	inline XBOOL IsTransparent(int xmin,int xmax,int y)
	{
		if(!lineAlpha) return XTRUE;
		y<<=1;
		if(xmin<lineAlpha[y]||xmax>lineAlpha[y+1]) return XTRUE;
		return XFALSE;
	}*/
public:
	void FormatBitmap();
	void GetColorBounds(XRect&rect,XU32 mask,XU32 color,XBOOL bFind);
	void AddUse()
	{
		useCount++;
	}
	void ReleaseUse()
	{
		useCount--;
	}
	void CreateTexture(bool bAdd=true);
	void CalcImageDiv(int w,int h);
	void DivImage();
	void PICreate(XDib&dib,XBOOL bTrans,void*pData);
	void PICreate(SBitmapCore*bits);
	XBOOL CreateEmpty();
	void SetDib(XDib&dib);
	XBOOL PICreate(void*pData,int nSize,XU8Array*pTab);
	XBOOL PICreate(XStream*pStream);
	XBOOL PICreate(int w,int h,int nDiv);
	void GetRGBPixel(int x, int y, RGBI* c);
	
	void GetSSRGBPixel(SFIXED x, SFIXED y, RGBI* c);
	void GetRGBPixel8(int x, int y, RGB8* c);
	void GetLineData(int x,int y,int ex,RGB8*c);
	void GetLineData2(int x,int y,int ex,RGB8*c);
	void GetHorzData(int x,int y,int ey,RGB8*c);
	void GetHorzDataR(int x,int y,int ey,RGB8*c);
	void GetData(int x,int y,RGB8*c,int n,int dx,int dy);
	void GetLineDataR(int x,int y,int ex,RGB8*c);
	
	void GetSSRGBPixel8(SFIXED x, SFIXED y, RGB8* c)
	{
		RGBI pix;
		GetSSRGBPixel(x,y,&pix);
		c->alpha=pix.alpha;
		c->red=pix.red;
		c->green=pix.green;
		c->blue=pix.blue;
		//*((XU32*)c)=PackPix32(&pix);
	}

	BOOL HasBits() { return baseAddr != 0; }
	void LockBits();
	void UnlockBits();

	// Create a platform independent bitmap
	void PIInit(int nDiv=0);
	BOOL CopyFrom(SBitmapCore*bits);
	BOOL PICreate(U8*pData,int length);
	BOOL PICreate(int format, int nColors,int w, int h, XStream&stream,XBOOL alpha);
	XBOOL PICreate();
	void PIFree();
	void ReleaseData();
	void SwapRB();
//#if (__CORE_VERSION__>=0x02075000)
//	//void SetBlockDiv(int w,int h);
//#endif
};

int SBitmapCalcRowbytes(int bmFormat, int width);
int SBitmapBitsPerPixel(int bmFormat);

#ifdef BIG_ENDIAN
inline void SBitmapSwapBits(void* data, S32 bytes, int bmFormat) {}
#else
void SBitmapSwapBits(void* data, S32 bytes, int bmFormat);
#endif


//
// Pixel packing and unpacking routines
//

inline void ExpandColor(RGB8 src, RGBI* dst)
{
	dst->alpha = src.alpha;
	dst->red   = src.red;
	dst->green = src.green;
	dst->blue  = src.blue;
}

inline void PackColor(RGBI* src, RGB8* dst)
{
	dst->alpha = (U8)src->alpha;
	dst->red   = (U8)src->red;
	dst->green = (U8)src->green;
	dst->blue  = (U8)src->blue;
}

inline U16 PackPix16(RGBI* c)
{
	return 	(c->red   & 0xF8) << 7  | 
			(c->green & 0xF8) << 2  | 
			(c->blue  /*& 0xF8*/) >> 3;
}

inline U16 PackPix16A(RGBI* c)
{
	return 	(c->red   & 0xF8) << 8  | 
			(c->green & 0xFC) << 3  | 
			(c->blue  /*& 0xF8*/) >> 3;
}

#if defined(_UNIX) && !defined(GENERATOR) 

extern int g_redShift, g_greenShift, g_blueShift;

inline U32 PackPix32(RGBI* c)
{
  return (U32) ((c->blue  << g_blueShift) |
		(c->green << g_greenShift) |
		(c->red   << g_redShift));
}

inline U32 PackPix32A(RGBI* c, int alpha)
{
  return (U32) ((c->blue  << g_blueShift) |
		(c->green << g_greenShift) |
		(c->red   << g_redShift));
}

#else
#ifdef _ARGB_PIXEL_

inline U32 PackPix32(RGBI* c)
{
	return 	(U32)(c->alpha /*& 0xFF*/) << 24 |
			(U32)(c->red   /*& 0xFF*/)		 | 
			(U32)(c->green /*& 0xFF*/) << 8  | 
				 (c->blue  /*& 0xFF*/) << 16;
}

inline U32 PackPix32A(RGBI* c, int /*alpha*/)
{
	return 	(U32)(c->alpha /*& 0xFF*/) << 24 |
			(U32)(c->red   /*& 0xFF*/)		 | 
			(U32)(c->green /*& 0xFF*/) << 8  | 
				 (c->blue  /*& 0xFF*/) << 16;
}

#else
inline U32 PackPix32(RGBI* c)
{
	return 	(U32)(c->alpha /*& 0xFF*/) << 24 |
			(U32)(c->red   /*& 0xFF*/) << 16 | 
			(U32)(c->green /*& 0xFF*/) << 8  | 
				 (c->blue  /*& 0xFF*/);
}

inline U32 PackPix32A(RGBI* c, int /*alpha*/)
{
	return 	(U32)(c->alpha /*& 0xFF*/) << 24 |
			(U32)(c->red   /*& 0xFF*/) << 16 | 
			(U32)(c->green /*& 0xFF*/) << 8  | 
				 (c->blue  /*& 0xFF*/);
}
#endif
#endif

extern const U32 pix16ExpandR[32];
extern const U32 pix16ExpandG[32];
extern const int pix16Expand[32];	// use this for blue
inline void UnpackPix16(U32 pix, RGBI* c)
{
	c->alpha = 255;
	c->red   = (U16)(pix16Expand[(pix>>10)&0x1f]);
	c->green = (U16)(pix16Expand[(pix>> 5)&0x1f]);
	c->blue  = (U16)(pix16Expand[(pix    )&0x1f]);
}

#if defined(_UNIX) && !defined(GENERATOR)
inline U32 Pix16To32(int pix)
{
	return (pix16Expand[(pix>>10)&0x1f]<<g_redShift) |
		   (pix16Expand[(pix>> 5)&0x1f]<<g_greenShift) |
		   (pix16Expand[(pix    )&0x1f]<<g_blueShift);
}
#else
inline U32 Pix16To32(int pix)
{
	return pix16ExpandR[(pix>>10)&0x1f] |
		   pix16ExpandG[(pix>> 5)&0x1f] |
		   pix16Expand [(pix    )&0x1f];
}
#endif

inline void UnpackPix16A(U32 pix, RGBI* c)
{
	c->alpha = 255;
	c->red   = (U16)(pix16Expand[(pix>>11)&0x1f]);
	//c->green = pix16Expand[(pix>> 6)&0x1f];	// note we throw away the extra bit for simplicity
	int i = ((int)pix & 0x07E0) >> 3;
	c->green = i == 0 ? 0 : i | 0x03;
	c->blue  = (U16)(pix16Expand[(pix    )&0x1f]);
}

#ifdef _ARGB_PIXEL_
inline void UnpackPix32(U32 pix, RGBI* c)
{
	c->alpha = (U8)(pix >> 24);
	c->blue   = (U8)(pix >> 16);//&0xFF;//((U8*)&pix)[1];
	c->green = (U8)(pix >>  8);//&0xFF;//((U8*)&pix)[2];
	c->red  = (U8) pix       ;//&0xFF;//((U8*)&pix)[3];
}

inline void UnpackPix32A(U32 pix, RGBI* c, int* alpha)
{
	*alpha   = (U8)(pix >> 24);//&0xFF;//((U8*)&pix)[0];
	c->blue  = (U8)(pix >> 16);//&0xFF;//((U8*)&pix)[1];
	c->green = (U8)(pix >>  8);//&0xFF;//((U8*)&pix)[2];
	c->red	 = (U8) pix		  ;//&0xFF;//((U8*)&pix)[3];
}
#else
inline void UnpackPix32(U32 pix, RGBI* c)
{
	c->alpha = (U8)(pix >> 24);
	c->red   = (U8)(pix >> 16);//&0xFF;//((U8*)&pix)[1];
	c->green = (U8)(pix >>  8);//&0xFF;//((U8*)&pix)[2];
	c->blue  = (U8) pix       ;//&0xFF;//((U8*)&pix)[3];
}

inline void UnpackPix32A(U32 pix, RGBI* c, int* alpha)
{
	*alpha   = (U8)(pix >> 24);//&0xFF;//((U8*)&pix)[0];
	c->red   = (U8)(pix >> 16);//&0xFF;//((U8*)&pix)[1];
	c->green = (U8)(pix >>  8);//&0xFF;//((U8*)&pix)[2];
	c->blue  = (U8) pix		  ;//&0xFF;//((U8*)&pix)[3];
}

#endif
inline BOOL PreMulAlpha(RGBI* color)
{
	int alpha = color->alpha;
	if ( alpha < 255 ) {
		// Pre-multiply the color values
		alpha++;
		color->red   = alpha*color->red   >> 8;
		color->green = alpha*color->green >> 8;
		color->blue  = alpha*color->blue  >> 8;
		return true;
	} else {
		return false;
	}
}


#endif
