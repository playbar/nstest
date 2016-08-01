// XImageThread.h: interface for the XImageThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XIMAGETHREAD_H__D7BABF68_40AC_4D47_B297_6E533D299216__INCLUDED_)
#define AFX_XIMAGETHREAD_H__D7BABF68_40AC_4D47_B297_6E533D299216__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMutex.h"
#include "flashgeom.h"
#include "XDib.h"
#include "XDrawDib.h"
#include "Raster_gl.h"

enum {//SE_ZOOMOUT=0,
	  //SE_ZOOMIN,
	  SE_OVERLAY=0,
	  SE_OVERLAY2,
	  SE_COPY,
	  SE_COPY2,
	  SE_COPY3,
	  //SE_COPY4,
	  SE_CLEAR,
	  SE_TRANS,
	  SE_TRANS2,
	  SE_TRANS_COPY,
	  SE_FILL,
	  SE_FILL2,
	  SE_SCALE,
	  SE_SCALE2,
	  SE_SCALE_COPY,
	  SE_RASTER
	};

typedef struct _ILDATA
{
	//BYTE*pAddr;
	SPOINT  pt;
	//int	 dx;
	//int  dy;
	int  dpx;
	int  dpy;
	int  ddx;
	int  ddy;
	int  mx;
	int  my;
	int  sx;
	int  sy;
	int  step;
}ILDATA;

typedef struct _ILDATA1
{
	SPOINT pt;
	int	   dp;
	int	   dd;
	int	   mm;
	//int	   s;
	int	   step;
}ILDATA1;


typedef struct _ISDATA
{
	XU8		sID;
	XEvent	sEvent;
	//XEvent	mEvent;
	XU8*	pSrc;
	XU8*	pDst;
	XU32    nSrcPitch;
	XU32	nDstPitch;
	XU32	nLine;
	XU32	nLines;
	XU32	nMid;
	XU32    nStart;
	void*	pData;
//	SPOINT  pts[4];
	union
	{
		_ILDATA		pts[2];
		_ILDATA1	pts1[2];
		XRaster*	raster;
	};
//	_ILDATA line;
}ISDATA;



#define PIXELLEN 4
#define XLONGLONGTIME 0x7fffffff
#define XLINELEN   512

class XImageThread;
/*struct _XRActiveEdgeI;
struct _XRColor;

typedef void (*DoEdgeProcI)(XImageThread*, _XRActiveEdgeI*);
typedef void (*StepProcI)(_XRActiveEdgeI*, S32 yline);

typedef struct _XRActiveEdgeI {
	_XRActiveEdgeI *next, *prev;
	S32 x;
	S32 ymax;

	SPOINT d, d2;		// difference stored as 10.22
	SCOORD xl, yl;		// location stored as 16.16
	S32 stepLimit;		// countdown to zero so we know when t == 1

	_XRColor *color1, *color2;
	int dir;

	DoEdgeProcI doEdgeProc;
	StepProcI stepProc;

	void SetUp(REdge* e, S32 y, BOOL interpolate);
}RActiveEdgeI;



#ifdef _MultiThread_
class XImageThread;
struct _XRRunI;
typedef struct _XRRunCoreI {// these fields are always copied on a Split()
	S32 xmin, xmax;
	_XRRunI* next;
	int nColors;
	U8 isComplex;	// there are non-solid colors in this run
	U8 isPure;		// all color stacks are the same
	U8 transparent;	// there is a transparent color in this run
	U8 hasCopyLayer;
	U8 stackN[4];
}RRunCoreI;

typedef struct _XRRunI : public _XRRunCoreI {
	RRunStack stacks[4];

	void Init(S32 l, S32 r);
	void AddColor(_XRColor*);
	void CalcColor(S32 x, S32 y, RGBI* rgb);
	_XRRun* Split(XImageThread*, S32 x);
}RRunI;

typedef struct _XRPixelI {
	XImageThread* raster;
	RGBI rgb;
	S32 x, y,nShift;

	void Init(XImageThread* r, S32 py,int shift);
	void Add(RGBI* rgb, int yw, S32 px, int xw);
	void Paint();
}RPixelI;

#endif*/

