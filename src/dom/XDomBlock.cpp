// XDomBlock.cpp: implementation of the XDomBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomBlock.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


XU32 XDomBlock::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{

	case XDO_GET_RECT:
		 if(!IsVisiable()) return 0;

		 if(m_nWidth>0)
		 {
			*((XRect*)pData1)=XRect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
			return XTRUE;
		 }break;

	case XDO_HITTEST:
		 if(!IsVisiable()) return 0;		 
		 
		 if(m_nWidth<=0) break;//return 0;
		 else
			{
			  XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
			  if(!rect.PtInRect(pData1,pData2)) return 0;
			}break;
	case XDO_SET_FINAL:
		 //m_nWidth=0;
		 m_nWidth=0;
		 m_bFinal=XTRUE;
		 //m_nMaxed=0;
		 break;
	case XDO_LAYEROUT_CELL:
		 if(!IsVisiable()) return 0;
		 return LayeroutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
	case XDO_LAYEROUT_CELLS:
		 if(!IsVisiable()) return 0;
		 return LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
	case XDO_UPDATE_TABLE:
		 if(!IsVisiable()) return 0;
		 m_nMaxed=0;
		 //if(FindAttrib(XEAB::WIDTH,0)>0) return 0;
		 m_nWidth=0;
		 break;
	case XDO_PAINT:
		 if(!IsVisiable()) return 0;
		 if(m_nWidth>0)
		 {			 
			 DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			 if(pDraw->nLayerMode==LA_SAMPLE) 
				break;

			 XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
			 if(!rect.IsUnion(pDraw->paint)) return 0;
			 //XU8 bm=PrePaint(pDraw,XTRUE);
			 PrePaint(pDraw);
			 PaintBack(pDraw,rect);
			 XU32 s=HandleChild(nOpera,pData1,0);
			 PaintBorder(pDraw,rect);
			 EndPaint(pDraw);
			 return s;
			 //EndPaint(pDraw,bm,XTRUE);
			 //if(bSet)
			 //	 pDraw->Restore();
			 //return s;
		 }
		 //return 0;
	}
	return GetID()==XDOM_A?
		   XDomA::Handle(nOpera,pData1,pData2):
		   XDomNode::Handle(nOpera,pData1,pData2);
}

void XDomBlock::SaveData(CELLDATA *pData)
{
	if(pData->rowws.GetSize()<=0) return;

	m_rows.DataFrom(pData->rowws);
    m_cols.DataFrom(pData->fixCols);
	m_rows.FreeExtra();
    m_cols.FreeExtra();
}

XU8 XDomBlock::InitFixed(DRAWCONTEXT *pDraw, CELLDATA *pData,int ws)
{
//	XString8 strClass=FindAttrib(XEAB::CLASS,"");
//	if(strClass=="pic01")
//		int a=0;

	/*int ddw=GetWinWidth(pDraw,pData);
	if(m_nWidth>0&&m_nMaxed!=2)
	{
		if(m_bFinal&&m_nWidth==ddw)
		   return 0;
		else if(m_nMaxed&&ddw>m_nWidth)
			   return 0;
	} */
	int w=pDraw->SETWIDTH;//FindAttrib(XEAB::WIDTH,0);
	//if(w==73)
	//	int a=0;
 	int wm=w;
	if(w<0) 
		w=(pDraw->win.Width()*(-w))/100;
	//else if(ws>0&&w>ws) w=ws;

	int ds=(pDraw->PADDING<<1)+pDraw->SPACING;
	int dl=pDraw->PADDING+pDraw->SPACING;
	if(ws<=0) 
		ws=pDraw->win.Width();
		//GetWinWidth(pDraw,pData);
		//GetWinRight(pDraw,pData)-pData->cur.x;//GetWinWidth(pDraw,pData);//pDraw->win.Width();
	//{	//if(bReturn)
	//		ws=GetWinRight(pDraw,pData)-pData->cur.x;
	//}
	
	if(w>0&&w<m_nMin) 
		w=m_nMin;
	pData->cur.x+=dl;//+=dl;
	pData->cur.y+=dl;
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

	int ind=pDraw->IND;//FindAttrib(XEAB::TEXT_INDENT,pDraw->IND);
	int ddw=pDraw->win.Width();	 
	pData->cur.x+=ind;
	pData->max.cx+=ind;
	XPoint pt=pData->cur;
	if(m_nWidth>0&&m_nMaxed!=2)
	{
		if(m_bFinal&&m_nWidth==ddw)
		   return 0;
		else if(m_nMaxed&&ddw>m_nWidth)
			   return 0;
	}

	/*if(m_bFinal&&m_nWidth>0&&m_nWidth==pDraw->win.Width())
	{
		return 0;
	}*/

	
	HandleChild(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)pData);
	EndFloat(pDraw,pData);
	//NewFloat(pDraw,pData,FALSE);
	int h=pDraw->SETHEIGHT;//FindAttrib(XEAB::HEIGHT,0);
	
	m_nWidth=pData->max.cx-pt.x+ds;
	//if(m_nWidth>=pDraw->SPACE<<1)
	m_nWidth=XMAX(m_nWidth,(m_nWidth>(pDraw->SPACE<<1)?w:wm));
	m_nHeight=XMAX(pData->max.cy-pt.y+ds,h);
	m_nMin=XMAX(pData->nMin+ds,wm);
    //if(m_nWidth>pDraw->win.Width())
	//	int a=0;
	//m_nMaxed=(m_bFinal&&(m_nWidth<pDraw->win.Width()))?1:0;
	//if(m_nWidth>pDraw->win.Width())
	//	m_nMaxed=2;
	
	XU8 bFix=(w!=0)&&m_nWidth>=w;
	SaveData(pData);

	return bFix?1:2;


}

