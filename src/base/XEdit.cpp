// XEdit.cpp: implementation of the XEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XEdit.h"

#ifdef __APPLE__
#include "GlobalPortingFunction.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static XWCHAR _strPassChar='*';//_XW('*');

#define MAXLISTLINES 7

XEdit::XEdit()
{
   m_nEditStyle=XEDS_ANY;
   m_nCur=0;
   m_nSelectCount=0;
   m_nOffset=0;
   m_nMatchOffset=0;
   m_nMatchIndex=(XU8)-1;
   m_nMaxLength=256;
   m_pGraphics=XNULL;
   m_nDownCur=(XU16)-1;
   m_nMatchs=0;
   m_nMaxList=MAXLISTLINES;
}

XEdit::~XEdit()
{

}

XBOOL XEdit::Create(XRect &rect, XWindow *pWnd,XGraphics*p, XU8 s)
{
#ifdef __APPLE__
	m_pGraphics = (XGraphics*)g_getGlobalGraphics();
#else
   m_pGraphics=p;
#endif
   m_nCur=0;
   m_nSelectCount=0;
   if(s!=0) m_nEditStyle=s;
   m_nOffset=0;
   m_nTicks=0;
   m_nDownCur=(XU16)-1;
   m_nMatchs=0;
   //if(!(s&XEDS_MULTILINE)) m_rect.bottom=m_rect.top
   //m_strText.Empty();
   return XWindow::Create(rect,XES_VISIABLE|XES_FOCUS,pWnd,XCMD_EDIT);

}


XBOOL XEdit::OnPaint(XGraphics &g)
{
   //XColor cb(GetSysColor(XCW_BUTTON));
   PaintEdit(g);

   if(!(m_nEditStyle&XEDS_MULTILINE)&&m_nFormats.GetSize()>0)
   {
	 PaintList(g);

   }
   return XTRUE;
   //if(m_nEditStyle&XEDS
   //g.DrawFrame(rect,cb,1);
}

void XEdit::PaintCursor(XGraphics &g)
{
   if(!(m_nStyle&XES_SETFOCUS)||m_nSelectCount>0) return;
   if(m_nTicks<4) return;
   XPoint pt;
   CurToPosition(g,pt);
   g.DrawLine(pt.x,pt.y,pt.x,pt.y+m_nTextHeight-4);
   g.DrawLine(pt.x+1,pt.y,pt.x+1,pt.y+m_nTextHeight-4);
}

void XEdit::PaintText(XGraphics &g)
{
	if(m_strText.GetLength()<=0) return;
	if(m_nEditStyle&XEDS_MULTILINE)
	{
		int by=2;
		for(XU32 i=m_nOffset*3;i<m_nFormats.GetSize();i+=3)
		{
			DrawLine(g,m_strText.GetData()+m_nFormats[i],by,m_nFormats[i],m_nFormats[i+1]);
			by+=m_nTextHeight;
		}
	}
	else
	{
		XRect rect;
	    GetClientRect(rect);
		if(m_nEditStyle&XEDS_PASSWORD)
		{
			XString16 str;
			str.Insert(0,_strPassChar,(XU16)(m_strText.GetLength()-m_nOffset));
			DrawLine(g,str.GetData(),rect.top+1,m_nOffset,
				m_strText.GetLength()-m_nOffset);
		}
		else
		{
			DrawLine(g,m_strText.GetData()+m_nOffset,rect.top+1,m_nOffset,
				m_strText.GetLength()-m_nOffset);
		}
	}
}

void XEdit::OnTimer()
{
	if(m_nSelectCount<=0&&(m_nStyle&XES_SETFOCUS))
	{
		if(m_nTicks<12)
		{
			m_nTicks++;
			if(m_nTicks==4) Invalidate();
		}
		else
		{
			m_nTicks=0;
			Invalidate();
		}
	}
	if(!(m_nEditStyle&XEDS_MULTILINE))
	{
		if(TickList(m_nMatchIndex))
			Invalidate();
	}
}



