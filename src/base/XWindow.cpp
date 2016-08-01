// XWindow.cpp: implementation of the XWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XWindow.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XWindow::m_nColors[XCW_BUTTON+1]=
{
	0xFFF0F0F0,// Background
	0xFF000000, // Text
	0xFF0000FF, // Link
#if defined (__ANDROID__)||(__APPLE__)
	0x00FFFFFF, // Select Background
	0xFFFF00FF, // Select Text
#else
	0xFF21686E, // Select Background
	0xFFFFFFAF, // Select Text
#endif
	0xFFFF00FF, // Active
	0xFFCDDBFD
};    // Button

XImage XWindow::m_icons[XMAX_ICONS];
XU8 XWindow::m_nTextHeight=16;
XU8 XWindow::m_nTextSize=1;
XWindow* XWindow::m_pCapture=XNULL;
XWindow* XWindow::m_pTopmost=XNULL;
XPCWSTR XWindow::m_strAppTitle=XNULL;
XWindow* XWindow::m_pMainWnd=XNULL;



XWindow::XWindow()
{
	m_pParent=XNULL;
	m_pActive=XNULL;
	m_rect=XRect(0,0,0,0);
	m_bFocus=XFALSE;
	m_nStyle=0;
	m_nReadID=m_nSetID=0;
}

XWindow::~XWindow()
{
	m_childs.RemoveAll();
	m_pParent=XNULL;
	m_pActive=XNULL;

}

XWindow* XWindow::GetPopupWindow()
{
	XU32 i,nc=m_childs.GetSize();
	XWindow*pWnd=XNULL;
	if(this->IsPopup()) pWnd=this;
	for(i=0;i<nc;i++)
	{
		XWindow*p=m_childs[i]->GetPopupWindow();
		if(p)
			pWnd=p;
	}
	return pWnd;
}

XBOOL XWindow::Paint(XGraphics &g)
{
	//return XTRUE;
	if(!IsVisiable())
		return XTRUE;
	int mx,my;
	XBOOL bOK=XTRUE;
	g.GetOrigin(mx,my);
	XU32 ss=g.SetFontStyle(XF_STANDARD);
	g.SetFontSize(XWindow::m_nTextSize);
	XRect rect=m_clip;

	g.SetOrigin(mx+m_rect.left,my+m_rect.top);
	XWindow*pPop=GetPopupWindow();

	if(!m_clip.IsRectEmpty())
	{
		//LOGWHERE();
		g.SetClip(m_clip);
		//if(pPop==NULL||pPop==this)
			OnPaint(g);
		#ifdef __APPLE__ //充值偶尔黑屏bug
		#else
		m_clip.SetRectEmpty();
		#endif
	}
	for(XU32 i=0;i<m_childs.GetSize();i++)
	{
		XRect r=rect;
		r&=m_childs[i]->m_rect;
		r-=m_childs[i]->m_rect.TopLeft();
		if(!r.IsRectEmpty())
			m_childs[i]->m_clip|=r;
		r=m_rect;
		r-=m_rect.TopLeft();
		r-=m_childs[i]->m_rect.TopLeft();
		m_childs[i]->m_clip&=r;

		if(!m_childs[i]->Paint(g))
			bOK=XFALSE;
	}
	g.SetOrigin(mx,my);
	g.SetFontStyle(ss);

	return bOK;
}

XBOOL XWindow::Create(const XRect &rect,XU8 nStyle,XWindow *pParent,XU32 nCmd)
{
	if(pParent==XNULL||
		nStyle&XES_POPUP) m_bFocus=XTRUE;
	m_nStyle=nStyle;
	if(nStyle&XES_TOPMOST)
		m_pTopmost=this;
	m_nCmd=nCmd;
    m_rect=rect;
	m_clip=XRect(0,0,rect.Width(),rect.Height());
	XPoint pt(0,0);
	FormatClip(m_clip,pt);
	m_pParent=pParent;
	OnCreate();
	if(pParent!=XNULL)
		pParent->AddChild(this);
	Invalidate();

	return XTRUE;
}