class XImageThread : public XBase
{
public:
	int m_nThreadID;
	void InitILDataY(ILDATA1&data,SPOINT&p1,SPOINT&p2,int step)
	{
		 data.step=step;
		 data.pt=p1;
		 int dd=(p2.y-p1.y);
		 data.dp=dd/step;
		 data.dd=dd%step;
		 //data.s=dd>=0?1:-1;
		 data.mm=step>>1;
		 /*if(data.dd<0)
		 {
			 //data.mm=-data.mm;
			 data.dd=-data.dd;
		 }*/
	}

	void InitILDataX(ILDATA1&data,SPOINT&p1,SPOINT&p2,int step)
	{
		 data.step=step;
		 data.pt=p1;
		 int dd=(p2.x-p1.x);
		 data.dp=dd/step;
		 data.dd=dd%step;
		 //data.s=dd>=0?1:-1;
		 data.mm=step>>1;
		 /*if(data.dd<0)
		 {
			 //data.mm=-data.mm;
			 data.dd=-data.dd;
		 }*/
	}

	XU8* StepY(int nDelta,ILDATA1&data,int iPitch=0,XU8*pSrc=XNULL)
	{
		int step=data.step;
		if(data.dp)
		{
			int dd=nDelta*data.dp;
			data.pt.y+=dd;
			if(iPitch)
				pSrc+=(dd)*iPitch;
		}
		if(data.dd)
		{
			data.mm+=data.dd*nDelta;
			if(data.mm>=step)
			{
				int dd=data.mm/step;
				data.pt.y+=dd;
				if(iPitch)
					pSrc+=(dd)*iPitch;
				data.mm=data.mm%step;
			}
		}
		return pSrc;
	}

	inline RGB8* StepOneY(ILDATA1&data,int iPitch,RGB8*pSrc)
	{
		int step=data.step;
		if(data.dp)
		{
			//int dd=nDelta*data.dp;
			data.pt.y+=data.dp;
			//if(iPitch)
			pSrc+=data.dp*iPitch;
		}
		if(data.dd)
		{
			data.mm+=data.dd;
			if(data.mm>=step)
			{
				//int dd=data.s>0?data.mm/step:-data.mm/step;
				data.pt.y++;
				pSrc+=iPitch;
				data.mm=data.mm%step;
			}
		}
		return pSrc;

	}



	

	inline RGB8* StepOneX(ILDATA1&data,int iPitch,RGB8*pSrc)
	{
		int step=data.step;
		if(data.dp)
		{
			//int dd=nDelta*data.dp;
			data.pt.x+=data.dp;
			//if(iPitch)
			pSrc++;
		}
		if(data.dd)
		{
			data.mm+=data.dd;
			if(data.mm>=step)
			{
				//int dd=data.s>0?data.mm/step:-data.mm/step;
				data.pt.x++;
				pSrc++;
				data.mm=data.mm%step;
			}
		}
		return pSrc;

	}

	inline void StepOneX2(ILDATA1&data)
	{
		int step=data.step;
		if(data.dp)
		{
			//int dd=nDelta*data.dp;
			data.pt.x+=data.dp;
			//if(iPitch)
			//pSrc++;
		}
		if(data.dd)
		{
			data.mm+=data.dd;
			if(data.mm>=step)
			{
				//int dd=data.s>0?data.mm/step:-data.mm/step;
				data.pt.x++;
				//pSrc++;
				data.mm=data.mm%step;
			}
		}
		//return pSrc;

	}

	inline RGB8* StepOneX3(ILDATA1&data,int iPitch,RGB8*pSrc)
	{
		int step=data.step;
		if(data.dp)
		{
			//int dd=nDelta*data.dp;
			//data.pt.x+=data.dp;
			//if(iPitch)
			pSrc+=data.dp;
		}
		if(data.dd)
		{
			data.mm+=data.dd;
			if(data.mm>=step)
			{
				//int dd=data.s>0?data.mm/step:-data.mm/step;
				//data.pt.x++;
				pSrc++;
				data.mm=data.mm%step;
			}
		}
		return pSrc;

	}


