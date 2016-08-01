// XDrawDib.cpp: implementation of the XDrawDib class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XDrawDib.h"
#include "sbitmap.h"
#include "Raster_gl.h"
#include "sbitmap.h"
#ifdef _WINEMU
	#include "splayer.h"
	#include "AVM2.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*inline void OVERRGB8(RGB8*DST,RGB8*SRC)
	{switch(SRC->alpha){
		case 0:break;
		case 0xff:*((XU32*)DST)=*((XU32*)SRC);break;
		default:int a1 = 256-SRC->alpha;
				DST->alpha=(SRC->alpha+(DST->alpha*a1>>8))&0xff;
				DST->red=(SRC->red+((DST->red-SRC->red)*a1>>8))&0xff;
				DST->green=(SRC->green+((DST->green-SRC->green)*a1>>8))&0xff;
				DST->blue=(SRC->blue+((DST->blue-SRC->blue)*a1>>8))&0xff;
				break;
	}}*/

inline void OVERRGB8(RGB8*DST,RGB8*SRC)
	{switch(SRC->alpha){
		case 0:break;
		case 0xff:*((XU32*)DST)=*((XU32*)SRC);break;
		default:int a1 = 256-SRC->alpha;
				DST->alpha=(SRC->alpha+(DST->alpha*a1>>8))&0xff;
				DST->red=(SRC->red+((DST->red)*a1>>8))&0xff;
				DST->green=(SRC->green+((DST->green)*a1>>8))&0xff;
				DST->blue=(SRC->blue+((DST->blue)*a1>>8))&0xff;
				break;
	}}


//#if (__CORE_VERSION__>=0x02072000)
inline void OVERRGB82(RGB8*DST,RGB8*SRC,RGB8*ALPHA)
	{switch(ALPHA->alpha){
		case 0:break;
		case 0xff:*((XU32*)DST)=*((XU32*)SRC);break;
		default:int a1 = 256-ALPHA->alpha;
				DST->alpha=(ALPHA->alpha+(DST->alpha*a1>>8))&0xff;
				DST->red=(SRC->red+((DST->red)*a1>>8))&0xff;
				DST->green=(SRC->green+((DST->green)*a1>>8))&0xff;
				DST->blue=(SRC->blue+((DST->blue)*a1>>8))&0xff;
				break;
	}}
//#endif
inline void OVERRGB8A(RGB8*DST,RGB8*SRC,int a)
	{
	RGB8 rgb;
	switch(a)
	{
	case 0:a=0;return;
	case 0xff:break;
	default:
		 rgb.alpha=(SRC->alpha*a)>>8;
		 rgb.red=(SRC->red*a)>>8;
		 rgb.blue=(SRC->blue*a)>>8;
		 rgb.green=(SRC->green*a)>>8;
		 SRC=&rgb;
		 //a=(a*SRC->alpha)>>8;
		 break;
	}
	switch(SRC->alpha)
	 {
		case 0:break;
		case 0xff:*((XU32*)DST)=*((XU32*)SRC);break;
		default:int a1 = 256-SRC->alpha;
				DST->alpha=(SRC->alpha+(DST->alpha*a1>>8))&0xff;
				DST->red=(SRC->red+((DST->red)*a1>>8))&0xff;
				DST->green=(SRC->green+((DST->green)*a1>>8))&0xff;
				DST->blue=(SRC->blue+((DST->blue)*a1>>8))&0xff;
				break;
	}
}


void XSWFRect::GetBoundRect(XRect&rect,MATRIX*pMat)
{
	if(pMat==XNULL) {rect=*this;return;}
	SPOINT pt[4];
	pt[0].x=left;
	pt[0].y=top;
	pt[1].x=right;
	pt[1].y=top;
	pt[2].x=right;
	pt[2].y=bottom;
	pt[3].x=left;
	pt[3].y=bottom;
	/*long nData[]=
	{
		left,top,
		right,top,
		right,bottom,
		left,bottom
	};*/
	::MatrixTransformPoint(pMat,&pt[0],&pt[0]);
	//pMat->Translate(nData[0],nData[1]);
	XRect rt(pt[0].x,pt[0].y,pt[0].x,pt[0].y);//nData[0],nData[1],nData[0],nData[1]);
	for(int i=1;i<4;i++)
	{
		::MatrixTransformPoint(pMat,&pt[i],&pt[i]);
		//pMat->Translate(nData[i],nData[i+1]);
		if(rt.left>pt[i].x) rt.left=pt[i].x;//nData[i];
		if(rt.top>pt[i].y) rt.top=pt[i].y;//nData[i+1];
		if(rt.right<pt[i].x) rt.right=pt[i].x;
		if(rt.bottom<pt[i].y) rt.bottom=pt[i].y;
	}
	rect|=rt;
}

XDrawDib::XDrawDib()
{
	//m_pSbitmap=XNULL;
	m_bAttach=XFALSE;
	lockCount=0;
	m_invalid=false;
	//RectSetEmpty(&m_invalidRect);
}

void XDrawDib::Release()
{
	if(m_bAttach)
		m_pDib=NULL;
#ifdef _WINEMU_BMTRACE
	if(m_pDib&&IsRunning())
	{
		XString16 s("[Memory] Free Bitmap memory ");
		XString16 t;
		t.FromInt(this->m_nDataLength);
		s+=t;
		s+=XString16(" Bytes");
		_tracePlayer->Trace(s);
	}
#endif
	XDib::Release();
	
	//m_pSbitmap=XNULL;
}

XDrawDib::~XDrawDib()
{
	if(m_bAttach)
		m_pDib=XNULL;
	/*if(m_pSbitmap)
	{
		m_pSbitmap->m_pDib=XNULL;
		delete m_pSbitmap;
	}
	m_pSbitmap=XNULL;*/
}


void  XDrawDib::DivDib(int nDiv)
{
	int nw=m_nWidth>>nDiv;
	int nh=m_nHeight>>nDiv;
	int nLine=m_nWidth<<(nDiv);
	if(nw<1) nw=1;
	if(nh<1) {nh=1;nLine=m_nWidth;}
	XU32*pSrt=(XU32*)this->m_pDib;
	XU8*pTmp=this->m_pDib;
	m_pDib=NULL;
	Create(nw,nh,32);
	XU32*pDst=(XU32*)m_pDib;
	int x,y;
	for(y=0;y<nh;y++)
	{
		for(x=0;x<nw;x++)
		{
			pDst[x]=pSrt[x<<nDiv];
		}
		pSrt+=nLine;
		pDst+=nw;
	}
	delete pTmp;
}
XBOOL XDrawDib::IsBlankAll()
{
	if(m_pDib==XNULL||m_nBits!=32) return XTRUE;
	XU32*pData=(XU32*)m_pDib;
	int l=m_nWidth*m_nHeight;
	while(l--)
	{
		if((*pData)>>24) 
			return XFALSE;
		pData++;
	}
	return XTRUE;

}
void XDrawDib::ClipRect(XS32 &x, XS32 &y, XRECT &rect)
{
    if(x<m_clip.left) {rect.left+=m_clip.left-x;x=m_clip.left;}
	if(y<m_clip.top) {rect.top+=m_clip.top-y;y=m_clip.top;}

	if(x+rect.right-rect.left>m_clip.right)
		rect.right=m_clip.right-x+rect.left;
	if(y+rect.bottom-rect.top>m_clip.bottom)
		rect.bottom=m_clip.bottom+rect.top-y;
}

//DEL void XDrawDib::Paint(XHANDLE hHandle,int x,int y, XRect &rect)
//DEL {
//DEL 	XGraphics g(hHandle);
//DEL 	g.DrawDIB(*this,XPoint(x,y),rect);
//DEL /*#ifdef _WINGDI_
//DEL 	BITMAPINFOHEADER m_header;
//DEL 	BITMAPINFOHEADER* pHeader=&m_header;
//DEL
//DEL 		//m_pColor  =  m_pMemory + headSize + nSize;
//DEL 	pHeader -> biSize = sizeof( BITMAPINFOHEADER );
//DEL     pHeader -> biWidth = m_nWidth;
//DEL     pHeader -> biHeight = -(LONG)m_nHeight;
//DEL     pHeader -> biPlanes = 1;
//DEL     pHeader -> biBitCount = 32;
//DEL     pHeader -> biCompression = BI_RGB;
//DEL     pHeader -> biSizeImage = m_nWidth*m_nHeight<<2;
//DEL     pHeader -> biClrImportant = 0;
//DEL 	pHeader -> biClrUsed = 0;
//DEL 	pHeader -> biXPelsPerMeter=3780;
//DEL 	pHeader -> biYPelsPerMeter=3780;
//DEL
//DEL 	//(rect.top!=0)
//DEL 	//nt a=0;
//DEL
//DEL 	::SetDIBitsToDevice((HDC)hHandle,                    // hDC
//DEL 						x,//rect.left,             // DestX
//DEL 						y,//rect.top,              // DestY
//DEL 						rect.Width(  ),
//DEL 						rect.Height(  ),
//DEL 						rect.left,
//DEL 						-m_header.biHeight-rect.bottom,
//DEL 						0,//rect.top,                          // nStartScan
//DEL 						-m_header.biHeight,
//DEL 						GetBuffer(),
//DEL 						(LPBITMAPINFO)&m_header,     // lpBitsInfo
//DEL 						DIB_RGB_COLORS);
//DEL #endif*/
//DEL
//DEL }

XBOOL XDrawDib::Create(int w, int h,int nBits,int nPals,_XCOLOR*colors)
{
#ifdef _WINEMU_BMTRACE
	if(m_pDib&&IsRunning())
	{
		XString16 s("[Memory] Free Bitmap memory ");
		XString16 t;
		t.FromInt(this->m_nDataLength);
		s+=t;
		s+=XString16(" Bytes");
		_tracePlayer->Trace(s);
	}
#endif
	if(!XDib::Create(w,h,nBits,nPals)) return XFALSE;
	m_clip.left=0;
	m_clip.top=0;
	m_clip.right=w;
	m_clip.bottom=h;
	if(colors)
	{
		XU32*pDst=(XU32*)m_pDib;
		for(int i=0;i<nPals;i++)
			pDst[i]=colors[i].nColor;
	}
#ifdef _WINEMU_BMTRACE
	if(IsRunning())
	{
		XString16 s("[Memory] Alloc Bitmap memory ");
		XString16 t;
		t.FromInt(this->m_nDataLength);
		s+=t;
		s+=XString16(" Bytes");
		_tracePlayer->Trace(s);
	}
#endif
	return  XTRUE;
}

#define _XChunkSize 256

