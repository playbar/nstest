// XDibSrv.cpp: implementation of the XDibSrv class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XDibSrv.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XDibSrv* XDibSrv::instance=XNULL;

XDibSrv::XDibSrv():m_doThread(0)
{
	//m_pThreads=XNULL;
	m_nThreads=0;
	if(instance==XNULL)
		instance=this;
}

XDibSrv::~XDibSrv()
{
	if(instance==this)
		instance=XNULL;
	Release();
}

void XDibSrv::Release()
{
	if(m_nThreads)
	{
		for(XU8 i=0;i<m_nThreads;i++)
		{
			m_threads[i]->Stop();
			delete m_threads[i];
			m_threads[i]=XNULL;
			m_datas[i].sEvent.Release();
			//m_datas[i].mEvent.Release();
		}
		m_nThreads=0;
	}
}

XBOOL XDibSrv::Create(XU8 nThreads)
{
	if(nThreads>MAXTHREADS) return XFALSE;
	if(nThreads==m_nThreads) return XTRUE;
	Release();
	if(nThreads<2) return XTRUE;
	XU8 i;
	for(i=0;i<nThreads;i++)
	{
		m_threads[i]=new XImageThread(200+i);
		m_threads[i]->m_nThreadID=i;
		m_datas[i].sEvent.Create();
		//m_datas[i].mEvent.Create();
	}
	m_nThreads=nThreads;
	return XTRUE;
}

inline void XDibSrv::LimitArea(XDib &dib, SRECT &area,SPOINT&pt)
{
	if(area.xmin<0)
	{
		pt.x-=area.xmin;
		area.xmin=0;
	}
	if(area.ymin<0)
	{
		pt.y-=area.ymin;
		area.ymin=0;
	}
	if(area.xmax>(SCOORD)dib.m_nWidth)
		area.xmax=dib.m_nWidth;
	if(area.ymax>(SCOORD)dib.m_nHeight)
		area.ymax=dib.m_nHeight;
}

inline void XDibSrv::LimitLocal(XDib &dib, SRECT &area, SPOINT &pt)
{
	 if(pt.x<0)
	 {
		 area.xmin-=pt.x;
		 pt.x=0;
	 }
	 if(pt.y<0)
	 {
		 area.ymin-=pt.y;
		 pt.y=0;
	 }
	 if(pt.x+area.xmax-area.xmin>(SCOORD)dib.m_nWidth)
	 {
		 area.xmax=dib.m_nWidth-pt.x+area.xmin;
	 }
	 if(pt.y+area.ymax-area.ymin>(SCOORD)dib.m_nHeight)
	 {
		 area.ymax=dib.m_nHeight-pt.y+area.ymin;
	 }
}

