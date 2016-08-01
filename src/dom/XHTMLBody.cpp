// XHTMLBody.cpp: implementation of the XHTMLBody class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLBody.h"
#include "XDomText.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XHTMLBody::XHTMLBody(XU16 nID):XDomCard(nID)
{
	m_nPosX=0;
	m_nPosY=0;
	m_nWidth=0;
	m_nHeight=0;
}

 XU32 XHTMLBody::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
 {
	XU32 i;
 	switch(nOpera)
 	{
 	//case XDO_SET_START:
 	//	 return StartEvent((DRAWCONTEXT*)pData1);
	/*case XDO_LAYEROUT_CELLS:
		{
			CELLDATA*p=(CELLDATA*)pData2;
			DRAWCONTEXT*pd=(DRAWCONTEXT*)pData1;
			XU32 ss=PreLayerout(pd,p);
 			XDomCard::Handle(nOpera,pData1,pData2);
 			EndLayerout(pd,p);
		}return 1;*/
 	case XDO_LAYEROUT_CELL:
 		{
			  CELLDATA data;
			  LAYEROUTDATA margin;
			  //data.Reset(pData->cur.x,pData->cur.y,XTRUE);
			  CELLDATA*pData=&data;//(CELLDATA*)pData2;
			  DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			  pDraw->Init();
			  pData->Reset(pDraw->win.left,pDraw->win.top,XTRUE);

			  PreLayerout(pDraw,pData,&margin);
			  if(m_nWidth>0) 
			  {
				  if(margin.nMargin[BORDERDATA::LEFT]>0&&
					 margin.nMargin[BORDERDATA::LEFT]+m_nWidth>pDraw->view.Width())
				  {
					  margin.nMargin[BORDERDATA::LEFT]=XMAX(
						  (pDraw->view.Width()-m_nWidth)>>1,0);
				  }
				  if(m_nWidth>pDraw->win.Width())
					pDraw->win.right=pDraw->win.left+m_nWidth;
			  }
			  
			  
			  //pData->Init();
			  InitMargin(pDraw,pData,&margin,m_nPosX,m_nPosY,m_nWidth,m_nHeight);
			  XPoint pt=pData->cur;
			  for(i=0;i<m_childs.GetSize();i++)
				m_childs[i]->LayeroutItem(pDraw,pData,XTRUE);//Handle(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)&data);
			  pData->Reset(pt.x,pt.y,XFALSE);
			  NewRow(0,pDraw,pData,pDraw->IND);
			  pData->bEmpty=XTRUE;
			  for(i=0;i<m_childs.GetSize();i++)
				m_childs[i]->LayeroutItem(pDraw,pData,XFALSE);//Handle(XDO_LAYEROUT_CELL,(XU32)pDraw,(XU32)&data);
			  //data.max.cx-=pData->cur.x;
			  //data.max.cy-=pData->cur.y;
			  m_nWidth=pData->max.cx-pt.x;
			  m_nHeight=pData->max.cy-pt.y;
			  if(m_nWidth<pDraw->view.Width())
				  m_nWidth=pDraw->view.Width();
			  if(m_nHeight<pDraw->view.Height())
				  m_nHeight=pDraw->view.Height();
			  XSize size(m_nWidth,m_nHeight);
			  SetMargin(&margin,m_nWidth,m_nHeight,size);
			  //m_nWidth=data.max.cx-pData->cur.x;
	          pData=(CELLDATA*)pData2;
			  SetRowSize(pDraw,pData,size,m_nWidth,XFALSE);
			  NewRow(0,pDraw,pData,pDraw->IND);

			/*CELLDATA*p=(CELLDATA*)pData2;
			DRAWCONTEXT*pd=(DRAWCONTEXT*)pData1;
			LAYEROUTDATA margin;
			XU32 ss=PreLayerout(pd,p,margin);
//			StartMargin(pd,p,XTRUE);
			//BeginMargin(pd,p);
			m_nPosX=p->cur.x;
			m_nPosY=p->cur.y;
 			XDomCard::Handle(nOpera,pData1,pData2);
 			EndLayerout(pd,p);
 			m_nWidth=XMAX(p->max.cx-m_nPosX,pd->view.Width()-(m_nPosX<<1)-1);
 			m_nHeight=XMAX(p->max.cy-m_nPosY,pd->view.Height()-(m_nPosY<<1)-1);*/
 		}return 1;
 	case XDO_PAINT:
 		{
 			XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
 			PaintBack((DRAWCONTEXT*)pData1,rect);
			XU32 s=XDomCard::Handle(nOpera,pData1,pData2);
			PaintBorder((DRAWCONTEXT*)pData1,rect);
			return s;
 		};
	case XDO_EVENT_ONTIMER:
		{
			XRect rect(0,0,m_nWidth,m_nHeight);
			return OnTimer((XEVENTDATA*)pData1,rect)|
				   XDomCard::Handle(nOpera,pData1,pData2);
		}break;
 /*	case XDO_SET_ATTR:
 	case XDO_ADD_ATTR:
 		 SetBodyAttrib((XVar*)pData1);
 		 break;	*/
 		 //return XTRUE;
 	}
 	return XDomCard::Handle(nOpera,pData1,pData2);
 }