void XDrawDib::FillRect(XRect &rect, XU32 nColor,XU8 bOverlay)
{
	//ClipRectX(rect);
	//return;
	if(IsEmpty()) return;
	if(m_nBits!=32) return;
	rect&=m_clip;
	if(rect.IsRectEmpty()) return;

	//XU8*pColor=(XU8*)&nColor;
	if((nColor>>24)>=0xff)
		bOverlay=false;
	else
	{
		RGB8*pColor=(RGB8*)&nColor;
		int a=pColor->alpha;
		pColor->red=(pColor->red*a)>>8;
		pColor->green=(pColor->green*a)>>8;
		pColor->blue=(pColor->blue*a)>>8;
	}
	if(bOverlay)
	{
		//int alpha=nColor>>24;
		RGB8*pd=(RGB8*)GetBuffer()+rect.top*m_nWidth;
		RGB8* pColor=(RGB8*)&nColor;
		for(int y=rect.top;y<rect.bottom;y++)
		{
			for(int x=rect.left;x<rect.right;x++)
			{
				OVERRGB8((pd+x),pColor);
			  //if(alpha>=0xff)
			  //	  *(pd+x)=nColor;
			  //else
			  //{
			  //	 OVERPIXEL(pd+x,pColor);//,alpha);
			  //}
			}
			pd+=m_nWidth;
		}
	}
	else
	{
		XU32*pd=(XU32*)GetBuffer()+rect.top*m_nWidth+rect.left;
		XU32 trunk[_XChunkSize];
		for(int i=0;i<_XChunkSize;i++)
			trunk[i]=nColor;
		/*int l=m_nWidth*m_nHeight;
		while(l)
		{
			int lc=XMIN(l,_XChunkSize);
			memcpy(pd,trunk,lc<<2);
			pd+=lc;
			l-=lc;
		}*/
		int len=rect.right-rect.left;
		for(int y=rect.top;y<rect.bottom;y++)
		{
			int tl=len;
			XU32*pDst=pd;
			while(tl)
			{
				int l=XMIN(tl,_XChunkSize);
				memcpy(pDst,trunk,l<<2);
				tl-=l;
				pDst+=l;
			}
			//int len=rect.right-rect.left;
			/*for(int x=rect.left;x<rect.right;x++)
			{
			  *(pd+x)=nColor;
			}*/
			pd+=m_nWidth;
		}
	}
	/*int bx=(rect.left>>2),ex=((rect.right)>>2);
	int by=(rect.top>>2),ey=((rect.bottom)>>2);
	XU32*pDst=(XU32*)GetBuffer()+by*m_nWidth;
	XU32*pd=pDst+m_nWidth;
	XU8*pColor=(XU8*)&nColor;
	int alpha=(XU8)(nColor>>24);
	for(int y=by+1;y<ey;y++)
	{
		for(int x=bx+1;x<ex;x++)
		{
		  if(alpha>=0xff)
			  *(pd+x)=nColor;
		  else
		  {
			 OVERPIXEL(pd+x,pColor,alpha);
		  }
		}
		pd+=m_nWidth;
	}
	int fl=4-(rect.left%4),ft=4-(rect.top%4);
	int fr=rect.right%4,fb=rect.bottom%4;

	if(ft!=0)
	{
		pd=pDst;
		int al=SETFLAG(alpha,ft*fl);
		OVERPIXEL(pd+bx,pColor,al);
		al=SETFLAG(alpha,ft*fr);
		OVERPIXEL(pd+ex,pColor,al);
		al=SETFLAG(alpha,(ft<<2));
		//int ab=SETFLAG(alpha,(fb<<2));
		for(int x=bx+1;x<ex;x++)
			OVERPIXEL(pd+x,pColor,al);
	}
	//if(fr!=0)
	{
		pd=pDst+m_nWidth;
		int al=SETFLAG(alpha,(fl<<2));
		int ar=SETFLAG(alpha,(fr<<2));
		for(y=by+1;y<ey;y++)
		{
			if(al)
				OVERPIXEL(pd+bx,pColor,al);
			if(ar)
				OVERPIXEL(pd+ex,pColor,ar);
			pd+=m_nWidth;
		}

	}
	if(fb!=0)
	{
		//pd=pDst+(ey-1)*m_nWidth;
		int al=SETFLAG(alpha,fb*fl);
		OVERPIXEL(pd+bx,pColor,al);
		al=SETFLAG(alpha,fb*fr);
		OVERPIXEL(pd+ex,pColor,al);
		al=SETFLAG(alpha,(fb<<2));

		for(int x=bx+1;x<ex;x++)
			OVERPIXEL(pd+x,pColor,al);
	}*/
}

//DEL void XDrawDib::ClipRectX(XRect &rect)
//DEL {
//DEL 	rect&=m_clip;
//DEL }

void XDrawDib::Overlay(XDib &dib, XS32 x, XS32 y, XRect &rect)
{
	if(m_nBits!=32&&dib.GetBits()!=32) return;
	if(IsEmpty()||dib.IsEmpty()) return;
	FormatRect(x,y,rect,dib.Width(),dib.Height());
	ClipRect(x,y,rect);
	int w=dib.Width();
	int dw=rect.right-rect.left;
	int dh=rect.bottom-rect.top;
	if(dw<=0||dh<=0) return;
	RGB8*pSrt=(RGB8*)dib.GetBuffer()+rect.top*w+rect.left;
	//XU8*pColor=(XU8*)&color;
	RGB8* pDst=(RGB8*)GetBuffer();
	//int alpha=color>>24,al;
	pDst+=y*m_nWidth+x;
	//this->Reset();
	//this->Clear(rect);
	//if(dw==320) return;
	for(y=0;y<dh;y++)
	{
		for(x=0;x<dw;x++)
		{
			/*int alpha=pSrt[x]>>24;
			if(alpha)
			{
				if(alpha==0xff) *(pDst+x)=*(pSrt+x);
				else
				{
					OVERPIXEL(pDst+x,((XU8*)(pSrt+x)),alpha);
				}
			}*/
			//OVERPIXEL(pDst+x,pSrt+x);
			OVERRGB8((pDst+x),(pSrt+x));
			//pSrt++;
		}
		pSrt+=w;
		pDst+=m_nWidth;
	}
}

void XDrawDib::ResetAlpha(XU8 alpha)
{
	if(this->m_nBits!=32) return;
	int l=m_nWidth*m_nHeight;
	RGB8*pDst=(RGB8*)m_pDib;
	//while(l>0)
	int i;
	for(i=0;i<l;i++)
	{
		//pDst+=4;
		pDst[i].alpha=alpha;
		//l--;
	}
}

void XDrawDib::Clear(XRect &rect)
{
	//ClipRectX(rect);
	if(m_nBits!=32) return;
	rect&=m_clip;
	if(rect.IsRectEmpty()) return;
	XU32*pd=(XU32*)GetBuffer()+rect.top*m_nWidth+rect.left;
	int l=(rect.right-rect.left)<<2;
	for(int y=rect.top;y<rect.bottom;y++)
	{
		XGlobal::Memset(pd,0,l);
		pd+=m_nWidth;
	}
}


XBOOL XDrawDib::Create(XImage &image,int nFrame)
{
	if(image.IsEmpty()) return XFALSE;
	int w=image.Width();
	int h=image.Height();

	image.SetActive(nFrame);
	XGraphics g;
	g.Create(w,h);
	XDib::Create(w,h,32);
	g.DrawImage(image,XPoint(0,0));
	g.GetBitmapData(GetBuffer(),w*h*4);
	return XTRUE;
}

void XDrawDib::Copy(XDib &dib, XS32 x, XS32 y, XRect &rect, XDib &alphaDib, XS32 ax, XS32 ay)
{
	if(IsEmpty()||dib.IsEmpty()) return;
	XRect r1=rect;
	//Copy(dib,x,y,rect);
	if(m_nBits!=32&&alphaDib.GetBits()!=32) return;
	//r1&=alphaDib.m_clip;
	FormatRect(x,y,r1,dib.Width(),dib.Height());
	FormatRect(ax,ay,r1,alphaDib.Width(),alphaDib.Height());
	ClipRect(x,y,r1);
	if(r1.IsRectEmpty()) return;
	RGB8*pDst=(RGB8*)GetBuffer();
	pDst+=(y*m_nWidth+x);
	//pDst+=ay*m_nWidth;
	RGB8*pAlpha=(RGB8*)alphaDib.GetBuffer();
	RGB8*pSrt=(RGB8*)dib.GetBuffer();
	int ww=alphaDib.Width();
	int ws=dib.Width();
	pSrt+=r1.top*ws+r1.left;
	pAlpha+=r1.top*ww+r1.left;
	int w=r1.Width();
	for(y=r1.top;y<r1.bottom;y++)
	{
		for(int x=0;x<w;x++)
		{
			OVERRGB8A((pDst+x),(pSrt+x),pAlpha[x].alpha);
		   //((XU8*)(&pDst[x]))[3]=((XU8*)(&pSrt[x]))[3];
		}
		//XGlobal::Memcpy(pDst,pSrt,l);
		pDst+=m_nWidth;
		pSrt+=ws;
		pAlpha+=ww;
	}

}

void XDrawDib::Copy(SBitmapCore &bits, XS32 x, XS32 y, XRect &rect)
{
	if(IsEmpty()||!bits.HasBits()) return;



	if(m_nBits!=32) return;

	/*if(bits.nImgDiv)
	{
		XRect r0=rect;
		int x0=x,y0=y;
		//if(x0==110&&y0==-15)
		//	int v=0;
		FormatRect(x,y,rect,bits.width<<bits.nImgDiv,bits.height<<bits.nImgDiv);
		ClipRect(x,y,rect);
		if(rect.IsRectEmpty()) return;
		//if(rect.top<0)
		//	int v=0;
		XU32*pDst=(XU32*)GetBuffer();
		pDst+=y*m_nWidth+x;
		//XU32*pSrt=(XU32*)dib.GetBuffer();
		//int ww=dib.Width();
		//pSrt+=rect.top*ww+rect.left;
		int l=rect.Width()<<2;
		for(y=rect.top;y<rect.bottom;y++)
		{
			//XGlobal::Memcpy(pDst,pSrt,l);
			bits.GetLineData2(rect.left,y,rect.right,(RGB8*)pDst);
			pDst+=m_nWidth;
			//pSrt+=ww;
		}
	}
	else*/
	{
		XRect r0=rect;
		int x0=x,y0=y;
		//if(x0==110&&y0==-15)
		//	int v=0;
		FormatRect(x,y,rect,bits.width,bits.height);
		ClipRect(x,y,rect);
		if(rect.IsRectEmpty()) return;
		//if(rect.top<0)
		//	int v=0;
		XU32*pDst=(XU32*)GetBuffer();
		pDst+=y*m_nWidth+x;
		//XU32*pSrt=(XU32*)dib.GetBuffer();
		//int ww=dib.Width();
		//pSrt+=rect.top*ww+rect.left;
		int l=rect.Width()<<2;
		for(y=rect.top;y<rect.bottom;y++)
		{
			//XGlobal::Memcpy(pDst,pSrt,l);
			bits.GetLineData(rect.left,y,rect.right,(RGB8*)pDst);
			pDst+=m_nWidth;
			//pSrt+=ww;
		}
	}
}

void XDrawDib::preAlpha()
{
	if(IsEmpty()) return;
	if(m_nBits!=32) return;
	RGB8*pBuf=(RGB8*)m_pDib;
	int size=m_nWidth*m_nHeight;
	for(int i=0;i<size;i++)
	{
		int a=pBuf[i].alpha;
		//if(a>=0xff) continue;
		switch(a)
		{
		case 0xff:break;
		case 0:
			//transparent=true;
			break;
		default:
			//transparent=true;
		//if(pBuf[i].red>a) pBuf[i].red=a;
		//if(pBuf[i].blue>a) pBuf[i].blue=a;
		//if(pBuf[i].green>a) pBuf[i].green=a;
			pBuf[i].red=(pBuf[i].red*a)>>8;
			pBuf[i].green=(pBuf[i].green*a)>>8;
			pBuf[i].blue=(pBuf[i].blue*a)>>8;
			break;
		}
	}
}

//#if (__CORE_VERSION__>=0x02072000)
void XDrawDib::Overlay(SBitmapCore &bits, XS32 x, XS32 y, XRect &rect,XDib&alphaDib)
{
	if(IsEmpty()||!bits.HasBits()) return;

	if(m_nBits!=32) return;
	{
		XRect r0=rect;
		int x0=x,y0=y;
		//if(x0==110&&y0==-15)
		//	int v=0;
		FormatRect(x,y,rect,bits.width,bits.height);
		FormatRect(x,y,rect,alphaDib.Width(),alphaDib.Height());
		ClipRect(x,y,rect);
		if(rect.IsRectEmpty()) return;
		//if(rect.top<0)
		//	int v=0;
		int w1=alphaDib.Width();
		RGB8*pDst=(RGB8*)GetBuffer();
		RGB8*pAlpha=(RGB8*)alphaDib.GetBuffer()+rect.top*w1+rect.left;
		pDst+=y*m_nWidth+x;
		//XU32*pSrt=(XU32*)dib.GetBuffer();
		//int ww=dib.Width();
		//pSrt+=rect.top*ww+rect.left;
		//int l=rect.Width();
		for(y=rect.top;y<rect.bottom;y++)
		{
			//XGlobal::Memcpy(pDst,pSrt,l);
			RGB8 tempBuf[256];
			int x=rect.left;
			int l=rect.right-rect.left;
			RGB8*pd=pDst;
			RGB8*pAD=pAlpha;
			while(x<rect.right)
			{
				int ll=XMIN(256,l);
				bits.GetLineData(x,y,x+ll,tempBuf);
				//RGB8* pw=(RGB8*)pDst;
				/*if(bits.preAlpha)
				{
					for(int i=0;i<ll;i++)
					{
						OVERRGB8P((pDst+i),(tempBuf+i));
					}
				}
				else*/
				{
					for(int i=0;i<ll;i++)
					{
						OVERRGB82((pd+i),(tempBuf+i),(pAD+i));
					}
				}
				l-=ll;
				x+=ll;
				pd+=ll;
				pAD+=ll;
			}
			//bits.GetLineData(rect.left,y,rect.right,(RGB8*)pDst);
			pAlpha+=w1;
			pDst+=m_nWidth;
			//pSrt+=ww;
		}
	}
}
//#endif

