// XGraphics.cpp: implementation of the XGraphics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XGraphics.h"
#include "XString.h"
#include "XResource.h"
#include "XWindow.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define _strFont XResource::LoadWString(XIDS_FONT)

#define _XGDIDRAW

XGraphics::XGraphics()
{
//   	m_pCatched=XNULL;
	
	m_pBitmap=XNULL;	
	m_pFont=XNULL;
	m_pRgn=XNULL;
	m_pGraphics=XNULL;
	Init();
}

XGraphics::~XGraphics()
{
   //if(m_pCatched!=XNULL) delete (CachedBitmap*)m_pCatched;
	Release();
}

//#define _XGDIDRAW
XGraphics::XGraphics(XHANDLE hHandle)
{
	m_pGraphics=XNULL;
	m_pFont=XNULL;
	m_pRgn=XNULL;
	m_pBitmap=XNULL;
	m_nTextHeight=16;
	//m_pDC=hHandle;
#ifdef _XGDIDRAW
	m_pGraphics=Graphics::FromHDC((HDC)hHandle);
#else
	m_pGraphics=hHandle;
#endif
	Init();
//	m_pCatched=XNULL;
	//return m_pGraphics!=XNULL;
}

void XGraphics::DrawString(XPCWSTR pText, const XPoint &pt,XINT nLength)
{
#ifdef _XGDIDRAW
	PointF point((REAL)pt.x,(REAL)pt.y);
	SolidBrush brush(m_nTextColor);
    ((Graphics*)m_pGraphics)->DrawString(pText,nLength,(Font*)GetFont(),point,
		&brush);
#else
	GetFont();
	::SetBkMode((HDC)m_pGraphics,TRANSPARENT);
	::SetBkColor((HDC)m_pGraphics,DCCOLOR(m_nBackColor));
	::SetTextColor((HDC)m_pGraphics,DCCOLOR(m_nTextColor));
	if(nLength<0) 
		nLength=wcslen(pText);
	::TextOutW((HDC)m_pGraphics,pt.x,pt.y,pText,nLength);
#endif
}

void XGraphics::DrawString(XPCWSTR pText, const XRect &rect,XINT nLength)
{
#ifdef _XGDIDRAW
	SolidBrush brush(m_nTextColor);
	RectF rt((REAL)rect.left,(REAL)rect.top,(REAL)rect.Width(),(REAL)rect.Height());
    ((Graphics*)m_pGraphics)->DrawString(pText,nLength,
		(Font*)GetFont(),rt,XNULL,&brush);
#else
	if(nLength<0) 
		nLength=wcslen(pText);
	GetFont();
	::SetBkMode((HDC)m_pGraphics,TRANSPARENT);
	::SetBkColor((HDC)m_pGraphics,DCCOLOR(m_nBackColor));
	::SetTextColor((HDC)m_pGraphics,DCCOLOR(m_nTextColor));
	::DrawTextW((HDC)m_pGraphics,pText,nLength,(LPRECT)&rect,DT_LEFT);
#endif
}

void XGraphics::DrawString(XPCWSTR pText, int x,int y,XINT nLength)
{
#ifdef _XGDIDRAW
	Graphics*g=(Graphics*)m_pGraphics;
	SolidBrush brush(m_nTextColor);
    g->DrawString(pText,nLength,
		(Font*)GetFont(),PointF((REAL)x,(REAL)y),
		&brush);
#else
	GetFont();
	if(nLength<0) 
		nLength=wcslen(pText);
	::SetBkMode((HDC)m_pGraphics,TRANSPARENT);
	::SetBkColor((HDC)m_pGraphics,DCCOLOR(m_nBackColor));
	::SetTextColor((HDC)m_pGraphics,DCCOLOR(m_nTextColor));
	::TextOutW((HDC)m_pGraphics,x,y,pText,nLength);
#endif
}


void XGraphics::MeasureString(XPCWSTR pText,XSIZE &size,XINT nLength)
{
#ifdef _XGDIDRAW
	GetFont();
	RectF rect;
	if(nLength==0)
	{
	   //((Graphics*)m_pGraphics)->MeasureString(_strA,1,(Font*)GetFont(),PointF(0,0),NULL,&rect);
	   size.cx=0;
	   size.cy=m_nTextHeight;//(XINT)rect.Height;
	}
	else
	{
		if(nLength<0) nLength=wcslen(pText);
		//return nLength<<4;
		//size.cx=nLength*18;
		//size.cy=18;
		//return;

		((Graphics*)m_pGraphics)->MeasureString(pText,nLength,(Font*)GetFont(),PointF(0,0),NULL,&rect);
		nLength--;
		size.cx=(XINT)rect.Width-2;
		size.cy=m_nTextHeight;//(XINT)m_nTextHeight;//rect.Height;
		while(nLength>=0)
		{
			if(pText[nLength]!=' ') break;
			nLength--;
			size.cx+=size.cy/3;
		}
		
	}
	/*RectF rect,rt;
	
	((Graphics*)m_pGraphics)->MeasureString(_strA,1,(Font*)GetFont(),PointF(0,0),NULL,&rt);
	if(nLength==0)
	{
		size.cx=0;
		size.cy=(XINT)rt.Height;
		return;
	}
	XString16 strTxt(pText,nLength>=0?nLength:0);
	strTxt+=_strA;

	((Graphics*)m_pGraphics)->MeasureString(strTxt,-1,(Font*)GetFont(),PointF(0,0),NULL,&rect);
	size.cx=(XINT)(rect.Width-rt.Width+2);
	size.cy=(XINT)(rect.Height);*/
#else
	GetFont();
	if(nLength<0) nLength=wcslen(pText);
	::GetTextExtentPoint32W((HDC)m_pGraphics,pText,nLength,(LPSIZE)&size);
	//size.cy+=2;
	size.cy=m_nTextHeight;
#endif
}

