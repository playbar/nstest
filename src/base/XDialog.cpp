// XDialog.cpp: implementation of the XDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XDialog.h"
#include "XResource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XDialog::XDialog()
{
   	m_pIcon=XNULL;
	m_nIndex=0;
	m_pHandle=XNULL;
	m_nCommand=0;
//	m_nCommand=0;
}

XDialog::~XDialog()
{
//	SetCapture(XNULL);
}

XBOOL XDialog::Create(const XRect &rect, XWindow *pWnd,XPCWSTR strTitle,XImage*pImage)
{
	if(pWnd==XNULL)
		pWnd=m_pTopmost;
	m_strTitle=strTitle;
	m_pIcon=pImage;
	m_nIndex=0;
//	m_nCommand=0;
	//SetCapture(this);
//	rect+=XSize(0,rect.Height());
//	SetCapture(this);
	return XWindow::Create(rect /*+ XSize(0,rect.Height())*/,XES_VISIABLE|XES_POPUP|XES_DIALOG,pWnd);
}

/*XU32 XDialog::DoModal()
{
   while(!IsLoopEnd())
   {
	   //Sleep(10);
	   //PeekMessage();
   }
   PostDestory();
   //SetCapture(XNULL);
   return m_nCommand;
}*/



XBOOL XDialog::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XWM_ENDDIALOG:
		 //m_nCommand=XIDQUIT;
		 EndDialog((XU32)XIDQUIT);
		 return XTRUE;
	case XWM_COMMAND:
		 EndDialog(pData1);
		 //m_nCommand=pData1;
		 return XTRUE;
	}
	return XWindow::Handle(nOpera,pData1,pData2);
}

XBOOL XDialog::OnPaint(XGraphics &g)
{
#ifdef __ANDROID__
#else
   XColor cc(GetSysColor(XCW_BUTTON));
   //cc.Alpha(0xc0);
   XRect r;
   XWindow::GetClientRect(r);
   DrawButton(g,r,cc,XBS_WFRAME);
   //g.DrawButton(r,cc,XBS_BFRAME);
   int ty=GetTitleHeight(m_pIcon,m_strTitle.GetLength()<=0?XNULL:(XPCWSTR)m_strTitle);
   if(ty>0)
   {
	   XRect rect(r.left+3,r.top+3,r.right-3,r.top+ty-1);
	   //cc.Bright(10);
	   //g.SetBackColor(cc);
	   //g.FillRectangle(rect);
	   XImage*pImage=GetIcon(XICON_BLUEBLOCK);
	   if(pImage==XNULL)
		g.DrawFrame(rect,cc,1);
	   else
	   {
		   int x,w=pImage->Width();
		   XRect ri(0,1,w,pImage->Height());
		   for(x=2;x<rect.right+2;x+=w)
		   {
			   XPoint dt(x,rect.top-1);
			   if(x+ri.right>rect.right+2)
				   ri.right=rect.right-x+2;
			   g.DrawImage(*pImage,dt,ri);
		   }
	   }
	   int bx=r.left+3;
	   if(m_pIcon!=XNULL)
	   {
		 int dd=(ty-4-m_pIcon->Height())>>1;
		 g.DrawImage(*m_pIcon,XPoint(rect.left,rect.top+dd+1));
		 bx+=m_pIcon->Width();
	   }
	   //XColor cm=GetSysColor(XCW_TEXT);
	   int dd=(ty-4-m_nTextHeight)>>1;
	   //cm.Mix(cc,0.3f);
	   //XColor cc1(cm);
	   //cc1.Mix(cc,0.8f);
	   //g.SetColor(cc1);
	   g.SetColor(0xff3463b9);
	   g.DrawString(m_strTitle,bx+3,rect.top+dd+3);
	   g.SetColor(0xffe5eff3);
	   //g.SetColor(cm);
	   g.DrawString(m_strTitle,bx+2,rect.top+dd+2);
   }
#endif
   return XTRUE;
}

void XDialog::GetClientRect(XRect &r)
{
	XWindow::GetClientRect(r);
	r.InflateRect(-4,-4);
	int cy=GetTitleHeight(m_pIcon,m_strTitle.GetLength()<=0?XNULL:(XPCWSTR)m_strTitle);	
	r.top+=cy;
}

int XDialog::GetTitleHeight(XImage *pImage, XPCWSTR strTitle)
{
   int ty=0;
   if(strTitle!=XNULL||(pImage!=XNULL&&pImage->Height()>0)) 
   {
		if(pImage!=XNULL) ty=pImage->Height()+1;
		if(strTitle!=XNULL) ty=XMAX(ty,m_nTextHeight);
		ty+=4;
   }
   return ty;
}


