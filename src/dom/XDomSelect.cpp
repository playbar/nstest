// XDomSelect.cpp: implementation of the XDomSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomSelect.h"
#include "XEnumData.h"
#include "XDomView.h"
#include "XDomOption.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomSelect::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
//	if(!IsVisiable()) return 0;
	XVar*p=(XVar*)pData1;
	switch(nOpera)
	{
	case XDO_CONNECT:
		 //if(m_pEdit==XNULL)
			 Connect((XEVENTDATA*)pData1);
		 break;
	case XDO_SETINPUT:
		{
			XEVENTDATA*pData=(XEVENTDATA*)pData2;
			m_strSelect=FindAttrib(XEAB::VALUE,"");
			OnLoad(pData);
			XRect r;
			GetRoundRect(r);
			pData->rect|=r;
			return XRREPAINT;
		}
		 break;
	case XDO_EVENT_ONKEYDOWN:
		 if(!IsVisiable()) return 0;
		 if(m_pList!=XNULL)
		 {
			 XEVENTDATA*pData=(XEVENTDATA*)pData2;
			 //if(pData1==XK_DOWN) break;
			 XU32 s=XRNONE;
			 XRect rect=m_pList->GetWindowRect();
			 int ns=m_pList->GetSelect();
			 XKEYMSG msg={pData1,pData1,0,0,0,0};
			 if(m_pList->OnKeyDown(msg))
			 {
			   if(ns!=m_pList->GetSelect())
			   {
				   if(SetVarible(pData,XTRUE))
				   {
					   pData->nData=(XU32)this;
					   s|=XRVARCHANGE;
				   }
			   }
			   s|=SetRepaint(pData,rect);
			 }
			 
			 return s;
		 }break;
	//case XDO_EVENT_ONCLICK:
	//	 return OnClick(*(XPoint*)pData1,(XEVENTDATA*)pData2);
	case XDO_EVENT_ONTIMER:
		 if(!IsVisiable()) return 0;
		 if(m_pList!=XNULL)
		 {
			XRect rect=m_pList->GetWindowRect();
			switch(nOpera)
			{
			case XDO_EVENT_ONTIMER:
				 m_pList->OnTimer();
				 break;
			}
			return SetRepaint((XEVENTDATA*)pData1,rect);
		 }break;
	case XDO_EVENT_ONMOUSEMOVE:
	case XDO_EVENT_ONMOUSEDOWN:
		 if(!IsVisiable()) return 0;
		 if(m_pList!=XNULL)
		 {
			 XPoint pt=*(XPoint*)pData1;
			 XRect rect=m_pList->GetWindowRect();
			 XU32 s=XRNONE;
			 XRect rc;
			 
			 pt-=rect.TopLeft();
			 XEVENTDATA*pData=(XEVENTDATA*)pData2;
			 int ns=m_pList->GetSelect();
			 switch(nOpera)
			 {
			 case XDO_EVENT_ONMOUSEDOWN:
				  m_pList->OnClick(pt,0);
				  if(ns==m_pList->GetSelect()) break;
				  if(SetVarible(pData,XTRUE))
				  {
					  s|=XRVARCHANGE;
					  pData->nData=(XU32)this;
				  }
				  break;
			 case XDO_EVENT_ONMOUSEMOVE:
				  m_pList->OnMouseMove(pt,0);
				  m_pList->GetClientRect(rc);
				  if(rc.PtInRect(pt)) s|=XRCANCHOR;
				  break;
			 }
			 return s|SetRepaint(pData,rect);
		 }
		 else if(nOpera==XDO_EVENT_ONCLICK)
			 OnClick(*(XPoint*)pData1,(XEVENTDATA*)pData2);
		 break;
	case XDO_EVENT_ONBLUR:
		 if(!IsVisiable()) return 0;
		 if(m_pList!=XNULL)
		 {
			 XRect rect=m_pList->GetWindowRect();
			 m_pList->OnBlur();
			 return SetRepaint((XEVENTDATA*)pData1,rect);
		 }
		 break;
	case XDO_EVENT_ONFOCUS:
		 if(!IsVisiable()) return 0;
		 if(m_pList!=XNULL)
		 {
			XRect rect=m_pList->GetWindowRect();
			m_pList->OnFocus();
			return SetRepaint((XEVENTDATA*)pData1,rect);
		 }
		 break;
	case XDO_REFRESH:
		{
			XEVENTDATA*pData=(XEVENTDATA*)pData1;
			XPCTSTR strValue=RefreshVar((XDomLeaf*)pData->nData,XNULL,
										pData);
			if(strValue==XNULL) break;
			if(FindAttrib(XEAB::INAME,(XPCTSTR)XNULL)==XNULL)
				m_strSelect=strValue;
			else
				IndexToSelect(XString8(strValue).ToInt(),m_strSelect);
			if(m_pList!=XNULL)
				m_pList->Select(IndexOf(m_strSelect));
			return XRREPAINT;
		}break;
	case XDO_SET_FINAL:
		{
			//XPCTSTR strValue=FindAttrib(XEAB::IVALUE,(XPCTSTR)XNULL);
			int id=FindAttrib(XEAB::IVALUE,0);
			//if(strValue!=XNULL)
			  IndexToSelect(id,m_strSelect);
			CreateList((DRAWCONTEXT*)pData1);
			//m_pSelect=GetSelect();
		}
		 SetMultiMark();
		 return OnLoad((XEVENTDATA*)pData2);
		 break;
	case XDO_SELECTION:
		 if(!IsVisiable()) return 0;
		  if(m_pList==XNULL) break;
	      return XDomItem::Selection((XSelection*)pData1);
	case XDO_SELECT:
		 if(!IsVisiable()) return 0;
		 if(m_pList!=XNULL)
			return (XU32)XDomItem::SelectItem((SELECTMARK*)pData1);
		 break;
	case XDO_HITTEST:
		 if(!IsVisiable()) return 0;
		 if(m_pList!=XNULL) 
		 {
			   XRect rect;
			   if(!GetRoundRect(rect)) return XNULL;
			   return (XU32)(rect.PtInRect(pData1,pData2)?this:XNULL);
		 }break;
//	case XDO_SET_FINAL:
//		 return OnLoad((XEVENTDATA*)pData1);
	case XDO_GET_STYLE:
		 return XDomNode::Handle(nOpera,pData1,pData2)|DOM_SELECTABLE|DOM_PAINT;
	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		 switch(p->nID)
		 {
		 /*case XEAB::MULTIPLE:
			  p->ToLogic();
			  break;
		 case XEAB::IVALUE:
			  break;*/
		 case XEAB::VALUE:		 
			  m_strSelect=p->ToString();
			  //p->ToInt();
			  //m_nIndex=(XU8)p->iData;
			  break;
		 }
		 break;
	case XDO_GET_ATTR:
		 switch(p->nID)
		 {
		 case XEAB::VALUE:
			  *p=m_strSelect;
			  break;
		 case XEAB::IVALUE:
			  *p=IndexOf(m_strSelect);
			  break;
		 }break;
	case XDO_PAINT:
		 //if(m_pList!=XNULL)
		 if(!IsVisiable()) return 0;
		 return Paint((DRAWCONTEXT*)pData1);
		 break;
    case XDO_GET_RECT:
		 if(!IsVisiable()) return 0;
		 if(m_pList!=XNULL)
		 {
			GetRoundRect(*(XRect*)pData1);
			return XTRUE;
		 }break;
	case XDO_LAYEROUT_CELLS:
		 if(!IsVisiable()) return 0;
		 if(m_pList!=XNULL)
		 {
			 LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XTRUE);
			 return XTRUE;
		 }break;
	case XDO_LAYEROUT_CELL:
		 if(!IsVisiable()) return 0;
		 if(m_pList!=XNULL)
		 {
			 LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
			 return XTRUE;
		 }
		 break;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