void XGraphics::FillRectangle(const XRect &rect)
{
#ifdef _XGDIDRAW
	Rect rt(rect.left,rect.top,rect.Width(),rect.Height());
	//if((m_nBackColor&0xffffff)==0xffffff)
	//	int vv=0;
	SolidBrush brush(m_nBackColor);
	((Graphics*)m_pGraphics)->FillRectangle(&brush,rt);
#else
	HBRUSH brush=::CreateSolidBrush(DCCOLOR(m_nBackColor));
	BOOL bOK=::FillRect((HDC)m_pGraphics,(LPRECT)&rect,brush);
	::DeleteObject(brush);
#endif
}

void XGraphics::DrawImage(const XImage &img,const XRect &rect)
{
#ifdef _XGDIDRAW
	Graphics* p=(Graphics*)m_pGraphics;
#else
	Graphics* p=Graphics::FromHDC((HDC)m_pGraphics);
#endif
	Rect rt(rect.left,rect.top,rect.Width(),rect.Height());
	p->DrawImage((Image*)img.GetImage(),rt);
#ifdef _XGDIDRAW
#else
	delete p;
#endif
}

void XGraphics::DrawImage(const XImage&img,const XPoint&dst,const XRect&rst)
{
#ifdef _XGDIDRAW
	Graphics* p=(Graphics*)m_pGraphics;
#else
	Graphics* p=Graphics::FromHDC((HDC)m_pGraphics);
#endif
	p->DrawImage((Image*)img.GetImage(),
						dst.x,dst.y,
						rst.left,rst.top,
						rst.Width(),rst.Height(),UnitPixel);
#ifdef _XGDIDRAW
#else
	delete p;
#endif
}


void XGraphics::DrawImage(const XImage &img,const XPoint &point,int rotate)
{
#ifdef _XGDIDRAW
	Graphics* p=(Graphics*)m_pGraphics;
#else
	Graphics* p=Graphics::FromHDC((HDC)m_pGraphics);
#endif
	if(rotate)
		p->RotateTransform((REAL)rotate);
	
	p->DrawImage((Image*)img.GetImage(),point.x,point.y);
	if(rotate)
		p->RotateTransform(0);
#ifdef _XGDIDRAW
#else
	delete p;
#endif
}

void XGraphics::FrameRectangle(const XRect &rect)
{
#ifdef _XGDIDRAW
	Pen pen(m_nTextColor);
   Rect rt(rect.left,rect.top,rect.Width()-1,rect.Height()-1);
   ((Graphics*)m_pGraphics)->DrawRectangle(&pen,rt);
#else
   HBRUSH brush=::CreateSolidBrush(DCCOLOR(m_nTextColor));
   ::FrameRect((HDC)m_pGraphics,(RECT*)&rect,brush);
   ::DeleteObject(brush);
#endif
}

void XGraphics::DrawLine(int sx, int sy, int ex, int ey,XU8 w,XU8 nStyle)
{
#ifdef _XGDIDRAW
	Pen pen(m_nTextColor,w);
	if(nStyle!=XDLS_SOLID)
		pen.SetDashStyle((DashStyle)nStyle);
	((Graphics*)m_pGraphics)->DrawLine(&pen,sx,sy,ex,ey);
#else
	HPEN pen=::CreatePen(nStyle,1,DCCOLOR(m_nTextColor));
	HPEN old=(HPEN)::SelectObject((HDC)m_pGraphics,pen);
	::MoveToEx((HDC)m_pGraphics,sx,sy,NULL);
	::LineTo((HDC)m_pGraphics,ex,ey);
	::SetPixel((HDC)m_pGraphics,ex,ey,DCCOLOR(m_nTextColor));
	::SelectObject((HDC)m_pGraphics,old);
	::DeleteObject(pen);
#endif

}

