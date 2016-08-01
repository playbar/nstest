#include "StdAfxGGBase.h"
#include "XFontDraw.h"
#include "XString.h"
//#include <math.h>

XFontDraw* XFontDraw::m_pInstance = NULL;
#define _FONTBLOCK 256*256*4
#define _FONTALPHABLOCK 256*256
static XU8 _fontData[_FONTBLOCK];
static XU8 _fontAlphaData[_FONTALPHABLOCK];
XFontDraw::XFontDraw()
{
	m_pFont = new CFont();
	m_fontSize = 0;
	m_fontID = 0;
}

XFontDraw::~XFontDraw()
{
	if(m_pFont)
	{
		CFont* pFont=(CFont*)m_pFont;
		pFont->DeleteObject();
		delete (CFont*)m_pFont;
	}
}

void XFontDraw::SetFont(const char* fontName,int id,int nSize)
{
   if(m_fontID == id&&m_fontSize==nSize) return;
   XString16 str(fontName);
   //str.ConvertToGB();
   //str.ConvertFrom(fontName,XCHARSET_UTF8);
   m_fontID = id;
   CFont* pFont=(CFont*)m_pFont;
   pFont->DeleteObject();
   m_fontSize = nSize;
   pFont->CreateFontW(nSize,nSize>>1,
			0,//(m_nFontStyle&XF_ITALIC)?10:0,
			0,
			FW_NORMAL,
			0,
			0,
			0,
			DEFAULT_CHARSET,//GB2312_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH,
			(LPCTSTR)str.GetData()); 

}

bool XFontDraw::Draw(XPCWSTR str, int len,_FONTINFO&info)
{
	if(m_fontSize<=0) return false;
	CWindowDC dc(NULL);	
	CFont* pFont=dc.SelectObject((CFont*)m_pFont);
	CSize sz=dc.GetTextExtent(str,len);
	dc.SelectObject(pFont);
	if(sz.cx<=0) return false;
	info.fw = sz.cx;
	sz.cx = (sz.cx+3)/4*4;
	sz.cy = m_fontSize;

	CDC drawDC;
	drawDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,sz.cx,sz.cy);
//	bitmap.SetBitmapDimension(sz.cx,sz.cy);
	CBitmap* pBmp = drawDC.SelectObject(&bitmap);
	pFont = drawDC.SelectObject((CFont*)m_pFont);
	drawDC.SetBkMode(TRANSPARENT);
	drawDC.SetTextColor(0xffffff);
	//CBrush brush;//(BS_SOLID,0x0);//BS_SOLID,0xff000000);	
	//brush.CreateSolidBrush(0xffffff);
	//drawDC.FillRect(CRect(0,0,sz.cx,sz.cy),&brush);	
	drawDC.TextOutW(0,0,str,len);
//	drawDC.SelectObject(pBmp);
//	CSize bs=bitmap.GetBitmapDimension();
	//BITMAP bf;
	//bitmap.GetObject(sizeof(bf),&bf);
	int l=sz.cx*sz.cy*4;
	bool bAlloc = false;
	XU8*pData = _fontData;
	if(l>_FONTBLOCK)
	{
		bAlloc = true;
	    pData=new XU8[l];
	}
	l=bitmap.GetBitmapBits(l,pData);
	info.w = sz.cx;
	info.h = sz.cy;
	l=sz.cx*sz.cy;
	if(l<_FONTALPHABLOCK)
	{
		info.pData = _fontAlphaData;
		info.bAlloc = false;
	}
	else
	{
		info.pData = new XU8[l];
		info.bAlloc = true;
	}
	XU8*pSrc=pData+1;
	XU8*pDst=info.pData;
	l=sz.cx*sz.cy;
	for(int i=0;i<l;i++)
	{
		pDst[i]=*pSrc;
		pSrc+=4;
	}
	//bitmap.GetBitmapBits(
	if(bAlloc)
		delete pData;
	drawDC.SelectObject(pBmp);
	drawDC.SelectObject(pFont);
	return true;
}
