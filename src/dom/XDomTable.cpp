// XDomTable.cpp: implementation of the XDomTable class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomTable.h"
#include "XEnumData.h"
#include "XDomText.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomTable::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	XVar*p=(XVar*)pData1;
	switch(nOpera)
	{
	case XDO_GET_RECT:
		 *((XRect*)pData1)=XRect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
		 return XTRUE;
	case XDO_HITTEST:
		{
		  XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
		  if(!rect.PtInRect(pData1,pData2)) return 0;
		}break;
/*	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		 switch(p->nID)
		 {
		 case XEnumAttribs::COLUMNS:
			  p->ToInt();
			  break;
		 case XEnumAttribs::ALIGN:
			  if(p->nType==VSTRING) 
				  *p=XEnumAlign().Index(p->strData);
			  break;
		 }
		 break;*/
	case XDO_UPDATE_TABLECHILD:
//		 m_bNoChild=0;
		 break;
	case XDO_UPDATE_TABLE:
		 //m_nWin=0;
		 //XDomNode::Handle(XDO_
		 m_nWidth=0;//m_nFixWidth=0;
		 break;
	case XDO_SET_FINAL:
		 //if(m_bFinal)
		 if(m_pParent!=XNULL)
		   m_pParent->Handle(XDO_UPDATE_TABLECHILD,0,0);
		 m_bFinal=1;
		 //m_nWin=0;
		 m_nWidth=0;//m_nFixWidth=0;
		 //m_bFinal=1;
		 break;
	case XDO_GET_STYLE:
		 return XDomNode::Handle(nOpera,pData1,pData2)|DOM_TABLE|DOM_PAINT;
	case XDO_LAYEROUT_CELL:
		 LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
	case XDO_LAYEROUT_CELLS:
		 LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
	case XDO_PAINT:
		 return Paint((DRAWCONTEXT*)pData1);
		 
		//{
			//XRect 
		//}
		 //return XDomNode::Handle(XDO_PAINT_TABLE,pData1,pData2);
		 //break;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

XU32 XDomTable::Paint(DRAWCONTEXT *pDraw)
{
   	//XString8 str=FindAttrib(XEAB::ID,"");
	//if(str=="last")
	//	int a=0;


   if(m_nWidth<=0) return 0;
   if(m_nPosX>=pDraw->paint.right||
	  m_nPosX+m_nWidth<=pDraw->paint.left||
	  m_nPosY>=pDraw->paint.bottom||
	  m_nPosY+m_nHeight<=pDraw->paint.top) return 0;

  XU32 ss=PrePaint(pDraw);

  XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);

  int bd=FindAttrib(XEAB::BORDER,0);
  XU8 nType=FindAttrib(XEAB::FRAME,XEnumFrame::BOX);
  XU8 bCollapse=pDraw->bCollapse;
  pDraw->bCollapse=FindAttrib(XEAB::BORDER_COLLAPSE,0);
  int nr=pDraw->SPACING;
  pDraw->SPACING=XMAX(FindAttrib(XEAB::CELLSPACING,1),0);
  XU8 d=pDraw->TABBORDER;
  pDraw->TABBORDER=bd;

  XRect ar=XRect(rect.left,rect.top,rect.right,rect.bottom);
  
  PaintBack(pDraw,ar,XTRUE);
  
  XU32 s=HandleChild(XDO_PAINT_TABLE,(XU32)pDraw,0);
  pDraw->bCollapse=bCollapse;
  pDraw->TABBORDER=d;
  if(nType==XEF::VOID_X) bd=0;

  if(bd>0)
  {
	  XGraphics*pg=pDraw->pDraw;
	  //pDraw->Save();
	  XColor cc(pDraw->DCBACK);
	  XColor dc(cc);
	  dc.Dark(60);
	  cc.Dark(30);
	  
	  int bx=m_nPosX,by=m_nPosY;
	  int ex=m_nPosX+m_nWidth-1,ey=m_nPosY+m_nHeight-1;
	  for(int i=0;i<bd;i++)
	  {
		pg->SetColor(cc);
		switch(nType)
		{
		case XEF::BOX:
		case XEF::ABOVE:
		case XEF::BORDER:
		case XEF::HSIDES:
			 pg->DrawLine(bx,by,ex,by);
			 break;
		}
		switch(nType)
		{
		case XEF::BORDER:
		case XEF::BOX:
		case XEF::VSIDES:
		case XEF::LHS:
			 pg->DrawLine(bx,by,bx,ey);
			 break;
		}
		pg->SetColor(dc);
		switch(nType)
		{
		case XEF::BORDER:
		case XEF::BOX:
		case XEF::VSIDES:
		case XEF::RHS:
			 pg->DrawLine(ex,by,ex,ey);
			 break;
		}
		switch(nType)
		{
		case XEF::BORDER:
		case XEF::BOX:
		case XEF::HSIDES:
		case XEF::BELOW:
			 pg->DrawLine(bx,ey,ex,ey);
			 break;
		}
		cc.Bright(3);
		dc.Bright(3);
		bx++;
		by++;
		ex--;
		ey--;
	  }

	  //cc.Dark(20);
	  /*pDraw->pDraw->DrawFrame(
		  XRect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight),
		  cc,bd,XTRUE);*/
//	  pDraw->Restore();
  }
  pDraw->SPACING=nr;
  PaintBorder(pDraw,ar);
  EndPaint(pDraw);
  //if(bSave) 
	//  pDraw->Restore();

  return s;
}