void XDomSelect::LayeroutCell(DRAWCONTEXT *pDraw, CELLDATA *pData,XBOOL bCells)
{
	/*if(m_pList==XNULL)
	{
		XDomNode		
		return;
	}*/
	//CELLDATA data;
	//data.Reset(m_nPosX,m_nPosY,XEA::LEFT,XEA::NOWRAP);
	XRect rt;
	m_pList->GetClientRect(rt);
	XSize sz=rt.Size();

	//if(!pData->bReturn&&pData->cur.x+sz.cx>pDraw->win.right&&pData->IsWrap())
	if(bCells)
		m_bReturn=NewRowIfNeed(pDraw,pData,sz.cx);
	else if(m_bReturn)
		NewRow(0,pDraw,pData,0);

	
	if(!bCells)
	{
		XSize szz=m_pList->GetWindowRect().Size();
		XRect rt(pData->cur.x+2,pData->cur.y+1,0,0);
		rt.right=rt.left+szz.cx;
		rt.bottom=rt.top+szz.cy;
		m_pList->MoveWindow(rt);
	}
	sz.cy+=2;
	sz.cx+=4;
	SetRowSize(pDraw,pData,sz,sz.cx,bCells);
}

//DEL void XDomSelect::CalcRect(DRAWCONTEXT *pDraw, CELLDATA *pData)
//DEL {
//DEL 	  
//DEL 	  m_nWidth=pDraw->SPACE;
//DEL 	  m_nHeight=pDraw->SPACE+2;
//DEL 	  	  
//DEL 	  for(XU32 i=0;i<m_childs.GetSize();i++)
//DEL 	  {
//DEL 		CELLDATA data;
//DEL 		data.Reset(pDraw->win.left,pDraw->win.top,pData->align,XEA::NOWRAP);
//DEL 		if(m_childs[i]->GetID()!=XDOM_OPTION) continue;
//DEL 		m_childs[i]->LayeroutItem(pDraw,&data,XTRUE);
//DEL 		data.max.cx+=4;
//DEL 		data.max.cy+=4;
//DEL 		if(m_nWidth<data.max.cx)
//DEL 			m_nWidth=data.max.cx;
//DEL 		if(m_nHeight<data.max.cy)
//DEL 			m_nHeight=data.max.cy;
//DEL 		//NewRow(0,pDraw,pData,XTRUE);
//DEL 	  }
//DEL 	  //m_nHeight=pDraw->SPACE+4;
//DEL 	  //m_nWidth=data.max.cx;
//DEL 	  m_nWidth+=m_nHeight;
//DEL 	  m_nWidth=XMIN(pDraw->win.right-pDraw->win.left,m_nWidth);
//DEL }

