// XImageThread.cpp: implementation of the XImageThread class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XImageThread.h"
#include "XDrawDib.h"
#include "Raster_gl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define _DIRECTDRAW_
//#define _LINEALPHA_
//#define _FASTBITMAP_
#define HALFOFF 32768

//#if (__CORE_VERSION__>=0x02079000)
#define MAXCOLORSTACK 16
//#else
//#define MAXCOLORSTACK 255
//#endif
#ifdef _DIRECTDRAW_
	#define _ROWADDR(R,X) ((U32 *)R->rowAddr + X);
#endif
//#define PIXELLEN 4
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
inline void OVERRGB8I(RGB8*DST,RGB8*SRC)
{
	switch(SRC->alpha)
	{
		case 0:break;
		case 0xff:*((XU32*)DST)=*((XU32*)SRC);break;
		default:int a1 = 256-SRC->alpha;
				DST->alpha=(SRC->alpha+(DST->alpha*a1>>8))&0xff;
				DST->red=((SRC->red*SRC->alpha>>8)+((DST->red)*a1>>8))&0xff;
				DST->green=((SRC->green*SRC->alpha>>8)+((DST->green)*a1>>8))&0xff;
				DST->blue=((SRC->blue*SRC->alpha>>8)+((DST->blue)*a1>>8))&0xff;
				break;
	}
}