XBOOL XDibSrv::Scroll(XDib&dib,SRECT&area,int sx,int sy,bool clear)
{
	
	int nh=area.ymax-area.ymin;
	int nw=area.xmax-area.xmin;
	int nDelta;
	int dh=sy>0?sy:-sy;
	int dw=sx>0?sx:-sx;
	if(sy!=0||m_nThreads<2||nh-dh<m_nThreads*(dh+32)) nDelta=nh-dh;
	else								  nDelta=(nh-dh)/m_nThreads;
 	_ISDATA data;
	
	data.nSrcPitch=dib.m_nLinePitch;
	data.nDstPitch=dib.m_nLinePitch;
	data.nLines=nDelta;
	data.nLine=nw-dw;
	data.sID=SE_COPY;
	data.nMid=dh;

	//if(nDelta<=0||data.nLine<=0) return;
	SRECT fill1,fill2;
	if(clear)
	{
		::RectSetEmpty(&fill1);
		::RectSetEmpty(&fill2);
	}
	if(sy==0)
	{
		data.sID=SE_COPY3;
		if(sx>0)
		{
			data.pSrc=dib.m_pDib+area.ymin*dib.m_nLinePitch+area.xmin*PIXELLEN;
			data.pDst=data.pSrc+dw*PIXELLEN;
			if(clear)
			{
				fill1.xmin=area.xmin;
				fill1.ymin=area.ymin;
				fill1.xmax=fill1.xmin+dw;
				fill1.ymax=area.ymax;
			}
		}
		else
		{
			data.pDst=dib.m_pDib+area.ymin*dib.m_nLinePitch+area.xmin*PIXELLEN;
			data.pSrc=data.pDst+dw*PIXELLEN;
			if(clear)
			{
				fill1.xmin=area.xmax-dw;
				fill1.ymin=area.ymin;
				fill1.xmax=area.xmax;
				fill1.ymax=area.ymax;
			}
		}
	}
	else if(sy<0)
	{
		data.sID=SE_COPY;
		if(sx>0)
		{
			data.pDst=dib.m_pDib+area.ymin*dib.m_nLinePitch+(area.xmin+dw)*PIXELLEN;
			data.pSrc=dib.m_pDib+(area.ymin+dh)*dib.m_nLinePitch+area.xmin*PIXELLEN;
			if(clear)
			{
				fill1.xmin=area.xmin;
				fill1.ymin=area.ymin;
				fill1.xmax=fill1.xmin+dw;
				fill1.ymax=area.ymax;
				fill2.xmin=area.xmin+dw;
				fill2.xmax=area.xmax;
				fill2.ymin=area.ymax=dh;
				fill2.ymax=area.ymax;
			}
		}
		else
		{
			data.pDst=dib.m_pDib+area.ymin*dib.m_nLinePitch+area.xmin*PIXELLEN;
			data.pSrc=dib.m_pDib+(area.ymin+dh)*dib.m_nLinePitch+(area.xmin+dw)*PIXELLEN;
			if(clear)
			{
				fill1.xmin=area.xmax-dw;
				fill1.ymin=area.ymin;
				fill1.xmax=area.xmax;
				fill1.ymax=area.ymax;
				fill2.xmin=area.xmin;
				fill2.xmax=area.xmax-dw;
				fill2.ymin=area.ymax-dh;
				fill2.ymax=area.ymax;
			}
		}
	}
	else
	{
		data.sID=SE_COPY2;
		if(sx>0)
		{
			data.pDst=dib.m_pDib+(area.ymin+dh)*dib.m_nLinePitch+(area.xmin+dw)*PIXELLEN;
			data.pSrc=dib.m_pDib+(area.ymin)*dib.m_nLinePitch+(area.xmin)*PIXELLEN;	
			if(clear)
			{
			    fill1.xmin=area.xmin;
				fill1.ymin=area.ymin;
				fill1.xmax=fill1.xmin+dw;
				fill1.ymax=area.ymax;
			    fill2.xmin=area.xmin+dw;
				fill2.xmax=area.xmax;
				fill2.ymin=area.ymin;
				fill2.ymax=area.ymin+dh;
			}
		}
		else
		{
			data.pDst=dib.m_pDib+(area.ymin+dh)*dib.m_nLinePitch+(area.xmin)*PIXELLEN;
			data.pSrc=dib.m_pDib+(area.ymin)*dib.m_nLinePitch+(area.xmin+dw)*PIXELLEN;
			if(clear)
			{
				fill1.xmin=area.xmax-dw;
				fill1.ymin=area.ymin;
				fill1.xmax=area.xmax;
				fill1.ymax=area.ymax;
				fill2.xmin=area.xmin;
				fill2.xmax=area.xmax-dw;
				fill2.ymin=area.ymin;
				fill2.ymax=area.ymin+dh;
			}
		}
		int dd=(nDelta-1)*dib.m_nLinePitch;
		data.pDst+=dd;
		data.pSrc+=dd;
	
	}

	if(data.nLines<=0||data.nLine<=0)
	{
		if(clear)
			this->Clear(dib,area);
		return XTRUE;
	}

	if(nDelta+dh>=nh)
		m_doThread.DoACT(&data);
	else
	{
		if(data.sID==SE_COPY3)
		{
			int i,ns=area.ymin;
			int nsb=nDelta*data.nSrcPitch;
			for(i=0;i<m_nThreads;i++)
			{
				m_datas[i].nSrcPitch=data.nSrcPitch;
				m_datas[i].nDstPitch=data.nDstPitch;
				m_datas[i].nLines=data.nLines;
				m_datas[i].nLine=data.nLine;
				m_datas[i].pDst=data.pDst;
				m_datas[i].pSrc=data.pSrc;
				m_datas[i].nMid=data.nMid;
				//m_datas[i].sEvent.Create();
				m_datas[i].sID=data.sID;
				m_threads[i]->Start(&m_datas[i]);
				ns+=nDelta;
				data.pSrc+=nsb;
				data.pDst+=nsb;
			}
			if(ns<area.ymax-dh)
			{
				data.nLines=area.ymax-ns-dh;
				m_doThread.DoACT(&data);
			}
		}
	
		WaitEvents();
		
	}

	if(clear)
	{
		//::RectSetEmpty
		if(!RectIsEmpty(&fill1))
			Clear(dib,fill1);
		if(!RectIsEmpty(&fill2))
			Clear(dib,fill2);
	}

	return XTRUE;

	//return XTRUE;
} 

