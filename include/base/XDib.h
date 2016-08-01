// XDib.h: interface for the XDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDIB_H__84FD17EC_74F3_48DB_8291_2874A4898DBB__INCLUDED_)
#define AFX_XDIB_H__84FD17EC_74F3_48DB_8291_2874A4898DBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GGType.h"
#include "XData.h"
#include "XString.h"
class XGraphics;
#ifdef __ANDROID__
	#define COLOR16TO32(C16,C32) {C32=0;if(C16&0x8000) C32|=0xff000000;\
								  C32|=(((C16>>11) &0x1f)<<3)<<16;\
								  C32|=(((C16>>5 ) &0x3f)<<2)<<8;\
								  C32|=(((C16   ) &0x1f)<<3);}
	#define COLOR24TO32(C24,C32) (C32=0xff000000|(C24[2]<<16)|(C24[1]<<8)|C24[0])
#else
	#define COLOR16TO32(C16,C32) {C32=0;if(C16&0x8000) C32|=0xff000000;\
								  C32|=((((C16>>10)&0x3f)<<3)<<16);\
								  C32|=((((C16>>5)&0x3f)<<3)<<8);\
								  C32|=((((C16)&0x3f)<<3));}
	#define COLOR24TO32(C24,C32) (C32=0xff000000|(C24[2]<<16)|(C24[1]<<8)|C24[0])

#endif
#define SWAPPIXELRB(COLOR) {XU8 n=((XU8*)&COLOR)[0];((XU8*)&COLOR)[0]=((XU8*)&COLOR)[2];((XU8*)&COLOR)[2]=n;}


#pragma pack(1)
typedef struct tagBWBITMAPFILEHEADER {
	XU16    bfType;
	XU32    bfSize;
	XU16    bfReserved1;
	XU16    bfReserved2;
	XU32    bfOffBits;
} BWBITMAPFILEHEADER; //FAR *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBWBITMAPINFOHEADER{
	XU32     biSize;
	XU32     biWidth;
	XU32     biHeight;
	XU16     biPlanes;
	XU16     biBitCount;
	XU32     biCompression;
	XU32     biSizeImage;
	XU32     biXPelsPerMeter;
	XU32     biYPelsPerMeter;
	XU32     biClrUsed;
	XU32     biClrImportant;
} BWBITMAPINFOHEADER; //FAR *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;
#pragma pack()

class _XEXT_CLASS XDib  
{
public:
	enum {BITS8=8,BITS16=16,BITS32=32};

public:	
	XBOOL ResetSize(int w,int h);
	void Textout(XString16&strTxt,XRect&bounds,XU8 fontHeight,XU8 nStyle,XU8 align);
	XU32 GetPixel(int x,int y);
	void Paint(XHANDLE hHandle,int x,int y,const XRect&rect);
	void Reset();

	XBOOL Create(int w,int h,XU8 nBits=32,XU8 colortab=0);
	virtual void Release();
	XDib();
	virtual ~XDib();
	XU32 GetLinePitch(){return m_nLinePitch;}
	XU8	 GetBits(){return m_nBits;}
	XU8* GetBuffer();	
	XU8* GetAddress(){return m_pDib;}
	XU32* GetColorTable()
	{
		if(!m_nColorTabs) return XNULL;
		return (XU32*)m_pDib;
	}
	XU32 GetDataLength();
	XU32 Width(){return m_nWidth;}
	XU32 Height(){return m_nHeight;}
	XBOOL IsEmpty(){return m_pDib==XNULL;}
public:
	XU8* m_pDib;
	XU32 m_nWidth,m_nHeight,m_nDataLength,m_nLinePitch;
	XU8	 m_nBits,m_nPixelSize;
	XU32 m_nMemSize;
	XU8  m_nColorTabs;
};

#endif // !defined(AFX_XDIB_H__84FD17EC_74F3_48DB_8291_2874A4898DBB__INCLUDED_)
