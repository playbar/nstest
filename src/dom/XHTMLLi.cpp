// XHTMLLi.cpp: implementation of the XHTMLLi class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLLi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XHTMLLi::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_GET_RECT:
		 if(IsInline()) return XDomNode::Handle(nOpera,pData1,pData2);
		 break;
	case XDO_HITTEST:
		 if(IsInline()) return XDomNode::Handle(nOpera,pData1,pData2);
		 break;
	case XDO_SET_FINAL:
		 if(IsInline()) return XDomNode::Handle(nOpera,pData1,pData2);
		 break;
	case XDO_LAYEROUT_CELLS:
		 if(!IsVisiable()) return 0;
		 if(IsInline()) 
			return Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XTRUE);
		 break;
		 /*else
		 {
			 DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			 XU8 sd=pDraw->SPACING;
			 pDraw->SPACING=8;
			 XDomBlock::LayeroutCells(pDraw,(CELLDATA*)pData2,XTRUE);
			 pDraw->SPACING=sd;
			 return 1;
		 }break;*/
	case XDO_LAYEROUT_CELL:
		 if(!IsVisiable()) return 0;
		 if(IsInline())
			return Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
		 break;
		 /*else
		 {
			 DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			 XU8 sd=pDraw->SPACING;
			 pDraw->SPACING=8;
			 XDomBlock::LayeroutCell(pDraw,(CELLDATA*)pData2,XTRUE);
			 pDraw->SPACING=sd;
			 return 1;
		 }break;*/
		 //break;
	case XDO_PAINT:
		 if(IsInline())
			return Paint((DRAWCONTEXT*)pData1);
		 else
			break;
	case XDO_UPDATE_TABLE:
		 if(IsInline()) return XDomNode::Handle(nOpera,pData1,pData2);
		 break;
		 //break;
	}
	return XDomBlock::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLLi::Paint(DRAWCONTEXT*pDraw)
{
	if(!IsVisiable()) return 0;
	XRect rect(m_nPosX,m_nPosY,m_nPosX+pDraw->SPACE,m_nPosY+pDraw->SPACE);
	
	

	XU32 ss=PrePaint(pDraw);

	if(rect.IsUnion(pDraw->paint)) //return 0;

	{
		PaintBack(pDraw,rect);

		XIMAGEDATA*p=(XIMAGEDATA*)FindAttrib(XEAB::LIST_IMAGE,XNULL);
		if(p!=XNULL&&!p->image.IsEmpty())
		{
			 pDraw->pDraw->DrawImage(p->image,XPoint(m_nPosX,m_nPosY));
		}
		else
		{

			XU8 nType=FindAttrib(XEAB::TYPE,pDraw->paintData.nListType);
			XPCTSTR strValue=FindAttrib(XEAB::VALUE,(XPCTSTR)XNULL);
			int dd=(pDraw->SPACE-XPOTSIZE)>>1;
			XGraphics*g=pDraw->pDraw;
			XU32 c=g->SetBackColor(g->GetColor());
			switch(nType)
			{
			case XELT::TYPE_NONE:
				break;
			default:
				if(strValue!=XNULL)
				{
					XString16 strTxt(strValue);
					strTxt+=XString16(".");
					XSize sz;
					g->MeasureString(strTxt,sz);
					g->DrawString(strTxt,m_nPosX+(pDraw->SPACE)-sz.cx,m_nPosY);
					break;
				}break;
			case XELT::TYPE_DISC:
				rect.DeflateRect(dd,dd);
				//rect+=XSize(pDraw->SPACE,0);
				g->FillEllipse(rect);
				break;
			case XELT::TYPE_SQURE:
				rect.DeflateRect(dd,dd);
				//rect+=XSize(pDraw->SPACE,0);
				g->FillRectangle(rect);
				break;
			case XELT::TYPE_CIRCLE:
				rect.DeflateRect(dd,dd);
				//rect+=XSize(pDraw->SPACE,0);
				g->DrawEllipse(rect);
				break;
			}
			g->SetBackColor(c);
		}
	}
	HandleChild(XDO_PAINT,(XU32)pDraw,XNULL);//(XU32)this);
	EndPaint(pDraw);
	return 1;

}