/*XBOOL XDibSrv::Scroll(XDib&dib,SRECT&area,int sx,int sy,bool clear)
{
	
	int nh=area.ymax-area.ymin;
	int nw=area.xmax-area.xmin;
	int nDelta;
	int dh=sy>0?sy:-sy;
	int dw=sx>0?sx:-sx;
	if(m_nThreads<2||nh-dh<m_nThreads*(dh+32)) nDelta=nh-dh;
	else								  nDelta=(nh-dh)/m_nThreads;
 	_ISDATA data;
	
	data.nSrcPitch=dib.m_nLinePitch;
	data.nDstPitch=dib.m_nLinePitch;
	data.nLines=nDelta;
	data.nLine=nw-dw;
	data.sID=SE_COPY;
	data.nMid=dh;

	//if(nDelta<=0||data.nLine<=0) return;
	SRECT fill1,fill2;
	if(clear)
	{
		::RectSetEmpty(&fill1);
		::RectSetEmpty(&fill2);
	}
	if(sy==0)
	{
		data.sID=SE_COPY3;
		if(sx>0)
		{
			data.pSrc=dib.m_pDib+area.ymin*dib.m_nLinePitch+area.xmin*PIXELLEN;
			data.pDst=data.pSrc+dw*PIXELLEN;
			if(clear)
			{
				fill1.xmin=area.xmin;
				fill1.ymin=area.ymin;
				fill1.xmax=fill1.xmin+dw;
				fill1.ymax=area.ymax;
			}
		}
		else
		{
			data.pDst=dib.m_pDib+area.ymin*dib.m_nLinePitch+area.xmin*PIXELLEN;
			data.pSrc=data.pDst+dw*PIXELLEN;
			if(clear)
			{
				fill1.xmin=area.xmax-dw;
				fill1.ymin=area.ymin;
				fill1.xmax=area.xmax;
				fill1.ymax=area.ymax;
			}
		}
	}
	else if(sy<0)
	{
		data.sID=SE_COPY4;
		if(sx>0)
		{
			data.pDst=dib.m_pDib+area.ymin*dib.m_nLinePitch+(area.xmin+dw)*PIXELLEN;
			data.pSrc=dib.m_pDib+(area.ymin+dh)*dib.m_nLinePitch+area.xmin*PIXELLEN;
			if(clear)
			{
				fill1.xmin=area.xmin;
				fill1.ymin=area.ymin;
				fill1.xmax=fill1.xmin+dw;
				fill1.ymax=area.ymax;
				fill2.xmin=area.xmin+dw;
				fill2.xmax=area.xmax;
				fill2.ymin=area.ymax=dh;
				fill2.ymax=area.ymax;
			}
		}
		else
		{
			data.pDst=dib.m_pDib+area.ymin*dib.m_nLinePitch+area.xmin*PIXELLEN;
			data.pSrc=dib.m_pDib+(area.ymin+dh)*dib.m_nLinePitch+(area.xmin+dw)*PIXELLEN;
			if(clear)
			{
				fill1.xmin=area.xmax-dw;
				fill1.ymin=area.ymin;
				fill1.xmax=area.xmax;
				fill1.ymax=area.ymax;
				fill2.xmin=area.xmin;
				fill2.xmax=area.xmax-dw;
				fill2.ymin=area.ymax-dh;
				fill2.ymax=area.ymax;
			}
		}
	}
	else
	{
		data.sID=SE_COPY2;
		if(sx>0)
		{
			data.pDst=dib.m_pDib+(area.ymin+dh)*dib.m_nLinePitch+(area.xmin+dw)*PIXELLEN;
			data.pSrc=dib.m_pDib+(area.ymin)*dib.m_nLinePitch+(area.xmin)*PIXELLEN;	
			if(clear)
			{
			    fill1.xmin=area.xmin;
				fill1.ymin=area.ymin;
				fill1.xmax=fill1.xmin+dw;
				fill1.ymax=area.ymax;
			    fill2.xmin=area.xmin+dw;
				fill2.xmax=area.xmax;
				fill2.ymin=area.ymin;
				fill2.ymax=area.ymin+dh;
			}
		}
		else
		{
			data.pDst=dib.m_pDib+(area.ymin+dh)*dib.m_nLinePitch+(area.xmin)*PIXELLEN;
			data.pSrc=dib.m_pDib+(area.ymin)*dib.m_nLinePitch+(area.xmin+dw)*PIXELLEN;
			if(clear)
			{
				fill1.xmin=area.xmax-dw;
				fill1.ymin=area.ymin;
				fill1.xmax=area.xmax;
				fill1.ymax=area.ymax;
				fill2.xmin=area.xmin;
				fill2.xmax=area.xmax-dw;
				fill2.ymin=area.ymin;
				fill2.ymax=area.ymin+dh;
			}
		}
		int dd=(nDelta-1)*dib.m_nLinePitch;
		data.pDst+=dd;
		data.pSrc+=dd;
		if(nDelta+dh<nh)
		{
			dd=(m_nThreads-1)*nDelta*dib.m_nLinePitch;
			data.pDst+=dd;
			data.pSrc+=dd;
		}
	}

	if(data.nLines<=0||data.nLine<=0)
	{
		if(clear)
			this->Clear(dib,area);
		return XTRUE;
	}

	if(nDelta+dh>=nh)
		m_doThread.DoACT(&data);
	else
	{
		if(data.sID==SE_COPY3)
		{
			int i,ns=area.ymin;
			int nsb=nDelta*data.nSrcPitch;
			for(i=0;i<m_nThreads;i++)
			{
				m_datas[i].nSrcPitch=data.nSrcPitch;
				m_datas[i].nDstPitch=data.nDstPitch;
				m_datas[i].nLines=data.nLines;
				m_datas[i].nLine=data.nLine;
				m_datas[i].pDst=data.pDst;
				m_datas[i].pSrc=data.pSrc;
				m_datas[i].nMid=data.nMid;
				//m_datas[i].sEvent.Create();
				m_datas[i].sID=data.sID;
				m_threads[i]->Start(&m_datas[i]);
				ns+=nDelta;
				data.pSrc+=nsb;
				data.pDst+=nsb;
			}
			if(ns<area.ymax-dh)
			{
				data.nLines=area.ymax-ns-dh;
				m_doThread.DoACT(&data);
			}
		}
		else if(data.sID==SE_COPY4)
		{
			int i,ns=area.ymin;
			int nsb=nDelta*data.nSrcPitch;
			
			for(i=0;i<m_nThreads;i++)
			{
				m_datas[i].nSrcPitch=data.nSrcPitch;
				m_datas[i].nDstPitch=data.nDstPitch;
				m_datas[i].nLines=data.nLines;
				m_datas[i].nLine=data.nLine;
				m_datas[i].pDst=data.pDst;
				m_datas[i].pSrc=data.pSrc;
				m_datas[i].nMid=data.nMid;
				//m_datas[i].sEvent.Create();
				m_datas[i].sID=data.sID;
				m_threads[i]->Start(&m_datas[i]);
				m_datas[i].mEvent.IsEvent(XLONGLONGTIME);
				m_datas[i].mEvent.ResetEvent();
				ns+=nDelta;
				data.pSrc+=nsb;
				data.pDst+=nsb;
			}
			if(ns<area.ymax-dh)
			{
				data.nLines=area.ymax-ns-dh;
				m_doThread.DoACT(&data);
			}
		}
		else
		{
			int i,ns=area.ymin;
			int nsb=nDelta*data.nSrcPitch;			
			for(i=0;i<1;i++)
			{
				m_datas[i].nSrcPitch=data.nSrcPitch;
				m_datas[i].nDstPitch=data.nDstPitch;
				m_datas[i].nLines=data.nLines;
				m_datas[i].nLine=data.nLine;
				m_datas[i].pDst=data.pDst;
				m_datas[i].pSrc=data.pSrc;
				m_datas[i].nMid=data.nMid;
				//m_datas[i].sEvent.Create();
				m_datas[i].sID=data.sID;
				m_threads[i]->Start(&m_datas[i]);
				m_datas[i].mEvent.IsEvent(XLONGLONGTIME);
				m_datas[i].mEvent.ResetEvent();
				ns+=nDelta;
				data.pSrc-=nsb;
				data.pDst-=nsb;
			}
			if(ns<area.ymax-dh)
			{
				data.nLines=area.ymax-ns-dh;
				m_doThread.DoACT(&data);
			}
		}
	
		WaitEvents();
		
	}

	if(clear)
	{
		//::RectSetEmpty
		if(!RectIsEmpty(&fill1))
			Clear(dib,fill1);
		if(!RectIsEmpty(&fill2))
			Clear(dib,fill2);
	}

	return XTRUE;

	//return XTRUE;
} */