void XDomTable::LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData)
{

   if(m_nWidth<=0) return;

   int aw=pDraw->ALIGNW;
   pDraw->ALIGNW=XEA::LEFT;
   LAYEROUTDATA margin;
   CELLDATA data;
   data.Reset(pData->cur.x,pData->cur.y,XTRUE);
   PreLayerout(pDraw,&data,&margin);
   data.pData=XNULL;
   pDraw->bCollapse=FindAttrib(XEAB::BORDER_COLLAPSE,0);
   //if(!pData->bReturn)
   //if(m_bReturn)
   XU8 bInline=IsFingerInline();


   //if(pDraw->SETWIDTH==-92)
   //   int a=0;

   if(!bInline)//&&m_bReturn)
   {
	  AlignRow(pDraw,pData);
	  NewRowMust(pDraw,pData,m_bReturn);
   }
   //else 
	  

   m_nPosX=pData->cur.x;//+margin.nMargin[BORDERDATA::LEFT];
   m_nPosY=pData->cur.y;//+margin.nMargin[BORDERDATA::TOP];

   int bd=FindAttrib(XEAB::BORDER,0);
   int cs=pDraw->PADDING;//XMAX(FindAttrib(XEAB::CELLPADDING,1),0);
   int cp=pDraw->SPACING;//XMAX(FindAttrib(XEAB::CELLSPACING,1),0);
   /*int bdx=margin.nMargin[BORDERDATA::LEFT]+
	       margin.nPadding[BORDERDATA::LEFT]+
		   margin.nBorderWidth[BORDERDATA::LEFT];
   int bdy=margin.nMargin[BORDERDATA::RIGHT]+
	       margin.nPadding[BORDERDATA::RIGHT]+
		   margin.nBorderWidth[BORDERDATA::RIGHT];*/
   
   XU8 bCollapse=FindAttrib(XEAB::BORDER_COLLAPSE,0);
   if(bCollapse)
	 bd+=2;
   //XU8 nType=FindAttrib(XEAB::FRAME,XEnumFrame::BOX);
   //if(nType==XEF::VOID_X) bd=0;
   
   //
   XRect rt=pDraw->win;
   //if(bd>0)
   {
	pDraw->win.left=m_nPosX+bd;
	pDraw->win.right=m_nPosX+m_nWidth-(bd+cs);
   }
   //else pDraw->win.right--;

   //if(m_bFinal&&m_nWidth==pDraw->win.Width())
   data.rowws.DataFrom(m_rows);

   data.Reset(pData->cur.x+bd,pData->cur.y+bd,XFALSE);
   //if(margin.nBorderWidth[0]&&m_nPosX==3)
   	 //  int a=0;
   
   InitMargin(pDraw,&data,&margin,m_nPosX,m_nPosY,m_nWidth,m_nHeight,XTRUE);
   m_nPosX-=bd;
   m_nPosY-=bd;

   HandleChild(XDO_LAYEROUT_TAB,(XU32)pDraw,(XU32)&data);
   XSize size(m_nWidth,m_nHeight);
   SetMargin(&margin,m_nWidth,m_nHeight,size);
   m_rows.DataFrom(data.rowws);

  // pDraw->PADDING=nPad;
  // pDraw->SPACING=nSpa;
   pDraw->win=rt; 
  // pData->alignw=aw;
   //data.max.cx-=pData->cur.x;
   //data.max.cy-=pData->cur.y;
   EndLayerout(pDraw,pData);
   pDraw->ALIGNW=aw;
   SetRowSize(pDraw,pData,size,data.nMin,XFALSE);
   NewRow(0,pDraw,pData,pDraw->IND);
   
   //pDraw->win=rect;
}
		
void XDomTable::LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData)
{
	//if(!pData->bReturn)
	//pDraw->PADDING=1;
    //pDraw->SPACING=1;
	LAYEROUTDATA margin; 
	CELLDATA data;
	data.Reset(pData->cur.x,pData->cur.y,XTRUE);

	

	PreLayerout(pDraw,&data,&margin);
	//XU8 bCollapse=pDraw->bCollapse;
	pDraw->bCollapse=FindAttrib(XEAB::BORDER_COLLAPSE,0);
	data.pData=XNULL;

	XU8 bInline=IsFingerInline();
	XRect rt=pDraw->win;
	InitMargin(pDraw,&data,&margin,m_nPosX,m_nPosY,m_nWidth,m_nHeight,XTRUE);
	if(m_nWidth>0&&m_nWidth==pDraw->win.Width()&&m_bFinal)
	{
		//bSet=XTRUE;
		data.max.cx=m_nWidth;
		data.max.cy=m_nHeight;
		data.nMin=m_nMin;
	}
	else
	{
		//m_nWin=pDraw->win.Width();
		
		LayeroutTable(pDraw,data);
		
		m_rows.DataFrom(data.rowws);
		m_nMin=data.nMin;
	}
	XSize size(m_nWidth,m_nHeight);
	SetMargin(&margin,m_nWidth,m_nHeight,size);
	//SetMargin(&margin,m_nWidth,m_nHeight,size);
	pDraw->win=rt;
//	XU8 bInline=IsFingerInline();
	if(!bInline)
		m_bReturn=NewRowIfMust(pDraw,pData,m_nWidth);
	SetRowSize(pDraw,pData,size,data.nMin,XTRUE);
	if(!bInline) 
		NewRow(0,pDraw,pData,pDraw->IND);
	//pDraw->bCollapse=
	EndLayerout(pDraw,pData);
	//pDraw->win=rect;
}


