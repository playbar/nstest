// XDomItem.cpp: implementation of the XDomItem class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomItem.h"
#include "XSelection.h"
#include "XDom.h"
#include "XDomView.h"
#ifdef _SYMBIAN
#include <wchar.h>
#endif
#ifdef __APPLE__
#include "string_comp.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XU32 XDomLeaf::Handle(XU32 nOpera,XU32 pData1,XU32 pData2)
    {
	   XVar*p=(XVar*)pData1;
	   switch(nOpera)
	   {
	   case XDO_GET_ENTRIES:
		   {
			   XU16Array*p=(XU16Array*)pData1;
			   //XSortU16 st(p);
			   //st.Add(GetID(),XTRUE);
			   p->Add(GetID());
			   XU8 ic=0;
			   for(XU32 i=0;i<m_attribs.GetSize();i++)
			   {
				   switch(m_attribs[i].nID)
				   {
				   case XEAB::ID:
					    //st.Add((XU16)m_attribs[i].iData,XTRUE);
					    p->Add((XU16)m_attribs[i].iData);
						ic++;
						break;
				   case XEAB::CLASS:
					    for(int j=0;j<64;j+=16)
						{
							XU16 id=(XU16)(m_attribs[i].iData>>j);
							if(id==0) break;
							p->Add(id);
							//st.Add(id,XTRUE);
						}
					    ic++;
				   }
				   if(ic>=2) break;
			   }
		   }return XTRUE;
	   default:
		    return XDomItem::Handle(nOpera,pData1,pData2);
	   case XDO_DO_STYLE:
		    DoStyle(&((XDom*)pData1)->m_css);
			break;
	   case XDO_DO_CSS:
		    DoCSS((XCSS*)pData1);
		    return 0;
	   case XDO_GET_PARENT:
			return (XU32)m_pParent;
			break;
	   case XDO_SET_PARENT:
			m_pParent=(XDomItem*)pData1;
			break;
		case XDO_SET_FINAL:
			 if(pData2!=0)
			 {
				XEVENTDATA*p=(XEVENTDATA*)pData2;
				/*if(p->pDom!=XNULL)
				{

				}*/
				XPCTSTR strURL=FindAttrib(XEAB::BACKGROUND,(XPCTSTR)XNULL);
				if(strURL==XNULL) return XFALSE;
				XIMAGEDATA* pData=p->pDom->m_imgList.SetImage(strURL,this,p->pDom);
				if(pData==XNULL) return 0;
				XVar var(XEAB::BACKIMAGE,(XINT)pData);
				SetAttrib(var,XTRUE);
			 }
			 return XTRUE;

/*	   case XDO_GET_IMAGE:
			*p=FindAttrib(XEAB::BACKGROUND,(XPCTSTR)XNULL);
			return XTRUE;
		case XDO_SET_IMAGE:
			 //if(m_pData!=XNULL) return XFALSE;
			 //else
			 {
				 XIMAGEDATA*p=(XIMAGEDATA*)pData1;
				 p->nMethod=XEM::GET;
				 XPCTSTR pStr=FindAttrib(XEAB::BACKGROUND,(XPCTSTR)XNULL);
				 if(pStr==XNULL) return XFALSE;
				 //SetAttrib(XEAB::BACKGROUND,pData1);
				 XVar var(XEAB::BACKIMAGE,(XINT)pData1);
				 SetAttrib(var,XTRUE);
				 //XINT id=FindAttrib(XEAB::SRC);
				 //if(id<0) return XFALSE;
				 p->strURL=pStr;
				 return XTRUE;
			 }
			 return XTRUE;*/
	   case XDO_GET_STYLE:
		    return XDomItem::Handle(nOpera,pData1,pData2)|DOM_LEAF;
	   //case XDO_GET_RECT:
	   //	return GetRoundRect(*(XRect*)pData1);
	   case XDO_RESET:
			m_attribs.RemoveAll();
		    break;
	   case XDO_GET_ATTR_COUNT:
		   return m_attribs.GetSize();
	   case XDO_ADD_ATTR:
	   case XDO_SET_ATTR:
		   //if(p->nID==XEAB::XML_LANG&&p->nType==VSTRING)
		   // *p=XEAB().Index(p->strData);
		   SetAttrib(*((XVar*)pData1),nOpera==XDO_ADD_ATTR);
		   break;
	   case XDO_GET_ATTR:
		   return GetAttrib(*((XVar*)pData1));
		   break;
	   case XDO_GET_ATTR_BYINDEX:
		   if(pData2<m_attribs.GetSize())
		   {
			   *((XVar*)pData1)=m_attribs[pData2];
			   ((XVar*)pData1)->nID=m_attribs[pData2].nID;
			   return XTRUE;
		   }
		   else return XFALSE;
	   case XDO_HITTEST:
		   {
			   XRect rect;
			   if(!GetRoundRect(rect)) return XNULL;
			   return (XU32)(rect.PtInRect(pData1,pData2)?this:XNULL);
		   }
	   }
	   return 0;
   }

XU32 XDomNode::Handle(XU32 nOpera,XU32 pData1,XU32 pData2)
 {

   switch(nOpera)
   {
   default:
		return XDomLeaf::Handle(nOpera,pData1,pData2);
   case XDO_GET_RECT:
	    if(!IsVisiable()) return 0;
	    return GetRoundRect(*(XRect*)pData1,pData2);
   //case XDO_SELECTION:
	//      return XDomItem::Selection((XSelection*)pData1);
   case XDO_SELECT:
	    if(!IsVisiable()) return 0;
	    return (XU32)SelectItem((SELECTMARK*)pData1);
   case XDO_GET_STYLE:
	    //if(!IsVisiable()) return 0;
		return XDomItem::Handle(nOpera,pData1,pData2)|DOM_NODE;

   case XDO_GET_CHILDCOUNT:
		return m_childs.GetSize();
		break;
   case XDO_GET_CHILDBYINDEX:
		if(pData1>=m_childs.GetSize()) return XNULL;
		return (XU32)m_childs[pData1];
   case XDO_GET_CHILDLIST:
		return (XU32)&m_childs;
		break;
   case XDO_ADD_CHILD:
		if(pData1!=NULL)
		{
			XDomItem*pItem=(XDomItem*)pData1;
			pItem->SetParent(this);
			m_childs.Add(pItem);
			return XTRUE;
		}
		break;
   case XDO_REMOVE_CHILD:
	   {
		   XU32 id=(XU32)pData1;
		   if(id>=m_childs.GetSize()) return XFALSE;
		   m_childs.RemoveAt(id);
		   return XTRUE;
	   }break;
   case XDO_HITTEST:
	   if(IsVisiable())
	   {
		   //if(XDomItem::HitTest(pData1,pData2))
		   //if(HitRect(pData1,pData2))
			//   return (XU32)this;
		   for(XU32 i=0;i<m_childs.GetSize();i++)
		   {
			   XDomItem*p=m_childs[i]->HitTest(pData1,pData2);
			   if(p) return (XU32)p;
		   }
		   return XNULL;
	   }
	    break;
   case XDO_PAINT:
   case XDO_PAINT_TABLE:
	   if(IsVisiable())
	   {
		   DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
		   PrePaint(pDraw);
		   XBOOL bOK=XDomLeaf::Handle(nOpera,pData1,pData2);

		   for(XU32 i=0;i<m_childs.GetSize();i++)
		   {
#ifdef __ANDROID__
//			   if( (int)m_childs.GetData() > 0xFF )
//			   {
				   bOK|=m_childs[i]->Handle(nOpera,pData1,pData2);
//			   }
//			   else
//			   {
//				   LOGWHERE();
//				   break;
//			   }
#else
			   bOK|=m_childs[i]->Handle(nOpera,pData1,pData2);
#endif
		   }
		   // return bOK;

		   EndPaint(pDraw);
		   return bOK;
	   }
	    break;
   case XDO_LAYEROUT_CELLS:
   case XDO_LAYEROUT_CELL:
   case XDO_LAYEROUT_TAB:
   case XDO_LAYEROUT_TABS:
   case XDO_LAYEROUT_TABPRE:

	    if(IsVisiable())
	    {
		 //LAYEROUTDATA margin;
		 CELLDATA*pData=(CELLDATA*)pData2;
		 DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
		 PreLayerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XNULL);

		 //pData->pData=XNULL;
		 XBOOL bOK=XDomLeaf::Handle(nOpera,pData1,pData2);
		 //XBOOL bOK=XFALSE;
		 //if(pData->bNewLine)
		//	 AlignRow(pDraw,pData);

		 for(XU32 i=0;i<m_childs.GetSize();i++)
			 bOK|=m_childs[i]->Handle(nOpera,pData1,pData2);

		 EndLayerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2);
		 return bOK;
	   }
   case XDO_SETINPUT:
   case XDO_EVENT_ONTIMEROVER:
   case XDO_SELECTION:
	   if(!IsVisiable()) break;
   //case XDO_PAINT_TABLE:
  // case XDO_LAYEROUT_TAB:
  // case XDO_LAYEROUT_TABS:
  // case XDO_LAYEROUT_TABPRE:
   case XDO_SETRADIO:
   case XDO_CONNECT:
   case XDO_EVENT_ONLOAD:
   case XDO_EVENT_ONUNLOAD:
   case XDO_EVENT_ONTIMER:
   case XDO_EVENT_ONENTERFORWARD:
   case XDO_EVENT_ONENTERBACKWARD:
   case XDO_RESET:
   case XDO_DO_CSS:
   case XDO_REFRESH:
   case XDO_GET_ALLTEXT:
//   case XDO_LAYEROUT_CELLS:
 //  case XDO_LAYEROUT_CELL:

	   {
		 XBOOL bOK=XDomLeaf::Handle(nOpera,pData1,pData2);
		 for(XU32 i=0;i<m_childs.GetSize();i++)
			 bOK|=m_childs[i]->Handle(nOpera,pData1,pData2);
		 return bOK;
	   }
   }
   return 0;
}




/*void XDomItem::NewRow(XU8 h,DRAWCONTEXT *pDraw, CELLDATA *pData,XBOOL bAdd,XBOOL bTable)
{
	if(pData->bEmpty) return;
#ifdef _DEBUG
	if(pData->nRow>=(XINT)pData->rowws.GetSize())
		printf("Row Num Error!");
#endif
	pData->cur.x=pDraw->win.left;
	pData->cur.y+=pData->rowws[pData->nRow]+pDraw->nRowSpace;
	if(bAdd)
	{
		pData->nRow++;
		pData->rowws.Add(h);//pDraw->nSpace+pDraw->nRowSpace);
		if(!bTable)
		{
			pData->fixCols.Add(0);//pData->cur.x);
			pData->cols.Add(0);
		}
		//pData->nRows++;

	}
	else
	{
		pData->nRow++;
		if(pData->nRow<(XINT)pData->rowws.GetSize())
			AlignRow(pDraw,pData);
	}
	pData->bReturn=XTRUE;

} */