/*XU32 XHTMLBody::Color(XString8 &strColor)
{
   if(strColor.GetLength()<4) return 0;
   if(strColor[0]=='#') return strColor.ToInt(16)|0xff000000;
   else if(strColor.Compare("rgb(",XTRUE,4)==0)
   {
	  XString8Array list;
	  list.SplitString(strColor,',',XTRUE,4);
	  XU32 nColor=0xff000000;
	  if(list.GetSize()>=3)
	  {
		  nColor|=((list[0].ToInt()&0xff)<<16);
		  nColor|=((list[1].ToInt()&0xff)<<8);
		  nColor|=((list[2].ToInt()&0xff));
	  }
	  return nColor;
   }
   else return XEnumColor().Color(strColor);
}*/

/*XBOOL XHTMLBody::SetBodyAttrib(XVar *p)
{
	switch(p->nID)
	{
	case XEAB::TEXT:
	case XEAB::VLINK:
	case XEAB::ALINK:
	case XEAB::BGCOLOR:
	case XEAB::LINK:
		 if(p->nType==VSTRING)
			 *p=XEnumColor().ColorOf(p->strData);
		 break;
	}
	return XTRUE;
}*/


/*XU32 XHTMLBody::StartEvent(DRAWCONTEXT *pDraw)
{
	XU32 nColor=FindAttrib(XEAB::TEXT,0);
	if(nColor==0) nColor=FindAttrib(XEAB::COLOR,0);
	if(nColor!=0) 
		pDraw->DCTEXT=nColor;
	nColor=FindAttrib(XEAB::ALINK,0);
	if(nColor!=0)
		pDraw->DCACTIVE=nColor;
	nColor=FindAttrib(XEAB::LINK,0);
	if(nColor!=0)
		pDraw->DCLINK=nColor;
	nColor=FindAttrib(XEAB::BGCOLOR,0);
	if(nColor!=0)
		pDraw->DCBACK=nColor;
	//nColor=FindAttrib(XEAB::
	return 0;
}*/

