#ifndef _XGRAPHICS_
#define _XGRAPHICS_

#include "XImage.h"
#include "XData.h"
#include "XDib.h"
#include "GGType.h"	// Added by ClassView

#ifdef __ANDROID__
#include "ft2build.h"
#include FT_FREETYPE_H
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftoutln.h"
#endif

#define DCCOLOR(CC) ((CC&0xFF00)|((CC>>16)&0xff)|((CC&0xff)<<16))


enum {XBS_WFRAME=2,XBS_BFRAME=4,XBS_THICK=1,XBS_HHAND=8,XBS_VHAND=0x10,XBS_NOFILL=0x20};
#define XBUTCOLOR 0xffCDDBFD
enum {XFMT_WORDALIGN=0x01,XFMT_ONELINE=0x10,XFMT_NOWRAP=0x100};

#define XPI 3.1415926F
#define XANGLE(AA) ((float)AA*XPI/180)

#define XF_STANDARD 0x100
enum {XDLS_SOLID=0,XDLS_DASH,XDLS_DOT,XDLS_DASHDOT,XDLS_DASDOTDOT};

class _XEXT_CLASS XGraphics
{
public:
	enum {XF_BOLD=1,XF_ITALIC=2,XF_UNDERLINE=4,XF_STRIKE=8,
		  XF_UPLINE=0x10,XF_BLINK=0x40,
		  XF_UPDATE=0x80000000};
	enum {XS_SMALL=0,XS_NORMAL=0x1,XS_BIG=0x2,
		  XS_HEAD1=0x7,XS_HEAD2=0x6,XS_HEAD3=0x5,
		  XS_HEAD4=0x4,XS_HEAD5=0x3,XS_HEAD6=0x2};
public:
#ifdef __APPLE__
	void begin();
	void end();
	XGraphics(void* context, int width, int height);
	XBOOL DrawDIB(XDib &dib, const XPOINT &pt, const XRECT &rect, XBOOL isButton);
#endif
	void DrawImage(const XImage&img,const XRect&rect);
	void DrawImage(const XImage&img,const XPoint&point,int rotate=0);
	void DrawImage(const XImage&img,const XPoint&dst,const XRect&rst);
	void FillRectangle(const XRect&rect);
	void MeasureString(XPCWSTR pText,XSIZE&size,XINT nLengt=-1);
	void DrawString(XPCWSTR pText,const XPoint&pt,XINT nLength=-1);
	void DrawString(XPCWSTR pText,XINT x,XINT y,XINT nLength=-1);
	void DrawString(XPCWSTR pText,const XRect&rect,XINT nLength=-1);
	//XBOOL FromHandle(XHANDLE hHandle);
	//XBOOL FromImage(XImage&bmp);
	XGraphics();
	//XGraphics(XImage&bmp);
#ifdef __ANDROID__
	void SaveImage(char*pName );
	XImage *GetXImage( );
	void Reset();
	XBOOL DrawDIB(XDib &dib, const XPOINT &pt, const XRECT &rect, XBOOL isButton);
	void DrawImage(const XImage & img, const XRect&dstRect, const XRect &srcRect );
	void DrawImage( gdx2d_pixmap* pImgDst, const gdx2d_pixmap* pImgSrc,
			const XRect &drt, const XRect &srt );
	int MeasureChar( unsigned short ch );
	void FTDrawString(XPCWSTR pText,XINT x,XINT y,XINT nLength );

#endif
    
	XGraphics(XHANDLE hHandle);
	virtual ~XGraphics();
public:
	XBOOL DrawDIB(XDib&dib,const XPOINT&pt,const XRECT&rect);
	void GetBitmapData(void*pBuf,int nLength,XU8 bits=32);
	XU32 GetPixel(int x,int y);
	XBOOL Copy(XGraphics&g, const XPoint&pt, const XRect&src);
	void Create(int w,int h,XU8 nBits=32);
	XGraphics(int w,int h);
	void FillEllipse(const XRect&rect);
	void DrawEllipse(const XRect&rect);
	void DrawCheckBox(const XRect&rect,XU32 c,XU8 bCheck);
	void DrawRadio(const XRect&rect,XU32 nColor,XU8 bSelect);
	XBOOL Scroll(const XPoint&dst,const XRect&srt);