void XDomTable::LayeroutTable(DRAWCONTEXT *pDraw, CELLDATA &data)
{
	XU32 i;
	int bd=FindAttrib(XEAB::BORDER,0);
	int cp=pDraw->SPACING;//XMAX(FindAttrib(XEAB::CELLSPACING,1),0);
	int wd=pDraw->SETWIDTH;//FindAttrib(XEAB::WIDTH,0);

	//if(wd==900)
	//	int a=0;
	//int wmm=wd;
	if(wd<0)
		wd=-pDraw->win.Width()*wd/100;

	//if(wd==660)
	 //  int a=0;

	XRect win=pDraw->win;
	int cs=pDraw->SPACING;//XMAX(FindAttrib(XEAB::CELLPADDING,1),0);//cellspacing



    XU8 nType=FindAttrib(XEAB::FRAME,XEnumFrame::BOX);
    //if(nType==XEF::VOID_X) bd=0;
	//XU8 nPad=pDraw->PADDING;
	//XU8 nSpa=pDraw->SPACING;
	//if(bd) cs++;
	{
		pDraw->SPACING=cp;
		if(bd) pDraw->SPACING+=2;
		pDraw->PADDING=cs;
	}
	if(wd>0) 
	{
		wd-=(bd<<1)+cp;
		pDraw->win.left+=bd;
		pDraw->win.right=pDraw->win.left+wd;
	}
	else	 
	{
		pDraw->win.left+=bd;
		pDraw->win.right-=bd+cp;
	}
	XPoint pt(pDraw->win.left,pDraw->win.top);
	//if(0)
	{
		
		HandleChild(XDO_LAYEROUT_TABPRE,(XU32)pDraw,(XU32)&data);
		XU32 nc=data.setCols.GetSize();
		data.Reset(pt.x,pt.y,XFALSE);

		if(nc>0) 
		{
			int pw=0,ww=pDraw->win.Width(),nsc=0,nfc=0;
			int tw=ww,ssw=0;
			for(i=0;i<nc;i++)
			{
				if(data.setCols[i]>0) 
				{
					nfc++;
					tw-=data.setCols[i];
					ssw+=data.setCols[i];
				}
				else if(data.setCols[i]<0)
					pw+=data.setCols[i];
				else nsc++;
			}
			//if((nsc+nfc)<=0&&pw>-100&&pw<0)
			//{
				
				//ww=ww*-100/pw;
			//}
			
			tw=tw*(100+pw)/100;
			if(pw!=0&&nsc>1) tw=tw/nsc;
			if(tw<=0) tw=ww/nc;
			if(nsc+nfc>0) pw=-100;
			//if(nsc<=0&&pw>-100)
			//	ww=ww*100/pw;

			for(i=0;i<nc;i++)
			{
				if(data.setCols[i]>0)
				{
					if(ssw>wd)
					{
						data.setCols[i]=data.setCols[i]*wd/ssw;
						//XVar var(XEAB::WIDTH,data.setCols[i]);

					}
					continue;
				}
				else if(data.setCols[i]<0)
				{
					data.setCols[i]=data.setCols[i]*ww/pw;
				}
				else
					data.setCols[i]=tw;
			}
		}
		//data.tabs.InsertAt(0,(XU8)0,data.fixCols.GetSize());
		
	}


	//else pDraw->win.right--;
	//XString8 str=FindAttrib(XEAB::ID,"");
	//if(str=="100")
	//	int a=0;
	//else if(m_nWidth>0) pDraw->win.right=pDraw->win.left+wd;

	HandleChild(XDO_LAYEROUT_TABS,(XU32)pDraw,(XU32)&data);
	MoreRowSpan(&data);
	data.max.cx=0;	
	//data.max.cy-=my;
	data.nMin=0;
	int nFixed=0;
	for(i=0;i<data.cols.GetSize();i++)
	{
		if(data.tabs[i])
			nFixed+=data.fixCols[i];
		else
			nFixed+=data.nMin;

		data.nMin+=data.cols[i];
		data.max.cx+=data.fixCols[i];
	}

	//if(data.max.cx==905)
	//	int a=0;

	int mx=pDraw->win.Width();
	int nMin=data.nMin;
	XBOOL bReset=XFALSE;
	if(data.max.cx<wd)
	{
		bReset=XTRUE;
		int nTotal=0,nLeft=wd,nLast=wd;
		for(i=0;i<data.tabs.GetSize();i++)
		{
			if(data.tabs[i]) //continue;
				 nLeft-=data.fixCols[i];
			else nTotal+=data.fixCols[i];
		}
		if(nTotal==0)
		{
			for(i=0;i<data.tabs.GetSize();i++)
			{
				if(i+1==data.fixCols.GetSize())
					data.fixCols[i]=nLast;
				else
				{
					if(data.max.cx<=0)
						data.fixCols[i]=wd/data.tabs.GetSize();
					else
						data.fixCols[i]=data.fixCols[i]*wd/data.max.cx;
					nLast-=data.fixCols[i];
				}
			}
		}
		else
		{
			for(i=0;i<data.tabs.GetSize();i++)
			{
				if(i+1==data.fixCols.GetSize())
					data.fixCols[i]=nLast;
				else
				{
					if(!data.tabs[i]) //continue;
						data.fixCols[i]=data.fixCols[i]*nLeft/nTotal;
					nLast-=data.fixCols[i];
				}
			}
		}
		//if(wmm>0)
		data.max.cx=wd;
		//else
		//	data.max.cx=
	}
	else if(data.max.cx>mx)//&&m_bNoChild)
	{
		/*if(nFixed<mx)
		{
			int nLeft=mx-nFixed;
			for(i=0;i<data.cols.GetSize();i++)
			{
				if(data.tabs[i]) continue;
				int dd=XMAX(data.fixCols[i]-data.cols[i],0);
				data.fixCols[i]=data.cols[i]+nLeft*dd/(data.max.cx-nFixed);
			}
			data.max.cx=mx;
		}
		else*/ if(data.nMin<mx)
		{
			int nLeft=mx-data.nMin;
			for(i=0;i<data.cols.GetSize();i++)
			{
				int dd=XMAX(data.fixCols[i]-data.cols[i],0);
				data.fixCols[i]=data.cols[i]+nLeft*dd/(data.max.cx-data.nMin);
			}
			data.max.cx=mx;
		}
		else
		{
			for(i=0;i<data.cols.GetSize();i++)
				data.fixCols[i]=data.cols[i];
			data.max.cx=data.nMin;
		}
		bReset=XTRUE;
	}
	//if(bReset)
	{
		int maxx=data.max.cx;
		data.Reset(pt.x,pt.y,XFALSE);
		//data.rowws.RemoveAll(XFALSE);
		//data.cols.RemoveAll(XFALSE);
		//data.fixCols.RemoveAll(XFALSE);
		
		data.bFinal=XTRUE;
		for(i=0;i<data.rowws.GetSize();i++)
			data.rowws[i]=0;
		HandleChild(XDO_LAYEROUT_TABS,(XU32)pDraw,(XU32)&data);
		MoreRowSpan(&data);
		data.max.cx=maxx;
		//data.max.cy-=my;
		data.nMin=nMin;
	}

	int ds=(bd<<1)+cp;
	//if(bd>0)
	{
		//bd--;
		data.max.cx+=ds;//bd<<1;
		//data.max.cy+=ds;//bd<<1;
		data.nMin+=ds;//bd<<1;
	}

	m_nHeight=ds;
	for(i=0;i<data.rowws.GetSize();i++)
		m_nHeight+=data.rowws[i];
	int sh=pDraw->SETHEIGHT;//FindAttrib(XEAB::HEIGHT,0);
	/*if(m_nHeight<sh)
	{
		for(i=0;i<data.rowws.GetSize();i++)
		{
			if(m_nHeight>0)
				data.rowws[i]=data.rowws[i]*sh/m_nHeight;
			else
				data.rowws[i]=sh/data.rowws.GetSize();
		}
	} &*/
	m_nHeight=XMAX(m_nHeight,sh);
//	pDraw->win=rt;
	m_nMin=data.nMin;
	m_nWidth=data.max.cx;
	//m_nHeight=data.max.cy;

	//pDraw->PADDING=nPad;
	//pDraw->SPACING=nSpa;
	pDraw->win=win;

}