XBOOL XDibSrv::Copy(XDib &copy, XDib &copyTo, SPOINT copyPt, SRECT copyArea,int nMode)
{
	LimitArea(copy,copyArea,copyPt);
	LimitLocal(copyTo,copyArea,copyPt);
	if(copyArea.ymax<=copyArea.ymin||
	   copyArea.xmax<=copyArea.xmin) return XFALSE;
	int nDelta;
	int nh=copyArea.ymax-copyArea.ymin;
	int nw=copyArea.xmax-copyArea.xmin;
	if(nw*nh<40000) nDelta=nh;
	else if(m_nThreads<2||nh<m_nThreads*4) nDelta=nh;
	else			 nDelta=nh/m_nThreads;
	_ISDATA data;
	data.sID=nMode;
	data.nSrcPitch=copy.m_nLinePitch;
	data.nDstPitch=copyTo.m_nLinePitch;
	data.nLine=nw;
	data.nLines=nDelta;
	data.pDst=copyTo.m_pDib+copyTo.m_nLinePitch*copyPt.y+copyPt.x*PIXELLEN;
	data.pSrc=copy.m_pDib+copyArea.ymin*copy.m_nLinePitch+copyArea.xmin*PIXELLEN;
	if(nDelta>=nh)
		m_doThread.DoACT(&data);
	else
	{
		int i,ns=copyArea.ymin;
		int nsb=nDelta*data.nSrcPitch;
		int ndb=nDelta*data.nDstPitch;
		for(i=0;i<m_nThreads;i++)
		{
			m_datas[i].nSrcPitch=data.nSrcPitch;
			m_datas[i].nDstPitch=data.nDstPitch;
			m_datas[i].nLines=nDelta;
			m_datas[i].nLine=data.nLine;
			m_datas[i].pDst=data.pDst;
			m_datas[i].pSrc=data.pSrc;
			//m_datas[i].sEvent.Create();
			m_datas[i].sID=data.sID;
			m_threads[i]->Start(&m_datas[i]);
			ns+=nDelta;
			data.pSrc+=nsb;
			data.pDst+=ndb;
		}
		if(ns<copyArea.ymax)
		{
			data.nLines=copyArea.ymax-ns;
			m_doThread.DoACT(&data);
		}
		WaitEvents();
	}
	return XTRUE;
}