void XGraphics::DrawButton(const XRect &r, XU32 c,XU8 b)
{
   XRect rect=r;
   XColor cr(c);
   if(!(b&XBS_NOFILL))
   {
	SetBackColor(cr);
	FillRectangle(rect);
   }
   //((Graphics*)m_pGraphics)->FillRectangle(brush,(Rect)rect);
   if(b&XBS_WFRAME)
   {
	   XColor cc(c);
	   cc.Bright(50);
	   SetColor(cc);
	   FrameRectangle(rect);
	   rect.DeflateRect(1,1);
   }
   if(b&XBS_BFRAME)
   {
	   cr.Dark(30);
	   SetColor(cr);
	   rect.bottom--;
	   rect.right--;
	   DrawLine(rect.left+1,rect.top,rect.right-1,rect.top);
	   DrawLine(rect.left,rect.top+1,rect.left,rect.bottom-1);
	   cr.Dark(20);
	   SetColor(cr);
	   DrawLine(rect.left+1,rect.bottom,rect.right-1,rect.bottom);
	   DrawLine(rect.right,rect.top+1,rect.right,rect.bottom-1);
	   rect.DeflateRect(1,1);
   }
   else {rect.right--;rect.bottom--;}
   cr=c;
   cr.Bright(20);
   SetColor(cr);
   
   int cx=rect.Width(),cy=rect.Height();
   DrawLine(rect.left+1,rect.top,rect.left+(cx>>1),rect.top);
   DrawLine(rect.left,rect.top+1,rect.left,rect.top+(cy>>1));
   cr=c;
   cr.Bright(16);
   SetColor(cr);
   DrawLine(rect.left+(cx>>1)+1,rect.top,rect.right-1,rect.top);
   DrawLine(rect.left,rect.top+(cy>>1)+1,rect.left,rect.bottom-1);
   //if(b&XBS_HHAND)
   {
	   for(int i=0;i<3;i++)
	   {
		  if(b&XBS_HHAND)
			DrawLine(rect.left+2,rect.top+(cy>>1)-3+i*3,rect.right-3,rect.top+(cy>>1)-3+i*3);
		  else if(b&XBS_VHAND)
			DrawLine(rect.left+(cx>>1)-3+i*3,rect.top+2,rect.left+(cx>>1)-3+i*3,rect.bottom-3);
	   }
   }
   if(b&XBS_THICK)
   {
	DrawLine(rect.left+2,rect.top+1,rect.left+(cx/3),rect.top+1);
	DrawLine(rect.left+1,rect.top+2,rect.left+1,rect.top+(cy/3));
	cr=c;
	cr.Bright(10);
	SetColor(cr);
	DrawLine(rect.left+(cx/3)+1,rect.top+1,rect.right-2,rect.top+1);
	DrawLine(rect.left+1,rect.top+(cy/3)+1,rect.left+1,rect.bottom-2);
   }
   cr=c;
   cr.Dark(24);
   SetColor(cr);
   DrawLine(rect.left+(cx>>1),rect.bottom,rect.right-1,rect.bottom);
   DrawLine(rect.right,rect.top+(cy>>1),rect.right,rect.bottom-1);

   {
	   for(int i=0;i<3;i++)
	   {
		  if(b&XBS_HHAND)
			DrawLine(rect.left+3,rect.top+(cy>>1)-2+i*3,rect.right-2,rect.top+(cy>>1)-2+i*3);
		  else if(b&XBS_VHAND)
			DrawLine(rect.left+(cx>>1)-2+i*3,rect.top+3,rect.left+(cx>>1)-2+i*3,rect.bottom-2);
	   }
   }

   cr=c;
   cr.Dark(16);
   SetColor(cr);
   DrawLine(rect.left+1,rect.bottom,rect.left+(cx>>1)-1,rect.bottom);
   DrawLine(rect.right,rect.top+1,rect.right,rect.top+(cy>>1)-1);

   

   if(b&XBS_THICK)
   {
	 DrawLine(rect.right-(cx/3),rect.bottom-1,rect.right-2,rect.bottom-1);
	 DrawLine(rect.right-1,rect.bottom-(cy/3),rect.right-1,rect.bottom-2);
	 cr=c;
	 cr.Dark(8);
	 SetColor(cr);
	 DrawLine(rect.left+2,rect.bottom-1,rect.right-(cx/3)-1,rect.bottom-1);
	 DrawLine(rect.right-1,rect.top+2,rect.right-1,rect.bottom-(cy/3)-1);
   }
   //DrawLine(
}

void XGraphics::SetClip(const XRect &rect)
{
   m_clip=rect;
#ifdef _XGDIDRAW
   Rect rt(rect.left,rect.top,rect.Width(),rect.Height());
   ((Graphics*)m_pGraphics)->SetClip(rt);
#else
   int x,y;
   GetOrigin(x,y);

   //rect+=XPoint(x,y);
   HRGN pOld=(HRGN)m_pRgn;
   
   m_pRgn=CreateRectRgn(rect.left+x,rect.top+y,rect.right+x,rect.bottom+y);
   ::SelectClipRgn((HDC)m_pGraphics,(HRGN)m_pRgn);
   RECT rt;
   ::GetClipBox((HDC)m_pGraphics,&rt);

   if(pOld!=XNULL) ::DeleteObject(pOld);

  // ::DeleteObject(rgn);
   
   
#endif
}

void XGraphics::GetClip(XRect &clip)
{
   clip=m_clip;
}

void XGraphics::SetOrigin(XINT x, XINT y)
{
   //Matrix m;
   //m.Translate(x,y);
//	m_offset.x=x;
//	m_offset.y=y;
#ifdef _XGDIDRAW
   ((Graphics*)m_pGraphics)->ResetTransform();
   if(x!=0||y!=0)
   ((Graphics*)m_pGraphics)->TranslateTransform((REAL)x,(REAL)y);
#else
   POINT pt;
   BOOL bOK=SetViewportOrgEx((HDC)m_pGraphics,x,y,&pt);
#endif
   m_origin.x=x;
   m_origin.y=y;
}

void XGraphics::GetOrigin(XINT&x,XINT&y)
{
#ifdef _XGDIDRAW
	Matrix m;
	((Graphics*)m_pGraphics)->GetTransform(&m);
	x=(XINT)m.OffsetX();
	y=(XINT)m.OffsetY();
#else
	POINT pt;
	::GetViewportOrgEx((HDC)m_pGraphics,&pt);
	x=pt.x;
	y=pt.y;
#endif
	//x=m_offset.x;
	//y=m_offset.y;
}