XU32 XHTMLFont::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	if(!IsVisiable()) return 0;
	switch(nOpera)
	{
/*	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		{
			XVar*p=(XVar*)pData1;
			switch(p->nID)
			{
			case XEAB::ALIGN:
				*p=XEnumAlign().Index(p->strData);break;
			case XEAB::COLOR:
				{
					XString8 strColor(p->ToString(),STRING_ATTACH);
					p->strData=XNULL;
					p->nType=VINT;
					*p=XEnumColor().Color(strColor);
				}
				break;
			case XEAB::SIZE:
				p->ToInt();
				break;
			}
			//return XTRUE;
		}break;*/
	//case XDO_PAINT_TABLE:
	//case XDO_LAYEROUT_TAB:
	//case XDO_LAYEROUT_TABS:
	case XDO_PAINT:
	case XDO_LAYEROUT_CELLS:
	case XDO_LAYEROUT_CELL:
		{
			DRAWCONTEXT*p=(DRAWCONTEXT*)pData1;
			XU32 f=p->pDraw->GetFontStyle();
			XU32 ft=p->FONTSTYLE;
			XU32 fs=p->FONTSIZEALL;
			switch(m_nID)
			{
			case XDOM_FONT:
				 if(IsInline())
					return XDomNode::Handle(nOpera,pData1,pData2);
				 else
					return XDomBlock::Handle(nOpera,pData1,pData2);
				 break;
				 //return HandleFont(nOpera,pData1,pData2);
			case XDOM_S:
			case XDOM_STRIKE:p->SetStrike(XGraphics::XF_STRIKE);break;
			case XDOM_H1:p->SetFontSize(XGraphics::XS_HEAD1);break;
			case XDOM_H2:p->SetFontSize(XGraphics::XS_HEAD2);break;
			case XDOM_H3:p->SetFontSize(XGraphics::XS_HEAD3);break;
			case XDOM_H4:p->SetFontSize(XGraphics::XS_HEAD4);break;
			case XDOM_H5:p->SetFontSize(XGraphics::XS_HEAD5);break;
			case XDOM_H6:p->SetFontSize(XGraphics::XS_HEAD6);break;
			}
			p->SetBold(XTRUE);
			//p->OnFontChange();
			//XU32 ss=PrePaint(p);
			if(IsInline())
			{
				if(nOpera!=XDO_PAINT)
				{
					CELLDATA*pData=(CELLDATA*)pData2;			
					XU8 bInline=IsFingerInline();
					if(!bInline&&!pData->bNewLine)
						NewRow(0,p,pData,p->IND);
					//AlignRow(p,pData);
					XDomNode::Handle(nOpera,pData1,pData2);
					if(!bInline&&!pData->bNewLine)
						NewRow(0,p,pData,p->IND);//,nOpera==XDO_LAYEROUT_CELLS);

				}
				else 
					XDomNode::Handle(nOpera,pData1,pData2);
			}
			else XDomBlock::Handle(nOpera,pData1,pData2);
			//EndPaint(p,ss);
			p->FONTSIZEALL=fs;
			p->FONTSTYLE=ft;
			p->pDraw->SetFontStyle(f);
			//p->OnFontChange();
			//EndPaint(ss);
		}
		return XTRUE;
	}
	if(IsInline())
		return XDomNode::Handle(nOpera,pData1,pData2);
	else
		return XDomBlock::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLHR::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	if(!IsVisiable()) return 0;
	switch(nOpera)
	{
	case XDO_LAYEROUT_CELLS:
		 return Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XTRUE);
	case XDO_LAYEROUT_CELL:
		 return Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
	case XDO_PAINT:
		 return Paint((DRAWCONTEXT*)pData1);
	case XDO_ADD_ATTR:
	case XDO_SET_ATTR:
/*		{
			XVar*p=(XVar*)pData1;
			switch(p->nID)
			{
			 case XEnumAttribs::ALIGN:
				  if(p->nType==VSTRING) 
					  *p=XEnumAlign().Index(p->strData);
				  break;
			 case XEnumAttribs::NOSHADE:
				  p->ToLogic();
				  break;
			 case XEnumAttribs::SIZE:
			 case XEnumAttribs::WIDTH:
				  if(p->nType==VSTRING)
					  *p=PECENT(XString8(p->strData));
			  break;
			}
		}*/
		 break;
	}
	return XDomLeaf::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLHR::Layerout(DRAWCONTEXT *pDraw, CELLDATA *pData,XU8 bCells)
{
	if(!pData->bNewLine)
		NewRow(0,pDraw,pData,0);//,bCells);
	XSize sz;
	GetSize(pDraw,pData,sz);
	if(!bCells)
	{
		m_nPosX=pData->cur.x;
//		XU8 a=pData->alignw;
//		pData->alignw=FindAttrib(XEAB::ALIGN,a);
//		AlignRow(pDraw,pData);
//		pData->alignw=a;
		m_nPosY=pData->cur.y+2;
	}
	m_nWidth=sz.cx;
	m_nHeight=sz.cy;
	sz.cy+=4;
	
	SetRowSize(pDraw,pData,sz,pDraw->SPACE,bCells);
	NewRow(0,pDraw,pData,0);//,bCells);
	return XTRUE;
}

