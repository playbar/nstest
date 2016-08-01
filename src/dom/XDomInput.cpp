// XDomInput.cpp: implementation of the XDomInput class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomInput.h"
#include "XEnumData.h"
#include "XDomView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static XPCTSTR _strUrlLikes="http://;ftp://;https://;/;.;:;?;wap.;www.;.com;.cn;.net;.edu;.asp;.wml;";

XU32 XDomInput::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	if(!IsVisiable()) return 0;
	XVar*p=(XVar*)pData1;
	switch(nOpera)
	{
	// case XDO_GET_STYLE:
	// case XDO_SET_FINAL:
	//	  break;
	 case XDO_ADD_CHILD:return 0;
	 case XDO_SELECTION:
	      return XDomItem::Selection((XSelection*)pData1);
	 case XDO_SELECT:
	      return (XU32)XDomItem::SelectItem((SELECTMARK*)pData1);
	 case XDO_CONNECT:
		  if(m_pEdit!=XNULL)
		  {
			  PostVariable((XEVENTDATA*)pData1,XString8(m_pEdit->GetText()));
			  return XRSETVAR;
		  }
		  else return XRNONE;
	 case XDO_SETINPUT:
		  if(m_pEdit!=XNULL)
		  {
			  m_pEdit->SetText(XString16(FindAttrib(XEAB::VALUE,"")));
			  OnLoad((XEVENTDATA*)pData2,XFALSE);
			  return XRREPAINT;
		  }break;
	 case XDO_REFRESH:
		  //if(m_pEdit!=XNULL)
			 {
				XEVENTDATA*pData=(XEVENTDATA*)pData1;
				XPCTSTR strValue=RefreshVar((XDomNode*)pData->nData,XNULL,pData);
				if(strValue==XNULL) break;
				m_strTmp=strValue;
				if(m_pEdit!=XNULL)
				{
					//if(strTxt!=m_pEdit->GetText())
					//{
						m_pEdit->SetText(m_strTmp);
					//}
					m_strTmp.Empty();
					return XRREPAINT;
				}
			 }break;
	 case XDO_GET_TEXT:
		  if(m_pEdit!=XNULL)
			*((XString16*)pData1)=m_pEdit->GetText();
		  break;
	 case XDO_EVENT_ONBLUR:
		  //m_nOff=0;
		  if(m_pEdit!=XNULL)
		  {
			  m_pEdit->SetFocus(XFALSE);
			  if(m_pEdit->GetEditStyle()&XEDS_MULTILINE)
				  return ChangeMode((XEVENTDATA*)pData1)&~XRNEXT;
			  return SetRepaint((XEVENTDATA*)pData1,m_pEdit->GetWindowRect());
		  }break;
	 /*case XDO_EVENT_ONMOUSEDOWN:
	 case XDO_EVENT_ONMOUSEUP:
	 case XDO_EVENT_ONMOUSEMOVE:
		 {
			XRect rold=m_edit.GetWindowRect();
			m_edit.MouseMove(*(XPoint*)pData1);
			if(m_edit.IsUpdate())
			{				
				SetRepaint((XEVENTDATA*)pData2,rold);
				return XRREPAINT;
			}

		 }break;*/
	 case XDO_EVENT_ONMOUSEDOWN:
	 case XDO_EVENT_ONMOUSEUP:
	 case XDO_EVENT_ONMOUSEMOVE:
	 case XDO_EVENT_ONCLICK:
		  if(m_pEdit!=XNULL)
			 {
			  XRect rold=m_pEdit->GetWindowRect();
			  XU32 s=XRNONE;
			  
			  switch(nOpera)
			  {
			  case XDO_EVENT_ONCLICK:
				  {
					XPoint*pt=(XPoint*)pData1;
					XPoint ptt(*pt);
					m_pEdit->Click(*pt,0);
					XEVENTDATA*pEvent=(XEVENTDATA*)pData2;
					s|=OnLoad(pEvent,XTRUE);
					if(ptt.x<0)
					{
						
						m_pEdit->SetFocus(XFALSE);
						if(m_pEdit->GetEditStyle()&XEDS_MULTILINE)
							s|=ChangeMode((XEVENTDATA*)pData2)&~XRNEXT;
						s|=SetRepaint((XEVENTDATA*)pData2,m_pEdit->GetWindowRect());
		 
					}

				  }	
					break;
			   case XDO_EVENT_ONMOUSEUP:
				   {
					m_pEdit->MouseUp(*(XPoint*)pData1,0);
#ifdef _FULLEDIT
					XEVENTDATA*pEvent=(XEVENTDATA*)pData2;
					if(pEvent->pView)
					{
						//m_pEdit->SelectAll();
						m_pEdit->SetFocus(XFALSE);
						m_pEdit->SetFocus(XTRUE);
						pEvent->pView->PostMsg(XWM_STARTEDIT,m_pEdit->GetEditStyle()|(m_pEdit->GetMaxLength()<<16),
											   (XU32)m_pEdit->GetText(),XMS_TOPMOST);
					}
#endif
					break;
				   }
			   case XDO_EVENT_ONMOUSEDOWN:
				   {
					//XRect rect;
					XPoint pt=*(XPoint*)pData1;
					//pt-=m_pEdit->GetWindowRect().TopLeft();
					//m_pEdit->GetClientRect(rect);
					XRect rect=m_pEdit->GetWindowRect();

					if(rect.PtInRect(pt))
						s|=XRCINPUT;
					pt=*(XPoint*)pData1;
					pt-=m_pEdit->GetWindowRect().TopLeft();
					m_pEdit->OnMouseDown(pt,0);
				   }break;
				case XDO_EVENT_ONMOUSEMOVE:
					m_pEdit->MouseMove(*(XPoint*)pData1,0);
					{
						XRect rect;
						XPoint pt=*(XPoint*)pData1;
						//pt-=m_pEdit->GetWindowRect().TopLeft();
						m_pEdit->GetClientRect(rect);
						if(rect.PtInRect(pt))
							s|=XRCINPUT;
					}
					break;
			  }
			  if(m_pEdit->IsUpdate())
		  		  s|=SetRepaint((XEVENTDATA*)pData2,rold);
			  return s;
			  
			 }
		  break;
	 case XDO_EVENT_ONFOCUS:
		  if(m_pEdit!=XNULL)
		  {
			m_pEdit->SetFocus(XTRUE);
			return SetRepaint((XEVENTDATA*)pData1,m_pEdit->GetWindowRect());
		  }break;
	 case XDO_EVENT_ONSTRING:
		  if(m_pEdit!=XNULL)
		  {
			  return ReplaceString((XPCWSTR)pData1,(XEVENTDATA*)pData2);
		  }
	 case XDO_EVENT_ONCHAR:
		  if(m_pEdit!=XNULL)
		  {
			return InsertChar((XWCHAR)pData1,(XEVENTDATA*)pData2);
		  }break;
	 case XDO_EVENT_ONKEYDOWN:
		  return OnKeyDown((XWCHAR)pData1,(XEVENTDATA*)pData2);
	 case XDO_SET_FINAL:
		  return OnLoad((XEVENTDATA*)pData2,XFALSE);
	 case XDO_EVENT_ONTIMER:
		  return OnTimer((XEVENTDATA*)pData1);
	 case XDO_GET_RECT:
		  GetRoundRect(*(XRect*)pData1);
		 return XTRUE;
	 case XDO_HITTEST:
		  if(HitTest(pData1,pData2)) return (XU32)this;
		  return XNULL;
	 case XDO_PAINT:
		  return Paint((DRAWCONTEXT*)pData1);
/*	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		 switch(p->nID)
		 {
		 //case XEAB::CHECKED:
		 case XEAB::EMPTYOK:
			  p->ToLogic();
			  break;
		 case XEAB::MAXLENGTH:			  
			  p->ToInt();
			  //m_edit.SetMaxLength((XU16)p->iData);
			  break;
		 case XEAB::VALUE:
			  //m_strValue=p->ToString();
			  //m_nCur=(XU16)m_strValue.GetLength();
			  //SetText(XString16(p->ToString()));
			  break;
		 case XEAB::TABINDEX:
		 case XEAB::SIZE:
			  p->ToInt();
			  break;
		 case XEAB::TYPE:
			  if(p->nType==VSTRING)
			  {
				  XEnumInputType e;
				  *p=e.Index(p->strData);//(p->strData[0]=='P'||p->strData[0]=='p')?PASSWORD:TEXT;
			  }

			  break;
		 case XEAB::FORMAT:
			  if(p->nType==VSTRING)
			  {
				  char c=p->strData[0];
				  for(XU32 i=1;i<strlen(p->strData);i++)
				  {
					  if(c<'0'||c>'9') break;
					  c=p->strData[i];
				  }
				  XU8 nType=0;//m_edit.GetEditStyle()&~XEDS_ANY;
				  switch(c)
				  {
				  case 'A':nType|=XEDS_UCHAR|XEDS_SIGN;break;
				  case 'a':nType|=XEDS_LCHAR|XEDS_SIGN;break;
				  case 'N':nType|=XEDS_NUMBER;break;
				  case 'X':nType|=XEDS_UCHAR|XEDS_WCHAR;break;
				  case 'x':nType|=XEDS_LCHAR|XEDS_WCHAR;break;
				  default:
				  case 'M':
				  case 'm':nType=XEDS_ANY;break;
				  }
				  *p=nType;
				  //m_edit.SetEditStyle(nType);
			  }
			  //else m_edit.SetEditStyle((XU8)p->iData|m_edit.GetEditStyle());
			  //else m_nType=(XU8)p->iData;
			  break;
		 }break;*/
	  case XDO_GET_STYLE:
		  if(m_pEdit!=XNULL)
		  {
			  if(m_pEdit->GetEditStyle()&XEDS_MULTILINE)
				return DOM_SELECTABLE|DOM_INPUT|DOM_PAINT|DOM_EDITING;
			  else
			    return DOM_SELECTABLE|DOM_INPUT|DOM_PAINT;
		  }
		  else return DOM_SELECTABLE|DOM_PAINT;
		  break;
	  case XDO_LAYEROUT_CELLS:
		  Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XTRUE);
		  //return XTRUE;
		  break;
	  case XDO_LAYEROUT_CELL:
		  Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
		  break;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