void XGraphics::DrawDirect(const XRect&rect,XU8 nDirect, XU32 c,XU8 bGray)
{
	XColor cc(c);
	cc.Bright(bGray?50:70);
	//cc.Mix(c,0.4f);
	SetColor(cc);
	int dd=(rect.Width()-8)>>1;
	int bx=rect.left+4,by=rect.bottom-4;
	switch(nDirect)
	{
	case XK_UP:
		 DrawLine(bx,by-1,bx+dd,by-dd-1);
		 DrawLine(bx+dd,by-dd-1,bx+(dd<<1),by-1);
		 break;
	case XK_DOWN:
		 DrawLine(bx,by-dd,bx+dd,by);
		 DrawLine(bx+dd,by,bx+(dd<<1),by-dd);
		 break;
	case XK_LEFT:
		 DrawLine(bx+dd+2,by-(dd<<1),bx+2,by-dd);
		 DrawLine(bx+2,by-dd,bx+dd+2,by);
		 break;
	case XK_RIGHT:
		 DrawLine(bx+3,by-(dd<<1),bx+3+dd,by-dd);
		 DrawLine(bx+3+dd,by-dd,bx+3,by);
	}
	cc=c;
	//cc.Dark(bGray?20:50);
	if(bGray) cc.Bright(20);
	SetColor(cc);
	for(int i=1;i<3;i++)
	{
	switch(nDirect)
	{
		case XK_UP:
			 DrawLine(bx,by-1-i,bx+dd,by-dd-1-i);
			 DrawLine(bx+dd,by-dd-1-i,bx+(dd<<1),by-1-i);
			 break;
		case XK_DOWN:
			 DrawLine(bx,by-dd-i,bx+dd,by-i);
			 DrawLine(bx+dd,by-i,bx+(dd<<1),by-dd-i);
			 break;
		case XK_LEFT:
			 DrawLine(bx+dd+2-i,by-(dd<<1),bx+2-i,by-dd);
			 DrawLine(bx+2-i,by-dd,bx+dd+2-i,by);
			 break;
		case XK_RIGHT:
			 DrawLine(bx+3-i,by-(dd<<1),bx+3+dd-i,by-dd);
			 DrawLine(bx+3-i+dd,by-dd,bx+3-i,by);
		}
	}

}

void XGraphics::DrawCheck(const XRect &rect, XU32 c,XU32 c1)
{
	int dd=rect.Width()/3;
	
	for(int i=0;i<3;i++)
	{		
		if(i!=1)
			SetColor(c1);
		else SetColor(c);
		DrawLine(rect.left,rect.bottom-dd-i,rect.left+dd,rect.bottom-i);
		DrawLine(rect.left+dd,rect.bottom-i,rect.left+dd*3,rect.bottom-i-dd*2);
	}
}


#define ISALPHA(CC) (CC<0x80&&::isalpha(CC))
#define ISNUM(CC) (CC<0x80&&::isdigit(CC))
#define ISMARK(CC) (::wcschr(XResource::LoadWString(XIDS_MARK),CC)!=XNULL)
#define ISLEFTMARK(CC) (::wcschr(XResource::LoadWString(XIDS_LEFTMARK),CC)!=XNULL)

XU16 XGraphics::FormatString(XPCWSTR pData,int len,int bx, int win, DOMPOSARRAY &list,XU8 bWord,DOMPOS*pSpan)
{
	XSIZE sz;//(0,0);
	sz.cx=0;
	sz.cy=0;
	
	
	XU16 nMax=(XU16)(XMIN(len,0xfffe));
	XU8  bStart=bx==0;
	
	int w=win,ssy=0;
	if(pSpan!=XNULL)
	{
	   if(ssy<pSpan[3]) w=pSpan[2];
	}
	XU16 iBegin=0,ei=XMIN(XMAX(2+((w-bx)>>5),2),nMax),si=0,mi=ei,ci;
	while(iBegin<nMax)
	{
		MeasureString(pData,sz,ei);
		ci=ei;
		while(sz.cx+bx<=w)
		{
			si=ei;
			if(ei+iBegin>=nMax) break;
		    ei=XMIN(ei+mi,nMax-iBegin);			
			MeasureString(pData,sz,ei);
			ci=ei;
		}
		mi=(ei+si)>>1;
		while(ei>si+1)
		{
			MeasureString(pData,sz,mi);
			ci=mi;
			if(sz.cx+bx>w)
			{
				ei=mi;
				//sz.cx-=(sz.cy>>1)+2;
			}
			else
				si=mi;
			mi=(ei+si)>>1;
		}
		ei=mi;
		if(XFALSE)//(bWord&XFMT_WORDALIGN)&&si>1&&si+iBegin<nMax)
		{
			if(ISALPHA(pData[si-1]))
			{
			  mi=si;
			  while(mi>0&&(ISALPHA(pData[mi-1])||ISLEFTMARK(pData[mi-1]))) 
				  mi--;
			  if(mi>0) si=mi;
			}
		   else if(ISNUM(pData[si-1]))
		   {
			  mi=si;
			  while(mi>1&&(ISNUM(pData[mi-1])||ISLEFTMARK(pData[mi-1])))
				  mi--;
			  if(mi>1) si=mi;
		   }
		   else if(si+iBegin<nMax&&ISMARK(pData[si]))
		   {
			   if(si+1+iBegin>=nMax||
				  !ISMARK(pData[si+1]))
			   si++;
		   }
		   else if(si>2&&ISLEFTMARK(pData[si-1]))
			   si--;
		}

		if(si==0&&bStart) si++;
		//if(si>0)
		{
			if(si!=ci) MeasureString(pData,sz,si);
			list.Add(iBegin);
			list.Add(si);
			list.Add(si>0?sz.cx:0);
		}
#ifdef _DEBUG
		if(list.GetSize()>9)
		{
			int a=0;
		}
#endif
		bStart=XTRUE;

		if(pSpan!=XNULL)
		{
		   ssy+=sz.cy;
		   if(ssy<pSpan[3]) w=pSpan[2];
		   else w=win;
		   if(ssy<pSpan[1]) bx=pSpan[0];
		   else bx=0;
		}
		else
			bx=0;
		bStart=XTRUE;
		iBegin+=si;
		pData+=si;
		ei=XMIN(2+((XMAX(w-bx,0))>>5),nMax-iBegin);
		mi=ei;
		si=0;
		if((bWord&XFMT_ONELINE)&&list.GetSize()>0) return (XU16)sz.cy;
		//pData+=iBegin;
	}
	return (XU16)sz.cy;
}

