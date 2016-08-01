// XHTMLInput.cpp: implementation of the XHTMLInput class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLInput.h"
#include "XDom.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XHTMLInput::XHTMLInput(XU16 id):XDomInput(id)
{
//    m_pButton=XNULL;
	m_nWidth=0;
	m_nHeight=0;
	m_nStatus=0;
	m_nPosX=0;
	m_nPosY=0;
	m_pImage=XNULL;
	m_nTicks=0;
	m_nIndex=0;
	m_bFinal=0;
	m_bReturn=XFALSE;
}

XHTMLInput::~XHTMLInput()
{
//	if(m_pButton!=XNULL) delete m_pButton;
//	m_pButton=XNULL;
}

XU32 XHTMLInput::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
//	if(!IsVisiable()) return 0;
	switch(nOpera)
	{
	case XDO_ADD_CHILD:
		 {
			 XU8 t=FindAttrib(XEAB::TYPE,XEIT::TEXT);
			 if(t==XEIT::RADIO)
				return XDomNode::Handle(nOpera,pData1,pData2);
		 }break;
	case XDO_EVENT_ONTIMER:
		 if(!IsVisiable()) return 0;
		 return OnTimer((XEVENTDATA*)pData1);
	case XDO_SETINPUT:
		 if(!IsVisiable()) return 0;
		 if(m_pEdit==XNULL)
		 {
			 XEVENTDATA*pData=(XEVENTDATA*)pData2;
			 XU8 s=FindAttrib(XEAB::TYPE,TEXT);
			 if(s==XEIT::CHECKBOX||s==XEIT::RADIO)
			 {
				m_nStatus=FindAttrib(XEAB::CHECKED,0);
				pData->rect|=XRect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
				SetVariable(pData);
				return XRREPAINT;
			 }
		 }break;
	case XDO_GET_STYLE:
		{
			XU32 s=XDomInput::Handle(nOpera,pData1,pData2);
			//XU8 t=
			XU8 t=FindAttrib(XEAB::TYPE,XEIT::TEXT);
			if(t==XEIT::IMAGE)
				s|=DOM_IMAGE;
			else if(t==XEIT::HIDDEN)
				s&=~DOM_SELECTABLE;
			else if(t==XEIT::RADIO)
				s|=DOM_NODE;
			return s;
		}break;
/*	case XDO_GET_IMAGE:
		 if(FindAttrib(XEAB::TYPE,XEIT::TEXT)==XEIT::IMAGE)
		 {
		  XVar*p=(XVar*)pData1;
		  XPCTSTR pStr=FindAttrib(XEAB::SRC,(XPCTSTR)XNULL);
		  if(pStr!=XNULL)
		  {	  *p=pStr;return XTRUE;}
		 }
		 break;*/
		 //break;
		 break;
	case XDO_SET_FINAL:
		 m_bFinal=XTRUE;
      		 if(m_pEdit==XNULL&&pData2!=0)
		 {	  
			 XEVENTDATA*p=(XEVENTDATA*)pData2;

			  XU8 s=FindAttrib(XEAB::TYPE,TEXT);
			 if(s==XEIT::CHECKBOX||s==XEIT::RADIO)
			 {
				m_nStatus=FindAttrib(XEAB::CHECKED,0);				
			 }

			 SetVariable(p);
			 
			 XPCTSTR strURL=FindAttrib(XEAB::LOCALSRC,FindAttrib(XEAB::SRC,(XPCTSTR)XNULL));
			 if(strURL==XNULL) return XFALSE;
				m_pImage=p->pDom->m_imgList.SetImage(strURL,this,p->pDom);
		     return m_pImage!=XNULL;
			 //return m_pImage!=XNULL;
		 }
		 break;

	case XDO_UPDATE_IMAGE:
		 return Handle(XDO_UPDATE_TABLE,pData1,pData2);
/*	case XDO_SET_IMAGE:
		 if(m_pImage!=XNULL) return XFALSE;
		 else
		 {
			 m_pImage=(XIMAGEDATA*)pData1;
			 m_pImage->nMethod=XEM::GET;
			 XPCTSTR pURL=FindAttrib(XEAB::SRC,(XPCTSTR)XNULL);
			 if(pURL!=XNULL)
				m_pImage->strURL=pURL;
			 return XTRUE;
		 }
		 return XTRUE;*/
	case XDO_REFRESH:
	     if(m_pEdit==XNULL)
		 {
			XU8 nType=FindAttrib(XEAB::TYPE,XEIT::TEXT);
			if(nType==XEIT::TEXT)
				break;
			XEVENTDATA*pData=(XEVENTDATA*)pData1;
			XPCTSTR strValue=RefreshVar((XDomLeaf*)pData->nData,XNULL,pData);
			if(strValue!=XNULL)
			{
				XString8 strv(strValue,STRING_ATTACH);
				if(nType==XEIT::CHECKBOX)
					m_nStatus=strv.Compare(XON,XTRUE)==0;
				else
					m_nStatus=strv.Compare(FindAttrib(XEAB::VALUE,(XPCTSTR)XNULL))==0;
				strv.EmptyNoFree();
				return XRREPAINT;
			}
		 }
		 break;

	case XDO_SETRADIO:
		 if((XDomItem*)pData2!=this&&FindAttrib(XEAB::TYPE,XEIT::TEXT)==XEIT::RADIO)
		 {
			 XEVENTDATA*pData=(XEVENTDATA*)pData1;
			 if(CheckName((XPCTSTR)pData->nData))
			 {
				 m_nStatus=0;
				 XU32 s=XRSETVAR;
				 s|=XRREPAINT;
				 pData->rect|=XRect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
				//str.EmptyNoFree();
				 return s;
			 }
		 }
		 break;
/*	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		{
		   XVar*p=(XVar*)pData1;
		   switch(p->nID)
		   {
		   case XEAB::CHECKED:
			    m_nStatus=XTRUE;//p->ToLogic();
				*p=XTRUE;
				break;
		   case XEAB::READONLY:
		   case XEAB::DISABLED:
			    //p->ToLogic();
			    *p=XTRUE;
				break;
		   }
		}break;	*/
	case XDO_CONNECT:
		 if(m_pEdit==XNULL)
			 Connect((XEVENTDATA*)pData1);
		 break;
	case XDO_EVENT_ONKEYDOWN:
		 if(pData1!=XK_RETURN) break;
	//case XDO_EVENT_ONMOUSEUP:
	case XDO_EVENT_ONCLICK:
		 if(m_pEdit==XNULL)
		 {
			 return OnMouseUp((XEVENTDATA*)pData2);
		 }break;
	case XDO_EVENT_ONMOUSEOUT:
		 if(m_pEdit==XNULL)
		 {
			 return OnMouseLeave((XEVENTDATA*)pData2);
		 }break;
	case XDO_EVENT_ONMOUSEDOWN:
		 if(m_pEdit==XNULL)
			 return OnMouseDown(*(XPoint*)pData1,(XEVENTDATA*)pData2);
		 break;
	case XDO_GET_RECT:
		 if(m_pEdit==XNULL)
		 {
			 XRect*pr=(XRect*)pData1;
			 *pr=XRect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
			 pr->InflateRect(1,1);
			 return XTRUE;
		 }break;
	case XDO_HITTEST:
		 return HitTest(pData1,pData2);
	case XDO_PAINT:
		 if(m_pEdit==XNULL)
		 {
			  return Paint((DRAWCONTEXT*)pData1);
		 }break;
	case XDO_LAYEROUT_CELLS:
		 return Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XTRUE);
	case XDO_LAYEROUT_CELL:
		 return Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
	}
	return XDomInput::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLInput::Layerout(DRAWCONTEXT *pDraw, CELLDATA *pData,XBOOL bCells)
{
	if(!m_bFinal) return 0;
	XU8 nType=FindAttrib(XEAB::TYPE,XEIT::TEXT);
	switch(nType)
	{
	case XEIT::HIDDEN:
		 return XTRUE;
	case XEIT::IMAGE:
		 LayeroutImage(pDraw,pData,bCells);
		 break;
	case XEIT::BUTTON:
	case XEIT::SUBMIT:
	case XEIT::RESET:
		 LayeroutButton(pDraw,pData,bCells);
		 break;
	case XEIT::FILE:
		 break;
	case XEIT::RADIO:
	case XEIT::CHECKBOX:
		{
			int nSize=XMIN(XBOXSIZE+4,XBOXSIZE+4);
			XSize sz(XBOXSIZE+4,XBOXSIZE+4);
			if(bCells)
				m_bReturn=NewRowIfNeed(pDraw,pData,sz.cx);
			else if(m_bReturn)
				NewRow(0,pDraw,pData,0);
			m_nWidth=m_nHeight=XBOXSIZE;
			m_nPosX=pData->cur.x+2;
			//int a=pData->alignh;
			//pData->alignh=XEA::MIDDLE;
			m_nPosY=AlignCol(pDraw,pData,XBOXSIZE,XEA::MIDDLE);
			//pData->alignh=a;
			SetRowSize(pDraw,pData,sz,sz.cx,bCells);
		}
		 break;
	default:
		return XDomInput::Handle(bCells?XDO_LAYEROUT_CELLS:XDO_LAYEROUT_CELL,
			(XU32)pDraw,(XU32)pData);
	}
	return XDomNode::Handle(bCells?XDO_LAYEROUT_CELLS:XDO_LAYEROUT_CELL,
							(XU32)pDraw,(XU32)pData);
}