inline void OVERRGB8(RGB8*DST,RGB8*SRC)
{
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


XImageThread::XImageThread(XU32 nID,XBOOL bAutoDelete)
//#ifdef _MultiThread_
//			:activeEdgeAlloc(sizeof(RActiveEdgeI),64),
//			runAlloc(sizeof(RRunI),128)
//#endif
{
	m_nThreadID=0;
	m_bAutoDelete=bAutoDelete;
	if(nID!=0)
	{
		m_session.Create();
		m_stop.Create();		
		m_thread.Create(this,nID);
	}
	m_pData=XNULL;
//#ifdef _MultiThread_
//	this->rowAddr=0;
//#endif
}

XImageThread::~XImageThread()
{
	m_stop.Release();
	m_session.Release();
	m_thread.Release(XFALSE);
}

XUINT XImageThread::CallBack(XU32 nID)
{
	 for(;;)
	 {
		 if(!m_session.IsEvent(XLONGLONGTIME))
		 {
			 continue;
		 }
		 if(m_stop.IsEvent(0)) break;
		 if(m_pData==XNULL)
			 continue;
			 //int v=0;
		 DoACT(m_pData);
		 if(m_stop.IsEvent(0)) break;
		 _ISDATA*pData=m_pData;
		 m_pData=XNULL;
		 m_session.ResetEvent();
		 pData->sEvent.SetEvent();
		 //m_pData->sEvent.SetEvent();
		 //m_pData=XNULL;
		 
	 }
	 m_stop.ResetEvent();
	 //m_thread.Stop(false);
	 if(m_bAutoDelete)
		delete this;
	 return nID;
}

void XImageThread::CopyS()
{
	 XU32 i,nL=m_pData->nLine*PIXELLEN;
	 XU8*pDst=m_pData->pDst;
	 XU8*pSrc=m_pData->pSrc;
	 for(i=0;i<m_pData->nLines;i++)
	 {
		 memcpy(pDst,pSrc,nL);
		
		 pSrc+=m_pData->nSrcPitch;
		 pDst+=m_pData->nDstPitch;
	 }
}

/*void XImageThread::Copy4()
{
	 XU32 i,nL=m_pData->nLine*PIXELLEN;
	 XU8*pDst=m_pData->pDst;
	 XU8*pSrc=m_pData->pSrc;
	 for(i=0;i<m_pData->nLines;i++)
	 {
		 memcpy(pDst,pSrc,nL);
		 if(i==m_pData->nMid)
			 m_pData->mEvent.SetEvent();
		 pSrc+=m_pData->nSrcPitch;
		 pDst+=m_pData->nDstPitch;
	 }
}*/


void XImageThread::Copy2()
{
	 XU32 i,nL=m_pData->nLine*PIXELLEN;
	 //XU8*pDst=m_pData->pDst;
	 //XU8*pSrc=m_pData->pSrc;
	 for(i=0;i<m_pData->nLines;i++)
	 {
		 memcpy(m_pData->pDst,m_pData->pSrc,nL);
		 m_pData->pSrc-=m_pData->nSrcPitch;
		 m_pData->pDst-=m_pData->nDstPitch;
	 }
}

void XImageThread::Copy3()
{
	 XU32 i,nL=m_pData->nLine*PIXELLEN;
	 XU8*pDst=m_pData->pDst;
	 XU8*pSrc=m_pData->pSrc;
	 XU8 buf[XLINELEN];
	 if(pDst<pSrc)
	 {
		 for(i=0;i<m_pData->nLines;i++)
		 {
			 //memcpy(pDst,pSrc,nL);
			 int cl=nL;
			 int tl=0;
			 while(cl)
			 {
				 int thisL=cl>XLINELEN?XLINELEN:cl;
				 memcpy(buf,pSrc+tl,thisL);
				 memcpy(pDst+tl,buf,thisL);
				 cl-=thisL;
				 tl+=thisL;
			 }
			 pSrc+=m_pData->nSrcPitch;
			 pDst+=m_pData->nDstPitch;
		 }
	 }
	 else
	 {
		 for(i=0;i<m_pData->nLines;i++)
		 {
			 //memcpy(pDst,pSrc,nL);
			 int cl=nL;
			 XU8*pDst1=pDst+nL;
			 XU8*pSrc1=pSrc+nL;
			 while(cl)
			 {
				 int thisL=cl>XLINELEN?XLINELEN:cl;
				 pDst1-=thisL;
				 pSrc1-=thisL;
				 memcpy(buf,pSrc1,thisL);
				 memcpy(pDst1,buf,thisL);
				 cl-=thisL;
			 }
			 pSrc+=m_pData->nSrcPitch;
			 pDst+=m_pData->nDstPitch;
		 }
	 }
}

void XImageThread::Clear()
{
	XU32 i,nL=m_pData->nLine*PIXELLEN;
	XU8*pSrc=m_pData->pSrc;
	for(i=0;i<m_pData->nLines;i++)
	{
		memset(pSrc,0,nL);
		pSrc+=m_pData->nSrcPitch;
	}
}

void XImageThread::Fill()
{
	XU32 i,nL=m_pData->nLine*PIXELLEN;
	XU8*pSrc=m_pData->pSrc;
	XU32 buf[XLINELEN];
	XU32 nColor=(XU32)m_pData->pData;
	for(i=0;i<XLINELEN;i++)
		buf[i]=nColor;
	XU32 nSL=XLINELEN*sizeof(XU32);
	for(i=0;i<m_pData->nLines;i++)
	{
		//memset(pSrc,0,nL);
		XU8*pDst=pSrc;
		int cl=nL;
		while(cl>0)
		{
			XU32 thisL=cl>nSL?nSL:cl;
			memcpy(pDst,buf,thisL);
			pDst+=thisL;
			cl-=thisL;
		}
		pSrc+=m_pData->nSrcPitch;
	}
}

void XImageThread::Fill2()
{
	XU32 i,x,nL=m_pData->nLine;
	XU32 nColor=(XU32)m_pData->pData;
	RGB8* pColor=(RGB8*)&nColor;
	XU8*pSrc=m_pData->pSrc;
	pColor->red=(pColor->red*pColor->alpha)>>8;
	pColor->blue=(pColor->blue*pColor->alpha)>>8;
	pColor->green=(pColor->green*pColor->alpha)>>8;
	for(i=0;i<m_pData->nLines;i++)
	{
		//memset(pSrc,0,nL);
		RGB8*pDst=(RGB8*)pSrc;
	    for(x=0;x<nL;x++)
		{
			OVERRGB8((pDst+x),pColor);
		}
		pSrc+=m_pData->nSrcPitch;
	}
}

void XImageThread::OverlayS()
{
	RGB8* pDst=(RGB8*)m_pData->pDst;
	RGB8* pSrc=(RGB8*)m_pData->pSrc;
	XU32 x,y;
	int nd=m_pData->nDstPitch>>2;
	int ns=m_pData->nSrcPitch>>2;
	for(y=0;y<m_pData->nLines;y++)
	{
		for(x=0;x<m_pData->nLine;x++)
		{
			OVERRGB8I((pDst+x),(pSrc+x));
		}
		pDst+=nd;
		pSrc+=ns;
	}
}

void XImageThread::OverlayS2()
{
	RGB8* pDst=(RGB8*)m_pData->pDst;
	RGB8* pSrc=(RGB8*)m_pData->pSrc;
	XU32 x,y;
	int nd=m_pData->nDstPitch>>2;
	int ns=m_pData->nSrcPitch>>2;
	for(y=0;y<m_pData->nLines;y++)
	{
		for(x=0;x<m_pData->nLine;x++)
		{
			OVERRGB8((pDst+x),(pSrc+x));
		}
		pDst+=nd;
		pSrc+=ns;
	}
}

void XImageThread::Trans()
{
	ILDATA ld;
	XU32 i;
	XDib*pDib=(XDib*)m_pData->pData;
	int w=pDib->m_nWidth;
	int h=pDib->m_nHeight;
	RGB8*pDst=(RGB8*)m_pData->pDst;
	int nd=m_pData->nDstPitch>>2,x;
	for(i=0;i<m_pData->nLines;i++)
	{
		InitILData(ld,m_pData->pts[0].pt,m_pData->pts[1].pt,m_pData->nLine);
		RGB8*pSrc=(RGB8*)m_pData->pSrc;
		//if(i>=363)
		//	int vv=0;
		x=0;
		int nw=m_pData->nLine;
		pSrc=GetBegin(ld,w,h,x,nw,pSrc);
		for(x;x<nw;x++)
		{
			OVERRGB8I((pDst+x),pSrc);
			pSrc=StepOne1(ld,w,pSrc);
		}
		m_pData->pSrc=(XU8*)StepOne(m_pData->pts[0],w,(RGB8*)m_pData->pSrc);
		StepOne2(m_pData->pts[1],w);
		pDst+=nd;
	}
	//Sleep(10000);
}

void XImageThread::Trans2()
{
	ILDATA ld;
	XU32 i;
	XDib*pDib=(XDib*)m_pData->pData;
	int w=pDib->m_nWidth;
	int h=pDib->m_nHeight;
	RGB8*pDst=(RGB8*)m_pData->pDst;
	int nd=m_pData->nDstPitch>>2,x;
	for(i=0;i<m_pData->nLines;i++)
	{
		InitILData(ld,m_pData->pts[0].pt,m_pData->pts[1].pt,m_pData->nLine);
		RGB8*pSrc=(RGB8*)m_pData->pSrc;
		//if(i>=486)
		//	int vv=0;
		x=0;
		int nw=m_pData->nLine;
		pSrc=GetBegin(ld,w,h,x,nw,pSrc);
		for(x;x<nw;x++)
		{
			OVERRGB8((pDst+x),pSrc);
			pSrc=StepOne1(ld,w,pSrc);
		}
		m_pData->pSrc=(XU8*)StepOne(m_pData->pts[0],w,(RGB8*)m_pData->pSrc);
		StepOne2(m_pData->pts[1],w);
		pDst+=nd;
	}
	//Sleep(10000);
}

void XImageThread::TransCopy()
{
	ILDATA ld;
	XU32 i;
	XDib*pDib=(XDib*)m_pData->pData;
	int w=pDib->m_nWidth;
	int h=pDib->m_nHeight;
	XU32*pDst=(XU32*)m_pData->pDst;
	int nd=m_pData->nDstPitch>>2,x;
	for(i=0;i<m_pData->nLines;i++)
	{
		InitILData(ld,m_pData->pts[0].pt,m_pData->pts[1].pt,m_pData->nLine);
		XU32*pSrc=(XU32*)m_pData->pSrc;
		//if(i>=486)
		//	int vv=0;
		x=0;
		int nw=m_pData->nLine;
		pSrc=(XU32*)GetBegin(ld,w,h,x,nw,(RGB8*)pSrc);
		for(x;x<nw;x++)
		{
			//OVERRGB8I((pDst+x),pSrc);
			pDst[x]=*pSrc;
			pSrc=(XU32*)StepOne1(ld,w,(RGB8*)pSrc);
		}
		m_pData->pSrc=(XU8*)StepOne(m_pData->pts[0],w,(RGB8*)m_pData->pSrc);
		StepOne2(m_pData->pts[1],w);
		pDst+=nd;
	}
	//Sleep(10000);
}


void XImageThread::Scale()
{
	ILDATA1 ld;
	XU32 i;
	XDib*pDib=(XDib*)m_pData->pData;
	int w=pDib->m_nWidth;
	int h=pDib->m_nHeight;
	RGB8*pDst=(RGB8*)m_pData->pDst;
	int nd=m_pData->nDstPitch>>2,x;
	for(i=0;i<m_pData->nLines;i++)
	{
		if(m_pData->pts1[0].pt.y>=h)
			break;
		InitILDataX(ld,m_pData->pts1[0].pt,m_pData->pts1[1].pt,m_pData->nLine);
		RGB8*pSrc=(RGB8*)m_pData->pSrc;
		//if(m_pData->pts1[0].pt.y>=451)
		//	int vv=0;
		x=0;
		int nw=m_pData->nLine;
		if(ld.pt.y>=0)
		{
			pSrc=GetBeginX(ld,w,x,nw,pSrc);
			for(x;x<nw;x++)
			{
				OVERRGB8I((pDst+x),pSrc);
				//if((XU8*)pSrc>=pDib->m_pDib+w*h*4)
				//	int vv=0;
				pSrc=StepOneX3(ld,w,pSrc);
			}
		}
		m_pData->pSrc=(XU8*)StepOneY(m_pData->pts1[0],w,(RGB8*)m_pData->pSrc);
		
		//StepOneY2(m_pData->pts[1],w);
		pDst+=nd;
	}
	//Sleep(10000);
}

void XImageThread::Scale2()
{
	ILDATA1 ld;
	XU32 i;
	XDib*pDib=(XDib*)m_pData->pData;
	int w=pDib->m_nWidth;
	int h=pDib->m_nHeight;
	RGB8*pDst=(RGB8*)m_pData->pDst;
	int nd=m_pData->nDstPitch>>2,x;
	for(i=0;i<m_pData->nLines;i++)
	{
		if(m_pData->pts1[0].pt.y>=h)
			break;
		InitILDataX(ld,m_pData->pts1[0].pt,m_pData->pts1[1].pt,m_pData->nLine);
		RGB8*pSrc=(RGB8*)m_pData->pSrc;
		//if(m_pData->pts1[0].pt.y>=451)
		//	int vv=0;
		x=0;
		int nw=m_pData->nLine;
		if(ld.pt.y>=0)
		{
			pSrc=GetBeginX(ld,w,x,nw,pSrc);
			for(x;x<nw;x++)
			{
				OVERRGB8((pDst+x),pSrc);
				//if((XU8*)pSrc>=pDib->m_pDib+w*h*4)
				//	int vv=0;
				pSrc=StepOneX3(ld,w,pSrc);
			}
		}
		m_pData->pSrc=(XU8*)StepOneY(m_pData->pts1[0],w,(RGB8*)m_pData->pSrc);
		
		//StepOneY2(m_pData->pts[1],w);
		pDst+=nd;
	}
	//Sleep(10000);
}

void XImageThread::ScaleCopy()
{
	ILDATA1 ld;
	XU32 i;
	XDib*pDib=(XDib*)m_pData->pData;
	int w=pDib->m_nWidth;
	int h=pDib->m_nHeight;
	RGB8*pDst=(RGB8*)m_pData->pDst;
	int nd=m_pData->nDstPitch>>2,x;
	for(i=0;i<m_pData->nLines;i++)
	{
		if(m_pData->pts1[0].pt.y>=h)
			break;
		InitILDataX(ld,m_pData->pts1[0].pt,m_pData->pts1[1].pt,m_pData->nLine);
		RGB8*pSrc=(RGB8*)m_pData->pSrc;
		//if(m_pData->pts1[0].pt.y>=451)
		//	int vv=0;
		x=0;
		int nw=m_pData->nLine;
		if(ld.pt.y>=0)
		{
			pSrc=GetBeginX(ld,w,x,nw,pSrc);
			for(x;x<nw;x++)
			{
				//OVERRGB8I((pDst+x),pSrc);
				*((XU32*)(pDst+x))=*((XU32*)pSrc);
				//if((XU8*)pSrc>=pDib->m_pDib+w*h*4)
				//	int vv=0;
				pSrc=StepOneX3(ld,w,pSrc);
			}
		}
		m_pData->pSrc=(XU8*)StepOneY(m_pData->pts1[0],w,(RGB8*)m_pData->pSrc);
		
		//StepOneY2(m_pData->pts[1],w);
		pDst+=nd;
	}
	//Sleep(10000);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
#ifdef _MultiThread_
/*
inline void DoEdgeEdgeRuleI(XImageThread* r, RActiveEdgeI* a)
{
	// Handle color1
	//if(!a->color1||!a->color2) return;
	_XRColor* c = a->color1;

	if( c->nFilterColor)
	{

	}else
	if ( c->visible ) {
		r->HideColor(c, a->x);
	 	c->visible = false;
	} else {
		r->ShowColor(c, a->x);
	 	c->visible = true;
	}

	// Handle color2
	c = a->color2;
	if( c->nFilterColor)
	{

	}else
	if ( c->visible ) {
		r->HideColor(c, a->x);
	 	c->visible = false;
	} else {
		r->ShowColor(c, a->x);
	 	c->visible = true;
	}
}

inline void DoEdgeEvenOddRuleI(XImageThread* r, RActiveEdgeI* a)
{
	// Handle color1
	_XRColor* c = a->color1;
	if( c->nFilterColor)
	{

	}else
	if ( c->visible ) {
		r->HideColor(c, a->x);
	 	c->visible = false;
	} else {
		r->ShowColor(c, a->x);
	 	c->visible = true;
	}
}

void DoEdgeWindingRuleI(XImageThread* r, RActiveEdgeI* a)
{
	// Handle color1
	_XRColor* c = a->color1;
	if( c->nFilterColor)
	{

	}else
	if ( c->visible == 0 ) {
		// The color is becoming active
		r->ShowColor(c, a->x);
		c->visible += a->dir;
	} else {
		int vis = c->visible + a->dir;
		if ( vis == 0 ) {
			// The color disappeared
			r->HideColor(c, a->x);
		}
		c->visible = vis;
	}
}

enum { RGBSlabChunkSize=1024 };
static const DoEdgeProcI edgeProcTable[] = {
		DoEdgeEdgeRuleI,
		DoEdgeEvenOddRuleI,
		DoEdgeWindingRuleI
	};

inline void StepLineI(RActiveEdgeI* edge, S32 yline)
{
 	// Step down a line
	edge->xl += edge->d.x;
	edge->x = (edge->xl + 0x8000L) >> 16;
}

inline void StepCurveI(RActiveEdgeI* edge, S32 yline)
{
	// Step down a curve
	S32 ymin = yline << 16;
	while ( edge->yl < ymin && edge->stepLimit >= 0 ) {
		// Step
		edge->yl += edge->d.y >> 8;
		edge->d.y += edge->d2.y;

		edge->xl += edge->d.x >> 8;
		edge->d.x += edge->d2.x;

		edge->stepLimit--;
	}

	edge->x = (edge->xl + 0x8000L) >> 16;
}

inline void StepCurveInterpolateI(RActiveEdgeI* edge, S32 yline)
{
	// Step down a curve
	S32 ymin = yline << 16;
	S32 prevX = edge->xl;
	S32 prevY = edge->yl;

	while ( edge->yl < ymin && edge->stepLimit >= 0 ) {
		// Step
		edge->yl += edge->d.y >> 8;
		edge->d.y += edge->d2.y;

		edge->xl += edge->d.x >> 8;
		edge->d.x += edge->d2.x;

		edge->stepLimit--;
	}

	// Calculate a precise x position
	SCOORD dy= edge->yl - prevY;
	if ( dy != 0 ) {
		// Calculate a position along the line
		SCOORD dx = edge->xl - prevX;
		SCOORD ky = ymin - edge->yl;
		edge->x = (edge->xl + FixedDiv(FixedMul(ky, dx), dy) + 0x8000L) >> 16;
	} else {
		edge->x = (edge->xl + 0x8000L) >> 16;
	}
}

void RActiveEdgeI::SetUp(REdge* e, S32 y, BOOL interpolate)
{
	// Set up the difference equation
	if ( e->isLine ) {
		// Do a straight line
		stepProc = StepLineI;

	 	SCOORD dx = (SCOORD)e->anchor2x - (SCOORD)e->anchor1x;
	 	SCOORD dy = (SCOORD)e->anchor2y - (SCOORD)e->anchor1y;

		// Set up the difference equation
		d.x = (dx << 16)/dy;
		xl = (SCOORD)e->anchor1x << 16;

		// Adjust for missed lines
		dy = y - (SCOORD)e->anchor1y;
		if ( dy != 0 )
			xl += d.x*dy;

		x = (xl+0x8000L)>>16;

	} else {
		SPOINT a, b;
		SCOORD dt, d2t;

		// Set up for a curve
		stepProc = interpolate ? StepCurveInterpolateI : StepCurveI;

		// compute quadratic coeffs w/ 16 bits of fraction
		a.x = (SCOORD)e->anchor1x - ((SCOORD)e->controlx<<1) + (SCOORD)e->anchor2x;
		b.x = ((SCOORD)e->controlx - (SCOORD)e->anchor1x)<<1;

		a.y = (SCOORD)e->anchor1y - ((SCOORD)e->controly<<1) + (SCOORD)e->anchor2y;
		b.y = ((SCOORD)e->controly - (SCOORD)e->anchor1y)<<1;

		stepLimit = ((SCOORD)e->anchor2y - (SCOORD)e->anchor1y)<<1;
		dt = (1L<<24)/stepLimit;	// Guess at two steps per scanline
		d2t = _FPMul(dt, dt, 24);

		// Calc step values
		d.x = b.x * dt;
		d2.x = (a.x<<1) * d2t;

		d.y = b.y * dt;
		d2.y = (a.y<<1) * d2t;

		FLASHASSERT(d.y > 0 || d2.y > 0);

		// Setup the location
		xl = (SCOORD)e->anchor1x << 16;
		yl = (SCOORD)e->anchor1y << 16;

		x = e->anchor1x;

		// Handle starting in the middle of a curve
		if ( y > e->anchor1y )
		 	stepProc(this, y);
	}

	ymax = e->anchor2y;

	color1 = e->color1;
	color2 = e->color2;
//	FLASHASSERT(e->fillRule!=fillWindingRule);
//	if(e->fillRule==0&&!color1||!color2)
//		int v=0;
	doEdgeProc = edgeProcTable[e->fillRule];
	dir = e->dir;
}

inline void XImageThread::AddActive()
{
	REdge *e, *lastE;
	RActiveEdgeI *a, *pos;
	REdge** index;
	S32 x;

	pos = firstActive;
	index = yindex + y-edgeClip.ymin;
	for ( e = *index; e; e = e->nextActive ) {
		a = CreateActiveEdge();
		if ( !a ) return;

		a->SetUp(e, y, !antialias_b);

		// Insert into the active edge list
		if ( pos ) {
			// Find the proper position
			x = a->x;
			if ( pos->x < x ) {
				while ( pos->x < x && pos->next )
					pos = pos->next;
			} else if ( pos->x > x ) {
				while ( pos->x > x && pos->prev )
					pos = pos->prev;
			}

			DListInsertAfter(pos, a, RActiveEdgeI);

		} else {
			// Insert into an empty list
			DListAdd(firstActive, a, RActiveEdgeI);
		}
		pos = a;

		lastE = e;	// remember the tail of the list
	}
}

inline void XImageThread::SortActive()
{
	RActiveEdgeI *a, *b, *stop;
	BOOL swapped;

	stop = 0;
	do {
		swapped = false;
		a = firstActive;

		while ( a != 0 && (b = a->next) != 0 ) {
			if ( a->x > b->x ) {
				// Swap
			 	DListSwap(firstActive, a, b, RActiveEdgeI);
			 	if ( !swapped && b->prev && b->prev->x > b->x )
			 		swapped = true;
			} else {
				// Just keep going
				a = b;
			}
		}
	} while ( swapped );
}

inline void XImageThread::PaintActive()
{
	S32 ynext = y+1;

	// Create a run object
    if ( antialias_b ) {
		if ( !firstRun ) {
		 	// Create an empty run
		 	firstRun = CreateRun();
		 	if ( !firstRun ) return;
		 	firstRun->Init(edgeClip.xmin, edgeClip.xmax);
		}
		curRun = firstRun;
    } else {
		SetYCoord(y);
	}

	{// This should never happen but we can minimize the effect
		//FLASHASSERT(!topColor);
		for ( _XRColor* c = topColor; c; c = c->nextActive )
		{
			c->visibleThread[m_nThreadID] = 0;
// #ifdef SPRITE_XTRA
// 			if (c == c->nextActive)
// 				break;
// #endif
		}

		topColor = 0;
	}

	register RActiveEdgeI* a = firstActive;
    while ( a ) {
		// Track the active colors
		(a->doEdgeProc)(this, a);

		// Remove out of date active edge entries
		// And calculate new x coordinates for the ones we keep
		if ( a->ymax > ynext ) {
			// Keep this edge
		 	a->stepProc(a, ynext);
		 	a = a->next;
		} else {
			// Remove this edge
    		RActiveEdgeI* next = a->next;
			DListRemove(firstActive, a, RActiveEdgeI);
			FreeActiveEdge(a);
			a = next;
		}
    }
}

inline void XImageThread::SetYCoord(S32 yCoord)
{
//	FLASHASSERT(y >= bitClip.ymin && y < bitClip.ymax);

	bitY = yCoord;

	if ( inverted ) {
		// The rows are inverted for a Windows DIB
		rowAddr = baseAddr + (bitHeight-(bitY+1))*rowBytes;
	} else {
		// We are using a standard row order
		rowAddr = baseAddr + bitY*rowBytes;
	}

	#ifdef ENABLE_MASK
	if ( maskBits ) {
		if ( maskBits->m_inverted ) {
			// The rows are inverted for a Windows DIB
			maskRowAddr = (char  *)maskBits->m_baseAddr + (bitHeight-(bitY+1))*maskBits->m_rowBytes;
		} else {
			// We are using a standard row order
			maskRowAddr = (char  *)maskBits->m_baseAddr + bitY*maskBits->m_rowBytes;
		}
	}
	#endif


}

inline void XImageThread::PaintAARuns()
// Paint the runs
{

	S32 xl, xr;
	int xlf, xrf;
	S32 yd;
	RPixelI pixel;
	RRunI *r, *next;

	yd = y>>antiShift;
	SetYCoord(yd);

	pixel.Init(this, yd,1<<antiShift);


	r = firstRun;
	firstRun = 0;

	while ( r ) {
 		if ( r->nColors ) 
		{
			#ifdef MERGE_RUNS
    		// Check to merge adjacent identical runs
    		if ( r->isPure && r->nColors == antiStep ) {
				nPureRuns++;
    			while ( (next = r->next) != 0 ) {
					if ( r->stacks[0].colors[0] != next->stacks[0].colors[0] ) goto MergeDone;	// this should cause failure most of the time
					if ( !next->isPure ) goto MergeDone;
					if ( next->nColors != antiStep ) goto MergeDone;
					int i = r->stackN[0];
					if ( i != next->stackN[0] ) goto MergeDone;
					_XRColor** ra = r->stacks[0].colors+1;
					_XRColor** rb = next->stacks[0].colors+1;
					i--;
					while ( i-- > 0) {
						if ( *ra++ != *rb++ )
							goto MergeDone;
					}
    				r->xmax = next->xmax;
    				r->next = next->next;
    				FreeRun(next);
					nPureMerged++;
    			}
			  MergeDone:
				;
    		}
    		{
    			while ( (next = r->next) && RunEqual(r, next) ) {
    				r->xmax = next->xmax;
    				r->next = next->next;
    				FreeRun(next);
					nMerged++;
    			}
    		}
			nRuns++;
			#endif

    		// Draw a run

			xl = r->xmin>>antiShift;	 		
	 		xr = r->xmax>>antiShift;
#ifdef		_FASTBITMAP_
			_XRColor*color=r->stacks[0].colors[0];
			if(color->colorType==colorBitmap&&
				color->transparent)
			{
	 				if ( xl < xr ) {

		 				// Handle the solid run
		 				//if ( r->isPure && r->nColors == antiStep ) 
						{
		 					// This is a fully covered run
							if ( r->transparent ) {
								//if(IsCopyLine(r->stacks[0].colors[0],xl,xr))
									//(r->stacks[0].colors[0]->drawSlabProc)(r->stacks[0].colors[0], xl, xr);
								//{
								//	(r->stacks[0].colors[0]->drawSlabProc)(r->stacks[0].colors[0], xl, xr);
								//}
								//else 
									CompositeSlab(xl, xr, r->stacks[0].colors, r->stackN[0]);
							} else {
								(r->stacks[0].colors[0]->drawSlabProc)(r->stacks[0].colors[0], xl, xr);
							}

		 				}
					}
			}
			else
#endif
			{

				xlf = (int)r->xmin & antiMask;
	 			xrf = (int)r->xmax & antiMask;

				RGBI runRGB;
				BOOL runRGBValid = false;

	 			if ( xl == xr ) {
	 				// The run is completely within the current pixel
					if ( !runRGBValid )	{
						r->CalcColor(xl, yd, &runRGB);
						runRGBValid = true;
					}
	 				pixel.Add(&runRGB, r->nColors, xl, xrf - xlf);

	 			} else {
	 				// The run covers more than a single pixel
		 			if ( xlf ) {
		 				// Handle the leading fraction
						if ( !runRGBValid )	{
							r->CalcColor(xl, yd, &runRGB);
							runRGBValid = true;
						}
		 				pixel.Add(&runRGB, r->nColors, xl, antiStep-xlf);
			 			xl++;	// round up
		 			}

		 			if ( xl < xr ) {
		 				// Handle the solid run
		 				if ( r->isPure && r->nColors == antiStep ) {
		 					// This is a fully covered run
							if ( r->transparent ) {
								//if(IsCopyLine(r->stacks[0].colors[0],xl,xr))
									//(r->stacks[0].colors[0]->drawSlabProc)(r->stacks[0].colors[0], xl, xr);
								//{
								//	(r->stacks[0].colors[0]->drawSlabProc)(r->stacks[0].colors[0], xl, xr);
								//}
								//else 
									CompositeSlab(xl, xr, r->stacks[0].colors, r->stackN[0]);
							} else {
								(r->stacks[0].colors[0]->drawSlabProc)(r->stacks[0].colors[0], xl, xr);
							}

		 				} else {
							// Handle a non-pure or partially covered run
							int x = xl;
							do {
								RGBI pixBuf[RGBSlabChunkSize];
	#ifdef _DIRECTDRAW_
								XU32*pixBuf8=(XU32*)_ROWADDR(this,x);
	#else
								XU32 pixBuf8[RGBSlabChunkSize];
	#endif
								int limit = Min(xr, x+RGBSlabChunkSize);

	#ifdef _USE64BITS
								if ( r->isComplex ) {
									RGBI* pix = pixBuf;
									for ( S32 j = x; j < limit; j++, pix++ ) {
										r->CalcColor(j, yd, pix);
										*((XU64*)pix)= ((*((XU64*)pix))>>antiShift)&_MASK64;
										pixBuf8[j-x]=PackPix32(*pix);
										//pixelBuf8
										//(*(XU32*)pixBuf8)=Pack
										//((RGBIL*)pix)->c0 = (((RGBIL*)pix)->c0 >> antiShift) & 0x00FF00FFL;
										//((RGBIL*)pix)->c1 = (((RGBIL*)pix)->c1 >> antiShift) & 0x00FF00FFL;
									}

								} else {
									if ( !runRGBValid )	{
										r->CalcColor(xl, yd, &runRGB);
										runRGBValid = true;
									}
									*((XU64*)pixBuf)=((*((XU64*)&runRGB))>>antiShift)&_MASK64;
									pixBuf8[0]=PackPix32(*pixBuf);
									//((RGBIL*)pixBuf)->c0 = (((RGBIL*)&runRGB)->c0 >> antiShift) & 0x00FF00FFL;
									//((RGBIL*)pixBuf)->c1 = (((RGBIL*)&runRGB)->c1 >> antiShift) & 0x00FF00FFL;
	#else

								if ( r->isComplex ) {
									RGBI* pix = pixBuf;
									for ( S32 j = x; j < limit; j++, pix++ ) {
										r->CalcColor(j, yd, pix);
	#ifdef __ANDROID__
										pix->blue = (pix->blue >>antiShift)& 0xFF;
										pix->green = (pix->green >>antiShift)& 0xFF;
										pix->red = (pix->red >>antiShift)& 0xFF;
										pix->alpha = (pix->alpha >>antiShift)& 0xFF;
	#else
										((RGBIL*)pix)->c0 = (((RGBIL*)pix)->c0 >> antiShift) & 0x00FF00FFL;
										((RGBIL*)pix)->c1 = (((RGBIL*)pix)->c1 >> antiShift) & 0x00FF00FFL;
	#endif
										pixBuf8[j-x]=PackPix32(pix);
									}

								} else {
									if ( !runRGBValid )	{
										r->CalcColor(xl, yd, &runRGB);
										runRGBValid = true;
									}
	#ifdef __ANDROID__
									pixBuf->blue = (runRGB.blue >> antiShift)& 0xFF;
									pixBuf->green = (runRGB.green >> antiShift)& 0xFF;
									pixBuf->red = (runRGB.red >> antiShift)& 0xFF;
									pixBuf->alpha = (runRGB.alpha >> antiShift)& 0xFF;
	#else
									((RGBIL*)pixBuf)->c0 = (((RGBIL*)&runRGB)->c0 >> antiShift) & 0x00FF00FFL;
									((RGBIL*)pixBuf)->c1 = (((RGBIL*)&runRGB)->c1 >> antiShift) & 0x00FF00FFL;
	#endif
									pixBuf8[0]=PackPix32(pixBuf);
	#endif
									//RGBI* pix = pixBuf+1;
									XU32*pix=pixBuf8+1;
									for ( S32 j = x+1; j < limit; j++, pix++ )
										*pix=pixBuf8[0];
										//*pix = pixBuf[0];
								}
	#ifndef _DIRECTDRAW_
								if ( r->nColors == antiStep && !r->transparent )
									drawRGBSlab(this, x, limit, (RGB8*)pixBuf8);// there is no transparency - fast case
								else
									DrawRGBASlab(x, limit, (RGB8*)pixBuf8);
	#endif

								x = limit;
							} while ( x < xr );
		 				}

						#ifdef ENABLE_MASK
						if ( maskBits )
							SetMask(xl, xr);
						#endif
		 			}
		 			if ( xrf ) {
		 				// Handle the trailing fraction
						if ( r->isComplex || !runRGBValid )
							r->CalcColor(xr, yd, &runRGB);
		 				pixel.Add(&runRGB, r->nColors, xr, xrf);
		 			}
		 		}
			}
		}

		// Advance
		next = r->next;
		FreeRun(r);
		r = next;
	}
	pixel.Paint();	// Flush the pixel...
}


void XImageThread::Raster()
{
	BeginRaster();
	int ymin=m_pData->nStart;
	int ymax=m_pData->nStart+m_pData->nLines;
	for ( y = ymin;y<ymax; y++ ) {
		AddActive();
		SortActive();
		PaintActive();
		if ( antialias_b && (y & antiMask) == antiMask )
		{
	    	PaintAARuns();
		}

		if ( (y & 0x1f) == 0 ) {
			Spin();
// 			#if defined(USE_MMX) || defined(USE_KATMAI)
// 			if ( gUseMMX || gUseKatmai ) {
// 				_asm EMMS
// 			}
// 			#endif
//			PollSound();
		}
	}

	{// Free any excess active edges
		for ( RActiveEdgeI* a = firstActive; a; ) {
			RActiveEdgeI* next = a->next;
			FreeActiveEdge(a);
			a = next;
		}
		firstActive = 0;
	}
	EndRaster();
}

inline void XImageThread::BeginRaster()
{
	//if(!rowAddr)
	{
		XRaster*raster=(XRaster*)m_pData->pData;
		rowAddr=raster->rowAddr;
		this->antialias_b=raster->antialias_b;
		this->antiMask=raster->antiMask;
		this->antiShift=raster->antiShift;
		this->antiStep=raster->antiStep;
		rowBytes=raster->rowBytes;
		firstActive = 0;
		topColor = 0;
		//doGetBackground=raster->doGetBackground;
	}
}

inline void XImageThread::EndRaster()
{
}*/

#endif