void XGraphics::DrawFrame(const XRect &rect, XU32 c, XU8 s,XU8 bSet)
{
	XRect r=rect;
	for(XU8 i=0;i<s;i++)
	{
		int v=(i+1)*40/s;
		XColor cc=c;
		if(bSet)
			cc.Bright(50-v);
		else
			cc.Dark(v+10);
		SetColor(cc);
		DrawLine(r.left,r.top,r.right,r.top);
		DrawLine(r.left,r.top,r.left,r.bottom);
		cc=c;
		if(bSet)
			cc.Dark(v+10);
		else
			cc.Bright(50-v);
		SetColor(cc);
		DrawLine(r.right,r.top,r.right,r.bottom);
		DrawLine(r.left,r.bottom,r.right,r.bottom);
		r.InflateRect(-1,-1);
	}
}



void XGraphics::DrawBar(const XRect &rect, XU32 c, XU8 dc,XU8 bHorz)
{	
	XColor cc(c);
	if(bHorz)
	{
		int my=((rect.bottom-rect.top+1)>>1);
		for(int y=0;y<my;y++)
		{
		   SetColor(cc);
		   cc.Bright(dc);
		   DrawLine(rect.left,rect.top+y,rect.right,rect.top+y);
		   DrawLine(rect.left,rect.bottom-y-1,rect.right,rect.bottom-y-1);
		}
	}
}


XBOOL XGraphics::Scroll(const XPoint &dst, const XRect &rst)
{
	if(m_pBitmap==XNULL) return XFALSE;
#ifdef _XGDIDRAW
	Bitmap*pBmp=(Bitmap*)m_pBitmap;
	Graphics*p=(Graphics*)m_pGraphics;
	//int w=pBmp->GetWidth();
	//int h=pBmp->GetHeight();
	Bitmap*pClone=pBmp->Clone(rst.left+m_origin.x,rst.top+m_origin.y,rst.Width(),rst.Height(),PixelFormat32bppARGB);
	p->DrawImage(pClone,dst.x,dst.y);//pClone,dst.x,dst.y,0,0,rst.Width(),rst.Height(),UnitPixel);
	delete pClone;
	//Status s=p->DrawImage(pBmp,
	//			dst.x,dst.y,rst.left,rst.top,rst.Width(),rst.Height(),UnitPixel);
#else
	::BitBlt((HDC)m_pGraphics,dst.x,dst.y,rst.Width(),rst.Height(),
			(HDC)m_pGraphics,rst.left,rst.top,SRCCOPY);
#endif
	return XTRUE;
}


void XGraphics::DrawRadio(const XRect &inRect, XU32 nColor,XU8 bSelect)
{
	XRect rect = inRect;
	rect.DeflateRect(1,1);

	rect.right++;
	rect.bottom++;
	XColor wc(nColor);
	wc.Bright(20);
	SetBackColor(wc);
	FillEllipse(rect);

	rect-=XPoint(1,1);
	XColor gc(nColor);
	gc.Dark(30);
	SetBackColor(gc);
	FillEllipse(rect);

	rect.DeflateRect(1,1);
	XColor dc(gc);
	dc.Dark(50);
	SetBackColor(dc);
	FillEllipse(rect);

	rect.left++;
	rect.top++;
	SetBackColor(wc);
	FillEllipse(rect);

	if(bSelect)
	{
	  rect.DeflateRect(1,1);
	  SetBackColor(gc);
	  FillEllipse(rect);
	  SetBackColor(dc);
	  rect.DeflateRect(1,1);
	  FillEllipse(rect);
	}


	//rect-=XPoint(1,1);
	/*XColor cc(nColor);
	cc.Dark(30);
	rect.left--;
	rect.top--;
	SetBackColor(cc);
	FillEllipse(rect);
	rect.left++;
	rect.top++;

	XColor wc(nColor);
	wc.Bright(30);
	rect+=XPoint(1,1);
	SetBackColor(wc);
	FillEllipse(rect);
	rect-=XPoint(1,1);

	rect.right--;
	rect.bottom--;
	XColor dc(cc);
	dc.Dark(40);
	SetBackColor(dc);
	FillEllipse(rect);*/
	
	
    /*XColor cc(nColor);
	cc.Bright(30);
	SetBackColor(cc);
	FillEllipse(rect);
	XColor bc(nColor);
	bc.Dark(50);
	SetColor(bc);
	//rect-=XPoint(1,1);
	DrawEllipse(rect);
	SetColor(cc);*/
	/*float s=XANGLE(45);
	float e=XANGLE(225);
	float ee=XANGLE(405);*/
	//rect+=XPoint(1,1);
	//rect.InflateRect(1,1);
	//SetColor(0xffff0000);
	//DrawPie(rect,225.0f,180.0f);
}

void XGraphics::DrawCheckBox(const XRect &inRect, XU32 c, XU8 bCheck)
{
	XRect rect = inRect;
	 XColor cc(c);
	 cc.Bright(30);
	 SetBackColor(cc);
	 FillRectangle(rect);
	 XColor cg(c);
	 cg.Dark(20);
	 DrawFrame(rect,cg,2);
	 if(bCheck)
	 {
		 rect.DeflateRect(2,2);
		 //rect.left++;
		 rect.top--;
		 rect.bottom--;
		 cc=c;
		 cc.Dark(80);
		 DrawCheck(rect,cc,cg);
	 }
}

void XGraphics::DrawEllipse(const XRect &rect)
{
#ifdef _XGDIDRAW
   Pen pen(m_nTextColor);
   Rect rt(rect.left,rect.top,rect.Width(),rect.Height());
	((Graphics*)m_pGraphics)->DrawEllipse(&pen,rt);
#else
	::Ellipse((HDC)m_pGraphics,rect.left,rect.top,rect.right,rect.bottom);
#endif
}

