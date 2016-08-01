// XListBox.cpp: implementation of the XListBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XListBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XListBox::XListBox()
{
   m_nListIndex=(XU8)-1;
   m_bDropList=XFALSE;
}

XListBox::~XListBox()
{

}

XBOOL XListBox::Create(XRect &rect, XWindow *pWnd, XGraphics *g, XU8 nStyle)
{
   m_nListStyle=nStyle;
   m_nMaxList=rect.Height()/m_nTextHeight;
   XBOOL bOK=XEdit::Create(rect,pWnd,g,XEDS_ANY);
   m_nListIndex=0;
   m_nMatchOffset=0;
   switch(nStyle)
   {
   case XLS_LIST:
	    m_nEditStyle|=XEDS_UPLIST;
		m_nMatchIndex=0;
	    break;
   case XLS_DROPLIST:
	    m_nMatchIndex=0;
		m_nListIndex=0;
	    m_nMaxList--;
		//m_bDropList=XFALSE;
		SetDropList(XFALSE);
		break;
   default:
	    m_nMatchIndex=0;
	    m_nListIndex=0;
	    SetDropList(XFALSE);
	    m_nMaxList--;
   }
   
   m_nCmd=XCMD_LIST;
   return bOK;
}

XBOOL XListBox::OnPaint(XGraphics &g)
{
   switch(m_nListStyle)
   {
   case XLS_LIST:
	    PaintList(g);
	    break;
   case XLS_DROPLIST:
	    PaintDropList(g);
		break;
   case XLS_DROPDOWN:
	    PaintDropList(g);
		break;
   }
   return XTRUE;
}

void XListBox::PaintList(XGraphics &g)
{
	//m_rect.top-=m_nTextHeight;
	XEdit::PaintList(g);
	if(m_nListIndex==m_nMatchIndex) return;
	if(m_nListIndex>=m_nMatchOffset&&
	   m_nListIndex-m_nMatchOffset<m_nMaxList)
	{
		g.SetColor(GetSysColor(XCW_LINK));
		g.DrawString(m_list[m_nListIndex],
					 3,1+m_nTextHeight*(m_nListIndex-m_nMatchOffset));
	}
	//m_rect.top+=m_nTextHeight;
}

XBOOL XListBox::AddString(XPCWSTR str)
{
	if(m_list.GetSize()>=0xfe) return XFALSE;
	m_list.Add(str);
	m_nFormats.Add((XU16)(m_list.GetSize()-1));
	if(m_nListStyle!=XLS_LIST&&m_list.GetSize()-1==m_nListIndex)
	{
		m_strText=str;
		m_nCur=0;
		m_nSelectCount=(XU16)m_strText.GetLength();
		//OnFocus();
	}
	Invalidate();
	return XTRUE;
}

void XListBox::OnMouseMove(XPoint &pt,int iPos)
{
	if(iPos) return;
	switch(m_nListStyle)
	{
	case XLS_DROPDOWN:
	case XLS_DROPLIST:
		 XEdit::OnMouseMove(pt,iPos);
		 break;
	case XLS_LIST:
		 OnMoveList(pt);
		 break;
	}
}

void XListBox::OnMoveList(XPoint &pt)
{
	XU8 id=m_nMatchIndex;
	m_nMatchIndex=m_nListIndex;
	XEdit::OnMouseMove(pt,0);
	m_nListIndex=m_nMatchIndex;
	m_nMatchIndex=id;
}

void XListBox::OnClick(XPoint &pt,int iPos)
{
   if(iPos) return;
   switch(m_nListStyle)
   {
   case XLS_LIST:
	    if(m_nMatchIndex!=m_nListIndex)
		{
		 m_nMatchIndex=m_nListIndex;
		 Invalidate();
		 PostMsg(XWM_COMMAND,XCMD_LIST_CHANGE,(XU32)this,XMS_PARENT);
		}
		PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
	    break;
   case XLS_DROPDOWN:
	   {
		   XRect rect;
		   GetClientRect(rect);
		   rect.right-=rect.Height();
		   if(rect.PtInRect(pt))
		   {			   
			   if(m_bDropList)
			   {
				   Invalidate();
				   SetDropList(XFALSE);
			   }
			   XEdit::OnClick(pt,iPos);
			   break;
		   }
	   }
   case XLS_DROPLIST:
	    ClickDropList(pt);
		break;
   }
}