void XDrawDib::Overlay(SBitmapCore &bits, XS32 x, XS32 y, XRect &rect)
{
	if(IsEmpty()||!bits.HasBits())
		return;

	if(m_nBits!=32) 
		return;
	{
		XRect r0=rect;
		int x0=x,y0=y;
		//if(x0==110&&y0==-15)
		//	int v=0;
		FormatRect(x,y,rect,bits.width,bits.height);
		ClipRect(x,y,rect);
		if(rect.IsRectEmpty()) 
			return;
		//if(rect.top<0)
		//	int v=0;
		RGB8*pDst=(RGB8*)GetBuffer();
		pDst+=y*m_nWidth+x;
		//XU32*pSrt=(XU32*)dib.GetBuffer();
		//int ww=dib.Width();
		//pSrt+=rect.top*ww+rect.left;
		//int l=rect.Width();
		for(y=rect.top;y<rect.bottom;y++)
		{
			//XGlobal::Memcpy(pDst,pSrt,l);
			RGB8 tempBuf[256];
			int x=rect.left;
			int l=rect.right-rect.left;
			RGB8*pd=pDst;
			while(x<rect.right)
			{
				int ll=XMIN(256,l);
				bits.GetLineData(x,y,x+ll,tempBuf);
				//RGB8* pw=(RGB8*)pDst;
				/*if(bits.preAlpha)
				{
					for(int i=0;i<ll;i++)
					{
						OVERRGB8P((pDst+i),(tempBuf+i));
					}
				}
				else*/
				{
					for(int i=0;i<ll;i++)
					{
						OVERRGB8((pd+i),(tempBuf+i));
					}
				}
				l-=ll;
				x+=ll;
				pd+=ll;
			}
			//bits.GetLineData(rect.left,y,rect.right,(RGB8*)pDst);
			pDst+=m_nWidth;
			//pSrt+=ww;
		}
	}
}


void XDrawDib::Copy(XDib &dib, XS32 x, XS32 y, XRect &rect)
{
	if(IsEmpty()||dib.IsEmpty()) return;
	if(m_nBits!=32&&dib.GetBits()!=32) return;

	XRect r0=rect;
	int x0=x,y0=y;
	//if(x0==110&&y0==-15)
	//	int v=0;
	FormatRect(x,y,rect,dib.Width(),dib.Height());
	ClipRect(x,y,rect);
	if(rect.IsRectEmpty()) return;
	//if(rect.top<0)
	//	int v=0;
	XU32*pDst=(XU32*)GetBuffer();
	pDst+=y*m_nWidth+x;
	XU32*pSrt=(XU32*)dib.GetBuffer();
	int ww=dib.Width();
	pSrt+=rect.top*ww+rect.left;
	int l=rect.Width()<<2;
	for(y=rect.top;y<rect.bottom;y++)
	{
		XGlobal::Memcpy(pDst,pSrt,l);
		pDst+=m_nWidth;
		pSrt+=ww;
	}
}

void XDrawDib::Clear(XRect&rect,XU32 color)
{
	if(IsEmpty()) return;
	rect&=m_clip;
	if((color>>24)!=0xff)	
	{
		RGB8*pColor=(RGB8*)&color;
		int a=pColor->alpha;
		pColor->red=(pColor->red*a)>>8;
		pColor->green=(pColor->green*a)>>8;
		pColor->blue=(pColor->blue*a)>>8;
	}
	XU32*pd=(XU32*)GetBuffer();//+rect.top*m_nWidth;
	XU32 trunk[_XChunkSize];
	for(int i=0;i<_XChunkSize;i++)
		trunk[i]=color;
	int l=m_nWidth*m_nHeight;
	while(l)
	{
		int lc=XMIN(l,_XChunkSize);
		memcpy(pd,trunk,lc<<2);
		pd+=lc;
		l-=lc;
	}
	/*XU32*pd=(XU32*)GetBuffer()+rect.top*m_nWidth+rect.left;
	int l=(rect.right-rect.left);//*sizeof(XU32);
	for(int y=rect.top;y<rect.bottom;y++)
	{
		for(int x=0;x<l;x++)
			pd[x]=color;
		pd+=m_nWidth;
	}*/
}

void XDrawDib::Clear(XU32 color)
{
	if(IsEmpty()) return;	
	if((color>>24)!=0xff)	
	{
		RGB8*pColor=(RGB8*)&color;
		int a=pColor->alpha;
		pColor->red=(pColor->red*a)>>8;
		pColor->green=(pColor->green*a)>>8;
		pColor->blue=(pColor->blue*a)>>8;
	}
	XU32*pd=(XU32*)GetBuffer();//+rect.top*m_nWidth;
	XU32 trunk[_XChunkSize];
	for(int i=0;i<_XChunkSize;i++)
		trunk[i]=color;
	int l=m_nWidth*m_nHeight;
	while(l)
	{
		int lc=XMIN(l,_XChunkSize);
		memcpy(pd,trunk,lc<<2);
		pd+=lc;
		l-=lc;
	}
}



//DEL XBOOL XDrawDib::Create(int w, int h, XU8 nBits,XU8 colorTableCount,
//DEL 					   XStream *pStream, XU8 nType,XU8 alpha)
//DEL {
//DEL 	XZLib lib;
//DEL 	XStream out;
//DEL 	if(!lib.Init(pStream,&out)) return XFALSE;
//DEL 	switch(lib.Decode(XNULL))
//DEL 	{
//DEL 		case XDC_OK:
//DEL 		case XDC_FINISHED:break;
//DEL 		default:return XFALSE;
//DEL 	}
//DEL 	lib.Release();
//DEL 	if(!XDib::Create(w,h,32)) return XFALSE;//nBits,colorTableCount)) return XFALSE;
//DEL 	XU8*pTab=XNULL;//(XU8*)GetColorTable();
//DEL 	int nOffset=0;
//DEL 	if(nBits<=8)
//DEL 	{
//DEL 		colorTableCount=XMIN(256,colorTableCount);
//DEL 		//int nOffset=0;
//DEL 		pTab=new XU8[256<<2];
//DEL 		if(pTab==XNULL) return XFALSE;
//DEL 		if(pTab!=XNULL)
//DEL 		{
//DEL 			int s=alpha?4:3;
//DEL 			XU8*pSrt=out.BufferAddr();
//DEL 			XU8*pDst=pTab;
//DEL 			for(int i=0;i<=colorTableCount;i++)
//DEL 			{
//DEL 				pDst[2]=pSrt[0];
//DEL 				pDst[1]=pSrt[1];
//DEL 				pDst[0]=pSrt[2];
//DEL 				pDst[3]=alpha?pSrt[3]:0xff;
//DEL 				pDst+=4;
//DEL 				pSrt+=s;
//DEL 			}
//DEL 			nOffset+=s*(colorTableCount+1);
//DEL 		}
//DEL 	}
//DEL 	XU32 nSize=m_nDataLength-(m_nColorTabs<<2);
//DEL 	nSize=XMIN(nSize,out.GetDataSize()-nOffset);
//DEL 	switch(nBits)
//DEL 	{
//DEL 	//default:
//DEL 			//XGlobal::Memcpy(GetBuffer(),out.BufferAddr()+nOffset,nSize);
//DEL 	case 16:
//DEL 		{
//DEL 			int iPitch=((w+1)>>1)<<1;
//DEL 			{
//DEL 				XU32*pDst=(XU32*)GetBuffer();
//DEL 				XU8*pSrt=(XU8*)out.BufferAddr();
//DEL 				//nSize>>=2;
//DEL 				for(int y=0;y<h;y++)
//DEL 				{
//DEL 					for(int x=0;x<w;x++)
//DEL 					{
//DEL 						XU16 c16=pSrt[x];
//DEL 						pDst[x]=(c16&0x8000)?0xff000000:0;
//DEL 						pDst[x]|=(((((c16>>10)&0x3f)*0xff)>>5)<<16)|
//DEL 								 (((((c16>>5)&0x3f)*0xff)>>5)<<8)|
//DEL 								 ((((c16)&0x3f)*0xff)>>5);
//DEL 							//pTab[pSrt[x]];
//DEL 					}
//DEL 					pDst+=w;
//DEL 					pSrt+=iPitch;
//DEL 				}
//DEL 				/*for(XU32 i=0;i<nSize;i+=4)
//DEL 				{
//DEL 					pDst[i+3]=pSrt[i];
//DEL 					pDst[i+2]=pSrt[i+1];
//DEL 					pDst[i+1]=pSrt[i+2];
//DEL 					pDst[i]=pSrt[i+3];
//DEL 				}*/
//DEL 			}break;
//DEL 		}break;
//DEL 	case 8:
//DEL 		if(pTab)
//DEL 		{
//DEL 			int iPitch=((w+3)>>2)<<2;
//DEL 			{
//DEL 				XU32*pDst=(XU32*)GetBuffer();
//DEL 				XU8*pSrt=out.BufferAddr()+nOffset;
//DEL 				//nSize>>=2;
//DEL 				XU32*pPal=(XU32*)pTab;
//DEL 				for(int y=0;y<h;y++)
//DEL 				{
//DEL 					for(int x=0;x<w;x++)
//DEL 						pDst[x]=pPal[pSrt[x]];
//DEL 					pDst+=w;
//DEL 					pSrt+=iPitch;
//DEL 				}
//DEL 				/*for(XU32 i=0;i<nSize;i+=4)
//DEL 				{
//DEL 					pDst[i+3]=pSrt[i];
//DEL 					pDst[i+2]=pSrt[i+1];
//DEL 					pDst[i+1]=pSrt[i+2];
//DEL 					pDst[i]=pSrt[i+3];
//DEL 				}*/
//DEL 			}break;
//DEL 		}break;
//DEL 	case 32:
//DEL 			{
//DEL 				XU8*pDst=GetBuffer();
//DEL 				XU8*pSrt=out.BufferAddr();
//DEL 				//nSize>>=2;
//DEL 				for(XU32 i=0;i<nSize;i+=4)
//DEL 				{
//DEL 					pDst[i+3]=pSrt[i];
//DEL 					pDst[i+2]=pSrt[i+1];
//DEL 					pDst[i+1]=pSrt[i+2];
//DEL 					pDst[i]=pSrt[i+3];
//DEL 				}
//DEL 			}break;
//DEL 	}
//DEL 	if(pTab) delete pTab;
//DEL 	//Release();
//DEL
//DEL    	/*if(nBits<=8) nBits=8;
//DEL 	else if(nBits<=16) nBits=16;
//DEL 	else if(nBits<=24) nBits=24;
//DEL 	else nBits=32;
//DEL 	m_nLinePitch=(((w*nBits>>3)+3)>>2)<<2;
//DEL
//DEL 	m_nWidth=w;
//DEL 	m_nHeight=h;
//DEL 	m_nBits=nBits;
//DEL 	m_nDataLength=m_nLinePitch*h;
//DEL 	if(nBits<=8)
//DEL 	{
//DEL 		m_nDataLength+=1024;
//DEL 		m_nColorTabs=0xff;
//DEL 		if(colorTableCount)
//DEL 			m_nColorTabs=colorTableCount;
//DEL 	}*/
//DEL
//DEL 	return XTRUE;
//DEL }

//DEL XBOOL XDrawDib::Create(XStream *pStream,XU8Array*pTab)
//DEL {
//DEL 	XImage image;
//DEL 	int nSize=pStream->GetDataSize();
//DEL 	if(nSize<=0) return XFALSE;
//DEL
//DEL 	XU8*pData=pStream->ReadAddr();
//DEL
//DEL 	XU16 nCode=*(XU16*)pData;
//DEL 	/*if(nCode==0xD9FF)
//DEL 	{
//DEL 		pData+=4;
//DEL 		nSize-=4;
//DEL 	}*/
//DEL
//DEL 	//if(pTab!=XNULL)
//DEL 	{
//DEL 		XU8 bHeader=XFALSE;
//DEL 		while(nSize>0)
//DEL 		{
//DEL 			nCode=*(XU16*)pData;
//DEL 			XU16 s=2;
//DEL 			XU8 bSkip=XFALSE;
//DEL 			switch(nCode)
//DEL 			{
//DEL 			case 0xDAFF:
//DEL 				 goto ImageData;
//DEL 			case 0xD8FF:
//DEL 				 if(!bHeader)
//DEL 					 bHeader=XTRUE;
//DEL 				 else
//DEL 					 bSkip=XTRUE;
//DEL 				 break;
//DEL 			case 0xD9FF:
//DEL 				 bSkip=XTRUE;
//DEL 				 break;
//DEL 			default:
//DEL 				 s+=((pData[2]<<8)|pData[3]);
//DEL 				 if(nSize<=s) return XFALSE;
//DEL 			}
//DEL 			//if(nCode==0xDAFF) break;
//DEL 			if(!bSkip)
//DEL 			{
//DEL 				if(!image.Append(pData,s)) return XFALSE;
//DEL 			}
//DEL 			nSize-=s;
//DEL 			pData+=s;
//DEL 		}
//DEL 		if(nSize<=0) return XFALSE;
//DEL 	ImageData:
//DEL 		//if(!image.Append(pStream->ReadAddr(),nSize)) return XFALSE;
//DEL 		if(pTab!=XNULL)
//DEL 		{
//DEL 			if(!image.Append(pTab->GetData()+2,pTab->GetSize()-4)) return XFALSE;
//DEL 		}
//DEL 	}
//DEL
//DEL 	if(!image.Append(pData,nSize)) return XFALSE;
//DEL
//DEL
//DEL 	if(!image.Final(XTRUE)) return XFALSE;
//DEL 	int w=image.Width();
//DEL 	int h=image.Height();
//DEL 	int nBits=image.GetPixelBits();
//DEL 	//if(nBits<8||nBits>32) return XFALSE;
//DEL 	if(!XDib::Create(w,h,32)) return XFALSE;//nBits)) return XFALSE;
//DEL 	//if(nBits==8)
//DEL 	//	image.GetPalette((XU32*)m_pDib,256);
//DEL 	image.GetBitmapData(*this);//GetBuffer(),m_nLinePitch*h);
//DEL 	return XTRUE;
//DEL }