XBOOL XDibSrv::Clear(XDib&dib,SRECT&area)
{
	int nDelta;
	int nh=area.ymax-area.ymin;
	int nw=area.xmax-area.xmin;
	if(nw*nh<40000) nDelta=nh;
	else if(m_nThreads<2||nh<m_nThreads*4) nDelta=nh;
	else								 nDelta=nh/m_nThreads;
	_ISDATA data;
	data.sID=SE_CLEAR;
	data.nSrcPitch=dib.m_nLinePitch;
	data.nLine=area.xmax-area.xmin;
	data.nLines=nDelta;
	data.pSrc=dib.m_pDib+area.ymin*dib.m_nLinePitch+area.xmin*PIXELLEN;
	if(nDelta>=nh)
		m_doThread.DoACT(&data);
	else
	{
		int i,ns=area.ymin;
		int nsb=nDelta*data.nSrcPitch;
		for(i=0;i<m_nThreads;i++)
		{
			m_datas[i].sID=data.sID;
			m_datas[i].nSrcPitch=data.nSrcPitch;
			m_datas[i].nLine=data.nLine;
			m_datas[i].pSrc=data.pSrc;	  
			m_datas[i].nLines=nDelta;
			m_threads[i]->Start(&m_datas[i]);
			ns+=nDelta;
			data.pSrc+=nsb;
		}
		if(ns<area.ymax)
		{
			data.nLines=area.ymax-ns;
			m_doThread.DoACT(&data);
		}	   
		WaitEvents();

	}
	return XTRUE;
}