void XDomItem::AlignRow(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
	if(pData->nRow<(XINT)pData->fixCols.GetSize())
	{
	  int mx=pData->fixCols[pData->nRow];
	  int left=GetWinLeft(pDraw,pData);
	  int wx=GetWinRight(pDraw,pData)-left;
	  switch(pDraw->ALIGNW)//pData->alignw)
	  {
	  case XEnumAlign::LEFT:
		   pData->cur.x=left;//pDraw->win.left;
		   break;
	  case XEnumAlign::CENTER:
		   pData->cur.x=left+XMAX(((wx-mx)>>1),0);
		   break;
	  case XEnumAlign::RIGHT:
		   pData->cur.x=left+XMAX((wx-mx-1),0);
		   break;
	  }
	}
	else pData->cur.x=pDraw->win.left;
}

XU16 XDomItem::AlignCol(DRAWCONTEXT *pDraw, CELLDATA *pData,DOMPOS h,XU8 ali,DOMPOS th)
{
	XINT nPos=pData->cur.y;
	if(pData->nRow<(XINT)pData->rowws.GetSize())
	{
		int my=th<=0?pData->rowws[pData->nRow]:th;
		if(ali==0) ali=pDraw->ALIGNH;//pData->alignh;
		switch(ali)
		{
		case XEA::TOP:
			 nPos=pData->cur.y;
			 break;
		case XEA::MIDDLE:
			 nPos=pData->cur.y+XMAX(((my-h)>>1),0);
			 break;
		case XEA::BOTTOM:
			 nPos=pData->cur.y+XMAX(my-h,0);
		}
	}
	return nPos;
}

XBOOL XDomNode::GetRoundRect(XRect &rect,XU32 nType)
{
	XBOOL bOK=XFALSE;
	for(XU32 i=0;i<m_childs.GetSize();i++)
	{
		XRect rt;
		bOK|=m_childs[i]->Handle(XDO_GET_RECT,(XU32)&rt,nType);
	    rect|=rt;
	}
	return bOK;
}

XU32 XDomItem::Selection(XSelection*p)
{
	return p->Select(this);
}

XDomItem* XDomItem::SelectItem(SELECTMARK *pInfo)
{

	if(pInfo->pSelect==this)
	{
		pInfo->nCount=1;
		//pInfo->pLast=this;
		//if(pInfo->pSelect==pInfo->pText)
		//	pInfo->nTxtCount++;
	}

	else if(GetStyle()&DOM_SELECTABLE)
	{
		if(pInfo->nCount||pInfo->pSelect==XNULL)
		{
			//XRect rect;
			//GetRoundRect(rect);
			//if(!rect.IsRectEmpty())
				return this;
		}
		/*if(pInfo->pSelect==XNULL) return this;
		else if(pInfo->pLast==XNULL) return XNULL;
		if(pInfo->nDirect==SELECTINFO::XNEXT||
		   pInfo->nDirect==SELECTINFO::XLAST)
			return this;
		if(pInfo->nDirect==SELECTINFO::XDOWN||
		   pInfo->nDirect==SELECTINFO::XUP)
		{
		   XRect rt1,rt2;
		   GetRoundRect(rt1,1);
		   rt1.InflateRect(-2,-2);
		   if(rt1.IsRectEmpty()) return XNULL;
		   pInfo->pLast->GetRoundRect(rt2,1);
		   rt2.InflateRect(-2,-2);
		   if(rt1.bottom<=rt2.top||
			  rt1.top>=rt2.bottom)
		   {
			 if(pInfo->nCount>=pInfo->nLength) return pInfo->pLast;
			 pInfo->nCount++;
		     pInfo->pLast=this;
		   }
		   else	if(pInfo->pLast!=pInfo->pSelect)
		   {
			  XRect rt3;
			  pInfo->pSelect->GetRoundRect(rt3,1);
			  //rt3.InflateRect(-2
			  XINT l2=rt1.left-rt3.left;//((rt1.left+rt1.right)>>1)-((rt3.left+rt3.right)>>1);
			  l2=XABS(l2);
			  XINT l1=rt2.left-rt3.left;//((rt2.left+rt2.right)>>1)-((rt3.left+rt3.right)>>1);
			  l1=XABS(l1);
			  if(l2>=l1)
			  {
				  if(pInfo->nCount>=pInfo->nLength)
					return pInfo->pLast;
			  }
			  else pInfo->pLast=this;
		   }
		}
		else
		{
			pInfo->pLast=this;
			pInfo->nCount++;
			if(pInfo->nCount>=pInfo->nLength)
			  return this;
		}
	}
	else if(GetStyle()&DOM_STATIC&&pInfo->pNextText==XNULL)
	{
		if(pInfo->pText==this) pInfo->nTxtCount++;
		else if((pInfo->pText==XNULL||pInfo->nTxtCount)&&
			pInfo->nCount<pInfo->nLength)
		{
			pInfo->pNextText=this;
		}*/
	}
	return XNULL;
}

XDomItem* XDomNode::SelectItem(SELECTMARK *pInfo)
{
	if(m_childs.GetSize()<=0) return XNULL;
	if(//pInfo->nDirect==SELECTMARK::XUP||
	   //pInfo->nDirect==SELECTMARK::XLEFT||
	   pInfo->nDirect==SELECTMARK::XLAST)
	{
		for(XU32 i=m_childs.GetSize();i>0;i--)
		{
			XU32 id=m_childs[i-1]->Handle(XDO_SELECT,(XU32)pInfo,0);
			if(id) return (XDomItem*)id;
		}
		//if(pInfo->nCount>=pInfo->nLength)
		//	return pInfo->pLast;
	}
	else
	{
		for(XU32 i=0;i<m_childs.GetSize();i++)
		{
		   XU32 id=m_childs[i]->Handle(XDO_SELECT,(XU32)pInfo,0);
		   if(id) return (XDomItem*)id;
		}
		//if(pInfo->nCount>=pInfo->nLength)
		//	return pInfo->pLast;
	}
	return XNULL;
}



XBOOL XDomItem::Translate(XString8Array &list, XString8 &strValue)
{
   XString8 strKey;
   int is=strValue.Find("$("),ie=0;
   if(is>=0)
   {
	   ie=strValue.Find(')',is+2);
	   if(ie>is+2)
	   {
		   strValue.Mid(strKey,is+2,ie-is-2);
	   }
   }
   if(!strKey.IsEmpty())
   {
	   XString8 str;
	   strValue.Left(str,is);
	   for(XU32 i=0;i<list.GetSize();i+=2)
	   {
		   if(list[i].Compare(strKey,XTRUE)==0)
		   {
			   //strValue=list[i+1];
			   str+=list[i+1];
			   break;
		   }
	   }
	   //XString8 strTmp;
	   strValue.Mid(strKey,ie+1);
	   str+=strKey;
	   strValue=str;
	   return XTRUE;
   }
   return XFALSE;
}

//#define ISALPHA(CC) ::iswalpha(CC)
#define ISALPHA(CC) (CC<0x80&&::isalpha(CC))
//#define ISNUM(CC)	::iswdigit(CC)
#define ISNUM(CC) (CC<0x80&&::isdigit(CC))

static XWCHAR _strMark[]={',','.','?',':',';','\'','"','!',')',']','}','>',
	(XWCHAR)0xff0c,0x3002,0xff1f,/*0x201c,*/0x201d,/*0x2018,*/0x2019,0xff01,0x3001,0xff1b,0xff1a,
	0xff09,0xff3d,0xff5d,0};
#ifdef __APPLE__
	#define ISMARK(CC) (gg_wcschr(_strMark,CC)!=XNULL)
#elif defined (__ANDROID__ )
	#define ISMARK(CC) (::wcschr((wchar_t *)_strMark,CC)!=XNULL)
#elif defined (_SYMBIAN )
	#define ISMARK(CC) (::wcschr((wchar_t *)_strMark,(wchar_t)CC)!=XNULL)
#else
	#define ISMARK(CC) (::wcschr(_strMark,CC)!=XNULL)
#endif

static XWCHAR _strLeftMark[]={'<','(','{','[',0xff08,0x201c,0x2018,0xff1c,0xff3b,0xff5b,0xff1e,0};
#ifdef __APPLE__
#define ISLEFTMARK(CC) (gg_wcschr(_strLeftMark,CC)!=XNULL)
#elif defined(__ANDROID__)
	#include <wchar.h>
	#define ISLEFTMARK(CC) (wcschr((wchar_t *)_strLeftMark,CC)!=XNULL)
#elif defined(_SYMBIAN)
	#include <wchar.h>
	#define ISLEFTMARK(CC) (wcschr((wchar_t *)_strLeftMark,(wchar_t)CC)!=XNULL)
#else
#define ISLEFTMARK(CC) (wcschr(_strLeftMark,CC)!=XNULL)
#endif


XU16 XDomItem::FormatString(XGraphics&g,XString16 &strTxt, int bx, int w, XU16Array &list)
{
	XSIZE sz;

	XPCWSTR pData=strTxt;
	//XU16 cy=sz.cy;
	XU16 nMax=(XU16)(XMIN(strTxt.GetLength(),0xfffe));
	XU16 iBegin=0,iEnd=1;
	g.MeasureString(pData,sz,1);
	if(bx+sz.cx>=w)
	{
		list.Add(0);
		list.Add(0);
		list.Add(0);
		bx=0;
	}
	for(XU16 i=1;i<nMax;i++)
	{
	   g.MeasureString(pData+iBegin,sz,i-iBegin+1);
	   XBOOL bSet=XFALSE;
	   if(bx+sz.cx>=w)
	   {
		   bSet=XTRUE;
		   iEnd=i;
		   if(ISALPHA(pData[i-1]))
		   {
			  while(iEnd>=iBegin&& (ISALPHA(pData[iEnd-1])||ISLEFTMARK(pData[iEnd-1])))
				  iEnd--;
			  if(iEnd<=iBegin) iEnd=i;
			  else i=iEnd;
		   }
		   else if(ISNUM(pData[i-1]))
		   {
			  while(iEnd>=iBegin&&ISNUM(pData[iEnd-1])||ISLEFTMARK(pData[iEnd-1])) iEnd--;
			  if(iEnd<=iBegin) iEnd=i;
			  else i=iEnd;
		   }
		   else if(ISMARK(pData[i]))
		   {
			   iEnd++;
			   //while(iEnd<nMax&&ISMARK(pData[iEnd])&&iEnd-i<3) iEnd++;
				   //iEnd--;
			   i=iEnd;
		   }
		   else if(ISLEFTMARK(pData[i-1]))
		   {
			   iEnd--;
			   i=iEnd;
		   }
	   }
	   else if(pData[i]==XCR)
	   {
		   iEnd=i;
		   while(i<nMax&&pData[i]<XSP)
		     i++;
		   bSet=XTRUE;
	   }
	   if(!bSet) continue;
	   g.MeasureString(pData+iBegin,sz,iEnd-iBegin);
	   list.Add(iBegin);
	   list.Add(iEnd-iBegin);
	   list.Add((XU16)sz.cx);
	   iBegin=iEnd;
	   bx=0;
	   //XWCHAR c=strTxt[i];
	}
	if(nMax>iBegin)
	{
		g.MeasureString(pData+iBegin,sz,0);
		list.Add(iBegin);
		list.Add(nMax-iBegin);
		list.Add((XU16)sz.cx);
	}
	return (XU16)sz.cy;
}