XBOOL XDomSelect::Paint(DRAWCONTEXT *pDraw)
{
	if(m_pList==XNULL)
	{
		XU32 nData=pDraw->DATA;
		pDraw->DATA=(XU32)&m_strSelect;
		XDomNode::Handle(XDO_PAINT,(XU32)pDraw,0);
		pDraw->DATA=nData;
		return XTRUE;
	}
	XRect rect;
	GetRoundRect(rect);

	if(!rect.IsUnion(pDraw->paint)) return XFALSE;
	
	m_pList->Invalidate();
	pDraw->pView->AddPopup(m_pList);

	if(pDraw->pActive==this||pDraw->pSelect==this)
	{
		XRect rt;
		m_pList->GetClientRect(rt);
		rt+=m_pList->GetWindowRect().TopLeft();
		XColor cc(pDraw->pActive==this?pDraw->DCACTIVE:pDraw->DCSELECTBACK);
		cc.Mix(XColor(pDraw->DCBACK),0.8f);
		XU32 oc=pDraw->pDraw->SetColor(cc);
		rt.InflateRect(1,1);
		pDraw->pDraw->FrameRectangle(rt);
		pDraw->pDraw->SetColor(oc);
	}

	//return XTRUE;
	//m_pList->Invalidate();
	if(!m_pList->IsPopup()) return XTRUE;
	XRect &r=m_pList->GetWindowRect();
	if(r.bottom-pDraw->offset.y>pDraw->view.bottom)
	{
		if(!(m_pList->GetEditStyle()&XEDS_UPLIST))
		{
			if(r.top-pDraw->offset.y>pDraw->view.bottom-r.top+pDraw->offset.y)
			{
				int nLines=(r.top-pDraw->offset.y-2)/pDraw->SPACE;
				m_pList->SetUpList();
				
				m_pList->SetLines(nLines);
			}
			else
			{
				int nLines=(pDraw->view.bottom-r.top-2)/pDraw->SPACE-1;
				m_pList->SetLines(nLines);
			}
		}
	}
	else if(r.top-pDraw->offset.y<pDraw->view.top)
	{
		if(m_pList->GetEditStyle()&XEDS_UPLIST)
		{
			if(r.bottom-pDraw->offset.y<pDraw->view.bottom-r.bottom+pDraw->offset.y)
			{
				int nLines=(pDraw->view.bottom-r.bottom+pDraw->offset.y-2)/pDraw->SPACE;
				m_pList->SetDownList();
				
				m_pList->SetLines(nLines);
			}
			else
			{
				int nLines=(r.bottom-pDraw->offset.y-2)/pDraw->SPACE-1;
				m_pList->SetLines(nLines);
			}
		}
	}

	//m_pList->Paint(*pDraw->pDraw);
	return XTRUE;
	/*XRect rect;
	GetRoundRect(rect);
	//rect-=pDraw->offset;
	rect.bottom--;rect.right--;
	rect.InflateRect(-1,-1);

	if(!rect.IsUnion(pDraw->paint)) return XFALSE;
	XColor color(pDraw->DCBACK);
	color.Dark(50);
	XColor bkColor(pDraw->DCBACK);
	bkColor.Bright(30);
	XU32 bc=pDraw->pDraw->SetBackColor(bkColor);
	pDraw->pDraw->FillRectangle(rect);

	rect.top+=2;
	//PaintText(*pDraw,rect);
	XDomItem*pItem=m_pSelect;//GetSelect();

	if(pItem!=XNULL)
	{
		CELLDATA data;
		data.Reset(m_nPosX+2,m_nPosY+2,XEA::LEFT,XEA::NOWRAP);
		pItem->LayeroutItem(pDraw,&data,XTRUE);
		data.Reset(m_nPosX+2,m_nPosY+2,XEA::LEFT,XEA::NOWRAP);
		pItem->LayeroutItem(pDraw,&data,XFALSE);
		pItem->Paint(pDraw);
	}
	rect.top-=2;

	XU32 tc=pDraw->pDraw->SetColor(color);
	pDraw->pDraw->FrameRectangle(rect);
	XU8 id=0;
	if(pDraw->pActive==this||pDraw->pSelect==this)
	{
		rect.InflateRect(1,1);
		color=pDraw->DCBACK;
		color.Dark(20);
		pDraw->pDraw->SetColor(color);
		pDraw->pDraw->FrameRectangle(rect);
		rect.InflateRect(-1,-1);
		id=1;
	}

	rect.DeflateRect(2,2);
	rect.left=rect.right-rect.Height();
	XColor cc(pDraw->DCBUTTON);
	if(id==0) {cc=pDraw->DCBACK;cc.Mix(XColor(bc),0.5f);}
	pDraw->pDraw->DrawButton(rect,cc,XBS_BFRAME);
	cc.Dark(60);
	pDraw->pDraw->DrawDirect(rect,XK_DOWN,cc,id==0);
	XImage*pImage=&pDraw->images[XICON_BUTTON_DOWN];
	if(pImage!=XNULL&&!pImage->IsEmpty())
	{
		rect.right-=2;
		rect.bottom-=2;
		rect.top+=2;
		rect.left=rect.right-rect.Height();
		pImage->SetActive(id);
		pDraw->pDraw->DrawImage(*pImage,rect);
	}

	pDraw->pDraw->SetBackColor(bc);
	pDraw->pDraw->SetColor(tc);

	return XTRUE;*/

}