void XDomInput::Layerout(DRAWCONTEXT *pDraw, CELLDATA *pData,XBOOL bCells)
{
   XSIZE sz;
   LAYEROUTDATA margin;
   XU8 ah=pDraw->ALIGNH;
   pDraw->ALIGNH=XEA::MIDDLE;
   PreLayerout(pDraw,pData,&margin);
   GetSize(pDraw,pData,sz);
   //if(pData->cur.x+sz.cx>pDraw->win.right&&pData->IsWrap()&&!pData->bReturn)
   if(bCells)
		m_bReturn=NewRowIfNeed(pDraw,pData,sz.cx);
   else if(m_bReturn)
	   NewRow(0,pDraw,pData,0);
   
   XRect r;
   r.left=pData->cur.x+2;
   r.top=AlignCol(pDraw,pData,sz.cy-2,pDraw->ALIGNH);
   SetRowSize(pDraw,pData,sz,sz.cx,bCells);   
   r.right=r.left+sz.cx-4;
   r.bottom=r.top+sz.cy-2;
   //r.bottom = 50;
   //if(m_pEdit!=XNULL) //!bCells)
   {
	   if(m_pEdit==XNULL)//!m_edit.IsValid())
	   {

		   //XU8 ns=XES_VISIABLE;//m_edit.GetStyle()|XES_VISIABLE;
		   XU8 nes=FindAttrib(XEAB::FORMAT,XEDS_ANY);//m_edit.GetEditStyle();
		   XU8 type=FindAttrib(XEAB::TYPE,XEIT::TEXT);
		   if(type==XEIT::PASSWORD) 
			   nes|=XEDS_PASSWORD;
		   m_pEdit=new XEdit();
		   m_pEdit->Create(r,XNULL,pDraw->pContext?pDraw->pContext:pDraw->pDraw,nes);
		   //m_pEdit->OnBlur();
		   //m_edit.SetStyle(ns);
		   m_pEdit->SetFocus(XFALSE);
		   if(m_strTmp.GetLength()>0)
		   {
			   m_pEdit->SetText(m_strTmp);
			   m_strTmp.Empty();
			   m_strTmp.FreeExtra();
		   }
		   else
		   {
			   XPCTSTR pStr=FindAttrib(XEAB::VALUE,(XPCTSTR)XNULL);
			   if(pStr!=XNULL)
			      m_pEdit->SetText(XString16(pStr));
		   }
		   if(type==XEIT::URL) 
		   {
			   XString8 urlLikes(_strUrlLikes);
			   m_pEdit->SetLikeText(urlLikes);
		   }
	   }
	   else
	   {
		   m_pEdit->MoveWindow(r);
	   }
   }
   EndLayerout(pDraw,pData);
   pDraw->ALIGNH=ah;
}