void XDomItem::GetList(XDomList &list, XU16 id,bool bChild)
{
	if(m_nID==id)
		list.Add(this);
	if(m_nID!=id||bChild)
	{
		XU32 nCount=Handle(XDO_GET_CHILDCOUNT,0,0);
		for(XU32 i=0;i<nCount;i++)
		{
			XDomItem*p=(XDomItem*)Handle(XDO_GET_CHILDBYINDEX,i,0);
			if(p!=XNULL)
				p->GetList(list,id,bChild);
		}
	}
}

void XDomItem::DestoryChild()
{
    XU32 nc=GetChildCount();
	for(XU32 i=nc;i>0;i--)
	{
		XDomItem*p=GetChild(i-1);
		if(p==XNULL) continue;
		p->DestoryChild();
		p->Handle(XDO_REMOVE_CHILD,i-1,0);
		delete p;
	}
}

void XDomItem::PaintSelect(XRect&rt,DRAWCONTEXT *pDraw,XBOOL bForce)
{
	if(pDraw->pActive==this||pDraw->pSelect==this||bForce)
	{
		XColor cc((pDraw->pActive==this||bForce==2)?pDraw->DCACTIVE:pDraw->DCSELECTBACK);
		cc.Mix(XColor(pDraw->DCBACK),0.5f);
		XU32 oc=pDraw->pDraw->SetColor(cc);
		//rt.InflateRect(1,1);
		pDraw->pDraw->FrameRectangle(rt);
		rt.InflateRect(1,1);
		cc.Mix(XColor(pDraw->DCBACK),0.7f);
		pDraw->pDraw->SetColor(cc);
		pDraw->pDraw->FrameRectangle(rt);
		pDraw->pDraw->SetColor(oc);
		rt.DeflateRect(1,1);
	}
}

XBOOL XDomLeaf::PostVariable(XEVENTDATA *pData,XPCTSTR strValue)
{
 	XPCTSTR strName=FindAttrib(XEAB::NAME,(XPCTSTR)XNULL);
	if(strName==XNULL) return XFALSE;
	if(strValue==XNULL)
		strValue=FindAttrib(XEAB::VALUE,(XPCTSTR)XNULL);
	if(strValue==XNULL) return XFALSE;
	XString8 strv=strValue;
	if(!strv.IsEmpty()&&FindAttrib(XEAB::MD5ENC,false))
	{
		XU8Array data;
		data.Append((XU8*)strv.GetData(),strv.GetLength());
		strv.Empty();
		XDomView::GetMD5Of(data,strv);
		//strv.MakeLower();
	}

	Translate(pData->strValues,strv);
//	strv.ConvertTo(XCHARSET_UTF8);
	

	//XSortString8::SSetValue(pData->strDatas,strName,strv);
	/*XPCTSTR strInterface=FindAttrib(XEAB::XINTERFACE,(XPCTSTR)XNULL);
	if(strInterface)
	{
		if(XString8::Compare(strInterface,"time",0,true)==0)
		{
			XString8 strTime;
			XTime time;
			time.SetCurrentTime();
			strTime.FromInt(time.GetTime());
			strv+=strTime;
		}
		if(XString8::Compare(strInterface,"mtime",0,true)==0)
		{
			XString8 strTime;
			XTime time;
			time.SetCurrentTime();
			strTime.FromInt(time.GetTime());
			strv+=strTime;
			strv+="000";
		}
		else if(XString8::Compare(strInterface,"refer",0,true)==0)
		{
			pData->strRefer=strv;
			return XTRUE;
		}
	}*/
	XURL::Encode(strv);

	pData->strDatas.Add(strName);
	pData->strDatas.Add(strv);

	return XTRUE;
}

XBOOL XDomLeaf::SetAttrib(XU16 id,XPCTSTR strValue)
{
	for(XU32 i=0;i<m_attribs.GetSize();i++)
	{
		if(m_attribs[i].nID==id)
		{
			m_attribs[i]=strValue;
			return XTRUE;
		}
	}
	return XFALSE;
}
XBOOL XDomLeaf::SetAttrib(XU16 id,XU32 iValue)
{
	for(XU32 i=0;i<m_attribs.GetSize();i++)
	{
		if(m_attribs[i].nID==id)
		{
			m_attribs[i]=iValue;
			return XTRUE;
		}
	}
	return XFALSE;
}
void XDomLeaf::SetAttrib(const XVar&p,XBOOL bAdd )
{
	XINT id=FindAttrib(p.nID);
	if(id>=0)
		m_attribs[id]=p;
	else if(bAdd)
		m_attribs.Add(p);
}

XPCTSTR XDomLeaf::RefreshVar(XDomLeaf*pItem,XPCTSTR strMyKey,XEVENTDATA *pData)
{
	//if(strKey==XNULL) return XNULL;
	if(pItem==this) return XNULL;
	if(strMyKey==XNULL)
	{
		strMyKey=FindAttrib(XEAB::NAME,(XPCTSTR)XNULL);
		if(strMyKey==XNULL) return XNULL;
	}
	if(pItem!=XNULL)
	{

		XPCTSTR strKey=pItem->FindAttrib(XEAB::NAME,(XPCTSTR)XNULL);
		if(strKey==XNULL) return XNULL;
		XString8 str(strKey,STRING_ATTACH);
		if(str.Compare(strMyKey,XTRUE)!=0)
		{
			str.EmptyNoFree();
			return XNULL;
		}
		str.EmptyNoFree();
	}

	for(XU32 i=0;i<pData->strValues.GetSize();i+=2)
	{
		if(pData->strValues[i].Compare(strMyKey,XTRUE)==0)
		{
			   return pData->strValues[i+1];
		}
	}
	return XNULL;
}

XBOOL XDomLeaf::CheckName(XPCTSTR strKey)
{
	if(strKey==XNULL) return XFALSE;
	XPCTSTR str=FindAttrib(XEAB::NAME,(XPCTSTR)XNULL);
	if(str==XNULL) return XFALSE;
	XString8 strv(str,STRING_ATTACH);
	XBOOL bOK=strv.Compare(strKey)==0;
	strv.EmptyNoFree();
	return bOK;
}

XBOOL XDomLeaf::SetVariable(XEVENTDATA *pData, XPCTSTR strKey, XPCTSTR strValue)
{
	if(strKey==XNULL) strKey=FindAttrib(XEAB::NAME,(XPCTSTR)XNULL);
	if(strKey==XNULL) return XFALSE;
	if(strValue==XNULL) strValue=FindAttrib(XEAB::VALUE,(XPCTSTR)XNULL);
	if(strValue==XNULL) return XFALSE;

	//if(bFromEdit&&m_pEdit==XNULL) return XRNONE;
	if(XSortString8::SSetValue(pData->strValues,strKey,strValue))
	  return XTRUE;
	return XFALSE;
}

//DEL void _DRAWCONTEXT::OnFontChange()
//DEL {
//DEL 	XSize sz;
//DEL 	pDraw->MeasureString(XNULL,sz,0);
//DEL 	nSpace=sz.cy;
//DEL }

