// XDrawDib.h: interface for the XDrawDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDRAWDIB_H__5297594B_CF92_40E7_85A5_7F5B68D4B39F__INCLUDED_)
#define AFX_XDRAWDIB_H__5297594B_CF92_40E7_85A5_7F5B68D4B39F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#define OVERLAY(A,B,ALPHA) (XU8)((A*(0x100-ALPHA))>>8)+(((B*(ALPHA+1))>>8))
//#define SETALPHA(A,ALPHA) (XU8)(A+(((0x100-A)*ALPHA)>>8))
//#define OVERPIXEL(C1,C2,ALPHA) {XU8*pdd=(XU8*)(C1);\
//					pdd[1]=OVERLAY(pdd[1],C2[1],ALPHA);\
//					pdd[2]=OVERLAY(pdd[2],C2[2],ALPHA);\
//					pdd[0]=OVERLAY(pdd[0],C2[0],ALPHA);}
//class XCXForm;

enum {XRED_CHANNEL=1,XGREEN_CHANNEL=2,XBLUE_CHANNEL=4,XALPHA_CHANNEL=8};
#include "flashglobal.h"
#include "flashgeom.h"
#include "XDib.h"
#include "XImage.h"
//#include "sbitmap.h"	// Added by ClassView
//enum { bm8Bit, bm16Bit, bm32Bit };

#define XLIMIT(VV,MIN,MAX) {if(VV<MIN) VV=MIN;else if(VV>MAX) VV=MAX;}
enum PixelFormat {
//	pixFmt1,
//	pixFmt2,
//	pixFmt4,
	pixFmt8, //!< Indexed, 256 color
	pixFmt16, 	//!< Direct color, 555 RGB
//	pixFmt16A,
	pixFmt24,
	pixFmt32, 	//!< Direct color, 888 RGB
	pixFmt32A,
};
const XU8 pixelDepth[]={8,16,24,32,32};
const XU8 pixelSize[]={1,2,3,4,4};
#ifdef _ARGB_PIXEL_
typedef struct _XRGB8 {
	// Note the color order is determined by what the MMX code likes
	U8 red;
	U8 green;
	U8 blue;
	U8 alpha;
}RGB8;

typedef struct _XRGBI {	// an RGB struct that uses 16 integers for intermediate calculations like compositing
	// Note the color order is determined by what the MMX code likes
	U16 red;
	U16 green;
	U16 blue;
	U16 alpha;
}RGBI;
#else
typedef struct _XRGB8 {
	// Note the color order is determined by what the MMX code likes
	U8 blue;
	U8 green;
	U8 red;
	U8 alpha;
}RGB8;

typedef struct _XRGBI {	// an RGB struct that uses 16 integers for intermediate calculations like compositing
	// Note the color order is determined by what the MMX code likes
	U16 blue;
	U16 green;
	U16 red;
	U16 alpha;
}RGBI;
#endif
//#define _USE64BITS

#ifdef _USE64BITS
///typedef struct _XRGBIL
//{
//}
#define _MASK64	((XU64(0x00ff00ff) << 32) + 0x00ff00ff)

typedef union _XRGBIU {	// an RGB struct that uses 16 integers for intermedia calculations like compositing
	_XRGBI i;
	XU64   l;
}RGBIU;
#else
typedef struct _XRGBIL {	// a struct that can be overlaid over an RGBI for compositing operations
	U32 c0;
	U32 c1;
}RGBIL;

typedef union _XRGBIU {	// an RGB struct that uses 16 integers for intermedia calculations like compositing
	_XRGBI i;
	_XRGBIL l;
}RGBIU;
#endif
typedef struct _XCOLORTABLE {
	int n;
	RGB8 colors[256];
}SColorTable;

typedef struct _XCoverEntry {
  	int ce[4];
}CoverEntry;
extern CoverEntry PixCoverage[8][8];