XBOOL XEdit::OnKeyDown(XKEYMSG&m)
{
   XBOOL bList=!(m_nEditStyle&XEDS_MULTILINE)&&m_nFormats.GetSize()>0;
   int c=m.nKeyCode;
   switch(c)
   {
     default:
		  if(c<' ')
			  return XFALSE;
		  break;
	 case XK_RETURN:
		  if(bList)
		  {
			  InsertList();
			  SetMatchList(XTRUE);
			  bList=XFALSE;
			  break;
		  }
		  else if(m_pParent==XNULL)
		  {
			  return XFALSE;
		  }
		  else
		  {
			  PostMsg(XWM_COMMAND,XCMD_EDIT_CHANGE,(XU32)this,XMS_PARENT);
			  return XTRUE;
		  }

	 case XK_DOWN:
		  if(bList)
		  {
			  if(m_nMatchIndex==(XU8)-1)
			  {
				  m_nMatchIndex=0;
				  XU8 v=m_nMatchIndex;
				  TickList(v);
			  }
			  else if(m_nMatchIndex+1<(XINT)m_nFormats.GetSize())
			  {
				  m_nMatchIndex++;
				  XU8 v=m_nMatchIndex;
				  TickList(v);
			  }
			  bList=XFALSE;
			  break;
		  }
		  else if(m_nEditStyle&XEDS_MULTILINE)
		  {
			  XPoint pt;
			  CurToPosition(*m_pGraphics,pt);
			  pt.y+=m_nTextHeight+(m_nTextHeight>>1);
			  m_nCur=PositionToCur(pt);
			  break;
		  }
		  return XFALSE;
	 case XK_UP:
		  if(bList)
		  {
			  if(m_nMatchIndex==(XU8)-1)
			  {
				  //if(m_nEditStyle&XEDS_UPLIST)
					//  m_nMatchIndex=0;
				  //else
				  m_nMatchIndex=m_nFormats.GetSize()-1;
				  XU8 v=m_nMatchIndex;
				  TickList(v);
			  }
			  else if(m_nMatchIndex>0)
			  {
				  m_nMatchIndex--;
				  XU8 v=m_nMatchIndex;
				  TickList(v);
			  }
			  /*else if(m_nMatchIndex==(XU8)-1)
			  {
				  m_nMatchIndex=0;
				  m_nMatchOffset=0;
			  }*/
			  bList=XFALSE;
			  break;
		  }
		  else if(m_nEditStyle&XEDS_MULTILINE)
		  {
			  XPoint pt;
			  CurToPosition(*m_pGraphics,pt);
			  pt.y-=m_nTextHeight-(m_nTextHeight>>1);
			  m_nCur=PositionToCur(pt);
			  break;
		  }
		  return XFALSE;
	 case XK_RIGHT:
		  if(m_nSelectCount>0)
		  {
			  m_nCur+=m_nSelectCount;
			  m_nSelectCount=0;
			  m_nMatchs=0;
		  }
		  else if(m_nCur<m_strText.GetLength())
		  {
			  m_nCur++;
			  m_nMatchs=0;
		  }
		  else if(!(m_nEditStyle&XEDS_MULTILINE))return XFALSE;
		  SetMatchList(XTRUE);
		  break;
	 case XK_LEFT:
		  if(m_nSelectCount>0)
		  {
			  m_nCur+=m_nSelectCount;
			  m_nSelectCount=0;
			  m_nMatchs=0;
		  }
		  if(m_nCur>0)
		  {
			  m_nCur--;
			  m_nMatchs=0;
		  }
		  else if(!(m_nEditStyle&XEDS_MULTILINE)) return XFALSE;
		  SetMatchList();
		  break;
	 case XK_CLEAR:
		  //if(m_nCur==0&&m_nSelectCount<=0) return XFALSE;
		  if(m_nSelectCount>0)
		  {
			  m_strText.RemoveAt(m_nCur,m_nSelectCount);
			  m_nSelectCount=0;
		  }
		  else if(m_nCur>0)
		  {
			 m_strText.RemoveAt(m_nCur-1);
			 m_nCur--;
		  }
		  if(m_nEditStyle&XEDS_MULTILINE)
			{
				m_nFormats.RemoveAll();
				m_pGraphics->FormatString(m_strText,m_strText.GetLength(),0,m_rect.Width()-2,m_nFormats,0);
			}
		  PostMsg(XWM_COMMAND,XCMD_EDIT_CHANGE,(XU32)this,XMS_PARENT);
		  m_nMatchs=0;
		  SetMatchList(XTRUE);
		  break;
   }
   SetOffset();
   //if(bList)
	//   SetMatchList();
   Invalidate();
   return XTRUE;
}