void XWindow::Close()
{
	OnClose();
	for(XU32 i=0;i<m_childs.GetSize();i++)
	{
		m_childs[i]->Close();
		/*if(m_childs[i]->m_nStyle&XES_DIALOG)
		{
		if(m_pActive==m_childs[i])
		m_pActive=XNULL;
		//SetActive(XNULL);
		delete m_childs[i];
		}*/
	}

	m_pActive=XNULL;
	m_childs.RemoveAll();
	//m_childs.RemoveAll();
	//if(m_nStyle&XES_DIALOG)
	//   delete this;
}

void XWindow::Invalidate()
{
	if(m_rect.IsRectEmpty()) return;
	XRect rect;
	GetClientRect(rect);
	m_clip=rect;
	XPoint pt(0,0);
	FormatClip(m_clip,pt);
	XWindow*p=GetParent();
	rect+=m_rect.TopLeft();
	if(p!=XNULL)
		p->InvalidateRect(rect);
	else
		PostMsg(XWM_REPAINT,0,(XU32)this,XMS_THIS);
	//Handle(XWM_REPAINT,0,0);
}

void XWindow::OnClick(XPoint&pt,int iPos)
{
	if(iPos==0&&m_nCmd>0) PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
}

void XWindow::InvalidateRect(const XRect &rt)
{
#if defined (__APPLE__ ) || defined ( __ANDROID__ ) 
	this->Invalidate();
#else
	if(m_rect.IsRectEmpty())
        return;
	XRect rect(rt);
	m_clip|=rect;
	XRect r;
	GetClientRect(r);
	m_clip&=r;
	XPoint pt(0,0);
	FormatClip(m_clip,pt);
	rect+=m_rect.TopLeft();
	XWindow*p=GetParent();
	if(p!=XNULL)
        p->InvalidateRect(rect);
	else
        PostMsg(XWM_REPAINT,0,(XU32)this,XMS_THIS);
#endif
    return;
}

XBOOL XWindow::PostMsg(XU32 nOpera, XU32 pData1, XU32 pData2,XU8 es)
{
	if(pData2==0)
		pData2=(XU32)this;
	XWindow*p=GetParent();

	if(es==XMS_THIS||p==XNULL||((m_nStyle&XES_DIALOG)&&nOpera==XWM_COMMAND))
	{
		XBOOL bModify=XFALSE;
#ifdef _NOCOMPOSERPAINT_
		if(nOpera==XWM_REPAINT)
		{
			int iSet=m_nReadID;
			int iCount=0;
			while(iSet!=m_nSetID)
			{
				if(m_messages[iSet]==nOpera)
				{
					iCount++;
					if(iCount >= 3 )
					{
						m_messages[iSet+1]=pData1;
						m_messages[iSet+2]=pData2;
						bModify=XTRUE;
						break;
					}
				}
				iSet+=3;
				if(iSet>=MAX_MESSAGES)
				iSet-=MAX_MESSAGES;
			}
		}
#else
		if(nOpera==XWM_REPAINT)
		{
			int iSet=m_nReadID;
			while(iSet!=m_nSetID)
			{
				if(m_messages[iSet]==nOpera)
				{
					m_messages[iSet+1]=pData1;
					m_messages[iSet+2]=pData2;
					bModify=XTRUE;
					break;
				}
				iSet+=3;
				if(iSet>=MAX_MESSAGES)
				iSet-=MAX_MESSAGES;
			}
		}
#endif
		if(!bModify)
		{
			int iSet=m_nSetID+3;
			if(iSet>=MAX_MESSAGES)
				iSet-=MAX_MESSAGES;
			if(iSet==m_nReadID)
			{
				return XFALSE;
			}

			m_messages[m_nSetID]=(nOpera);
			m_messages[m_nSetID+1]=(pData1);
			m_messages[m_nSetID+2]=(pData2);
			m_nSetID=iSet;
		}

	}
	else if(es==XMS_TOPMOST)
	{
		if(m_pTopmost==XNULL) return XFALSE;
		m_pTopmost->PostMsg(nOpera,pData1,pData2,XMS_THIS);
	}
	else if(es==XMS_PARENT)
	{
		if(p==XNULL) return XNULL;
		p->PostMsg(nOpera,pData1,pData2,XMS_THIS);
	}
	else
	{
		p->PostMsg(nOpera,pData1,pData2,XMS_NORMAL);
	}
	return XTRUE;
}