void XDomInput::GetSize(DRAWCONTEXT *pDraw,CELLDATA*pData,XSIZE&sz)
{  
	 int w=pDraw->SETWIDTH;
	 int h = pDraw->SETHEIGHT;
	 if ( h <= 0 )
	 {
		 h = pDraw->SPACE+4;
	 }
	 sz.cx=FindAttrib(XEAB::SIZE,20);//*(pDraw->SPACE-4)+2;
	 sz.cy = h;
	 //XINT nMax=FindAttrib(XEAB::
	 int ww=XMIN(pDraw->win.Width(),pDraw->view.Width());
	 
	 if(sz.cx==0)//&&m_pEdit!=XNULL) 
	 {
	   sz.cx=XMIN((GetWinRight(pDraw,pData)-pData->cur.x-2),ww);
	   if(sz.cx<pDraw->SPACE<<1) sz.cx=pDraw->SPACE<<1;
	 }
     else 
	 {	   
	   sz.cx=XMIN(XMAX(sz.cx*(pDraw->SPACE>>1)+2,pDraw->SPACE),ww-2);
	   sz.cx=XMAX(sz.cx,(pDraw->SPACE+4));
	 }
	 if(w>0)
		 sz.cx=w;
}

XBOOL XDomInput::Paint(DRAWCONTEXT *pDraw)
{
	
	XRect rect;
	GetRoundRect(rect);

	if(!rect.IsUnion(pDraw->paint)||m_pEdit==XNULL)
		return XFALSE;
	//rect=m_pEdit->GetWindowRect();
	//m_pEdit->GetClientRect(rect);
	//rect+=m_pEdit->GetWindowRect().TopLeft();
	rect.InflateRect(1,1);
	PaintSelect(rect,pDraw);

	if(pDraw->pSelect==this)
		m_pEdit->SetFocus(XTRUE);
	else
		m_pEdit->SetFocus(XFALSE);


	m_pEdit->Invalidate();
	XRect &r=m_pEdit->GetWindowRect();
	if(r.bottom-pDraw->offset.y>pDraw->win.bottom)
		m_pEdit->SetUpList();
	else if(r.top-pDraw->offset.y<pDraw->win.top)
		m_pEdit->SetDownList();
	pDraw->pView->AddPopup(m_pEdit);
//	m_edit.SetParent(pDraw->pView);
//	m_edit.Paint(*pDraw->pDraw);


	return XTRUE;
}