XBOOL XDibSrv::FillRect(XDib&dib,SRECT&area,XU32 nColor)
{
	int nDelta;
	int nh=area.ymax-area.ymin;
	int nw=area.xmax-area.xmin;
	if(nw*nh<40000) nDelta=nh;
	else if(m_nThreads<2||nh<m_nThreads*4) nDelta=nh;
	else								 nDelta=nh/m_nThreads;
	_ISDATA data;
	data.sID=(nColor>>24)==0xff?SE_FILL:SE_FILL2;
	data.nSrcPitch=dib.m_nLinePitch;
	data.nLine=area.xmax-area.xmin;
	data.nLines=nDelta;
	data.pData=(void*)nColor;
	data.pSrc=dib.m_pDib+area.ymin*dib.m_nLinePitch+area.xmin*PIXELLEN;
	if(nDelta>=nh)
		m_doThread.DoACT(&data);
	else
	{
		int i,ns=area.ymin;
		int nsb=nDelta*data.nSrcPitch;
		for(i=0;i<m_nThreads;i++)
		{
			m_datas[i].sID=data.sID;
			m_datas[i].nSrcPitch=data.nSrcPitch;
			m_datas[i].nLine=data.nLine;
			m_datas[i].pSrc=data.pSrc;
			m_datas[i].pData=data.pData;
			m_datas[i].nLines=nDelta;
			m_threads[i]->Start(&m_datas[i]);
			ns+=nDelta;
			data.pSrc+=nsb;
		}
		if(ns<area.ymax)
		{
			data.nLines=area.ymax-ns;
			m_doThread.DoACT(&data);
		}	   
		WaitEvents();

	}
	return XTRUE;
}