//DEL void XGraphics::DrawPie(XRect &rect, float fStart, float fEnd)
//DEL {
//DEL #ifdef _XGDIDRAW
//DEL 	Pen pen(m_nTextColor);
//DEL     Rect rt(rect.left,rect.top,rect.Width(),rect.Height());
//DEL 	((Graphics*)m_pGraphics)->DrawPie(&pen,rt,360-fStart,-fEnd);
//DEL #else
//DEL 	//::Arc((HDC)m_pGraphics,rect.left,rect.top,rect.right,rect.bottom,
//DEL 	//	  (int)fStart,(int)fEnd);			
//DEL #endif
//DEL }

void XGraphics::FillEllipse(const XRect &rect)
{
#ifdef _XGDIDRAW
	SolidBrush brush(m_nBackColor);
    Rect rt(rect.left,rect.top,rect.Width(),rect.Height());
	((Graphics*)m_pGraphics)->FillEllipse(&brush,rt);
#else
	/*HBRUSH brush=::CreateSolidBrush(DCCOLOR(m_nBackColor));
	HBRUSH old=(HBRUSH)::SelectObject((HDC)m_pGraphics,brush);
	::FillEllipse((HDC)m_pGraphics,rect.left,rect.top,rect.right,rect.bottom);
	::SelectObject((HDC)m_pGraphics,brush);
	::DeleteObject(brush);*/
	Rect rt(rect.left,rect.top,rect.Width(),rect.Height());
	Graphics*p=Graphics::FromHDC((HDC)m_pGraphics);
	p->FillEllipse(&SolidBrush(m_nBackColor),rt);
	delete p;
#endif
}

//DEL void XGraphics::FillPie(XRect &rect, float fStart, float fEnd)
//DEL {
//DEL 	
//DEL     Rect rt(rect.left,rect.top,rect.Width(),rect.Height());
//DEL 	((Graphics*)m_pGraphics)->FillPie(&SolidBrush(m_nBackColor),rt,
//DEL 			    360-fStart,-fEnd);
//DEL }

XGraphics::XGraphics(int w, int h)
{
	m_pBitmap=XNULL;
	m_pFont=XNULL;
	m_pRgn=XNULL;
	m_pGraphics=XNULL;
	Create(w,h);
	Init();
}

void XGraphics::Create(int w, int h,XU8 nBits)
{
	Release();
#ifdef _XGDIDRAW
	XU32 nFmt=PixelFormat32bppARGB;
	if(nBits<=8)
		nFmt=PixelFormat8bppIndexed;
	else if(nBits<=16)
		nFmt=PixelFormat16bppARGB1555;
	else if(nBits<=24)
		nFmt=PixelFormat24bppRGB;
	m_pBitmap=new Bitmap(w,h,nFmt);
	//m_pBitmap->

	m_pGraphics=Graphics::FromImage((Image*)m_pBitmap);
	//((Graphics*)m_pGraphics)->SetSmoothingMode(SmoothingModeHighSpeed);
#else
	HDC dc=::GetWindowDC(NULL);
	m_pBitmap=::CreateCompatibleBitmap(dc,w,h);
	m_pGraphics=::CreateCompatibleDC(dc);
	::SelectObject((HDC)m_pGraphics,(HBITMAP)m_pBitmap);
#endif
}

//DEL int XGraphics::Width()
//DEL {	 
//DEL 	if(m_pBitmap==XNULL) return 0;
//DEL #ifdef _XGDIDRAW
//DEL     
//DEL 	return ((Bitmap*)m_pBitmap)->GetWidth();
//DEL #else
//DEL 	SIZE sz;
//DEL 	GetBitmapDimensionEx((HBITMAP)m_pBitmap,&sz);
//DEL 	return sz.cx;
//DEL #endif
//DEL }

//DEL int XGraphics::Height()
//DEL {
//DEL 	if(m_pBitmap==XNULL) return 0;
//DEL #ifdef _XGDIDRAW
//DEL 	return ((Bitmap*)m_pBitmap)->GetHeight();
//DEL #else
//DEL 	SIZE sz;
//DEL 	GetBitmapDimensionEx((HBITMAP)m_pBitmap,&sz);
//DEL 	return sz.cx;
//DEL #endif
//DEL }

XBOOL XGraphics::Copy(XGraphics&g, const XPoint&dst, const XRect&rst)
{
	if(g.m_pBitmap==XNULL) return XFALSE;
#ifdef _XGDIDRAW
	Bitmap*pBmp=(Bitmap*)g.m_pBitmap;
	Graphics*p=(Graphics*)m_pGraphics;
	p->DrawImage(pBmp,
				dst.x,dst.y,rst.left,rst.top,rst.Width(),rst.Height(),UnitPixel);
#else
	::BitBlt((HDC)m_pGraphics,dst.x,dst.y,rst.Width(),rst.Height(),
			(HDC)g.m_pGraphics,rst.left,rst.top,SRCCOPY);
#endif
	return XTRUE;
}

XU32 XGraphics::GetPixel(int x, int y)
{
    if(m_pBitmap==XNULL) return 0;
#ifdef _XGDIDRAW
	Color c;
	((Bitmap*)m_pBitmap)->GetPixel(x,y,&c);
	return c.GetValue();
#else
	return ::GetPixel((HDC)m_pGraphics,x,y);
#endif
}

XU32 XGraphics::SetColor(XU32 nColor) 
	{
		XU32 c=m_nTextColor;
		m_nTextColor=nColor;
		return c;
	}
XU32 XGraphics::SetBackColor(XU32 nColor)
{
	XU32 c=m_nBackColor;
	m_nBackColor=nColor;
	return c;
}


