// InfoWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "GGClient.h"
#include "InfoWnd.h"
#include "XSystem.h"

// CInfoWnd

IMPLEMENT_DYNAMIC(CInfoWnd, CButton)

CInfoWnd::CInfoWnd()
{
	m_nMemory=10;
	m_nMaxMemory=10;
	m_nNetRate=0;
	m_nMaxNetRate=0;
	m_nFrames=50;
	m_nMinFrames=80;
	m_nNetBytes=0;
	m_nLastNetBytes=0;
	m_nTime=0;
}

CInfoWnd::~CInfoWnd()
{
}


BEGIN_MESSAGE_MAP(CInfoWnd, CButton)
END_MESSAGE_MAP()


void CInfoWnd::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC*pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect;
	GetClientRect(rect);
	/*CBrush brush(0xffffff);
	pDC->FillRect(rect,&brush);*/
	int w=rect.Width(),h=rect.Height();
	XRect rd(0,0,w,h);
	XGraphics g;
	g.Create(rect.Width(),rect.Height());
	g.SetBackColor(0xff09262D);
	g.SetColor(0xff09262D);
	g.FillRectangle(rd);
	
	XPoint dst(4,2);
	//Draw Memory bar
	{
		g.DrawImage(m_frame,dst);
		int nNow=this->GetMemoryPecent(m_nMemory);
		int h=m_memory.Height();
		XRect  rdraw(0,0,nNow,h);
		XPoint pt(dst.x+3,dst.y+3);
		g.DrawImage(m_memory,pt,rdraw);
		int nMax=this->GetMemoryPecent(m_nMaxMemory);
		g.SetColor(0x9fFFEFCB);
		pt.x=dst.x+3+nMax;
		pt.y=dst.y+4+h;
		g.DrawLine(pt.x,pt.y,pt.x+4,pt.y+4,1);
		g.DrawLine(pt.x+4,pt.y+4,pt.x-4,pt.y+4,1);
		g.DrawLine(pt.x-4,pt.y+4,pt.x,pt.y,1);
		XString16 nStr;
		nStr.FromInt(m_nMemory);
		nStr+=XString16("M");
		XSize sz;
		g.MeasureString(nStr,sz);
		pt.x=rect.right-sz.cx-4;
		pt.y=dst.y;
		g.SetColor(0xfffffff0);
		g.DrawString(nStr,pt);
		dst.y+=h+12;
	}

	//Draw Frame bar
	{
		g.DrawImage(m_frame,dst);
		int nNow=this->GetFramePecent(m_nFrames);
		int h=m_frames.Height();
		XRect  rdraw(0,0,nNow,h);
		XPoint pt(dst.x+3,dst.y+3);
		g.DrawImage(m_frames,pt,rdraw);
		int nMax=this->GetFramePecent(m_nMinFrames);
		g.SetColor(0x9fFFEFCB);
		pt.x=dst.x+3+nMax;
		pt.y=dst.y+4+h;
		g.DrawLine(pt.x,pt.y,pt.x+4,pt.y+4,1);
		g.DrawLine(pt.x+4,pt.y+4,pt.x-4,pt.y+4,1);
		g.DrawLine(pt.x-4,pt.y+4,pt.x,pt.y,1);
		XString16 nStr;
		nStr.FromInt(m_nFrames);
		nStr+=XString16("F");
		XSize sz;
		g.MeasureString(nStr,sz);
		pt.x=rect.right-sz.cx-4;
		pt.y=dst.y;
		g.SetColor(0xfffffff0);
		g.DrawString(nStr,pt);
		dst.y+=h+12;
	}

	//Draw Net bar
	{
		g.DrawImage(m_frame,dst);
		int nNow=this->GetRatePecent(m_nNetRate);
		int h=m_netrate.Height();
		XRect  rdraw(0,0,nNow,h);
		XPoint pt(dst.x+3,dst.y+3);
		g.DrawImage(m_netrate,pt,rdraw);
		int nMax=this->GetRatePecent(m_nMaxNetRate);
		g.SetColor(0x9fFFEFCB);
		pt.x=dst.x+3+nMax;
		pt.y=dst.y+4+h;
		g.DrawLine(pt.x,pt.y,pt.x+4,pt.y+4,1);
		g.DrawLine(pt.x+4,pt.y+4,pt.x-4,pt.y+4,1);
		g.DrawLine(pt.x-4,pt.y+4,pt.x,pt.y,1);
		XString16 nStr;
		nStr.FromInt(m_nNetRate);
		nStr+=XString16("K");
		XSize sz;
		g.MeasureString(nStr,sz);
		pt.x=rect.right-sz.cx-4;
		pt.y=dst.y;
		g.SetColor(0xfffffff0);
		g.DrawString(nStr,pt);
		dst.y+=h+12;
	}

	XGraphics draw(*pDC);
	draw.Copy(g,XPoint(0,0),rd);
	
}

void CInfoWnd::UpdateInfo()
{
	XU32 nTime=XTime::GetTimeMSec();
	XU32 nDelta=nTime-m_nTime;
	m_nTime=nTime;
	int nMemory=XSystem::GetUsedMemory()/(1024*1024);
	SetMemory(nMemory);
	int nBytes=m_nNetBytes-m_nLastNetBytes;
	m_nLastNetBytes=m_nNetBytes;
	if(nDelta)
	{
		int nRate=nBytes/nDelta;
		SetRate(nRate);
	}
	if(this->GetSafeHwnd()!=NULL)
	Invalidate();
}

// CInfoWnd 消息处理程序