void XDomTable::MoreRowSpan(CELLDATA*pData)
{
	int id=pData->rowws.GetSize()-1;
	if(id<0) return;
	int h=pData->rowws[id];
	for(XU32 i=0;i<pData->spans.GetSize();i+=5)
	{
		int vh=h+pData->spans[3];
		if(pData->rowws[id]<vh)
			pData->rowws[id]=vh;
	}
}

//////////////////////////////////////////////////////////////////////////////
XU32 XDomTD::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
		case XDO_GET_RECT:
		 if(IsVisiable()&&m_nWidth>0)
		 {
			XRect*pr=(XRect*)pData1;
			*pr=XRect(m_nPosX,m_nPosY,
					  m_nPosX+m_nWidth,m_nPosY+m_nHeight);
			//rt.InflateRect(2,2);
			
			return XTRUE;
		 }break;

	case XDO_PAINT:return XDomNode::Handle(nOpera,pData1,pData2);
	case XDO_LAYEROUT_TABPRE:
		 return LayeroutPre((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
	case XDO_LAYEROUT_TAB:
		 LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
	case XDO_LAYEROUT_TABS:
		 LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
	case XDO_PAINT_TABLE:
		 if(!IsVisiable()) return 0;
		 return Paint((DRAWCONTEXT*)pData1,pData2);
	}
	return XDomBlock::Handle(nOpera,pData1,pData2);
}