	void DrawBar(const XRect&rect,XU32 c,XU8 dc,XU8 bHorz=XTRUE);
	void GetClip(XRect&clip);
	void DrawFrame(const XRect&rect,XU32 c,XU8 s=2,XU8 bSet=XFALSE);
	XU16 FormatString(XPCWSTR pData,int l,int bx,int w,DOMPOSARRAY&list,XU8 bWord=XFMT_WORDALIGN,DOMPOS*pSpan=XNULL);
	void DrawCheck(const XRect&rect,XU32 c,XU32 c1);
	void DrawDirect(const XRect&rect,XU8 nDirect,XU32 c,XU8 bGray=XFALSE);
	void SetOrigin(XINT x,XINT y);
	void GetOrigin(XINT&x,XINT&y);
	void SetClip(const XRect&rect);
	//void GetClip(XRect&rect);
	void DrawButton(const XRect&rect,XU32 c,XU8 b=XBS_BFRAME|XBS_THICK);
	void DrawLine(int sx,int sy,int ex,int ey,XU8 w=1,XU8 nStyle=XDLS_SOLID);
	void FrameRectangle(const XRect&rect);

	XU32 GetBackColor(){return m_nBackColor;}
	//XU32 GetColor(){return m_nTextColor;}
	XU32 GetColor(){return m_nTextColor=(m_nTextColor >> 8 | m_nTextColor << 24);}
	XU32 SetColor(XU32 nColor);
	XU32 SetBackColor(XU32 nColor);
	XU32 GetFontStyle(){return m_nFontStyle;}
	XU32 GetFontSize(){return (m_nFontStyle>>8)&0xff;}
	XU32 SetBold(XBOOL b);
	XU32 SetItalic(XBOOL b);
	XU32 SetStrike(XBOOL b);
	XU32 SetUnderline(XBOOL b);
	XU32 SetFontStyle(XU32 ns);
	XU32 SetFontSize(XU8 nSize);
	void Release();

protected:

#ifdef _SYMBIAN
	void* m_pDC;
	pthread_t m_tId[KUseThreadNum];
	unsigned long m_tsdId[KUseThreadNum];
	CFont* GetFontP();
	bool SaveFontP(CFont* font);
#endif
	//XPOINT m_offset;
	void Init();

#ifdef __ANDROID__
	void GetFont();
	bool mbImgCreate;
	FT_Library mFTLibrary;
	FT_Face mFTFace;
	//int mOriginX;
	//int mOriginY;
	XPoint	m_origin;	//change by zhangchen
	XImage* m_pBitmap;
#else
	void* GetFont();
	XPoint m_origin;
	void* m_pGraphics;
	void* m_pBitmap;
	void* m_pFont;
	void* m_pRgn;
#endif
	//void* m_pCatched;
public:
	//void SetRotate(int r);
	void GetBitmapMask(int sx,int sy,XDib&dib);

	XRect m_clip;
	XU32  m_nFontStyle;
	XU16 m_nTextHeight;
	XU32 m_nBackColor;
	XU32 m_nTextColor;
	//XBrush   m_brush,m_backBrush;
	//XFont	 m_font;

#ifdef __APPLE__

	void clipToRectInGlobal(const XRect& rect);
	void SetOffsetForClipErrorAdjust(int x, int y);
	void* getIphoneContext()
	{
		return m_pGraphics;
	}
	float m_backColor[4];
	float m_textColor[4];
	bool m_isPushContextForClip;
	int m_originXWhenPushContextForClip;
	int m_originYWhenPushContextForClip;

	int m_width;
	int m_height;
	int m_originX;
	int m_originY;
//	std::stack<CGPoint> m_originStack;
#endif

};


#endif // !defined(AFX_XGRAPHICS_H__19745F4D_CAD0_44C4_961C_5908DE594F31__INCLUDED_)