	inline RGB8* GetBeginX(ILDATA1&data,int w,int&x,int&nw,RGB8*pBuf)
	{
		//int step=data.step;
		//int w1=w<<4;
		//int h1=h<<4;
		while(x<nw)
		{
			if(data.pt.x>=0) break;
			pBuf=StepOneX(data,w,pBuf);
			x++;
		}
		int tx=x;
		int mm=data.mm;
		while(tx<nw)
		{
			if(data.pt.x>=w)
				break;

			StepOneX2(data);
			tx++;
			
		}
		data.mm=mm;
		nw=tx;
		return pBuf;
	}



	void	InitILData(ILDATA&data,SPOINT&p1,SPOINT&p2,int step)
	{
		data.step=step;
		data.pt.x=p1.x;
		data.pt.y=p1.y;
		//data.et.x=p2.x/16;
		//data.et.y=p2.y/16;
		int dx=(p2.x-data.pt.x);
		int dy=(p2.y-data.pt.y);
		//data.mx=0;//(p1.x%16)*step;
		//data.my=0;//(p1.y%16)*step;
		data.dpx=(dx)/step;
		data.dpy=(dy)/step;
		data.ddx=dx%step;
		data.ddy=dy%step;
		data.sx=dx>=0?1:-1;
		data.sy=dy>=0?1:-1;
		if(data.ddx<0)
		{
			//data.mx=-data.mx;
			data.ddx=-data.ddx;
		}
		if(data.ddy<0)
		{
			//data.my=-data.my;
			data.ddy=-data.ddy;
		}
		data.mx=step>>1;
		data.my=step>>1;
		/*if(data.mx>=step)
		{
			data.pt.x+=data.sx;
			data.mx-=step;
		}
		if(data.my>=step)
		{
			data.pt.y+=data.sy;
			data.my-=step;
		}*/
	}

	
	XU8* Step(int nDelta,ILDATA&data,int iPitch=0,XU8*pSrc=XNULL)
	{
		int step=data.step;
		if(data.dpx)
		{
			int dd=nDelta*data.dpx;
			data.pt.x+=dd;
			if(iPitch)
				pSrc+=(dd)*PIXELLEN;
		}
		if(data.dpy)
		{
			int dd=nDelta*data.dpy;
			data.pt.y+=dd;
			if(iPitch)
				pSrc+=(dd)*iPitch;
		}
		if(data.ddx)
		{
			data.mx+=data.ddx*nDelta;
			//int w=isd.pDraw->m_nWidth;
			//int h=isd.pDraw->m_nHeight;
			if(data.mx>=step)
			{
				int dd=data.sx>0?data.mx/step:-data.mx/step;
				data.pt.x+=dd;
				if(iPitch)
					pSrc+=(dd)*PIXELLEN;
				data.mx=data.mx%step;

			}
		}
		if(data.ddy)
		{
			data.my+=data.ddy*nDelta;
			if(data.my>=step)
			{
				int dd=data.sy>0?data.my/step:-data.my/step;
				data.pt.y+=dd;
				if(iPitch)
					pSrc+=(dd)*iPitch;
				data.my=data.my%step;
			}
		}
		return pSrc;
	}

	/*inline int GetEnd(ILDATA&data,int w,int h,int nw)
	{
		while(nw>0)
		{
			if(data.et.x>=0&&data.et.y>=0&&
			   
			nw--;
		}
		return nw;
	}*/
	inline RGB8* GetBegin(ILDATA&data,int w,int h,int&x,int&nw,RGB8*pBuf)
	{
		//int step=data.step;
		//int w1=w<<4;
		//int h1=h<<4;
		if(data.sx>0)
		{
			if(data.sy>0)
			{	//
				while(x<nw)
				{
					if(data.pt.x>=0&&data.pt.y>=0) break;
					pBuf=StepOne(data,w,pBuf);
					x++;
				}
				if(x>=nw) return pBuf;
				int tx=x;
				ILDATA back=data;
				while(tx<nw)
				{
					if(data.pt.x>=w||data.pt.y>=h)
						break;

					StepOne2(data,w);
					tx++;
					
				}
				nw=tx;
				data=back;
			}
			else
			{
				while(x<nw)
				{
					if(data.pt.x>=0&&data.pt.y<h) break;
					pBuf=StepOne(data,w,pBuf);
					x++;
				}
				//if(x>=nw) return pBuf;
				int tx=x;
				ILDATA back=data;
				while(tx<nw)
				{
					if(data.pt.x>=w||data.pt.y<0)
						break;
					StepOne2(data,w);
					tx++;
					

				}
				nw=tx;
				data=back;
			}
		}
		else
		{
			if(data.sy>0)
			{	//
				while(x<nw)
				{
					if(data.pt.x<w&&data.pt.y>=0) break;
					pBuf=StepOne(data,w,pBuf);
					x++;
				}
				if(x>=nw) return pBuf;
				int tx=x;
				ILDATA back=data;
				while(tx<nw)
				{
					if(data.pt.x<0||data.pt.y>=h)
						break;

					StepOne2(data,w);
					tx++;
					
				}
				nw=tx;
				data=back;
			}
			else
			{	//
				while(x<nw)
				{
					if(data.pt.x<w&&data.pt.y<h) break;
					pBuf=StepOne(data,w,pBuf);
					x++;
				}
				if(x>=nw) return pBuf;
				int tx=x;
				ILDATA back=data;
				while(tx<nw)
				{
					if(data.pt.x<0||data.pt.y<0)
						break;
					StepOne2(data,w);
					tx++;
				
				}
				nw=tx;
				data=back;
			}
		}
		return pBuf;
	}


