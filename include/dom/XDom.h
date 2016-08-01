// XDom.h: interface for the XDom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDOM_H__B800311D_ED43_43B1_9954_76789FE23C3A__INCLUDED_)
#define AFX_XDOM_H__B800311D_ED43_43B1_9954_76789FE23C3A__INCLUDED_

#include "XDomPhase.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"
#include "XDomImg.h"
#include "XURL.h"	// Added by ClassView
#include "XDomBR.h"
#include "XClient.h"
#include "XCSS.h"	// Added by ClassView

class XDomPhase;
class XDomPhaseImage;
class XHomeView;
class XFlashView;

//enum {XD_ANCHOR=1,XD_INPUT,XD_OTHER};

class _XEXT_CLASS XDom
{
	friend class XDomPhase;
	friend class XDomPhaseXML;
	friend class XDomPhaseImage;
	friend class XHomeView;
	friend class XFlashView;
	friend class EditText;
	friend class URLLoaderObject;

public:
	XDomView* GetView()
	{
		if(!m_pDraw) return NULL;
		return m_pDraw->pView;
	}
	XString8Array m_argList;
	
	static void AddArgList(const char*url,XString8Array&list);
	void AddArgList(const char*url)
	{
		AddArgList(url,m_argList);	
	}
	static int IsExist(XString8Array&list,XString8&key)
	{
		int i,nCount=list.GetSize()-1;
		for(i=0;i<nCount;i+=2)
		{
			if(list[i]==key) return i+1;
		}
		return 0;
	}
	static void GetArgList(const char*url,RESPONSEINFO*pInfo,XString8Array&list);
	//XDOMFILTER m_filter;
	XDOMFILTER*m_pFilter;
	XString8 m_strCache;
	int	     m_nMatchID,m_nMatchBegin;

	XBOOL IsFinished(){return m_nLoadStatus==2;}
	XDom* m_pChildDom;
	XDom* CreateChild()
	{
		if(m_pChildDom!=XNULL) return m_pChildDom;
		m_pChildDom=new XDom;
		return m_pChildDom;
		//m_pChildDom->Reset(
	}
	void SetFilter(XDOMFILTER*p)
	{
		m_pFilter=p;m_nMatchBegin=0;m_nMatchID=-1;m_strCache.Empty();
		//if(!m_pFilter)
		//	m_pFilter=&m_filter;
	}
public://Event Function
	void UpdateCSS()
	{
		if(!m_css.IsEmpty())
		{
			GetActiveCard()->SetCSS(&m_css);
			m_css.Clear();
		}
	}
	XU32 OnChar(XWCHAR nChar);
	XU32 OnString(XPCWSTR strBuf);
	XU32 OnKeyDown(XWCHAR nChar,DRAWCONTEXT*pDraw,XU8 bNext=XFALSE);
	XU32 OnClick(XINT x,XINT y);
	XU32 OnDblClick(XINT x,XINT y);
	XU32 OnTimer();
	XU32 OnMove(XINT x,XINT y);
	XU32 OnMouseDown(XINT x,XINT y);
	XU32 OnMouseUp(XINT x,XINT y);
	XU32 OnFocus();
	XU32 OnBlur();

public:
	XSIZE& GetDocSize(){return m_docSize;}
	class XSortDom:public XSort
	{
	public:
		class XSortByClass:public XSort
		{
			struct CLASSMAP
			{	CLASSMAP(XU16 i,XDomItem*p)
				{
					ID=i;
					pItem=p;
				}
				CLASSMAP(){ID=0;pItem=XNULL;}
				XU16 ID;XDomItem*pItem;
			};
		public:

			XU32 GetCount(){return m_list.GetSize();}
			void* GetData(XU32 id)
			{
				return (void*)m_list[id].ID;
			}
			XINT Compare(void*iSrt,void*iDst,XU8 nMode)
			{
				return CompareInt((XINT)iSrt,(XINT)iDst);
			}
			XDomItem*GetItem(XU16 id)
			{
				int im=Index((void*)id);
				if(im<0) return XNULL;
				return m_list[im].pItem;
			}
			XBOOL AddItem(XDomItem*p,XU16 id)
			{
				int mi=0;
				Index((void*)id,mi);
				m_list.InsertAt(mi,CLASSMAP(id,p));
				return XTRUE;
			}
			void GetList(XDomList&list,XU16 id,XDomItem*pParent=XNULL)
			{
				XINT i;
				int im=Index((void*)id),iv=0;
				if(im<0) return;
				for(i=im;i<(XINT)m_list.GetSize();i++)
				{
					if(m_list[i].ID!=id) break;
					if(pParent!=XNULL&&!
					   m_list[i].pItem->IsLeafOf(pParent)) continue;
					//if(pParent!=XNULL&&
					//   m_list[i].pItem->GetParent()!=pParent)
					//   continue;
					list.Add(m_list[i].pItem);
				}
				for(i=im-1;i>=0;i--)
				{
					if(m_list[i].ID!=id) break;
					//if(pParent!=XNULL&&
					//   m_list[i].pItem->GetParent()!=pParent)
					//   continue;
					if(pParent!=XNULL&&!
						m_list[i].pItem->IsLeafOf(pParent)) continue;
					list.Add(m_list[i].pItem);
				}
			}
		public:
			//XSortByID(XU16 id){m_nID=id;}
			virtual ~XSortByClass(){m_list.RemoveAll();}
			XVector<CLASSMAP> m_list;
			//XU16	 m_nID;
	};

		class XSortByID:public XSort
		{
		public:

			XU32 GetCount(){return m_list.GetSize();}
			void* GetData(XU32 id)
			{
				XINT v=0;
				m_list[id]->GetAttrib(XEAB::ID,v);
				return (void*)v;
			}
			XINT Compare(void*iSrt,void*iDst,XU8 nMode)
			{
				return CompareInt((XINT)iSrt,(XINT)iDst);
			}
			XDomItem*GetItem(XU16 id)
			{
				int im=Index((void*)id);
				if(im<0) return XNULL;
				return m_list[im];
			}
			XBOOL AddItem(XDomItem*p,XU16 ID)
			{
				//XINT v=0;
				//if(!p->GetAttrib(XEAB::ID,v)) return XFALSE;
				int mi;
				Index((void*)ID,mi);
				m_list.InsertAt(mi,p);
				return XTRUE;
			}
			void GetList(XDomList&list,XU16 id,XDomItem*pParent=XNULL)
			{
				XINT i;
				int im=Index((void*)id),iv=0;
				if(im<0) return;
				for(i=im;i<(XINT)m_list.GetSize();i++)
				{
					//if(pParent!=XNULL&&
					//   m_list[i]->GetParent()!=pParent)
					//   continue;
					if(!m_list[i]->GetAttrib(XEAB::ID,iv)) break;
					if(pParent!=XNULL&&!m_list[i]->IsLeafOf(pParent)) continue;
					if(iv!=id) break;
					list.Add(m_list[i]);
				}
				for(i=im-1;i>=0;i--)
				{
					if(pParent!=XNULL&&
					   m_list[i]->GetParent()!=pParent)
					   continue;
					if(!m_list[i]->GetAttrib(XEAB::ID,iv)) break;
					if(iv!=id) break;
					list.Add(m_list[i]);
				}
			}
		public:
			//XSortByID(XU16 id){m_nID=id;}
			virtual ~XSortByID(){m_list.RemoveAll();}
			XDomList m_list;
			//XU16	 m_nID;
		};
	public:
		XBOOL Remove(XDomItem*p)
		{
			XINT i;
			int im=Index((void*)p->GetID());
			if(im<0) return XFALSE;
			for(i=im;i<(XINT)m_list.GetSize();i++)
			{
				if(m_list[i]==p)
				{
					m_list.RemoveAt(i);
					return XTRUE;
				}
			}
			for(i=im-1;i>=0;i--)
			{
				if(m_list[i]==p)
				{
					m_list.RemoveAt(i);
					return XTRUE;
				}
			}
			return XTRUE;
		}
		XBOOL AddItem(XDomItem*p)
		{
			if(p==&XDomBR::m_br||
			   p==&XDomSpace::m_space)
			   return XFALSE;
			//m_listByID.AddItem(p);
			//m_listByClass.AddItem(p);
			int mi;
			int id=Index((void*)p->GetID(),mi);
			m_list.InsertAt(mi,p);
			return XTRUE;
		}
		void Release()
		{
			m_listByID.m_list.RemoveAll();
			m_listByClass.m_list.RemoveAll();
			m_list.FreeAll();
		}
		virtual ~XSortDom(){Release();}
		XU32 GetCount(){return m_list.GetSize();}
		void* GetData(XU32 id)
		{
			return (void*)m_list[id]->GetID();
		}
		XINT Compare(void*iSrt,void*iDst,XU8 nMode)
		{
			return CompareInt((XINT)iSrt,(XINT)iDst);
		}
		void GetListByID(XDomList&list,XU16 id,XDomItem*pParent=XNULL)
		{
			switch(id>>13)
			{
			case XCSS::XCSS_ID:
				 m_listByID.GetList(list,id,pParent);
				 break;
			case XCSS::XCSS_CLASS:
				 m_listByClass.GetList(list,id,pParent);
				 break;
			default:
				 GetList(list,id,pParent);
				 break;
			}
		}
		void GetList(XDomList&list,XU16 id,XDomItem*pParent=XNULL)
		{
			XINT i;
			int im=Index((void*)id);
			if(im<0) return;
			for(i=im;i<(XINT)m_list.GetSize();i++)
			{
				//if(pParent!=XNULL&&
				//   m_list[i]->GetParent()!=pParent)
				//	   continue;
				if(m_list[i]->GetID()!=id) break;
				if(pParent!=XNULL&&!m_list[i]->IsLeafOf(pParent)) continue;
				list.Add(m_list[i]);
			}
			for(i=im-1;i>=0;i--)
			{
				if(pParent!=XNULL&&
					   m_list[i]->GetParent()!=pParent)
					   continue;
				if(m_list[i]->GetID()!=id) break;
				list.Add(m_list[i]);
			}

		}
	public:

		XDomList m_list;
		XSortByID m_listByID;
		XSortByClass m_listByClass;
	};
public:
	void FormatURL(XString8&strURL,XDOMFILTER*pFilter);
	DRAWCONTEXT* GetDrawContext(){return m_pDraw;}
	XBOOL IsInitOK(){return m_bInitOK;}
	XBOOL Layerout(DRAWCONTEXT&draw);
	XU32 GetSelectID(XBOOL bSelect=XTRUE)
	{
		XDomItem*p=bSelect?m_pSelect:m_pText;
		if(p==NULL||IsEmpty()) return -1;
		XU32 nIndex=0;
		return GetActiveCard()->FindByItem(p,nIndex);
	}
	//XBOOL JustCard()
	//{
	//	return m_data.bJustCard;
	//}
	XU32 SetCard(CONNECTREQ&req);
	XU32 Final(CONNECTREQ&req,DRAWCONTEXT&draw,XBOOL bForward,XDOMFILTER*pFilter);
//	void SetLayerout(){m_bLayerout=XTRUE;}

	XU32 Paint(DRAWCONTEXT&draw,CONNECTREQ&req,XBOOL bFocus=XTRUE,XBOOL bShow=XTRUE);
	XBOOL PhaseXML(XU8*pData,XU32 nSize,bool bFlash=false,bool bFull=false, bool bCondenseWhite = false );
	XBOOL Reset(XU8 nCharset,XU8 nClass,XU16 nType,DRAWCONTEXT*pDraw);
	XBOOL ResetIfRoot(XU8 nCharset,XU8 nClass,XU16 nType,DRAWCONTEXT*pDraw);
	XBOOL TestContent(XU8 nCharset,XU8 nClass,XU16 nType,DRAWCONTEXT*pDraw);
	XDom();
	virtual ~XDom();
	XU32  GetImageCount(){return m_imgList.GetCount();}
	XIMAGEDATA* GetImageData(XU32 nIndex)
	{
		if(nIndex>=m_imgList.GetCount()) return XFALSE;
		return m_imgList[nIndex];
	}
	void SetReferURL(XString8&strRefer)
	{
		m_referURL.SetURL(strRefer);
	}
	void SetURL(XString8&strURL)
	{
		m_data.SetURL(strURL);
	}
	void SetURL(XURL&url)
	{
		m_data.SetURL(url);
	}
	XURL& GetURL(){return m_data.tempURL;}
	XURL& GetURL0(){return m_data.setURL;}
	XEVENTDATA* GetEventData(){return&m_data;}
	void RemoveCard(XDomItem*p);
	XDomItem*GetActiveCard()
	{ return IsEmpty()?XNULL:m_cardList[m_nActiveCard];}
	XDomItem*GetDoc(){return m_pDoc;}
	void SetLayerout()
	{m_bLayerout=XTRUE;}
	XBOOL IsEmpty()
	{ return m_nActiveCard>=m_cardList.GetSize(); }
protected:
//	XMessageBox* m_pBox;