XBOOL XListBox::OnKeyDown(XKEYMSG&m)
{
   switch(m_nListStyle)
   {
   case XLS_LIST:
	   switch(m.nKeyCode)
		{
		case XK_RETURN:
			 PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
			 return XTRUE;
		case XK_UP:
		case XK_DOWN:
			 if(XEdit::OnKeyDown(m))
			 {
				 PostMsg(XWM_COMMAND,XCMD_LIST_CHANGE,(XU32)this,XMS_PARENT);
				 return XTRUE;
			 }
			 break;
		}
	    break;
	case XLS_DROPDOWN:
	case XLS_DROPLIST:
		 if(OnKeyDropList(m))
		 {
			 if(m_nListStyle==XLS_DROPDOWN)
				SetOffset();
			 return XTRUE;
		 }
		 else if(m_nListStyle==XLS_DROPDOWN)
		 {
			 if(XEdit::OnKeyDown(m))
			 {
				 if(m_bDropList)
					 SetDropList(XFALSE);
				 //MatchList();
				 //XU8 v=MatchList();
				 //if(v!=(XU8)-1)
				//	 m_nMatchIndex=v;
			 }
			//	 ResetMatchList();
		 }
		 
		
   }
   return XFALSE;
}

XBOOL XListBox::OnChar(XWCHAR c)
{
	 if(m_nListStyle==XLS_DROPDOWN)
	 {
		 return XEdit::OnChar(c);
		 /*{
			 return XTRUE;
			 MatchList();
		 }*/
	     //ResetMatchList();
	 }
	 return XFALSE;
}

void XListBox::OnTimer()
{
    switch(m_nListStyle)
	{
	case XLS_DROPDOWN:
		 XEdit::OnTimer();
		 break;
	case XLS_DROPLIST:
		 if(!m_bDropList) break;
		 if(TickList(m_nMatchIndex))
			 Invalidate();
		 break;
	case XLS_LIST:
		 if(TickList(m_nListIndex))
			 Invalidate();
		 break;
	}
}

void XListBox::OnMouseDown(XPoint &pt,int iPos)
{
	if(iPos) return;
	if(m_nListStyle==XLS_DROPDOWN)
		XEdit::OnMouseDown(pt,iPos);
}

void XListBox::OnMouseUp(XPoint &pt,int iPos)
{
	if(iPos) return;
	if(m_nListStyle==XLS_DROPDOWN)
		XEdit::OnMouseUp(pt,iPos);
}

void XListBox::OnDblClick(XPoint &pt,int iPos)
{
	if(iPos) return;
	if(m_nListStyle==XLS_DROPDOWN)
		XEdit::OnDblClick(pt,iPos);
}