void XDomLeaf::PaintBack(DRAWCONTEXT *pDraw, XRect &rt,XU8 bInside)
{
	XU32 i;
	XRect rect(rt.left,rt.top,rt.right,rt.bottom);
	if(bInside)
	{
		BORDERDATA data;
		data.Init();
	    XU8 bSet=MakeBorderData(data,m_attribs,pDraw->nStatus);
		rect.left+=data.nBorderWidth[BORDERDATA::LEFT];
		rect.top+=data.nBorderWidth[BORDERDATA::TOP];
		rect.right-=data.nBorderWidth[BORDERDATA::RIGHT];
		rect.bottom-=data.nBorderWidth[BORDERDATA::BOTTOM];
	}
	XIMAGEDATA*pImage=XNULL;
	XU32 color=0;//pDraw->nStatus==STA_SELECTED?pDraw->DCSELECTBACK:0;
	//XINT posx=0,posy=0;
	XVar* ppx=XNULL,*ppy=XNULL;
	XU8  bAttach=0;
	XU8  rep=XEA::REPEAT,id=STA_NORMAL;

	for(i=0;i<m_attribs.GetSize();i++)
	{
		if((m_attribs[i].nID&0x3fff)!=XEAB::BACKIMAGE) continue;
		XU8 ss=m_attribs[i].nID>>14;
		if(ss==pDraw->nStatus||ss==STA_NORMAL)
		{
			pImage=(XIMAGEDATA*)m_attribs[i].iData;
			id=ss;
			break;
		}
		if((pDraw->nStatus==STA_SELECTED)||
		   (pDraw->nStatus==STA_NORMAL&&ss==STA_LINK)||
		   (pDraw->nStatus==STA_SELECTED&&ss!=STA_ACTIVE))
		{
			if(ss>id)
			{
				pImage=(XIMAGEDATA*)m_attribs[i].iData;
				id=ss;
			}
		}
	}
	//pImage=FindAttrib(XEAB::BACKIMAGE|(pDraw->nStatus<<14),
	for(i=0;i<m_attribs.GetSize();i++)
	{
		XU8 nStatus=(XU8)(m_attribs[i].nID>>14);
		if(nStatus!=id)	continue;
		switch(m_attribs[i].nID&0x3fff)
		{
		case XEAB::BGCOLOR:
			 color=(XU32)m_attribs[i].iData;
			 break;
		case XEAB::REPEAT:
			 rep=(XU8)m_attribs[i].iData;break;
		case XEAB::POSITIONX:
			 ppx=&m_attribs[i];break;
		case XEAB::POSITIONY:
			 ppy=&m_attribs[i];break;
			 break;
		case XEAB::ATTACH:
			 bAttach=(XU8)m_attribs[i].iData;break;
		}
	}
	//XIMAGEDATA*pImage=(XIMAGEDATA*)FindAttrib(XEAB::BACKIMAGE|(pDraw->nStatus<<14),XNULL);
	//XU8 bSet=XFALSE;
	//if(pImage==XNULL||pImage->image.IsEmpty())
	//if(0)
	if(pDraw->nStatus==STA_SELECTED&&
	   pDraw->DCSELECTBACK!=0)
	{
		XRect rt=rect;
		rt&=pDraw->paint;
		XU32 c=pDraw->pDraw->SetBackColor(pDraw->DCSELECTBACK);
		pDraw->pDraw->FillRectangle(rt);
		pDraw->pDraw->SetBackColor(c);
	}
	else {
		if(color!=0)
		{
			//pDraw->Save();
			//bSet=XTRUE;
			XRect rt=rect;
			rt&=pDraw->paint;
			XU32 c=pDraw->pDraw->SetBackColor(color);
			pDraw->pDraw->FillRectangle(rt);
			pDraw->pDraw->SetBackColor(c);
		}
	}
	//else
//	XString8 str=FindAttrib(XEAB::CLASS,"");

	if(pImage!=XNULL&&!pImage->GetImage()->IsEmpty())
	{
		//XString8 str=FindAttrib(XEAB::CLASS,"");
		XPoint pt=rect.TopLeft();
		XImage*pi=pImage->GetImage();
		XRect ri(0,0,pi->Width(), pi->Height());

		//XINT rep=FindAttrib(XEAB::REPEAT|(pDraw->nStatus<<14),0);
		//XINT pos=FindAttrib(XEAB::POSITION|(pDraw->nStatus<<14),0);
		if(bAttach)
		{
			pt.x+=pDraw->offset.x;
			while(pt.x+ri.right<rect.left)
				pt.x+=ri.right;
			pt.y+=pDraw->offset.y;
			while(pt.y+ri.bottom<rect.top)
				pt.y+=ri.bottom;
		}
		//if(pos)
		if(ppx!=XNULL)
		{
			int x=pDraw->ToPixel(*ppx,rect.Width()-ri.Width());
			if(rep!=XEA::NOREPEAT&&rep!=XEA::REPEAT_Y)
				while(x>0) x-=ri.Width();
			pt.x+=x;
		}
		if(ppy!=XNULL)
		{
			int y=pDraw->ToPixel(*ppy,rect.Height()-ri.Height());
			if(rep!=XEA::NOREPEAT&&rep!=XEA::REPEAT_X)
				while(y>0) y-=ri.Height();
			pt.y+=y;
		}
		/*{
			int y=(short)(pos>>16);
			int x=(short)(pos&0xffff);
			if(x<0) x=-(rect.Width()-ri.right)*x/100;
			if(y<0) y=-(rect.Height()-ri.bottom)*y/100;
			if(rep!=XEA::NOREPEAT&&rep!=XEA::REPEAT_Y)
				while(x>0) x-=ri.Width();
			if(rep!=XEA::NOREPEAT&&rep!=XEA::REPEAT_X)
				while(y>0) y-=ri.Height();
			pt.x+=x;
			pt.y+=y;
		} */
		for(int y=pt.y;y<rect.bottom;y+=ri.Height())
		{
			if(y+ri.Height()>pDraw->paint.top)// continue;
			{
				for(int x=pt.x;x<rect.right;x+=ri.Width())
				{
					if(x+ri.Width()>pDraw->paint.left) //continue;
					{
						XPoint dst(x,y);
						XRect rc(0,0,rect.right-x,rect.bottom-y);
						XRect rd=ri;
						if(dst.x<rect.left)
						{
							int dd=rect.left-dst.x;
							dst.x=rect.left;
							rc.left+=dd;
							rd.left+=dd;
						}
						if(dst.y<rect.top)
						{
							int dd=rect.top-dst.y;
							dst.y=rect.top;
							rc.top+=dd;
							rd.top+=dd;
						}
						rd&=rc;

						pDraw->pDraw->DrawImage(*pi,dst,rd);
					}
					if(rep==XEA::REPEAT_Y||
						rep==XEA::NOREPEAT) break;
					if(x>=pDraw->paint.right) break;
				}
			}
			if(rep==XEA::REPEAT_X||
			   rep==XEA::NOREPEAT) break;
			if(y>=pDraw->paint.bottom) break;
		}
	}
/*	XU32 color=FindAttrib(XEAB::COLOR,0);
	if(color)
	{
		if(!bSet) pDraw->Save();
		pDraw->pDraw->SetColor(color);
		bSet=XTRUE;
	}
	return bSet;*/
}

void XDomItem::SetAttribs(XVarList &list,XU16Array&ids,int nOffset,XDom*pDom)
{
//	return;

/*	if(nOffset<(int)ids.GetSize()||ids[nOffset]>=XCSS::XI_ATEXT)
	{
	//	if(ids[nOffset]==XDOM_SPAN)
	//		int a=0;
		XDomList doms;
		pDom->m_doms.GetListByID(doms,ids[nOffset],this);
		for(XU32 i=0;i<doms.GetSize();i++)
		{
			doms[i]->SetAttribs(list,ids,nOffset+1,pDom);
			//for(XU32 j=0;j<doms.GetSize();j++)
			///	doms[j]->AddAttrib(*list[i]);
		}
		if(doms.GetSize()>0) return;
	}
	XU8 bModify=XFALSE;
	for(XU32 i=0;i<list.GetSize();i++)
	{
		if(nOffset<(int)ids.GetSize())
		{
			switch(list[i].nID)
			{
			case XEAB::COLOR:
				 switch(ids[nOffset])
				 {
				 case XCSS::XI_ATEXT:
					  //.m_pDraw->DCLINK=(XU32)list[i].iData;
					  {
						  XVar var(XEAB::LINK,(XINT)list[i].iData);
						  //bModify=XTRUE;
						  AddAttrib(var);
					  }
					  return;
				 case XCSS::XI_AACTIVE:
					  //m_pDraw->DCACTIVE=(XU32)list[i].iData;
					  {
						  XVar var(XEAB::ALINK,(XINT)list[i].iData);
						  //bModify=XTRUE;
						  AddAttrib(var);
					  }
					  return;
				  case XCSS::XI_ASELECT:
				//	  m_pDraw->DCSELECTTEXT=(XU32)list[i].iData;break;
				  case XCSS::XI_ABACK:
				//	  m_pDraw->DCSELECTBACK=(XU32)list[i].iData;break;
				  case XCSS::XI_AABACK:
					   return;
				//	  break;
				 }break;
			default:
				 return;
			}
		}
		AddAttrib(list[i]);
		bModify=XTRUE;
	}*/
	/*if(bModify)
	{
		Handle(XDO_DO_STYLE,(XU32)pDom,0);
	} */
	/*if(bModify)
	{
		//XString

		XPCTSTR str=FindAttrib(XEAB::STYLE,(XPCTSTR)XNULL));
		if(str==XNULL) return;
		XString8 strValue=str;
		XVarList list1;
		pDom->m_css.PhaseCSS(list1,strValue);
		for(XU32 i=0;i<list1.GetSize();i++)
			AddAttrib(list1[i]);
	} */
}


XU8 XDomItem::NewFloatIfNeed(DRAWCONTEXT *pDraw, CELLDATA *pData,int w,XU8 bLeft)
{
	//if(pData->spans.GetSize()<=0) return 0;
	//int wx=0;
	//if(bLeft)
	int	 wx=GetWinRight(pDraw,pData)-pData->cur.x;
	//else
		//pData->spans[2]-pData->spans[0];
	if(w<=wx) return 0;
	NewFloat(pDraw,pData,bLeft);
//	NewRow(0,pDraw,pData,0);
	return 1;
}


void XDomItem::EndFloat(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
	if(pData->spans.GetSize()<4) return;
	int dy=XMAX(pData->spans[1],pData->spans[3])-pData->cur.y;
	int dx=XMAX(pDraw->win.right-pData->cur.x,0);

	if(!pData->bNewLine)
	{
		dy-=pData->rowws[pData->nRow];
		NewRow(0,pDraw,pData,0);
	}
	SetRowSize(pDraw,pData,XSize(dx,0),0,XTRUE);


	if(dy>0)
	{
		SetRowSize(pDraw,pData,XSize(0,dy),0,XTRUE);
	}
}

void XDomItem::NewFloat(DRAWCONTEXT *pDraw, CELLDATA *pData,XU8 bLeft)
{
	if(pData->spans.GetSize()<=0) {NewRow(0,pDraw,pData,0);return;}
	if(bLeft)
	{
		int dy=pData->spans[1]-pData->cur.y;//pData->rowws[pData->nRow];
		if(dy>0)
			SetRowSize(pDraw,pData,XSize(0,dy),pData->spans[0],XTRUE);
		pData->spans[0]=0;
		pData->spans[1]=0;
		NewRow(0,pDraw,pData,0);

	}
	else
	{
		int dy=pData->spans[3]-pData->cur.y;//pData->rowws[pData->nRow];
		if(dy>0)
			SetRowSize(pDraw,pData,XSize(0,dy),pData->spans[0],XTRUE);
		pData->spans[2]=0;
		pData->spans[3]=0;
		NewRow(0,pDraw,pData,0);
	}
}

void XDomItem::SetFloat(DRAWCONTEXT *pDraw, CELLDATA *pData, const XSIZE &sz,XU8 bLeft)
{
	if(pData->bEmpty)//pData->rowws.GetSize()<=0)
	{
		pData->rowws.Add(0);
		pData->fixCols.Add(0);
		pData->bEmpty=XFALSE;
	}
	if(pData->spans.GetSize()<4)
		//pData->spans.InsertAt(0,0,4);
	{
		pData->spans.Add(pDraw->win.left);
		pData->spans.Add(pDraw->win.top);
		pData->spans.Add(pDraw->win.right);
		pData->spans.Add(pDraw->win.top);
	}
	if(pData->max.cy<pData->cur.y+sz.cy)
		pData->max.cy=pData->cur.y+sz.cy;
	if(pData->nMin<sz.cx)
		pData->nMin=sz.cx;

	if(bLeft)
	{
		pData->cur.x+=sz.cx;
		/*if(bInit)
		{
			if(pData->fixCols[pData->nRow]<pData->cur.x-pDraw->win.left)
			pData->fixCols[pData->nRow]=pData->cur.x-pDraw->win.left;
		}*/
		pData->spans[0]=pData->cur.x;
		pData->spans[1]=XMAX(pData->spans[1],pData->cur.y+sz.cy);
		if(pData->max.cx<pData->cur.x+sz.cx)
			pData->max.cy=pData->cur.y+sz.cy;
	}
	else
	{
		pData->spans[2]=GetWinRight(pDraw,pData)-sz.cx;
		pData->spans[3]=XMAX(pData->spans[3],pData->cur.y+sz.cy);
		if(pData->max.cx<pDraw->win.right)
			pData->max.cx=pDraw->win.right;
	}


}


