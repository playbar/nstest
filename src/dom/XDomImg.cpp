// XDomImg.cpp: implementation of the XDomImg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomImg.h"
#include "XEnumData.h"
#include "XDom.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//static XPCTSTR _strLoading="??????...";
//static XPCTSTR _strFailed="????????";
#define _NOPAINTWAIT_
XU32 XDomImg::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
//	if(!IsVisiable()) return 0;
	XVar*p=(XVar*)pData1;
	switch(nOpera)
	{
	case XDO_HITTEST:
		 if(!IsVisiable()) return 0;
		 if(FindAttrib(XEAB::USEMAP,0)) return 0;
		 break;
	case XDO_EVENT_ONKEYDOWN:
		 if(!IsVisiable()) return 0;
		 if(pData1!=XK_RETURN) return XRNONE;
	case XDO_EVENT_ONCLICK:
		 if(!IsVisiable()) return 0;
		 if(!m_bSelectAble) return XRNONE;
		 //m_nMode=1-m_nMode;
		 if(m_nMode<2) m_nMode++;
		 else m_nMode=0;
		 //if(m_nMode)
		 {
			 return SetOffset((XEVENTDATA*)pData2);
		 }
		 return XRLAYEROUT;
	case XDO_GET_RECT:
		 if(!IsVisiable()) return 0;
		 GetRoundRect(*(XRect*)pData1);
		 return XTRUE;
	case XDO_EVENT_ONTIMER:
		{
		 if(!IsVisiable()) return 0;
		 XRect rect(m_nPosX,m_nPosY,m_nPosX+m_nWidth,m_nPosY+m_nHeight);
		 if(m_pData==XNULL) return XRNONE;
		 XEVENTDATA*pData=(XEVENTDATA*)pData1;
		 if(m_pData->bModify)
		 {
			 pData->rect|=rect;
			 return XRREPAINT;
		 }
		 else if(m_pData->nStatus!=XL_OK)
		 {
			//if(m_pData->nStatus!=XL_LOADING) return XRNONE;
			XImage*pImage=GetImage();

			if(pImage==XNULL||pImage->FrameCount()<2) return XRNONE;

			m_nTicks++;
			int nc=pImage->GetFrameDelay(m_nIndex);
			if(m_nTicks<nc) return XRNONE;
			m_nTicks=0;
			m_nIndex++;
			if(m_nIndex>=pImage->FrameCount())
				m_nIndex=0;
			pData->rect|=rect;
			return XRREPAINT;
		 }
		 return XRNONE;
		 //return OnTimer((XEVENTDATA*)pData1,rect);
		}break;
	case XDO_LAYEROUT_CELL:
		 if(!IsVisiable()) return 0;
		 Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
	case XDO_LAYEROUT_CELLS:
		 if(!IsVisiable()) return 0;
		 LayeroutCells((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return XTRUE;
//	case XDO_GET_IMAGE:
//		 *p=FindAttrib(XEAB::LOCALSRC,FindAttrib(XEAB::SRC,XNULL));
	case XDO_SET_FINAL:
		 //break;
		 if(pData2!=0)
		 {
			XEVENTDATA*p=(XEVENTDATA*)pData2;
			XPCTSTR strURL=FindAttrib(XEAB::LOCALSRC,FindAttrib(XEAB::SRC,(XPCTSTR)XNULL));
			if(strURL==XNULL)
				return XFALSE;
			m_pData=p->pDom->m_imgList.SetImage(strURL,this,p->pDom);
			return m_pData!=XNULL;
		 }
		 return XTRUE;
		 //break;
	case XDO_UPDATE_IMAGE:
		 if(!IsVisiable()) return 0;
		if(FindAttrib(XEAB::WIDTH,0)>0&&
		   FindAttrib(XEAB::HEIGHT,0)>0&&m_nMode==0) break;
	/*	{
			   	XString8 str=FindAttrib(XEAB::SRC,"");
				if(str=="images/tu2.gif")
					int a=0;
		}*/
		 return Handle(XDO_UPDATE_TABLE,pData1,pData2);
	/*case XDO_SET_IMAGE:
		 if(m_pData!=XNULL) return XFALSE;
		 else
		 {
			 m_pData=(XIMAGEDATA*)pData1;
			 m_pData->nMethod=XEM::GET;
			 XINT id=FindAttrib(XEAB::SRC);
			 if(id<0) return XFALSE;
			 m_pData->strURL=m_attribs[id].strData;
			 return XTRUE;
		 }
		 return XTRUE;*/
	case XDO_PAINT:
		 if(!IsVisiable())
			 return 0;
		 return Paint((DRAWCONTEXT*)pData1,(XDomLeaf*)pData2);
	case XDO_GET_STYLE:
		{
		 XU32 s=XDomLeaf::Handle(nOpera,pData1,pData2)|DOM_IMAGE|DOM_PAINT;//|DOM_SELECTABLE;
		 // No Select Image Modify
		 //if(m_pData!=XNULL&&!m_pData->GetImage()->IsEmpty())
		 //{
		//	 if(FindAttrib(XEAB::USEMAP,0)==0&&m_nWidth>48&&m_nHeight>36)
		//		s|=DOM_SELECTABLE;
		 //}
		 return s;
		}
		 break;
/*	case XDO_SET_ATTR:
	case XDO_ADD_ATTR:
		 switch(p->nID)
		 {
		 case XEnumAttribs::ALIGN:
			  if(p->nType==VSTRING)
				  *p=XEnumAlign().Index(p->strData);
			  break;
		 case XEnumAttribs::HEIGHT:
		 case XEnumAttribs::HSPACE:
		 case XEnumAttribs::VSPACE:
		 case XEnumAttribs::WIDTH:
			  if(p->nType==VSTRING)
				  *p=PECENT(XString8(p->strData));
			  break;
		 }
		 break;*/
	}
	return XDomLeaf::Handle(nOpera,pData1,pData2);
}


void XDomImg::Layerout(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
   LAYEROUTDATA margin;
   PreLayerout(pDraw,pData,&margin);
   XSize sz(m_nWidth,m_nHeight);

  // XString8 str=FindAttrib(XEAB::SRC,"");
 //  if(str=="images/top1.jpg")
//	   int a=0;

   XINT vs=FindAttrib(XEAB::VSPACE,0);
   if(vs<0) vs=-vs*(sz.cx)/100;
   XINT hs=FindAttrib(XEAB::HSPACE,0);
   if(hs<0) hs=-hs*(sz.cy)/100;

   /*sz.cx+=(vs<<1);
   sz.cy+=(hs<<1);*/
   int ft=pDraw->SETFLOAT;//FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);
   if(ft==0)
   {
	   XU8 aw=FindAttrib(XEAB::ALIGN,0);
	   if(aw==XEA::LEFT) ft=XELT::FLOAT_LEFT;
	   else if(aw==XEA::RIGHT) ft=XELT::FLOAT_RIGHT;
   }

   if(ft)
   {
	   if(m_bReturn)
		NewFloat(pDraw,pData,ft==XELT::FLOAT_LEFT);
	   //else NewFloatI
		   //NewRow(0,pDraw,pData,0);
		//NewRowIfNeed(pDraw,pData,0);
   }
   else {
	 //if(m_bReturn)
	   NewRowMust(pDraw,pData,m_bReturn);
   }

   m_nPosX=pData->cur.x+vs+margin.nMargin[BORDERDATA::LEFT];
   XU8 align=pDraw->ALIGNH;//FindAttrib(XEAB::ALIGN,XEA::BOTTOM);
   m_nPosY=AlignCol(pDraw,pData,sz.cy,align)+hs;
   m_bSelectAble=SelectAble(pDraw);
   if(ft==XELT::FLOAT_RIGHT)
	   m_nPosX=pDraw->win.right-m_nDeltaX;
	   //GetWinRight(pDraw,pData)-m_nWidth;
   else if(ft==XELT::FLOAT_LEFT)
	   m_nPosX=pDraw->win.left+m_nDeltaX;

   EndLayerout(pDraw,pData);
   //sz.cx+=2;
   //sz.cy+=2;
   if(ft)
   //{
   //}
	  SetFloat(pDraw,pData,XSize(m_nWidth,m_nHeight),ft==XELT::FLOAT_LEFT);
   else
	   SetRowSize(pDraw,pData,sz,sz.cx,XFALSE);


}

XBOOL XDomImg::Paint(DRAWCONTEXT *pDraw,XDomLeaf*pItem)
{
	XImage*pImage=GetImage();
	XSize sz;
	int nMin;
	GetSize(pDraw,pImage,sz,nMin);
	XRect rect(m_nPosX,m_nPosY,m_nPosX+sz.cx,m_nPosY+sz.cy);//m_nWidth,m_nPosY+m_nHeight);

	if(!rect.IsUnion(pDraw->paint))
		return XFALSE;

	XINT vs=FindAttrib(XEAB::VSPACE,0);
    if(vs<0) vs=-vs*(rect.Width())/100;
    XINT hs=FindAttrib(XEAB::HSPACE,0);
    if(hs<0) hs=-hs*(rect.Height())/100;
	XRect rt(rect.left-vs,rect.top-hs,rect.right+vs,rect.bottom+vs);

	PaintBack(pDraw,rt);
#ifdef  _NOPAINTWAIT_
	if(m_pData&&m_pData->nStatus!=XL_OK)
	{
		PaintBorder(pDraw,rt);
		return XTRUE;
	}
#endif
	if(pImage!=XNULL&&!pImage->IsEmpty())
	{
		if(m_pData==XNULL||(m_pData->nStatus!=XL_OK&&
			pImage!=&m_pData->image))
			pImage->SetActive(m_nIndex);
		if(m_pData!=XNULL&&m_pData->nStatus!=XL_OK)
		{
			pDraw->pDraw->DrawImage(*pImage,rect.TopLeft());
			rect.left+=pImage->Width();
		}
		else
		{
			int w=pImage->Width();
			pDraw->pDraw->DrawImage(*pImage,rect);
			PaintSelect(rect,pDraw,pDraw->DATA);
			PaintBorder(pDraw,rt);
			return XTRUE;
		}
	}
	else
	{
	   XString16 str;
	   GetString(str);
	   pDraw->pDraw->DrawString(str,rect.left,rect.top);
	}
	PaintBorder(pDraw,rt);
	return XTRUE;
}

XImage* XDomImg::GetImage()
{
    XImage*pImage = XNULL;
	if (m_pData != XNULL)
	{
		switch (m_pData->nStatus)
		{
		case XL_WAIT:
			pImage = XWindow::GetIcon(XICON_LOADING);
			break;
		case XL_LOADING:
			pImage = XWindow::GetIcon(XICON_LOADING);
			break;
		case XL_FAILED:
			pImage = XWindow::GetIcon(XICON_FAILED);
			break;
		case XL_OK:
			if (!m_pData->GetImage()->IsEmpty())
			{
				pImage = m_pData->GetImage();
			}
			break;
		default:
			break;

		}
	}
	if (pImage != XNULL && pImage->IsEmpty())
		return XNULL;
	return pImage;
}

void XDomImg::GetSize(DRAWCONTEXT*pDraw,XImage *pImage,XSIZE&sz,int& nMin)
{
   if(pImage==XNULL||pImage->IsEmpty())
   {
	   XString16 str;
	   GetString(str);
	   if(str.GetLength()<=0)
		   sz=XSize(0,0);
	   else pDraw->pDraw->MeasureString(str,sz);
	   nMin=XMIN(sz.cx,pDraw->SPACE<<1);
   }
   else
   {
	    int nMode=m_nMode;
		if(nMode==4)
		{
			if(pDraw->nLayerMode==LA_SAMPLE)
				nMode=2;
			else
			    nMode=0;
		}
	    if(nMode==0)
		{
			int w=pImage->Width();
			int h=pImage->Height();
			sz.cx=FindAttrib(XEAB::WIDTH,-100);
			sz.cy=FindAttrib(XEAB::HEIGHT,-100);
			if(sz.cx==-100&&sz.cy!=-100&&h!=0)
			{
				sz.cx=sz.cy*w/h;
			}
			if(sz.cy==-100&&sz.cx!=-100&&h!=0)
			{
				sz.cy=sz.cx*h/w;
			}
			/*if(sz.cx==-100||sz.cy==-100&&sz.cx!=sz.cy)
			{
				if(sz.cx<sz.cy) sz.cx=sz.cy;
				else sz.cy=sz.cx;
			}*/
			/*if(m_pData->nStatus!=XL_OK)
			{
				sz.cx=-100;
				sz.cy=-100;
			} */
			if(sz.cx<0) sz.cx=XMAX(w*(-sz.cx)/100,1);
			if(sz.cy<0) sz.cy=XMAX(h*(-sz.cy)/100,1);
			/*if(sz.cx>pDraw->win.Width())
			{
				sz.cy=sz.cy*pDraw->win.Width()/sz.cx;
				sz.cx=pDraw->win.Width();
			} */
			nMin=sz.cx;
		}
		else if(nMode==2)
		{
			int w=pImage->Width(),h=pImage->Height();
			sz.cx=w;
			sz.cy=h;
			if(w>pDraw->view.Width())
			{
				sz.cy=sz.cy*pDraw->view.Width()/sz.cx;
				sz.cx=pDraw->view.Width();
			}
			nMin=sz.cx;
		}
		else
		{
			int w=pImage->Width(),h=pImage->Height();
			int fx=w*1000/(pDraw->view.Width()-2);
			int fy=h*1000/(pDraw->view.Height()-2);
			int ff=XMAX(XMAX(fx,fy),1000);
			sz.cx=w*1000/ff;
			sz.cy=h*1000/ff;
			nMin=sz.cx;
			return;
		}
		/*if(m_pData!=XNULL&&m_pData->nStatus<XL_OK)
		{
		  XString16 str;
		  GetString(str);
		  if(str.GetLength()>0)
		  {
			  XSIZE sz1;
			  pDraw->pDraw->MeasureString(str,sz1);
			  sz.cx+=sz1.cx;
			  sz.cy=XMAX(sz.cy,sz1.cy);
		  }
		  nMin+=XMIN(sz.cx,pDraw->SPACE);
		}*/
   }
}

void XDomImg::GetString(XString16 &str)
{
	   XU8 nStatus=m_pData==XNULL?0:m_pData->nStatus;
	   switch(nStatus)
	   {
	   case XL_WAIT:
	   case XL_LOADING:
		    str=FindAttrib(XEAB::ALT,FindAttrib(XEAB::TITLE,""));//_strLoading));
		    //str=_strLoading;
		    break;
	   default:
		    str=FindAttrib(XEAB::TITLE,FindAttrib(XEAB::ALT,""));
		    break;
	   }
}

//DEL XU32 XDomImg::OnTimer(XEVENTDATA*pData)
//DEL {
//DEL 	XRect rt;
//DEL 	GetRoundRect(rt);
//DEL 	return OnTimer(pData,rt);
//DEL //	rt-=pDraw->offset;
//DEL 	if(!rt.IsUnion(pData->viewRect)) return XRNONE;
//DEL //	rt+=pDraw->offset;
//DEL 	XImage*pImage=GetImage();
//DEL 	if(pImage==XNULL||
//DEL 	   pImage->IsEmpty()||
//DEL 	   pImage->FrameCount()<2) return XRNONE;
//DEL 	m_nTicks++;
//DEL 	int nc=pImage->GetFrameDelay(m_nIndex);
//DEL 	if(m_nTicks<nc) return XRNONE;
//DEL 	m_nTicks=0;
//DEL 	m_nIndex++;
//DEL 	if(m_nIndex>=pImage->FrameCount())
//DEL 		m_nIndex=0;
//DEL
//DEL 	pImage->SetActive(m_nIndex);
//DEL
//DEL 	pData->rect|=rt;
//DEL 	return XRREPAINT;
//DEL }

void XDomImg::GetRoundRect(XRect &rect)
{
	rect.left=m_nPosX;
	rect.top=m_nPosY;
	rect.right=m_nPosX+m_nWidth;
	rect.bottom=m_nPosY+m_nHeight;
	//rect.In(1,1);
/*	XSIZE sz;
	XImage*pImage=GetImage(pDraw);
	GetSize(pDraw,pImage,sz);
	rect.left=m_nPosX-pDraw->offset.x;
	rect.top=m_nPosY-pDraw->offset.y;
	rect.right=m_nPosX+sz.cx-pDraw->offset.x;
	rect.bottom=m_nPosY+sz.cy-pDraw->offset.y;*/
}

XBOOL XDomImg::SelectAble(DRAWCONTEXT*pDraw)
{
	if(m_pData==XNULL||m_pData->nStatus!=XL_OK) return XFALSE;
	if(FindAttrib(XEAB::USEMAP,0)) return XFALSE;
	XImage*pImage=GetImage();
	if(pImage==XNULL||pImage->IsEmpty()) return XFALSE;
	int w=pImage->Width(),h=pImage->Height();
	if(w>pDraw->view.Width()||
	   h>pDraw->view.Height()) return XTRUE;
	if(w!=m_nWidth||h!=m_nHeight) return XTRUE;
	return XFALSE;
}

XU32 XDomImg::SetOffset(XEVENTDATA *pData)
{
	XU32 s=XRLAYEROUT;
	int h=pData->viewRect.Height(),
		w=pData->viewRect.Width();
	if(m_nWidth>m_nHeight)
		h=h*pData->viewRect.Width()/m_nWidth;
	else
		w=w*pData->viewRect.Height()/m_nHeight;

	if(m_nPosY<pData->viewRect.top||
	   m_nPosY+h>pData->viewRect.bottom)
	{
		pData->offset.y=m_nPosY-((pData->viewRect.Height()-h)>>1);
		s|=XROFFSET;
	}
	if(m_nPosX<pData->viewRect.left||
	   m_nPosX+w>pData->viewRect.right)
	{
		s|=XROFFSET;
		pData->offset.x=m_nPosX-((pData->viewRect.Width()-w)>>1);
	}
	/*if(m_nPosX<pData->offset.x)
	{
		pData->offset.x=m_nPosX;
		s|=XROFFSET;
	}*/

	return s;
}

void XDomImg::LayeroutCells(DRAWCONTEXT *pDraw, CELLDATA *pData)
{

   LAYEROUTDATA margin;
   PreLayerout(pDraw,pData,&margin);

   XImage*pImage=GetImage();
   XSize sz;
   int nMin=0;
   GetSize(pDraw,pImage,sz,nMin);

   XINT vs=FindAttrib(XEAB::VSPACE,0);
   if(vs<0) vs=-vs*(sz.cx)/100;
   XINT hs=FindAttrib(XEAB::HSPACE,0);
   if(hs<0) hs=-hs*(sz.cy)/100;

   sz.cx+=(vs<<1);
   sz.cy+=(hs<<1);
   m_nWidth=sz.cx;
   m_nHeight=sz.cy;


   //if(pData->cur.x+sz.cx>pDraw->win.right&&pData->IsWrap()&&!pData->bReturn)
   XU8 ft=pDraw->SETFLOAT;//FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);
   if(ft==0)
   {
	   XU8 aw=FindAttrib(XEAB::ALIGN,0);
	   if(aw==XEA::LEFT) ft=XELT::FLOAT_LEFT;
	   else if(aw==XEA::RIGHT) ft=XELT::FLOAT_RIGHT;

	   //{

	   //}
   }
   SetMargin(&margin,m_nWidth,m_nHeight,sz);

   if(!ft)
   {
	   m_bReturn=NewRowIfNeed(pDraw,pData,sz.cx);
	   SetRowSize(pDraw,pData,sz,sz.cx,XTRUE);
   }
   else
   {
	   m_bReturn=NewFloatIfNeed(pDraw,pData,sz.cx,ft==XELT::FLOAT_LEFT);
	   //if(!m_bReturn) //NewRowIfNeed(pDraw,pData,0);
		//   NewRow(0,pDraw,pData,0);
	   if(ft==XELT::FLOAT_LEFT)
		   //m_nDeltaX=GetWinLeft(pDraw,pData)-pDraw->win.left;
		   m_nDeltaX=GetWinLeft(pDraw,pData)-pDraw->win.left+margin.nMargin[BORDERDATA::LEFT];
	   //else
		 //  m_nDeltaX=pDraw->win.right-GetWinRight(pDraw,pData);
	   SetFloat(pDraw,pData,sz,ft==XELT::FLOAT_LEFT);
	   if(ft==XELT::FLOAT_RIGHT)
		   m_nDeltaX=pDraw->win.right-GetWinRight(pDraw,pData);//-margin.nMargin[BORDERDATA::RIGHT];
	   /*if(ft==XELT::FLOAT_LEFT)
		   m_nDeltaX=pData->cur.x-pDraw->win.left-m_nWidth;
	   else
		   m_nDeltaX=pDraw->win.right-GetWinRight(pDraw,pData);*/

	   	 //  if(ft==XELT::FLOAT_LEFT)

	   //else
		//   m_nDeltaX=pDraw->win.right-GetWinRight(pDraw,pData);
	   //SetFloat(pDraw,pData,size,ft==XELT::FLOAT_LEFT);
	   //if(clear&XELT::FLOAT_RIGHT)
		//   NewRowMust(pDraw,pData,1);



   }

   EndLayerout(pDraw,pData);
   //SetRowSize(pDraw,pData,sz,sz.cx,XTRUE);

}