//DEL XBOOL XDrawDib::CreateWithAlpha(XStream *pStream, XU32 nSize)
//DEL {
//DEL 	XImage image;
//DEL 	//int nSize=pStream->GetDataSize();
//DEL 	//if(nSize<=0) return XFALSE;
//DEL 	XU32 as=pStream->GetDataSize();
//DEL 	as-=nSize;
//DEL
//DEL 	XU8*pData=pStream->ReadAddr();
//DEL
//DEL 	XU8 bHeader=XFALSE;
//DEL 	while(nSize>0)
//DEL 	{
//DEL 		XU16 nCode=*(XU16*)pData;
//DEL 		XU16 s=2;
//DEL 		XU8 bSkip=XFALSE;
//DEL 		switch(nCode)
//DEL 		{
//DEL 		case 0xDAFF:
//DEL 			 goto ImageData;
//DEL 		case 0xD8FF:
//DEL 			 if(!bHeader)
//DEL 				 bHeader=XTRUE;
//DEL 			 else
//DEL 				 bSkip=XTRUE;
//DEL 			 break;
//DEL 		case 0xD9FF:
//DEL 			 bSkip=XTRUE;
//DEL 			 break;
//DEL 		default:
//DEL 			 s+=((pData[2]<<8)|pData[3]);
//DEL 			 if(nSize<=s) return XFALSE;
//DEL 		}
//DEL 		//if(nCode==0xDAFF) break;
//DEL 		if(!bSkip)
//DEL 		{
//DEL 			if(!image.Append(pData,s)) return XFALSE;
//DEL 		}
//DEL 		nSize-=s;
//DEL 		pData+=s;
//DEL 		pStream->Skip(s);
//DEL 	}
//DEL 	if(nSize<=0) return XFALSE;
//DEL ImageData:
//DEL
//DEL 	//XFile file;
//DEL 	//file.Open("D:\\test.jpg",XFile::XCREATE|XFile::XBINARY|XFile::XWRITE);
//DEL 	//file.Write(pData,nSize);
//DEL 	//file.Close();
//DEL
//DEL 	if(!image.Append(pData,nSize)) return XFALSE;
//DEL
//DEL 	pStream->Skip(nSize);
//DEL
//DEL 	if(!image.Final(XTRUE)) return XFALSE;
//DEL 	int w=image.Width();
//DEL 	int h=image.Height();
//DEL 	if(!XDib::Create(w,h,32)) return XFALSE;
//DEL
//DEL 	image.GetBitmapData(*this);
//DEL
//DEL 	if(as>0)
//DEL 	{
//DEL 		XStream out;
//DEL 		XZLib lib;
//DEL 		lib.Init(pStream,&out);
//DEL 		lib.Decode(XNULL);
//DEL 		lib.Release();
//DEL 		nSize=w*h;
//DEL 		nSize=XMIN(nSize,out.GetDataSize());
//DEL 		XU32*pDst=(XU32*)GetBuffer();
//DEL 		XU8*pSrt=out.ReadAddr();
//DEL 		for(XU32 i=0;i<nSize;i++)
//DEL 		{
//DEL 			((XU8*)&pDst[i])[3]=pSrt[i];
//DEL 		}
//DEL 	}
//DEL
//DEL 	return XTRUE;
//DEL
//DEL }

/*void XDrawDib::SetPaint(XPaintDib &dib,XPaintDib&mask,int dstx, int dsty, XRECT &rect,XU32Array&colors)
{
	//return;
	if(m_nBits!=32) return;
	int w=dib.Width(),h=dib.Height();
	ClipRect(dstx,dsty,rect);
	int dw=rect.right-rect.left;
	int dh=rect.bottom-rect.top;
	if(dw<=0||dh<=0) return;
	int iStart=rect.top*w+rect.left;
	XU8*pSrt=dib.GetBuffer();
	pSrt+=iStart;
	XU8*pMask=mask.GetBuffer();
	pMask+=iStart;
	XU32* pDst=(XU32*)GetBuffer();
	pDst+=dsty*m_nWidth+dstx;
	int al,iColor=0,alpha=0,iColor0=0;

	int  fCount=colors.GetSize();
	XU32 nColor=0,nColor0;
	XU8* pColor=(XU8*)&nColor,*pColor0=(XU8*)&nColor0;
	XU8  nMix[4];
	for(int y=0;y<dh;y++)
	{
		for(int x=0;x<dw;x++)
		{
			XU8 nFlag=(pSrt[x]);
			XU8 nFill=pMask[x];
			if(nFill)
			{
				iColor0=iColor;
				if(iColor0==iColor) iColor0=0;
				else				nColor0=nColor;
				iColor=nFill;
				if(iColor<=fCount)
				{
					nColor=colors[iColor-1];
					alpha=nColor>>24;
				}
			}
			if(nFlag)
			{
				if(iColor0&&nFlag<ANTIMAX)
				{
					int sf=ANTIMAX-nFlag;
					nMix[0]=(pColor[0]*nFlag+pColor0[0]*sf)>>ANTIOFFSET;
					nMix[1]=(pColor[1]*nFlag+pColor0[1]*sf)>>ANTIOFFSET;
					nMix[2]=(pColor[2]*nFlag+pColor0[2]*sf)>>ANTIOFFSET;
					nMix[3]=(pColor[3]*nFlag+pColor0[3]*sf)>>ANTIOFFSET;
					al=nMix[3];
					switch(al)
					{
					case 0xff:
						 *(pDst+x)=*(XU32*)nMix;
						 break;
					case 0:break;
					default:
						 OVERPIXEL(pDst+x,nMix,al);
						 break;
					}
				}
				else
				{
					al=SETFLAG(alpha,nFlag);
					if(al==0xff)
						*(pDst+x)=nColor;
					else
					{
						OVERPIXEL(pDst+x,pColor,al);
					}
				}
			}
		}
		iColor=iColor0=0;
		pSrt+=w;
		pDst+=m_nWidth;
		pMask+=m_nWidth;
	}
}*/

void XDrawDib::SetBackground(XU32 nColor)
{
	if(IsEmpty()) return;
	if(m_nBits!=32) return;
	int il=m_nWidth*m_nHeight;
	XU32*pColor=(XU32*)m_pDib;
	if((nColor>>24)<0xff)
	{
		RGB8*pColor=(RGB8*)&nColor;
		int a=pColor->alpha;
		pColor->red=(pColor->red*a)>>8;
		pColor->green=(pColor->green*a)>>8;
		pColor->blue=(pColor->blue*a)>>8;
	}
	for(int i=0;i<il;i++)
		pColor[i]=nColor;
}



void XDrawDib::FormatRect(XS32 &x, XS32 &y, XRect &rect,int w,int h)
{
	if(rect.left<0)
	{
		x-=rect.left;
		rect.left=0;
	}
	if(rect.top<0)
	{
		y-=rect.top;
		rect.top=0;
	}
	if(rect.right>w)
	{
		rect.right=w;//rect.left+w;
	}
	if(rect.bottom>h)
	{
		rect.bottom=h;//rect.top+h;
	}
}

void XDrawDib::FromDib(XDib &dib)
{
	if(dib.IsEmpty()) return;
	int w=dib.Width();
	int h=dib.Height();
	int nBits=dib.GetBits();
	XDib::Create(w,h,nBits);
	if(IsEmpty()) return;
	m_clip.left=0;
	m_clip.top=0;
	m_clip.right=w;
	m_clip.bottom=h;
	XGlobal::Memcpy(m_pDib,dib.GetAddress(),m_nDataLength);
}

void XDrawDib::Transform(XRect &rect, XCXForm &form)
{
	if(IsEmpty()) return;
	if(m_nBits!=32) return;
	rect&=m_clip;
	if(rect.IsRectEmpty()) return;
	XU32*pDst=(XU32*)m_pDib;
	pDst+=rect.top*m_nWidth;
	for(int y=rect.top;y<rect.bottom;y++)
	{
		for(int x=rect.left;x<rect.right;x++)
			form.Apply(pDst[x]);
		pDst+=m_nWidth;
	}
}

bool XDrawDib::CopyChannel(XDib &dib, XRect &rect, XPoint &dst, XU8 sc, XU8 dc)
{
	//return false;
	bool bAlpha=false;
	if(IsEmpty()||dib.IsEmpty()) return bAlpha;
	if(m_nBits!=32&&dib.GetBits()!=32) return bAlpha;
	FormatRect(dst.x,dst.y,rect,dib.Width(),dib.Height());
	ClipRect(dst.x,dst.y,rect);
	if(rect.IsRectEmpty()) return bAlpha;
	int ip=dib.GetLinePitch();
	XU8*pDst=GetBuffer()+dst.y*m_nLinePitch+(dst.x<<2);
	XU8*pSrt=dib.GetBuffer()+rect.top*ip+(rect.left<<2);
	XU8*pAlpha=pSrt+3;
	int w=rect.Width()<<2;
//#ifdef _ARGB_PIXEL_
	switch(dc)
	{
	case XBLUE_CHANNEL:pDst+=2;break;
	case XGREEN_CHANNEL:pDst++;break;
	case XRED_CHANNEL:break;
	case XALPHA_CHANNEL:pDst+=3;bAlpha=true;break;
	}
	switch(sc)
	{
	case XBLUE_CHANNEL:pSrt+=2;break;
	case XGREEN_CHANNEL:pSrt++;break;
	case XRED_CHANNEL:break;
	case XALPHA_CHANNEL:pSrt+=3;break;
	}
//#else
/*	switch(dc)
	{
	case XBLUE_CHANNEL:break;
	case XGREEN_CHANNEL:pDst++;break;
	case XRED_CHANNEL:pDst+=2;break;
	case XALPHA_CHANNEL:
		 //return true;
		 pDst+=3;bAlpha=true;break;
	}
	switch(sc)
	{
	case XBLUE_CHANNEL:break;
	case XGREEN_CHANNEL:pSrt++;break;
	case XRED_CHANNEL:pSrt+=2;break;
	case XALPHA_CHANNEL:pSrt+=3;break;
	}
#endif*/
	/*if(bAlpha)
	{
		for(int y=rect.top;y<rect.bottom;y++)
		{
			for(int x=0;x<w;x+=4)
			{
				int alpha=pAlpha[x];
				switch(alpha)
				{
				case 0:break;
				case 0xff:
					 pDst[x]=pSrt[x];
					 break;
				default:
					 pDst[x]=(pSrt[x]<<8)/alpha;
				}
			}
			pDst+=m_nLinePitch;
			pSrt+=ip;
			pAlpha+=ip;
		}
	}
	else*/
	{
		for(int y=rect.top;y<rect.bottom;y++)
		{
			for(int x=0;x<w;x+=4)
			{
				pDst[x]=pSrt[x];
			}
			pDst+=m_nLinePitch;
			pSrt+=ip;
		}
	}
	return bAlpha;
}