void XGraphics::Release()
{
#ifdef _XGDIDRAW
	if(m_pBitmap!=XNULL) delete (Bitmap*)m_pBitmap;
    m_pBitmap=XNULL;
	if(m_pFont!=XNULL) delete (Font*)m_pFont;
	m_pFont=XNULL;
	if(m_pGraphics)
		delete (Graphics*)m_pGraphics;
#else
	if(m_pRgn!=XNULL)
		::DeleteObject((HRGN)m_pRgn);
	m_pRgn=XNULL;
	if(m_pBitmap!=XNULL)
	{
		::DeleteObject((HBITMAP)m_pBitmap);
		::DeleteObject((HDC)m_pGraphics);
	}
	if(m_pFont!=XNULL) ::DeleteObject(m_pFont);
	m_pBitmap=XNULL;
	m_pGraphics=XNULL;
#endif
}

void XGraphics::Init()
{
	m_nTextHeight=18;
	m_nTextColor=0xff000000;
	m_nBackColor=0xffefffef;
	m_nFontStyle=XWindow::m_nTextSize<<8;
		//XS_NORMAL<<8;
//	m_offset.x=0;
//	m_offset.y=0;
#ifdef _XGDIDRAW
#else
	if(m_pGraphics!=XNULL)
	{
		GetFont();
		//::SetTextBac
		::SetBkMode((HDC)m_pGraphics,TRANSPARENT);
		::SetBkColor((HDC)m_pGraphics,DCCOLOR(m_nBackColor));
		::SetTextColor((HDC)m_pGraphics,DCCOLOR(m_nTextColor));
	}
#endif
}

//#define _strA XResource::LoadWString(XIDS_HELLO)

void* XGraphics::GetFont()
{
   if(m_nFontStyle&XF_UPDATE)
   {
	   if(m_pFont!=NULL) 
#ifdef _XGDIDRAW
		   delete (Font*)m_pFont;
#else
	   ::DeleteObject((HFONT)m_pFont);
#endif
	   m_pFont=NULL;
	   m_nFontStyle&=~XF_UPDATE;
   }
   if(m_pFont==NULL)
   {
#ifdef _XGDIDRAW
	   m_pFont=new Font(_strFont,
						(REAL)(((m_nFontStyle>>8)&0xff)+9),//8),
						m_nFontStyle&0xff);
	   RectF rect;
	   ((Graphics*)m_pGraphics)->MeasureString(XResource::LoadWString(XIDS_HELLO),-1,(Font*)m_pFont,PointF(0,0),NULL,&rect);
	   m_nTextHeight=(XINT)rect.Height;
#else
	   int h=((m_nFontStyle>>8)&0xff)*1+14;
	   m_pFont=::CreateFontW(h,h>>1,
			0,//(m_nFontStyle&XF_ITALIC)?10:0,
			0,
			(m_nFontStyle&XF_BOLD)?FW_BOLD:FW_NORMAL,
			(m_nFontStyle&XF_ITALIC),
			(m_nFontStyle&XF_UNDERLINE),
			(m_nFontStyle&XF_STRIKE),
			GB2312_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH,
			_strFont);
	   ::SelectObject((HDC)m_pGraphics,(HFONT)m_pFont);
 
	SIZE size;    
	::GetTextExtentPoint32W((HDC)m_pGraphics,_strA,3,(LPSIZE)&size);
	//size.cy+=2;
	m_nTextHeight=size.cy+2;

#endif
   }
   return m_pFont;
}

void XGraphics::GetBitmapData(void*pBuf,int nLength,XU8 nBits)
{
#ifdef _XGDIDRAW
	Bitmap* pBmp=(Bitmap*)m_pBitmap;

	int w=pBmp->GetWidth();
	int h=pBmp->GetHeight();
	Rect rt(0,0,w,h);
	BitmapData data;
	XU32 nFmt=PixelFormat32bppARGB;
	if(nBits<=8)
		nFmt=PixelFormat8bppIndexed;
	else if(nBits<=16)
		nFmt=PixelFormat16bppARGB1555;
	else if(nBits<=24)
		nFmt=PixelFormat24bppRGB;
	pBmp->LockBits(&rt,0,nFmt,&data);
	memcpy(pBuf,data.Scan0,nLength);
	pBmp->UnlockBits(&data);
	//return XNULL;
#else
	BOOL bOK=::GetBitmapBits((HBITMAP)m_pBitmap,nLength,pBuf);
	/*BITMAPINFO info;
	SIZE sz;
	BOOL bOK=GetBitmapDimensionEx((HBITMAP)m_pBitmap,&sz);
	::GetDIBits((HDC)m_pGraphics,(HBITMAP)m_pBitmap,
				0,100,NULL,&info,
				DIB_RGB_COLORS);*/
#endif
}