XBOOL XEdit::OnChar(XWCHAR c)
{
   if(c<' ')
	   return XFALSE;
   if(m_strText.GetLength()>=m_nMaxLength)
	   return XFALSE;
   if(!(m_nEditStyle&XEDS_ANY))
   {
	   XU8 s=0;
	   if(c>='0'&&c<='9')
		   s=XEDS_NUMBER;
	   else if(c>='a'&&c<='z')
		   s=XEDS_LCHAR;
	   else if(c>='A'&&c<='Z')
		   s=XEDS_UCHAR;
	   else if(c>0xff)
		   s=XEDS_WCHAR;
	   else
		   s=XEDS_SIGN;
	   if(!(s&m_nEditStyle))
		   return XFALSE;
   }
   if(m_nSelectCount>0)
   {
	   m_strText.RemoveAt(m_nCur,m_nSelectCount);
	   m_nSelectCount=0;
   }
   m_strText.Insert(m_nCur,c);
   if(m_nEditStyle&XEDS_MULTILINE)
   {
		m_nFormats.RemoveAll();
		m_pGraphics->FormatString(m_strText,m_strText.GetLength(),0,m_rect.Width()-2,m_nFormats,0);
   }
   m_nCur++;
   m_nMatchs++;
   SetMatchList();
   SetOffset();
   Invalidate();
   PostMsg(XWM_COMMAND,XCMD_EDIT_CHANGE,(XU32)this,XMS_PARENT);
   return XTRUE;
}

void XEdit::OnMouseMove(XPoint &pt,int iPos)
{
	if(iPos) return;
    if(m_nDownCur==(XU16)-1)
	{
		if(!(m_nEditStyle&XEDS_MULTILINE)&&m_nFormats.GetSize()>0)
		{
		   XRect rect;
		   GetListRect(rect);
		   if(!rect.PtInRect(pt))
			   return;
		   int i=(pt.y-rect.top)/m_nTextHeight+m_nMatchOffset;
		   if(i<0||i>=(int)m_nFormats.GetSize()||i==m_nMatchIndex)
			   return;
		   m_nMatchIndex=i;
		   Invalidate();
		}
		return;
	}

	int c=PositionToCur(pt);
	if(m_nCur!=c)
	{
		m_nSelectCount=XABS((c-m_nDownCur));
		if(c<m_nDownCur)
		{
			m_nCur=c;
			SetOffset();
		}
		else
		{
			int cc=m_nCur;
			m_nCur=c;
			SetOffset();
			m_nCur=cc;
		}

		Invalidate();
	}
}

void XEdit::OnFocus()
{

//#ifdef __ANDROID__
//	XWCHAR *str = m_strText.GetData();
//	jstring jstr = xw2js( str );
//	GFunCallTextEdit( jstr, this );
//#endif

	XWindow::OnFocus();
	m_nCur=(XU16)m_strText.GetLength();
	SetOffset();
	m_nSelectCount=m_nCur;
	m_nCur=0;
	//m_nSelectCount=(XU16)m_strText.GetLength();
	Invalidate();
}