XBOOL XDomSelect::GetRoundRect(XRect &rect)
{
	rect=m_pList->GetWindowRect();
	return XTRUE;
}

/*XDomItem* XDomSelect::FindSelect()
{
	XString8 strValue=FindAttrib(XEAB::VALUE,"");
	int      nValue=FindAttrib(XEAB::IVALUE,-1);
	int id=0;
	for(XU32 i=0;i<m_childs.GetSize();i++)
	{
		if(m_childs[i]->GetID()!=XDOM_OPTION) continue;
		if(id==nValue) return m_childs[i];
		id++;
		XVar var(XEAB::VALUE,0);
		if(!m_childs[i]->GetAttrib(var)) continue;
		if(var.nType==VSTRING&&strValue.Compare(var.strData,XTRUE)==0)
			return m_childs[i];
	}
	return XNULL;
}*/

//DEL XDomItem* XDomSelect::GetNext()
//DEL {
//DEL    //for(XU32 i=0;i<
//DEL 	return XNULL;
//DEL }

//DEL XDomItem* XDomSelect::GetPrev()
//DEL {
//DEL 	return XNULL;
//DEL }

XBOOL XDomSelect::OnLoad(XEVENTDATA *pData)
{
	
	if(SetVarible(pData,XFALSE))
		return XRSETVAR;
	/*XPCTSTR strName=FindAttrib(XEAB::NAME,(XPCTSTR)XNULL);
	XPCTSTR strIName=FindAttrib(XEAB::INAME,(XPCTSTR)XNULL);
	
	int		iValue=FindAttrib(XEAB::IVALUE,0);	
	if(strHeader!=XNULL)
	{
		XEnum::SetValue(pData->strValues,strHeader,strValue);
		return XRSETVAR;
	}*/
	return XRNONE;
}

