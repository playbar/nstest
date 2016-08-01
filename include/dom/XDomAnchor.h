// XDomAnchor.h: interface for the XDomAnchor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOMANCHOR_H__C1808C4E_27F4_40A7_AB6F_44B37B43D8AD__INCLUDED_)
#define AFX_XDOMANCHOR_H__C1808C4E_27F4_40A7_AB6F_44B37B43D8AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class _XEXT_CLASS XDomA : public XDomNode
{
 	XDEC_CREATE(XDomA)
public:
	XDomA(XU16 ID=XDOM_A):XDomNode(ID)
	{
		m_bJustImage=XFALSE;
//		m_nWidth=0;
	}
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2)
	{
		switch(nOpera)
		{
		case XDO_SET_FINAL:
			 DoFinal();
			 break;
		case XDO_EVENT_ONMOUSEMOVE:
			 if(!IsVisiable()) return 0;

			 return XRCANCHOR;
		case XDO_EVENT_ONKEYDOWN:
			 if(!IsVisiable()) return 0;
			 if(pData1!=XK_RETURN) return XRNONE;
		case XDO_EVENT_ONCLICK:
			 if(!IsVisiable()) return 0;
			 return XRCONNECT|Connect((XEVENTDATA*)pData2);
		case XDO_GET_TEXT:
			{
				for(XU32 i=0;i<m_childs.GetSize();i++)
					if(m_childs[i]->Handle(XDO_GET_TEXT,pData1,pData2)) return XTRUE;
			}return XFALSE;
		case XDO_CONNECT:
			 if(pData2!=2)
				return Connect((XEVENTDATA*)pData1);
			 else return 0;
		case XDO_SELECTION:
			 if(!IsVisiable()) return 0;
			 return XDomItem::Selection((XSelection*)pData1);
		case XDO_SELECT:
			 return (XU32)XDomItem::SelectItem((SELECTMARK*)pData1);
			 //if(XDomItem::SelectItem((SELECTINFO*)pData1)) return (XU32)this;
			 return 0;
		case XDO_HITTEST:
			 if(!IsVisiable()) return 0;
			 
			 if(XDomNode::Handle(nOpera,pData1,pData2)) return (XU32)this;
			 return XNULL;
		case XDO_LAYEROUT_CELLS:
			 if(!IsVisiable()) return 0;
			 return Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XTRUE);
		case XDO_LAYEROUT_CELL:
			 if(!IsVisiable()) return 0;
			 return Layerout((DRAWCONTEXT*)pData1,(CELLDATA*)pData2,XFALSE);
		//case XDO_
		case XDO_GET_STYLE:
			{
				XU32 s=XDomNode::Handle(nOpera,pData1,pData2)|DOM_ANCHOR;//|DOM_SELECTABLE;;
				if(m_bJustImage) s|=DOM_SELECTABLE;
				else if(m_childs.GetSize()>0)
				{
					s|=DOM_SELECTABLE;
					/*XRect rect;
					GetRoundRect(rect,0);
					if(!rect.IsRectEmpty()) s|=DOM_SELECTABLE;*/
				}
				return s;
			}
			 break;
		case XDO_PAINT:
			{
				DRAWCONTEXT*p=(DRAWCONTEXT*)pData1;
				XU8 nStatus=p->nStatus;
				p->nStatus=GetStatus(p);
				XU32 nd=p->DATA;
				p->DATA=(m_bJustImage&&p->nStatus>1)?p->nStatus:0;
				PrePaint(p);				
				for(XU32 i=0;i<m_childs.GetSize();i++)
					m_childs[i]->Handle(nOpera,pData1,(XU32)this);
				p->DATA=nd;
				p->nStatus=nStatus;
				/*p->Save();
				p->pDraw->SetColor(p->DCLINK);
				XU32 fs=p->pDraw->GetFontStyle();
				XBOOL bJust=XFALSE;
				if(p->pSelect==this)
				{	p->bFillStyle|=DRAWCONTEXT::XDFS_FILLBACK;
					p->pDraw->SetBackColor(p->DCSELECTBACK);
					p->pDraw->SetColor(p->DCSELECTTEXT);
					SetFontStyle(fs,(XU8)((p->nDecLink>>16)&0xff));
					bJust=m_bJustImage==1?1:0;
				}
				else if(p->pActive==this)
				{	//p->bFillStyle|=DRAWCONTEXT::XDFS_FILLBACK;
					p->pDraw->SetColor(p->DCACTIVE);
					//p->pDraw->SetUnderline(XTRUE);
					SetFontStyle(fs,(XU8)((p->nDecLink>>8)&0xff));
					bJust=(m_bJustImage==1?1:0)<<1;
				}  
				else SetFontStyle(fs,(XU8)(p->nDecLink&0xff));
					//p->pDraw->SetUnderline(XTRUE);
				p->pDraw->SetFontStyle(fs);
				
				for(XU32 i=0;i<m_childs.GetSize();i++)
					m_childs[i]->Handle(nOpera,pData1,pData2);*/

				//XDomNode::Handle(nOpera,pData1,bJust);
				//PaintEnd(p,pData);
				//p->Restore();
				//p->bFillStyle&=~DRAWCONTEXT::XDFS_FILLBACK;
				EndPaint(p);
				//if(p->pSelect==this)
				//	p->pDraw->SetBackColor(nBack);
				//p->pDraw->SetColor(nColor);
				//p->pDraw->SetFontStyle(s);
			}
			return XTRUE;
		}
		return XDomNode::Handle(nOpera,pData1,pData2);
	}
protected:
	void DoFinal();
#ifdef _SYMBIAN	  // modify by hgl
#ifdef _DEBUG
	#pragma pack(1)
#endif
	XU8 m_bJustImage;
	//	XU16 m_nWidth;
#ifdef _DEBUG
	#pragma pack()
#endif
#else
	#pragma pack(1)
	XU8 m_bJustImage;
	//	XU16 m_nWidth;
	#pragma pack()
#endif

	XBOOL Layerout(DRAWCONTEXT*pDraw,CELLDATA*pData,XBOOL bCells);
	XU32 Connect(XEVENTDATA*pData);
};
class _XEXT_CLASS XDomAnchor : public XDomA 
{
	XDEC_CREATE(XDomAnchor)
public:
	XDomAnchor(XU16 ID=XDOM_ANCHOR):XDomA(ID)
	{

	}
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
protected:
//	XString16 m_strTitle;
};


#endif // !defined(AFX_XDOMANCHOR_H__C1808C4E_27F4_40A7_AB6F_44B37B43D8AD__INCLUDED_)