void XHTMLHR::GetSize(DRAWCONTEXT*pDraw,CELLDATA*pData,XSize &sz)
{
	sz.cx=FindAttrib(XEAB::WIDTH,-100);
	if(sz.cx<0) sz.cx=GetWinWidth(pDraw,pData)*(-sz.cx)/100;
	sz.cy=FindAttrib(XEAB::SIZE,1);
	if(sz.cy<0) sz.cy=pDraw->win.Height()*(-sz.cy)/100;
}

XU32 XHTMLHR::Paint(DRAWCONTEXT *pDraw)
{
	XSize sz(m_nWidth,m_nHeight);
	//GetSize(pDraw,sz);
	XRect rect(m_nPosX,m_nPosY,m_nPosX+sz.cx,m_nPosY+sz.cy+1);
	if(!rect.IsUnion(pDraw->paint)) return XFALSE;
	rect.bottom--;
	XU32 c=pDraw->pDraw->GetBackColor();
	if(!FindAttrib(XEAB::NOSHADE,XFALSE))
	{
	   rect.right--;
	   rect+=XPoint(1,1);
	   XColor color(pDraw->DCBACK);
	   color.Bright(30);
	   pDraw->pDraw->SetBackColor(color);
	   pDraw->pDraw->FillRectangle(rect);
	   rect-=XPoint(1,1);
	}
	XColor color(pDraw->DCBACK);
	color.Dark(30);
	pDraw->pDraw->SetBackColor(color);
	pDraw->pDraw->FillRectangle(rect);
	pDraw->pDraw->SetBackColor(c);
	return XTRUE;
}

/*XU32 XHTMLFont::HandleFont(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
	//CELLDATA*pData=(CELLDATA*pData2);

//	XU32 nColor=FindAttrib(XEAB::COLOR,pDraw->DCTEXT);
//	XU32 s=pDraw->pDraw->GetFontStyle();
//	XU8  nSize=FindAttrib(XEAB::SIZE,0);
	
//	if(nSize>0&&nSize<=7)
	
//	{
//		pDraw->pDraw->SetFontSize(nSize);
		//pDraw->OnFontChange();
//	}
//	nColor=pDraw->pDraw->SetColor(nColor);
//	XU32 ss=PrePaint(p);
	XU32 r=XDomNode::Handle(nOpera,pData1,pData2);
//	EndPaint(p,ss);
	//if(nSize>0)		   
	//{
	//	pDraw->pDraw->SetFontStyle(s);
		//pDraw->OnFontChange();
	//}
	//pDraw->pDraw->SetColor(nColor);
	return r;
}*/

XU32 XHTMLDiv::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
//	if(IsFingerInline())
//		int a=0;
	//	return XDomNode::Handle(nOpera,pData1,pData2);
	DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
	switch(nOpera)
	{
	case XDO_PAINT:
		 if(!IsVisiable()) return 0;
		 if(m_nWidth>0)
		 {
			 XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
			 
			 if(!rect.IsUnion(pDraw->paint)) return 0;
			 //if(FindAttrib(XEAB::CLASS,0)==16390)
		      //	int a=0;

			 PaintBack(pDraw,rect);
			 PrePaint(pDraw);
			 XU32 s=HandleChild(nOpera,pData1,0);
			 EndPaint(pDraw);
			 PaintBorder(pDraw,rect);
			 //if(bSet)
			//	 pDraw->Restore();
			 return s;
		 }
		 else if(pDraw->nLayerMode==LA_SAMPLE) break;
		 return 0;
	case XDO_LAYEROUT_CELLS:
		 if(!IsVisiable()) return 0;
//		 if(FindAttrib(XEAB::CLASS,0)==16393)
//			int a=0;

		 return LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,!IsFingerInline());
	case XDO_LAYEROUT_CELL:
		 if(!IsVisiable()) return 0;

		 return LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,!IsFingerInline());

	}
	return XDomBlock::Handle(nOpera,pData1,pData2);
}