int XDomItem::GetWinWidth(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
	int l=pDraw->win.left;
	if(pData->spans.GetSize()>=2&&pData->cur.y<pData->spans[1])
		l=pData->spans[0];
	int r=pDraw->win.right;
	if(pData->spans.GetSize()>3&&pData->cur.y<pData->spans[3])
		r=pData->spans[2];
	return r-l;
}

int XDomItem::GetWinLeft(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
    if(pData->spans.GetSize()<2)
		return pDraw->win.left;
	else if(pData->cur.y<pData->spans[1])
		return pData->spans[0];
	else return pDraw->win.left;
}

int XDomItem::GetWinRight(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
    if(pData->spans.GetSize()<4)
		return pDraw->win.right;
	else if(pData->cur.y<pData->spans[3])
		return pData->spans[2];
	else return pDraw->win.right;
}

XBOOL XDomItem::NewRowIfNeed(DRAWCONTEXT *pDraw, CELLDATA *pData, int w)
{
	if(pData->bNewLine) return XFALSE;
	if(!pDraw->IsWrap()) return XFALSE;
	if(pData->cur.x+w<=GetWinRight(pDraw,pData)) return XFALSE;
	NewRow(0,pDraw,pData,0);
	return XTRUE;
}
void XDomItem::NewRow(int h,DRAWCONTEXT *pDraw, CELLDATA *pData,DOMPOS ind)
{
	XU8 bNew=XFALSE;
	int nNewRow=pData->bEmpty?0:pData->nRow+1;
	if(nNewRow>=(XINT)pData->rowws.GetSize()
	  ||(pData->bEmpty&&pData->rowws.GetSize()==0))
	{
		pData->rowws.Add(h);
		pData->fixCols.Add(0);
		bNew=XTRUE;
	}
	if(!pData->bEmpty)
	{
		pData->cur.y+=pData->rowws[pData->nRow];
		pData->nRow++;
	}
	else
	{
		pData->bEmpty=XFALSE;
		pData->nRow=0;
	}
	pData->cur.x=GetWinLeft(pDraw,pData)+ind;
	if(!bNew)
	{
		AlignRow(pDraw,pData);
		if(pDraw->ALIGNW==XEA::LEFT)
			pData->cur.x+=ind;
	}
	//else if(pData->ind!=0)
	if(pData->spans.GetSize()>0)
	{
		if(pData->cur.y>=pData->spans[1]&&
		   pData->cur.y>=pData->spans[3])
		   pData->spans.RemoveAll();
	}

	pData->bNewLine=XTRUE;

}

void XDomItem::SetRowSize(DRAWCONTEXT *pDraw, CELLDATA *pData, const XSIZE &sz,DOMPOS nMin,XU8 bInit)
{
	//XSize sz(szz.cx,XMAX(szz.cy,pDraw->SPACE));
	if(pData->bEmpty||pData->rowws.GetSize()<=0)
	{
		pData->rowws.Add(sz.cy);
		pData->fixCols.Add(0);
		pData->bEmpty=XFALSE;
		//NewRow(sz.cy,pDraw,pData);
	}
	pData->bNewLine=XFALSE;
	if(pData->nMin<nMin)
		pData->nMin=nMin;
	pData->cur.x+=sz.cx;
	if(pData->max.cy<pData->cur.y+sz.cy)
		pData->max.cy=pData->cur.y+sz.cy;
	if(pData->max.cx<pData->cur.x)
		pData->max.cx=pData->cur.x;

//	if(pData->max.cx>pDraw->win.right)
//		int a=0;
#ifdef _DEBUG
	if(pData->nRow>=(XINT)pData->rowws.GetSize())
		printf("Row Num Error!");
		//TRACE0("Row Num Error");
#endif
	if(bInit)
	{
		if(pData->rowws[pData->nRow]<sz.cy)
			pData->rowws[pData->nRow]=sz.cy;
		if(pData->fixCols[pData->nRow]<pData->cur.x-pDraw->win.left)
			pData->fixCols[pData->nRow]=pData->cur.x-pDraw->win.left;
	}
}

void XDomItem::SetTabRow(DRAWCONTEXT *pDraw,CELLDATA*pData,int w,int h)// CELLDATA *pData, XSIZE &sz, int nMin, XU8 bCells)
{
	if(pData->bEmpty)
	{
		pData->nRow=0;
		if(pData->rowws.GetSize()<=0)
			pData->rowws.Add(h);
		pData->bEmpty=XFALSE;
	}

	//pData->bReturn=XFALSE;
	//if(pData->nMin<nMin)
	//	pData->nMin=nMin;
	pData->cur.x+=w;
	/*if(pData->max.cy<pData->cur.y+sz.cy)
		pData->max.cy=pData->cur.y+sz.cy;
	if(pData->max.cx<pData->cur.x)
		pData->max.cx=pData->cur.x;*/

#ifdef _DEBUG
	if(pData->nRow>=(XINT)pData->rowws.GetSize())
		printf("Row Num Error!");
		//TRACE0("Row Num Error");
#endif
	//if(bInit)
	{
		if(pData->rowws[pData->nRow]<h)
			pData->rowws[pData->nRow]=h;
		//if(pData->fixCols[pData->nRow]<sz.cx)pData->cur.x-pDraw->win.left)
		//	pData->fixCols[pData->nRow]=w;//pData->cur.x-pDraw->win.left;
	}

	//SetRowSize(pDraw,pData,sz,nMin,bCells);
}

void XDomItem::NewTabRow(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
	XU8 bNew=XFALSE;
	if(pData->nRow<(XINT)pData->rowws.GetSize()
		||(pData->bEmpty&&pData->rowws.GetSize()==0))
	{
		pData->rowws.Add(0);
		bNew=XTRUE;
	}
	pData->cur.x=pDraw->win.left;

	if(!pData->bEmpty)
	{
		pData->cur.y+=pData->rowws[pData->nRow];
		pData->nRow++;
	}
	else
	{
		pData->bEmpty=XFALSE;
		pData->nRow=0;
	}
	//if(!bNew) AlignRow(pDraw,pData);

	pData->bNewLine=XTRUE;

}




XU8 XDomItem::NewRowIfMust(DRAWCONTEXT*pDraw,CELLDATA*pData,int w)
{
	if(pData->bNewLine)
	{
		if(pData->spans.GetSize()<=0) return XFALSE;
		//int wx=pData->spans[2]-pData->spans[0];
		//if(wx<w)
		if(pData->cur.x+w>GetWinRight(pDraw,pData))
		{
			//NewFloat(pDraw,pData,XTRUE);
			EndFloat(pDraw,pData);
			NewRow(0,pDraw,pData,pDraw->IND);
			return 2;
		}
		return 0;
	}
	//EndFloat(pDraw,pData);
	NewRow(0,pDraw,pData,pDraw->IND);
	return 1;
}

void XDomItem::NewRowMust(DRAWCONTEXT *pDraw, CELLDATA *pData,XU8 nType)
{
   if(nType==0) return;
	//NewRow(0,pDraw,pData);
   if(nType==2) //pData->bNewLine&&pData->spans.GetSize()>0)
	   //NewFloat(pDraw,pData,XTRUE);
   {
	   EndFloat(pDraw,pData);
	   //NewRow(0,pDraw,pData);
   }
   //else
   {
	  NewRow(0,pDraw,pData,pDraw->IND);
   }

}


XU32 XDomLeaf::PrePaint(DRAWCONTEXT *pDraw)
{
	XU32 s=0;
	XGraphics*g=pDraw->pDraw;
	pDraw->SaveData();
	if(pDraw->nStatus==STA_SELECTED)
		g->SetBackColor(pDraw->DCSELECTBACK);
	else
		g->SetBackColor(0);
	for(XU32 i=0;i<m_attribs.GetSize();i++)
		SetDrawContext(pDraw,m_attribs[i]);
	/*if(pDraw->pCss!=XNULL)
	{
		XVarList attribs;
		pDraw->pCss->GetAttribs(this,attribs);
		for(XU32 i=0;i<attribs.GetSize();i++)
			s|=SetDrawContext(pDraw,attribs[i]);
	}*/
	//if(bPaint)
	{
		//g->SetBackColor(pDraw->DCBACK);
		//XU8 id[]={XCW_TEXT,XCW_LINKTEXT,XCW
		g->SetColor(pDraw->paintData.nTextColor[pDraw->nStatus]);
		XU32 fs=((XU8*)&pDraw->FONTSIZEALL)[pDraw->nStatus]<<8;
		//g->SetFontSize(fs);
		fs|=((XU8*)&pDraw->FONTSTYLE)[pDraw->nStatus];
		g->SetFontStyle(fs);
	}
	return s;
	/*if(bPaint)
	{
		//pDraw->nStyles.Push(pDraw->nStatus);
		XU32 c=FindAttrib(XEAB::BGCOLOR,0);
		//XGraphics* g=pDraw->pDraw;
		if(c)
		{
			s|=XC_SAVEBACK;
			pDraw->nColors.Push(g->SetBackColor(c));
		}
		c=FindAttrib(XEAB::TEXT,0);
		if(!c)
		c=FindAttrib(XEAB::COLOR,0);
		if(c)
		{
			s|=XC_SAVETEXT;
			pDraw->nColors.Push(g->SetColor(c));
		}
		c=FindAttrib(XEAB::ALINK,0);
		if(c)
		{
		   s|=XC_SAVEACTIVE;
		   pDraw->nColors.Push(pDraw->DCACTIVE);
		   pDraw->DCACTIVE=c;
		}
		c=FindAttrib(XEAB::LINK,0);
		if(c)
		{
			s|=XC_SAVELINK;
			pDraw->nColors.Push(pDraw->DCLINK);
			pDraw->DCLINK=c;
		}
	}
	XU8  nSize=FindAttrib(XEAB::SIZE,0);
	if(nSize>0&&nSize<=10)

	{
		s|=XC_SAVEFONT;
		pDraw->nColors.Push(g->GetFontStyle());
		g->SetFontSize(nSize);
		//pDraw->OnFontChange();
	}
	nSize=FindAttrib(XEAB::TEXT_DECORATION,XELT::DEC_EMPTY);
	if(nSize)
	{
		XU32 fs=g->GetFontStyle();
		if(!(s&XC_SAVEFONT))
		{
			s|=XC_SAVEFONT;
			pDraw->nColors.Push(fs);
		}
		SetFontStyle(fs,nSize);
		g->SetFontStyle(fs);
	}
	XU8 ss=FindAttrib(XEAB::TEXT_DECORATION,XELT::DEC_EMPTY);
	if(ss)
	{
//		s|=XC_SAVEADEC;
		XU32 fs=pDraw->pDraw->GetFontSize();
		if(!(s&XC_SAVEFONT))
		{
			s|=XC_SAVEFONT;
			pDraw->nColors.Push(fs);
		}
		SetFontStyle(fs,ss);
		pDraw->pDraw->SetFontStyle(fs);
		//pDraw->nColors.Push(pDraw->nDecLink);
		//pDraw->nDecLink=ss;
	}

	return s;*/
}