void XHTMLInput::LayeroutButton(DRAWCONTEXT *pDraw, CELLDATA *pData, XBOOL bCells)
{
    if(m_nWidth==0)
	{
		m_strTitle;
		if(FindAttrib(XEAB::TYPE,TEXT)==XEIT::IMAGE)
			m_strTitle=FindAttrib(XEAB::ALT," ");
		else
			m_strTitle=FindAttrib(XEAB::VALUE," ");
		XSize sz;
		pDraw->pDraw->MeasureString(m_strTitle,sz);
		m_nWidth=sz.cx+8;
		m_nHeight=sz.cy+4;
		int w=FindAttrib(XEAB::WIDTH,0);
		int h=FindAttrib(XEAB::HEIGHT,0);
		if(m_nWidth<w)
			m_nWidth=w;
		if(m_nHeight<h)
			m_nHeight=h;
	}
	XSize sz(m_nWidth+2,m_nHeight+2);
	//if(pData->IsWrap()&&!pData->bReturn&&
	//	sz.cx+pData->cur.x>pDraw->win.right)
	if(bCells)
		m_bReturn=NewRowIfNeed(pDraw,pData,sz.cx);
	else if(m_bReturn)
		NewRow(0,pDraw,pData,0);

	m_nPosX=pData->cur.x+2;
	//int a=pData->alignh;
	//pData->alignh=XEA::CENTER;
	m_nPosY=AlignCol(pDraw,pData,m_nHeight+2,pDraw->ALIGNH);//pData->cur.y+1;
	//pData->alignh=a;	
	//sz.cx++;

	SetRowSize(pDraw,pData,sz,sz.cx,bCells);
}