XU8 XDomBlock::InitSize(DRAWCONTEXT *pDraw, CELLDATA *pData, int nWidth,XU8 bInit)
{
	int ds=(pDraw->PADDING<<1)+pDraw->SPACING;
	int dl=pDraw->PADDING+pDraw->SPACING;
	pData->cur.x+=dl;
	pData->cur.y+=dl;
	pDraw->win.left=pData->cur.x;
	pDraw->win.right=pDraw->win.left+nWidth-ds;
	int ind=pDraw->IND;//FindAttrib(XEAB::TEXT_INDENT,pDraw->IND);
	//if(pDraw->win.Width()<=4) 
	//	return 0;
	
	if(bInit||
	  (m_bFinal&&m_nWidth>0&&m_nWidth==nWidth&&m_nMaxed!=2)||
	  (m_nMaxed==1&&nWidth>m_nWidth)||pDraw->win.Width()<4)
	{
			pData->rowws.DataFrom(m_rows);
			pData->fixCols.DataFrom(m_cols);
			m_nWidth=nWidth;
			//pData->cur.x+=ind;
			//pData->max.cx+=ind;
			//pData->Reset(pDat,pt.y);
			//NewRow(0,pDraw,pData,ind);
			//pData->bEmpty=XTRUE;
			return 0;
	}
	m_nWidth=nWidth;//pDraw->win.Width()+(pDraw->nColSpace<<1);

	
	pData->cur.x+=ind;
	pData->max.cx+=ind;
	XPoint pt=pData->cur;
	HandleChild(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)pData);
	EndFloat(pDraw,pData);
	m_nMin=pData->nMin;
	int h=pDraw->SETHEIGHT;//FindAttrib(XEAB::HEIGHT,0);
	m_nHeight=XMAX(pData->max.cy-pt.y+ds,h);
	pData->cur.x=pt.x;
	pData->cur.y=pt.y;
	//pData->Reset(,0,XFALSE);
	//NewRow(0,pDraw,pData,pDraw->IND);
	pData->bEmpty=XTRUE;
	

	return 1;
	//m_nFixWidth=XMAX(pData->max.cx-pt.x,w)+(dd<<1);
	//m_nFixHeight=XMAX(m_nHeight,h);
}

