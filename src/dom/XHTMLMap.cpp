// XHTMLMap.cpp: implementation of the XHTMLMap class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLMap.h"
#include "XDom.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XHTMLMap::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XDO_LAYEROUT_CELLS:return 0;
	case XDO_LAYEROUT_CELL:
		 if(pData1)
		 {
			DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			CELLDATA*pData=(CELLDATA*)pData2;
			XU16 id=FindAttrib(XEAB::ID,0);
			if(!id) return 0;
			XDomList list;
			pDraw->pDom->GetList(list,id);
			if(list.GetSize()<=0) return 0;
			XRect rw=pDraw->win;
			pDraw->win.SetRectEmpty();
			list[0]->GetRoundRect(pDraw->win);
			XDomNode::Handle(nOpera,pData1,pData2);
			pDraw->win=rw;
			//pDraw->pCls
			
		 }return 1;
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLArea::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	
	switch(nOpera)
	{
	
	case XDO_PAINT:
		 if(!IsVisiable()) return 0;
		 return Paint((DRAWCONTEXT*)pData1);
	case XDO_GET_RECT:
		 if(!IsVisiable()) return 0;
		 *((XRect*)pData1)=m_rect;
		 return 1;
	case XDO_HITTEST:
		 if(!IsVisiable()) return 0;
		 if(m_rect.IsRectEmpty()) return 0;
		 else
		 {
			 if(m_rect.PtInRect(pData1,pData2)) 
				 return HitTest(pData1,pData2);
			 else return 0;
		 }break;
	case XDO_LAYEROUT_CELLS:return 0;
	case XDO_LAYEROUT_CELL:	 
		 if(!IsVisiable()) return 0;
		 if(pData1)
		 {
			 DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
			 XPCTSTR str=FindAttrib(XEAB::COORDS,(XPCTSTR)XNULL);
			 if(str==XNULL) return 0;
			 XString8 strCoords(str,STRING_ATTACH);
			 XString8Array list;
			 list.SplitString(strCoords,',');
			 m_coords.RemoveAll(XFALSE);
			 for(XU32 i=0;i<list.GetSize();i++)
				m_coords.Add(list[i].ToInt());
			 strCoords.EmptyNoFree();
			 FormatCoords(pDraw);
		 }return 1;
	case XDO_GET_STYLE:
		 return (XDomA::Handle(nOpera,pData1,pData2)&~XDomItem::DOM_NODE)|XDomItem::DOM_SELECTABLE;
	}
	return XDomA::Handle(nOpera,pData1,pData2);
}

void XHTMLArea::FormatCoords(DRAWCONTEXT *pDraw)
{
    XU8 nType=FindAttrib(XEAB::SHAPE,XELT::SHA_RECT);
	int iCount=m_coords.GetSize();
	if(nType==XELT::SHA_CIRCLE)
		iCount=2;
	else if(nType==XELT::SHA_POLY)
	{
		if(iCount>0&&(iCount&1))
		{
			if(iCount>2)
			{
				m_coords.Add(m_coords[iCount-2]);
				iCount++;
			}
			else 
			{
				m_coords.RemoveAll();
				iCount=0;
			}
		}
	}
	m_rect.SetRectEmpty();
	XU8 bStart=XFALSE;
	for(int i=0;i<iCount;i+=2)
	{
		m_coords[i]+=pDraw->win.left;
		if(!bStart) 
		{
			m_rect.left=m_coords[i];
			m_rect.right=m_rect.left;
		}
		else
		{
			m_rect.left=XMIN(m_rect.left,m_coords[i]);
			m_rect.right=XMAX(m_rect.right,m_coords[i]);
		}
		if(i+1<iCount)
		{
			m_coords[i+1]+=pDraw->win.top;
			if(!bStart)
			{
				m_rect.top=m_coords[i+1];
				m_rect.bottom=m_rect.top;
			}
			else
			{
				m_rect.top=XMIN(m_rect.left,m_coords[i+1]);
				m_rect.bottom=XMAX(m_rect.right,m_coords[i+1]);
			}
		}
		bStart=XTRUE;
	}
	switch(nType)
	{
	default:
	case XELT::SHA_RECT:
		 if(m_coords.GetSize()>=4)
		 {
			 m_rect=XRect(m_coords[0],m_coords[1],
						  m_coords[2],m_coords[3]);
		 }
		 else m_rect.SetRectEmpty();
		 m_coords.RemoveAll();
		 break;
	case XELT::SHA_CIRCLE:
		 if(m_coords.GetSize()>=3)
		 {
			 m_rect=XRect(m_coords[0]-m_coords[2],
						  m_coords[1]-m_coords[2],
						  m_coords[0]+m_coords[2],
						  m_coords[1]+m_coords[2]);
		 }
		 else m_rect.SetRectEmpty();
		 m_coords.RemoveAll();
		 break;
	case XELT::SHA_POLY:
		 if(m_coords.GetSize()<6)
			 m_rect.SetRectEmpty();
	}
}