int XDomSelect::IndexOf(XPCTSTR str)
{
	XVar v(XEAB::VALUE,str);
	XU32 nIndex=0;
	if(FindByAttrib(v,nIndex,XDOM_OPTION)) return nIndex;
	/*XString8 strValue(str);
	int      nValue=FindAttrib(XEAB::IVALUE,-1);
	int id=0;
	for(XU32 i=0;i<m_childs.GetSize();i++)
	{
		if(m_childs[i]->GetID()!=XDOM_OPTION) continue;
		id++;
		XVar var(XEAB::VALUE,0);
		if(!m_childs[i]->GetAttrib(var)) continue;
		if(var.nType==VSTRING&&strValue.Compare(var.strData,XTRUE)==0)
			return id-1;
	} */
	return 0;
}

//DEL XDomItem* XDomSelect::GetSelect()
//DEL {
//DEL     /*int id=0;
//DEL 	for(XU32 i=0;i<m_childs.GetSize();i++)
//DEL 	{
//DEL 		if(m_childs[i]->GetID()!=XDOM_OPTION) continue;
//DEL 		if(id==m_nIndex) return m_childs[i];
//DEL 		id++;		
//DEL 	} */
//DEL 	XU32 id=0;
//DEL 	return FindByIndex(m_nIndex,id,XDOM_OPTION);
//DEL 	//return XNULL;
//DEL }

XBOOL XDomSelect::SetVarible(XEVENTDATA*pData,XBOOL bFromList)
{
	if(bFromList) pData->nData=(XU32)this;

	if(bFromList&&m_pList!=XNULL)
		IndexToSelect(m_pList->GetSelect(),m_strSelect);
		//m_strSelect.FromInt(m_pList->GetSelect());
    XPCTSTR strName=FindAttrib(XEAB::NAME,(XPCTSTR)XNULL);
	XPCTSTR strIName=FindAttrib(XEAB::INAME,(XPCTSTR)XNULL);
	if(strName!=XNULL)
	{
  	   if(XSortString8::SSetValue(pData->strValues,strName,m_strSelect))
		 return XTRUE;
	}
	else if(strIName!=XNULL)
	{
		int id=IndexOf(m_strSelect);
		XString8 str;
		str.FromInt(id);
		{
		   if(XSortString8::SSetValue(pData->strValues,strIName,str))
			return XTRUE;
		}
	}
	return XFALSE;
}

XU8 XDomSelect::IsListMode()
{
	//return XFALSE;
	XBOOL bMulti=FindAttrib(XEAB::MULTIPLE,XFALSE);
	if(bMulti) 
		return XFALSE;
	XDomList list;
	GetList(list,XDOM_OPTGROUP);
	GetList(list,XDOM_FIELDSET);
	return list.GetSize()==0;
	//multiple
}

void XDomSelect::CreateList(DRAWCONTEXT*pDraw)
{
   if(!IsListMode()||m_pList!=XNULL) return;
   XDomList list;
   GetList(list,XDOM_OPTION);
   XSize size(pDraw->SPACE<<1,pDraw->SPACE+2);
   XU8 nc=XMIN(list.GetSize(),0xf0);
   XU8 nLines=pDraw->win.Height()/pDraw->SPACE-2;
   if(nLines<4) nLines=4;
   if(nLines>nc) nLines=nc;
   //if(nLines<4) nLines=4;
   m_pList=new XListBox;
   XRect rect(0,0,size.cx,size.cy+nLines*pDraw->SPACE+2);
   m_pList->Create(rect,XNULL,pDraw->pDraw);
   
   for(XU8 i=0;i<nc;i++)
   {
	  XString16 str;
	  list[i]->GetText(str);
	  XSize sz;
	  pDraw->pDraw->MeasureString(str,sz);
	  sz.cx+=(pDraw->SPACE+8);
	  if(size.cx<sz.cx)
		  size.cx=sz.cx;
	  if(size.cy<sz.cy)
		  size.cy=sz.cy;
	  m_pList->AddString(str);
   }
   m_pList->Select(IndexOf(m_strSelect));
   XRect moveRect(0,0,size.cx,size.cy);
   m_pList->MoveWindow(moveRect);
}