void XWindow::OnIdle()
{
	if(m_rect.IsRectEmpty())
		return;
	if(m_nSetID==m_nReadID)
	{
		for(XU32 i=0;i<m_childs.GetSize();i++)
		{
			if(m_childs[i])
				m_childs[i]->OnIdle();
		}
		return;
	}
#ifdef _NOCOMPOSERPAINT_
	int nID=m_nReadID;
	int nRepaintCount=0;
	while(nID!=m_nSetID)
	{
		if(m_messages[nID]==XWM_REPAINT)
		{
			nRepaintCount++;
			if( nRepaintCount >= 3 )
				break;
		}
		nID+=3;
		if(nID>=MAX_MESSAGES)
		{
			nID-=MAX_MESSAGES;
		}
	}
#endif
	while(m_nReadID!=m_nSetID)
	{
		//LOGE("%s, %d------> %d, %d", __FUNCTION__, __LINE__, m_nReadID, m_nSetID );
#ifdef _NOCOMPOSERPAINT_
		if(m_messages[m_nReadID]==XWM_REPAINT)
		{
			if(nRepaintCount)
				nRepaintCount--;
			if(!nRepaintCount)
				Handle(m_messages[m_nReadID],m_messages[m_nReadID+1],m_messages[m_nReadID+2]);
		}
		else
#endif
			Handle(m_messages[m_nReadID],m_messages[m_nReadID+1],m_messages[m_nReadID+2]);
		m_nReadID+=3;
		if(m_nReadID>=MAX_MESSAGES)
		{
			m_nReadID-=MAX_MESSAGES;
		}
	}
	return;
}

XBOOL XWindow::Handle(XU32 nOpera,XU32 pData1,XU32 pData2)
{
	switch(nOpera)
	{
	case XWM_ENDDIALOG:
		return XTRUE;
	case XWM_COMMAND:
		if(!IsVisiable())
			return XFALSE;
		if(OnCommand(pData1,(XWindow*)pData2))
			return XTRUE;
		break;

	case XWM_KEYUP:
		{
			XKEYMSG msg={pData1,pData1,0,false,false,false};
			KeyUp(msg);
		}
		return XTRUE;
	case XWM_KEYDOWN:
		{
			XKEYMSG msg={pData1,pData1,0,false,false,false};
			KeyDown(msg);
		}
		return XTRUE;
		//	  case XWM_SETACTIVE:
		//		   ChangeActive();
		return XTRUE;
	case XWM_DESTORY:
		return DestoryChild((XWindow*)pData2);
	}

	for(XU32 i=0;i<m_childs.GetSize();i++)
	{
		if(!m_childs[i])
			break;
		if(m_childs[i]->Handle(nOpera,pData1,pData2))
			return XTRUE;
	}

	return XFALSE;
}

void XWindow::AddChild(XWindow *pWnd)
{
	m_childs.Add(pWnd);
	if((m_pActive==XNULL||!m_pActive->IsPopup())||pWnd->IsPopup())
	{
		if(pWnd->IsFocus()||pWnd->IsPopup())
		{
			SetActive(pWnd);
			//m_pActive=pWnd;
			//m_pActive->OnFocus();
		}
	}
}

void XWindow::SetActive(XWindow *pActive)
{
	if(m_pActive==pActive)
        return;
	if(pActive!=XNULL&&!pActive->IsVisiable())
        return;

	if(m_pActive&&m_pActive->m_nStyle&XES_DIALOG)
	{
		if(!pActive||!(pActive->m_nStyle&XES_DIALOG))
            return;
	}

	if(m_pActive!=XNULL)
	{
		if(m_pActive->IsPopup()&&
			(pActive==XNULL||!pActive->IsPopup())) return;
		m_pActive->SetFocus(XFALSE);
	}
	m_pActive=pActive;
	if(pActive!=XNULL)
		pActive->SetFocus(XTRUE);
}