XU32 XDomBlock::LayeroutCell(DRAWCONTEXT*pDraw,CELLDATA*pData,XU8 bReturn)
{
   if(pDraw->nLayerMode==LA_SAMPLE) 
   {
	   NewRowMust(pDraw,pData,m_bReturn);
	   return XDomNode::Handle(XDO_LAYEROUT_CELL,(XU32)pDraw,(XU32)pData);
   }
   //if(!bReturn)
	//   int a=0;

   if(pData->nRow>=(XINT)pData->rowws.GetSize()) return 0;
   
   /*{
			 XPCTSTR str=FindAttrib(XEAB::STYLE,"");
			 if(strcmp(str,"float:left;")==0)
				 int v=0;
	}*/

   CELLDATA data;
   LAYEROUTDATA margin;
   //margin.Init();
   XRect rect=pDraw->win;
   data.Reset(pData->cur.x,pData->cur.y,XTRUE);
   PreLayerout(pDraw,&data,&margin);
   data.pData=XNULL;
   XU8 clear=pDraw->SETCLEAR;
   
   
  // if(FindAttrib(XEAB::CLASS,0)==16395)
//	   int a=0;
   
   XU8 ft=pDraw->SETFLOAT;//FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);
   if(!ft)//&&!pData->bReturn)//&&pData->cur.x+m_nWidth>pDraw->win.right)
   {
	   EndFloat(pDraw,pData);
	  if(bReturn)
		 NewRowMust(pDraw,pData,m_bReturn);
	  else if(m_bReturn)
		 NewRow(0,pDraw,pData,0);
		  //NewRowMust(pDraw,pData);
	  //
   }
   else if(m_bReturn)
	   NewFloat(pDraw,pData,ft==XELT::FLOAT_LEFT);
   m_nPosX=pData->cur.x;
   m_nPosY=pData->cur.y;
   if(ft==XELT::FLOAT_RIGHT)
	   m_nPosX=pDraw->win.right-m_nDeltaX;
	   //GetWinRight(pDraw,pData)-m_nWidth;
   else if(ft==XELT::FLOAT_LEFT)
	   m_nPosX=pDraw->win.left+m_nDeltaX;
   int ftx=m_nPosX;
	  // m_nPosX=GetWinLeft(pDraw,pData);
   data.cur.x=m_nPosX;
   data.cur.y=m_nPosY;
   InitMargin(pDraw,&data,&margin,m_nPosX,m_nPosY,m_nWidth,m_nHeight);
   if(ft) 
   {
	   m_nWidth+=m_nPosX-ftx;
	   m_nPosX=ftx;
   }
   
 //  if(pData->cur.x+m_nWidth>226)
//	   int a=0;
//   data.Reset(m_nPosX,m_nPosY,XFALSE);

//   InitMargin(pDraw,&data,&margin,m_nPosX,m_nPosY,m_nWidth,m_nHeight);
//   StartMargin(pDraw,&data,XTRUE);
//   m_nPosX=data.cur.x;
//   m_nPosY=data.cur.y;
//   StartMargin(pDraw,&data);
//   data.bReturn=XTRUE;
   // XPoint pt=data.cur;
   //InitBlock(pDraw,pData,data,XFALSE);
   InitSize(pDraw,&data,m_nWidth,XTRUE);//m_nWin);
   //data.Reset(pt.x,pt.y,XFALSE);
   //InitMargin(pDraw,&data,&margin,m_nPosX,m_nPosY,m_nWidth,m_nHeight);
   //data.Reset(data.cur.x,m_nPosY,XFALSE);
   NewRow(0,pDraw,&data,pDraw->IND);
   data.bEmpty=XTRUE;
//   XU8 ft=FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);
//   NewRow(0,pDraw,&data,pDraw->ind);
//   data.bEmpty=XTRUE;
   //AlignRow(pDraw,pData);
   data.pData=XNULL;
   HandleChild(XDO_LAYEROUT_CELL,(XU32)pDraw,(XU32)&data);
   
    
   XSize size(m_nWidth,m_nHeight);
   SetMargin(&margin,m_nWidth,m_nHeight,size);

//   if(pData->cur.x+size.cx>226)
//	   int a=0;

   SaveData(&data);
   EndLayerout(pDraw,&data);

   pDraw->win=rect;
   if(ft)
   {
	   SetFloat(pDraw,pData,size,ft==XELT::FLOAT_LEFT);
	   if(clear&XELT::FLOAT_RIGHT)
		   NewRowMust(pDraw,pData,1);
	   //if(FindAttrib(XEAB::CLEAR,0))
	  // {
	//	   NewFloat(pDraw,pData,ft==XELT::FLOAT_LEFT);
	//	   NewRow(0,pDraw,pData);
	  // }
   }
   else
   {

	  SetRowSize(pDraw,pData,size,m_nMin,XFALSE);
	  if(bReturn&&!pData->bNewLine)
		   NewRow(0,pDraw,pData,pDraw->IND);
   }
   
   //pDraw->nFixWin=nFix;
   return 1;
}

XU32 XDomBlock::LayeroutCells(DRAWCONTEXT*pDraw,CELLDATA*pData,XU8 bReturn)
{
   if(pDraw->nLayerMode==LA_SAMPLE) 
   {
	   m_bReturn=NewRowIfMust(pDraw,pData,0);
	   return XDomNode::Handle(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)pData);
   }
   

  // if(pDraw->paintData.nWidth==637)
//	   int a=0;

   CELLDATA data;
   LAYEROUTDATA margin;
  // margin.Init();	  
   XRect rw=pDraw->win;
   data.Reset(pData->cur.x,pData->cur.y,XTRUE);
   PreLayerout(pDraw,&data,&margin);
   data.pData=XNULL;
   
  // StartMargin(pDraw,&data);