	inline RGB8* StepOne(ILDATA&data,int w,RGB8*pBuf)
	{
		int step=data.step;
		if(data.dpx)
		{
			data.pt.x+=data.dpx;
			//if(pBuf)
			pBuf+=(data.dpx);
		}
		if(data.dpy)
		{
			data.pt.y+=data.dpy;
			//if(pBuf)
			pBuf+=(data.dpy)*w;
		}
		if(data.ddx)
		{
			data.mx+=data.ddx;
			if(data.mx>=step)
			{
				data.pt.x+=data.sx;
				//if(pBuf)
				pBuf+=(data.sx);
				data.mx-=step;
			}
		}
		if(data.ddy)
		{
			data.my+=data.ddy;
			if(data.my>=step)
			{
				data.pt.y+=data.sy;
				//if(pBuf)
				pBuf+=(data.sy)*w;
				data.my-=step;
			}
		}
		return pBuf;
	}

	inline RGB8* StepOne1(ILDATA&data,int w,RGB8*pBuf)
	{
		int step=data.step;
		if(data.dpx)
		{
			//data.pt.x+=data.dpx;
			//if(pBuf)
			pBuf+=(data.dpx);
		}
		if(data.dpy)
		{
			//data.pt.y+=data.dpy;
			//if(pBuf)
			pBuf+=(data.dpy)*w;
		}
		if(data.ddx)
		{
			data.mx+=data.ddx;
			if(data.mx>=step)
			{
				//data.pt.x+=data.sx;
				//if(pBuf)
				pBuf+=(data.sx);
				data.mx-=step;
			}
		}
		if(data.ddy)
		{
			data.my+=data.ddy;
			if(data.my>=step)
			{
				//data.pt.y+=data.sy;
				//if(pBuf)
				pBuf+=(data.sy)*w;
				data.my-=step;
			}
		}
		return pBuf;
	}


	inline void StepOne2(ILDATA&data,int w)
	{
		int step=data.step;
		if(data.dpx)
		{
			data.pt.x+=data.dpx;
			//if(pBuf)
			//pBuf+=(data.dpx);
		}
		if(data.dpy)
		{
			data.pt.y+=data.dpy;
			//if(pBuf)
			//pBuf+=(data.dpy)*w;
		}
		if(data.ddx)
		{
			data.mx+=data.ddx;
			if(data.mx>=step)
			{
				data.pt.x+=data.sx;
				//if(pBuf)
				//pBuf+=(data.sx);
				data.mx-=step;
			}
		}
		if(data.ddy)
		{
			data.my+=data.ddy;
			if(data.my>=step)
			{
				data.pt.y+=data.sy;
				//if(pBuf)
				//pBuf+=(data.sy)*w;
				data.my-=step;
			}
		}
		//return pBuf;
	}

	