void XDomTD::LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData,int span)
{		
 

   SpanCol(pData);
   //if(pDraw->win.Width()<6) return;
   if(pData->nRow>=(XINT)pData->rowws.GetSize()) return;
   XU8 ah=pDraw->ALIGNH;
   pDraw->ALIGNH=XEA::MIDDLE;
   LAYEROUTDATA margin;	
   CELLDATA data;
   data.Reset(pData->cur.x,pData->cur.y,XTRUE);
   PreLayerout(pDraw,&data,&margin);
   data.pData=XNULL;
   XRect rect=pDraw->win;
   
   m_nPosX=pData->cur.x;
   m_nPosY=pData->cur.y;


 //  if(pDraw->SETHEIGHT==180)
//	   int a=0;
   int rspan=pDraw->DROWSPAN;//XMAX(FindAttrib(XEAB::ROWSPAN,1),1);
   int cspan=span==0?pDraw->DCOLSPAN:span;//XMAX(FindAttrib(XEAB::COLSPAN,1),1):span;
   int h=pData->rowws[pData->nRow];

   for(int i=1;i<rspan;i++)
   {
	   if(pData->nRow+i>=(int)pData->rowws.GetSize()) break;
	   h+=pData->rowws[pData->nRow+i];
   }
   if(h>m_nHeight) m_nHeight=h;
   InitMargin(pDraw,&data,&margin,m_nPosX,m_nPosY,m_nWidth,m_nHeight,XFALSE);
   //if(pDraw->bCollapse)
   //{data.cur.y+=2;m_nPosY+=2;}

   switch(pDraw->ALIGNH)
   {
	 case XEA::MIDDLE:data.cur.y+=(m_nHeight-m_nFixHeight)>>1;break;
	 case XEA::BOTTOM:data.cur.y+=(m_nHeight-m_nFixHeight);break;
   }
  
   //data.cur.y=AlignCol(pDraw,pData,m_nFixHeight+(pDraw->PADDING<<1),pDraw->ALIGNH,m_nHeight)+
   //		  pDraw->SPACING;//pDraw->nRowSpace;
   //int px=data.cur.x;//+dd;//pDraw->nColSpace;
   //data.cur.x=px;
   //data.cur.y=py;

   //data.bReturn=XTRUE;
   
   //InitMargin(pDraw,
   InitSize(pDraw,&data,m_nWidth,XTRUE);
	//   XDomNode::Handle(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)&data);
   
   //
//   data.Reset(px,py,data.alignw,data.alignh,data.nowrap);
//   data.bReturn=XTRUE;
 //  AlignRow(pDraw,&data);
   NewRow(0,pDraw,&data,0);
   data.bEmpty=XTRUE;
   if(pDraw->bCollapse)
   {
	   data.cur.y+=2;
	   data.cur.x+=2;
	   //m_nWidth+=2;
	   //m_nHeight+=2;
   }
   HandleChild(XDO_LAYEROUT_CELL,(XU32)pDraw,(XU32)&data);
   XSize size(m_nWidth,m_nHeight);
   SetMargin(&margin,m_nWidth,m_nHeight,size);
   /*if(pData->bCollapse)
   {
	   m_nWidth+=2;
	   m_nHeight+=2;
   }*/
   pDraw->win=rect;
   
   SaveData(&data);

   EndLayerout(pDraw,&data);
   pDraw->ALIGNH=ah;
   //int rspan=FindAttrib(XEAB::ROWSPAN,1);
   if(rspan<=1)
		SetTabRow(pDraw,pData,size.cx,size.cy);//m_nWidth,m_nHeight);//XSize(m_nWidth,m_nHeight),m_nMin,XFALSE);
   //else
   else //if(rspan>1)
   {
	   	SetTabRow(pDraw,pData,size.cx,pDraw->SPACE);//XSize(m_nWidth,pDraw->nRowSpace),m_nMin,XFALSE);
		pData->spans.Add(rspan);
		pData->spans.Add(pData->nCol);
		pData->spans.Add(m_nWidth);
		pData->spans.Add(m_nFixHeight);
		pData->spans.Add(cspan);
   }

   //SetRow(pDraw,pData,XSize(m_nWidth,m_nHeight),data.nMin,XFALSE,XFALSE);
   //pData->cur.x+=m_nWidth;////pData->fixCols[pData->nCol];
   pData->nCol+=cspan;//=cspan;
   
   //if(pData->max.cx<m_nPosX+m_nWidth)
	//   pData->max.cx=m_nPosX+m_nWidth;
   //if(pData->max.cy<m_nPosY+m_nHeight)
	//   pData->max.cy=m_nPosY+m_nHeight;
}

XU32 XDomTD::LayeroutPre(DRAWCONTEXT*pDraw,CELLDATA*pData)
{
	XINT i;
	SpanCol(pData);
	//LAYEROUTDATA margin;
	//PreLayerout(pDraw,pData,margin);	
//	if(pDraw->SETWIDTH==-84)
//	   int a=0;
	int w=FindAttrib(XEAB::WIDTH,0);
	int cspan=XMAX(FindAttrib(XEAB::COLSPAN,1),1);
	int rspan=XMAX(FindAttrib(XEAB::ROWSPAN,1),1);
	/*if(w==0)
	{
		if(m_childs.GetSize()>0)
			pData->setCols.Add(0);
	} */
	//else
	if(w)
	{
		for(i=pData->setCols.GetSize();i<pData->nCol;i++)
			pData->setCols.Add(0);
		for(i=0;i<cspan;i++)
		{
		   XU16 id=pData->nCol+i;
		   if(id>=pData->setCols.GetSize())
		   {
			   pData->setCols.Add(w/(cspan-i));
			   w-=w/(cspan-i);
		   }
		   else if(i+1<cspan)
		   {
			   if(w>0)
			   {
				   if(pData->setCols[id]>=0)
					 w=XMAX(w-pData->setCols[id],0);
				   else w-=w/(cspan-i);
			   }
			   else
			   {
				   if(pData->setCols[id]<0)
					   w=XMIN(w-pData->setCols[id],0);
				   else w-=w/(cspan-i);
			   }
		   }
		   else
		   {
			   if(w>0)
			   {
					if(pData->setCols[id]<w)
					 pData->setCols[id]=w;
			   }
			   else if(pData->setCols[id]>w)
				   pData->setCols[id]=w;

		   }
		}
	}
//	else if(m_childs.GetSize()>0)

	pData->nCol+=cspan;
	if(rspan>1)
	{
		pData->spans.Add(rspan);
		pData->spans.Add(pData->nCol);
		pData->spans.Add(0);
		pData->spans.Add(0);
		pData->spans.Add(cspan);
   }
   //EndLayerout(pDraw,pData);
   return 0;
}
		