void XDomLeaf::EndPaint(DRAWCONTEXT *pDraw)
{
	XGraphics*g=pDraw->pDraw;
	/*if(s&XC_SAVESIZE) pDraw->nColors.Pop(pDraw->nFontSize);
	if(s&XC_SAVESTYLE) pDraw->nColors.Pop(pDraw->nFontStyle);
	if(s&XC_SAVESELECT)
		pDraw->nColors.Pop(pDraw->nSelectBack);
	if(s&XC_SAVELINK)
		pDraw->nColors.Pop(pDraw->DCLINK);
	if(s&XC_SAVEACTIVE) pDraw->nColors.Pop(pDraw->DCACTIVE);
	if(s&XC_SAVETEXT)
		pDraw->nColors.Pop(pDraw->DCTEXT);
	if(s&XC_SAVEBACK)
		pDraw->nColors.Pop(pDraw->DCBACK);*/
	pDraw->RestoreData();
	if(pDraw->nStatus!=STA_SELECTED)
		//g->SetBackColor(pDraw->DCSELECTBACK);
	//else
		g->SetBackColor(0);
	//g->SetBackColor(0);
	g->SetColor(pDraw->paintData.nTextColor[pDraw->nStatus]);
	XU32 fs=((XU8*)&pDraw->FONTSIZEALL)[pDraw->nStatus]<<8;
	fs|=((XU8*)&pDraw->FONTSTYLE)[pDraw->nStatus];
	g->SetFontStyle(fs);
}

XU32 XDomLeaf::PreLayerout(DRAWCONTEXT *pDraw,CELLDATA *pData,LAYEROUTDATA*pd)
{
	XU32 s=0;
//	pDraw->paintData.nWinLeft=pDraw->win.left;
//	pDraw->paintData.nWinRight=pDraw->win.right;
	pDraw->SaveData();
	//pDraw->PADDING=1;
	//pDraw->SPACING=1;
	pDraw->DROWSPAN=1;
	pDraw->DCOLSPAN=1;
//	pDraw->ALIGNW=XEAB::LEFT;
//	pDraw->paintData.nWidth=0;
//	pDraw->paintData.nHeight=0;
	pDraw->SETCLEAR=XELT::FLOAT_NONE;
	pDraw->paintData.nFloat=XELT::FLOAT_NONE;
	pDraw->SETWIDTH=0;
	pDraw->SETHEIGHT=0;
//	data.Init();
	//pd->Init();
	//LAYEROUTDATA data;
	//data.Init();
//	pDraw->paintData.nMargin[0]=pDraw->paintData.nMargin[1]=
//		pDraw->paintData.nMargin[2]=pDraw->paintData.nMargin[3]=0;
	pData->pData=pd;
	if(pd!=XNULL) pd->Init();
	for(XU32 i=0;i<m_attribs.GetSize();i++)
		s|=SetLayerData(pDraw,pd,m_attribs[i]);

	XU32 fs=((XU8*)&pDraw->FONTSIZEALL)[pDraw->nStatus]<<8;
	fs|=((XU8*)&pDraw->FONTSTYLE)[pDraw->nStatus];
	pDraw->pDraw->SetFontStyle(fs);
	return s;
}

void XDomLeaf::EndLayerout(DRAWCONTEXT *pDraw, CELLDATA *pData)
{
   //pData->cur.x+=pDraw->paintData.nMargin[1];
   //pData->cur.y+=pDraw->paintData.nMargin[3];
   pDraw->RestoreData();
   pData->pData=XNULL;
   //pDraw->win.left=pDraw->paintData.nWinLeft;
   //pDraw->win.right=pDraw->paintData.nWinRight;
   XU32 fs=((XU8*)&pDraw->FONTSIZEALL)[pDraw->nStatus]<<8;
   fs|=((XU8*)&pDraw->FONTSTYLE)[pDraw->nStatus];
   pDraw->pDraw->SetFontStyle(fs);
}


XBOOL XDomItem::IsLeafOf(XDomItem *p)
{
	if(p==XNULL) return XTRUE;
	XDomItem*pp=GetParent();
	while(pp!=p)
	{
		if(pp==XNULL) return XFALSE;
		pp=pp->GetParent();
	}
	return XTRUE;
}





void XDomLeaf::SetFontStyle(XU8 &fs, XU8 nMode)
{
		switch(nMode)
		{
		case XELT::DEC_NONE:
			 fs&=(XGraphics::XF_BOLD|XGraphics::XF_ITALIC);
			 break;
		case XELT::DEC_UNDERLINE:
			 fs|=XGraphics::XF_UNDERLINE;
			 break;
		case XELT::DEC_MIDLINE:
			 fs|=XGraphics::XF_STRIKE;
			 break;
		case XELT::DEC_UPLINE:
			 fs|=XGraphics::XF_UPLINE;
			 break;
		case XELT::DEC_BLINK:
			 fs|=XGraphics::XF_BLINK;
			 break;
		}

}

XU32 XDomLeaf::OnTimer(XEVENTDATA *pData, XRect &rt)
{
	if(!rt.IsUnion(pData->viewRect)) return XRNONE;
	XBOOL bSet=XFALSE;
	for(XU32 i=0;i<m_attribs.GetSize();i++)
	{
		if((m_attribs[i].nID&0x3fff)!=XEAB::BACKIMAGE) continue;
		XIMAGEDATA*p=(XIMAGEDATA*)m_attribs[i].iData;
		if(p==XNULL) continue;
		if(p->bModify)
		{
			bSet=XTRUE;
			break;
		}
	}

	if(!bSet) return XRNONE;

	pData->rect|=rt;
	return XRREPAINT;
}

XU8 XDomItem::GetStatus(DRAWCONTEXT *pDraw)
{
	if(pDraw->nStatus) return pDraw->nStatus;
	else if(pDraw->pSelect==this) return STA_SELECTED;
	else if(pDraw->pActive==this) return STA_ACTIVE;

	else if(GetStyle()&DOM_ANCHOR) return STA_LINK;
	else return STA_NORMAL;
}

XU32 XDomLeaf::SetDrawContext(DRAWCONTEXT *pDraw, XVar &var)
{
	XU8 nStatus=var.nID>>14;
	XU8 bSet=XFALSE;
    switch(var.nID&0x3fff)
	  {
  	   case XEAB::LIST_STYLE_TYPE:
		    pDraw->paintData.nListType=(XU8)var.iData;
			break;

	  case XEAB::FONT_WEIGHT:
		   SetFontType(pDraw->FONTSTYLE,XGraphics::XF_BOLD,nStatus,var.iData==XELT::FWT_BOLD);
		   break;
	  case XEAB::FONT_STYLE:
		   SetFontType(pDraw->FONTSTYLE,XGraphics::XF_ITALIC,
					   nStatus,var.iData==XELT::FS_ITALIC);
		   break;
	  case XEAB::FONT_SIZE:
		   if(nStatus==STA_NORMAL)
			  pDraw->FONTSIZEALL=(((XU8)var.iData)<<24)|
			                  (((XU8)var.iData)<<16)|
							  (((XU8)var.iData)<<8)|
							  (((XU8)var.iData));
		   else
			 ((XU8*)&pDraw->FONTSIZEALL)[nStatus]=(XU8)var.iData;
		   break;
	  case XEAB::BGCOLOR:
		   if(nStatus==STA_NORMAL)
			   pDraw->DCBACK=(XU32)var.iData;
		   else	if(nStatus==STA_SELECTED)
			   pDraw->DCSELECTBACK=(XU32)var.iData;
		   //if(nStatus==pDraw->nStatus)
			//   pDraw->pDraw->SetBackColor((XU32)var.iData);
		   bSet=XTRUE;
		   break;
	  case XEAB::COLOR:
		   if(nStatus==STA_NORMAL)
		   {
			   pDraw->paintData.nTextColor[STA_NORMAL]=(XU32)var.iData;
			   pDraw->paintData.nTextColor[STA_LINK]=(XU32)var.iData;
			   //pDraw->paintData.nTextColor[STA_NORMAL]=(XU32)var.iData;
			   //pDraw->paintData.nTextColor[STA_NORMAL]=(XU32)var.iData;
		   }
		   else
			  pDraw->paintData.nTextColor[nStatus]=(XU32)var.iData;
		   bSet=XTRUE;
		   break;
	  case XEAB::TEXT:
		   pDraw->DCTEXT=(XU32)var.iData;
		   break;
	  case XEAB::ALINK:
		   pDraw->DCLINK=(XU32)var.iData;
		   bSet=XTRUE;
		   break;
	  case XEAB::TEXT_DECORATION:
		   if(nStatus==STA_NORMAL)
		   {
			   SetFontStyle(((XU8*)&pDraw->FONTSTYLE)[STA_NORMAL],(XU8)var.iData);
			   SetFontStyle(((XU8*)&pDraw->FONTSTYLE)[STA_LINK],(XU8)var.iData);
			   //SetFontStyle(((XU8*)&pDraw->FONTSTYLE)[STA_ACTIVE],(XU8)var.iData);
			   //SetFontStyle(((XU8*)&pDraw->FONTSTYLE)[STA_SELECTED],(XU8)var.iData);
		   }
		   else
				SetFontStyle(((XU8*)&pDraw->FONTSTYLE)[nStatus],(XU8)var.iData);
		   bSet=XTRUE;
		   break;
	  //case XEAB::
	}
	return bSet;
}

void XDomLeaf::SetFontType(XU32 &s, XU8 nType, XU8 nStatus,XU8 bSet)
{
   if(nStatus==STA_NORMAL)
   {
	   for(int i=0;i<4;i++)
	   {
		   if(bSet)
			((XU8*)&s)[i]|=nType;
		   else
			((XU8*)&s)[i]&=~nType;
	   }
   }
   else
   {
	   if(bSet)
			((XU8*)&s)[nStatus]|=nType;
	   else
			((XU8*)&s)[nStatus]&=~nType;
   }
}