XBOOL XDomInput::HitTest(XINT x, XINT y)
{
	XRect rect;//(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
	GetRoundRect(rect);
	return rect.PtInRect(x,y);
}

void XDomInput::GetRoundRect(XRect &rect)
{
	if(m_pEdit!=XNULL)
	{
		rect=m_pEdit->GetWindowRect();
		rect.InflateRect(1,1);
	}
	//rect.left=m_nPosX;
	//rect.right=m_nPosX+m_nWidth;
	//rect.top=m_nPosY;
	//rect.bottom=m_nPosY+m_nHeight;
}

//DEL void XDomInput::PaintText(DRAWCONTEXT &draw, const XRect &rect)
//DEL {    
//DEL 	XSIZE sz={0,0};
//DEL 	XString8 str;
//DEL 	str.FromInt(m_nClick);
//DEL 	XString16 str16(str);
//DEL 	if(m_nCur>m_nOff)
//DEL 		draw.pDraw->MeasureString((XPCWSTR)m_strValue+m_nOff,sz,m_nCur-m_nOff);
//DEL 	draw.pDraw->DrawString((XPCWSTR)m_strValue+m_nOff,rect);
//DEL 	//draw.DrawString(str16,rect);
//DEL 	if(m_nClick>5&&draw.pSelect==this&&rect.left+sz.cx<rect.right)
//DEL 	{
//DEL 		draw.pDraw->DrawLine(rect.left+sz.cx+1,rect.top-1,rect.left+sz.cx+1,rect.bottom-1);
//DEL 		//draw.draw.DrawLine(rect.left+sz.cx+2,rect.top-1,rect.left+sz.cx+2,rect.bottom-1);
//DEL 	}
//DEL }

XU32 XDomInput::OnTimer(XEVENTDATA*pData)
{
	if(m_pEdit==XNULL) return XRNONE;
	XRect rt;
	GetRoundRect(rt);
	if(!rt.IsUnion(pData->viewRect)) return XFALSE;
	m_pEdit->OnTimer();
	if(m_pEdit->IsUpdate())
	{
		pData->rect|=rt;
		return XRREPAINT;
	}
	return XRNONE;
/*	if(m_nClick==0||m_nClick==6)
	{
		pData->rect|=rt;
		return XRREPAINT;
	}
	return XRNONE;*/
}