void XDomTD::LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData,int span)
{
   CELLDATA data;  
   LAYEROUTDATA margin;
   SpanCol(pData); 
   data.Reset(pData->cur.x,pData->cur.y,XTRUE);
   PreLayerout(pDraw,&data,&margin);
   data.pData=XNULL;
   int cspan=//pDraw->DCOLSPAN;//
	        //span==0?XMAX(FindAttrib(XEAB::COLSPAN,1),1):span;
			span==0?pDraw->DCOLSPAN:span;
   int rspan=pDraw->DROWSPAN;//
             //XMAX(FindAttrib(XEAB::ROWSPAN,1),1);
	//for debug 
   
   XRect rw=pDraw->win;
   
   

  // if(pDraw->SETWIDTH==663)
//	   int a=0;

   XSize sz;
   if(pData->bFinal)
   {
	   
	   int w=pData->fixCols[pData->nCol];
	   for(int i=1;i<cspan;i++)
	   {
		   if(pData->nCol+i>=(int)pData->fixCols.GetSize()) break;
		   w+=pData->fixCols[pData->nCol+i];
	   }
	   InitMargin(pDraw,&data,&margin,m_nPosX,m_nPosY,w,m_nHeight);
	   XPoint pt=data.cur;
	   if(InitSize(pDraw,&data,w,XFALSE))
	   	 m_nFixHeight=data.max.cy-pt.y;
	   //XSize sz;
	   sz=XSize(m_nWidth,m_nHeight);
	   SetMargin(&margin,m_nWidth,m_nHeight,sz);
	   SaveData(&data);
	   //pDraw->win=rt;
	   //m_nFixHeight=m_nHeight;
   }
   else
   {
	   //data.Reset(pData->cur.x,pData->cur.y,XTRUE);
	   
	   int w=0;
	   //if(pData->nCol+cspan<(XINT)pData->setCols.GetSize())
		{
			//w=pData->setCols[pData->nCol];
			for(int i=0;i<cspan;i++)
			{
				if(pData->nCol+i>=(int)pData->setCols.GetSize()) break;
				w+=pData->setCols[pData->nCol+i];
			}
			//int w1=0;
			/*for(int i=0;i<cspan;i++)
			{
				if(pData->nCol+i>=(int)pData->cols[i]
			}*/
		}

	   InitMargin(pDraw,&data,&margin,m_nPosX,m_nPosY,w,m_nHeight);
	   XPoint pt=data.cur;

	   XU8 bFix=InitFixed(pDraw,&data,w);

	   sz=XSize(m_nWidth,m_nHeight);
	   SetMargin(&margin,m_nWidth,m_nHeight,sz);
	   //pDraw->win=rt;
	   //m_nHeight=XMAX(m_nHeight,pDraw->SETHEIGHT);//m_nHeight;//XMAX(m_nHeight,pDraw->SPACE);
	   //w=m_nWidth;
	   if(bFix)
	   {
		    m_nFixWidth=m_nWidth;
			m_nFixHeight=data.max.cy-pt.y;
	   }
	   if(span==0)
	   {
		   int nMin=m_nMin;
		   //if(bFix) m_nFixWidth=data.max.cx-
		   w=m_nFixWidth;
		   for(int i=0;i<cspan;i++)
		   {
			   XU16 id=pData->nCol+i;
			   
			   if(id>=pData->cols.GetSize())
			   {
				   pData->tabs.Add(bFix==1);
				   pData->cols.Add(nMin/(cspan-i));
				   pData->fixCols.Add(w/(cspan-i));
				   nMin-=nMin/(cspan-i);
				   w-=w/(cspan-i);
				   //break;
			   }
			   else if(i+1<cspan)
			   {
				   pData->tabs[id]|=bFix;
				   nMin=XMAX(nMin-pData->cols[id],0);
				   w=XMAX(w-pData->fixCols[id],0);
			   }
			   else
			   {
				   pData->tabs[id]|=bFix;
				   if(pData->cols[id]<nMin)
			   		   pData->cols[id]=nMin;
				   if(pData->fixCols[id]<w)
			   		   pData->fixCols[id]=w;
			   }
		   }
	   }
   }
   //m_rows.DataFrom(data.rowws);
   //m_cols.DataFrom(data.fixCols);
   //XSize size(m_nWidth,m_nHeight);	
   //SetMargin(&margin,m_nWidth,m_nHeight,size);
   /*	XString8 str=FindAttrib(XEAB::ID,"");
	if(str=="999")
		int a=0;*/
   if(pDraw->bCollapse)
   {
	   //m_nWidth-=2;
	   //m_nHeight-=2;
	   //sz.cx-=2;
	   sz.cy-=2;
   } //*///*/

   if(rspan<=1)
		SetTabRow(pDraw,pData,sz.cx,sz.cy);//m_nWidth,m_nHeight);//XSize(m_nWidth,m_nHeight),m_nMin,XTRUE);
   else
   {
	   	SetTabRow(pDraw,pData,sz.cx,pDraw->SPACE);//XSize(m_nWidth,pDraw->nRowSpace),m_nMin,XTRUE);
		pData->spans.Add(rspan);
		pData->spans.Add(pData->nCol);
		pData->spans.Add(m_nWidth);
		pData->spans.Add(m_nFixHeight);
		pData->spans.Add(cspan);
   }
   pDraw->win=rw;
   
   pData->nCol+=cspan;
   EndLayerout(pDraw,&data);
}