/*void XHTMLDiv::InitBlock(DRAWCONTEXT *pDraw, CELLDATA *pData, 
						 CELLDATA &block,XU8 bCells)
{
   XU8 ft=FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);
   if(!ft&&!pData->bReturn)//&&pData->cur.x+m_nWidth>pDraw->win.right)
	  NewRow(0,pDraw,pData);
 
   m_nPosX=pData->cur.x;
   m_nPosY=pData->cur.y;
   if(ft==XELT::FLOAT_RIGHT&&!bCells)
	   m_nPosX=pDraw->win.right-m_nWidth;
   block.Reset(m_nPosX,pData->cur.y,FindAttrib(XEAB::ALIGN,pData->alignw),
			  pData->alignh,pData->nowrap,pData->ind);
   block.bReturn=XTRUE;
}*/

//DEL XU32 XHTMLDiv::LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData)
//DEL {
//DEL 
//DEL 
//DEL //   if(m_nWidth<=6) 
//DEL //	   return 0;
//DEL    if(pData->nRow>=(XINT)pData->rowws.GetSize()) return 0;
//DEL    
//DEL    XRect rect=pDraw->win;
//DEL    CELLDATA data;
//DEL 
//DEL    //XString8 strClass=FindAttrib(XEAB::CLASS,"");
//DEL    //if(strClass=="content")
//DEL 	//	int a=0;
//DEL 
//DEL    XU8 ft=FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);
//DEL    if(!ft)//&&!pData->bReturn)//&&pData->cur.x+m_nWidth>pDraw->win.right)
//DEL    {
//DEL 	  //if(m_bReturn)
//DEL 	  NewRowMust(pDraw,pData,m_bReturn);
//DEL 		  //NewRowMust(pDraw,pData);
//DEL 	  //NewRow(0,pDraw,pData);
//DEL    }
//DEL    else if(m_bReturn)
//DEL 	   NewFloat(pDraw,pData,ft==XELT::FLOAT_LEFT);
//DEL    m_nPosX=pData->cur.x;
//DEL    m_nPosY=pData->cur.y;
//DEL    if(ft==XELT::FLOAT_RIGHT)
//DEL 	   m_nPosX=pDraw->win.right-m_nDeltaX;
//DEL 	   //GetWinRight(pDraw,pData)-m_nWidth;
//DEL    else if(ft==XELT::FLOAT_LEFT)
//DEL 	   m_nPosX=pDraw->win.left+m_nDeltaX;
//DEL 	  // m_nPosX=GetWinLeft(pDraw,pData);
//DEL    data.Reset(m_nPosX,pData->cur.y,FindAttrib(XEAB::ALIGN,pData->alignw),
//DEL 			  pData->alignh,pData->nowrap);
//DEL //   data.bReturn=XTRUE;
//DEL 
//DEL    //InitBlock(pDraw,pData,data,XFALSE);
//DEL    InitSize(pDraw,&data,m_nWidth,XTRUE);//m_nWin);
//DEL //   XU8 ft=FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);
//DEL //   NewRow(0,pDraw,&data,pDraw->ind);
//DEL //   data.bEmpty=XTRUE;
//DEL    //AlignRow(pDraw,pData);
//DEL    XDomNode::Handle(XDO_LAYEROUT_CELL,(XU32)pDraw,(XU32)&data);
//DEL    pDraw->win=rect;
//DEL    SaveData(&data);
//DEL    
//DEL    if(ft)
//DEL    {
//DEL 	   SetFloat(pDraw,pData,XSize(m_nWidth,m_nHeight),ft==XELT::FLOAT_LEFT);
//DEL 	   //if(FindAttrib(XEAB::CLEAR,0))
//DEL 	  // {
//DEL 	//	   NewFloat(pDraw,pData,ft==XELT::FLOAT_LEFT);
//DEL 	//	   NewRow(0,pDraw,pData);
//DEL 	  // }
//DEL    }
//DEL    else
//DEL    {
//DEL 	  SetRowSize(pDraw,pData,XSize(m_nWidth,m_nHeight),m_nMin,XFALSE);
//DEL 	  if(!pData->bNewLine)
//DEL 		   NewRow(0,pDraw,pData,pDraw->ind);
//DEL    }
//DEL    //pDraw->nFixWin=nFix;
//DEL    return 1;
//DEL }