XBOOL XWindow::DestoryChild(XWindow *pWnd)
{
	for(XU32 i=0;i<m_childs.GetSize();i++)
	{
		if(m_childs[i]==pWnd)
		{
			XRect rect=m_childs[i]->m_rect;
			m_childs[i]->Close();
			m_childs.RemoveAt(i);
			if(m_pActive==pWnd) m_pActive=XNULL;
			for(i=m_childs.GetSize();i>0;i--)
			{
				if(!m_childs[i-1]->IsVisiable()&&
					!m_childs[i-1]->IsFocus()) continue;
				SetActive(m_childs[i-1]);
				//m_pActive=m_childs[i-1];
				//m_pActive->OnF
				break;
			}
			//			if(pWnd->m_nStyle&XES_DIALOG)
			//				delete pWnd;
			InvalidateRect(rect);
			return XTRUE;
		}
	}
	return XFALSE;
}

void XWindow::ChangeActive(XBOOL bNext)
{

	//	XWindow*pActive=XNULL;

	int id=-1;
	XVector<XWindow*>list;

	for(int i=0;i<(int)m_childs.GetSize();i++)
	{
		if(!m_childs[i]->IsVisiable()) continue;
		if(!m_childs[i]->IsFocus()) continue;
		list.Add(m_childs[i]);
		if(m_childs[i]==m_pActive)
		{id=list.GetSize()-1;}
	}
	if(list.GetSize()<=0) return;
	if(bNext) {if(id+1<(XINT)list.GetSize()) id++;else id=0;}
	else	  {if(id>0) id--;else id=list.GetSize()-1;}

	SetActive(list[id]);
}

//DEL void XWindow::SetValidClip()
//DEL {
//DEL 	if(m_clip.IsRectEmpty()) return;
//DEL 	m_clip
//DEL }

//DEL void XWindow::GetRoundRect(XRect &r)
//DEL {
//DEL
//DEL 	r|=
//DEL }

void XWindow::FormatClip(XRect&rect,XPoint&pt)
{
	//if(m_pParent==XNULL) return;
	XRect r=m_rect;
	//r-=r.TopLeft();
	//GetClientRect(r);
	pt+=m_rect.TopLeft();

	r-=pt;

	rect&=r;
	if(m_pParent!=XNULL)
		m_pParent->FormatClip(rect,pt);
}

//DEL void XWindow::DockWindow(XWindow *pWnd, XU8 s)
//DEL {
//DEL 	switch(s)
//DEL 	{
//DEL 	}
//DEL }

void XWindow::ClearMessage()
{
	m_nReadID=m_nSetID=0;
}


void XWindow::SetFocus(XBOOL bFocus)
{
	if(bFocus)
	{	//Ëé∑ÂæóÁÑ¶ÁÇπ
		if(!(m_nStyle&XES_SETFOCUS))
			OnFocus();
	}
	else
	{	//Â§±ÂéªÁÑ¶ÁÇπ
		if(m_nStyle&XES_SETFOCUS)
			OnBlur();
	}
}

