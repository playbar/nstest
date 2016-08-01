// XCSS.h: interface for the XCSS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCSS_H__FEF2DF02_0B01_4142_B49F_B037603D8C7A__INCLUDED_)
#define AFX_XCSS_H__FEF2DF02_0B01_4142_B49F_B037603D8C7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XVar.h"
#include "XURL.h"

/*class XCSSSelector//:public XSortObj
{
public:
	~XCSSSelector()
	{
		m_attribs.RemoveAll();
	}
public:
	XU32 GetCount() {return m_attribs.GetSize();}
	void AddVars(XVarList&list,XPCTSTR strClass)
	{
		for(XU32 i=0;i<list.GetSize();i++)
			AddVar(list[i],strClass);
	}
	//XU32 m_nSelector;
	XU16Array m_selectors;
	XVarList  m_attribs;
protected:
	void TransID(XU16&id,XPCTSTR strClass);
};
typedef XArray<XCSSSelector*>CSSLIST;*/


class XDom;
class XDomItem;

class XCSS//:public XSortObj  
{
public:
	bool m_bClass;
	const char* AttribName(XVar&var);
	//friend class StyleSheetObject;
	class XSelector;
	typedef XVector<XSelector*> SELECTORS;
	class XSelector:public XSort
	{
	public:
		XU32 GetCount(){return m_entries.GetSize();}
		void* GetData(XU32 nID){return (void*)m_entries[nID]->m_nToken;}
		XINT Compare(void*iSrt,void*iDst,XU8 nMode)
		{
			return CompareInt((XINT)iSrt,(XINT)iDst);
		}
		void AddAttribs(XU16Array&entries,int nOffset,XVarList&vars,XU8 nStatus)
		{
			if(nOffset<0)//>=(XINT)entries.GetSize())
			{
				for(XU32 i=0;i<vars.GetSize();i++)
					AddAttrib(vars[i],nStatus);
			}
			else
			{
				int mi;
				int id=Index((void*)entries[nOffset],mi);
				if(id>=0) m_entries[id]->AddAttribs(entries,nOffset-1,vars,nStatus);
				else
				{
				   XSelector*p=new XSelector;
				   m_entries.InsertAt(mi,p);
				   p->m_nToken=entries[nOffset];
				   p->AddAttribs(entries,nOffset-1,vars,nStatus);
				}
			}
		}
		void AddAttrib(XVar&var,XU8 nStatus);
		virtual ~XSelector()
		{
			Release();	
		}
		void Release()
		{
			m_entries.FreeAll();
			m_attribs.FreeAll();
		}
	public:
		