XU32 XDomInput::OnLoad(XEVENTDATA *pData,XBOOL bFromEdit)
{
	if(bFromEdit&&m_pEdit==XNULL) return XRNONE;
	XPCTSTR strHeader=FindAttrib(XEAB::NAME,(XPCTSTR)XNULL);
	XString8 strValue;
	if(bFromEdit) strValue=m_pEdit->GetText();
	else strValue=FindAttrib(XEAB::VALUE,"");
	
	if(strHeader!=XNULL)
	{
		if(XSortString8::SSetValue(pData->strValues,strHeader,strValue)&&bFromEdit)
		{
			pData->nData=(XU32)this;
			return XRVARCHANGE;
		}
		
	}
	return XRNONE;
}

XU32 XDomInput::OnKeyDown(XWCHAR c, XEVENTDATA *pData)
{
    if(m_pEdit==XNULL) return XRNONE;
	XU32 s=XRNONE;
	XRect rold=m_pEdit->GetWindowRect();
	XKEYMSG msg={c,c,0,0,0,0};
	if(m_pEdit->OnKeyDown(msg))
	{
	  s|=OnLoad(pData,XTRUE);
	  SetRepaint(pData,rold);
	  s|=XRREPAINT;
	}
	else if(c==XK_RETURN)
	{
		s|=ChangeMode(pData);
	}
    return s;
}

XU32 XDomInput::SetRepaint(XEVENTDATA *pData,XRect&rt)
{
	if(m_pEdit==XNULL||
	   m_pEdit->GetClipRect().IsRectEmpty()) return XRNONE;
    XRect rect=m_pEdit->GetWindowRect();
	rect|=rt;
	rect.InflateRect(2,2);
	//GetRoundRect(rect);
	pData->rect|=rect;
	return XRREPAINT;
}

XU32 XDomInput::ReplaceString(XPCWSTR buf,XEVENTDATA*pData)
{
	if(m_pEdit==XNULL) return XRNONE;
	XRect rold=m_pEdit->GetWindowRect();
	//if(!m_pEdit->OnChar(c)) return XRNONE;
	m_pEdit->SetText(buf);
	XU32 s=OnLoad(pData,XTRUE);
	s|=SetRepaint(pData,rold);
	return s|XRREPAINT;
}

XU32 XDomInput::InsertChar(XWCHAR c,XEVENTDATA*pData)
{
	if(m_pEdit==XNULL) return XRNONE;
	XRect rold=m_pEdit->GetWindowRect();
	if(!m_pEdit->OnChar(c)) return XRNONE;
	XU32 s=OnLoad(pData,XTRUE);
	s|=SetRepaint(pData,rold);
	return s|XRREPAINT;
}

//DEL void XDomInput::SetText(XPCWSTR strText)
//DEL {
//DEL 	m_edit.SetText(strText);
//DEL }

void XDomInput::SetText(XPCTSTR strText)
{
	XVar var(XEAB::VALUE,strText);
	SetAttrib(var);
	//m_edit.SetText(XString16(strText));
}

XU32 XDomInput::ChangeMode(XEVENTDATA*pData)
{
	/*if(m_pEdit==XNULL) return XRNONE;
	XU8 s=m_pEdit->GetEditStyle();
	if(s&XEDS_PASSWORD) return XRNEXT;
	if(s&XEDS_MULTILINE)
	{
		m_pEdit->SetEditStyle(s&~XEDS_MULTILINE);
		return XRLAYEROUT|XRNEXT;
	}
	else if(FindAttrib(XEAB::SIZE,20)>20&&FindAttrib(XEAB::MAXLENGTH,100)>20)
	{
		XU32 rs=XRLAYEROUT;
		m_pEdit->SetEditStyle(s|XEDS_MULTILINE);
		XRect&r=m_pEdit->GetWindowRect();
		int dLine=(pData->viewRect.Height()>>1)/XWindow::m_nTextHeight;
		dLine=XMAX(XMIN(dLine,4),8);
		r.bottom=r.top+dLine*XWindow::m_nTextHeight+2;
		r.left=pData->viewRect.left+2;
		r.right=pData->viewRect.right-2;

		if(r.bottom>pData->viewRect.bottom)
		{
			pData->offset.y+=(r.bottom-pData->viewRect.bottom)+XWindow::m_nTextHeight;
			rs|=XROFFSET;
		}
		return rs;
	}
	else */
	return XRNEXT;
}