void XDomLeaf::CalcLineData(BORDERDATA&data,XRect&rect,int&bx,int&by,int&ex,int&ey,
				  XU8 nDir,XU8 nStep,XU8 bFull)
{
	int tt=nStep*100/data.nBorderWidth[nDir];
	if(bFull) tt=100;//nStep=data.nBorderWidth[nDir];}
	switch(nDir)
	{
	case BORDERDATA::TOP:
		 by=ey=rect.top-nStep;
		 bx=rect.left-data.nBorderWidth[BORDERDATA::LEFT]*tt/100;
		 ex=rect.right+data.nBorderWidth[BORDERDATA::RIGHT]*tt/100;
		 //if(bFull) ex++;
		 break;
	case BORDERDATA::BOTTOM:
		 by=ey=rect.bottom+nStep;
		 bx=rect.left-data.nBorderWidth[BORDERDATA::LEFT]*tt/100;
		 ex=rect.right+data.nBorderWidth[BORDERDATA::RIGHT]*tt/100;
		 //if(bFull) {ex++;ey++;by++;}
		 break;
	case BORDERDATA::LEFT:
		 bx=ex=rect.left-nStep;
		 by=rect.top-data.nBorderWidth[BORDERDATA::TOP]*tt/100;
		 ey=rect.bottom+data.nBorderWidth[BORDERDATA::BOTTOM]*tt/100;
		 //if(bFull) ey++;
		 break;
	case BORDERDATA::RIGHT:
		 bx=ex=rect.right+nStep;
		 by=rect.top-data.nBorderWidth[BORDERDATA::TOP]*tt/100;
		 ey=rect.bottom+data.nBorderWidth[BORDERDATA::BOTTOM]*tt/100;
		 /*if(bFull)
		 {
			 bx++;
			 ex++;
			 ey++;
		 } */
		 break;

	}
}

void XDomLeaf::PaintBorder(DRAWCONTEXT *pDraw, XRect &r)
{
   BORDERDATA data;
   data.Init();

   XU8 bSet=MakeBorderData(data,m_attribs,pDraw->nStatus);
   XRect rect(r.left+data.nBorderWidth[BORDERDATA::LEFT],
			  r.top+data.nBorderWidth[BORDERDATA::TOP],
			  r.right-data.nBorderWidth[BORDERDATA::RIGHT]-1,
			  r.bottom-data.nBorderWidth[BORDERDATA::BOTTOM]-1);
   XRect rect1(rect);rect1.right++;rect1.bottom++;
   if(!bSet) return;
   XGraphics* g=pDraw->pDraw;
   XU32 c=g->GetColor();
   for(int i=0;i<4;i++)
   {
	   if(data.nBorderWidth[i]<=0) continue;
	   XU8 bOdd=data.nBorderWidth[i]&1;
	   switch(data.nBorderStyle[i])
	   {
	   case XELT::LS_NONE:
	   case XELT::LS_HIDDEN:
	   case XELT::LS_EMPTY:
		    continue;
	   case XELT::LS_GROOVE:
	   case XELT::LS_RIDGE:
		   {
			   int bx,by,ex,ey;
			   XColor color(data.nColor[i]);
			   //color.Dark(20);
			   int nStep=120/data.nBorderWidth[i];
			   for(int j=0;j<data.nBorderWidth[i];j++)
			   {
				   int ns=j;
				   if(data.nBorderStyle[i]==XELT::LS_RIDGE)
					   ns=(i==BORDERDATA::TOP||i==BORDERDATA::LEFT)?j:(data.nBorderWidth[i]-j-1);
				   else
					 ns=(i==BORDERDATA::TOP||i==BORDERDATA::LEFT)?(data.nBorderWidth[i]-j-1):j;
				   CalcLineData(data,rect,bx,by,ex,ey,i,ns+1,XFALSE);
				   g->SetColor(color);

				   color.Bright(nStep);
				   g->DrawLine(bx,by,ex,ey);
			   }
		   }
		    break;
	   case XELT::LS_INSET:
	   case XELT::LS_OUTSET:
		   {
			   int bx,by,ex,ey;
			   XColor color(data.nColor[i]);
			   //color.Dark(20);
			   int nStep=120/data.nBorderWidth[i],ms=data.nBorderWidth[i]>>1;
			   for(int j=0;j<=ms;j++)
			   {
				   g->SetColor(color);
				   if(data.nBorderStyle[i]==XELT::LS_INSET)
					  color.Bright(nStep);
				   else
					  color.Dark(nStep);
				   CalcLineData(data,rect,bx,by,ex,ey,i,j+1,XFALSE);
				   g->DrawLine(bx,by,ex,ey);
				   CalcLineData(data,rect,bx,by,ex,ey,i,data.nBorderWidth[i]-j,XFALSE);
				   g->DrawLine(bx,by,ex,ey);
			   }
		   }
		    break;
	   case XELT::LS_DOUBLE:
		   {
			   int bx,by,ex,ey;
			   CalcLineData(data,rect,bx,by,ex,ey,i,1,XFALSE);
			   g->SetColor(data.nColor[i]);
			   g->DrawLine(bx,by,ex,ey);
			   CalcLineData(data,rect,bx,by,ex,ey,i,data.nBorderWidth[i],XFALSE);
			   g->DrawLine(bx,by,ex,ey);
		   }break;
	   case XELT::LS_DASHED:
		   {
			   int bx,by,ex,ey;
			   CalcLineData(data,bOdd?rect:rect1,bx,by,ex,ey,i,(data.nBorderWidth[i]+1)>>1,XTRUE);
			   g->SetColor(data.nColor[i]);
			   g->DrawLine(bx,by,ex,ey,data.nBorderWidth[i],XDLS_DASH);
		   }break;
	   case XELT::LS_DOTTED:
		   {
			   int bx,by,ex,ey;
			   CalcLineData(data,bOdd?rect:rect1,bx,by,ex,ey,i,(data.nBorderWidth[i]+1)>>1,XTRUE);
			   g->SetColor(data.nColor[i]);
			   g->DrawLine(bx,by,ex,ey,data.nBorderWidth[i],XDLS_DOT);
		   }break;
	   case XELT::LS_SOLID:
		   {
			   int bx,by,ex,ey;
			   CalcLineData(data,bOdd?rect:rect1,bx,by,ex,ey,i,(data.nBorderWidth[i]+1)>>1,XTRUE);
			   g->SetColor(data.nColor[i]);
			   g->DrawLine(bx,by,ex,ey,data.nBorderWidth[i]);
		   }break;
	   }
   }
   g->SetColor(c);
}

XU32 XDomLeaf::SetLayerData(DRAWCONTEXT *pDraw, LAYEROUTDATA*pd, XVar &var)
{
	XU8 nStatus=var.nID>>14;
	XU8 bSet=XFALSE;
	int id=var.nID&0x3fff;
    switch(id)
	  {
	   case XEAB::LIST_STYLE_TYPE:
		    pDraw->paintData.nListType=(XU8)var.iData;
			break;
	   case XEAB::MARGIN_RIGHT:
		    //if(var.iData<0||var.iData>128) break;
	   case XEAB::MARGIN_TOP:
	   		//if(var.iSign) break;
	   case XEAB::MARGIN_BOTTOM:
	   case XEAB::MARGIN_LEFT:
		    if(pd!=XNULL)
				pd->nMargin[id-XEAB::MARGIN_TOP]=
				(short)pDraw->ToPixel(var,pDraw->win.Width());//XFALSE);
		    break;
	   case XEAB::PADDING_TOP:
	   case XEAB::PADDING_LEFT:
	   case XEAB::PADDING_RIGHT:
	   case XEAB::PADDING_BOTTOM:
		    if(pd!=XNULL&&!pDraw->bCollapse)
				pd->nPadding[id-XEAB::PADDING_TOP]=
				(XU8)pDraw->ToPixel(var,pDraw->win.Width());
		    break;
	   case XEAB::CLEAR:
			pDraw->SETCLEAR=(XU8)var.iData;break;
	   case XEAB::FLOAT:
		    if(pDraw->nLayerMode!=LA_SAMPLE)
		    pDraw->SETFLOAT=(XU8)var.iData;break;

	   case XEAB::FONT_HEIGHT:
		   {
			   /*int v=(int)var.iData;
			   if(v<0) v=-pDraw->pDraw->m_nTextHeight*v/100;
			   if(v<12) v=12;
			   else if(v>32) v=32;*/
			   pDraw->SPACE=(XU8)pDraw->ToPixel(var,pDraw->pDraw->m_nTextHeight);break;
		   }break;
	   case XEAB::WIDTH:
		    if(pDraw->nLayerMode!=LA_SAMPLE)
				pDraw->SETWIDTH=(DOMPOS)var.iValue;
			break;
	   case XEAB::HEIGHT:
		    if(pDraw->nLayerMode!=LA_SAMPLE)
				pDraw->SETHEIGHT=(DOMPOS)var.iValue;break;
	   case XEAB::ROWSPAN:
		    pDraw->DROWSPAN=XMAX((XU8)var.iData,1);
		    break;
	   case XEAB::COLSPAN:
		    pDraw->DCOLSPAN=XMAX((XU8)var.iData,1);
		    break;
	   case XEAB::NOWRAP:
		    pDraw->WRAPFLAG=(XU8)var.iData;break;
	   case XEAB::CELLPADDING:
		    pDraw->PADDING=(XU8)var.iData;break;
	   case XEAB::CELLSPACING:
		 	pDraw->SPACING=(XU8)var.iData;break;
	   case XEAB::TEXT_ALIGN:
	   case XEAB::ALIGN:
		    pDraw->ALIGNW=(XU8)var.iData;break;
	   case XEAB::VALIGN:
		    pDraw->ALIGNH=(XU8)var.iData;break;
	   //case XEAB::TEXT_ALIGN:
		//	pDraw->ALIGNH=(XU8)var.iData;break;
	   case XEAB::BORDER_TOPWIDTH:
	   case XEAB::BORDER_RIGHTWIDTH:
	   case XEAB::BORDER_LEFTWIDTH:
	   case XEAB::BORDER_BOTTOMWIDTH:
		     //if(pDraw->nStatus!=nStatus) break;
		    if(pd!=XNULL)
				pd->nBorderWidth[id-XEAB::BORDER_TOPWIDTH]=
					(XU8)pDraw->ToPixel(var,pDraw->win.Width());
			 bSet=XTRUE;
			 break;
	  case XEAB::FONT_WEIGHT:
		   SetFontType(pDraw->FONTSTYLE,XGraphics::XF_BOLD,nStatus,var.iData==XELT::FWT_BOLD);
		   break;
	  case XEAB::FONT_STYLE:
		   SetFontType(pDraw->FONTSTYLE,XGraphics::XF_ITALIC,
					   nStatus,var.iData==XELT::FS_ITALIC);
		   break;
	  case XEAB::FONT_SIZE:
		   if(nStatus==STA_NORMAL)
			  pDraw->FONTSIZEALL=(((XU8)var.iData)<<24)|
			                  (((XU8)var.iData)<<16)|
							  (((XU8)var.iData)<<8)|
							  (((XU8)var.iData));
		   else
			 ((XU8*)&pDraw->FONTSIZEALL)[nStatus]=(XU8)var.iData;
		   break;
	  case XEAB::TEXT_INDENT:
		   pDraw->IND=pDraw->ToPixel(var,pDraw->win.Width());
		   break;
	  case XEAB::TEXT_DECORATION:
		   SetFontStyle(((XU8*)&pDraw->FONTSTYLE)[nStatus],(XU8)var.iData);
		   bSet=XTRUE;
		   break;
	  //case XEAB::
	}
	return 0;
}