//DEL XU32 XHTMLDiv::LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData)
//DEL {
//DEL    CELLDATA data;
//DEL    XRect rw=pDraw->win;
//DEL 
//DEL   //XString8 strClass=FindAttrib(XEAB::CLASS,"");
//DEL    //if(strClass=="content")
//DEL 	//	int a=0;
//DEL    //int nFix=GetWinRight(pDraw,pData)-pData->cur.x;
//DEL    XU8 ft=FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);
//DEL    
//DEL    int w=FindAttrib(XEAB::WIDTH,0);   
//DEL    if(w<0) w=-GetWinWidth(pDraw,pData)*w/100;
//DEL    if(!ft)
//DEL    {
//DEL 	  //if(w==937)
//DEL 	//	  int a=0;
//DEL   	  m_bReturn=NewRowIfMust(pDraw,pData,w);
//DEL    }
//DEL    else
//DEL    {
//DEL 	 //  if(w==637)
//DEL 	//	   int a=0;
//DEL 	   m_bReturn=NewFloatIfNeed(pDraw,pData,w,ft==XELT::FLOAT_LEFT);
//DEL    }
//DEL 
//DEL    data.Reset(pData->cur.x,pData->cur.y,FindAttrib(XEAB::ALIGN,pData->alignw),
//DEL 			  pData->alignh,pData->nowrap);
//DEL //   data.bReturn=XTRUE;
//DEL 	   
//DEL    InitFixed(pDraw,&data,0);//pDraw->win.Width());//,w,XFALSE);
//DEL    pDraw->win=rw;
//DEL 
//DEL    if(!ft)
//DEL    {
//DEL 	   if(w==0)
//DEL 		  m_nWidth=XMAX(m_nWidth,GetWinWidth(pDraw,pData));
//DEL 	   //m_bReturn=NewRowIfMust(pDraw,pData,m_nWidth);
//DEL 	   SetRowSize(pDraw,pData,XSize(m_nWidth,m_nHeight),m_nMin,XTRUE);  
//DEL 	   if(!pData->bNewLine)
//DEL 		   NewRow(0,pDraw,pData,pDraw->ind);
//DEL    }
//DEL    else
//DEL    {
//DEL 	   //m_bReturn=NewFloatIfNeed(pDraw,pData,m_nWidth,ft==XELT::FLOAT_LEFT);
//DEL 	   SetFloat(pDraw,pData,XSize(m_nWidth,m_nHeight),ft==XELT::FLOAT_LEFT);
//DEL 	   if(ft==XELT::FLOAT_LEFT)
//DEL 		   m_nDeltaX=pData->cur.x-pDraw->win.left-m_nWidth;
//DEL 	   else
//DEL 		   m_nDeltaX=pDraw->win.right-GetWinRight(pDraw,pData);
//DEL    }
//DEL       
//DEL    return 0;
//DEL    
//DEL 
//DEL }


////////////////////////////////////////////////////////////////////

XU32 XHTMLNobr::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_LAYEROUT_CELL:
	case XDO_LAYEROUT_CELLS:
		{
			CELLDATA*pData=(CELLDATA*)pData2;
			DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			if(pData->bNewLine)//pData->cur.x+pDraw->SPACE>=pDraw->win.right)
				NewRow(0,pDraw,pData,pDraw->IND);
			XU8 nw=pDraw->WRAPFLAG;
			pDraw->WRAPFLAG=XEA::NOWRAP;
			XU32 s=XDomNode::Handle(nOpera,pData1,pData2);
			pDraw->WRAPFLAG=nw;
			
			return s;
		}break;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}