XBOOL XDialog::OnKeyDown(XKEYMSG&m)
{
	int c=m.nKeyCode;
	switch(c)
	{
	case XK_RIGHT:
	case XK_DOWN:
		 ChangeActive(XTRUE);
		 Invalidate();
		 return XTRUE;
	case XK_LEFT:
	case XK_UP:
		 ChangeActive(XFALSE);
		 Invalidate();
		 return XTRUE;
	case XK_RETURN:
		 //XWindow::OnKeyDown(c);
		 if(m_pActive!=XNULL)
			 m_pActive->KeyDown(m);
		 return XTRUE;
	case XK_CLEAR:
		 EndDialog(XWM_ENDDIALOG);
		 //m_nCommand=XWM_ENDDIALOG;
		 return XTRUE;
	}
	for(XU32 i=0;i<m_childs.GetSize();i++)
	{
		if(m_childs[i]->KeyDown(m)) return XTRUE;
	}
	return XTRUE;
	//return XWindow::OnKeyDown(c);
}

//DEL XBOOL XDialog::IsLoopEnd()
//DEL {
//DEL 	PeekMessage();
//DEL 	return m_nCommand;
//DEL }



void XDialog::EndDialog(XU32 nCmd)
{
   if(m_pHandle)
	   m_pHandle->PostMsg(XWM_ENDDIALOG,nCmd,m_nCommand,XMS_THIS);
   //m_nCommand=nCmd;
   PostDestory();

}

XBOOL XDialog::OnKeyUp(XKEYMSG& /*c*/)
{
	return XTRUE;
}

void XDialog::OnClick(XPoint&,int iPos)
{
   return;
}


//////////////////////////////////////////////////////////////////////
// XMessageBox Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef __ANDROID__
#else

XMessageBox::XMessageBox(XPCWSTR strTitle,
						 XPCWSTR strPrompt,
						 XWindow*pCallBack,
						 XU32 ID,
						 XU32 nStyle,						 
						 XU16 nSpaceHeight)
{
	if(m_pTopmost==XNULL) return;
	//m_pHandle=pCallBack;
	m_pImage=XNULL;
	XImage* p=GetIcon(XICON_MAIN);
	XRect rect;
	m_pTopmost->GetClientRect(rect);
	m_strPrompt=strPrompt;
	//XBitmap bmp(1,1);
	XGraphics g(1,1);//bmp);
	int bx=4;
	int nIcon=(XU8)(nStyle&0xff);
	m_nIndex=(XU8)((nStyle>>8)&0xff);
	int cy=m_nTextHeight;
	if(nIcon>0)
	{
	   m_pImage=GetIcon(nIcon);
	   if(m_pImage)
	   {
		bx+=m_pImage->Width();
		cy=XMAX(cy,(XINT)m_pImage->Height());
	   }
	}

	g.FormatString(m_strPrompt,m_strPrompt.GetLength(),
					bx,rect.Width()-bx-6,m_nFormats);
	cy=XMAX(cy,(XINT)(m_nFormats.GetSize()/3)*m_nTextHeight);
	cy+=XDialog::GetTitleHeight(p,strTitle);
	
	cy+=24+m_nTextHeight+nSpaceHeight;
	XRect rd(rect.left,rect.bottom-cy,rect.right,rect.bottom);
	/*if(rd.Width()>300)
	{
		int c=(rd.left+rd.right)>>1;
		rd.left=c-150;
		rd.right=c+150;
	}*/
	XDialog::Create(rd,m_pTopmost,strTitle,p);
	SetHandle(pCallBack,ID);
	CreateButtons(nStyle);
}

XMessageBox::~XMessageBox()
{

}

 
void XMessageBox::OnIdle()
{
	XWindow::OnIdle();
/*	XRect r;
	m_pParent->GetClientRect(r);
	int my=r.Height();
	if(m_rect.bottom<=my) return;
	int dy=XMIN(10,m_rect.bottom-my);
	r=m_rect;
	r-=XSize(0,dy);
	MoveWindow(r);
	//r.bottom+=4;
	//InvalidateRect(r);
	Invalidate();*/
	
}

XBOOL XMessageBox::OnPaint(XGraphics &g)
{
	XDialog::OnPaint(g);
	XRect r;
	GetClientRect(r);
	int bx=r.left,by=r.top+2;
	if(m_pImage!=XNULL)
			bx+=m_pImage->Width();
	if(m_nFormats.GetSize()>0)
	{
		if(m_nFormats.GetSize()<4)
			bx=(r.Width()-m_nFormats[2])>>1;
	}
    if(m_pImage!=XNULL)
	{
		m_pImage->SetActive(m_nIndex);
		g.DrawImage(*m_pImage,XPoint(bx-m_pImage->Width(),by));
	}
	g.SetColor(GetSysColor(XCW_TEXT));
	XPCWSTR str=m_strPrompt;
	for(XU32 i=0;i<m_nFormats.GetSize();i+=3)
	{
		if(m_nFormats[i]>=(DOMPOS)m_strPrompt.GetLength()) break;
	    g.DrawString(str+m_nFormats[i],bx,by,
			XMIN(m_nFormats[i+1],((DOMPOS)m_strPrompt.GetLength()-m_nFormats[i])));
		by+=m_nTextHeight;
	}
	return XTRUE;
}