//   if(FindAttrib(XEAB::CLASS,0)==16387)
//	   int a=0;

  //XString8 strClass=FindAttrib(XEAB::CLASS,"");
   //if(strClass=="content")
	//	int a=0;
   //int nFix=GetWinRight(pDraw,pData)-pData->cur.x;
   XU8 ft=pDraw->SETFLOAT;//FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);

  // if(ft)
//	 int a=0;
   int tw=GetWinWidth(pDraw,pData);

   XU8 clear=pDraw->SETCLEAR;
   int w=pDraw->SETWIDTH;//FindAttrib(XEAB::WIDTH,0);   
   if(w<0) w=-GetWinWidth(pDraw,pData)*w/100;
   if(!ft)
   {
	  //if(w==937)
	//	  int a=0;
	  EndFloat(pDraw,pData);
	  if(clear&XELT::FLOAT_LEFT)
		m_bReturn=NewRowIfMust(pDraw,pData,pDraw->win.right);
	  else if(bReturn)
  		m_bReturn=NewRowIfMust(pDraw,pData,XMIN(w,pDraw->SPACE<<2));
	  else
		m_bReturn=NewRowIfNeed(pDraw,pData,XMIN(w,pDraw->SPACE<<2));
   }
   else
   {
	 //  if(w==637)
	//	   int a=0;
	   m_bReturn=NewFloatIfNeed(pDraw,pData,XMIN(w,pDraw->SPACE<<2),ft==XELT::FLOAT_LEFT);
   }
//   data.cur.x=pData->cur.x;
 //  data.cur.y=pData->cur.y;
   data.Reset(pData->cur.x,pData->cur.y,XFALSE);
   data.pData=XNULL;
   InitMargin(pDraw,&data,&margin,m_nPosX,m_nPosY,m_nWidth,m_nHeight);
//   StartMargin(pDraw,&data);
//   data.bReturn=XTRUE;
	   
   InitFixed(pDraw,&data,bReturn?0:pDraw->win.Width());//GetWinWidth(pDraw,pData));//pDraw->win.Width());//,w,XFALSE);
   
   

   if(!ft&&w==0&&bReturn)
   {
		m_nWidth=XMAX(m_nWidth,GetWinWidth(pDraw,pData));//-margin.nMargin[1]-margin.nMargin[2]));
		//size.cx=m_nWidth;
   }
   XSize size(m_nWidth,m_nHeight);
   SetMargin(&margin,m_nWidth,m_nHeight,size);
 
//   if(pData->cur.x+size.cx>226)
//	   int a=0;
   EndLayerout(pDraw,&data);
   pDraw->win=rw;
   // 
   if(!ft)
   {

	   if(!m_bReturn)
		  m_bReturn=NewRowIfNeed(pDraw,pData,m_nWidth);
	   //m_bReturn=NewRowIfMust(pDraw,pData,m_nWidth);
	   SetRowSize(pDraw,pData,size,m_nMin,XTRUE);  
	   if(bReturn&&!pData->bNewLine)
		   NewRow(0,pDraw,pData,pDraw->IND);
   }
   else
   {
	   //m_bReturn=NewFloatIfNeed(pDraw,pData,m_nWidth,ft==XELT::FLOAT_LEFT);
	   if(ft==XELT::FLOAT_LEFT)
		   m_nDeltaX=GetWinLeft(pDraw,pData)-pDraw->win.left;//+margin.nMargin[BORDERDATA::LEFT];
	   //else
		//   m_nDeltaX=pDraw->win.right-GetWinRight(pDraw,pData);
	   SetFloat(pDraw,pData,size,ft==XELT::FLOAT_LEFT);
	   if(clear&XELT::FLOAT_RIGHT)
		   NewRowMust(pDraw,pData,1);
	   if(ft==XELT::FLOAT_RIGHT)
		   m_nDeltaX=pDraw->win.right-GetWinRight(pDraw,pData);//-margin.nMargin[BORDERDATA::RIGHT];
	   //if(ft==XELT::FLOAT_LEFT)
		//   m_nDeltaX=pData->cur.x-pDraw->win.left-m_nWidth-margin.nBorderWidth[BORDERDATA::LEFT];
	   //else
		//   m_nDeltaX=pDraw->win.right-GetWinRight(pDraw,pData)-margin.nBorderWidth[BORDERDATA::RIGHT];
   }
      
   return 0;
   

}