XU32 XHTMLLi::Layerout(DRAWCONTEXT*pDraw,CELLDATA*pData,XU8 bCells)
{
	//XU8 ft=FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);
	if(pDraw->nLayerMode==LA_SAMPLE)
		return XDomNode::Handle(bCells?XDO_LAYEROUT_CELLS:XDO_LAYEROUT_CELL,
								(XU32)pDraw,(XU32)pData);
	//XU8 ft=FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);
	XU8 bInline=IsFingerInline();
	if(!bInline&&!pData->bNewLine)//Return)
			NewRow(0,pDraw,pData,pDraw->IND);
	XIMAGEDATA*p=(XIMAGEDATA*)FindAttrib(XEAB::LIST_IMAGE,XNULL);
	//XU8 nw=pData->nowrap;
	//pData->nowrap=XEA::NOWRAP;
	
	if(!bCells)
	{
		m_nPosX=pData->cur.x;
		m_nPosY=this->AlignCol(pDraw,pData,pDraw->SPACE,XEA::MIDDLE);
	}
	if(p!=XNULL&&!p->image.IsEmpty())
	{
		int w=p->image.Width();
		int h=p->image.Height();
		SetRowSize(pDraw,pData,XSize(w,h),w,bCells);
	}
	else
	{
		XU8 nType=FindAttrib(XEAB::TYPE,pDraw->paintData.nListType);
		if(nType==XELT::TYPE_NONE)
			pData->cur.x+=12;//pDraw->SPACE>>1;
		else
			pData->cur.x+=pDraw->SPACE;
	}
	pData->nSubWidth=pDraw->SETWIDTH;
	HandleChild(bCells?XDO_LAYEROUT_CELLS:XDO_LAYEROUT_CELL,
					 (XU32)pDraw,(XU32)pData);
	pData->nSubWidth=0;
	if(!bInline&&!pData->bNewLine)//Return)
			NewRow(0,pDraw,pData,pDraw->IND);
	//pData->nowrap=nw;
	return 1;
}


XU32 XHTMLUl::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
    //case XDO_PAINT:
	case XDO_LAYEROUT_CELL:
	case XDO_LAYEROUT_CELLS:
		 if(IsVisiable())
		 {
			//if(!IsVisiable()) return 0;
			if(!IsFingerInline())
			{
				CELLDATA*p=(CELLDATA*)pData2;
				if(!p->bNewLine)//Return)
					NewRow(0,(DRAWCONTEXT*)pData1,p,
						   ((DRAWCONTEXT*)pData1)->IND);
			}
		 }
		 break;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLOl::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
//	if(!IsVisiable()) return 0;
	switch(nOpera)
	{
	case XDO_SET_FINAL:
		 m_bFinal=1;
		 break;
	case XDO_LAYEROUT_CELL:
		 if(m_bFinal==1)
		{
			m_bFinal=2;
			XDomList list;
			GetList(list,XDOM_LI);
			XU8 nType=FindAttrib(XEAB::TYPE,XELT::TYPE_NUM);
			XVar var(XEAB::TYPE,nType),vt(XEAB::VALUE,0);
			char cc='A';
			for(XU32 i=0;i<list.GetSize();i++)
			{
				switch(nType)
				{	
				default:
					 var.iData=XELT::TYPE_NUM;
				case XELT::TYPE_NUM:
					 vt=i+1;
					 vt.ToString();
					 break;
				case XELT::TYPE_LA:cc='a';
				case XELT::TYPE_A:
					{
						XString8 str;
						int v=i;
						char c=(v%26)+cc;
						str.InsertAt(0,c);
						v=v/26;
						while(v>0)
						{
							char c=(v%27)+cc-1;
							str.InsertAt(0,c);
							v=v/27;
							if(v<=0) break;
						}
						vt=str;
					}
					 break;
				}
				list[i]->AddAttrib(var);
				list[i]->AddAttrib(vt);
			}
		}
	//case XDO_PAINT:
	case XDO_LAYEROUT_CELLS:
		if(IsVisiable())
		 {
			//if(!IsVisiable()) return 0;
			if(!IsFingerInline())
			{
				CELLDATA*p=(CELLDATA*)pData2;
				if(!p->bNewLine)//Return)
					NewRow(0,(DRAWCONTEXT*)pData1,p,
						   ((DRAWCONTEXT*)pData1)->IND);
			}
		 }
		 break;
	}

	return XDomNode::Handle(nOpera,pData1,pData2);
}