void XDrawDib::FloodFill(int x, int y, XU32 nColor)
{
	if(IsEmpty()) return;
    if(!m_clip.PtInRect(x,y)) return;
	if(m_nBits!=32||IsEmpty()) return;
	enum 
	{
		BW_SS_LEFT=1,
		BW_SS_RIGHT=2,
		BW_SS_UP=4,
		BW_SS_DOWN=8
	};
	XU32*pDst=(XU32*)GetBuffer();
	pDst+=y*m_nWidth;
	if((nColor>>24)<0xff)
	{
		RGB8*pColor=(RGB8*)&nColor;
		int a=pColor->alpha;
		pColor->red=(pColor->red*a)>>8;
		pColor->green=(pColor->green*a)>>8;
		pColor->blue=(pColor->blue*a)>>8;
	}
	if(*pDst==nColor) return;
	XStack<XU32> stack;
	stack.Push(x);
	stack.Push(x);
	stack.Push(y);
	stack.Push(BW_SS_LEFT|BW_SS_RIGHT|BW_SS_UP|BW_SS_DOWN);
	stack.Push((XU32)pDst);
	XU32 fColor=pDst[x];
	//if(!(nColor>>24))
	//	nColor|=fColor&0xff000000;
	pDst[x]=nColor;
	int maxx=m_clip.right-1,maxy=m_clip.bottom-1;
	while(stack.GetSize())
	{
		pDst=(XU32*)stack.Pop();
		int style=stack.Pop();
		int y=stack.Pop();
		int ex=stack.Pop();
		int bx=stack.Pop();
		if(style&BW_SS_LEFT)
		{
			while(bx>m_clip.left)
			{
				bx--;
				if(pDst[bx]!=fColor) break;
				pDst[bx]=nColor;
			}
		}
		if(style&BW_SS_RIGHT)
		{
			while(ex<maxx)
			{
				ex++;
				if(pDst[ex]!=fColor) break;
				pDst[ex]=nColor;
			}
		}
		if((style&BW_SS_UP)&&y>m_clip.top)
		{
			XU32*p=(pDst-m_nWidth);
			int x1,x2,bSet=XFALSE;
			for(int x=bx;x<=ex;x++)
			{
				if(p[x]==fColor)
				{
					p[x]=nColor;
					if(!bSet)
					{
						x1=x;
						bSet=XTRUE;
					}
				}
				else if(bSet)
				{
					bSet=XFALSE;
					x2=x;
					stack.Push(x1);
					stack.Push(x2);
					stack.Push(y-1);
					int ss=BW_SS_UP;
					if(x1==bx) ss|=BW_SS_LEFT;
					if(x2==ex) ss|=BW_SS_RIGHT;
					stack.Push(ss);
					stack.Push((XU32)p);
				}
			}
			if(bSet)
			{
				stack.Push(x1);
				stack.Push(ex);
				stack.Push(y-1);
				int ss=BW_SS_UP|BW_SS_RIGHT;
				if(x1==bx) ss|=BW_SS_LEFT;
				stack.Push(ss);
				stack.Push((XU32)p);
			}
		}
		if((style&BW_SS_DOWN)&&y<maxy)
		{
			XU32*p=(pDst+m_nWidth);
			int x1,x2,bSet=XFALSE;
			for(int x=bx;x<=ex;x++)
			{
				if(p[x]==fColor)
				{
					p[x]=nColor;
					if(!bSet)
					{
						x1=x;
						bSet=XTRUE;
					}
				}
				else if(bSet)
				{
					bSet=XFALSE;
					x2=x;
					stack.Push(x1);
					stack.Push(x2);
					stack.Push(y+1);
					int ss=BW_SS_DOWN;
					if(x1==bx) ss|=BW_SS_LEFT;
					if(x2==ex) ss|=BW_SS_RIGHT;
					stack.Push(ss);
					stack.Push((XU32)p);
				}
			}
			if(bSet)
			{
				stack.Push(x1);
				stack.Push(ex);
				stack.Push(y+1);
				int ss=BW_SS_DOWN|BW_SS_RIGHT;
				if(x1==bx) ss|=BW_SS_LEFT;
				stack.Push(ss);
				stack.Push((XU32)p);
			}
		}
		/*if(x>m_clip.left)
		{
			XU32*p=pDst-1;
			if(*p==fColor)
			{
			   *p=nColor;
			   stack.Push(x-1);
			   stack.Push(y);
			   stack.Push((XU32)p);
			}
		}
		if(x+1<m_clip.right)
		{
			XU32*p=pDst+1;
			if(*p==fColor)
			{
				*p=nColor;
				stack.Push(x+1);
				stack.Push(y);
				stack.Push((XU32)p);
			}
		}
		if(y>m_clip.top)
		{
			XU32*p=pDst-m_nWidth;
			if(*p==fColor)
			{
				*p=nColor;
				stack.Push(x);
				stack.Push(y-1);
				stack.Push((XU32)p);
			}
		}
		if(y+1<m_clip.bottom)
		{
			XU32*p=pDst+m_nWidth;
			if(*p==fColor)
			{
				*p=nColor;
				stack.Push(x);
				stack.Push(y+1);
				stack.Push((XU32)p);
			}
		}*/
	}
}

void XDrawDib::GetColorBounds(XRect &rect, XU32 mask, XU32 color, XBOOL bFind)
{
	if(IsEmpty()) return;
	if(m_nBits!=32) return;
	XU32*pDst=(XU32*)GetBuffer();
	XU32 minx=m_nWidth,miny=m_nHeight;
	XU32 maxx=0,maxy=0;
	for(XU32 y=0;y<m_nHeight;y++)
	{
		for(XU32 x=0;x<m_nWidth;x++)
		{
			if(bFind)
			{
				if((pDst[x]&mask)!=color) continue;
			}
			else
			{
				if((pDst[x]&mask)==color) continue;
			}
			minx=XMIN(minx,x);
			maxx=XMAX(maxx,x);
			miny=XMIN(miny,y);
			maxy=XMAX(maxy,y);
		}
		pDst+=m_nWidth;
	}
	if(maxx>=minx)
	{
		rect.left=minx;
		rect.right=maxx+1;
	}
	if(maxy>=miny)
	{
		rect.top=miny;
		rect.bottom=maxy+1;
	}
}

XU32 XDrawDib::GetPixel(int x, int y)
{
	if(IsEmpty()) return 0;
	if(m_nBits!=32) return 0;
	if(x<0||x>=(int)m_nWidth) return 0;
	if(y<0||y>=(int)m_nHeight) return 0;
	XU32*pDst=(XU32*)GetBuffer();
	pDst+=y*m_nWidth;
	return pDst[x];
}

XBOOL XDrawDib::PreAlpha()
{
	if(m_nBits!=32) return XTRUE;
	RGB8*pBuf=(RGB8*)m_pDib;//baseAddr;
	bool transparent=false;
		//memset(pBuf,0,size);
	int size=m_nWidth*m_nHeight;
	{
		for(int i=0;i<size;i++)
		{
			int a=pBuf[i].alpha;
			//if(a>=0xff) continue;
			switch(a)
			{
			case 0xff:break;
			case 0:
				transparent=true;
				break;
			default:
				transparent=true;
			//if(pBuf[i].red>a) pBuf[i].red=a;
			//if(pBuf[i].blue>a) pBuf[i].blue=a;
			//if(pBuf[i].green>a) pBuf[i].green=a;
				pBuf[i].red=(pBuf[i].red*a)>>8;
				pBuf[i].green=(pBuf[i].green*a)>>8;
				pBuf[i].blue=(pBuf[i].blue*a)>>8;
				break;
			}
		}
	}
	return transparent;
}

void XDrawDib::SetPixel(int x, int y, XU32 nColor, XU8 b32)
{
	if(IsEmpty()) return;
	if(m_nBits!=32) return;
	if(x<0||x>=(int)m_nWidth) return;
	if(y<0||y>=(int)m_nHeight) return;
	XU32*pDst=(XU32*)GetBuffer();
	pDst+=y*m_nWidth;
	int a=b32?(nColor>>24):(pDst[x]>>24);
	switch(a)
	{
	case 0:
		pDst[x]=0;
		break;
	case 0xff:
//#if (__CORE_VERSION__>=0x02074000)
		pDst[x]=nColor|(0xff<<24);
//#else
//		pDst[x]=nColor|0xff;
//#endif
		break;
	default:
		//if(a<0xff)
		{
			RGB8*pColor=(RGB8*)&nColor;
			//int a=pColor->alpha;
			pColor->red=(pColor->red*a)>>8;
			pColor->green=(pColor->green*a)>>8;
			pColor->blue=(pColor->blue*a)>>8;
		}
		
		if(b32)
			pDst[x]=nColor;
		else
		{
//#if (__CORE_VERSION__>=0x02074000)
			pDst[x]=(nColor&0xffffff)|(a<<24);//(pDst[x]&0xff000000);
//#else
//			pDst[x]=(nColor&0xffffff)|a;//(pDst[x]&0xff000000);
//#endif
		}break;
	}
}

XBOOL XDrawDib::HitTest(XRect &rect, int alpha)
{

	if(alpha>=0xff) return XFALSE;
	if(IsEmpty()) return XFALSE;
	if(m_nBits!=32) return XFALSE;
	rect.NormalizeRect();
	rect&=XRect(0,0,m_nWidth,m_nHeight);
	if(rect.IsRectEmpty()) return XFALSE;
	if(alpha<=0) return XTRUE;
	XU32*pDst=(XU32*)GetBuffer();
	pDst+=rect.top*m_nWidth;
	for(int y=rect.top;y<rect.bottom;y++)
	{
		for(int x=rect.left;x<rect.right;x++)
		{
			if(((XU8*)&pDst[x])[3]<alpha) continue;
			return XTRUE;
		}
		pDst+=m_nWidth;
	}
	return XFALSE;
}



void XDrawDib::Mix(XDib &dib, XRect &rect, XPoint &dst, int rm, int gm, int bm, int am)
{
	if(IsEmpty()||dib.IsEmpty()) return;
	if(m_nBits!=32&&dib.GetBits()!=32) return;
	FormatRect(dst.x,dst.y,rect,dib.Width(),dib.Height());
	ClipRect(dst.x,dst.y,rect);
	if(rect.IsRectEmpty()) return;
	int w=dib.Width(),iPitch=dib.GetLinePitch();
	XU8*pDst=GetBuffer();
	pDst+=dst.y*m_nLinePitch+(dst.x<<2);
	XU8*pSrt=dib.GetBuffer()+rect.top*iPitch+(rect.left<<2);
	int iLine=rect.Width()<<2;
	XLIMIT(rm,0,256)
	XLIMIT(gm,0,256)
	XLIMIT(bm,0,256)
	XLIMIT(am,0,256)
	int rmm=256-rm,gmm=256-gm,bmm=256-bm,amm=256-am;
	for(int y=rect.top;y<rect.bottom;y++)
	{
		for(int x=0;x<iLine;x+=4)
		{
			pDst[x]=(pDst[x]*bmm+pSrt[x]*bm)>>8;
			pDst[x+1]=(pDst[x+1]*gmm+pSrt[x+1]*gm)>>8;
			pDst[x+2]=(pDst[x+2]*rmm+pSrt[x+2]*rm)>>8;
			pDst[x+3]=(pDst[x+3]*amm+pSrt[x+3]*am)>>8;
		}
		pDst+=m_nLinePitch;
		//if((int)pDst-(int)m_pDib>this->m_nDataLength)
		//	int vvv=0;
		pSrt+=iPitch;
	}
}

void XDrawDib::Noise(int nLow, int nHigh, XU8 nOption, XBOOL bGray)
{
	if(IsEmpty()) return;
	if(m_nBits!=32) return;
	XU8*pLast=m_pDib+m_nDataLength-4;
	XU8*pDst=m_pDib;
	XU8 nNeed=nLow>0||nHigh<0xff;
	XU8 v=XGlobal::RandInt()&0xff;
	if(nNeed)
		XLIMIT(v,nLow,nHigh);
	while(pDst<pLast)
	{

		if(nOption&XALPHA_CHANNEL)
		{
			pDst[3]=v;
			v=XGlobal::RandInt()&0xff;
			if(nNeed)
				XLIMIT(v,nLow,nHigh);
		}
		if(bGray)
		{
			pDst[0]=pDst[1]=pDst[2]=v;
			v=XGlobal::RandInt()&0xff;
			if(nNeed)
				XLIMIT(v,nLow,nHigh);
		}
		else
		{
#ifdef _ARGB_PIXEL_
			if(nOption&XBLUE_CHANNEL)
			{
			  pDst[2]=v;
			  v=XGlobal::RandInt()&0xff;
			  if(nNeed)
				XLIMIT(v,nLow,nHigh);
			}
			if(nOption&XGREEN_CHANNEL)
			{
			  pDst[1]=v;
			  v=XGlobal::RandInt()&0xff;
			  if(nNeed)
				XLIMIT(v,nLow,nHigh);
			}
			if(nOption&XRED_CHANNEL)
			{
			  pDst[0]=v;
			  v=XGlobal::RandInt()&0xff;
			  if(nNeed)
				XLIMIT(v,nLow,nHigh);
			}
#else
			if(nOption&XBLUE_CHANNEL)
			{
			  pDst[0]=v;
			  v=XGlobal::RandInt()&0xff;
			  if(nNeed)
				XLIMIT(v,nLow,nHigh);
			}
			if(nOption&XGREEN_CHANNEL)
			{
			  pDst[1]=v;
			  v=XGlobal::RandInt()&0xff;
			  if(nNeed)
				XLIMIT(v,nLow,nHigh);
			}
			if(nOption&XRED_CHANNEL)
			{
			  pDst[2]=v;
			  v=XGlobal::RandInt()&0xff;
			  if(nNeed)
				XLIMIT(v,nLow,nHigh);
			}
#endif
		}

		pDst+=4;
	}
}