void XEdit::SetText(XPCWSTR str)
{
	m_strText=str;
	//m_nCur=0;
	m_nOffset=0;
	m_nCur=0;
	//m_nSelectCount=(XU16)m_strText.GetLength();
	if(m_nEditStyle&XEDS_MULTILINE)
	{
		m_nFormats.RemoveAll();
		m_pGraphics->FormatString(m_strText,m_strText.GetLength(),0,m_rect.Width(),m_nFormats,0);
	}
	Invalidate();
}

void XEdit::SetEditStyle(XU8 s)
{
	m_nEditStyle=s;
	if(s&XEDS_MULTILINE)
	{
		m_nFormats.RemoveAll();
		m_nCur=(XU16)m_strText.GetLength();
		m_nSelectCount=0;
		m_pGraphics->FormatString(m_strText,m_strText.GetLength(),0,m_rect.Width(),m_nFormats,0);
	}
	else
	{
		m_nFormats.RemoveAll();
	}
}

void XEdit::OnBlur()
{
	XWindow::OnBlur();
	m_nCur=0;
	m_nSelectCount=0;
	SetOffset();
	SetMatchList(XTRUE);
	Invalidate();
}

int XEdit::GetCurLine()
{
	for(XU32 i=0;i<m_nFormats.GetSize();i+=3)
	{
		if(m_nCur>=m_nFormats[i]&&
		   m_nCur<=m_nFormats[i]+m_nFormats[i+1])
		{
			return i/3;
		}
	}
	return 0;
}

void XEdit::CurToPosition(XGraphics&g,XPoint &pt)
{
	if(m_nEditStyle&XEDS_MULTILINE)
	{
		XU16 i=GetCurLine();
		pt.y=2+m_nTextHeight*(i-m_nOffset);
		pt.x=2;
		i=i*3;
		if(i<m_nFormats.GetSize())
		{
			if(m_nCur>m_nFormats[i])
			{
				XSize sz;
				g.MeasureString(m_strText.GetData()+m_nFormats[i],sz,
							m_nCur-m_nFormats[i]);
				pt.x+=sz.cx;
			}
		}
	}
	else
	{
	   XRect rect;
	   GetClientRect(rect);
	   pt.y=rect.top+2;
	   pt.x=rect.left+2;
	   if(m_nCur>m_nOffset)
	   {
		   XPCWSTR pData=m_strText.GetData()+m_nOffset;
		   XString16 str;
		   if(m_nEditStyle&XEDS_PASSWORD)
		   {
				str.Insert(0,_strPassChar,(XU16)(m_nCur-m_nOffset));
				pData=str.GetData();
		   }
		   {
			   XSize sz;
			   g.MeasureString(m_strText.GetData()+m_nOffset,
						   sz,m_nCur-m_nOffset);
			   pt.x+=sz.cx;
		   }
	   }
	}
}

void XEdit::SetOffset()
{
   if(!IsValid())
	   return;

   if(m_nEditStyle&XEDS_MULTILINE)
   {
	   XU16 i=GetCurLine();

	   if(i<m_nOffset)
	   {
		   if(i>0) m_nOffset=i;
		   else m_nOffset=0;
	   }
	   else
	   {
		   XU16 ls=m_rect.Height()/m_nTextHeight;
		   if(i+1>=m_nOffset+ls)
			  m_nOffset=i-ls+1;
	   }
   }
   else
   {
	   int nc=((m_rect.Width()/m_nTextHeight)>>1)+1;
	   XPoint pt;
	   CurToPosition(*m_pGraphics,pt);
	   while(m_nOffset>=m_nCur)
	   {
		  if(m_nOffset>=nc) m_nOffset-=nc;
		  else {m_nOffset=0;break;}
	   }
	   while(pt.x+m_nTextHeight>=m_rect.Width())
	   {
		   if(m_nOffset+nc<m_nCur) m_nOffset+=nc;
		   else {m_nOffset=m_nCur;break;}
		   CurToPosition(*m_pGraphics,pt);
	   }
   }
   if(m_nCur-m_nOffset<2) m_nOffset=0;
}