// 定时功能调用，没一个创建注册的窗口，该函数会被系统�?0毫秒频率调用
void XWindow::TimeTick()
{
	if(!IsVisiable()) return;
	OnTimer();
	// Ë∞ÉÁî®ÊâÄÊúâÂ≠êÁ™óÂè£Â§ÑÁêÜ
	for(XU32 i=0;i<m_childs.GetSize();i++)
		m_childs[i]->TimeTick();
}
/////////////////////////////////////////////////////////////////////
// ËæìÂÖ•ÊåáÈíàÔºàÈº†Ê†áÔºâÁßªÂä®
void XWindow::MouseMove(XPoint&pt,int iPos)
{
	if(m_pCapture!=XNULL)
	{ //ËÆæÁΩÆ‰∫ÜÂÖ®Â±ÄÊçïÊçâÁ™óÂè£ÔºåË∞ÉÁî®ËØ•Á™óÂè£Â§ÑÁêÜ

		m_pCapture->ScreenToClient(pt);
		m_pCapture->OnMouseMove(pt,iPos);
		return;
	}
	if(!IsVisiable()) return;//当前窗口不可�?
	if(m_pActive!=XNULL&&m_pActive->IsPopup())
	{
		//存在一个活动的弹出窗口（如菜单�?
		pt-=m_rect.TopLeft();
		m_pActive->MouseMove(pt,iPos);
		return;
	}
	//ÂÖâÊ†á‰ΩçÁΩÆ‰Ωç‰∫éÁ™óÂè£Âå∫ÂüüÂÜÖÈÉ®
	if(m_rect.PtInRect(pt))
	{
		if(!(m_nStyle&XES_MOUSEON))
			//第一次位于区域内，发送指针进入窗口命�?
			OnMouseEnter();

		m_nStyle|=XES_MOUSEON;
	}
	else
	{   //位于区域�?
		if(m_nStyle&XES_MOUSEON)
			//Á¨¨‰∏ÄÊ¨°‰Ωç‰∫éÂå∫ÂüüÂ§ñÔºåÂèëÈÄÅÊåáÈíàÁ¶ªÂºÄÂëΩ‰ª§
			OnMouseOut();
	}
	//发送指针移动命�?
	pt-=m_rect.TopLeft();
	OnMouseMove(pt,iPos);

	// 所有子窗口处理该消�?
	for(XU32 i=0;i<m_childs.GetSize();i++)
	{
		XPoint ptt=pt;
		m_childs[i]->MouseMove(ptt,iPos);

	}
}
//////////////////////////////////////////////
// 当该窗口有字符输入时候调�?
XBOOL XWindow::Char(XWCHAR c)
{
	if(m_pCapture!=XNULL)
	{ // ËÆæÁΩÆ‰∫ÜÂÖ®Â±ÄÊçïÊçâÁ™óÂè£
		return m_pCapture->OnChar(c);
	}
	// 窗口不可�?
	if(!IsVisiable()) return XFALSE;
	if(m_pActive!=XNULL)
	{
		// 存在活动子窗�?
		if(m_pActive->OnChar(c)) return XTRUE;
	}
	return OnChar(c);
}
// ÊåâÈîÆÊó∂Ë∞ÉÁî®ÔºåÊµÅÁ®ãÂíåOnCharÁõ∏Âêå
XBOOL XWindow::KeyDown(XKEYMSG&m)
{
	if(m_pCapture!=XNULL)
	{
		return m_pCapture->OnKeyDown(m);
	}
	if(!IsVisiable()) return XFALSE;
	if(m_pActive!=XNULL)
	{
		if(m_pActive->OnKeyDown(m)) return XTRUE;
	}
	return OnKeyDown(m);
}
// 按键放开时调�?
XBOOL XWindow::KeyUp(XKEYMSG&m)
{
	if(m_pCapture!=XNULL)
	{
		return  m_pCapture->OnKeyUp(m);
		//return;
	}
	if(!IsVisiable()) return XFALSE;
	if(m_pActive!=XNULL)
	{
		if(m_pActive->OnKeyUp(m)) return XTRUE;
		//if(m_pActive->IsPopup()) return;
	}
	return OnKeyUp(m);
}
// 设置窗口位置和大�?
void XWindow::MoveWindow(const XRect&rect)
{
	//	if(!IsVisiable()) return;
	m_rect=rect;
	m_clip=rect;
	m_clip-=rect.TopLeft();
	XRect tempRect = rect;
	OnMove(tempRect);
}
// ÊåáÈíàÁÇπÂáªÔºàÊåâ‰∏ãÊùæÂºÄ‰∏ÄÊ¨°Ôºâ
void XWindow::Click(XPoint&pt,int iPos)
{
	if(!IsVisiable()) return;
	pt-=m_rect.TopLeft();
	OnClick(pt,iPos);
}
// 指针双击（连续按下松开两次�?
void XWindow::DblClick(XPoint&pt,int iPos)
{
	if(m_pCapture!=XNULL)
	{
		m_pCapture->ScreenToClient(pt);
		m_pCapture->OnDblClick(pt,iPos);
		return;
	}
	if(!IsVisiable()) return;
	XWindow*p=MouseEvent(pt);
	if(p==XNULL) return;
	if(p==this) p->OnDblClick(pt,iPos);
	else p->DblClick(pt,iPos);
}
// ÊåáÈíàÊùæÂºÄ
void XWindow::MouseUp(XPoint&pt,int iPos)
{
	if(m_pCapture!=XNULL)
	{
		m_pCapture->ScreenToClient(pt);
		m_pCapture->OnMouseUp(pt,iPos);
		return;
	}
	if(!IsVisiable()) return;
	XWindow*p=//m_pActive?m_pActive://this->SetActive
			  MouseEvent(pt);
	bool bClick=p==m_pActive||m_pActive==NULL;
	if(m_pActive)
		p=m_pActive;
	if(p==XNULL) return;
	if(p==this)
	{
		XPoint ptt=pt;
		p->OnMouseUp(pt,iPos);
		if(bClick)
			p->OnClick(ptt,iPos);
	}
	else p->MouseUp(pt,iPos);
}
// ÊåáÈíàÊåâ‰∏ã
void XWindow::MouseDown(XPoint&pt,int iPos)
{
	//LOGE("%s, %d------> %d, %d", __FUNCTION__, __LINE__, pt, iPos );

	if(m_pCapture!=XNULL)
	{
		m_pCapture->ScreenToClient(pt);
		m_pCapture->OnMouseDown(pt,iPos);
		return;
	}
	if(!IsVisiable()) return;
	//XPoint pold=pt;
	XWindow*p=MouseEvent(pt);
	if(p==XNULL) return;
	if(p==this)
	{
		SetActive(XNULL);
		//OnClick(pt);
		OnMouseDown(pt,iPos);
	}
	else
	{
		if(m_pActive!=p&&p->IsFocus())
		{
			SetActive(p);
		}
		//		else
		//		p->SetFocus(XTRUE);
		p->MouseDown(pt,iPos);
	}
}