XBOOL XHTMLInput::Paint(DRAWCONTEXT *pDraw)
{
	if(m_nWidth==0) return XFALSE;
	XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
	if(!rect.IsUnion(pDraw->paint)) return XFALSE;

	XU32 bc=pDraw->pDraw->GetBackColor();
	XU32 tc=pDraw->pDraw->GetColor();
	XGraphics*p=pDraw->pDraw;
	XU8 nType=FindAttrib(XEAB::TYPE,XEIT::TEXT);
	if(nType==XEIT::RADIO||nType==XEIT::CHECKBOX)
	{
		//rect.bottom+=2;
		//XRect rt=rect;
		//p->SetBackColor(pDraw->DCSELECTBACK);
		//p->FillRectangle(rect);
		//rect.right++;rect.bottom++;
		rect.InflateRect(1,1);
		PaintSelect(rect,pDraw);
		rect.DeflateRect(1,1);
		//rect.right--;rect.bottom--;
		if(nType==XEIT::RADIO)
			p->DrawRadio(rect,pDraw->DCBACK,m_nStatus);
		else
			p->DrawCheckBox(rect,pDraw->DCBACK,m_nStatus);
		if(pDraw->pSelect==this)
		{
			pDraw->pDraw->SetBackColor(pDraw->DCSELECTBACK);
			pDraw->pDraw->SetColor(pDraw->DCSELECTTEXT);
		}
		else if(pDraw->pActive==this)
		{
			pDraw->pDraw->SetBackColor(bc);
			pDraw->pDraw->SetColor(pDraw->DCACTIVE);
		}
		else
		{
			p->SetBackColor(bc);
			p->SetColor(tc);
		}
		//PaintSelect(rt,pDraw);
		XDomNode::Handle(XDO_PAINT,(XU32)pDraw,0);
	}
	else
	{
		
		XU8 s=XBS_BFRAME;
		rect.InflateRect(1,1);
		PaintSelect(rect,pDraw);
		rect.DeflateRect(1,1);
		XImage*pImage=GetImage();
		if(nType==XEIT::IMAGE&&pImage!=XNULL)
		{
			
			if(pImage!=XNULL)
			{
				//int w=pImage->Width();
				//int h=pImage->Height();
				p->DrawImage(*pImage,rect);
			}

		}
		else
		{
			int border=FindAttrib(XEAB::BORDER_TOPWIDTH,-1);
			int dx=1,dy=1;
			if(m_nStatus==0) {s|=XBS_THICK;dx=0;dy=0;}
			XColor cc(pDraw->DCBACK);
			if(pDraw->DCBACK==0)
			   cc=XWindow::GetSysColor(XCW_BACK);
			cc.Dark(10);
			if(border!=0)
				p->DrawButton(rect,cc,s);//pDraw->DCBUTTON,s);
			else
				PaintBack(pDraw,rect);
			p->SetColor(pDraw->DCTEXT);
			p->DrawString(m_strTitle,m_nPosX+3+dx,m_nPosY+3+dy);
			
		}		
	}
	p->SetBackColor(bc);
	p->SetColor(tc);

	//m_pButton->Paint(*pDraw->pDraw);
    return XTRUE;
}