/*XU8 XDomTD::InitFixed(DRAWCONTEXT*pDraw,CELLDATA*pData,int ws)
{


	if(m_bFinal&&m_nWidth>0&&m_nWidth==pDraw->win.Width())
	{
		pData->rowws.DataFrom(m_rows);
		pData->fixCols.DataFrom(m_cols);
		return XTRUE;
	}


//	XDomList list,list1;
//	GetList(
	
	int w=FindAttrib(XEAB::WIDTH,0);
	int wm=w;
	//if(ws!=0) w=ws;
	if(w<0) 
		w=(pDraw->win.Width()*(-w))/100;

	XRect rt=pDraw->win;
	//int dd=(pDraw->PADDING+pDraw->SPACING);//<<1;
	int ds=(pDraw->PADDING<<1)+pDraw->SPACING;
	int dl=pDraw->PADDING+pDraw->SPACING;
	if(w>0&&w<m_nMin) 
		w=m_nMin;
	pData->cur.x+=dl;
	pData->cur.y+=dl;
	if(ws<=0) ws=pDraw->win.Width();
	//int wc=pDraw->win.Width();
	pDraw->win.left=pData->cur.x;//+=dl;
	if(w>0)
		pDraw->win.right=pDraw->win.left+w-ds;
	else
	{
		//if(ws>0)
		pDraw->win.right=pDraw->win.left+ws-ds;
		//else
			//pDraw->win.right-=pDraw->PADDING;
	}

	if(m_bFinal&&m_nWidth>0&&m_nWidth==pDraw->win.Width())
	{
		pData->rowws.DataFrom(m_rows);
		pData->fixCols.DataFrom(m_cols);
		return XTRUE;
	}



	XPoint pt=pData->cur;
	XDomNode::Handle(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)pData);

	int h=FindAttrib(XEAB::HEIGHT,0);
	
	m_nWidth=XMAX(pData->max.cx-pt.x+ds,w);
	m_nHeight=pData->max.cy-pt.y+ds;
	m_nMin=XMAX(pData->nMin+ds,wm);
	m_nFixWidth=m_nWidth;//XMAX(pData->max.cx-pt.x,w)+(dd<<1);
	m_nFixHeight=XMAX(m_nHeight,h);
	
	XU8 bFix=(w!=0)&&m_nWidth>=w;

	return bFix;
	/*if((XINT)p->tabs.GetSize()>p->nCol)
		p->tabs[pData->nCol]=p->tabs[pData->nCol]|bFix;
	else
		p->tabs.Add(bFix);
	pDraw->win=rt;
	//if(w&&m_nFixWidth
}

void XDomTD::InitSize(DRAWCONTEXT*pDraw,CELLDATA*pData,int nWidth,XU8 bSet)
{

	int ds=(pDraw->PADDING<<1)+pDraw->SPACING;
	int dl=pDraw->PADDING+pDraw->SPACING;
	pData->cur.x+=dl;
	pData->cur.y+=dl;
	pDraw->win.left=pData->cur.x;
	pDraw->win.right=pDraw->win.left+nWidth-ds;
	
	if(pDraw->win.Width()<=4) 
		return;

	
	if((m_bFinal&&m_nWidth>0&&m_nWidth==nWidth)||bSet)
	{
		//if(!bSet)
		{
			//if(m_rows.GetSize()<=0)
			//	int a=0;
			pData->rowws.DataFrom(m_rows);
			pData->fixCols.DataFrom(m_cols);
			return;
		}
	}

	
	
	//pDraw->win.left+=pDraw->PADDING+pDraw->SPACING;
	

	//pData->cur.x+=pDraw->nRowSpace;
	

	m_nWidth=nWidth;//pDraw->win.Width()+(pDraw->nColSpace<<1);
	
	XPoint pt=pData->cur;
	XDomNode::Handle(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)pData);
//	int h=FindAttrib(XEAB::HEIGHT,0);
//	int dd=pDraw->nRowSpace+pDraw->nColSpace;

	m_nHeight=pData->max.cy-pt.y+ds;


	int h=FindAttrib(XEAB::HEIGHT,0);
	
	//m_nFixWidth=XMAX(pData->max.cx-pt.x,w)+(dd<<1);
	m_nFixHeight=XMAX(m_nHeight,h);


	//m_nHeight=XMAX(m_nHeight,h);
	//if(m_nFixHeight<m_nHeight)
	//	m_nFixHeight=m_nHeight;
	//m_nFixHeight=XMAX(pData->max.cy-pt.y,h)+(pDraw->nRowSpace<<1);
}  */

XBOOL XDomTD::Paint(DRAWCONTEXT *pDraw,XU32 nData)
{
	if(m_nWidth<=0) 
		return 0;
	
	XRect rect(m_nPosX+pDraw->SPACING,m_nPosY+pDraw->SPACING,
				m_nPosX+m_nWidth-1,m_nPosY+m_nHeight-1);
	if(pDraw->bCollapse)
		rect.InflateRect(2,2);
	if(rect.IsUnion(pDraw->paint))
	{
//		pDraw->Save();
	//	XRect ra(m_nPosX,m_nPosY,m_nPosX
		if(pDraw->bCollapse)
		{	rect.DeflateRect(1,1);
			/*rect.InflateRect(1,1);
			rect.top--;
			rect.left--;
			rect.right++;
			rect.bottom++;*/
		}
		PaintBack(pDraw,rect);
		
		//	rect.InflateRect(1,1);
		XDomNode::Handle(XDO_PAINT,(XU32)pDraw,0);
		if(pDraw->TABBORDER)
		{
			XColor color(pDraw->DCBACK);
			color.Dark(30);
			XU32 nColor=pDraw->pDraw->SetColor(color);
			pDraw->pDraw->FrameRectangle(rect);
			pDraw->pDraw->SetColor(nColor);
		}
		if(pDraw->bCollapse)
		{
			//rect.top--;
			rect.right++;
			//rect.bottom++;
		}
		PaintBorder(pDraw,rect);
		//if(bSet) 
	//	pDraw->Restore();
		return XTRUE;
	}
	return XFALSE;
}