//	转换窗口坐标为屏幕坐标
void XWindow::ClientToScreen(XPoint&pt)
{
	pt+=m_rect.TopLeft();
	XWindow*p=GetParent();
	while(p!=XNULL)
	{
		pt+=p->m_rect.TopLeft();
		p=p->GetParent();
	}
}
//	转换屏幕坐标为窗口坐标
void XWindow::ScreenToClient(XPoint&pt)
{
	pt-=m_rect.TopLeft();
	XWindow*p=GetParent();
	while(p!=XNULL)
	{
		pt-=p->m_rect.TopLeft();
		p=p->GetParent();
	}
}

XWindow* XWindow::MouseEvent(XPoint&pt)
{
	//if(!m_rect.PtInRect(pt)) return XNULL;
	pt-=m_rect.TopLeft();
	if(m_pActive!=XNULL&&m_pActive->IsPopup())
	{
		//if(!m_pActive->m_rect.PtInRect(pt)) return XNULL;
		return m_pActive;
	}

	for(XU32 i=m_childs.GetSize();i>0;i--)
	{
		if(!m_childs[i-1]->IsVisiable()) continue;
		if(m_childs[i-1]->m_rect.PtInRect(pt))
			return m_childs[i-1];
	}

	return this;
}

int XGlobal::ScreenBitDepth()
{
#ifdef _SYMBIAN
	return 32;
#elif defined(_WIN32)
	HDC hdc	= GetDC(0);		// gets a screen compatible DC

	int bits =  GetDeviceCaps( hdc, BITSPIXEL );
	int planes = GetDeviceCaps( hdc, PLANES );
	int depth = bits * planes;

	ReleaseDC( 0, hdc );

	return depth;
#elif defined (__APPLE__)
	return 32;
#elif defined (__ANDROID__ )
	return 32;
#endif
}