XBOOL XDibSrv::ScaleTo(XDib&dst,XDib&src,SRECT draw,MATRIX&mat,int mode)
{
	switch(mode)
	{
	case SE_TRANS:mode=SE_SCALE;break;
	case SE_TRANS2:mode=SE_SCALE2;break;
	default:
	case SE_TRANS_COPY:mode=SE_SCALE_COPY;break;
	}
	
	int nh=draw.ymax-draw.ymin;
	int nDelta;
	//if(nh<m_nThreads*2||m_nThreads<2) nDelta=nh;
	if(true) nDelta=nh;
	else			    nDelta=nh/m_nThreads;

	_ISDATA data;
	data.nDstPitch=dst.m_nLinePitch;
	data.nLine=draw.xmax-draw.xmin;
	data.nLines=nDelta;
	data.nSrcPitch=src.m_nLinePitch;
	data.pDst=dst.m_pDib+draw.ymin*data.nDstPitch+draw.xmin*PIXELLEN;	
	data.sID=mode;
	data.pData=(void*)&src;

	int i;
	SPOINT pts[4]=
	{
		{draw.xmin,draw.ymin},
		{draw.xmin,draw.ymax},
		{draw.xmax,draw.ymin},
		{draw.xmax,draw.ymax}
	};
	for(i=0;i<4;i++)
		::MatrixTransformPoint(&mat,&pts[i],&pts[i]);
	//_ILDATA*pl=&data.begin;
	m_doThread.InitILDataY(data.pts1[0],pts[0],pts[1],nh);
	m_doThread.InitILDataY(data.pts1[1],pts[2],pts[3],nh);

	data.pSrc=src.m_pDib+data.pts1[0].pt.y*src.m_nLinePitch+data.pts1[0].pt.x*PIXELLEN;

	if(nDelta>=nh)
		m_doThread.DoACT(&data);
	else
	{
		int ns=draw.ymin;
		for(i=0;i<m_nThreads;i++)
		{
			m_datas[i].pts[0]=data.pts[0];
			m_datas[i].pts[1]=data.pts[1];
			m_datas[i].nDstPitch=data.nDstPitch;
			m_datas[i].nLine=data.nLine;
			m_datas[i].nLines=data.nLines;
			m_datas[i].nSrcPitch=data.nSrcPitch;
			m_datas[i].pData=data.pData;
			m_datas[i].pDst=data.pDst;
			m_datas[i].pSrc=data.pSrc;
			m_datas[i].sID=data.sID;
			m_threads[i]->Start(&m_datas[i]);
			data.pSrc=m_doThread.StepY(nDelta,data.pts1[0],src.m_nLinePitch,data.pSrc);
			data.pDst+=data.nDstPitch*nDelta;
			//m_doThread.Step(nDelta,data.pts[1],nh);
			//data.pSrc=src.m_pDib+data.begin.pt.y*src.m_nLinePitch+data.begin.pt.x*PIXELLEN;
			ns+=nDelta;
		}
		if(ns<draw.ymax)
		{
			data.nLines=draw.ymax-ns;
			m_doThread.DoACT(&data);
		}
		WaitEvents();
	}
	return XTRUE;
}


