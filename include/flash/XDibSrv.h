// XDibSrv.h: interface for the XDibSrv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDIBSRV_H__8F599C07_4CB2_4A11_B6A3_5B07637A2671__INCLUDED_)
#define AFX_XDIBSRV_H__8F599C07_4CB2_4A11_B6A3_5B07637A2671__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XImageThread.h"
#include "XDib.h"
#include "flashgeom.h"

#define MAXTHREADS 8
#define PIXELLEN   4

class XDibSrv  
{
private:
	static XDibSrv* instance;
public:
	static XDibSrv* GetInst()
	{
		if(instance==XNULL)
			instance=new XDibSrv();
		return instance;
	}
	static void ReleaseSrv()
	{
		if(instance)
		{
			delete instance;
			instance=XNULL;
		}
	}
public:
	//enum {COPY,OVERLAY,OVERLAY2};
	//XBOOL Overlay(XDib&dib,XDib&overlay,SPOINT drawPt,SPOINT area);
	void LimitLocal(XDib&dib,SRECT&area,SPOINT&pt);
	void LimitArea(XDib&dib,SRECT&area,SPOINT&pt);
	XBOOL Overlay(XDib&overlayTo,XDib&overlay,SPOINT& pt,SRECT&area)
	{
		return Copy(overlay,overlayTo,pt,area,SE_OVERLAY);
		//Copy(XDib&copy,XDib&copyTo,SPOINT copyPt,SRECT copyArea,int mode=SE_COPY);
	}
	XBOOL Overlay2(XDib&overlayTo,XDib&overlay,SPOINT& pt,SRECT&area)
	{
		return Copy(overlay,overlayTo,pt,area,SE_OVERLAY2);
		//Copy(XDib&copy,XDib&copyTo,SPOINT copyPt,SRECT copyArea,int mode=SE_COPY);
	}
	XBOOL Copy(XDib&copy,XDib&copyTo,SPOINT copyPt,SRECT copyArea,int mode=SE_COPY);
	XBOOL TransTo(XDib&dst,XDib&src,SRECT draw,MATRIX&mat,int mode=SE_TRANS);
	XBOOL TransTo2(XDib&dst,XDib&src,SRECT draw,MATRIX&mat)
	{
		return TransTo(dst,src,draw,mat,SE_TRANS2);
	}
	XBOOL TransCopyTo(XDib&dst,XDib&src,SRECT draw,MATRIX&mat)
	{
		return TransTo(dst,src,draw,mat,SE_TRANS_COPY);
	}
#ifdef _MultiThread_
	int Raster(XRaster*raster);
#endif
	XBOOL ScaleTo(XDib&dst,XDib&src,SRECT draw,MATRIX&mat,int mode=SE_SCALE);
	//XBOOL ScaleTo(XDib&dst,XDib&src,SRECT draw,MATRIX&mat,int mode
	XBOOL Scroll(XDib&dib,SRECT&area,int sx,int sy,bool clear=false);
	XBOOL Clear(XDib&dib,SRECT&area);
	XBOOL Create(XU8 nThreads);
	XBOOL FillRect(XDib&dib,SRECT&area,XU32 nColor);
	void Release();
	XDibSrv();
	virtual ~XDibSrv();
	XU8 GetThreads(){return m_nThreads;}
	void WaitEvents()
	{
		XU8 i;
		for(i=0;i<m_nThreads;i++)
		{
			m_datas[i].sEvent.IsEvent(XLONGLONGTIME);
			//{
			//	XThread::Sleep(20);
			//}
			m_datas[i].sEvent.ResetEvent();
		}
	}

protected:
	
	XU8			   m_nThreads;
	XImageThread*  m_threads[MAXTHREADS];
	XImageThread   m_doThread;
	_ISDATA		   m_datas[MAXTHREADS];
};

#endif // !defined(AFX_XDIBSRV_H__8F599C07_4CB2_4A11_B6A3_5B07637A2671__INCLUDED_)