XU8 XDomLeaf::MakeBorderData(BORDERDATA &data,XVarList&list,XU8 nStatus)
{
	XU8 bSet=XFALSE;
	for(XU32 i=0;i<list.GetSize();i++)
	{
#ifdef __ANDROID__
		if( (int)list.GetData() < 0xFF )
			return bSet;
#endif
		XU8 s=list[i].nID>>14;
		if(s!=nStatus&&s!=STA_NORMAL) continue;
		int id=list[i].nID&0x3fff;
		switch(id)
		{
		case XEAB::BORDER_TOPWIDTH:
		case XEAB::BORDER_RIGHTWIDTH:
		case XEAB::BORDER_LEFTWIDTH:
		case XEAB::BORDER_BOTTOMWIDTH:
			 data.nBorderWidth[id-XEAB::BORDER_TOPWIDTH]=(XU8)list[i].iData;
			 bSet=XTRUE;
			 break;
		case XEAB::BORDER_TOPSTYLE:
		case XEAB::BORDER_RIGHTSTYLE:
		case XEAB::BORDER_LEFTSTYLE:
		case XEAB::BORDER_BOTTOMSTYLE:
			 data.nBorderStyle[id-XEAB::BORDER_TOPSTYLE]=(XU8)list[i].iData;
			 bSet=XTRUE;
			 break;
		case XEAB::BORDER_TOPCOLOR:
		case XEAB::BORDER_RIGHTCOLOR:
		case XEAB::BORDER_LEFTCOLOR:
		case XEAB::BORDER_BOTTOMCOLOR:
			 data.nColor[id-XEAB::BORDER_TOPCOLOR]=(XU32)list[i].iData;
			 bSet=XTRUE;
			 break;
		}
	}
	return bSet;
}

XU32 XDomNode::HandleChild(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	XU32 s=0;
	for(XU32 i=0;i<m_childs.GetSize();i++)
	{
		//LOGE("fun:%s, line:%d, val:%0x, this:%0x", __FUNCTION__, __LINE__, m_childs[i], this );
#ifdef __ANDROID__
//		if( (int)(m_childs.GetData()) > 0xFF )
//		{
			s|=m_childs[i]->Handle(nOpera,pData1,pData2);
//		}
//		else
//		{
//			LOGWHERE();
//			return s;
//		}
#else
		if( m_childs[i] != NULL )
		{
			s|=m_childs[i]->Handle(nOpera,pData1,pData2);
		}
		else
		{
			LOGWHERE();
		}
#endif
	}
	return s;
}

void XDomLeaf::DoCSS(XCSS*pCss)
{
//	if(FindAttrib(XEAB::CLASS,0)==16392)
//		int a=0;
	if(m_bCssOK) return;
	m_bCssOK=XTRUE;
	XVARLIST list;
	pCss->GetAttribs(this,list);
	for(XU32 i=0;i<list.GetSize();i++)
		AddAttrib(*list[i]);
	DoStyle(pCss);
	Handle(XDO_UPDATE_TABLE,0,0);
}

void XDomLeaf::DoStyle(XCSS *pCss)
{
	if(pCss==XNULL) return;
	XPCTSTR str=FindAttrib(XEAB::STYLE,(XPCTSTR)XNULL);
	if(str==XNULL) return;
	XString8 strValue=str;
	XVarList list1;
	pCss->PhaseCSS(XNULL,list1,strValue);
	for(XU32 i=0;i<list1.GetSize();i++)
		AddAttrib(list1[i]);
}

//DEL void XDomLeaf::StartMargin(DRAWCONTEXT *pDraw, CELLDATA *pData,XU8 bBlock)
//DEL {
//DEL 	//if(pDraw->SETFLOAT&XELT::FLOAT_RIGHT
//DEL 	pData->cur.x+=pDraw->paintData.nMargin[2];
//DEL 	if(bBlock)
//DEL 	{
//DEL 		pData->cur.y+=pDraw->paintData.nMargin[0];
//DEL 		pDraw->win.left+=pDraw->paintData.nMargin[2];
//DEL 		pDraw->win.right-=pDraw->paintData.nMargin[1];
//DEL 	}
//DEL 	if(pDraw->paintData.nWidth>0)
//DEL 	{
//DEL 		pDraw->paintData.nWidth-=
//DEL 			(pDraw->paintData.nMargin[2]+pDraw->paintData.nMargin[1]);
//DEL 	}
//DEL 	if(pDraw->paintData.nHeight>0)
//DEL 	{
//DEL 		pDraw->paintData.nHeight-=
//DEL 			(pDraw->paintData.nMargin[0]+pDraw->paintData.nMargin[3]);
//DEL 	}
//DEL }

void XDomLeaf::InitMargin(DRAWCONTEXT *pDraw, CELLDATA*pData,
						  LAYEROUTDATA *pd,DOMPOS&x,DOMPOS&y,
						  DOMPOS&w,DOMPOS&h,XU8 bTableItem)
{
//   LAYEROUTDATA*pd=&pData->margin;
   //DOMPOS dl[4];//={0,0,0,0};
   //for(int i=0;i<4;i++)
	//   dl[i]=pd->nBorderWidth[i]+pd->nMargin[i]+pd->nPadding[i];
   pd->Total();
   x=pData->cur.x+pd->nMargin[BORDERDATA::LEFT];
   y=pData->cur.y+pd->nMargin[BORDERDATA::TOP];
   pData->cur.x+=pd->nOffset[BORDERDATA::LEFT];
   pDraw->win.left+=pd->nOffset[BORDERDATA::LEFT];
   pDraw->win.right-=pd->nOffset[BORDERDATA::RIGHT];
   pData->cur.y+=pd->nOffset[BORDERDATA::TOP];
   w-=pd->nBorderWidth[BORDERDATA::LEFT]+
	  pd->nPadding[BORDERDATA::LEFT];
   w-=pd->nBorderWidth[BORDERDATA::RIGHT]+
	  pd->nPadding[BORDERDATA::RIGHT];
   h-=pd->nBorderWidth[BORDERDATA::TOP]+
	   pd->nPadding[BORDERDATA::TOP];
   h-=pd->nBorderWidth[BORDERDATA::BOTTOM]+
	   pd->nPadding[BORDERDATA::BOTTOM];
   if(bTableItem)
   {
	 if(pDraw->SETWIDTH>0)
	   pDraw->SETWIDTH-=(pd->nOffset[BORDERDATA::LEFT]+pd->nOffset[BORDERDATA::RIGHT]);
     //if(pDraw->SETHEIGHT>0)
	  // pDraw->SETHEIGHT-=(dl[BORDERDATA::TOP]+dl[BORDERDATA::BOTTOM]);
	}
}

void XDomLeaf::SetMargin(LAYEROUTDATA *pd, int &w, int &h, XSize &sz)
{
   //LAYEROUTDATA*pd=&pData->margin;
//	if(pd->nWidth==60)
//	   int a=0;
   w+=pd->nBorderWidth[BORDERDATA::LEFT]+
	  pd->nPadding[BORDERDATA::LEFT];
   w+=pd->nBorderWidth[BORDERDATA::RIGHT]+
	  pd->nPadding[BORDERDATA::RIGHT];
   h+=pd->nBorderWidth[BORDERDATA::TOP]+
	   pd->nPadding[BORDERDATA::TOP];
   h+=pd->nBorderWidth[BORDERDATA::BOTTOM]+
	   pd->nPadding[BORDERDATA::BOTTOM];
   sz.cx=w+pd->nMargin[BORDERDATA::LEFT]+pd->nMargin[BORDERDATA::RIGHT];
   sz.cy=h+pd->nMargin[BORDERDATA::TOP]+pd->nMargin[BORDERDATA::BOTTOM];
}

XBOOL XDomLeaf::IsVisiable()
{
	//return XTRUE;
	if(FindAttrib(XEAB::HIDDENITEM,0))
		return XFALSE;
	if(FindAttrib(XEAB::DISPLAY_MODE,XELT::DISPLAY_EMPTY)==
		XELT::DISPLAY_NONE)
		return XFALSE;
	return XTRUE;
}

XBOOL XDomLeaf::IsFingerBlock()
{

	XU8 nMode=FindAttrib(XEAB::DISPLAY_MODE,XELT::DISPLAY_EMPTY);
	switch(nMode)
	{
	case XELT::DISPLAY_BLOCK:
	case XELT::DISPLAY_TABLE:
	case XELT::DISPLAY_LIST:
		 return XTRUE;
	}
	return XFALSE;
}

XBOOL XDomLeaf::IsFingerInline()
{
	XU8 nMode=FindAttrib(XEAB::DISPLAY_MODE,XELT::DISPLAY_EMPTY);
	switch(nMode)
	{
	case XELT::DISPLAY_INLINE:
	case XELT::DISPLAY_INTABLE:
		return XTRUE;
	}
	nMode=FindAttrib(XEAB::POSITION_MODE,XELT::POS_STATIC);
	switch(nMode)
	{
	case XELT::POS_RELATIVE:
	case XELT::POS_ABSOLUTE:return XTRUE;
	}
	return XFALSE;
}

XU8 XDomLeaf::IsInline()
{
	for(int i=0;i<(XINT)m_attribs.GetSize();i++)
	{
		switch(m_attribs[i].nID&0x3fff)
		{
		case XEAB::DISPLAY_MODE:
			 switch(m_attribs[i].iData)
			 {case XELT::DISPLAY_BLOCK:
			  case XELT::DISPLAY_LIST:
			  case XELT::DISPLAY_TABLE:return XFALSE;}
			 break;
		case XEAB::WIDTH:
		case XEAB::HEIGHT:return XFALSE;
		case XEAB::FLOAT:
			 if(m_attribs[i].iData!=XELT::FLOAT_NONE) return XFALSE;
			 break;
		case XEAB::BGCOLOR:
			 if(m_attribs[i].iData) return XFALSE;
			 break;
		//case XEAB::BACKIMAGE:
		//	 return XFALSE;
		}
	}
    //if(IsFingerBlock()) return XFALSE;
	//XU8 ft=FindAttrib(XEAB::FLOAT,XELT::FLOAT_NONE);
	//if(ft) return XFALSE;
	//if(FindAttrib(XEAB::WIDTH,0)>0) return XFALSE;
	//if(FindAttrib(XEAB::WIDTH,
	return XTRUE;
}