class XCXForm;
class SBitmapCore;
class XRaster;
class XDrawDib : public XDib
{
public:
	//bool m_bInvalid;
	bool	     m_invalid;
	//SBitmapCore* m_pSbitmap;
	//SBitmapCore* GetBitmapCore();
public:
	void SwapRB();
	bool Resize(XSIZE&sz);
	XBOOL HasBits() { return m_pDib!=XNULL; }
	void LockBits() { lockCount++; }
	void UnlockBits() { lockCount--; }
	virtual void Release();
	XS32 lockCount;
	/*XU8  PixelFormat()
	{
		switch(m_nBits)
		{
		default:
		case 8:return pixFmt8;break;
		case 16:return pixFmt16;break;
		case 24:return pixFmt24;break;
		case 32:return pixFmt32;break;
		}
	}*/
	//XU8	 bmFormat;
public:
//#ifdef _WINEMU
	bool SaveFile(const char* strFile);
//#endif
	//SBitmapCore m_sbitmap;
	void ResetAlpha(XU8 alpha);
	XBOOL PreAlpha();
	XBOOL SetBitmapCore(SBitmapCore*bm);
	XDrawDib* Clone();
	XBOOL CreateFrom(SBitmapCore*bm,bool bCopy);
	XBOOL CreateFrom(XImage*pStream);
	XBOOL FromDib(XDib &dib, XIntArray &points, XRect &clip);
	void Draw(XDib&dib,MATRIX*pMatrix,XCXForm*pForm,XRect&rec,XBOOL bObj);
	void GetSSRGBPixel(SFIXED xH, SFIXED yH, _XRGBI* c);
	void GetRGBPixel(int x, int y, _XRGBI* c);
	int PixelFormat();
	int Threshold(XDib&dib,XRect&rect,XPoint&dst,XU8 nOpt,XU32 thre,XU32 nMask,XU32 nColor,XU8 bCopy);
	void Scroll(int x,int y);
	int Dissolve(XRect&rect,XPoint&dst,int nSeed,int nPixels,XU32 nColor);
	int Dissolve(XDib&dib,XRect&rect,XPoint&dst,int nSeed,int nPixels);
	void PaletteMap(XDib&dib,XRect&rect,XPoint&dst,XU8Array&reds,XU8Array&greens,XU8Array&blues,XU8Array&alphas);
	void Noise(int nLow,int nHigh,XU8 nOption,XBOOL bGray);
	void Mix(XDib&dib,XRect&rect,XPoint&dst,int rm,int gm,int bm,int am);
	XBOOL HitTest(XRect&rect,int alpha);
	void SetPixel(int x,int y,XU32 nColor,XU8 b32);
	XU32 GetPixel(int x,int y);
	void GetColorBounds(XRect&rect,XU32 mask,XU32 color,XBOOL bFind);
	void FloodFill(int x,int y,XU32 nColor);
	bool CopyChannel(XDib&dib,XRect&rect,XPoint&dst,XU8  sc,XU8 dc);
	void preAlpha();
	void Transform(XRect&rect,XCXForm&form);
	void FromDib(XDib&dib);
	void FormatRect(XS32&x,XS32&y,XRect&rect,int w,int h);
	void Copy(XDib&dib,XS32 x,XS32 y,XRect&rect,XDib&alphaDib,XS32 ax,XS32 ay);
	void Copy(SBitmapCore&bits,XS32 x,XS32 y,XRect&rect);
	void Overlay(SBitmapCore&bits,XS32 x,XS32 y,XRect&rect);
	void SetBackground(XU32 nColor);
	//void FillPoly(XXFILLDATA&fill,XPaintDib&paint,XFillStyleArray&style);
	enum {XDATA_ZLIB,XDATA_JPEG,XDATA_PNG,XDATA_GIF};
	void Clear(XRect&rect,XU32 color);
	void Clear(XU32 color);
	void Copy(XDib&dib,XS32 x,XS32 y,XRect&rect);

	XBOOL Create(XImage&image,int nFrame);
	void Clear(XRect&rect);
	void Overlay(XDib&dib,XS32 x,XS32 y,XRect&rect);
//#if (__CORE_VERSION__>=0x02072000)
	void Overlay(SBitmapCore &bits,XS32 x,XS32 y,XRect&rect,XDib&alphaDib);
//#endif
	void FillRect(XRect&rect,XU32 nColor,XU8 bOverlay);
	XBOOL Create(int w,int h,int nBits=32,XINT nPals=0,_XCOLOR*colors=XNULL);
	//void SetPaint(XPaintDib&dib,XPaintDib&mask,int x,int y,XRECT&rect,XU32Array&colors);
	XDrawDib();
	virtual ~XDrawDib();
	XRect m_clip;
	void  DivDib(int nDiv);
	XBOOL IsAttach(){return m_bAttach;}
	void SetAttach(bool a){m_bAttach = a;}
	void CreateData();
	XBOOL IsBlankAll();
protected:
	void CalcRect(XDib &dib, int id, int &bx, int &by, int &dx, int &dy, XU8 bLeft);
	XBOOL CalcDDX(int&id, int nDelta,XIntArray&points,int&dx,int&dy,int&by,int&ey);
	XBOOL m_bAttach;
	void ClipRect(XS32&x,XS32&y,XRECT&rect);
};

struct _DIBINFO
{
	XDrawDib* pDib;
	bool  isAttach;
	struct	_XSCharacter* ch;
	int   nType;
	XU32  nLastTime;
	XU32  nUseTime;
	XU32  nCacheTime;
	int	  nLockCount;
	_DIBINFO*pNext;
};

#endif // !defined(AFX_XDRAWDIB_H__5297594B_CF92_40E7_85A5_7F5B68D4B39F__INCLUDED_)