int XEdit::PositionToCur(XPoint &pt)
{
   if(m_strText.GetLength()<=0)
	   return 0;
   if(m_nEditStyle&XEDS_MULTILINE)
   {
	   int i=((pt.y+m_nOffset*m_nTextHeight)/m_nTextHeight)*3;
	   if(i<0)
		   return 0;
	   else if(i>=(int)m_nFormats.GetSize())
		   return m_nCur;
	   int c=m_nFormats[i];
	   c+=PositionToCur(m_strText.GetData()+c,m_nFormats[i+1],pt.x);
	   return c;
	   //return 0;
   }
   else
   {
	   XPCWSTR pData=m_strText.GetData()+m_nOffset;
	   XString16 str;
	   if(m_nEditStyle&XEDS_PASSWORD)
	   {
			str.Insert(0,_strPassChar,(XU16)(m_strText.GetLength()-m_nOffset));
			pData=str.GetData();
	   }
	  return PositionToCur(pData,m_strText.GetLength()-m_nOffset,pt.x)+m_nOffset;
   }
}

int XEdit::PositionToCur(XPCWSTR pData, int l, int x)
{
   if(l<=0)
	   return 0;

   int dd=x;
   for(int i=1;i<=l;i++)
   {
	  XSize sz;
	  m_pGraphics->MeasureString(pData,sz,i);
	  int dd1=XABS((x-sz.cx));
	  if(dd1>dd)
		  return i-1;
	  else
		  dd=dd1;
   }
   return l;

}

void XEdit::DrawLine(XGraphics &g, XPCWSTR strText,int y,int nOffset, int nLen)
{
	if(m_nSelectCount>0&&m_nStyle&XES_SETFOCUS)
	{
		int bc=XMAX(m_nCur-nOffset,0);
		int ec=XMIN(m_nCur+m_nSelectCount-nOffset,nLen);
		if(bc>0||ec<nLen)
			g.DrawString(strText,1,y+1,nLen);
		if(ec>bc)
		{
			XSize sz,sz1;
			g.MeasureString(strText,sz,bc);
			g.MeasureString(strText,sz1,ec);
			g.SetBackColor(GetSysColor(XCW_SELECTBACK));
			g.SetColor(GetSysColor(XCW_SELECTTEXT));
#if defined (__ANDROID__)	// zhengjl 2011.09.05 for input text position of web game
			g.FillRectangle(XRect(sz.cx,y,1,y+m_nTextHeight));
			g.DrawString(strText+bc,1,y+1,ec-bc);
#else
			g.FillRectangle(XRect(sz.cx,y,sz1.cx+1,y+m_nTextHeight));	
			g.DrawString(strText+bc,sz.cx+1, y+1,ec-bc);
#endif
			g.SetColor(GetSysColor(XCW_TEXT));
		}
	}
	else
	{
		g.DrawString(strText,1,y+1,nLen);
	}

}

void XEdit::OnMouseDown(XPoint&pt,int iPos)
{
	if(iPos) return;
   if(!(m_nEditStyle&XEDS_MULTILINE)&&m_nFormats.GetSize()>0)
   {
	    XRect rect;
	 	GetListRect(rect);
	 	if(!rect.PtInRect(pt))
	 	{
		 	SetMatchList(XTRUE);
	 		SetOffset();
	 		Invalidate();
		}
   }
	XRect rect;
	GetClientRect(rect);
	if(!rect.PtInRect(pt)) return;

	m_nCur=PositionToCur(pt);
	m_nDownCur=m_nCur;
	m_nSelectCount=0;
	Invalidate();
	if(GetParent()!=XNULL)
	{
		SetCapture(this);
	}
}

