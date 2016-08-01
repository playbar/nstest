// XDialogClear.cpp: implementation of the XDialogClear class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDialogClear.h"
#include "XCatch.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef __ANDROID__
#else

static XPCWSTR _strTitles[]=
{	XResource::LoadWString(XIDS_CLEARCATCH),
	XResource::LoadWString(XIDS_BEGINCLEAR),
	XResource::LoadWString(XIDS_CLEARED),
	XResource::LoadWString(XIDS_CLEARCOUNT)};

XDialogClear::XDialogClear(XPCTSTR strPath,XU8 bAll):XMessageBox(_strTitles[0],_strTitles[1],XNULL,0,
																 (bAll?XMB_OKCANCEL:XMB_STOP)|XMB_ICONDELETE,m_nTextHeight)
{
	m_catch.Create(strPath);
	m_nCount=0;
	m_nProcCount=0;
	m_bAll=bAll;
}

XDialogClear::~XDialogClear()
{

}

/*XU32 XDialogClear::DoModal()
{
   while(m_nCommand==0)
   {
	   PeekMessage();
	   if(m_nCommand==XIDOK||!m_bAll)
	   {
		   if(m_bAll) DestoryChild(&m_buts[0]);
		   m_nCommand=0;
		   SetCount(0);
		   m_catch.Clear(this,m_bAll);
	   }
	   //else XThread::Sleep(10);
   }
   PostDestory();
   //SetCapture(XNULL);
   return m_nCommand;
} */

XUINT XDialogClear::CallBack(XU32 c)
{
	LOGWHERE();

	m_catch.Clear(this,m_bAll);
	XDialog::EndDialog(XIDCANCEL);
	m_thread.Stop(XFALSE);
	return c;
}

void XDialogClear::SetCount(XU32 nCount)
{
   m_nCount=nCount+1;
   if(m_nProcCount<m_nCount)
	   m_nProcCount=m_nCount;
   m_strPrompt=_strTitles[2];
   XString8 str;
   str.FromInt(nCount);
   m_strPrompt+=XString16(str);
   m_strPrompt+=_strTitles[3];

   //XGraphics g(1,1);
   //g.FormatString(m_strPrompt,m_strPrompt.GetLength(),
	//				bx,rect.Width()-bx-6,m_nFormats);

   Invalidate();
}

XBOOL XDialogClear::OnPaint(XGraphics &g)
{
	XMessageBox::OnPaint(g);
	if(m_nCount>0)
	{
		XRect rect;
		GetClientRect(rect);
		rect.InflateRect(-6,-6);
		rect.bottom-=m_nTextHeight+12;
		rect.top=rect.bottom-m_nTextHeight+6;
		XColor c(GetSysColor(XCW_BUTTON));
		int v=rect.right;
		rect.right=rect.left+(rect.Width()*m_nProcCount/(30+m_nProcCount));
		g.DrawBar(rect,GetSysColor(XCW_SELECTBACK),16);
		rect.right=v;
		g.DrawFrame(rect,c,1);
	}
	return XTRUE;
}

void XDialogClear::SetProcCount(XU32 nCount)
{
	m_nProcCount=nCount;
	Invalidate();
}

void XDialogClear::EndDialog(XU32 c)
{
	switch(c)
	{
	case XIDCANCEL:
		 if(m_thread.IsRunning())
			m_stop.SetEvent();
		 else
			 XDialog::EndDialog(c);
		 break;
	case XIDOK:
		 m_stop.Create();
		 m_thread.Create(this,0);
		 break;
	}
	//XDialog::EndDialog(c);
}

//DEL XBOOL XDialogClear::Create(const XRect &rect, XWindow *pWnd, XPCWSTR strTitle, XImage *pImage)
//DEL {
//DEL 	if(!XDialog::Create(rect,pWnd,strTitle,pImage)) return XFALSE;
//DEL 
//DEL 	//m_stop.Create();
//DEL 	m_thread.Create(this,0);
//DEL 
//DEL 	return XTRUE;
//DEL }

XBOOL XDialogClear::NeedStop()
{
	return m_stop.IsEvent(0);
}



//DEL XBOOL XDialogClear::OnCommand(XU32 nCommand, XWindow *pWnd)
//DEL {
//DEL 	return XDialog::OnCommand(nCommand,pWnd);
//DEL }
#endif