	virtual XUINT CallBack(XU32 nID);
	XImageThread(XU32 nID=200,XBOOL bAutoDelete=XFALSE);
	virtual ~XImageThread();
	void Stop()
	{
		m_stop.SetEvent();
		m_session.SetEvent();
		m_thread.Stop(XTRUE);
	}
	virtual XBOOL NeedStop()
	{
		return m_stop.IsEvent(0);
	}
	XBOOL Start(ISDATA*pData)
	{
		if(m_session.IsEvent(0))
			return XFALSE;
		m_pData=pData;
		//if(m_pData==XNULL)
		//	int v=0;
		m_session.SetEvent();
		return XTRUE;
	}
	void DoACT(_ISDATA*pData)
	{
		m_pData=pData;
		switch(m_pData->sID)
		{
		case SE_SCALE:
			 Scale();
			 break;
	    case SE_SCALE2:
			 Scale2();
			 break;
		case SE_SCALE_COPY:
			 ScaleCopy();
			 break;
		case SE_COPY:
			 CopyS();
			 break;
		case SE_COPY2:
			 Copy2();
			 break;
		/*case SE_COPY4:
			 Copy4();
			 break;*/
		case SE_COPY3:
			 Copy3();
			 break;
		case SE_OVERLAY:
			 OverlayS();
			 break;
		case SE_OVERLAY2:
			 OverlayS2();
			 break;
		case SE_CLEAR:
			 Clear();
			 break;
		case SE_TRANS:
			 Trans();
			 break;
		case SE_TRANS2:
			 Trans2();
			 break;
		case SE_TRANS_COPY:
			 TransCopy();
			 break;
		case SE_FILL:
			 Fill();
			 break;
		case SE_FILL2:
			 Fill2();
			 break;
		case SE_RASTER:
			// Raster();
			 break;
		}
	}
protected:
	void Scale();
	void Scale2();
	void ScaleCopy();
	void Clear();
	void CopyS();
	void Copy2();
	void Copy3();
	//void Copy4();
	void OverlayS();
	void OverlayS2();
	void Trans();
	void Trans2();
	void TransCopy();
	void Fill();
	void Fill2();
	void Raster();
	_ISDATA* m_pData;
	XBOOL  m_bAutoDelete;
	XEvent m_session;
	XEvent m_stop;
	XThread m_thread;
#ifdef _MultiThread_
/*private:
	BOOL inverted;
	char  * baseAddr;
	S32 rowBytes;		// should always be a multiple of 4 bytes
	int pixelFormat;
	S32 bitHeight;
	S32 xorg;			// x pixel alignment
	
	// Current line context
	char  * rowAddr;
	S32 bitY;
	_XRColor *topColor;
	S32 topColorXleft;	// the left edge of the current slab being processed
	
	// Error diffusion context
	//int ditherSize;
	//U8* ditherData;
	//int ditherErrSize;
	//void* curDitherErr;
	//void* nextDitherErr;
	
	// Context for Paint()
	_XRActiveEdgeI* firstActive;
	_XREdge** yindex;
	S32 yindexSize;
	S32 ylines;
	S32 y;
	
    // Antialiasling info
    _XRRunI* firstRun;
    _XRRunI* curRun;
	SRECT edgeClip;

	XU8	 antiShift,antiMask,antiStep;
	BOOL antialias_b;
private:
	void BeginRaster();
	void EndRaster();
	void AddActive();
	void SortActive();
	void PaintActive();
	void SetYCoord(S32 y);
	void PaintAARuns();
	void DrawRGBAPixelI(S32 x, RGBI* rgb);
	void DrawRGBAPixel(S32 x, RGB8* rgb);
	void DrawRGBASlab(S32 xleft, S32 xright, RGB8*);
	void CompositeSlab(S32 xleft, S32 xright, _XRColor** stack, int n);
	void GetBackground(S32 xleft, S32 xright, RGBI* pix);
private:
	XFastAlloc activeEdgeAlloc;
	XFastAlloc runAlloc;
	inline _XRActiveEdgeI* CreateActiveEdge() { return (_XRActiveEdgeI*)(activeEdgeAlloc.Alloc()); }
	inline void FreeActiveEdge(_XRActiveEdgeI* a) { activeEdgeAlloc.Free(a); }
	
	inline _XRRunI* CreateRun() { return (_XRRunI*)(runAlloc.Alloc()); }
	inline void FreeRun(_XRRunI* c) { runAlloc.Free(c); }*/
#endif
};

#endif // !defined(AFX_XIMAGETHREAD_H__D7BABF68_40AC_4D47_B297_6E533D299216__INCLUDED_)
