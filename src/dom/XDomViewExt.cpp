// XDomViewExt.cpp: implementation of the XDomViewExt class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomViewExt.h"
#include "XFlashView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XDomViewExt::XDomViewExt(XGraphics*g,XClientApply*pa):XDomView(g,pa)
{
   m_bShowView=XFALSE;
   m_bBackList=XFALSE;
   m_pView=NULL;
}

XDomViewExt::~XDomViewExt()
{

}

void XDomViewExt::SetPercent(int nPer)
{
	//if(m_pView)
	//	m_pView->SetLoadPercent(nPer);
//#ifdef __ANDROID__
//	if( nPer == 0 )
//	{
//		//this->Invalidate();
//		//m_pDom->OnMove( 1, 1 );
//		//PostMsg(XWM_REPAINT,0,(XU32)this,XMS_THIS);
//		//LOGWHEREVAL( nPer );
//		int iSet=m_nReadID;
//		while(iSet!=m_nSetID)
//		{
//			LOGE("message %d, %d,%d", (int)m_messages[iSet], (int)m_messages[iSet+1],
//					(int)m_messages[iSet+2]);
//			iSet+=3;
//			if(iSet >= MAX_MESSAGES)
//			iSet-= MAX_MESSAGES;
//		}
//	}
//#endif
}

XBOOL XDomViewExt::Create(XRect &rect, XWindow *pParent, XPCTSTR strHtml, int len, XU8 nCharset)
{
	//if(!XDomView::Create(rect,pParent)) return XFALSE;
	XBOOL bOK=XWindow::Create(rect,XES_VISIABLE|XES_FOCUS|XES_POPUP,pParent,0);
	m_close.CreateFromResource("CLOSE",XTRUE);
	int nh=m_nTextHeight+3;
	m_rect=rect;
	m_view=rect;
	m_bInstance=XTRUE;
	m_view-=rect.TopLeft();
	
	//m_view.bottom-=nh;
	//m_status.Create(XRect(0,rect.Height()-nh,rect.right,rect.Height()),this);
#ifdef _SCROLLVIEW	   
	m_view.right-=XSSIZE;
	m_vBar.Create(XRect(m_view.right,0,m_rect.Width(),m_view.bottom),this);
	//int y=m_rect.Height()-XSTATUSH-XSHSIZE;
	m_hBar.Create(XRect(0,m_view.bottom-XSSIZE,m_view.right+1,m_view.bottom),this);
#endif
	//m_docSize=XSize(m_view.right,m_view.bottom+m_nTextHeight);
	//SetDocSize(m_view.right,m_view.bottom+m_nTextHeight);
	InitContext();

	if(strHtml)
	{
		m_pDom->Reset(nCharset,XCNT::CTEXT,XCNT::TVND_WAP_WML,&m_draw);
		if(len==0)
			len=XString8::SafeStrlen(strHtml);
		m_pDom->PhaseXML((XU8*)strHtml,len,false,true);
	}
	
#ifdef __APPLE__
	PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASH_OPENSUBVIEW, (XU32)this, XMS_TOPMOST);
#endif

	//m_view.left+=3;
	//m_view.right-=3;
	//m_view.bottom-=3;
	//m_view.top+=50;
	
	SetDocSize(m_view.Width(),m_view.Height()+m_nTextHeight);

	return bOK;
	
	//ShowWindow(XES_VISIABLE);
	//return XTRUE;
}

void XDomViewExt::OnClose()
{
	m_bInstance=XFALSE;
}

void XDomViewExt::OnMove(XRect &rect)
{
	XDomView::OnMove(rect);
	//m_view.left+=3;
	//m_view.right-=3;
	//m_view.bottom-=3;
	//m_view.top+=50;
}

XBOOL XDomViewExt::OnPaint(XGraphics &g)
{
	XBOOL bOK=XDomView::OnPaint(g);
	/*if(!m_close.IsEmpty())
	{
		int cx=m_rect.Width();
		int cy=m_rect.Height();
		int w=m_close.Width(),
			h=m_close.Height();
		g.SetClip(XRect(0,0,cx,cy));
		g.DrawImage(m_close,XPoint(cx-w-1,1));
	}*/
	return bOK;
}

void XDomViewExt::OnMouseMove(XPoint&pt,int iPos)
{
	XDomView::OnMouseMove(pt,iPos);
}
void XDomViewExt::OnMouseDown(XPoint&pt,int iPos)
{

	XDomView::OnMouseDown(pt,iPos);
}

void XDomViewExt::OnMouseUp(XPoint &pt,int iPos)
{
	//if(!m_close.IsEmpty())
	{
		int cx=m_rect.Width();
		//int cy=m_rect.Height();
		int w=m_close.Width(),
			h=m_close.Height();
		if(w<40) w=40;
		if(h<40) h=40;
		XRect rt(cx-w-10,-h-10,cx+4,0);
		if(rt.PtInRect(pt))
		{
			LOGWHERE();
			PostMsg(XWM_COMMAND,XCMD_CLOSE_VIEWEX,(XU32)this,XMS_PARENT);
#ifdef __ANDROID__
			PostMsg(ANDROID_FLASH_CLOSESUBVIEW, 0, (XU32)this, XMS_TOPMOST);
#endif
			return;
		}
	}
	XDomView::OnMouseUp(pt,0);
}