int XGlobal::GetScreenPalette(_XCOLOR colors[])
{
#ifdef _SYMBIAN
	colors[0].nColor=0;
	return 0;
#elif defined(_WIN32)
	int depth = ScreenBitDepth();
	if(depth>8) return 0;
	int nColors = 1<<depth;

	// Get the system palette entries
	HDC dc = GetDC(0);
	//LOGPALETTE256 sysPal;
	PALETTEENTRY palEntry[256];

	int nNums = GetSystemPaletteEntries(dc, 0, nColors, palEntry);

	nColors=XMIN(nNums,nColors);

	ReleaseDC(0, dc);

	for ( int i = 0; i<nColors; i++ )
	{
		colors[i].r   = palEntry[i].peRed;
		colors[i].g = palEntry[i].peGreen;
		colors[i].b  = palEntry[i].peBlue;
		colors[i].a = 255;
	}

	ReleaseDC( 0, dc );
	return nColors;
#elif defined(__APPLE__)
	colors[0].nColor = 0;
	return 0;
#elif defined( __ANDROID__ )
	colors[0].nColor = 0;
	return 0;
#endif
}


void XWindow::DrawButton(XGraphics &g, const XRect &r,XU32 c,XU8 b)
{
	XImage*pImage=GetIcon(XICON_BUTTON);
	if(pImage==XNULL) g.DrawButton(r,c,b);

	int x,y;
	int w=pImage->Width(),h=pImage->Height();

	int di=2,si=0;
	//(b&XBS_BFRAME)?2:((b)
	if(b&XBS_BFRAME) di=0;
	else
		if(b&XBS_WFRAME) di=1;
	if(b&XBS_THICK)
	{
		if(di<1)
			di=1;
	}
	if(r.Height()>24)
	{
		XRect rr(12,12,36,36);
		for(y=r.top+12;y<r.bottom-12;y+=24)
		{
			for(x=r.left+12;x<r.right-12;x+=24)
			{
				XPoint dtt(x,y);
				g.DrawImage(*pImage,dtt,rr);
			}
		}
	}

	XRect tr(12,di,36,di+12);
	XRect br(12,h-di-12,36,h-di);
	for(x=r.left+12;x<r.right-12;x+=24)
	{
		XPoint dt1(x,0);
		g.DrawImage(*pImage,dt1,tr);
		XPoint dt2(x,r.bottom-12);
		g.DrawImage(*pImage,dt2,br);
	}

	XRect lr(di,12,di+12,36);
	XRect rr(w-di-12,12,w-di,36);
	for(y=r.top+12;y<r.bottom-12;y+=24)
	{
		XPoint dt1(0,y);
		g.DrawImage(*pImage,dt1,lr);
		XPoint dt2(r.right-12,y);
		g.DrawImage(*pImage,dt2,rr);
	}


	XPoint dt(r.left,r.top);
	XRect ri(di,di,12+di,12+di);
	g.DrawImage(*pImage,dt,ri);
	ri.right=w-di;
	ri.left=w-di-12;
	dt.x=r.right-12;
	g.DrawImage(*pImage,dt,ri);

	dt.y=r.bottom-12;
	ri.bottom=h-di;
	ri.top=h-di-12;
	g.DrawImage(*pImage,dt,ri);

	ri.right=di+12;
	ri.left=di;
	dt.x=r.left;
	g.DrawImage(*pImage,dt,ri);

}

void XWindow::ACC(int x, int y, int z)
{
	//LOGE("%s, %d, x=%d, y=%d, z=%d ", __FILE__, __LINE__, x, y, z );
	if(m_pCapture!=XNULL)
	{ //ËÆæÁΩÆ‰∫ÜÂÖ®Â±ÄÊçïÊçâÁ™óÂè£ÔºåË∞ÉÁî®ËØ•Á™óÂè£Â§ÑÁêÜ
		m_pCapture->OnACC(x,y,z);
		return;
	}

	OnACC(x,y,z);
	// 所有子窗口处理该消息
	for(XU32 i=0;i<m_childs.GetSize();i++)
	{
		if(m_childs[i])
		{
			m_childs[i]->ACC(x,y,z);
		}
	}
}


