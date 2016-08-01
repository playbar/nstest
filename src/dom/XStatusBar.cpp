// XStatusBar.cpp: implementation of the XStatusBar class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XStatusBar.h"
#include "XMainWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef __ANDROID__
#else

static XPCWSTR _strMenu[]=
{
	XResource::LoadWString(XIDS_MENU),
	XResource::LoadWString(XIDS_BACK),
	XResource::LoadWString(XIDS_STOP),
	XResource::LoadWString(XIDS_SELECT),
	XResource::LoadWString(XIDS_EDIT),
	XResource::LoadWString(XIDS_OVER),
};

XStatusBar::XStatusBar()
{
//   m_pIcon=XNULL;
   m_pAni=XNULL;
   m_nPecent=0;
   m_nActive=0;
   m_nMode=0;
   m_nTicks=0;
   m_pBlock=XNULL;
}

XStatusBar::~XStatusBar()
{

}

XBOOL XStatusBar::Create(XRect &rect, XWindow *pWnd)
{
//	m_pIcon=XMainWnd::GetIcon(XICON_HOME);
//_block.CreateFromResource("GREENBLOCK");
	m_pBlock=GetIcon(XICON_BLUEBLOCK);
	m_nMode=0;
	m_pAni=XMainWnd::GetIcon(XICON_ANI);
	m_nMenuBar=m_nTextHeight<<1;

#if defined(__TOUCHPAD__)||defined(__POINT__)
//#ifdef __POINT__
	AddItem(XCMD_DOM_QUIT,TICON_HIDE,XTS_NORMAL,XTP_LEFT);
//#endif
	AddItem(XCMD_DOM_STOP,TICON_CLOSE,XTS_DISABLE,XTP_RIGHT);
	AddItem(XCMD_DOM_HOME,TICON_HOME,XTS_DISABLE);
	AddItem(XCMD_DOM_BACK,TICON_BACK,XTS_DISABLE);
	AddItem(XCMD_VIEW_FORWARD,TICON_FORWARD,XTS_DISABLE);
	AddItem(XCMD_DOM_ADDBOOK,TICON_BOOKMARK,XTS_DISABLE);
	AddItem(XCMD_DOM_UPDATE,TICON_UPDATE,XTS_DISABLE);
	
	//Additem(XCMD_DOM_BOOKMARK,TICON_SCORE,XTS_SCORE)
#ifdef _WINEMU
	AddItem(XCMD_CLEAR_CATCH,TICON_ENUACC,XTS_NORMAL);
#endif
	XBOOL bOK=XToolBar::Create(pWnd,rect.bottom,rect.Width(),XFALSE,XICON_BLUEBLOCK);
	SetImage("ICONSSHOW","ICONSGRAY","ICONSACTIVE",XNULL);
	if(bOK)
		UpdateToolBar();
	return bOK;
#else
	return XWindow::Create(rect,XES_VISIABLE,pWnd);
#endif
}