void XEdit::OnMouseUp(XPoint &pt,int iPos)
{
	if(iPos) return;
	 if(!(m_nEditStyle&XEDS_MULTILINE)&&m_nFormats.GetSize()>0)
	 {
		 XRect rect;
		GetListRect(rect);
	 	if(rect.PtInRect(pt))
	 	{
	 		InsertList();
	 	}
	 	SetMatchList(XTRUE);
	 	SetOffset();
   		Invalidate();
	}

	m_nDownCur=(XU16)-1;
	if(m_pParent!=XNULL)
	{
		SetCapture(XNULL);
	}
}

void XEdit::OnDblClick(XPoint &pt,int iPos)
{
	if(iPos) return;
	 XRect rect;
	 GetClientRect(rect);
	 if(!rect.PtInRect(pt))
		 return;
	 SetMatchList(XTRUE);
	 OnFocus();
}

void XEdit::GetClientRect(XRect &rect)
{
	rect=m_rect;
	rect-=m_rect.TopLeft();
	//if(!(m_nEditStyle&XEDS_MULTILINE))
	//{
	//	if(m_nEditStyle&XEDS_UPLIST)
	//		rect.top=rect.bottom-m_nTextHeight-1;
	//	else
	//		rect.bottom=rect.top+m_nTextHeight+1;

	//}
}

void XEdit::SetDownList()
{
	if(m_nEditStyle&XEDS_MULTILINE)
		return;
	m_rect.top=m_rect.bottom-m_nTextHeight-1;
	if(m_nFormats.GetSize()>0)
	{
		int nLines=XMIN(m_nFormats.GetSize(),m_nMaxList)+1;
		m_rect.bottom=m_rect.top+(m_nTextHeight*nLines)+2;
		Invalidate();
	}
	m_nEditStyle&=~XEDS_UPLIST;
}

void XEdit::SetUpList()
{
	if(m_nEditStyle&XEDS_MULTILINE)
		return;

	m_rect.bottom=m_rect.top+m_nTextHeight+1;
	if(m_nFormats.GetSize()>0)
	{
		int nLines=XMIN(m_nFormats.GetSize(),m_nMaxList)+1;
		m_rect.top=m_rect.bottom-(m_nTextHeight*nLines)-2;
		Invalidate();
	}
	m_nEditStyle|=XEDS_UPLIST;
}

void XEdit::SetLikeText(XString8 &strTxt)
{
	m_list.SplitString(strTxt,';');
}

void XEdit::SetMatchList(XBOOL bEmpty)
{
	if(m_nEditStyle&XEDS_MULTILINE)
		return;

	if(m_nFormats.GetSize()>0)
	{
		Invalidate();
		m_nStyle&=~XES_POPUP;
		if(m_nEditStyle&XEDS_UPLIST)
			m_rect.top=m_rect.bottom-m_nTextHeight-2;
		else
			m_rect.bottom=m_rect.top+m_nTextHeight+2;
	}
    m_nFormats.RemoveAll();
	if(bEmpty)
		return;
	if(m_nCur>=m_nMatchs)
	{
		XPCWSTR pData=m_strText.GetData()+m_nCur-m_nMatchs;
		for(XU16 i=0;i<m_list.GetSize();i++)
		{
			if(m_nMatchs==0)
			{
				if(m_list[i].GetLength()<2) m_nFormats.Add(i);
			}
			else if(m_list[i].GetLength()<m_nMatchs)
				continue;
			if(m_list[i].Compare(pData,XTRUE,m_nMatchs)!=0)
				continue;
			m_nFormats.Add(i);
		}
		if(m_nFormats.GetSize()<=0)
			m_nMatchs=0;
		else
		{
			m_nStyle|=XES_POPUP;
			m_nMatchOffset=0;
			m_nMatchIndex=(XU8)-1;
			int nLines=XMIN(m_nFormats.GetSize(),m_nMaxList)+1;
			if(m_nEditStyle&XEDS_UPLIST)
				m_rect.top=m_rect.bottom-m_nTextHeight*nLines-2;
			else
				m_rect.bottom=m_rect.top+m_nTextHeight*nLines+2;
		}
	}
}