void XListBox::PaintDropList(XGraphics &g)
{
	XBOOL b=m_nStyle&XES_SETFOCUS;
	//XEdit::PaintEdit(g);
	XRect rect;
	GetClientRect(rect);

	if(m_nListStyle==XLS_DROPLIST)
	{
		XColor cw(GetSysColor(XCW_BACK));
		cw.Bright(30);
		g.SetBackColor(cw);
   
		
		g.FillRectangle(rect);
		XColor ct(GetSysColor(XCW_TEXT));
		g.SetColor(ct);
		if(b&&!m_bDropList)
		{
			g.SetBackColor(GetSysColor(XCW_SELECTBACK));
			g.FillRectangle(XRect(rect.left+2,rect.top+2,rect.right-2,rect.bottom-2));
			g.SetColor(GetSysColor(XCW_SELECTTEXT));
		}

		g.DrawString(m_strText,rect.left+4,rect.top+2);
		
		cw=GetSysColor(XCW_TEXT);
		cw.Bright(20);
		g.SetColor(cw);
		g.FrameRectangle(rect);
	}
	else XEdit::PaintEdit(g);


	rect.InflateRect(-1,-1);
	rect.left=rect.right-rect.Height();
	XColor cc(GetSysColor(XCW_BUTTON));
	if(!b) {cc=GetSysColor(XCW_BACK);cc.Mix(XColor(GetSysColor(XCW_TEXT)),0.1f);}
	g.DrawButton(rect,cc,XBS_BFRAME|XBS_WFRAME);
	cc.Dark(60);
	rect.bottom--;
	g.DrawDirect(rect,XK_DOWN,cc,!b);

	if(m_bDropList>0)
		XEdit::PaintList(g);
	//g.DrawButton(rect,GetSysColor(b?XCW_BUTTON),XBS_BFRAME|XBS_WFRAME);
	//g.DrawDirect(rect,XK_DOWN,GetSysColor(XCW_TEXT),XFALSE);
}

XBOOL XListBox::Select(XU8 nIndex)
{
    switch(m_nListStyle)
	{
	case XLS_LIST:
		 m_nMatchIndex=nIndex;
		 
		 Invalidate();
		 break;
	case XLS_DROPDOWN:
	case XLS_DROPLIST:
		 if(m_nListIndex!=nIndex)
		 {
			if(nIndex!=(XU8)-1&&nIndex<m_nFormats.GetSize())
			{
				m_nListIndex=nIndex;
				m_strText=m_list[nIndex];
				Invalidate();
			}

		 }break;
	}		  
	TickList(nIndex);
	return XTRUE;
}

void XListBox::OnBlur()
{

	Invalidate();

	XWindow::OnBlur();
	SetDropList(XFALSE);
	
	
}

void XListBox::SetDropList(XBOOL bList)
{
	//if(m_bDropList==bList) return;
	m_bDropList=bList;
	if(!bList)
	{
		m_nStyle&=~XES_POPUP;
		//m_nListIndex=m_nMatchIndex;
		if(m_nEditStyle&XEDS_UPLIST)
			m_rect.top=m_rect.bottom-m_nTextHeight-1;
		else
			m_rect.bottom=m_rect.top+m_nTextHeight+1;
	}
	else
	{		 
		m_nStyle|=XES_POPUP;
		m_nMatchIndex=m_nListIndex;
		int nLines=XMIN(m_nMaxList,m_nFormats.GetSize());
		XU8 v=m_nMatchIndex;
		TickList(v);
		if(m_nEditStyle&XEDS_UPLIST)
			m_rect.top=m_rect.bottom-m_nTextHeight*(nLines+1)-2;
		else
			m_rect.bottom=m_rect.top+m_nTextHeight*(nLines+1)+2;
	}
}