XBOOL XDibSrv::TransTo(XDib&dst,XDib&src,SRECT draw,MATRIX&mat,int mode)
{
	
	int nh=draw.ymax-draw.ymin;
	int nw=draw.xmax-draw.xmin;
	if(nh<=0||nw<=0) return XTRUE;
	int nDelta;
	if(nh<m_nThreads*2||m_nThreads<20) nDelta=nh;
	else			    nDelta=nh/m_nThreads;

	_ISDATA data;
	data.nDstPitch=dst.m_nLinePitch;
	data.nLine=nw;
	data.nLines=nDelta;
	data.nSrcPitch=src.m_nLinePitch;
	data.pDst=dst.m_pDib+draw.ymin*data.nDstPitch+draw.xmin*PIXELLEN;	
	data.sID=mode;
	data.pData=(void*)&src;

	int i;
	SPOINT pts[4]=
	{
		{draw.xmin,draw.ymin},
		{draw.xmin,draw.ymax},
		{draw.xmax,draw.ymin},
		{draw.xmax,draw.ymax}
	};
	for(i=0;i<4;i++)
		::MatrixTransformPoint(&mat,&pts[i],&pts[i]);
	//_ILDATA*pl=&data.begin;
	m_doThread.InitILData(data.pts[0],pts[0],pts[1],nh);
	m_doThread.InitILData(data.pts[1],pts[2],pts[3],nh);

	data.pSrc=src.m_pDib+data.pts[0].pt.y*src.m_nLinePitch+data.pts[0].pt.x*PIXELLEN;

	if(nDelta>=nh)
		m_doThread.DoACT(&data);
	else
	{
		int ns=draw.ymin;
		for(i=0;i<m_nThreads;i++)
		{

			//XEvent et=m_datas[i].sEvent;
			//m_datas[i]=data;
			//m_datas[i].sEvent=et;
			m_datas[i].pts[0]=data.pts[0];
			m_datas[i].pts[1]=data.pts[1];
			//m_datas[i].end=data.end;
			m_datas[i].nDstPitch=data.nDstPitch;
			m_datas[i].nLine=data.nLine;
			m_datas[i].nLines=data.nLines;
			m_datas[i].nSrcPitch=data.nSrcPitch;
			m_datas[i].pData=data.pData;
			m_datas[i].pDst=data.pDst;
			m_datas[i].pSrc=data.pSrc;
			m_datas[i].sID=data.sID;
			//m_datas[i].sEvent.Reset();//Create();
			m_threads[i]->Start(&m_datas[i]);

		//	_ILDATA*pl=&data.pts[0];
			data.pSrc=m_doThread.Step(nDelta,data.pts[0],src.m_nLinePitch,data.pSrc);
			data.pDst+=data.nDstPitch*nDelta;
			m_doThread.Step(nDelta,data.pts[1],nh);
			//data.pSrc=src.m_pDib+data.begin.pt.y*src.m_nLinePitch+data.begin.pt.x*PIXELLEN;
			ns+=nDelta;
		}
		if(ns<draw.ymax)
		{
			data.nLines=draw.ymax-ns;
			m_doThread.DoACT(&data);
		}
		WaitEvents();
	}

	return XTRUE;
}

#ifdef _MultiThread_
/*int XDibSrv::Raster(XRaster*raster)
{
	//return raster->edgeClip.ymin;
	int y=raster->edgeClip.ymin;
	if(m_nThreads<2) return y;
	int ymax=raster->edgeClip.ymax;
	int nh=ymax-y;
	if(nh<m_nThreads*8) return y;
	int nDelta=nh/m_nThreads;
	_ISDATA data;
	data.pData=(void*)raster;
	data.nStart=y;
	data.nLines=nDelta;
	data.sID=SE_RASTER;
	for(int i=0;i<m_nThreads;i++)
	{
		m_datas[i].sID=data.sID;
		m_datas[i].nLines=data.nLines;
		m_datas[i].nStart=data.nStart;
		m_threads[i]->Start(&m_datas[i]);
		y+=nDelta;
		m_datas[i].nStart=y;
		if(y<ymax)
		{
			data.nLines=ymax-y;
			m_doThread.DoACT(&data);
			y=ymax;
		}
	}
	return y;
}*/
#endif