XU32 XDomSelect::SetRepaint(XEVENTDATA *pData, XRect &rect)
{
	if(m_pList==XNULL)
	{
		GetRoundRect(rect);
		pData->rect|=rect;
	}
	else
	{
		if(m_pList->GetClipRect().IsRectEmpty()) return XRNONE;
		rect|=m_pList->GetWindowRect();
		rect.InflateRect(2,2);
		pData->rect|=rect;
	}
	return XRREPAINT;
}

void XDomSelect::IndexToSelect(XUINT nIndex, XString8 &strSelect)
{
	XU32 id=0;	
	XDomItem*p=FindByIndex(nIndex,id,XDOM_OPTION);
	if(p==XNULL) return;
	XVar v(XEAB::VALUE,"");
	if(!p->GetAttrib(v)) return;
	strSelect=v.ToString();
}

void XDomSelect::SetMultiMark()
{
	XBOOL bMulti=FindAttrib(XEAB::MULTIPLE,XFALSE);
	XDomList list;
	GetList(list,XDOM_OPTION);
	for(XU32 i=0;i<list.GetSize();i++)
		((XDomOption*)list[i])->SetCheckBox(bMulti);
}

//DEL void XDomSelect::LayeroutCells(DRAWCONTEXT *pDraw, CELLDATA *pData)
//DEL {
//DEL 
//DEL }

//DEL XU32 XDomSelect::OnClick(XPoint &pt, XEVENTDATA *pData)
//DEL {
//DEL 	XU32 s=XRNONE;
//DEL 
//DEL 	return s;
//DEL }

XU32 XDomSelect::Select(XPCTSTR strValue, XEVENTDATA *pData,XDomItem*pItem)
{
	XU32 s=XRNONE;
	XBOOL bMulti=FindAttrib(XEAB::MULTIPLE,XFALSE);
	if(!bMulti)
	{
		if(m_strSelect==strValue) return s;
		m_strSelect=strValue;
		XVar v(XEAB::VALUE,m_strSelect);
		XU32 nIndex=0;
		XDomItem*p=FindByAttrib(v,nIndex,XDOM_OPTION);
		if(p!=XNULL)
		{
			XRect rect;
			p->GetRoundRect(rect);
			pData->rect|=rect;
			s|=XRREPAINT;
		}
		s|=XRSETVAR;
	}
	else
	{
		XString8Array list;
		XString8 sv(strValue);
		list.SplitString(m_strSelect,';');
		int id=list.Find(sv);
		if(id<0) list.Add(sv);
		else	 list.RemoveAt(id);
		m_strSelect.Empty();
		for(XU32 i=0;i<list.GetSize();i++)
		{
			if(i>0) m_strSelect+=';';
			m_strSelect+=list[i];
		}
		s|=XRREPAINT;
		s|=XRSETVAR;
	}
	if(s)
	{
		SetVarible(pData,XFALSE);
		if(pItem!=XNULL)
		{
			s|=XRREPAINT;
			XRect rect;
			pItem->GetRoundRect(rect);
			pData->rect|=rect;
		}
		pData->nData=(XU32)this;
		//s|=XRSETVAR;
	}
	return s;
}

XU32 XDomSelect::Connect(XEVENTDATA *pData)
{

    XPCTSTR strName=FindAttrib(XEAB::NAME,(XPCTSTR)XNULL);
	XPCTSTR strIName=FindAttrib(XEAB::INAME,(XPCTSTR)XNULL);
	if(strName!=XNULL)
	{
		return PostVariable(pData,m_strSelect);
  	   //if(XSortString8::SSetValue(pData->strValues,strName,m_strSelect))
	  //return XTRUE;
	}
	else if(strIName!=XNULL)
	{
		int id=IndexOf(m_strSelect);
		XString8 str;
		str.FromInt(id);
		return PostVariable(pData,str);
		/*{
		   if(XSortString8::SSetValue(pData->strValues,strIName,str))
			return XTRUE;
		} */
	}
	return 0;
	//return XFALSE;



    //return PostVariable(pData);
}