XU32 XHTMLInput::HitTest(int x,int y)
{
	if(m_pEdit!=XNULL) return XDomInput::Handle(XDO_HITTEST,x,y);
	XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
	rect.InflateRect(1,1);
	if(rect.PtInRect(x,y)) return (XU32)this;

	XU8 type=FindAttrib(XEAB::TYPE,XEIT::TEXT);
	if(type==XEIT::RADIO||type==XEIT::CHECKBOX)
	{
	   if(XDomNode::Handle(XDO_HITTEST,x,y)) return (XU32)this;
	}

	return 0;
}

XU32 XHTMLInput::OnMouseDown(XPoint &pt, XEVENTDATA *pData)
{
	XU8 type=FindAttrib(XEAB::TYPE,XEIT::TEXT);
	if(type==XEIT::RADIO||type==XEIT::CHECKBOX) 
		return 0;
    m_nStatus=1;
	XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
	XU32 s=XRREPAINT;
	pData->rect|=rect;
	return s;
}

XU32 XHTMLInput::OnMouseLeave(XEVENTDATA *pData)
{
	XU8 type=FindAttrib(XEAB::TYPE,XEIT::TEXT);
	if(type==XEIT::RADIO||type==XEIT::CHECKBOX) 
		return 0;

	m_nStatus=0;
	XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
	XU32 s=XRREPAINT;
	pData->rect|=rect;
	return s;
}

XU32 XHTMLInput::OnMouseUp(XEVENTDATA *pData)
{
	XU32 s=0;
	XU8 type=FindAttrib(XEAB::TYPE,XEIT::TEXT);
	if(type==XEIT::RADIO||type==XEIT::CHECKBOX)
	{
		s=XRREPAINT;
		pData->rect|=XRect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
		if(type==XEIT::CHECKBOX)
			m_nStatus=!m_nStatus;
		else
		{
			m_nStatus=1;
			XDomItem*p=GetParent();
			XPCTSTR strName=FindAttrib(XEAB::NAME,(XPCTSTR)XNULL);
			if(p!=XNULL)
			{				
				XU32 nData=pData->nData;
				pData->nData=(XU32)strName;
				p->Handle(XDO_RESETRADIO,(XU32)pData,(XU32)this);
				pData->nData=nData;
			}
		}
		SetVariable(pData);
	}
	else
	{
		s=OnMouseLeave(pData);
		//if(type==XEIT::RESET)

		//else//if(type==XEIT::SUBMIT)
		{
			XDomItem*p=GetParent();
			if(p!=XNULL)
			{
				pData->strRefer=pData->setURL.m_strURL;
				PostVariable(pData);
				s|=p->Handle(type==XEIT::RESET?XDO_RESETINPUT:XDO_SUBMIT,
							 0,(XU32)pData);
			}
		}
	}
	return s;
}