		void GetAttribs(XVARLIST&vars,XU16Array&entries,int nOffset);//XSortU16&entries);
		XU16 m_nToken;
		XVARLIST  m_attribs;
		SELECTORS m_entries;
	};
	/*struct XSelector
	{
		~XSelector()
		{
			m_selectors.FreeAll();
		}
		XVector<XU16Array*> m_selectors;
		XVarList m_list;
	};*/
public:
	enum {XCSS_TOKEN=0,XCSS_ID=0x1,XCSS_CLASS=2};
	struct CSSID
	{
		XString8 strKey;
		XU16	 nID;
	};
	class XSortID:public XSort
	{
	public:
		XU32 GetCount()
		{
			return m_list.GetSize();
		}
		void* GetData(XU32 id){return m_list[id]->strKey;}
		XINT Compare(void*iSrt,void*iDst,XU8 nMode)
		{
			return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
		}
		XU16 CreateID(XPCTSTR strKey)
		{
			int mi=0;
			int id=Index((void*)strKey,mi,XSS_NORMAL);
			if(id>=0) return m_list[id]->nID;
			CSSID*p=new CSSID;
			p->strKey=strKey;
			p->nID=(XU16)m_list.GetSize();
			m_list.InsertAt(mi,p);
			return p->nID;
		}
	public:
		virtual ~XSortID(){m_list.FreeAll();}
		XVector<CSSID*> m_list;
	};
public:
	enum {CST_INT,CST_PECENT,
		  CST_COLOR,CST_STRING,
		  CST_URL,CST_IMAGE,CST_ALIGN,CST_POSITION,
		  CST_ATTACH,CST_BACKGROUND,CST_FLOAT,
		  CST_TTRANS,CST_DEC,CST_FONTSIZE,CST_FONTSTYLE,
		  CST_FONTWEIGHT,CST_FONTALL,
		  CST_BORDERWIDTH,
		  CST_BORDERSTYLE,
		  CST_BORDERALL,
		  CST_BORDERONE,
		  CST_BORDERCOLORS,
		  CST_BORDERSTYLES,
		  CST_BORDERWIDTHS,
		  CST_MARGINALL,
		  CST_PADDINGALL,
		  CST_LISTTYPE,
		  CST_LISTALL,CST_DISPLAY,CST_VISIABLE,
		  CST_POSITIONMODE,CST_OVERFLOW,
		  CST_COLLAPSE
	};
	//enum {COLOR};

public:
	const char* GetIDName(int id)
	{
		id&=0x1fff;
		int nc=m_ids.m_list.GetSize();
		if(id<nc) return m_ids.m_list[id]->strKey;
		//retur
		/*for(i=0;i<nc;i++)
		{
			if((m_ids.m_list[i]->nID&0x1fff)==id) 
				return m_ids.m_list[i]->strKey;
		}*/
		return NULL;
	}
	void GetAttribs(XDomItem*pItem,XVARLIST&var);
	int AttribID(XPCTSTR strName,int&type);
	XU8 IsEmpty(){return m_css.m_entries.GetSize()<=0;}
	void Clear(){m_css.Release();}
	void DoCSS(XDom*pDom,XU8 bClear);
	static void RemoveEcho(XString8&strCss);
	XU8 PhaseSelector(XU16Array&list,XString8&strNames);
	XDom* m_pDom;
	XINT IDOF(XPCTSTR strID,XU8 nClass)
	{
		int id=m_ids.Index((void*)strID);
		if(id>=0)
			id=(m_ids.m_list[id]->nID&0x1fff)|(nClass<<13);
		return id;
	}
	XINT CreateID(XPCTSTR strID,XU8 nClass);
	XINT CreateID(XString8&strKey,XU16&nSubKey);
//	XU32 GetCount(){return m_css.GetSize();}
//	XINT GetIData(XU32 id){return m_css[id]->m_nSelector;}
	void PhaseCSS(XURL*pUrl,XVarList&list,XString8&strAttribs);
	void AddCSS(XURL*url,XString8&strCss);
	void Reset();
	XCSS();
	virtual ~XCSS();
	XPCTSTR GetName(XU16 id)
	{
		//id=id&0x1fff;
		//if(id>=m_ids.GetCount()) return XNULL;
		//return m_ids[id];
		for(XU32 i=0;i<m_ids.m_list.GetSize();i++)
			if(m_ids.m_list[i]->nID==id)
				return m_ids.m_list[i]->strKey;
		return XNULL;
	}
protected:
	void MakeInt(XVar&var,XPCTSTR strCnt);
	XBOOL GetSettings(XU16 ID,XVarList&list,XString8Array&args,XPCTSTR strCnt);
	void MakePosition(XVarList&vars,XString8Array&list);
	int MakeURL(XPCTSTR str,XString8&strURL);
	
	
	void AddCSS(XURL*url,XString8&strNames,XString8&strAttribs);
	//XString8Array m_strNames;
public:
	void MakeVar(XURL*pURL,XVarList&list,XVar&var,XU8 nType,XPCTSTR strCnt);
	//void MakeStringVar(XXVar&var,int nType,XPCTSTR strCnt);
	XSortID			  m_ids;
	XSelector		  m_css;
//	XVector<XSelector*> m_css;

};

#endif // !defined(AFX_XCSS_H__FEF2DF02_0B01_4142_B49F_B037603D8C7A__INCLUDED_)