XU32 XHTMLArea::Paint(DRAWCONTEXT *pDraw)
{
   if(m_rect.IsRectEmpty()) return XFALSE;
   if(!m_rect.IsUnion(pDraw->paint)) return 0;
   XGraphics*pg=pDraw->pDraw;
   XU32 c=pg->GetColor();
   if(pDraw->pSelect==this)
	   pg->SetColor(pDraw->DCSELECTBACK);
   else	if(pDraw->pActive==this)
	   pg->SetColor(pDraw->DCACTIVE);
   else return 0;
   switch(FindAttrib(XEAB::SHAPE,XELT::SHA_RECT))
   {
   default:
   case XELT::SHA_RECT:
	    pg->FrameRectangle(m_rect);break;
   case XELT::SHA_CIRCLE:
	    pg->DrawEllipse(m_rect);break;
   case XELT::SHA_POLY:
	   if(m_coords.GetSize()>=4)
	   {
		   int bx=m_coords[0],by=m_coords[1],
			   ex=m_coords[2],ey=m_coords[3];
		   int iOffset=2;
		   while(iOffset<=(XINT)m_coords.GetSize())
		   {
			   iOffset+=2;
			   pg->DrawLine(bx,by,ex,ey);
			   bx=ex;
			   by=ey;
			   if(iOffset+1<(XINT)m_coords.GetSize())
			   {ex=m_coords[iOffset];
			    ey=m_coords[iOffset+1]; }
			   else
			   {
				ex=m_coords[0];
				ey=m_coords[1];
			   }
		   }
	   }break;
   }
   pg->SetColor(c);
   return 1;
}

XU32 XHTMLArea::HitTest(int x, int y)
{
	switch(FindAttrib(XEAB::SHAPE,XELT::SHA_RECT))
	{
	case XELT::SHA_RECT:return (XU32)this;
	case XELT::SHA_CIRCLE:
		{
			int ox=(m_rect.left+m_rect.right)>>1;
			int oy=(m_rect.top+m_rect.bottom)>>1;
			int ll=(x-ox)*(x-ox)+(y-oy)*(y-oy);
			if(ll<=(m_rect.left-ox)*(m_rect.left-ox)) return (XU32)this;
			return 0;
		};
	case XELT::SHA_POLY:
		{
			//return (XU32)this;
			int bx=m_coords[0],by=m_coords[1],
			   ex=m_coords[2],ey=m_coords[3];
		   int iOffset=2,iLeft=0,iRight=0,iCount=m_coords.GetSize()>>1;
		   while(iOffset<=(XINT)m_coords.GetSize())
		   {
			   iOffset+=2;
			   //pg->DrawLine(bx,by,ex,ey);
			   if(y>=by&&y<=ey||(y>=ey&&y<=by))
			   {
				   int dy=(ey-by);
				   if(dy==0) return (XU32)this;
				   int mx=bx+(ex-bx)*(y-by)/dy;
				   if(x==mx) return (XU32)this;
				   if(mx<x) iLeft++;
				   else     iRight++;
			   }
			   bx=ex;
			   by=ey;
			   if(iOffset+1<(XINT)m_coords.GetSize())
			   {ex=m_coords[iOffset];
			    ey=m_coords[iOffset+1]; }
			   else
			   {
				ex=m_coords[0];
				ey=m_coords[1];
			   }
		   }
		   if(iLeft==0||iRight==0) return 0;
		   while(iLeft+iRight>iCount)
		   {
			   iLeft--;
			   iRight--;
			   if(iLeft==0||iRight==0) return 0;
		   }
		   if((iLeft%2)||(iRight%2)) return (XU32)this;
		}break;
	}
	return 0;
}
