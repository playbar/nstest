// XDomAnchor.cpp: implementation of the XDomAnchor class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomAnchor.h"
#include "XEnumData.h"
#include "XDomText.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XDomAnchor::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	XVar*p=(XVar*)pData1;

	return XDomA::Handle(nOpera,pData1,pData2);
}

XU32 XDomA::Connect(XEVENTDATA *pData)
{
   XString8 strURL=FindAttrib(XEAB::HREF,(XPCTSTR)XNULL);
   XU32 s=XRNONE;
   if(!strURL.IsEmpty())
   {
	   s|=XRCONNECT;
	   Translate(pData->strValues,strURL);
	   pData->SetURL(strURL);//URL.SetURL(strURL);
   }
   s|=XDomNode::Handle(XDO_CONNECT,(XU32)pData,0);
   //if(m_nID==XDOM_A) s|=XR
   return s;//XRCONNECT;
}

XBOOL XDomA::Layerout(DRAWCONTEXT *pDraw, CELLDATA *pData, XBOOL bCells)
{

	//XString8 str=FindAttrib(XEAB::ID,"");
	//if(str=="debug")
	//	int a=0;
	

	/*if(!pData->bReturn&&pData->IsWrap())
	{
		CELLDATA data;
		data.Reset(pData->cur.x,
					   pData->cur.y,
					   pData->alignw,
					   pData->alignh,
					   pData->nowrap,
					   pData->ind);//0,0,XEA::LEFT,XEA::BOTTOM,XEA::NOWRAP);
			XDomNode::Handle(XDO_LAYEROUT_CELLS,(XU32)pDraw,(XU32)&data);
		if(data.rowws.GetSize()==2)
		{
			int cx=pData->cur.x-pDraw->win.left;
			//if((cx<<1)<data.fixCols[1])
			{
			if(data.fixCols[0]-cx
			   +data.fixCols[1]<pDraw->win.Width())//right)//Width())
				NewRow(pDraw->nRowSpace,pDraw,pData,bCells);
			}
		}
	} */
	//pData->cur.x++;
	if(m_childs.GetSize()<=0)
	{
		XVar var((int)XEAB::POSX,(int)pData->cur.x);
		XDomLeaf::SetAttrib(var);
		var.nID=XEAB::POSY;
		var.iData=pData->cur.y;
		XDomLeaf::SetAttrib(var);
	}
	XU32 s=XDomNode::Handle(bCells?XDO_LAYEROUT_CELLS:XDO_LAYEROUT_CELL,(XU32)pDraw,(XU32)pData);
	return s;
}

//DEL XBOOL XDomA::Paint(DRAWCONTEXT *pDraw)
//DEL {
//DEL 	XU32 s=XDomNode::Handle(nOpera,pData1,pData2);
//DEL 	return s;
//DEL }

void XDomA::DoFinal()
{
	//m_bJustImage=XTRUE;
	XDomList list;
	GetList(list,XDOM_TEXT);
	m_bJustImage=list.GetSize()<=0;
	if(m_bJustImage)
	{
		list.RemoveAll();
		GetList(list,XDOM_IMG);	
		if(list.GetSize()<=0) m_bJustImage=0;
	}
	/*XStack<XDomItem*> stack;
    stack.Push(this);
	XDomItem*p=XNULL;
	while(stack.Pop(p))
	{
		XU32 nCount=p->Handle(XDO_GET_CHILDCOUNT,0,0);
		for(XU32 i=0;i<nCount;i++)
		{
			XDomItem*pi=(XDomItem*)p->Handle(XDO_GET_CHILDBYINDEX,i,0);
			if(pi==XNULL) continue;
			XU32 s=pi->GetStyle();
			if(s&DOM_PAINT)
			{
				if(pi->GetID()!=XDOM_IMG) 
				{
					m_bJustImage=XFALSE;
					return;
				}
			}
			if(s&DOM_NODE) stack.Push(pi);
		}
	}*/
}