XBOOL XListBox::OnKeyDropList(XKEYMSG&m)
{
	int c=m.nKeyCode;
	switch(c)
	{
	case XK_CLEAR:
		 if(m_bDropList&&m_nListStyle==XLS_DROPLIST)
		 {
			 Invalidate();
			 SetDropList(XFALSE);
			 return XTRUE;
		 }
		 break;
	case XK_RETURN:
		 if(m_bDropList)
		 {
			 if(m_nMatchIndex!=(XU8)-1&&m_nMatchIndex<m_nFormats.GetSize())
			 {
				 m_nListIndex=m_nMatchIndex;
				 m_strText=m_list[m_nMatchIndex];
				 m_nCur=0;
				 m_nSelectCount=(XU16)m_strText.GetLength();
				 PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
			 }
			 Invalidate();
			 SetDropList(XFALSE);
		 }
		 else
		 {
			 SetDropList(XTRUE);
			 Invalidate();
		 }
		 return XTRUE;
	case XK_UP:
	case XK_DOWN:
		 if(m_bDropList)
			return XEdit::OnKeyDown(m);
	/*case XK_DOWN:
		 if(!m_bDropList)
		 {
			 SetDropList(XTRUE);
			 Invalidate();
			 return XTRUE;
		 }
		 else 
		 {
			 if(m_nMatchIndex>=m_nFormats.GetSize()-1&&(m_nEditStyle&XEDS_UPLIST))
			 {
				 Invalidate();
				 SetDropList(XFALSE);
			 }
			 return XEdit::OnKeyDown(c);
		 }
	case XK_UP:
		 if(!m_bDropList)
		 {
			 SetDropList(XTRUE);
			 Invalidate();
			 return XTRUE;
		 }
		 else
		 if(m_bDropList)
		 {
			 if(m_nMatchIndex<=0&&!(m_nEditStyle&XEDS_UPLIST))
			 {
				 Invalidate();
				 SetDropList(XFALSE);
				 return XTRUE;
			 }
			 return XEdit::OnKeyDown(c);
		 } */
	}
	return XFALSE;
}

XU8 XListBox::MatchList()
{
//	m_nFormats.RemoveAll();
	//m_nMatchIndex=0;
	for(XU8 i=0;i<m_list.GetSize();i++)
	{
		//m_nFormats.Add(i);
		int nSize=XMIN(m_list[i].GetLength(),m_strText.GetLength());
		if(m_list[i].Compare(m_strText,XTRUE,nSize)==0)
		{
			m_nListIndex=i;
			//TickList(i);
			return i;
		}
	}
	return (XU8)-1;
}

void XListBox::SetMatchList(XBOOL bEmpty)
{
	MatchList();
	//m_nMatchIndex=0;
	/*for(XU16 i=0;i<m_list.GetSize();i++)
	{
		m_nFormats.Add(i);
		int nSize=XMIN(m_list[i].GetLength(),m_strText.GetLength());
		if(m_list[i].Compare(m_strText,XTRUE,nSize)==0)
			m_nMatchIndex=(XU8)i;
	}*/
}

void XListBox::ClickDropList(XPoint &pt)
{
	    if(m_bDropList)
		{
			XRect rect;
			GetListRect(rect);
			if(rect.PtInRect(pt)&&m_nMatchIndex!=(XU8)-1&&m_nMatchIndex<m_nFormats.GetSize())
			{
			   m_nListIndex=m_nMatchIndex;
			   m_strText=m_list[m_nMatchIndex];
			   m_nCur=0;
			   m_nSelectCount=(XU16)m_strText.GetLength();
			   PostMsg(XWM_COMMAND,m_nCmd,(XU32)this,XMS_PARENT);
			}
		    Invalidate();
			SetDropList(XFALSE);
		}
		else
		{
			XRect rect;
			GetClientRect(rect);
			if(rect.PtInRect(pt))
			{
				SetDropList(XTRUE);
				//m_nMatchIndex=0;
				Invalidate();
			}
		}
}

void XListBox::SetLines(XU8 nLines)
{
	if(m_nMaxList==nLines) return;
	if(nLines>m_list.GetSize())
		nLines=(XU8)m_list.GetSize();
	m_nMaxList=nLines;
	if(m_bDropList)
	{
		if(m_nEditStyle&XEDS_UPLIST)
			m_rect.top=m_rect.bottom-m_nTextHeight*(nLines+1)-2;
		else
			m_rect.bottom=m_rect.top+m_nTextHeight*(nLines+1)+2;
	}
	XU8 v=m_nListIndex;
	TickList(v);
	//Invalidate();
}

void XListBox::OnFocus()
{
	if(m_nListStyle==XLS_DROPDOWN)
		XEdit::OnFocus();
	else
		XWindow::OnFocus();
}

XBOOL XListBox::IsPopup()
{
   return m_bDropList;
}