	XBOOL FinishToken(XBOOL bEnd);
	XPCTSTR IsScriptItem();
	DRAWCONTEXT* m_pDraw;
	XDomItem* HitTest(int x,int y);
	XBOOL AddCardItem(XDomItem*pc);
	XU32 SetSelect(CONNECTREQ&req,XDomItem*p=XNULL);
	void Release();
	XDomPhase* m_pPhase;
	XINT GetCardIndex();
	XU32 SetActive(XDomItem**pActive,XDomItem*pItem);

	XDomItem* Select(XDomItem*pSelect,/*XDomItem**pText,*/DRAWCONTEXT*pDraw,XWCHAR c,XPOINT &pt);
	XDomItem* SelectNext(XDomItem*pSelect,DRAWCONTEXT*pDraw,XWCHAR c,XPOINT&pt);
	XDomItem* SelectDirect(XDomItem*pSelect,DRAWCONTEXT*pDraw,XWCHAR&c,XPOINT&pt);
	XBOOL MoveSelect(XDomItem*pSelect,DRAWCONTEXT*pDraw,XWCHAR c,XPOINT&pt);

	XU8 m_nActiveCard;
	void AddAttrib(XString8&strKey,XString8&strValue);
	void SetData(XString8&strValue);
	XBOOL Parent(XU16 id=-1);//XPCTSTR strKey=XNULL,XINT id);
//#if (__CORE_VERSION__>=0x02073000)
	XBOOL AddText(XString8&strTxt,XBOOL bFlash);
//#else
//	XBOOL AddText(XString8&strTxt);
//#endif
	XBOOL AddText(XPCWSTR strTxt);
	void  AddBR();
	void AddSpace();
	XStack<XU8> m_stack;
public:
	XBOOL AddTag(XDomItem*pItem);  //modify by hgl
public:
	enum {XAP_HEAD,XAP_TAIL,XAP_DIV};
	void AppendDom(XDom*pDom,int s=XAP_DIV);
	void GetText(XString16&strTxt);
	XBOOL CanAdd(XU16 ID);
	void SetAttribs(XVarList&list,XU16Array&ids);
	XPCTSTR GetClass(XDomItem*pItem);
	XPCTSTR GetID(XDomItem*pItem);
	XBOOL IsStartItem();
	XU32 Refresh();
	XBOOL Select(XU32 id);

	void GetList(XDomList&list,XU16 id);
	XEVENTDATA m_data;
	XCSS m_css;
	XU8		m_nRefs,m_bInitOK;
	XU32	    m_nStatus;
	XU8			m_nFormat,m_nLoadStatus,m_bNoTitle,
				m_nCharset,m_nVersion,m_bLayerout;
	//XIMAGELIST  m_imgList;
	XImageList	m_imgList;
	XDomItem   *m_pRoot,*m_pItem,*m_pHeader,
			   *m_pTemplate,*m_pActive,*m_pSelect,*m_pDoc,
			   *m_pText;
	XDomItem   *m_pFrameRoot;
	XDomList    m_cardList;
	XSortDom	m_doms;
	XSIZE		m_docSize;
public:
	XURL& GetDomURL()
	{
		if(m_bFrame) return m_frameURL;
		return GetURL();
	}
	XBOOL m_bFrame;
	XURL  m_frameURL;
	XURL  m_referURL;
	XCSS* m_pCSS;
	int	  m_nDataSize;
	XString8 m_strCard;
};

//#pragma pack()

#endif // !defined(AFX_XDOM_H__B800311D_ED43_43B1_9954_76789FE23C3A__INCLUDED_)