void XDrawDib::PaletteMap(XDib &dib, XRect &rect, XPoint &dst, XU8Array &reds, XU8Array &greens, XU8Array &blues, XU8Array &alphas)
{
	if(IsEmpty()||dib.IsEmpty()) return;
	if(m_nBits!=32&&dib.GetBits()!=32) return;
	FormatRect(dst.x,dst.y,rect,dib.Width(),dib.Height());
	ClipRect(dst.x,dst.y,rect);
	if(rect.Height()+dst.y>this->Height())
	{
		ClipRect(dst.x,dst.y,rect);
	}
	if(rect.IsRectEmpty()) return;
	int iPitch=dib.GetLinePitch();
	XU8*pDst=GetBuffer()+dst.y*m_nLinePitch+(dst.x<<2);
	XU8*pSrt=GetBuffer()+rect.top*iPitch+(rect.left<<2);
	int w=rect.Width()<<2;
	int na=alphas.GetSize(),nr=reds.GetSize(),ng=greens.GetSize(),nb=blues.GetSize();
	for(int y=rect.top;y<rect.bottom;y++)
	{
		for(int x=0;x<w;x+=4)
		{
			XU8 alpha=pSrt[x+3];
			if(alpha<na) alpha=alphas[alpha];
			XU8 red=pSrt[x+2];
			if(red<nr) red=reds[red];
			XU8 green=pSrt[x+1];
			if(green<ng) green=greens[green];
			XU8 blue=pSrt[x];
			if(blue<nb) green=blues[blue];
			pDst[x]=blue;
			pDst[x+1]=green;
			pDst[x+2]=red;
			pDst[x+3]=alpha;
		}
		pDst+=m_nLinePitch;
		pSrt+=iPitch;
	}
}

int XDrawDib::Dissolve(XRect &rect, XPoint &dst, int nSeed, int nPixels, XU32 nColor)
{
	nSeed+=40;
	if(IsEmpty()) return nSeed;
	if(m_nBits!=32) return nSeed;
	ClipRect(dst.x,dst.y,rect);
	if(rect.IsRectEmpty()) return nSeed;
	if((nColor>>24)<0xff)
	{
		RGB8*pColor=(RGB8*)&nColor;
		int a=pColor->alpha;
		pColor->red=(pColor->red*a)>>8;
		pColor->green=(pColor->green*a)>>8;
		pColor->blue=(pColor->blue*a)>>8;
	}
	XU32*pDst=(XU32*)GetBuffer();
	pDst+=dst.y*m_nWidth+dst.x;
	int w=rect.Width();
	int ns=0;
	//XU8*pColor=(XU8*)&nColor;
	int nTotal=rect.Width()*rect.Height();
	int nLines=nPixels/rect.Height(),nf=nLines>>1;
	for(int y=rect.top;y<rect.bottom;y++)
	{
		int nThis=nLines;
		for(int x=0;x<w;x++)
		{
			nTotal--;
			if(pDst[x]==nColor) continue;
			if(nTotal>nPixels-ns)
			{
				if(!nThis) continue;
				//XU8*p=(XU8*)&pDst[x];
				//int nDelta=p[0]+p[1]+p[2]+p[3];
				if((int)XGlobal::Random(nTotal)>=nPixels) continue;
			}
			//else
			//	int v=0;
			nThis--;
			nPixels--;
			pDst[x]=nColor;
			ns++;
			if(!nPixels) return nSeed;
		}
		pDst+=m_nWidth;
	}
	return nSeed;
}

int XDrawDib::Dissolve(XDib&dib,XRect &rect, XPoint &dst, int nSeed, int nPixels)
{
	nSeed+=40;
	if(IsEmpty()||dib.IsEmpty()) return nSeed;
	if(m_nBits!=32||dib.GetBits()!=32) return nSeed;
	FormatRect(dst.x,dst.y,rect,dib.Width(),dib.Height());
	ClipRect(dst.x,dst.y,rect);
	if(rect.IsRectEmpty()) return nSeed;
	XU32*pDst=(XU32*)GetBuffer();
	pDst+=dst.y*m_nWidth+dst.x;
	XU32*pSrt=(XU32*)dib.GetBuffer();
	int ww=dib.Width();
	pSrt+=rect.top*ww+rect.left;

	int w=rect.Width();
	int ns=0;

	int nTotal=rect.Width()*rect.Height();
	int nLines=nPixels/rect.Height(),nf=nLines>>1;
	for(int y=rect.top;y<rect.bottom;y++)
	{
		int nThis=nLines;
		for(int x=0;x<w;x++)
		{
			nTotal--;
			if(pDst[x]==pSrt[x]) continue;
			if(nTotal>nPixels-ns)
			{
				if(!nThis) continue;
				//XU8*p=(XU8*)&pDst[x];
				//int nDelta=p[0]+p[1]+p[2]+p[3];
				if((int)XGlobal::Random(nTotal)>=nPixels) continue;
			}
			//else
			//	int v=0;
			nThis--;
			nPixels--;
			pDst[x]=pSrt[x];
			ns++;
			if(!nPixels) return nSeed;
		}
		pDst+=m_nWidth;
		pSrt+=ww;
	}

	return nSeed;
}

void XDrawDib::Scroll(int x, int y)
{
	if(x==0&&y==0)
		return;
	if(IsEmpty()) return;
	if(m_nBits!=32) return;
	/*int dstx=x,dsty=y,srtx=0,srty=0;
	if(x<0)
	{
		dstx=0;
		srtx=-x;
	}
	if(y<0)
	{
		dsty=0;
		srty=-y;
	}*/
	int len=(m_nWidth-XABS(x))<<2;//(m_nWidth-XMAX(srtx,dstx))<<2;
	int hh=m_nHeight-XABS(y);//-XMAX(srty,dsty);
	if(len<=0||hh<=0) return;
	XU32*pDst=(XU32*)GetBuffer();
	XU32*pSrt=pDst;//+srtx;
	if(x>0)
		pDst+=x;
	else
		pSrt+=(-x);
	//pDst+=dstx;
	if(x>0&&y==0)
	{
		XU32*pBuf=(XU32*)new XU8[len];
		for(int y=0;y<hh;y++)
		{
			XGlobal::Memcpy(pBuf,pSrt,len);
			XGlobal::Memcpy(pDst,pBuf,len);
			pDst+=m_nWidth;
			pSrt+=m_nWidth;
		}
		//else

		delete []pBuf;
	}
	else
	{
		if(y<=0)
		{
			//pDst+=dsty*m_nWidth;
			//pSrt+=srty*m_nWidth;
			pSrt+=(-y)*m_nWidth;
			for(int y=0;y<hh;y++)
			{
				XGlobal::Memcpy(pDst,pSrt,len);
				pDst+=m_nWidth;
				pSrt+=m_nWidth;
			}
		}
		else
		{
			pDst+=(m_nHeight-1)*m_nWidth;
			pSrt+=(m_nHeight-1-y)*m_nWidth;
			//pDst+=(dsty+hh-1)*m_nWidth;
			//pSrt+=(srty+hh-1)*m_nWidth;
			for(int y=0;y<hh;y++)
			{
				XGlobal::Memcpy(pDst,pSrt,len);
				pDst-=m_nWidth;
				pSrt-=m_nWidth;
			}
		}
	}
	///XU32*pDst=GetBuffer();
	//XU32*pSrt=pDst+(srty+hh-1)*m_nWidth+srtx;
	//pDst+=(dsty+hh-1)*m_nWidth+dstx;

}

int XDrawDib::Threshold(XDib &dib, XRect &rect, XPoint &dst,XU8 nOpt,XU32 thre, XU32 nMask, XU32 nColor, XU8 bCopy)
{
	if(IsEmpty()||dib.IsEmpty()) return 0;
	if(m_nBits!=32||dib.GetBits()!=32) return 0;
	FormatRect(dst.x,dst.y,rect,dib.Width(),dib.Height());
	ClipRect(dst.x,dst.y,rect);
	if(rect.IsRectEmpty()) return 0;
	if((nColor>>24)<0xff)
	{
		RGB8*pColor=(RGB8*)&nColor;
		int a=pColor->alpha;
		pColor->red=(pColor->red*a)>>8;
		pColor->green=(pColor->green*a)>>8;
		pColor->blue=(pColor->blue*a)>>8;
	}
	int nTotal=0;
	int ww=dib.Width(),w=rect.Width();
	thre&=nMask;
	XU32*pSrt=(XU32*)dib.GetBuffer()+rect.top*ww+rect.left;
	XU32*pDst=(XU32*)GetBuffer()+dst.y*m_nWidth+dst.x;
	for(int y=rect.top;y<rect.bottom;y++)
	{
		for(int x=0;x<w;x++)
		{
			XU8 bOK=XFALSE;
			XU32 c=pSrt[x]&nMask;
			switch(nOpt)
			{
			case 0://<
				bOK=c<thre;break;
			case 1://<=
				bOK=c<=thre;break;
			case 2://>
				bOK=c>thre;break;
			case 3://>=
				bOK=c>=thre;break;
			case 4://==
				bOK=c==thre;break;
			case 5://!=
				bOK=c!=thre;break;
			}
			if(bOK)
			{	pDst[x]=nColor;nTotal++;}
			else if(bCopy)
				pDst[x]=pSrt[x];
		}
		pSrt+=ww;
		pDst+=m_nWidth;
	}
	return nTotal;
}

int XDrawDib::PixelFormat()
{
    switch(m_nBits)
	{
	default:
	case 32:return pixFmt32;
	case 24:return pixFmt24;
	case 8:return pixFmt8;
	case 16:return pixFmt16;
	}
}

void XDrawDib::GetRGBPixel(int x, int y, _XRGBI *c)
{
	FLASHASSERT(m_pDib);

	// Clip to the edges
	if ( x < 0 ) x = 0;
	if ( y < 0 ) y = 0;
	if ( y >= (int)m_nHeight ) y = m_nHeight-1;
	if ( x >= (int)m_nWidth ) x = m_nWidth-1;

	U8 * rowAddr = (U8 *)GetBuffer() + y*m_nLinePitch;
	_XRGB8*colors=(_XRGB8*)GetColorTable();
	switch ( m_nBits ) {

		case 8: {
			ExpandColor(colors[rowAddr[x]], c);
		} return;

		case 16: {
			U16 pix = *((U16 *)rowAddr + x);
			UnpackPix16(pix, c);
		} return;

		case 32: {
			U32 pix = *((U32 *)rowAddr + x);
			UnpackPix32(pix, c);
		} return;
	}
}