#define XIDOK                1
#define XIDCANCEL            2
#define XIDABORT             3
#define XIDRETRY             4
#define XIDIGNORE            5
#define XIDYES               6
#define XIDNO                7
#define XIDQUIT				 -1

#define XMB_OK                       0x00000000
#define XMB_OKCANCEL                 0x00010000
#define XMB_ABORTRETRYIGNORE         0x00020000
#define XMB_YESNOCANCEL              0x00030000
#define XMB_YESNO                    0x00040000
#define XMB_RETRYCANCEL              0x00050000

static XPCWSTR _strButTitle[]=
{XResource::LoadWString(XIDS_EMPTY),
 XResource::LoadWString(XIDS_OK),
 XResource::LoadWString(XIDS_CANCEL),
 XResource::LoadWString(XIDS_ABORT),
 XResource::LoadWString(XIDS_RETRY),
 XResource::LoadWString(XIDS_IGNORE),
 XResource::LoadWString(XIDS_YES),
 XResource::LoadWString(XIDS_NO),
 XResource::LoadWString(XIDS_STOP),
 XResource::LoadWString(XIDS_START)};

void XMessageBox::CreateButtons(XU32 nStyle)
{
	XRect r;
	GetClientRect(r);
	
	int w=m_nTextHeight*3;
	int h=m_nTextHeight+5;
	XImage*pImage=GetIcon(XICON_BUTTON);
	if(pImage)
	{
		int dw=pImage->Width()>>1;
		int dh=pImage->Height()>>1;
		if(w<dw) w=dw;
		if(h<dh) h=dh;
	}
	int by=r.bottom-h-4;
	int mx=(r.left+r.right)>>1;
	XRect rl=XRect(r.left+2,by,r.left+w+2,by+h);
	XRect rm=XRect(mx-(w>>1),by,mx+(w>>1),by+h);
	XRect rr=XRect(r.right-8-w,by,r.right-2,by+h);
	switch(nStyle&0xffff0000)
	{
	case XMB_STARTSTOP:
		 m_buts[0].Create(rr,XIDSTART,_strButTitle[XIDSTART],XK_LEFTSOFT,this);
	case XMB_STOP:
		 m_buts[2].Create(rr,XIDSTOP,_strButTitle[XIDSTOP],XK_RIGHTSOFT,this);
		 break;
	case XMB_YESNO:
		 m_buts[0].Create(rl,XIDYES,_strButTitle[XIDYES],XK_LEFTSOFT,this);
		 m_buts[2].Create(rr,XIDNO,_strButTitle[XIDNO],XK_RIGHTSOFT,this);
		 break;
	case XMB_OKCANCEL:
		 m_buts[2].Create(rr,XIDCANCEL,_strButTitle[XIDCANCEL],XK_RIGHTSOFT,this);
	case XMB_OK:
		 m_buts[0].Create(rl,XIDOK,_strButTitle[XIDOK],XK_LEFTSOFT,this);
		 break;
	case XMB_ABORTRETRYIGNORE:
		 m_buts[0].Create(rl,XIDABORT,_strButTitle[XIDABORT],XK_LEFTSOFT,this);
		 m_buts[1].Create(rm,XIDRETRY,_strButTitle[XIDRETRY],XK_RETURN,this);
		 m_buts[2].Create(rr,XIDIGNORE,_strButTitle[XIDIGNORE],XK_RIGHTSOFT,this);
		 break;
	case XMB_YESNOCANCEL:
		 m_buts[0].Create(rl,XIDYES,_strButTitle[XIDYES],XK_LEFTSOFT,this);
		 m_buts[1].Create(rm,XIDNO,_strButTitle[XIDNO],XK_RETURN,this);
		 m_buts[2].Create(rr,XIDCANCEL,_strButTitle[XIDCANCEL],XK_RIGHTSOFT,this);
		 break;
	case XMB_RETRYCANCEL:
		 m_buts[0].Create(rl,XIDRETRY,_strButTitle[XIDRETRY],XK_LEFTSOFT,this);
		 m_buts[2].Create(rr,XIDCANCEL,_strButTitle[XIDCANCEL],XK_RIGHTSOFT,this);
		 break;
		 
	}
}

void XMessageBox::EndDialog(XU32 nCmd)
{
	XDialog::EndDialog(nCmd);
//	if(m_bDeleteSelf)
//	   delete this;
}

#endif


