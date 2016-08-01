// XDib.cpp: implementation of the XDib class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XDib.h"
#include "XGraphics.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XDib::XDib()
{
	m_pDib=0;
	m_nWidth=0;
	m_nHeight=0;
	m_nBits=0;
	m_nPixelSize=0;
	m_nColorTabs=0;
	m_nDataLength=0;
	m_nMemSize = 0;
}

XDib::~XDib()
{
    Release();
}

XU8* XDib::GetBuffer()
{
	if(!m_nColorTabs)
		return m_pDib;
	return 
		m_pDib+((m_nColorTabs+1)<<2);
}

XU32 XDib::GetDataLength()
{
	return m_nDataLength;
}

void XDib::Release()
{
	//if(m_nWidth*m_nHeight>1024*1012)
	//	int vv=0;
	if(m_pDib!=XNULL) 
		delete []m_pDib;
		//::free(m_pDib);
		//delete []m_pDib;
	m_pDib=XNULL;
	m_nWidth=0;
	m_nHeight=0;
}

XBOOL XDib::Create(int w, int h, XU8 nBits,XU8 colors)
{
	//return XFALSE;
	/*if(w==83&&h==24)
		int v=0;
	if(w==166&&h==24)
		int v=0;*/
	//if(w*h>1024*1012)
	//	int vv=0;
	if(nBits<=8) nBits=8;
	else if(nBits<=16) nBits=16;
	else if(nBits<=24) nBits=24;
	else nBits=32;
	m_nPixelSize=nBits>>3;
	m_nLinePitch=((w*m_nPixelSize+3)>>2)<<2;
	Release();
	m_nWidth=w;
	m_nHeight=h;
	m_nBits=nBits;
	m_nDataLength=m_nLinePitch*h;
	if(nBits<=8)
	{
		m_nColorTabs=colors;
		m_nDataLength+=(colors+1)<<2;		
	}
	m_pDib=new XU8[m_nDataLength];
	m_nMemSize=m_nDataLength;
		//alloc(new XU8[m_nDataLength];
	Reset();
	return m_pDib!=XNULL;	
}

void XDib::Reset()
{
	if(m_pDib==XNULL) return;
	memset(m_pDib,0,m_nDataLength);
}

void XDib::Paint(XHANDLE hHandle, int x, int y, const XRect &rect)
{
#ifdef _SYMBIAN

#elif defined(_WIN32)
	BWBITMAPINFOHEADER m_header;
	BWBITMAPINFOHEADER* pHeader=&m_header;

		//m_pColor  =  m_pMemory + headSize + nSize;			
	pHeader -> biSize = sizeof( BWBITMAPINFOHEADER );
    pHeader -> biWidth = m_nWidth;
    pHeader -> biHeight = -(LONG)m_nHeight;
    pHeader -> biPlanes = 1;
    pHeader -> biBitCount = m_nBits;
    pHeader -> biCompression = BI_RGB;
    pHeader -> biSizeImage = m_nLinePitch*m_nHeight;//m_nWidth*m_nHeight<<2;
    pHeader -> biClrImportant = 0;
	pHeader -> biClrUsed = 0;
	pHeader -> biXPelsPerMeter=3780;
	pHeader -> biYPelsPerMeter=3780;

	//(rect.top!=0)
	//nt a=0;

	::SetDIBitsToDevice((HDC)hHandle,                    // hDC
						x,//rect.left,             // DestX
						y,//rect.top,              // DestY
						rect.Width(  ),
						rect.Height(  ),
						rect.left,
						-m_header.biHeight-rect.bottom,
						0,//rect.top,                          // nStartScan
						-m_header.biHeight,
						GetBuffer(),
						(LPBITMAPINFO)&m_header,     // lpBitsInfo
						DIB_RGB_COLORS);
#endif
}