void XDrawDib::GetSSRGBPixel(SFIXED xH, SFIXED yH, _XRGBI *c)
{
	FLASHASSERT(m_pDib);

	int x = (int)(xH >> 16);
	int y = (int)(yH >> 16);
	int xf = (U16)xH >> 13;
	int yf = (U16)yH >> 13;

	_XCoverEntry cv = PixCoverage[xf][yf];

	// Clip to the edges
	if ( x < 0 ) {
		x = 0;
		cv.ce[0]+=cv.ce[1]; cv.ce[1]=0; // shift the coverage to the leftmost column
		cv.ce[2]+=cv.ce[3]; cv.ce[3]=0;
	} else if ( x >= (int)m_nWidth-1 ) {
		x = m_nWidth-2;
		cv.ce[1]+=cv.ce[0]; cv.ce[0]=0; // shift the coverage to the rightmost column
		cv.ce[3]+=cv.ce[2]; cv.ce[2]=0;
	}
	if ( y < 0 ) {
		y = 0;
		cv.ce[0]+=cv.ce[2]; cv.ce[2]=0; // shift the coverage to the top row
		cv.ce[1]+=cv.ce[3]; cv.ce[3]=0;
	} else if ( y >= (int)m_nHeight-1 ) {
		y = m_nHeight-2;
		cv.ce[2]+=cv.ce[0]; cv.ce[0]=0; // shift the coverage to the bottom row
		cv.ce[3]+=cv.ce[1]; cv.ce[1]=0;
	}

// #if 1
	_XRGBIU pix;
	_XRGBIU p;
	U8 * rowAddr = (U8 *)GetBuffer() + y*m_nLinePitch;
	_XRGB8*colors=(_XRGB8*)m_pDib;
	XU8 bmFormat=PixelFormat();
	switch ( m_nBits ) {

		case 8: {
			//const U8* shiftTab = shiftTabs[bmFormat];	// table of shift values
			//int pxMask   = pxMasks[bmFormat];		// mask the same width as a pixel
			//int rbShift  = rbShifts[bmFormat];	// shift count to calc rowbytes
			//int stMask   = stMasks[bmFormat];		// mask to extract index for shiftTab

			// Get the first column
			U8 * bits = rowAddr + x;//(x>>rbShift);
			//int shift = shiftTab[x&stMask];
#ifdef _USE64BITS
			ExpandColor(colors[(bits[0]/* >> shift*/)/* & pxMask*/], &pix.i);
			pix.l *= cv.ce[0];
			ExpandColor(colors[(bits[m_nLinePitch]/* >> shift*/)/* & pxMask*/], &p.i);
			pix.l += p.l*cv.ce[2];
			// Get the second column
			//x++;
			bits++;
			//bits = rowAddr + x;//(x>>rbShift);
			//shift = shiftTab[x/*&stMask*/];
			ExpandColor(colors[(bits[0]/* >> shift*/)/* & pxMask*/], &p.i);
			pix.l += p.l*cv.ce[1];
			ExpandColor(colors[(bits[m_nLinePitch]/* >> shift*/)/* & pxMask*/], &p.i);
			pix.l += p.l*cv.ce[3];
		} break;

		case 16: {
			U16 * bits = (U16 *)rowAddr + x;
			UnpackPix16(bits[0], &pix.i);
			pix.l *= cv.ce[0];
			UnpackPix16(bits[1], &p.i);
			pix.l += p.l*cv.ce[1];
			// Get the second row
			bits = (U16 *)((U8 *)bits + m_nLinePitch);
			UnpackPix16(bits[0], &p.i);
			pix.l += p.l*cv.ce[2];
			UnpackPix16(bits[1], &p.i);
			pix.l += p.l*cv.ce[3];
		} break;

		case 32: {
			U32 * bits = (U32 *)rowAddr + x;
			UnpackPix32(bits[0], &pix.i);
			pix.l *= cv.ce[0];
			UnpackPix32(bits[1], &p.i);
			pix.l += p.l*cv.ce[1];

			// Get the second row
			bits = (U32 *)((U8 *)bits + m_nLinePitch);
			UnpackPix32(bits[0], &p.i);
			pix.l += p.l*cv.ce[2];
			UnpackPix32(bits[1], &p.i);
			pix.l += p.l*cv.ce[3];

		} break;
	}

	*((XU64*)c) = (pix.l>>3)&_MASK64;

#else
			ExpandColor(colors[(bits[0]/* >> shift*/)/* & pxMask*/], &pix.i);
			pix.l.c0 *= cv.ce[0];
			pix.l.c1 *= cv.ce[0];
			ExpandColor(colors[(bits[m_nLinePitch]/* >> shift*/)/* & pxMask*/], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[2];
			pix.l.c1 += p.l.c1*cv.ce[2];

			// Get the second column
			//x++;
			bits++;
			//bits = rowAddr + x;//(x>>rbShift);
			//shift = shiftTab[x/*&stMask*/];
			ExpandColor(colors[(bits[0]/* >> shift*/)/* & pxMask*/], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[1];
			pix.l.c1 += p.l.c1*cv.ce[1];
			ExpandColor(colors[(bits[m_nLinePitch]/* >> shift*/)/* & pxMask*/], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[3];
			pix.l.c1 += p.l.c1*cv.ce[3];
		} break;

		case 16: {
			U16 * bits = (U16 *)rowAddr + x;
			UnpackPix16(bits[0], &pix.i);
			pix.l.c0 *= cv.ce[0];
			pix.l.c1 *= cv.ce[0];
			UnpackPix16(bits[1], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[1];
			pix.l.c1 += p.l.c1*cv.ce[1];

			// Get the second row
			bits = (U16 *)((U8 *)bits + m_nLinePitch);
			UnpackPix16(bits[0], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[2];
			pix.l.c1 += p.l.c1*cv.ce[2];
			UnpackPix16(bits[1], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[3];
			pix.l.c1 += p.l.c1*cv.ce[3];
		} break;

		case 32: {
			U32 * bits = (U32 *)rowAddr + x;
			UnpackPix32(bits[0], &pix.i);
			pix.l.c0 *= cv.ce[0];
			pix.l.c1 *= cv.ce[0];
			UnpackPix32(bits[1], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[1];
			pix.l.c1 += p.l.c1*cv.ce[1];

			// Get the second row
			bits = (U32 *)((U8 *)bits + m_nLinePitch);
			UnpackPix32(bits[0], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[2];
			pix.l.c1 += p.l.c1*cv.ce[2];
			UnpackPix32(bits[1], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[3];
			pix.l.c1 += p.l.c1*cv.ce[3];
		} break;
	}

	((_XRGBIL*)c)->c0 = (pix.l.c0>>3) & 0x00FF00FF;
	((_XRGBIL*)c)->c1 = (pix.l.c1>>3) & 0x00FF00FF;
#endif
}


void XDrawDib::Draw(XDib &dib, MATRIX *pMatrix, XCXForm *pForm, XRect &rc, XBOOL bObj)
{
	//return;
	if(IsEmpty()||dib.IsEmpty()) return;

	//Reset();
	//if(m_nWidth==3200)
	//{
	//	Reset();
	//	int a=0;
	//	int b=0;
	//	//return;
	//}
    //XSWFRect clip;
	//GetRoundRect(clip,pMatrix);
	//clip&=pCnt->round;
	//if(clip.IsRectEmpty()) return 0;
	if(dib.GetBits()!=32&&m_nBits!=32) return;
	if(pMatrix==XNULL||::MatrixIsIdentity(pMatrix))
	{
		XSWFRect clip;
		clip=rc;
		//clip.ToPixel();
		clip&=XRect(0,0,dib.Width(),dib.Height());
		clip&=m_clip;
		if(clip.IsRectEmpty()) return;
		if(pForm==XNULL)
			Overlay(dib,clip.left,clip.top,clip);
		else
		{
			XDrawDib rb;
			int w=clip.Width(),h=clip.Height();
			rb.Create(w,h);
			rb.Copy(dib,0,0,clip);
			XRect rcc(0,0,w,h);
			rb.Transform(rcc,*pForm);
			Overlay(rb,clip.left,clip.top,rcc);
		}
		return;
		//if(pForm==XULL)
		//	Overlay(
	}
	else if(pMatrix->a==fixed_1&&pMatrix->b==0&&
			pMatrix->c==0&&pMatrix->d==fixed_1)
	{
		XSWFRect clip;
		clip=rc;
		clip&=m_clip;
		clip+=XPoint(-pMatrix->tx,-pMatrix->ty);
		//clip.ToPixel();
		clip&=XRect(0,0,dib.Width(),dib.Height());
		//clip&=m_clip;
		if(clip.IsRectEmpty()) return;
		int sx=pMatrix->tx+clip.left;
		int sy=pMatrix->ty+clip.top;
		if(pForm==XNULL)
			Overlay(dib,sx,sy,clip);
		else
		{
			XDrawDib rb;
			int w=clip.Width(),h=clip.Height();
			rb.Create(w,h);
			rb.Copy(dib,0,0,clip);
			XRect rcc(0,0,w,h);
			rb.Transform(rcc,*pForm);
			Overlay(rb,sx,sy,rcc);
		}
		return;
	}
	/*XSWFRect cp;
	cp.left=rc.left;
	cp.top=rc.top;
	cp.right=rc.right;
	cp.bottom=rc.bottom;
	XSWFRect clip=cp;
	//cp.GetBoundRect(clip,pMatrix);*/
	//if(pMatrix->a<0)
	//	int v=0;

	XSWFRect clip;//=rc;
	clip.left=rc.left;
	clip.right=rc.right;
	clip.top=rc.top;
	clip.bottom=rc.bottom;
	clip&=m_clip;
	if(clip.IsRectEmpty()) return;
	MATRIX invMat;
	::MatrixInvert(pMatrix,&invMat);
	//XSWFRect cp;


	invMat.tx<<=XBITMAPOFF;
	invMat.ty<<=XBITMAPOFF;
	int minx=0,//rc.left<<XBITMAPOFF,
		miny=0,//rc.top<<XBITMAPOFF,
		maxx=dib.Width()<<XBITMAPOFF,//rc.right<<XBITMAPOFF,
		maxy=dib.Height()<<XBITMAPOFF;//rc.bottom<<XBITMAPOFF;

	SPOINT pt={clip.left<<XBITMAPOFF,clip.top<<XBITMAPOFF};
	::MatrixTransformPoint(&invMat,&pt,&pt);
	int y,dx=clip.right-clip.left,mmx=dx*invMat.a,mmy=dx*invMat.b;
	RGB8*dstBuf=(RGB8*)GetBuffer();
	RGB8*srtBuf=(RGB8*)dib.GetBuffer();
	bool bPixel=Abs(invMat.a-fixed_1)<0x100;
	bool bPixelR=Abs(invMat.a+fixed_1)<0x100;
	dstBuf+=clip.top*m_nWidth;
	int sw=dib.Width();
	for(y=clip.top;y<clip.bottom;y++)
	{
		//if(y==319)
		//	int v=0;
		SPOINT bt=pt;
		int bx=clip.left,ex=clip.right;
		{
			int xx=bt.x+mmx;
			int yy=bt.y+mmy;
			while(bx<ex)
			{
				if(xx>=minx&&xx<maxx&&
					yy>=miny&&yy<maxy) break;
				ex--;
				xx-=invMat.a;
				yy-=invMat.b;
			}
		}
		{
			while(bx<ex)
			{
				if(bt.x>=minx&&bt.x<maxx&&
                   bt.y>=miny&&bt.y<maxy) break;
				bx++;
				bt.x+=invMat.a;
				bt.y+=invMat.b;
			}
		}

		if(bx<ex)
		{
			int l=ex-bx;
			//RGB8*pDst=dstBuf
			if(invMat.b==0)
			{
				if(bPixel)
				{
					RGB8*pSrt=srtBuf+(bt.y>>XBITMAPOFF)*sw+(bt.x>>XBITMAPOFF);
					while(bx<ex)
					{
						OVERRGB8((dstBuf+bx),pSrt);
						pSrt++;
						bx++;
					}
				}
				else if(bPixelR)
				{
					RGB8*pSrt=srtBuf+(bt.y>>XBITMAPOFF)*sw+(bt.x>>XBITMAPOFF);
					while(bx<ex)
					{
						OVERRGB8((dstBuf+bx),pSrt);
						pSrt--;
						bx++;
					}
				}
				else
				{
					RGB8*pSrt=srtBuf+(bt.y>>XBITMAPOFF)*sw;
					while(bx<ex)
					{
						OVERRGB8((dstBuf+bx),pSrt+(bt.x>>XBITMAPOFF));
						bt.x+=invMat.a;
						bx++;
					}
				}
			}
			else
			{
				while(bx<ex)
				{
					RGB8*pSrt=srtBuf+(bt.y>>XBITMAPOFF)*sw+(bt.x>>XBITMAPOFF);
					OVERRGB8((dstBuf+bx),pSrt);
					bt.x+=invMat.a;
					bt.y+=invMat.b;
					bx++;
				}
			}
		}
		pt.x+=invMat.c;
		pt.y+=invMat.d;
		dstBuf+=m_nWidth;
	}
    return;
}

XBOOL XDrawDib::FromDib(XDib &dib, XIntArray &points, XRect &clip)
{
	int w=clip.Width();
	int h=clip.Height();
	if(w<=0||h<=0) return XFALSE;
	m_clip.left=0;
	m_clip.top=0;
	m_clip.right=w;
	m_clip.bottom=h;
	XDib::Create(w,h,32);
	int iLeft=0,iRight=0,id0=2;
	for(int i=0;i<4;i++)
	{
		int is=(i<<1)+2;
		if(points[is]<points[id0])
		{
			id0=is;
			iRight=i;
			iLeft=i;
			if(iLeft<0) iLeft+=4;
		}
	}
	int y0=points[id0];
	XU32*pDst=((XU32*)GetBuffer())+m_nWidth*((y0>>ANTIZOOM)-clip.top);

	int ldx=0,ldy=0,rdx=0,rdy=0,ley,rey,lby,rby;
	//int tx0,ty0,tx1,ty1,ttx,tty;
	if(!CalcDDX(iLeft,-1,points,ldx,ldy,lby,ley)) return XFALSE;
	if(!CalcDDX(iRight,1,points,rdx,rdy,rby,rey)) return XFALSE;
	int bx=points[(iLeft<<1)+1];
	int ex=points[(iRight<<1)+1];
	int ilx,ily,ildx,ildy;
	int irx,iry,irdx,irdy;
	CalcRect(dib,iLeft,ilx,ily,ildx,ildy,1);
	CalcRect(dib,iRight,irx,iry,irdx,irdy,0);

	XU32*pSrt=(XU32*)dib.GetBuffer();
	//int iy=0;
	while((y0>>ANTIZOOM)<clip.bottom)
	{
		if((y0>>ANTIZOOM)>=clip.top)
		{
			//y0+=4;
			int sbx=bx;
			if(ldx!=0)
				sbx+=ldx*(y0-lby)/ldy;
			int sex=ex;
			if(rdx!=0)
				sex+=rdx*(y0-rby)/rdy;

			sbx=(sbx)>>ANTIZOOM;
			sex=(sex)>>ANTIZOOM;
			//int nBaseX=sbx;
			int ww=sex-sbx;
			if(ww>0)
			{
				int nBaseX;

				if(sex>clip.right)
					sex=clip.right;
				sbx-=clip.left;
				sex-=clip.left;
				nBaseX=sbx;

				//sex-=sbx;
				int iilx=ilx;
				if(ildx)
					iilx+=ildx*(y0-lby)/ldy;
				int iily=ily;
				if(ildy)
					iily+=ildy*(y0-lby)/ldy;
				int iirx=irx;
				if(irdx)
					iirx+=irdx*(y0-rby)/rdy;
				int iiry=iry;
				if(irdy)
					iiry+=irdy*(y0-rby)/rdy;

				int ddx=iirx-iilx;
				int ddy=iiry-iily;

				int nStepX=ddx/ww;
				int nStepY=ddy/ww;
				int nDeltaX=ddx%ww;
				int nDeltaY=ddy%ww;
				int nDTX=0,nDTY=0;
				if(sbx<0)
				{
					int dtx=-ddx*sbx;
					int dty=-ddy*sbx;
					iilx+=dtx/ww;
					iily+=dty/ww;
					nDTX=dtx%ww;
					nDTY=dty%ww;
					/*if(nDeltaX)
					{
						nDTX=dtx%nDeltaX;
						//if(nDTX<0) nDTX=-nDTX;
					}
					if(nDeltaY)
					{
						nDTY=dty%nDeltaY;
						//if(nDTY<0) nDTY=-nDTY;
					} */
					sbx=0;
				}
				//if(nDeltaX<0) nDeltaX=-nDeltaX;
				//if(nDeltaY<0) nDeltaY=-nDeltaY;
				int ddw=dib.Width();
				XU32*pSrt1=(pSrt+iilx+iily*ddw);
				int nDay=nStepY*ddw;
				for(int x=sbx;x<sex;x++)
				{
					nDTX+=nDeltaX;
					nDTY+=nDeltaY;
					pSrt1+=nStepX;
					pSrt1+=nDay;
					if(nDeltaX>=0)
					{
						if(nDTX>=ww)
						{
							nDTX-=ww;
							pSrt1++;
						}
					}
					else if(nDTX<=-ww)
					{
						nDTX+=ww;
						pSrt1--;
					}
					if(nDeltaY>=0)
					{
						if(nDTY>=ww)
						{
							nDTY-=ww;
							pSrt1+=ddw;
						}
					}
					else if(nDTY<-ww)
					{
						nDTY+=ww;
						pSrt1-=ddw;
					}
					/*if(nDeltaX&&nDeltaY)
					{

					}
					else if(nDeltaX)
					{
					}
					else if(nDeltaY)
					{
					}
					else*/
						pDst[x]=*pSrt1;//*(pSrt+iiy*dib.Width()+iix);
				}
			}
		}
		//else
		y0+=ANTISTEP;


		pDst+=m_nWidth;


		//int yy=(y0+iy);
		if((y0>>ANTIZOOM)>=clip.bottom) break;
		if(y0>=ley)
		{
			//return XTRUE;
			iLeft--;
			if(iLeft<0) iLeft+=4;
			if(!CalcDDX(iLeft,-1,points,ldx,ldy,lby,ley)) return XTRUE;
			CalcRect(dib,iLeft,ilx,ily,ildx,ildy,1);
			bx=points[(iLeft<<1)+1];

		}
		if(y0>=rey)
		{
			//return XTRUE;
			iRight++;
			if(iRight>3) iRight-=4;
			if(!CalcDDX(iRight,1,points,rdx,rdy,rby,rey)) return XTRUE;
			CalcRect(dib,iRight,irx,iry,irdx,irdy,0);
			ex=points[(iRight<<1)+1];
		}


	}
	//for(int i=
	return XTRUE;
}

inline XBOOL XDrawDib::CalcDDX(int &id, int nDelta, XIntArray &points, int &dx, int &dy, int &by, int &ey)
{
	for(int i=0;i<2;i++)
	{
		int il=id+nDelta;
		if(il<0) il+=4;
		else if(il>3) il-=4;

		dx=points[(il<<1)+1]-points[(id<<1)+1];
		ey=points[(il<<1)+2];
		by=points[(id<<1)+2];
		dy=ey-by;
		if(dy==0)
		{
			id+=nDelta;
			if(id<0) id+=4;
			else if(id>3) id-=4;
		}
	}
	if(dy<0) return XFALSE;
	//dx=(dx<<12)/dy;
	return XTRUE;
}

inline void XDrawDib::CalcRect(XDib &dib, int id, int &bx, int &by, int &dx, int &dy, XU8 bLeft)
{
	switch(id)
	{
	case 0:
		bx=0;
		by=0;
		if(bLeft)
		{
			dx=0;
			dy=dib.Height();
		}
		else
		{
			dx=dib.Width();
			dy=0;
		}
		break;
	case 1:
		bx=dib.Width()-1;
		by=0;
		if(bLeft)
		{
			dx=-(int)dib.Width();
			dy=0;
		}
		else
		{
			dx=0;
			dy=dib.Height();
		}break;
	case 2:
		bx=dib.Width()-1;
		by=dib.Height()-1;
		if(bLeft)
		{
			dx=0;
			dy=-(int)dib.Height();
		}
		else
		{
			dx=-(int)dib.Width();
			dy=0;
		}break;
	case 3:
		bx=0;
		by=dib.Height()-1;
		if(bLeft)
		{
			dx=dib.Width();
			dy=0;
		}
		else
		{
			dx=0;
			dy=-(int)dib.Height();
		}break;
	}
}

void XDrawDib::CreateData()
{
	if(!m_bAttach||!m_pDib) return;//return XTRUE;
	XU8*pData=new XU8[m_nDataLength];
	memcpy(pData,m_pDib,m_nDataLength);
	m_pDib=pData;
	m_bAttach=XFALSE;
	//return XTRUE;
}

XBOOL XDrawDib::CreateFrom(XImage*pImage)
{
	//if(pImage->Final())
	{
		int w=pImage->Width(),
			h=pImage->Height();
		if(!Create(w,h,32)) return XFALSE;
		pImage->GetBitmapData(m_pDib,w,h,32,m_nDataLength);//GetBitmapData(m_pDib);
		return XTRUE;
	}
	return XFALSE;
}

XBOOL XDrawDib::CreateFrom(SBitmapCore *bm,bool bCopy)
{
	if(!bm||!bm->HasBits()) return XFALSE;
	m_clip=XRect(0,0,bm->width,bm->height);
	if(bm->bmFormat==bm32Bit)
	{
		if(!bCopy)
		{
			XDib::Create(bm->width,bm->height,32);
			XGlobal::Memcpy(GetBuffer(),bm->baseAddr,this->m_nDataLength);
		}
		else
		{
			m_nBits=32;
			m_nColorTabs=0;

			m_nHeight=bm->height;
			m_nLinePitch=bm->rowBytes;
			m_nPixelSize=4;
			m_nWidth=bm->width;
			m_pDib=(XU8*)bm->baseAddr;
			m_nDataLength=bm->rowBytes*m_nHeight;
			m_bAttach=XTRUE;
			//bm->pAlloc=XNULL;
		}
		//bm->baseAddr=XNULL;
		return XTRUE;
	}
	else if(Create(bm->width,bm->height,32))
	{
		bm->SetDib(*this);

		return XTRUE;
	}
	return XFALSE;
}

XDrawDib* XDrawDib::Clone()
{
	XDrawDib*p=new XDrawDib;
	if(p)
	{
		p->FromDib(*this);
	}
	return p;
}

/*SBitmapCore* XDrawDib::GetBitmapCore()
{
	if(m_pSbitmap==XNULL)
	{
		m_pSbitmap=new SBitmapCore();
		m_pSbitmap->PICreate(*this,XTRUE,XNULL);
		m_pSbitmap->m_pDib=this;
		m_pSbitmap->attachDib=XTRUE;
	}
	else if(!m_pSbitmap->HasBits())
	{
		m_pSbitmap->PICreate(*this,XTRUE,XNULL);
		m_pSbitmap->m_pDib=this;
		m_pSbitmap->attachDib=XTRUE;
	}
	return m_pSbitmap;
}*/

void XDrawDib::SwapRB()
{
#ifdef __ANDROID__
	return;
#endif;
	if(!m_pDib) return;
	RGB8* pRgb=(RGB8*)m_pDib;
	int i,l=m_nWidth*m_nHeight;
	int r = 0;
	for(i=0;i<l;i++)
	{
		r=pRgb->red;
		pRgb->red=pRgb->blue;
		pRgb->blue=r;
		pRgb++;
	}
}

bool XDrawDib::Resize(XSIZE&sz)
{
	int newTotal=sz.cx*sz.cy*4;
	if(newTotal>m_nDataLength)
		int v=0;
	m_nWidth=sz.cx;
	m_nHeight=sz.cy;
	m_nLinePitch=((m_nWidth*m_nPixelSize+3)>>2)<<2;
	m_clip.left=0;
	m_clip.top=0;
	m_clip.right=m_nWidth;
	m_clip.bottom=m_nHeight;
	XRect rr(0,0,m_nWidth,m_nHeight);
	this->Clear(rr,0xff000000);
	//Reset();
	return true;
}

XBOOL XDrawDib::SetBitmapCore(SBitmapCore *bm)
{
	if(IsEmpty()||!bm) return XFALSE;
	bm->pAlloc=XNULL;
	bm->baseAddr=(char*)m_pDib;
	bm->bmFormat=bm32Bit;
	bm->cTab=XNULL;
	bm->height=m_nHeight;
	bm->width=m_nWidth;
	//bm->lockCount=0;
	bm->rowBytes=m_nLinePitch;
	return XTRUE;
}

//#ifdef _WINEMU

/*typedef struct tagBITMAPFILEHEADER {
        XU16    bfType;
        XU32    bfSize;
        XU16    bfReserved1;
        XU16    bfReserved2;
        XU32    bfOffBits;
} BITMAPFILEHEADER, FAR *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER, FAR *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;*/
#include "../libpng/BWPngEncode.h"



bool XDrawDib::SaveFile(const char* strFile)
{
	if(m_pDib==NULL)
		return false;
	LOGWHERE();

	XFile file;
	if(!file.Open(strFile,XFile::XBINARY|XFile::XWRITE|XFile::XCREATE))
		return false;
	XU8Array output;
	XPngEncode encode;
	encode.Encode(*this,output);
	file.Write(output.GetData(),output.GetSize());
	file.Close();
	/*LOGWHERE();
	 BWBITMAPFILEHEADER bmfHdr; // Header for Bitmap file
	 BWBITMAPINFOHEADER header;
	 XU32 dwDIBSize=0;

	 int m_iPitch = Width()*4;//LinePitch(width,nBits);
	 XU32 imageSize = m_iPitch * Height();
	 XU16 headSize = sizeof( BWBITMAPINFOHEADER );
	 int nSize = 0;

	 BWBITMAPINFOHEADER*pHeader=&header;
	 pHeader -> biSize = headSize;
	 pHeader -> biWidth = Width();
	 pHeader -> biHeight = Height();
	 pHeader -> biPlanes = 1;
	 pHeader -> biBitCount = 32;
	 pHeader -> biCompression = 0;//BI_RGB;
	 pHeader -> biSizeImage = imageSize;
	 pHeader -> biClrImportant = 0;//nBits<=8?(1<<nBits):0;
	 pHeader -> biClrUsed = 0;//nBits<=8?(1<<nBits):0;
	 pHeader -> biXPelsPerMeter=3780;
	 pHeader -> biYPelsPerMeter=3780;


	 bmfHdr.bfType = ('M'<<8)|'B';//DIB_HEADER_MARKER;  // "BM"
	 dwDIBSize = headSize;  // Partial Calculation
	 {
		XU32 dwBmBitsSize;  // Size of Bitmap Bits only
		dwBmBitsSize = imageSize;
		dwDIBSize += dwBmBitsSize;
	}
	bmfHdr.bfSize = dwDIBSize + sizeof(BWBITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (XU32)sizeof(BWBITMAPFILEHEADER) + pHeader->biSize;

	file.Write(&bmfHdr, sizeof(BWBITMAPFILEHEADER));
	file.Write(pHeader,sizeof(BWBITMAPINFOHEADER));
	{
		int nSize=imageSize;
		XU8*pData=GetBuffer();
		while(nSize)
		{
			int l=XMIN(nSize,512000);
			file.Write(pData,l);
			pData+=l;
			nSize-=l;
			//file.Write(GetBuffer(), imageSize);
		}

	}
	file.Close();*/

	return true;
}

//#endif