XBOOL XGraphics::DrawDIB(XDib &dib, const XPOINT &pt, const XRECT &rect)
{
	if(dib.IsEmpty()||dib.GetBits()!=XDib::BITS32) return XFALSE;
	//int w=rect.right-rect.left;
	//int h=rect.bottom-rect.top;
	int w=dib.Width(),h=dib.Height();

#ifdef _XGDIDRAW
	int l=w<<2;
#ifdef _ARGB_PIXEL_
	//for Debug
	{	static char*pBuffer=new char[1024*1024*8];
		XU32*pDst=(XU32*)pBuffer;
		XU32*pSrt=(XU32*)dib.GetBuffer()+(rect.top*w)+rect.left;
		w=rect.right-rect.left;
		h=rect.bottom-rect.top;		
		int x,y;
		l=w<<2;
		for(y=0;y<h;y++)
		{
			for(x=0;x<w;x++)
			{
				((XU8*)&pDst[x])[0]=((XU8*)&pSrt[x])[2];
				((XU8*)&pDst[x])[1]=((XU8*)&pSrt[x])[1];
				((XU8*)&pDst[x])[2]=((XU8*)&pSrt[x])[0];
				((XU8*)&pDst[x])[3]=((XU8*)&pSrt[x])[3];
			}
			pDst+=w;
			pSrt+=dib.Width();
		}
		Bitmap bmp(w,h,
			   l,PixelFormat32bppARGB,
			   (BYTE*)pBuffer);//+rect.top*l+(rect.left<<2));
		Graphics*p=(Graphics*)m_pGraphics;
		p->DrawImage(&bmp,pt.x,pt.y,
						0,0,
						rect.right-rect.left,
						rect.bottom-rect.top,UnitPixel);
		return XTRUE;
	}	
#else
	Bitmap bmp(w,h,
			   l,PixelFormat32bppARGB,
			   dib.GetBuffer());//+rect.top*l+(rect.left<<2));
	Graphics*p=(Graphics*)m_pGraphics;
	p->DrawImage(&bmp,pt.x,pt.y,
						rect.left,rect.top,
						rect.right-rect.left,
						rect.bottom-rect.top,UnitPixel);
#endif
	//p->DrawImage(&bmp,pt.x,pt.y,);
#else
	HDC dc=(HDC)m_pGraphics;
	dib.Paint(dc,pt.x,pt.y,XRect(rect));
	/*::SetDIBitsToDevice(dc,                    // hDC
						pt.x,//rect.left,             // DestX
						pt.y,//rect.top,              // DestY
						//rect.Width(  ),
						//rect.Height(  ),
						//rect.left,

						-m_header.biHeight-rect.bottom,
						0,//rect.top,                          // nStartScan
						-m_header.biHeight,
						GetBuffer(),
						(LPBITMAPINFO)&m_header,     // lpBitsInfo
						DIB_RGB_COLORS);*/
#endif
	return XTRUE;
}

void XGraphics::GetBitmapMask(int sx, int sy, XDib &dib)
{
	if(dib.GetBits()!=8) return;
#ifdef _XGDIDRAW
	Bitmap* pBmp=(Bitmap*)m_pBitmap;

	int w=pBmp->GetWidth();
	int h=pBmp->GetHeight();
	Rect rt(0,0,w,h);
	BitmapData data;
	pBmp->LockBits(&rt,0,//PixelFormat8bppIndexed,&data);
		PixelFormat32bppARGB,&data);
	XU8*pSrt=(XU8*)data.Scan0;
	int wb=dib.Width(),ws=w<<2;
	XU8*pDst=(XU8*)dib.GetBuffer()+sy*wb+sx;	
	for(int y=0;y<h;y++)
	{
		for(int x=0;x<w;x++)
		{
			pDst[x]=pSrt[(x<<2)+1]>>2;
		}
		pSrt+=(ws);
		pDst+=wb;
	} 
	//memset(dib.GetBuffer(),0x80,wb*40);
	//memcpy(pBuf,data.Scan0,nLength);
	pBmp->UnlockBits(&data);
	//return XNULL;
#else
#endif
}

XU32 XGraphics::SetBold(XBOOL b) 
{
	XU32 n=m_nFontStyle;
	if(b)
	{
		if(!(m_nFontStyle&XF_BOLD))
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle|=(XF_BOLD);
	}
	else
	{
		if(m_nFontStyle&XF_BOLD)
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle&=~(XF_BOLD);
	}
	m_nFontStyle|=XF_UPDATE;
	return n;
}
XU32 XGraphics::SetItalic(XBOOL b) 
{
	XU32 n=m_nFontStyle;
	if(b) 
	{
		if(!(m_nFontStyle&XF_ITALIC))
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle|=(XF_ITALIC);
	}
	else 
	{
		if(m_nFontStyle&XF_ITALIC)
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle&=~(XF_ITALIC);
	}
	//m_nFontStyle|=XF_UPDATE;
	return n;
}
XU32 XGraphics::SetStrike(XBOOL b) 
{
	XU32 n=m_nFontStyle;
	if(b) 
	{
		if(!(m_nFontStyle&XF_STRIKE))
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle|=(XF_STRIKE);
	}
	else 
	{
		if(m_nFontStyle&XF_STRIKE)
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle&=~(XF_STRIKE);
	}
	//m_nFontStyle|=XF_UPDATE;
	return n;
}
XU32 XGraphics::SetUnderline(XBOOL b)
{
	XU32 n=m_nFontStyle;
	if(b) 
	{
		if(!(m_nFontStyle&XF_UNDERLINE))
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle|=(XF_UNDERLINE);
	}
	else 
	{
		if(m_nFontStyle&XF_UNDERLINE)
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle&=~(XF_UNDERLINE);
	}
	
	return n;

}
XU32 XGraphics::SetFontStyle(XU32 ns)
{
	//m_nFontStyle&=0x80000000;
	XU32 n=m_nFontStyle&0x7fffffff;
	if((n&0xff1f)!=(ns&0xff1f))
		m_nFontStyle|=XF_UPDATE;
	m_nFontStyle&=0x80000000;
	m_nFontStyle|=ns;
	return n;
}
XU32 XGraphics::SetFontSize(XU8 nSize)
{
	   XU32 n=m_nFontStyle;
	   XU32 ns=nSize<<8;
	   if((m_nFontStyle&0x0000ff00)!=ns)
	   {
		m_nFontStyle&=0xffff00ff;
		m_nFontStyle|=ns;
		m_nFontStyle|=XF_UPDATE;
	   }
	   return n;
}