XU32 XDib::GetPixel(int x, int y)
{
	if(x<0||y<0||x>=(int)m_nWidth||y>=(int)m_nWidth) return 0;
		return 0;
	switch(m_nBits)
	{
	case 8:
		{
			XU8 id=*(GetBuffer()+y*m_nLinePitch+x);
			if(id>m_nColorTabs) return 0;
			return GetColorTable()[id];
		}break;
	case 16:
		{
			XU16*p=(XU16*)(GetBuffer()+m_nLinePitch*y);
			p+=x;
			XU16 c=*p;
			XU32 color=0;
			COLOR16TO32(c,color);
			return color;
		}
		break;
	case 24:
		{
		  XU8*p=(XU8*)(GetBuffer()+m_nLinePitch*y);
		  p+=x*3;
		  XU32 c;
		  COLOR24TO32(p,c);
		  return c;
		}
		break;
	case 32:
		{
		  XU32*p=(XU32*)(GetBuffer()+m_nLinePitch*y);
		  p+=x;
		  return *p;
		}
		break;
	}
}

XBOOL XDib::ResetSize(int w, int h)
{
	//return;
	int t=w*h*4;
	if(t>m_nMemSize)
	{
		return Create(w,h,32,0);
	}
	else
	{
		//return;
		m_nWidth=w;
		m_nHeight=h;
		m_nLinePitch=m_nWidth*4;
		m_nDataLength=w*h*4;
		memset(m_pDib,0,m_nDataLength);
	}
	return true;
}

void XDib::Textout(XString16&strTxt, XRect &bounds, XU8 fontHeight,XU8 nStyle,XU8 align)
{
	if(IsEmpty()||m_nBits!=8) return;
	
	int bx=0,by=0;
	int w0=bounds.Width();//,fh0=fontHeight;//,h0=bounds.Height();
	if(bounds.left<0)
		bx=bounds.left;
	if(bounds.top<0)
		by=bounds.top;
	bounds&=XRect(0,0,m_nWidth,m_nHeight);
	if(bounds.IsRectEmpty()) return;
	int w=bounds.Width(),h=bounds.Height();
	XU8*pDst=m_pDib+bounds.left+bounds.top*m_nWidth;
	for(int y=bounds.top;y<bounds.bottom;y++)
	{
		memset(pDst,0,w);
		pDst+=m_nWidth;
	}
	
	XGraphics g;
	g.Create(w,h);
	g.SetColor(0xff000000);
	//g.FillRectangle(bounds);
	fontHeight=fontHeight>>3;

	if(fontHeight>XGraphics::XS_HEAD1)
		fontHeight=XGraphics::XS_HEAD1;
	g.SetColor(0xffffffff);
	g.SetFontStyle(nStyle);
	g.SetBackColor(0);
	g.SetFontSize(fontHeight);
	
	DOMPOSARRAY list;
	int hi=g.FormatString(strTxt,strTxt.GetLength(),0,w,list);
	//int ddh=hi*(list.GetSize()/3);
	//if(ddh<h0) by+=(h0-ddh)>>1;
	//if(hi<fh0)
	//	by+=(fh0-hi);
	
	XPCWSTR str=strTxt;
	for(int i=0;i<(int)list.GetSize();i+=3)
	{
		//int iLen=i+3<(int)list.GetSize()?
		//		 list[i+4]-list[i+1]:strTxt.GetLength()-list[i];
		if(by+hi>=0)
		{
			int ddx=0;
			switch(align)
			{
			case 1://Left
				 ddx=w0-list[2];break;
			case 2://center
				ddx=(w0-list[2])>>1;
				break;
			}
			g.DrawString(str+list[i],bx+ddx,by,list[i+1]);
		}
		//str+=iLen;
		by+=hi;
		if(by>=h) break;
	}
	//g.FillRectangle(bounds);
	//g.DrawEllipse(bounds);
	g.GetBitmapMask(bounds.left,bounds.top,*this);
	//for(i=0;i<iLine*h;i++)
	//	if(m_pDib[i]!=0)
	//		int v=0;
//	XU8*pDst=m_pDib+(bounds.bottom-1)*m_nLinePitch+bounds.left*iBytes;
//	XU8*pSrt=m_pDib+(bounds.bottom-1)*iLine;
	/*XU8*pBuf=new XU8[iLine];
	if(pBuf)
	{
		for(int i=0;i<h;i++)
		{
		  memcpy(pBuf,pSrt,iLine);
		  memcpy(pDst,pBuf,iLine);
		  pSrt-=iLine;
		  pDst-=m_nLinePitch;
		}
		delete pBuf;
	}*/
}



//DEL void XDib::Noise(int nLow, int nHigh, XU8 nOption, XBOOL bGray)
//DEL {
//DEL 	if(IsEmpty()) return;
//DEL }