////////////////////////////////////////////////////////////////////////////

XU32 XDomTR::Handle(XU32 nOpera, XU32 pData1,XU32 pData2)
{
    switch(nOpera)
	{
	case XDO_LAYEROUT_TABPRE:
		 return LayeroutPre((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
	case XDO_LAYEROUT_TAB:
		 LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
	case XDO_LAYEROUT_TABS:
		 LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
	case XDO_PAINT_TABLE:
		{
			//DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			//XU32 c=FindAttrib(XEAB::BGCOLOR,0);
			//if(c)
			//	c=pDraw->pDraw->SetBackColor(c);
			//XU32 ss=PrePaint((DRAWCONTEXT*)p
			XU32 s=XDomNode::Handle(nOpera,pData1,pData2);
			//if(c)
			//	c=pDraw->pDraw->SetBackColor(c);
			return s;
		}break;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

void XDomTR::LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData)
{
   //PreLayerout(pDraw,pData);
   //XU8 aw=pData->alignw;
   //XU8 ah=pData->alignh;
   //pData->alignw=FindAttrib(XEAB::ALIGN,aw);
   //pData->alignh=FindAttrib(XEAB::VALIGN,ah);

   pData->nCol=0;
   XDomNode::Handle(XDO_LAYEROUT_TAB,(XU32)pDraw,(XU32)pData);
   RowSpan(pDraw,pData,XFALSE);
   pData->cur.x=pDraw->win.left;
   if(!pData->bEmpty)
	   pData->cur.y+=pData->rowws[pData->nRow];	
   pData->nRow++;
   pData->nCol=0;
   //pData->alignh=ah;
   //pData->alignw=aw;
}
		

XU32 XDomTR::LayeroutPre(DRAWCONTEXT*pDraw,CELLDATA*pData)
{
//   XU8 aw=pData->alignw;
//   XU8 ah=pData->alignh;
//   pData->alignw=FindAttrib(XEAB::ALIGN,aw);
//   pData->alignh=FindAttrib(XEAB::VALIGN,ah);
   pData->nCol=0;
   XDomNode::Handle(XDO_LAYEROUT_TABPRE,(XU32)pDraw,(XU32)pData);
   
   	XU16 i=0;
	while(i<pData->spans.GetSize())
	{
		pData->spans[i]--;
		
		if(pData->spans[i]<=0)
			pData->spans.RemoveAt(i,5);
		else i+=5;
	}

	return 0;
	
	//pData->nCol=0;
}


void XDomTR::LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData)
{
//   XU8 aw=pData->alignw;
 //  XU8 ah=pData->alignh;
 //  pData->alignw=FindAttrib(XEAB::ALIGN,aw);
 //  pData->alignh=FindAttrib(XEAB::VALIGN,ah);
   //int r=pDraw->win.left;
   pData->nCol=0;
	/*if(!pData->bFinal)
	{

		NewRow(0,pDraw,pData,XTRUE,XTRUE);
	}*/
    NewTabRow(pDraw,pData);
	XDomNode::Handle(XDO_LAYEROUT_TABS,(XU32)pDraw,(XU32)pData);
	RowSpan(pDraw,pData,XTRUE);
	/*if(pData->bFinal&&(XINT)pData->rowws.GetSize()>pData->nRow)
	{
	  pData->cur.x=pDraw->win.left;
      pData->cur.y+=pData->rowws[pData->nRow];
	  pData->nRow++;
	}*/
   //pDraw->win.left=r;
//   pData->alignh=ah;
//   pData->alignw=aw;
	
	//pData->nCol=0;
}

void XDomTR::RowSpan(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bAdd)
{
	XU16 i=0;
	while(i<pData->spans.GetSize())
	{
		pData->spans[i]--;
		
		if(pData->spans[i]<=0)
		{
			SetTabRow(pDraw,pData,0,pData->spans[i+3]);//,0,bAdd);
			pData->spans.RemoveAt(i,5);
		}
		else
		{
			pData->spans[i+3]=XMAX(pData->spans[i+3]-pData->rowws[pData->nRow],0);
			i+=5;
		}
	}
}


//DEL void XDomTD::SetAttrib(XVar *p)
//DEL {
//DEL 	switch(p->nID)
//DEL 	{
//DEL 	case XEAB::BGCOLOR:
//DEL 		 if(p->nType==VSTRING)
//DEL 			 *p=XEnumColor().ColorOf(p->strData);
//DEL 		 break;
//DEL 	}
//DEL }

void XDomTD::SpanCol(CELLDATA *pData)
{
   for(XU32 i=0;i<pData->spans.GetSize();i+=5)
   {
	  if(pData->spans[i+1]==pData->nCol)
	  {
		  pData->cur.x+=pData->spans[i+2];
		  pData->nCol+=pData->spans[i+4];
		  break;
	  }
   }
}