XBOOL XStatusBar::OnPaint(XGraphics &g)
{
	XRect rect;
	GetClientRect(rect);
#if defined(__TOUCHPAD__)||defined(__POINT__)
	XToolBar::OnPaint(g);
	int bx=m_nItemWidth+2;
	XColor c=m_nColors[XCW_BUTTON];
	if(m_pAni!=XNULL&&m_nPecent)
	{
#else
	XColor c=m_nColors[XCW_BUTTON];
	XSIZE sz;
	//XU32 ss=g.GetFontStyle();//SetFontStyle(XF_STANDARD);
	g.MeasureString(_strMenu[0],sz);
	m_nMenuBar=sz.cx+6;
	
	
	rect.bottom++;
	int ddy=5;
	if(!m_pBlock||m_pBlock->IsEmpty())
		g.DrawButton(rect,c,XBS_BFRAME);
	else
	{
		int dw=m_pBlock->Width();
		XRect dr(0,0,dw,m_pBlock->Height());//m_block.Height());
		
		ddy=((m_pBlock->Height()-m_nTextHeight)>>1)+3;

		int x,by=(dr.bottom-rect.bottom)>>1;
		by = 0;
		for(x=0;x<rect.right;x+=dw)
		{
			XPoint pt(x,by);
			g.DrawImage(*m_pBlock,pt,dr);
		}
	}
	/*g.DrawButton(XRect(rect.left+m_nMenuBar,
					   rect.top+1,
					   rect.right-m_nMenuBar,
					   rect.bottom-1),c,0);*/
	int bx=(rect.right+rect.left-m_nMenuBar)>>1;

	//XColor cc(c);
	//cc.Bright(20);
	//g.SetColor(cc);
	//g.SetColor(0xff3463b9);
	//g.DrawString(str,3,by+1);
	g.SetColor(0xff86abe5);

	g.DrawLine(rect.left+m_nMenuBar,rect.top+2,rect.left+m_nMenuBar,rect.bottom-3);
	g.DrawLine(rect.right-m_nMenuBar,rect.top+2,rect.right-m_nMenuBar,rect.bottom-3);
	//cc.Bright(20);
	g.SetColor(0xff2756ad);
	g.DrawString(_strMenu[0],rect.left+4,rect.top+ddy-1);
	g.DrawString(_strMenu[m_nPecent>0?2:1],rect.right-m_nMenuBar+4,rect.top+ddy-1);

	if(m_nPecent<=0&&(m_nMode&0x7f)>0)
		g.DrawString(_strMenu[2+(m_nMode&0x7f)],bx,rect.top+ddy-1);
//	cc=c;
//	cc.Dark(20);
//	g.SetColor(cc);
	g.DrawLine(rect.left+m_nMenuBar-1,rect.top+2,rect.left+m_nMenuBar-1,rect.bottom-3);
	g.DrawLine(rect.right-m_nMenuBar-1,rect.top+2,rect.right-m_nMenuBar-1,rect.bottom-3);
	//if(!(m_nMode&XS_BACK))
	//	g.DrawString(_strMenu[1],rect.right-m_nMenuBar+3,rect.top+3);
//	cc.Dark(80);
//	g.SetColor(cc);
	//if(m_nMode&XS_BACK)
	//rect.top+=2;
	
	g.SetColor(0xffe5eff3);
	g.DrawString(_strMenu[0],rect.left+3,rect.top+ddy);
	if(m_nPecent<=0&&(m_nMode&0x7f)>0)
		g.DrawString(_strMenu[2+(m_nMode&0x7f)],bx-1,rect.top+ddy);
	if(m_nPecent<=0&&!(m_nMode&XS_BACK))
	{
		g.SetColor(0xffa5afb3);
		//cc=c;
		//cc.Dark(30);
		//g.SetColor(cc);
	}
	g.DrawString(_strMenu[m_nPecent>0?2:1],rect.right-m_nMenuBar+3,rect.top+ddy);
	
	bx=rect.left+m_nMenuBar+2;
	if(m_pAni!=XNULL)
	{

	
	   
#endif
	   
	   m_pAni->SetActive(m_nActive);
	   int by=((rect.Height()-m_pAni->Height()+1)>>1);
	   int ex=rect.right-bx-1;
	   g.DrawImage(*m_pAni,XPoint(bx,by));
	   //g.DrawFrame(XRect(bx,by,bx+m_pAni->Width(),by+m_pAni->Height()),c,1);
	   bx+=m_pAni->Width()+2;
	   if(m_nPecent>0)
	   {
		   by=3;
		   int mx=bx+(ex-bx)*m_nPecent/100;
		   int ey=rect.bottom-by;
		   XColor cc1(c),cc2(c);
		   cc1.Dark(60);
		   cc2.Dark(70);
		   g.DrawBar(XRect(mx,by,ex,ey),cc2,6);
		   g.DrawBar(XRect(bx,by,mx,ey),cc1,10);

		   //g.DrawFrame(XRect(bx,by,ex,ey),c,1);
	   }
	}
	return XTRUE;
//	g.SetFontStyle(ss);
}

void XStatusBar::OnTimer()
{
    if(m_nPecent<=0||m_pAni==XNULL) return;
	m_nTicks++;
	if(m_nTicks<m_pAni->GetFrameDelay(m_nActive)) return;
	m_nTicks=0;
	if(m_nActive+1<m_pAni->FrameCount()) 
		m_nActive++;
	else
		m_nActive=0;
	Invalidate();
}

void XStatusBar::OnClick(XPoint &pt,int iPos)
{
	if(!iPos) return;
    //if(m_pIcon==XNULL) return;
#if defined(__TOUCHPAD__)||defined(__POINT__)
#else
	XRect r(0,0,m_nMenuBar,m_rect.Height());
	if(r.PtInRect(pt))
		PostMsg(XWM_KEYDOWN,XK_LEFTSOFT,(XU32)this,XMS_NORMAL);
	r.right=m_rect.Width();
	r.left=m_rect.right-m_nMenuBar;
	if(r.PtInRect(pt))
		PostMsg(XWM_KEYDOWN,XK_RIGHTSOFT,(XU32)this,XMS_NORMAL);
	if(m_nPecent<=0)
	{
	    r.left=(m_rect.Width()-m_nMenuBar)>>1;
		r.right=r.left+m_nMenuBar;
		if(r.PtInRect(pt))
		{
			PostMsg(XWM_KEYDOWN,XK_RETURN,(XU32)this,XMS_NORMAL);
			PostMsg(XWM_KEYUP,XK_RETURN,(XU32)this,XMS_NORMAL);
		}
	}
#endif
}

void XStatusBar::SetPecent(XU8 nPecent)
{
//	if(nPecent)
//		int v=0;
	if(m_nPecent==nPecent) return;
	if(nPecent==0)
		m_nActive=0;
	m_nPecent=nPecent;
#if defined(__TOUCHPAD__)||defined(__POINT__)
	SetItemStatus(XCMD_DOM_STOP,m_nPecent?XTS_NORMAL:XTS_DISABLE);
	for(int i=0;i<m_nItemCount;i++)
	{
		if(m_items[i].iPos!=XTP_AUTO) continue;
		m_items[i].bHide=m_nPecent!=0;
	}
#endif
	Invalidate();
}

#endif