void XEdit::GetListRect(XRect &rect)
{
	int nLine=XMIN(m_nFormats.GetSize(),m_nMaxList);
	rect=XRect(0,0,m_rect.Width(),nLine*m_nTextHeight);
	if(!(m_nEditStyle&XEDS_UPLIST))
		rect+=XPoint(0,m_nTextHeight+2);
}

void XEdit::InsertList()
{
	if(m_nMatchIndex==(XU8)-1||m_nMatchIndex>=m_nFormats.GetSize())
		return;
	if(m_nCur>=m_nMatchs&&m_nMatchs>0)
	{
		m_strText.RemoveAt(m_nCur-m_nMatchs,m_nMatchs);
		m_nCur-=m_nMatchs;
	}
	m_strText.Insert(m_nCur,m_list[m_nFormats[m_nMatchIndex]]);
	m_nCur+=(XU16)m_list[m_nFormats[m_nMatchIndex]].GetLength();
	m_nMatchs=0;

	PostMsg(XWM_COMMAND,XCMD_EDIT_CHANGE,(XU32)this,XMS_PARENT);
}



XBOOL XEdit::IsPopup()
{
    return (!(m_nEditStyle&XEDS_MULTILINE))&&m_nFormats.GetSize()>0;
}

void XEdit::PaintList(XGraphics &g)
{
	  XRect rect;
	  XColor ccw=GetSysColor(XCW_BACK);
	  ccw.Bright(30);
	  g.SetBackColor(ccw);
	  GetListRect(rect);
	  g.FillRectangle(rect);

	  rect.InflateRect(-1,-1);
	  int by=rect.top;
	  for(XU16 i=0;i<m_nMaxList;i++)
	  {
		if(m_nMatchOffset+i>=(XINT)m_nFormats.GetSize())
			continue;
		if(m_nMatchOffset+i==m_nMatchIndex)
		{
		  g.SetBackColor(GetSysColor(XCW_SELECTBACK));
		  g.SetColor(GetSysColor(XCW_SELECTTEXT));
		  g.FillRectangle(XRect(rect.left,by,rect.right,by+m_nTextHeight-2));
		}
		else
		{
		  g.SetColor(GetSysColor(XCW_TEXT));
		}
		int id=m_nFormats[m_nMatchOffset+i];
		g.DrawString(m_list[id],rect.left+2,by);
		by+=m_nTextHeight;
	  }
	  g.SetColor(GetSysColor(XCW_TEXT));
	  g.FrameRectangle(rect);
}

int XEdit::TickList(XU8&index)
{
	if(m_nFormats.GetSize()<=m_nMaxList)
		return 0;
	if(index==(XU8)-1)
		return 0;
	if(index<=m_nMatchOffset)
	{
		if(index>0)
		{
			m_nMatchOffset=index-1;
			index--;
			return -1;
		}
	}
	else if(index>=m_nMatchOffset+m_nMaxList-1)
	{
		if(index+1<(XU8)m_nFormats.GetSize())
		{
			m_nMatchOffset=index-m_nMaxList+2;
			index++;
			return 1;
		}
	}
	return 0;
}

void XEdit::PaintEdit(XGraphics &g)
{
	XColor cw(GetSysColor(XCW_BACK));
    cw.Bright(m_nStyle&XES_SETFOCUS?30:5);
    g.SetBackColor(cw);

    XRect rect;
    GetClientRect(rect);

    g.FillRectangle(rect);
    XColor ct(GetSysColor(XCW_TEXT));
    g.SetColor(ct);

    PaintText(g);
    PaintCursor(g);

    cw=GetSysColor(XCW_TEXT);
	cw.Bright(20);
    g.SetColor(cw);
    g.FrameRectangle(rect);
}