XU32 XHTMLInput::Connect(XEVENTDATA *pData)
{
	int nType=FindAttrib(XEAB::TYPE,XEIT::TEXT);
	switch( nType)
	{
	case XEIT::HIDDEN:
		 PostVariable(pData);
		 break;
	case XEIT::IMAGE:
	case XEIT::BUTTON:
	case XEIT::SUBMIT:
		 break;
		 
	case XEIT::FILE:
		 return XRNONE;
	case XEIT::CHECKBOX:
		 if(m_nStatus)
		 {
			 //XPCTSTR str=XON;
			 
			 PostVariable(pData,FindAttrib(XEAB::VALUE,XON));
		 }
		 //else
		//	 PostVariable(pData,XOFF);
		 break;
		 
	case XEIT::RADIO:
		 if(m_nStatus)
			 PostVariable(pData);
		 break;
	default:
		PostVariable(pData);
		break;
		//return XDomInput::Connect(pData);
	}
	return XRSETVAR;
}

//DEL XU8 XHTMLInput::IsRadioCheck()
//DEL {
//DEL 	XU8 s=FindAttrib(XEAB::TYPE,XEIT::TEXT);
//DEL 	if(s==XEIT::RADIO) return 1;
//DEL 	else if(s==XEIT::CHECKBOX) return 2;
//DEL 	return 0;
//DEL 
//DEL }

void XHTMLInput::SetVariable(XEVENTDATA *pData)
{
	switch(FindAttrib(XEAB::TYPE,XEIT::TEXT))
	{
	case XEIT::CHECKBOX:
		 if(m_nStatus)
			 XDomLeaf::SetVariable(pData,XNULL,XON);
		 else
			 XDomLeaf::SetVariable(pData,XNULL,XOFF);
		 break;
	case XEIT::RADIO:
		 if(m_nStatus)
			 XDomLeaf::SetVariable(pData,XNULL,XNULL);
		 break;
	}
	return;
}

void XHTMLInput::LayeroutImage(DRAWCONTEXT *pDraw, CELLDATA *pData, XBOOL bCells)
{
	 XImage*pImage=GetImage();
	 if(pImage==XNULL||
		pImage->IsEmpty()) LayeroutButton(pDraw,pData,bCells);
	 else
	 {
		XSize sz;
		sz.cx=pImage->Width()+2;
		sz.cy=pImage->Height()+4;
		m_nWidth=sz.cx-2;
		m_nHeight=sz.cy-4;
		if(bCells)
			m_bReturn=NewRowIfNeed(pDraw,pData,sz.cx);
		else if(m_bReturn)
			NewRow(0,pDraw,pData,0);

		m_nPosX=pData->cur.x+1;
		//int a=pData->alignh;
		//pData->alignh=XEA::CENTER;
		m_nPosY=AlignCol(pDraw,pData,m_nHeight,XEA::MIDDLE);
		//pData->alignh=
		SetRowSize(pDraw,pData,sz,sz.cx,bCells);
		
	 }
}

XImage* XHTMLInput::GetImage()
{
   XImage*pImage=XNULL;
   if(m_pImage!=XNULL)
   { 
	 switch(m_pImage->nStatus)
	 {
	 case XL_WAIT:
	 case XL_LOADING:
		  pImage=NULL;//XWindow::GetIcon(XICON_LOADING);
		  break;
	 default:
	 case XL_OK:
		  //pImage=&pDraw->images[XICON_LOADING];
		  //if(!m_pImage->image.IsEmpty())
		  pImage=m_pImage->GetImage();//&m_pImage->image;
		  if(pImage->IsEmpty())
			  pImage=XNULL;
		  //pImage=&pDraw->images[XICON_LOADING];
		  break;
	 }
   }
   return pImage;

}

XU32 XHTMLInput::OnTimer(XEVENTDATA *pData)
{
	if(m_pEdit!=XNULL) return XDomInput::OnTimer(pData);
	XRect rt(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
	if(!rt.IsUnion(pData->viewRect)) return XRNONE;
	XImage*pImage=GetImage();
	if(pImage!=XNULL&&pImage->FrameCount()>1)
	{
		m_nTicks++;
		int nc=pImage->GetFrameDelay(m_nIndex);
		if(m_nTicks<nc) return XRNONE;
		m_nTicks=0;
		m_nIndex++;
		if(m_nIndex>=pImage->FrameCount())
			m_nIndex=0;
		
		pImage->SetActive(m_nIndex);

		pData->rect|=rt;
		return XRREPAINT;
	}
	return XRNONE;
}
