// XDomText.cpp: implementation of the XDomText class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomText.h"
//#ifdef __ANDROID__
//	#include "MultiStringUtil.h"
//#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XDomText::XDomText(XU16 ID) : XDomItem(ID)
{
	m_pVData=XNULL;
	m_nWin=m_nHeight=0;
	m_nOffset[0]= 0;
	m_nOffset[1]=0;
}

XDomText::~XDomText()
{
	if(m_pVData!=XNULL)
		delete m_pVData;
}


XU32 XDomText::Handle(XU32 nOpera,XU32 pData1,XU32 pData2)
{
	  XVar*p=(XVar*)pData1;
	  switch(nOpera)
	  {
	  /*case XDO_GET_TEXT:
			*((XString16*)pData1)=m_strTxt;
		   return XTRUE;*/
	  case XDO_DO_CSS:
		   m_lineDatas.RemoveAll();
		   break;
	  case XDO_REFRESH:
		   return TranslateText((XEVENTDATA*)pData1);
	  case XDO_SET_FINAL:
		   m_lineDatas.RemoveAll();
		   TranslateText((XEVENTDATA*)pData2);
		   break;
	  case XDO_RESET:
		   m_lineDatas.RemoveAll();break;
	  case XDO_GET_STYLE:
		   return XDomItem::Handle(nOpera,pData1,pData2)|DOM_STATIC|DOM_PAINT;
		   break;
	  case XDO_SELECTION:
	       return XDomItem::Selection((XSelection*)pData1);
	  case XDO_SELECT:
		   return XDomItem::Handle(nOpera,pData1,pData2);
	  case XDO_GET_RECT:
		   GetRect(*(XRect*)pData1,pData2);
		   return XTRUE;
	  case XDO_SET_ATTR:
	  case XDO_ADD_ATTR:
		   SetText(((XVar*)pData1)->strData);//m_strTxt);
		   //m_strTxt=(
		   return XTRUE;
	  case XDO_GET_ALLTEXT:
	  case XDO_GET_TEXT:
		   *((XString16*)pData1)+=m_strTxt;
		   return XTRUE;
/*	  case XDO_GET_ATTR_BYINDEX:
	  case XDO_GET_ATTR:
		   *p=m_strTxt;
		   break;*/
	  case XDO_GET_ATTR_COUNT:return 1;
	  case XDO_LAYEROUT_CELLS:
		   LayerOutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		   return XTRUE;
	  case XDO_LAYEROUT_CELL:
		   LayerOutCell((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		   return XTRUE;
	  case XDO_PAINT:
		   return Paint((DRAWCONTEXT*)pData1,(XDomLeaf*)pData2);
	  case XDO_HITTEST:
		   if(HitTest(pData1,pData2)) return (XU32)this;
	  }
	  return 0;
}

void XDomText::LayerOutCells(DRAWCONTEXT *pDraw, CELLDATA *pData)
{

	switch(pDraw->TRANS)
	{
	case XELT::TRANS_UPCASE:
		 m_strTxt.MakeUpper();
		 break;
	case XELT::TRANS_LOCASE:
		 m_strTxt.MakeLower();
		 break;
	case XELT::TRANS_CAP:
		 m_strTxt.MakeFirstUpper();
		 break;
	}
	LAYEROUTDATA*pd=pData->pData;
	XRect win=pDraw->win;
	//int cury=pData->cur.y;
	int mh=0,mw=0;
	if(pd!=XNULL)
	{
		pd->Total();
		pData->cur.x+=pd->nOffset[BORDERDATA::LEFT];
		//pData->cur.y+=pd->nOffset[BORDERDATA::TOP];
		pDraw->win.left+=pd->nOffset[BORDERDATA::LEFT];
		pDraw->win.right-=pd->nOffset[BORDERDATA::RIGHT];
		mh=pd->nOffset[BORDERDATA::TOP]+pd->nOffset[BORDERDATA::BOTTOM]-
			pd->nMargin[BORDERDATA::TOP]-pd->nMargin[BORDERDATA::BOTTOM];
		mw=pd->nOffset[BORDERDATA::LEFT]+pd->nOffset[BORDERDATA::RIGHT]-
			pd->nMargin[BORDERDATA::LEFT]-pd->nMargin[BORDERDATA::RIGHT];
		m_nOffset[0]=(XU8)(pd->nBorderWidth[BORDERDATA::LEFT]+
						  pd->nPadding[BORDERDATA::LEFT]);
		m_nOffset[1]=(XU8)(pd->nBorderWidth[BORDERDATA::TOP]+
						  pd->nPadding[BORDERDATA::TOP]);
	}

	MakeLineData(pDraw,pData,mh,mw,pData->nSubWidth);
	//m_nHeight+=mh;
	int ddh=XMAX((pDraw->SPACE-m_nHeight),0);
	XSize sz(m_lineDatas[3],m_nHeight+ddh);
//	pData->cur.y=cury;
	XU32 nCount=m_lineDatas.GetSize();
	//int dy=(int)pDraw->nData;
	//int wx=pDraw->win.Width();
	for(XU32 i=0;i<nCount;i+=4)
	{
		sz.cx=i+4<nCount?(XMAX(pDraw->win.right-pData->cur.x,0)):m_lineDatas[i+3];
		SetRowSize(pDraw,pData,sz,XMIN(sz.cx,pDraw->SPACE<<1),XTRUE);
		if(i+4<nCount)
			NewRow(sz.cy,pDraw,pData,0);
	}
	pDraw->win=win;
}

void XDomText::LayerOutCell(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
//	//	return;
	if(m_lineDatas.GetSize()<=0) return;



	//	MakeLineData(pDraw,pData);
	XU32 nCount=m_lineDatas.GetSize();
	
	//m_nPosX=pData->cur.x;
	//m_nPosX0=pDraw->win.left;
	//m_nWin=pDraw->win.Width();

	int dy=0,px=pData->cur.x;
	if(pDraw->DATA!=0)
		dy=(int)pDraw->DATA;
	
	LAYEROUTDATA*pd=pData->pData;
	//XRect win=pDraw->win;
	int cury=pData->cur.y;
	if(pd!=XNULL)
	{
		pData->cur.x+=pd->nMargin[BORDERDATA::LEFT];
		//pData->cur.y+=pd->nMargin[BORDERDATA::TOP];
		dy+=pd->nMargin[BORDERDATA::TOP];
		m_nOffset[0]=(XU8)(pd->nBorderWidth[BORDERDATA::LEFT]+
						  pd->nPadding[BORDERDATA::LEFT]);
		m_nOffset[1]=(XU8)(pd->nBorderWidth[BORDERDATA::TOP]+
						  pd->nPadding[BORDERDATA::TOP]);

		//pd->Total();
		//m_nHeight+=pd->nOffset[BORDERDATA::TOP]+
		//		   pd->nOffset[BORDERDATA::BOTTOM];
		//pData->cur.x+=pd->nOffset[BORDERDATA::LEFT];
		//pData->cur.y+=pd->nOffset[BORDERDATA::TOP];
		//pDraw->win.left+=pd->nOffset[BORDERDATA::LEFT];
		//pDraw->win.right-=pd->nOffset[BORDERDATA::RIGHT];
	}
	int ddh=XMAX((pDraw->SPACE-m_nHeight),0);
	XSize sz(m_lineDatas[3],m_nHeight+ddh);

	//int dh=XMIN(m_nHeight,pDraw->SPACE);
	int py=pData->cur.y+pData->rowws[pData->nRow]+dy;
	pData->cur.y=cury;
	//int wx=pDraw->win.Width();
	for(XU32 i=0;i<nCount;i+=4)
	{		
		m_lineDatas[i]=px;
		m_lineDatas[i+1]=py-m_nHeight-(ddh>>1);//+(ddh>>1);		
		sz.cx=i+4<nCount?(XMAX(pDraw->win.right-px,0)):m_lineDatas[i+3];
		SetRowSize(pDraw,pData,sz,pDraw->SPACE<<2,XFALSE);
		if(i+4<nCount)
			NewRow(sz.cy,pDraw,pData,0);
		py=pData->cur.y+pData->rowws[pData->nRow]+dy;
		px=pData->cur.x;
	}

}

XBOOL XDomText::Paint(DRAWCONTEXT *p,XDomLeaf*pParent)
{
	if(m_lineDatas.GetSize()<=0)
		return XFALSE;
	XPCWSTR strData=m_strTxt;
	DOMPOS* pData=m_lineDatas;
	
	XU32 nCount=m_lineDatas.GetSize();

	if(pData[1]>=p->paint.bottom|| pData[nCount-3]+m_nHeight<=p->paint.top)
		return XFALSE;

	XBOOL bPaint=XFALSE;

	XBOOL bUp=(p->pDraw->GetFontStyle()&XGraphics::XF_UPLINE);

	for(XU32 i=0;i<nCount;i+=4)
	{
	   XPoint pt(pData[i],pData[i+1]);
	   if(pt.y>=p->paint.bottom)
		   continue;
	   if(pt.x>=p->paint.right || pt.x+pData[i+3]<=p->paint.left || pt.y+m_nHeight<=p->paint.top)
		   continue;
	   int iLen=i+6<nCount?pData[i+6]-pData[i+2]:-1;
	   if(iLen==0)
		   continue;
	   XRect rect(pt.x+1,pt.y,pt.x+pData[i+3],pt.y+m_nHeight-2);
	   if(pParent)
	   {
		   pParent->PaintBack(p,rect);
	   }

	   pt.y++;
	   pt.x+=m_nOffset[0];
	   pt.y+=m_nOffset[1];

	   p->pDraw->DrawString(strData+pData[i+2],pt,iLen);
	   if(bUp)
	   {
		   p->pDraw->DrawLine(pt.x,pt.y,pt.x+pData[i+3],pt.y);
	   }
	   if(pParent)
	   {
		   pParent->PaintBorder(p,rect);
	   }
	   bPaint=XTRUE;
	}
	return bPaint;
}

XBOOL XDomText::HitTest(XINT x, XUINT y)
{
	if(m_lineDatas.GetSize()<=0) return XFALSE;
	DOMPOS* pData=m_lineDatas;
	XBOOL bPaint=XFALSE;
	//int bx=m_nPosX;
	for(XU32 i=0;i<m_lineDatas.GetSize();i+=4)
	{
	   XRect rt(pData[i],//bx,
		        pData[i+1],
				pData[i]+pData[i+3],
				pData[i+1]+m_nHeight);
	   if(rt.PtInRect(x,y))
		   return XTRUE;
	   //bx=m_nPosX0;
	}
	return XFALSE;
}

void XDomText::GetRect(XRect&rect,XU32 nType)
{
	XString8 str(m_strTxt);
	DOMPOS* pData=m_lineDatas;
	if(pData==NULL) 
		return;
//	int bx=m_nPosX;
	XBOOL bPaint=XFALSE;
	XU32 nCount=m_lineDatas.GetSize(),is=0;
	if(nType==1)
	{
		if(nCount>4) is+=4;
		if(nCount>8) nCount-=4;
	}
	for(XU32 i=is;i<nCount;i+=4)
	{
	   XRect rt(pData[i],//bx,
		        pData[i+1],
				pData[i]+pData[i+3]+1,
				pData[i+1]+m_nHeight);
	   rect|=rt;
	  // bx=m_nPosX0;
	}
}

void XDomText::MakeLineData(DRAWCONTEXT *pDraw,CELLDATA*pData,int mh,int mw,int sw)
{

	if(m_lineDatas.GetSize()>0)
	{
	   if(m_lineDatas.GetSize()<=4)
	   {
		   if(m_lineDatas[3]+pData->cur.x<GetWinRight(pDraw,pData))//Width())
		   {
			   m_nWin=pDraw->win.Width();
			   return;
		   }
	   }
	   else if(m_nWin==GetWinWidth(pDraw,pData))//pDraw->win.Width())
	   {
		   if(m_lineDatas[0]-m_lineDatas[4] == pData->cur.x-pDraw->win.left)
			  return;
	   }
	}  
	m_nWin=GetWinWidth(pDraw,pData);//pDraw->GetWinWidth();//win.Width();

	m_lineDatas.RemoveAll();
	
	DOMPOSARRAY list;
	DOMPOS sp[4];
	if(pData->spans.GetSize()>0)
	{
		sp[0]=pData->spans[0]-pDraw->win.left;
		sp[1]=pData->spans[1]-pData->cur.y;
		sp[2]=pData->spans[2]-pDraw->win.left;
		sp[3]=pData->spans[3]-pData->cur.y;

	}

	//int len = m_strTxt.GetLength();
	//int bx = pData->cur.x-pDraw->win.left;
	//int win = pDraw->win.Width();
	m_nHeight=pDraw->pDraw->FormatString(m_strTxt,
				m_strTxt.GetLength(), pData->cur.x-pDraw->win.left, pDraw->win.Width(),
				list, pDraw->IsWrap()?XFMT_WORDALIGN:XFMT_ONELINE,
				pData->spans.GetSize()==0?XNULL:sp );

	m_nHeight+=mh;

	DOMPOS px=pData->cur.x,py=pData->cur.y;
	int isize = list.GetSize();
	for(XU32 i=0; i<isize; i+=3)
	{
		m_lineDatas.Add(px);
		m_lineDatas.Add(py);
		m_lineDatas.Add(list[i]);  //Start Char
		m_lineDatas.Add(XMAX(list[i+2]+mw,sw));//Width
		py+=pDraw->SPACE;
		px=pDraw->win.left;
	}
	m_lineDatas.FreeExtra();

}

XU32 XDomText::TranslateText(XEVENTDATA *pData)
{
	if(m_pVData==XNULL) return XRNONE;
	
	XPCTSTR str=XNULL;//RefreshVar((XPCTSTR)pData->nData,m_pVData->strKey,pData);

	for(XU32 i=0;i<pData->strValues.GetSize();i+=2)
	{
		if(pData->strValues[i].Compare(m_pVData->strKey,XTRUE)==0)
		{
			   str=pData->strValues[i+1];
			   break;
		}
	}

	if(str==XNULL) return XRNONE;
	XString16 strValue(str);
	if(strValue.GetLength()==m_pVData->iCount)
	{
		if(strValue.Compare(m_strTxt.GetData()+m_pVData->iBegin,
						    m_pVData->iCount)==0) return XRNONE;
	}
	m_lineDatas.RemoveAll();
	m_strTxt.RemoveAt(m_pVData->iBegin,m_pVData->iCount);
	m_strTxt.Insert(m_pVData->iBegin,strValue);
	m_pVData->iCount=(XU16)strValue.GetLength();
	//XString16 str(m_pVData->strKey);
	return XRLAYEROUT;
}

void XDomText::SetText(XPCTSTR strTxt)
{
	m_strTxt=strTxt;
	XString16 strLeft("$("),strRight(")");
	int is=m_strTxt.Find(strLeft);
	if(is>=0)
	{
		int ie=m_strTxt.Find(strRight,is+2);
		if(ie>is)
		{
			if(ie>is+2)
			{
				if(m_pVData==XNULL)
					m_pVData=new VDATA;
				m_pVData->iBegin=is;
				m_pVData->iCount=0;
				m_pVData->strKey.SetString(strTxt+is+2,ie-is-2);
			}
			m_strTxt.RemoveAt(is,ie-is+1);
			return;
		}
	}
	if(m_pVData!=XNULL) delete m_pVData;
	m_pVData=XNULL;
}

void XDomText::SetText16(XPCWSTR str)
{
	m_strTxt=str;
	//XString16 strTxt(str);
	//m_strTxt=str;//strTxt.DataTo();
}
