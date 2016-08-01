// XDom.cpp: implementation of the XDom class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDom.h"
#include "XEnumData.h"
#include "XContentType.h"
#include "XDomText.h"
#include "XDomPhaseImage.h"
#include "XDomView.h"
#include "XSelection.h"
#include "XClientApply.h"
#include "XDomCard.h"
#include "XHTMLScript.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//static int nCount=0;
XDom::XDom()
{
	//m_bSetCard=XFALSE;
	//nCount++;
	m_bFrame=XFALSE;
	m_pFilter=XNULL;
	m_pPhase=XNULL;
	m_data.pView=XNULL;
	m_data.pDom=this;
	m_pChildDom=XNULL;
	m_nDataSize=0;
	Release();
	m_pDraw=XNULL;
	m_nCharset=XCHARSET_GB2312;
	m_css.m_pDom=this;
	m_nRefs=1;
	m_nStatus=XHTTP_STATUS_OK;
	m_bNoTitle=XFALSE;
	m_bInitOK=XFALSE;
	m_pCSS=NULL;
	m_pFrameRoot=NULL;

}

XDom::~XDom()
{
	//nCount--;
	Release();
	if(m_pPhase!=XNULL) delete m_pPhase;
	m_pPhase=XNULL;
}

void XDom::AddArgList(const char*url,XString8Array&list)
{
	const char*pArgs=strchr(url,'?');
	if(pArgs)
	{
		pArgs++;
		while(pArgs&&*pArgs)
		{
			const char*pAnd=strchr(pArgs,'&');
			if(pAnd==pArgs) {pArgs++;continue;}
			const char*pEQ=strchr(pArgs,'=');
			if(pEQ)
			{
				XString8 strKey,strValue;
				if(pEQ) strKey.SetString(pArgs,pEQ-pArgs);
				else	strKey.SetString(pArgs,0);
				pEQ++;
				if(pEQ)
				{
					if(pAnd==pEQ)
						strValue.Empty();
					else if(pAnd) strValue.SetString(pEQ,pAnd-pEQ);
					else	 strValue.SetString(pEQ,0);
				}
				if(!strKey.IsEmpty())
				{
					XURL::Decode(strValue);
					int ie=IsExist(list,strKey);
					if(!ie)
					{
						list.Add(strKey);
						list.Add(strValue);
					}
					else
						list[ie]=strValue;
				}
			}
			pArgs=pAnd;
			if(pArgs) pArgs++;
		}
	}
}

void XDom::GetArgList(const char*url,RESPONSEINFO*pInfo,XString8Array&list)
{
	if(pInfo)
	{
		list.Add("contextType");//XEH()[XEH::CONTENT_TYPE]);
		list.Add(pInfo->type.m_strType);
		int i,nSize=pInfo->strResHeaders.GetSize();
		XEnumHeader hd;
		for(i=0;i<nSize;i++)
		{
			XString8&s=pInfo->strResHeaders[i];
			int id=s.Find(':');
			if(id<=0) continue;
			XString8 strKey,strCnt;
			s.Left(strKey,id);
			if(strKey.IsEmpty()) continue;
			if(strKey[1]=='-'&&
			   (strKey[0]=='x'||strKey[0]=='X'))
				continue;
			int idd=hd.Index(strKey,XSS_NOCASE);
			if(idd>=0) continue;
			s.Mid(strCnt,id+1);
			strKey.TrimRight();
			strCnt.TrimRight();
			XURL::Encode(strCnt);
			int ie=IsExist(list,strKey);
			if(!ie)
			{
				list.Add(strKey);
				list.Add(strCnt);
			}
			else
				list[ie]=strCnt;
			//list.Append(pInfo->strResHeaders);
		}
	}
	AddArgList(url,list);
	/*const char*pArgs=strchr(url,'?');
	if(pArgs)
	{
		pArgs++;
		while(pArgs&&*pArgs)
		{
			const char*pAnd=strchr(pArgs,'&');
			if(pAnd==pArgs) {pArgs++;continue;}
			const char*pEQ=strchr(pArgs,'=');
			if(pEQ)
			{
				XString8 strKey,strValue;
				if(pEQ) strKey.SetString(pArgs,pEQ-pArgs);
				else	strKey.SetString(pArgs,0);
				pEQ++;
				if(pEQ)
				{
					if(pAnd==pEQ)
						strValue.Empty();
					else if(pAnd) strValue.SetString(pEQ,pAnd-pEQ);
					else	 strValue.SetString(pEQ,0);
				}
				if(!strKey.IsEmpty())
				{
					int ie=IsExist(list,strKey);
					if(!ie)
					{
						list.Add(strKey);
						list.Add(strValue);
					}
					else
						list[ie]=strValue;
				}
			}
			pArgs=pAnd;
			if(pArgs) pArgs++;
		}
	}*/
}

XBOOL XDom::TestContent(XU8 nCharset,XU8 nClass,XU16 nType,DRAWCONTEXT*pDraw)
{
	switch(nClass)
	{
	case XCNT::CAPPLICATION:
	case XCNT::CTEXT:
		 switch(nType)
		 {
		 case XCNT::TVND_WAP_XHTML_XML:
		 case XCNT::TXHTML_XML:
		 case XCNT::THTML:
		 case XCNT::TVND_WAP_WML:
		 case XCNT::TXML:
		 case XCNT::TSWF:
		 case XCNT::TX_SHOCK_WAVE_FLASH:
		 case XCNT::TX_JAVASCRIPT:
		 case XCNT::TJAVASCRIPT:
			  return XTRUE;
		 }
		 break;
	case XCNT::CIMAGE:
		 return XTRUE;
	}
	return XFALSE;
}

XBOOL XDom::ResetIfRoot(XU8 nCharset,XU8 nClass,XU16 nType,DRAWCONTEXT*pDraw)
{
	if(m_pFrameRoot==XNULL)
		return Reset(nCharset,nClass,nType,pDraw);
	else
	{
		m_pRoot=m_pFrameRoot;
	}
	m_bInitOK=XFALSE;
	m_strCache.Empty();
	m_nStatus=XHTTP_STATUS_OK;
	//if(!m_pPhase) return XFALSE;
	//if(m_pPhase==XNULL) return XFALSE;
	m_bLayerout=XFALSE;
	m_nCharset=nCharset;
	m_data.nData=0;
	m_data.strTitle=XNULL;
	m_data.bAuto=XFALSE;
	return XTRUE;
}

XBOOL XDom::Reset(XU8 nCharset,XU8 nClass,XU16 nType,DRAWCONTEXT*pDraw)
{
	//m_pFilter=XNULL;
	//m_bSetCard=XFALSE;
	m_strCard.Empty();
	m_bInitOK=XFALSE;
	m_strCache.Empty();
	if(pDraw!=XNULL)
		pDraw->pCss=&m_css;
	m_pDraw=pDraw;
	m_pSelect=XNULL;
	m_nStatus=XHTTP_STATUS_OK;
	if(m_pPhase!=XNULL) delete m_pPhase;
	m_pPhase=XNULL;
	switch(nClass)
	{
	case XCNT::CAPPLICATION:
	case XCNT::CTEXT:
		 switch(nType)
		 {
		 case XCNT::TVND_WAP_XHTML_XML:
		 case XCNT::TXHTML_XML:
		 case XCNT::THTML:
		 case XCNT::TVND_WAP_WML:
			  m_pPhase=new XDomPhaseXML(this);
			  break;
		 }
		 break;
	case XCNT::CIMAGE:
		 m_pPhase=new XDomPhaseImage(this);
		 break;
	}
	//if(!m_pPhase) return XFALSE;
	//if(m_pPhase==XNULL) return XFALSE;
	m_bLayerout=XFALSE;
	Release();
	m_nCharset=nCharset;
//	m_data.bJustCard=XFALSE;
	m_data.nData=0;
	m_data.strTitle=XNULL;
	m_data.bAuto=XFALSE;
	m_data.setURL=m_data.tempURL;
	//m_bFrame=XFALSE;
	return m_pPhase==XNULL;//XTRUE;
}

XBOOL XDom::PhaseXML(XU8 *pData, XU32 nSize,bool bFlash,bool bEnd, bool bCondenseWhite)
{
	m_bInitOK=XTRUE;
	if(m_pPhase!=XNULL)
	{
		/*if(m_pFilter&&m_pFilter->strBegins.GetSize())
		{
			XBOOL bOK=XTRUE;
			pData[nSize]=0;
			int iOld=m_strCache.GetLength();
			m_strCache+=(XPCTSTR)pData;
			if(m_nMatchID<0)
			{
				int i,nCount=m_pFilter->strBegins.GetSize();
FilterAgain:
				for(i=0;i<nCount;i++)
				{
					int iBegin=m_strCache.Find(m_pFilter->strBegins[i],m_nMatchBegin);
					if(iBegin>=0)
					{
						int iLenB=m_pFilter->strBegins[i].GetLength();
						int iLenE=m_pFilter->strEnds[i].GetLength();
						if(iLenE==0)
						{
							bOK=m_pPhase->PhaseData(m_strCache.GetData()+iBegin,iLenB,true);
							m_nMatchBegin=iBegin+iLenB;
							goto FilterAgain;
						}
						else
						{
							int iEnd=m_strCache.Find(m_pFilter->strEnds[i],iBegin+iLenB);
							if(iEnd>iBegin)
							{
								bOK=m_pPhase->PhaseData(m_strCache.GetData()+iBegin,iEnd-iBegin+iLenE,true);
								m_nMatchBegin=iEnd+iLenE;
								goto FilterAgain;
							}
							else
							{
								bOK=m_pPhase->PhaseData(m_strCache.GetData()+iBegin,m_strCache.GetLength()-iBegin,true);
								m_nMatchBegin=iBegin+iLenB;
								m_nMatchID=i;
							}
						}
						//m_strCache.Empty();
					}
				}
			}
			else
			{
				int iEnd=m_strCache.Find(m_pFilter->strEnds[m_nMatchID],m_nMatchBegin);
				if(iEnd>=0)
				{
					int iLenE=m_pFilter->strEnds[m_nMatchID].GetLength();
					bOK=m_pPhase->PhaseData((XTCHAR*)pData,iEnd-iOld+iLenE,true);
					m_nMatchBegin=iEnd+iLenE;
					m_nMatchID=-1;
				}
				else
					bOK=m_pPhase->PhaseData((XTCHAR*)pData,nSize,true);
			}
			if(!bOK)
				return XFALSE;
		}
		else*/
		{
			XBOOL bOK=m_pPhase->PhaseData((XTCHAR*)pData,nSize,bFlash, bCondenseWhite);
			if(bEnd)
//#if (__CORE_VERSION__>=0x02073000)
				m_pPhase->EndParse(bFlash);
//#else
//				m_pPhase->EndParse();
//#endif
			return bOK;
			//return XFALSE;
		}
	}

    if(!IsEmpty())
    {
	   m_pText=XNULL;
	   XDomItem*pSelect=GetActiveCard()->Select(XNULL);//,&m_pText,1,SELECTINFO::XDOWN);
	   if(m_pSelect==XNULL)
		   SetActive(&m_pSelect,pSelect);
   }
//	SetTitle();
//	m_bLayerout=XTRUE;
//	ProcImages();
	return m_nLoadStatus!=1;
}

/////////////////////////////////////////////////////////
// ��'<' ��ʼ����һ����ǩ
// ���� C��ʾһ�������ַ�


#if (__CORE_VERSION__>=0x02073000)
XBOOL XDom::AddText(XString8&strValue,XBOOL bFlash)
#else
XBOOL XDom::AddText(XString8&strValue)
#endif
{
	if(m_pItem==NULL)
		return XFALSE;

	if(strValue.IsEmpty())
		return XFALSE;
//#if (__CORE_VERSION__>=0x02073000)
	if(strValue.GetLength()>1&&!bFlash)
//#else
//	if(strValue.GetLength()>1)
//#endif
	{
		strValue.TrimLeft(' ');
		strValue.TrimRight(' ');
	}
	XString16 str;
	str=strValue;
	if(str.GetLength()<=0)
		return XFALSE;
	else if(str.GetLength()==1)
	{
	   XWCHAR c=str.GetData()[0];
		if(c==' '||c==160)
		{
			AddSpace();
			return XTRUE;
		}
	}
	XDomText* p=new XDomText(XDOM_TEXT);
	p->m_strTxt=str;
	//LOGE("%s, %d, data:%s", __FUNCTION__, __LINE__, strValue.GetData() );
	//p->Handle(XDO_ADD_ATTR,(XU32)&var,0);
	if(!m_pItem->AddChild(p))
		delete p;
	else
	{
		p->Final(m_pDraw,&m_data);
		m_doms.AddItem(p);
		//m_domList.Add(p);
	}
	SetLayerout();
	return XTRUE;
}

XBOOL XDom::AddTag(XDomItem*p)
{
	if(p==XNULL) return XFALSE;
	if(m_pItem!=XNULL)
	{
		while(!CanAdd(p->GetID()))
			Parent();
		if(!m_pItem->AddChild(p))
			return XFALSE;
		/*{
			Parent();
			if(m_pItem!=XNULL)
				m_pItem->AddChild(p);

		} */
	}
	else if(m_pDoc)
	{
		m_pDoc->AddChild(p);
	}
	if(m_pDoc==XNULL&&(p->GetStyle()&XDomItem::DOM_NODE))
	{
		/*switch(p->GetID())
		{
		case XDOM_WML:
		//case XDOM_XML:
		case XDOM_HTML:
			 break;
		default:
			 AddTag(new XDomNode(XDOM_WML));
			 AddTag(new XDomCard());
			 break;
		}*/
		m_pDoc=p;
	}
	if(p!=&XDomBR::m_br&&
	   p!=&XDomSpace::m_space)
	   m_doms.AddItem(p);
		//m_domList.Add(p);
	XU32 nType=p->GetStyle();
	m_pRoot=m_pItem;
	m_pItem=p;


	if(nType&XDomItem::DOM_TEMPLATE)
		m_pTemplate=m_pItem;
	if(nType&XDomItem::DOM_CARD)
		m_cardList.Add(m_pItem);
	if(m_pItem->GetID()==XDOM_HEAD)
	{
		m_pHeader=m_pItem;
		/*if(m_pFilter)
		{
			int i,iCount=m_pFilter->strLoaders.GetSize();
			for(i=0;i<iCount;i++)
			{
				XString8 strValue=m_pFilter->strLoaders[i];
				this->m_pDraw->pView->SetValueOf(strValue);
				XHTMLScript*pItem=new XHTMLScript(XDOM_SCRIPT);
				m_doms.AddItem(pItem);
				XVar atr(XEAB::SRC,strValue);
				pItem->AddAttrib(atr);
				m_pHeader->AddChild(pItem);
				pItem->Final(this->m_pDraw,&this->m_data);
			}
		}*/
	}

	/*if(p->GetID()==XDOM_HTML)
	{
		XDomItem*p=new XDomCard();
		AddTag(XDomItem*p);
	}*/
	SetLayerout();
	return XTRUE;
}


XBOOL XDom::Parent(XU16 id)
{
	if(m_pItem==XNULL) return XFALSE;
//Again:
	m_pItem->Final(m_pDraw,&m_data);
	XU16 nID=m_pItem->GetID();
	if(m_nFormat>0)
		m_nFormat-=m_pItem->IsFormat();
	XBOOL bOK=XTRUE;
//	if(id==XDOM_SELECT)
//		int a=0;
	if(id!=(XU16)-1&&id!=nID)//pKey!=XNULL)//&&nID!=(XU16)-1)
	{
		/*XDomItem*p=m_pItem->GetParent();
		if(p!=XNULL&&p->GetID()==id)
		{
			p->Final(m_pDraw,&m_data);
			m_pItem=p->GetParent();
		} */
		/*while(XDomPhase)
		{

		}*/
		if(XDomPhaseXML::MustEnd(id))
		{
			XDomItem*p=m_pItem->GetParent();
			while(p!=XNULL&&p->GetID()!=id)
			  p=p->GetParent();
			if(p!=XNULL)
			{
				p->Final(m_pDraw,&m_data);
				m_pItem=p->GetParent();
			}
		}
		//XU16 nPID=XDomPhaseXML
		//XString8 str(pKey,STRING_ATTACH);
		//XPCTSTR pStr=XDomPhaseXML::GetToken(nID);
		//if(str.Compare(pStr,XTRUE)!=0)
		// 	bOK=XFALSE;
		//str.EmptyNoFree();
	}
	else m_pItem=m_pRoot;
	if(m_pItem==XNULL||nID==XDOM_WML||nID==XDOM_HTML)
	{
		if(nID!=XDOM_QUESTION&&nID!=XDOM_ECHO)
		{
			m_nLoadStatus=1;
			if(m_pFilter&&m_pHeader)
			{
				int i,iCount=m_pFilter->strLoaders.GetSize();
				for(i=0;i<iCount;i++)
				{
					XString8 strValue=m_pFilter->strLoaders[i];
					this->m_pDraw->pView->SetValueOf(strValue);
					XHTMLScript*pItem=new XHTMLScript(XDOM_SCRIPT);
					m_doms.AddItem(pItem);
					XVar atr(XEAB::SRC,strValue);
					pItem->AddAttrib(atr);
					m_pHeader->AddChild(pItem);
					pItem->Final(this->m_pDraw,&this->m_data);
				}
			}
			return XFALSE;
		}
		else return XTRUE;
	}
	m_pRoot=m_pItem->GetParent();
	//if(!bOK&&m_pItem!=XNULL)
	//	goto Again;

	return XTRUE;
}





void XDom::AddAttrib(XString8&strKey,XString8&strValue)
{
	if(strKey.IsEmpty()) return;

//	if(strKey=="button_hor_pos")
//		int v=0;
	//m_strKey.MakeLower();
	XU16 id=XEnumAttribs().Index(strKey,XSS_NOCASE);

//	if(id<0) return;
//	if(id==XEAB::TEXT)
//		int a=0;
	if(m_pItem==NULL||m_pItem->GetID()==XDOM_META||m_pItem->GetID()==XDOM_QUESTION)
	{
		bool bRet=false;
		switch(id)
		{
		case XEnumAttribs::CONTENT:
			{
				XString8Array list;
				list.SplitString(strValue,';');
				int i,nCount=list.GetSize();
				XEnumAttribs attr;
				for(i=0;i<nCount;i++)
				{
					int id=list[i].Find('=');
					if(id<=0) continue;
					XString8 strKey,strValue;
					list[i].Left(strKey,id);
					list[i].Mid(strValue,id+1);
					//XString8Array sp;
					//sp.SplitString(list[i],'=');
					//if(sp.GetSize()<2) continue;
					if(strKey.Compare("url",XTRUE)==0)
					{
						XVar var(XEAB::SRC,strValue);
						m_pItem->AddAttrib(var);
						continue;
					}
					XU16 iss=attr.Index(strKey,XSS_NOCASE);
					if(iss==XEAB::CHARSET)
					{
						m_nCharset=XEnumCharset().Index(strValue);
					}
				}
			}
			bRet=true;
			 break;
		case XEnumAttribs::VERSION:
			 m_nVersion=(XU8)(strValue.ToFloat()*10);
			 bRet=true;
			 break;
		case XEnumAttribs::CHARSET:
		case XEnumAttribs::ENCODING:
			 //m_strValue.MakeLower();
			 m_nCharset=XEnumCharset().Index(strValue);
			 bRet=true;
			 break;
		}
		if(bRet) return;
	}

	XVar var(id,strValue);
	if(id==(XU16)-1)
	{
		XString8 ss(strKey);
		ss+="=";
		ss+=strValue;
		var=ss;
	}
	XBOOL bStringAttrib=XFALSE;
	if(m_pItem)
	{
		int idd=m_pItem->GetID();
		if(XDomPhaseXML::StringParam(idd))
			bStringAttrib=XTRUE;
	}
	if(!bStringAttrib)
		XEnumAttribs::FormatVar(var,m_pCSS?m_pCSS:&m_css);

	//var.From(strValue,m_nCharset);
	//Handle(XDO_ADD_ATTR,(XU32)&var,0);
	switch(id)
	{
	case XEAB::STYLE:
		{
			//var=strValue;
			XVarList list;
			if(m_pCSS)
				m_pCSS->PhaseCSS(XNULL,list,strValue);
			else
				m_css.PhaseCSS(XNULL,list,strValue);
			for(XU32 i=0;i<list.GetSize();i++)
				m_pItem->AddAttrib(list[i]);
		}
		 break;
	case XEAB::USEMAP:
		{
			//var.nID=XEAB::ID;
			XVar var1(XEAB::ID,(XINT)var.iData);
			m_pItem->AddAttrib(var1);
		 //int a=0;
		}
	case XEAB::ID:
		 m_doms.m_listByID.AddItem(m_pItem,(XU16)var.iData);
		 break;
	case XEAB::CLASS:
		{
			var.ToInt();
			var.iData=0;
			XString8Array list;
			list.SplitString(strValue);
			for(XU32 i=0;i<list.GetSize();i++)
			{
				list[i].TrimRight();
				XString8 str(".");
				str+=list[i];
				XU16 id=m_pCSS?m_pCSS->CreateID(str,XCSS::XCSS_CLASS):m_css.CreateID(str,XCSS::XCSS_CLASS);
				var.iData=(var.iData<<16)|id;
				m_doms.m_listByClass.AddItem(m_pItem,id);
			}
		}
		break;
	}
	m_pItem->AddAttrib(var);
	SetLayerout();
}



XU32 XDom::Paint(DRAWCONTEXT &draw,CONNECTREQ&req,XBOOL bFocus,XBOOL bShow)
{
	if(this->IsEmpty())
		return 0;
	draw.pDom=this;
	draw.Init();
	draw.pCss=m_pCSS?m_pCSS:&m_css;
	if(m_bLayerout)
	{
		//if(!m_pFilter)
			Layerout(draw);
		//else
		//	m_bLayerout=XFALSE;
	}
	if(!bFocus) return 0;
	draw.pActive=m_pActive;
	draw.pSelect=bFocus?m_pSelect:XNULL;
//	draw.bFillStyle=0;
//	draw.pText=m_pText;

	if(bShow||!IsEmpty())
	{
		draw.pDraw->SetBackColor(draw.DCBACK);
		draw.pDraw->FillRectangle(draw.paint);
	}
	if(!IsEmpty())
	{
		draw.pDraw->SetBackColor(0);
		draw.pDraw->SetColor(draw.DCTEXT);
		GetActiveCard()->Paint(&draw);
		m_imgList.ClearModify();
	}
	if(!m_strCard.IsEmpty())
	{
		XU32 s=SetCard(req);
		m_strCard.Empty();
		return s;
	}
	return 0;
}

XBOOL XDom::Layerout(DRAWCONTEXT &draw)
{
	//if(!m_bLayerout) return XTRUE;
	//return;
	if(!m_imgList.CanLayerout()||
		!m_nLoadStatus) return XFALSE;
	draw.pDom=this;
	draw.Init();
	draw.pCss=m_pCSS?m_pCSS:&m_css;
	if(!m_css.IsEmpty())
	{
//	    XU32 t1=GetTickCount();
		if(GetActiveCard())
			GetActiveCard()->SetCSS(&m_css);
//		XU32 t2=GetTickCount()-t1;
		m_css.Clear();

	}
	//if(m_nLoadStatus==2)
	//  m_css.DoCSS(this,m_nLoadStatus==2);
	CELLDATA data;
	m_bLayerout=XFALSE;
	data.Reset(0,0,XTRUE);//,XEA::LEFT,XEA::BOTTOM,XEA::WRAP);
	if(!IsEmpty())
	{
		XRect rt=draw.win;
		GetActiveCard()->LayeroutItem(&draw,&data,XFALSE);
		m_docSize.cx=XMAX(data.max.cx,draw.view.Width());//rect.right;//data.max.cx;
		m_docSize.cy=XMAX(data.max.cy,draw.view.Height());//rect.bottom;//data.max.cy;
		draw.pView->SetDocSize(m_docSize.cx,m_docSize.cy);
		draw.win=rt;
		if((m_docSize.cx>>1)>rt.Width())
		   draw.win=XRect(0,0,m_docSize.cx,m_docSize.cy);
	}
	return XTRUE;
	//return (data.max.cx<<16)|data.max.cy;
}

/*XBOOL XDom::ProcImages()
{
//	return XTRUE;
	XBOOL bUpdate=XFALSE;
	//return bUpdate;
	for(XU32 i=0;i<m_domList.GetSize();i++)
	{
		if(m_domList[i]->GetStyle()&XDomItem::DOM_IMAGE)
		{
			XDomItem*pItem=m_domList[i];
			XVar var;
			if(!pItem->GetImage(var)||
				var.strData==XNULL) continue;

			XBOOL bLoad=XTRUE;
			for(XU32 j=0;j<m_imgList.GetSize();j++)
			{
				if(m_imgList[j]->strURL==var.strData)
				{
					bLoad=XFALSE;
					m_imgList[j]->list.EnumAdd(m_domList[i]);
					//AddToList(m_imgList[j]->list,m_domList[i]);
					m_domList[i]->SetImage(m_imgList[j]);
					break;
				}
			}
			if(bLoad)
			{
				m_imgList.Add(new XIMAGEDATA());
				XIMAGEDATA*p=m_imgList[m_imgList.GetSize()-1];
				p->list.EnumAdd(m_domList[i]);
				p->strURL=var.strData;
				p->nSize=0;
				p->nStatus=XL_WAIT;
				p->nMethod=XEM::GET;
				{
					int w=100,h=100;
					XVar var1(XEAB::WIDTH,0);
					if(pItem->GetAttrib(var1))
						w=(int)var1.ToInt();
					var1.nID=XEAB::HEIGHT;
					if(pItem->GetAttrib(var1))
						h=(int)var1.ToInt();
					if(w*h<8)
						p->nStatus=XL_OK;
				}
				m_domList[i]->SetImage(p);//Handle(XDO_SET_IMAGE,(XU32)&p,0);
			}
		}
	}
	return bUpdate;
}*/


XU32 XDom::OnMove(XINT x,XINT y)
{
	XU32 s=0;
	if(!IsEmpty())
	{
		XPoint pt(x,y);
		m_data.rect.SetRectEmpty();
		//XU32 s=0;
		XDomItem*p=HitTest(x,y);////(XDomItem*)m_cardList[m_nActiveCard]->Handle(XDO_HITTEST,x,y);
		//if(m_pSelect!=XNULL&&m_pSelect->GetID()==XDOM_INPUT&&m_pSelect!=p)
		//	m_pSelect->OnMouseUp(XPoint(-100,-100),&m_data);
		if(p!=XNULL)
			s|=p->OnMouseMove(pt,&m_data);
		if(m_pActive!=p)
		{
			//m_pActive->OnMouseOut(pt,&m_data);
			s|=XRREPAINT;
			m_data.rect.SetRectEmpty();
			if(m_pActive!=XNULL)
			{
				s|=m_pActive->OnMouseOut(pt,&m_data);
				m_pActive->GetRoundRect(m_data.rect);
			}
			if(p!=XNULL)
			{
				p->OnMouseOver(pt,&m_data);
				if(!(p->GetStyle()&XDomItem::DOM_SELECTABLE))
					p=XNULL;
			}

			m_pActive=p;
			if(p!=XNULL)
			{
				XRect rt;
				m_pActive->GetRoundRect(rt);
				m_data.rect|=rt;
			}
		}
		/*if(m_pActive!=XNULL)
		{
			XU32 n=m_pActive->GetStyle();//Handle(XDO_GET_STYLE,0,0);
			if(n&XDomItem::DOM_ANCHOR)
				s|=XRCANCHOR;
			else if(n&XDomItem::DOM_INPUT)
				s|=XRCINPUT;

		}*/
	}
	return s;
}

XU32 XDom::OnTimer()
{
   if(!IsEmpty())
   {
	   //m_data.rect.SetRectEmpty();
	   //m_data.viewRect=view;
	   m_imgList.Timer();
	   XU32 s=GetActiveCard()->OnTimer(&m_data);
	   if(s&XRTIMEOVER)
	   {
		   s&=~XRTIMEOVER;
		   s|=GetActiveCard()->OnTimerOver(&m_data);
	   }
	   return s;
   }
   return XRNONE;
}

//DEL XBOOL XDom::Select(XU8 nDirect,XRect&rect)
//DEL {
//DEL    if(m_nActiveCard<m_cardList.GetSize())
//DEL    {
//DEL 	    m_pActive=XNULL;
//DEL 		XDomItem*p=XNULL;
//DEL 		switch(nDirect)
//DEL 		{
//DEL 		case SELECTINFO::XTOP:
//DEL 			p=m_cardList[m_nActiveCard]->Select(m_pSelect,0,SELECTINFO::XUP);
//DEL 			break;
//DEL 		default:
//DEL 			p=m_cardList[m_nActiveCard]->
//DEL 				Select(m_pSelect,1,nDirect);
//DEL 			break;
//DEL 		}
//DEL 		if(p==XNULL) return XFALSE;
//DEL 		if(m_pSelect!=p)
//DEL 		{
//DEL 			m_pSelect=p;
//DEL 			p->GetRoundRect(rect);
//DEL 			return XTRUE;
//DEL 		}
//DEL    }
//DEL    return XFALSE;
//DEL
//DEL }
//#include <windows.h>
static XBOOL SetParam(XString8Array&list,const char*key,int l,const char*value)
{
	int i,nc=list.GetSize();
	for(i=0;i<nc;i+=2)
	{
		if((!l||list[i].GetLength()==l)
		   &&list[i].Compare(key,XTRUE,l)==0) //return XTRUE;
		{
			list[i+1]=value;
			return XTRUE;
		}
	}
	if(l)
		list.Add(XString8(key,l));
	else
		list.Add(key);
//#if (__CORE_VERSION__>=0x02070200)
	int len=strlen(value);
	XString8 strValue;
	if(len>=2)
	{
		if((value[0]=='\''&&value[len-1]=='\'')||
			(value[0]=='"'&&value[len-1]=='"'))
			strValue.SetString(value+1,len-2);
		else
			strValue.SetString(value,len);
	}
	else
		strValue.SetString(value,len);
	list.Add(strValue);
//#else
//	list.Add(value);
//#endif
	return XFALSE;
}
static void SetFlashVars(XString8Array&args,XString8&strValue)
{
	XString8Array list;
	list.SplitString(strValue,'&');
	int i,nc=list.GetSize();
	for(i=0;i<nc;i++)
	{
		char*str=(char*)list[i];
		char*sp=strchr(str,'=');
		if(sp==str) continue;
		if(sp==XNULL)
		{
			SetParam(args,str,0,"");
		}
		else
		{
			SetParam(args,str,sp-str,sp+1);
		}
	}
}

void XDom::FormatURL(XString8&strURL,XDOMFILTER*pFilter)
{
	if(!pFilter||pFilter->strVars.GetSize()<2) return;
	char*pData=strURL.GetData();
	bool bSet=false;
	int nType=0;
	XString8 strNew,strWord,strKeep;
	while(*pData)
	{
		char c=*pData;
		switch(c)
		{
		case '$':
			if(nType==0)
			{
				strKeep="$";
				nType=1;
			}
			else if(nType==1)
			{
				if(!strKeep.IsEmpty())
				{
					strNew+=strKeep;
					strKeep="$";
				}
			}
			else
				strWord+=c;
			break;
		case '{':
			if(nType==0)
				strNew+=c;
			else if(nType==1)
			{
				nType=2;
				strKeep.Empty();
				strWord.Empty();
				nType=2;
			}
			else
				strWord+=c;
			break;
		case ' ':
			if(nType==0)
				strNew+=c;
			else if(nType==1)
				strKeep+=c;
			break;
		case '}':
			if(nType==0)
			{
				strNew+=c;
			}
			else if(nType==1)
			{
				if(!strKeep.IsEmpty())
				{
					strNew+=strKeep;
					strKeep.Empty();
				}
				strNew+=c;
				nType=0;
			}
			else
			{
				if(!strWord.IsEmpty())
				{
					int i,nCount=pFilter->strVars.GetSize();
					for(i=0;i<nCount;i+=2)
					{
						if(pFilter->strVars[i]==strWord)
						{
							strWord=pFilter->strVars[i+1];
							this->m_pDraw->pView->SetValueOf(strWord);
							strNew+=strWord;
							break;
						}
					}
				}
				nType=0;
			}
			break;
		default:
			if(nType==0)
				strNew+=c;
			else if(nType==1)
			{
				strKeep+=c;
			}
			else if(nType==2)
			{
				strWord+=c;
			}break;

		}
		pData++;
	}
	//if(bSet)
	strURL=strNew;
}

XU32 XDom::Final(CONNECTREQ&req,DRAWCONTEXT&draw,XBOOL bForward,XDOMFILTER*pFilter)
{
   m_strCache.Empty();
   if(m_nLoadStatus>1) return XRNONE;
   if(m_pPhase!=XNULL) delete m_pPhase;
   m_pPhase=XNULL;
   draw.pCss=m_pCSS?m_pCSS:&m_css;
   XU32 s=XRNONE;//m_bLayerout?XRLAYEROUT:XRNONE;
   /*if(m_pFilter&&!m_pFilter->strAutoLocation.IsEmpty())
	  {
		  s|=XRCONNECT;
		  m_data.SetURL(m_pFilter->strAutoLocation);
		  m_data.nMethod=m_pFilter->nMethod;
		  m_data.strRefer=m_pFilter->strHost;
		  m_data.strHeaders.Append(m_pFilter->strAutoHeaders);
	#ifdef _WINEMU
		  XString16 si("--->Auto location:");
		  si+=XString16(m_pFilter->strAutoLocation);
		  draw.pView->Trace(si);
	#endif
		  if(!m_pFilter->strAutoPost.IsEmpty())
			m_data.strDatas.Add(m_pFilter->strAutoPost);
		  m_pFilter=NULL;
		  return s;
	  }*/

   if(!m_pDoc) return XRNONE;
   XDomItem*pActive=m_pDoc;//GetActiveCard();
   //if(!pActive) pActive=this->m_pDoc;//GetActiveCard();
   if(bForward&&pActive)
   {
	    int nObject=1;
		if(pFilter) nObject=pFilter->nAutoObject;
		if(nObject)
		  {
			  XDomList list;
			  pActive->GetList(list,XDOM_OBJECT);
			  if(list.GetSize()==0)
				  pActive->GetList(list,XDOM_EMBED);
			  if(list.GetSize()>=nObject)
			  {
				  XDomItem*pItem=list[nObject-1];
				  list.RemoveAll();
				  //pItem->GetList(list,XDOM_EMBED);
				  XString8 strURL;
				  pItem->GetAttrib(XEAB::DATA_ATR,strURL);
				  pItem->GetList(list,XDOM_PARAM);
				  int i,nc=list.GetSize();
				  for(i=0;i<nc;i++)
				  {
					  XString8 strName,strValue;
					  if(!list[i]->GetAttrib(XEAB::NAME,strName)) continue;
					  if(!list[i]->GetAttrib(XEAB::VALUE,strValue)) continue;
					  if(strName.IsEmpty()) continue;
					  if(strName.Compare("flashvars",XTRUE)==0)
					  {
						  SetFlashVars(m_argList,strValue);
						  SetParam(m_argList,strName,0,strValue);
						  //if(!IsParamExist(m_pFilter,strName,strName.GetLength()))
						  //{
							//m_pFilter->strParams.Add(strName);
							//m_pFilter->strParams.Add(strValue);
						 // }
					  }
					  else if(strName.Compare("movie",XTRUE)==0)
					  {
						  strURL=strValue;
					  }
					  else
					  {
						  SetParam(m_argList,strName,0,strValue);
						  //if(!IsParamExist(m_pFilter,strName,strName.GetLength()))
						  //{
							//m_pFilter->strParams.Add(strName);
							//m_pFilter->strParams.Add(strValue);
						  //}
					  }
				  }
				  list.RemoveAll();
				  pItem->GetList(list,XDOM_EMBED);
				  if(list.GetSize())
				  {
					  XDomItem*pItem=list[0];
					  nc=pItem->GetAttribCount();
					  for(int i=0;i<nc;i++)
					  {
						  XVar var;
						  pItem->GetAttribByIndex(var,i);
						  switch(var.nID)
						  {
						  case XEAB::SRC:
							   //if(strURL.IsEmpty())
									strURL=var.ToString();
							   break;
						  case XEAB::FLASHVARS:
							  {
								  XString8 strValue=var.ToString();
								  SetFlashVars(m_argList,strValue);
							  }
						  default:
							  {
								  XPCTSTR strKey=(XPCTSTR)XEAB().GetData(var.nID);//XEAB::_attribs[var.nID].strName;
								  if(strKey)
								  {
									SetParam(m_argList,strKey,0,var.ToString());
								  }
								  else
								  {
									  XString8 strTmp(var.ToString() );
									  SetFlashVars(m_argList, strTmp );
								  }
								  //else
									//  int v=0;
								  //if(!IsParamExist(m_pFilter,strKey,0))
								  //{
									 // m_pFilter->strParams.Add(strKey);
									//  m_pFilter->strParams.Add(
								  //}
							  }break;
						  }
					  }
				  }
				  //XDomItem*pEm=list.GetSize()?list[0]:XNULL;
				  if(!strURL.IsEmpty())
				  {
					FormatURL(strURL,pFilter);
					m_data.strRefer=this->m_data.setURL.m_strURL;

					s|=XRCONNECT;
					if(!pFilter||!pFilter->bModifyDomain)
						s|=XREFRESH;
					else
						m_bFrame=XFALSE;
					m_data.SetURL(strURL);//URL.SetURL(strURL);
#ifdef _WINEMU
					XString16 ss("--->Auto Object URL:");
					ss+=XString16(m_data.tempURL.m_strURL);
					draw.pView->Trace(ss);
#endif
				  }
			  }
		  }
		  int nAutoFrame=1;
		  if(pFilter) nAutoFrame=pFilter->nAutoFrame;
		  if(nAutoFrame)
		  {
			  XDomList list;
			  pActive->GetList(list,XDOM_IFRAME,true);
			  pActive->GetList(list,XDOM_FRAME,true);
			  int nCount=list.GetSize();
			  m_data.strRefer=this->m_data.setURL.m_strURL;
			  //for(i=0;i<nCount;i++)
			  //i=nCount-1;
			  if(nCount>=nAutoFrame)
			  {
				  XString8 strURL;
				  list[nAutoFrame-1]->GetAttrib(XEAB::SRC,strURL);
				  if(!strURL.IsEmpty())
				  {
				  FormatURL(strURL,pFilter);
#ifdef _WINEMU
				  XString16 ss("--->Auto Frame URL:");
				  ss+=XString16(strURL);
				  draw.pView->Trace(ss);
#endif
				  s|=XRCONNECT;
				  if(pFilter&&!pFilter->bModifyDomain)//!pFilter||!pFilter->bModifyDomain)
				  {
					  s|=XREFRESH;
					  m_pFrameRoot=list[nAutoFrame-1];
					  //m_bFrame=XTRUE;
					  //m_frameURL=strURL;//GetURL0();
				  }
				  else
					  m_bFrame=XFALSE;
				  m_data.SetURL(strURL);//URL.SetURL(strURL);
				  }
			  }
		  }
   }
   if(pFilter)
   {
	  m_bLayerout=XFALSE;

	  if(pActive)
	  {

		  if(pFilter->nAutoForm)
		  {
			  XDomList list;
			  pActive->GetList(list,XDOM_FORM);
			  m_data.strRefer=this->m_data.setURL.m_strURL;
			  if(list.GetSize()>=pFilter->nAutoForm)
			  {
				  s|=list[pFilter->nAutoForm-1]->Handle(XDO_SUBMIT,0,(XU32)&m_data);
				  if(!pFilter->bModifyDomain)
						s|=XREFRESH;
				  else
					  m_bFrame=XFALSE;
				  if(pFilter->strFormDatas.GetSize())
				  {
					  int i,nc=pFilter->strFormDatas.GetSize();
					  for(i=0;i<nc;i+=2)
					  {
						  XString8&strKey=pFilter->strFormDatas[i];
						  XString8&strValue=pFilter->strFormDatas[i+1];
						  XSortString8::SSetValue(m_data.strDatas,strKey,strValue);
					  }
				  }
#ifdef _WINEMU
				  XString8 strURL;
				  list[0]->GetAttrib(XEAB::ACTION,strURL);
				  XString16 s("--->Auto Form URL:");
				  s+=XString16(strURL);
				  draw.pView->Trace(s);
#endif
			  }
		  }

		  if(!pFilter->strAutoGo.IsEmpty())//nAutoGo)
		  {
			  XDomList list;
			  pActive->GetList(list,XDOM_A);
			  m_data.strRefer=this->m_data.setURL.m_strURL;
			  int id=pFilter->strAutoGo.ToInt();
			  if(list.GetSize())//>=pFilter->nAutoGo)
			  {
				  if(id>0&&id<=list.GetSize())
				  {
						  s|=list[id-1]->Handle(XDO_EVENT_ONCLICK,0,(XU32)&m_data);
		#ifdef _WINEMU
						  XString8 strURL;
						  list[id-1]->GetAttrib(XEAB::HREF,strURL);
						  XString16 s("--->Auto GO URL:");
						  s+=XString16(strURL);
						  draw.pView->Trace(s);
		#endif
				  }
				  else
				  {
					  int i,nc=list.GetSize();
					  //XString16 strKey(pFilter->strAutoGo);
					  for(i=0;i<nc;i++)
					  {
						  XDomItem*pItem=list[i];
						  XString16 txt;
						  pItem->GetText(txt);
						  XString8 strKey(txt);
						  if(!(strKey==pFilter->strAutoGo)) continue;
						  s|=//list[pFilter->nAutoGo-1]
							 pItem->Handle(XDO_EVENT_ONCLICK,0,(XU32)&m_data);
		#ifdef _WINEMU
						  XString8 strURL;
						  pItem->GetAttrib(XEAB::HREF,strURL);
						  XString16 s("--->Auto GO URL:");
						  s+=XString16(strURL);
						  draw.pView->Trace(s);
		#endif
						  break;
					  }
				  }
			  }
		  }
	  }

   }

   if(IsEmpty()) return s;
   //m_css.DoCSS(this,XTRUE);
   /*if(!m_css.IsEmpty())
	{
	    XU32 t1=GetTickCount();
		GetActiveCard()->SetCSS(&m_css);
		XU32 t2=GetTickCount()-t1;
		m_css.Clear();

	}*/

   //if(m_bLayerout)
//	 Layerout(draw);
//   m_pFilter=NULL;
   m_pPhase=XNULL;
   m_nLoadStatus=2;
   m_cardList.FreeExtra();
   m_doms.m_list.FreeExtra();
   //m_domList.FreeExtra();
   m_nActiveCard=GetCardIndex();
   if(!req.URL.m_strCard.IsEmpty())
   {
	   m_strCard=req.URL.m_strCard;
	   //s|=SetCard(req);
   }
   if(m_pHeader!=XNULL)
	   s|=m_pHeader->OnLoad(&m_data);
   if(!IsEmpty())
   {
	   s|=GetActiveCard()->OnLoad(&m_data);
	   if(req.bForward!=XCT_CONNECT)
	   {
		   m_data.strValues.Copy(req.strValues);
		   s|=GetActiveCard()->OnRefresh(&m_data);
	   }
	   s|=SetSelect(req);
	   s|=req.bForward?
		  GetActiveCard()->OnEnterForward(&m_data):
		  GetActiveCard()->OnEnterBackward(&m_data);
   }
   return s;
}


XU32 XDom::OnMouseUp(XINT x,XINT y)
{
	if(IsEmpty()) return XRNONE;
	m_data.rect.SetRectEmpty();
	XDomItem*pItem=HitTest(x,y);//GetActiveCard()->HitTest(x,y);
	XU32 s=XRNONE;//GetActiveCard()->OnMouseDown(XPoint(x,y),&m_data);
	if(pItem!=XNULL)
	{
		XPoint pt(x, y);
		s|=pItem->OnMouseUp(pt,&m_data);
		//pItem->OnClick(XPoint(x,y),&m_data);
	}
	/*XDomItem*pItem=GetActiveCard()->HitTest(x,y);
	if(pItem!=XNULL)
	{
		if(pItem->GetStyle()&XDomItem::DOM_STATIC)
				s|=SetActive(&m_pText,pItem);
		else
				s|=SetActive(&m_pSelect,pItem);
		s|=pItem->OnClick(XPoint(x,y),&m_data);
	}*/
	return s;
}

XU32 XDom::OnMouseDown(XINT x,XINT y)
{
	if(IsEmpty()) return XRNONE;
	XU32 s=XRNONE;
	m_data.rect.SetRectEmpty();
	XDomItem*pItem=HitTest(x,y);//GetActiveCard()->HitTest(x,y);
	/*if(m_pSelect!=XNULL&&pItem!=m_pSelect)
	{
		s|=m_pSelect->OnMouseDown(XPoint(x,y),&m_data);
	}*/
	//GetActiveCard()->OnMouseDown(XPoint(x,y),&m_data);
	if(pItem!=XNULL)
	{
		XPoint pt(x, y);
		s|=pItem->OnMouseDown(pt,&m_data);
	}
	//XDomItem*pItem=GetActiveCard()->HitTest(x,y);
	if(pItem!=XNULL)
	{
		if(pItem->GetStyle()&XDomItem::DOM_STATIC)
				s|=SetActive(&m_pText,pItem);
		else
				s|=SetActive(&m_pSelect,pItem);
		//s|=pItem->OnClick(XPoint(x,y),&m_data);
	}
	return s;
}


XU32 XDom::OnDblClick(XINT x,XINT y)
{
	if(IsEmpty()) return XRNONE;
	XU32 s=XRNONE;
	m_data.rect.SetRectEmpty();
	XDomItem*pItem=HitTest(x,y);//GetActiveCard()->HitTest(x,y);
	if(pItem!=XNULL)
	{
		XPoint pt;
		s|=pItem->OnDblClick(pt,&m_data);
	}
	return s;
}

XU32 XDom::OnClick(XINT x,XINT y)
{
	if(IsEmpty()) return XRNONE;
	XU32 s=XRNONE;
	m_data.rect.SetRectEmpty();
	m_data.strDatas.RemoveAll();
	m_data.strValues.RemoveAll();
	XDomItem*pItem=HitTest(x,y);//GetActiveCard()->HitTest(x,y);
	if(m_pSelect!=XNULL&&m_pSelect!=pItem)
	{
		int nID=m_pSelect->GetID();
		if(nID==XDOM_INPUT)
		{
			int v;
			if(m_pSelect->GetAttrib(XEAB::TYPE,v)&&v==XEIT::TEXT)
			{
				s|=m_pSelect->OnClick(XPoint(-100,-100),&m_data);
			}
		}
		if(nID==XDOM_SELECT||nID==XDOM_TEXTAREA)
		   s|=m_pSelect->OnClick(XPoint(-100,-100),&m_data);
		 return s;
	}
	//{
	//	if(m_pSelect->GetID()==XDOM_INPUT)
	//	s|=m_pSelect->OnClick(XPoint(-100,-100),&m_data);
	//}
	if(pItem!=XNULL)
	{
		if(pItem->GetStyle()&XDomItem::DOM_STATIC)
				s|=SetActive(&m_pText,pItem);
		else
				s|=SetActive(&m_pSelect,pItem);
		s|=pItem->OnClick(XPoint(x,y),&m_data);
	}
	return s;
}

XU32 XDom::OnString(XPCWSTR strBuf)
{
	if(IsEmpty()) return XRNONE;
	XU32 s=XRNONE;
	//m_data.viewRect=view;
	//m_data.rect.SetRectEmpty();
	//m_data.offset.x=view.left;
	//m_data.offset.y=view.top;
	if(m_pSelect!=XNULL)
	{
		s=m_pSelect->OnString(strBuf,&m_data);
		if(s!=XRNONE) return s;
	}
	return s;
}

XU32 XDom::OnChar(XWCHAR c)
{
	if(IsEmpty()) return XRNONE;
	XU32 s=XRNONE;
	//m_data.viewRect=view;
	//m_data.rect.SetRectEmpty();
	//m_data.offset.x=view.left;
	//m_data.offset.y=view.top;
	if(m_pSelect!=XNULL)
	{
		s=m_pSelect->OnChar(c,&m_data);
		if(s!=XRNONE) return s;
	}
	return s;
}

XU32 XDom::OnKeyDown(XWCHAR c,DRAWCONTEXT*pDraw,XU8 bNext)
{
	if(IsEmpty()) return XRNONE;
	pDraw->pCss=&m_css;
	//XDomItem*pOld=m_pSelect==XNULL?m_pText:m_pSelect;
	XU32 s=XRNONE;

	m_data.rect.SetRectEmpty();

	if(!bNext)
	{
		if(m_pText!=XNULL)
		{
			s=m_pText->OnKeyDown(c,&m_data);
			if(s!=XRNONE) return s;
			//m_pText=XNULL;
		}
		if(m_pSelect!=XNULL)
		{
			s=m_pSelect->OnKeyDown(c,&m_data);
			if(s!=XRNONE) return s;
		}
	}

	{
		XPoint dest(pDraw->offset);
		XRect  rect;
		XDomItem*pText=m_pText;
		XDomItem*pSelect=Select(m_pSelect,/*&pText,*/pDraw,c,dest);

		//s|=SetActive(&m_pText,pText);

		if(pSelect!=XNULL)
			s|=SetActive(&m_pSelect,pSelect);
		else if(m_data.offset.y<=0&&(c==XK_UP||c==XK_NUM2))
			s|=XRACTIVE;
		if(dest!=m_data.offset)
		{
			s|=XROFFSET;
			m_data.offset=dest;
		}
	}
	return s;
}

XDomItem* XDom::Select(XDomItem*pOld,/*XDomItem**pTxt,*/DRAWCONTEXT*pDraw,XWCHAR c, XPOINT &dest)
{
	//if(m_nActiveCard>=m_cardList.GetSize()) return XNULL;
	if(MoveSelect(pOld,pDraw,c,dest)) return XNULL;
	pDraw->pCss=&m_css;

	XDomItem*pCard=GetActiveCard();
	if(pCard==XNULL) return XNULL;
	XDomItem*pSelect=SelectDirect(pOld,pDraw,c,dest);

	if(pSelect==XNULL)
	{
		switch(c)
		{
		case XK_RIGHT:
			 if(dest.x<=0)
				pSelect=SelectNext(pOld,pDraw,SELECTMARK::XNEXT,dest);
			 break;
		case XK_LEFT:
			 if(dest.x+pDraw->view.right>=m_docSize.cx)
				pSelect=SelectNext(pOld,pDraw,SELECTMARK::XLAST,dest);
			 break;
		default:
			 pSelect=SelectNext(pOld,pDraw,c,dest);
		}
		if(pSelect!=XNULL) return pSelect;
	}

	XRect rectDest;
	if(pSelect!=XNULL)
		pSelect->GetRoundRect(rectDest);
	else
		rectDest=XRect(0,0,m_docSize.cx,m_docSize.cy);
	int my=(pDraw->SPACE)<<1;
	int mx=(pDraw->view.Width())>>2;
	int mmy=(pDraw->view.Height()-my);
	int mmx=(pDraw->view.Width()-mx);

	switch(c)
	{
	case XK_DOWN:
		 if(pSelect!=XNULL&&
			 rectDest.top-dest.y-mmy<pDraw->view.bottom)
		 {
			 if((rectDest.left-dest.x+pDraw->SPACE>pDraw->view.right||
				 rectDest.right-dest.x-pDraw->SPACE<pDraw->view.left))
					 dest.x=XMIN(rectDest.left-pDraw->view.left,
								 rectDest.right-pDraw->view.right);
		 }
		 if(rectDest.bottom-dest.y+pDraw->SPACE>=pDraw->view.bottom)
			 dest.y=XMIN(dest.y+mmy,rectDest.bottom-pDraw->view.bottom+my);
		 /*else if(rectDest.bottom-dest.y<pDraw->view.top)
		 {
			 dest.y=rectDest.top-pDraw->view.top-pDraw->SPACE;
			 dest.x=rectDest.left-pDraw->view.left;
		 }*/

		 break;
	case XK_UP:

		 if(pSelect!=XNULL&&rectDest.bottom-dest.y+mmy>pDraw->view.top)
		 {
			 if(rectDest.left-dest.x+pDraw->SPACE>pDraw->view.right||
				rectDest.right-dest.x-pDraw->SPACE<pDraw->view.left)
				 dest.x=XMIN(rectDest.left-pDraw->view.left,
							 rectDest.right-pDraw->view.right);
		 }
		 if(rectDest.top-dest.y-pDraw->SPACE<=pDraw->view.top)
			 dest.y=XMAX(dest.y-mmy,rectDest.top-pDraw->view.top-my);
		 /*else if(rectDest.top-dest.y>pDraw->view.bottom)
		 {
			 dest.y=rectDest.top-pDraw->view.top-pDraw->SPACE;
			 dest.x=rectDest.left-pDraw->view.left;
		 }*/
			 //dest.y-=mmy;
		 break;
	case XK_RIGHT:

		 /*if(pSelect!=XNULL&&rectDest.bottom-dest.y<pDraw->view.top)
		 {
			 dest.y=rectDest.top-pDraw->view.top-pDraw->SPACE;
			 dest.x=rectDest.left-pDraw->view.left;
		 }
		 else*/
		 if(rectDest.right-dest.x>pDraw->view.right)
		 {
			 dest.x=XMIN(dest.x+mmx,rectDest.right-pDraw->view.right);
		 }
		 else if(rectDest.bottom-dest.y>pDraw->view.bottom)
			 dest.y=XMIN(dest.y+(my>>1),XMAX(dest.y,rectDest.bottom-pDraw->view.bottom));
		 break;
	case XK_LEFT:

		 //if(pSelect!=XNULL&&rectDest.top-dest.y>=pDraw->view.bottom)
		//	 dest.y=rectDest.bottom-pDraw->view.bottom+pDraw->SPACE;
		 //else
		 if(rectDest.left-dest.x<pDraw->view.left)
		 	  dest.x=XMAX(dest.x-mmx,rectDest.left-pDraw->view.left);
		 else if(rectDest.top-dest.y<pDraw->view.top)
			 dest.y=XMAX(dest.y-(my>>1),XMIN(dest.y,rectDest.top-pDraw->view.top));

		 break;
	}

	if(pSelect!=XNULL)
	{
		XRect rSelect;
		pSelect->GetRoundRect(rSelect);
		rSelect-=dest;
		if(!rSelect.IsUnion(pDraw->view)) pSelect=XNULL;

	}

	return pSelect;

}


XDomItem* XDom::SelectDirect(XDomItem*pSelect,DRAWCONTEXT*pDraw,XWCHAR& c,XPOINT&pt)
{
	int nd=1,np=XMAX(4,pDraw->view.Height()/(pDraw->SPACE)-2);
	switch(c)
	{
		case XK_NUM2:
			 c=XK_UP;nd=np;
			 break;
		case XK_NUM4:
			 c=XK_LEFT;nd=np>>1;break;
		case XK_NUM8:
			 c=XK_DOWN;nd=np;break;
		case XK_NUM6:
			 c=XK_RIGHT;nd=np>>1;
			 break;
		case XK_DOWN:
		case XK_UP:
		case XK_RIGHT:
		case XK_LEFT:break;
		default:return XNULL;
	}
	XSelection sec;
	sec.Init(m_pDraw,pSelect,m_docSize,(XU8)c,nd);
	GetActiveCard()->MarkSelection(&sec);
	return sec.GetSelect();
}

XBOOL XDom::MoveSelect(XDomItem*pSelect,DRAWCONTEXT*pDraw,XWCHAR c,XPOINT&pt)
{
	if(pSelect==XNULL) return XFALSE;
	XBOOL bSet=XFALSE;
	XRect rect;
	pSelect->GetRoundRect(rect);
	int my=XMAX(pDraw->view.Height()>>2,pDraw->SPACE<<1);
		   //(pDraw->nRowSpace+pDraw->SPACE)<<2;
	int mx=XMAX(pDraw->SPACE<<2,(pDraw->view.Width())>>2);
	int dd=(pDraw->SPACE)<<1;
	switch(c)
	{
	   case XK_RIGHT:
		    if(rect.right-pt.x>pDraw->view.right&&
			   rect.left-pt.x<pDraw->view.right)
			{
				bSet=XTRUE;
				pt.x=XMIN(pt.x+mx,rect.right-pDraw->view.right+dd);
			}break;
	   case XK_LEFT:
		    if(rect.left-pt.x<pDraw->view.left&&
			   rect.right-pt.x>pDraw->view.left)
			{
				bSet=XTRUE;
				pt.x=XMAX(pt.x-mx,rect.left-pDraw->view.left-dd);
			}break;
	   case XK_UP:
		    if(rect.top-pt.y<pDraw->view.top&&
			   rect.bottom-pt.y>pDraw->view.top)
			{
				bSet=XTRUE;
				pt.y=XMAX(pt.y-my,rect.top-pDraw->view.top-dd);
			}
		    break;
	   case XK_DOWN:
		    if(rect.bottom-pt.y>pDraw->view.bottom&&
			   rect.top-pt.y<pDraw->view.bottom)
			{
				bSet=XTRUE;
				pt.y=XMIN(pt.y+my,rect.bottom-pDraw->view.bottom+dd);
			}break;
	}
	return bSet;
}


XDomItem* XDom::SelectNext(XDomItem*pOld,/*XDomItem**pTxt,*/DRAWCONTEXT*pDraw,XWCHAR c, XPOINT &dest)
{
	//if(m_nActiveCard>=m_cardList.GetSize()) return XNULL;

	XDomItem*pCard=GetActiveCard();
	if(pCard==XNULL) return XNULL;
	XDomItem*pSelect=XNULL;

	switch(c)
	{
		case SELECTMARK::XNEXT:
		case SELECTMARK::XLAST:break;
		default:return pSelect;
	}

	pSelect=pCard->Select(pOld,(XU8)c);
	if(pSelect!=XNULL)
	{
		XRect rect;
		pSelect->GetRoundRect(rect);
		if(rect.left-dest.x<pDraw->view.left||
		   rect.right-dest.x>pDraw->view.right)
			dest.x=XMIN(rect.left-pDraw->view.left,
					    rect.right-pDraw->view.right);
		if(rect.top-dest.y-pDraw->SPACE<pDraw->view.top)
			dest.y=rect.top-pDraw->view.top-pDraw->SPACE;
		if(rect.bottom-dest.y+pDraw->SPACE>pDraw->view.bottom)
			dest.y=rect.bottom+pDraw->SPACE-pDraw->view.bottom;

	}

	return pSelect;

}

//DEL XBOOL XDom::MoveTo(DRAWCONTEXT *pDraw, XINT x, XINT y, XPOINT &dest,XS8 dx,XS8 dy)
//DEL {
//DEL 	XBOOL bOK=XTRUE;
//DEL 	XINT lx=x-pDraw->offset.x;
//DEL 	XINT ly=y-pDraw->offset.y;
//DEL 	XINT lh=(pDraw->SPACE+pDraw->nRowSpace);
//DEL 	XINT mx=XABS(dx)<2?pDraw->view.right-pDraw->view.left-(lh<<1):XABS(dx);
//DEL 	XINT my=XABS(dy)<2?pDraw->view.bottom-pDraw->view.top-(lh<<1):XABS(dy);
//DEL 	if(XABS(lx)<lh*3||dx*lx<=0)
//DEL 		dest.x=x;
//DEL 	else
//DEL 	{
//DEL 		bOK=XFALSE;
//DEL 		if(lx>0)
//DEL 			dest.x=pDraw->offset.x+mx;
//DEL 		else
//DEL 			dest.x=pDraw->offset.y-mx;
//DEL 	}
//DEL 	if(XABS(ly)<lh*3||dy*ly<=0)
//DEL 		dest.y=y;
//DEL 	else
//DEL 	{
//DEL 		bOK=XFALSE;
//DEL 		if(ly>0)
//DEL 			dest.y=pDraw->offset.y+my;
//DEL 		else
//DEL 			dest.y=pDraw->offset.y-my;
//DEL 	}
//DEL 	return bOK;
//DEL }





XU32 XDom::SetActive(XDomItem**pActive,XDomItem *pItem)
{
	if(*pActive==pItem) return XRNONE;
	XU32 s=XRREPAINT;
	if(*pActive!=XNULL)
	{
	   XRect rt;
	   (*pActive)->GetRoundRect(rt);
	   m_data.rect|=rt;
	   s|=(*pActive)->OnBlur(&m_data);
	}
	if(pItem!=XNULL)
	{
		XRect rt;
		pItem->GetRoundRect(rt);
		m_data.rect|=rt;
		s|=pItem->OnFocus(&m_data);
	}
	*pActive=pItem;
	return s;
}

XINT XDom::GetCardIndex()
{
   //XVar var(XEAB::ID,0);
   for(XU32 i=0;i<m_cardList.GetSize();i++)
   {
	  //if(!m_cardList[i]->GetAttrib(var)) continue;
	  XPCTSTR strID=GetID(m_cardList[i]);
	  if(strID==XNULL) continue;
	  if(m_data.setURL.m_strCard.Compare(strID,XTRUE)==0) return i;
   }
   return 0;
}

XU32 XDom::SetCard(CONNECTREQ&req)
{
   //if(m_data.URL.m_strCard==strCard) return XRNONE;
   //m_data.URL.m_strCard=strCard;
	XU32 s=XRNONE;
	if(IsEmpty()) return 0;
	if(!req.URL.m_strCard.IsEmpty())
	{
		XVar var(XEAB::NAME,req.URL.m_strCard);
		XU32 nIndex=0;
		XDomItem *pItem=GetActiveCard()->FindByAttrib(var,nIndex);
		if(pItem==XNULL)
		{
			XINT id=m_css.IDOF(req.URL.m_strCard,XCSS::XCSS_ID);
			if(id<0) return XRNONE;
			//var=id;
			//var.nID=XEAB::ID;
			pItem=m_doms.m_listByID.GetItem(id);
				//GetActiveCard()->FindByAttrib(var,nIndex);
		}
		if(pItem!=XNULL)
		{
			XRect rect;
			pItem->GetRoundRect(rect);
			if(rect.IsRectEmpty())
			{
				XVar var(XEAB::POSX,0);
				pItem->GetAttrib(var);
				m_data.offset.x=(XU16)var.iData;
				var.nID=XEAB::POSY;
				pItem->GetAttrib(var);
				m_data.offset.y=(XU16)var.iData-XWindow::m_nTextHeight;
			}
			else
			{
				m_data.offset.x=rect.left;
				m_data.offset.y=rect.top-XWindow::m_nTextHeight;
			}
			s|=XROFFSET;
			if(!(pItem->GetStyle()&XDomItem::DOM_SELECTABLE)||rect.IsRectEmpty())
				pItem=GetActiveCard()->Select(pItem);
			m_pSelect=XNULL;
			s|=SetSelect(req,pItem);
			return s;
		}
	}

   m_data.setURL=m_data.tempURL;
   XINT id=GetCardIndex();

   if(id!=m_nActiveCard) //return XRNONE;
   {
	m_pSelect=m_pText=XNULL;
	s=GetActiveCard()->OnUnLoad(&m_data)|XRREPAINT;
	m_data.rect=XRect(0,0,m_docSize.cx,m_docSize.cy);
	m_nActiveCard=id;

	s|=XRLAYEROUT;
   //m_bLayerout=XTRUE;
	s|=GetActiveCard()->OnLoad(&m_data);
	s|=req.bForward?
	  GetActiveCard()->OnEnterForward(&m_data):
	  GetActiveCard()->OnEnterBackward(&m_data);
	}
   if(!IsEmpty())
	   s|=SetSelect(req);
   return s;
}

void XDom::SetData(XString8&strValue)
{
	if(m_pItem==XNULL) return;
	m_pItem->SetData(strValue,strValue.GetLength());
	if(m_pItem->GetID()==XDOM_SCRIPT)
	{
		int nType=0;
		if(m_pItem->GetAttrib(XEAB::TYPE,nType))
		{
			//strType.MakeLower();
			//if(strType=="text/baiwanscript")
			if(nType==XEIT::TEXT_BAIWAN)
				XClientApply::GetInstance()->AddFilters(strValue,m_pDraw?this->m_pDraw->pView:NULL);
		}

	}
}

void XDom::Release()
{
	if(m_pChildDom)
		delete m_pChildDom;
	m_css.Reset();
	m_pChildDom=XNULL;
	m_pRoot=XNULL;
	m_pItem=XNULL;
	m_pDoc=XNULL;
	m_pHeader=XNULL;
	m_pTemplate=XNULL;
	m_nActiveCard=0;
	m_pSelect=XNULL;
	m_pText=XNULL;
	m_cardList.RemoveAll();
	m_docSize.cx=m_docSize.cy=0;
	m_nLoadStatus=0;
	/*for(XU32 i=0;i<m_domList.GetSize();i++)
	{
		if(m_domList[i]!=&XDomBR::m_br&&
		   m_domList[i]!=&XDomSpace::m_space)
			delete m_domList[i];
	}*/
	m_doms.Release();
	m_imgList.Release();
//	m_imgList.FreeAll();
//	m_domList.RemoveAll();
	m_stack.RemoveAll();
	m_nFormat=0;
	m_nVersion=10;
	m_pActive=XNULL;

	m_data.strRefer.Empty();
	m_data.nMethod=XEM::GET;
	m_data.strHeaders.RemoveAll();
	m_data.strValues.RemoveAll();
	m_data.strDatas.RemoveAll();
	m_data.offset.x=0;
	m_data.offset.y=0;
	m_data.rect.SetRectEmpty();
}



XU32 XDom::SetSelect(CONNECTREQ &req,XDomItem*ps)
{
	if(IsEmpty()) return XRNONE;
	/*XU32 s=req.bForward?
		  GetActiveCard()->OnEnterForward(&m_data):
		  GetActiveCard()->OnEnterBackward(&m_data);*/
	//if(s&XRCONNECT) return s;
	//m_pText=XNULL;
	//m_pSelect=XNULL;
	XU32 s=XRNONE;
	XDomItem*p=GetActiveCard();
	if(req.bForward==XCT_CONNECT||req.bForward==XCT_DIR)
	{
	   if(m_pSelect==XNULL)
	   {
		 XDomItem*pSelect=ps==XNULL?p->Select(XNULL):ps;//,&m_pText,1,SELECTINFO::XDOWN);
	     if(m_pSelect==XNULL)
		   s|=SetActive(&m_pSelect,pSelect);
	   }
	}
	else
	{
	   XU32 nIndex=0;
	   s|=SetActive(&m_pSelect,p->FindByIndex(req.nSelect,nIndex,0));
	   nIndex=0;
	   s|=SetActive(&m_pText,p->FindByIndex(req.nText,nIndex,0));
	}
	return s;
}

XBOOL XDom::AddText(XPCWSTR strCnt)
{
	if(m_pItem==NULL) return XFALSE;
	m_pItem->Start(m_pDraw,&m_data);
	XDomText* p=new XDomText(XDOM_TEXT);
	p->SetText16(strCnt);//=strCnt;
	if(!m_pItem->AddChild(p))
	{
		/*Parent();
		if(m_pItem!=XNULL)
		{
			if(!m_pItem->AddChild(p))
			{
				delete p;
				return XFALSE;
			}

		}
		else {delete p;return XFALSE;}*/
		delete p;
	}
	//if(p!=XNULL)
	else
	{
		//m_domList.Add(p);
		m_doms.AddItem(p);
		p->Final(m_pDraw,&m_data);
	}
	SetLayerout();
	return XTRUE;
}

void XDom::AddSpace()
{
   if(AddTag(XDomSpace::CreateObject(XDOM_SPACE)))
	Parent();
}

void XDom::AddBR()
{
   AddTag(XDomBR::CreateObject(XDOM_BR));
   Parent();
}

void XDom::RemoveCard(XDomItem *p)
{
   XU32 i;
   for(i=0;i<m_cardList.GetSize();i++)
   {
	   if(m_cardList[i]==p) {m_cardList.RemoveAt(i);break;}
   }
   XStack<XDomItem*> stack;
   stack.Push(p);
   //XU32 index=0;
   while(stack.GetSize()>0)
   {
	   if(!stack.Pop(p)) break;
	   m_doms.Remove(p);
	   /*for(i=0;i<m_domList.GetSize();i++)
	   {
		   if(m_domList[index]==p)
		   {
			   m_domList.RemoveAt(index);
			   if(index>=m_domList.GetSize()) index=0;
			   break;
		   }
		   if(index+1<m_domList.GetSize()) index++;
		   else index=0;
	   }*/
	   XU32 nCount=p->GetChildCount();
	   for(i=0;i<nCount;i++)
	   {
		   XDomItem*pi=p->GetChild(i);
		   stack.Push(pi);
	   }
   }
}

XBOOL XDom::AddCardItem(XDomItem *pc)
{
   XDomItem*pa=GetActiveCard();
   //if(pa==XNULL) return XFALSE;
   XStack<XDomItem*> stack;
   stack.Push(pc);

   while(stack.GetSize()>0)
   {
 	   XDomItem* p;
 	   if(!stack.Pop(p)) break;
 	   XU32 nCount=p->GetChildCount();
 	   for(XU32 i=0;i<nCount;i++)
 	   {
 		   XDomItem*pi=p->GetChild(i);
		   if(p==pc&&pa!=XNULL)
		     pa->AddChild(pi);
 		   stack.Push(pi);
 		   if(pa!=XNULL)
			   m_doms.AddItem(pi);
			   //m_domList.Add(pi);
		   else delete pi;
 	   }
   }
   delete pc;
//   ProcImages();
   SetLayerout();
   return pa!=XNULL;
}

//DEL void XDom::Invalidate()
//DEL {
//DEL    XRect rect=m_view;
//DEL    InvalidateRect(rect);
//DEL }


//DEL void XDom::SetTitle()
//DEL {
//DEL    if(m_data.pView==XNULL) return;
//DEL    XDomItem*p=GetActiveCard();
//DEL    if(p==XNULL) return;
//DEL    p->SetTitle(m_data.pView);
//DEL }

void XDom::GetList(XDomList &list, XU16 id)
{
	m_doms.GetListByID(list,id);
	/*for(XU32 i=0;i<m_domList.GetSize();i++)
	{
		if(m_domList[i]->GetID()!=id) continue;
		list.Add(m_domList[i]);
	}*/
}

//DEL XBOOL XDom::JustCard(CONNECTREQ &req1, CONNECTREQ &req2)
//DEL {
//DEL 	if(req1.URL.m_strURL!=req2.URL.m_strURL) return XFALSE;
//DEL 	if(req1.nMethod!=req2.nMethod) return XFALSE;
//DEL 	if(req1.nMethod==XEM::POST)
//DEL 	{
//DEL 		if(req1.data.GetSize()!=req1.data.GetSize()) return XFALSE;
//DEL 	}
//DEL 	return XTRUE;
//DEL }

XDomItem* XDom::HitTest(int x, int y)
{
	if(m_pSelect!=XNULL)
	{	if(m_pSelect->HitTest(x,y)) return m_pSelect;}
	if(!IsEmpty())
		return GetActiveCard()->HitTest(x,y);
	return XNULL;
}

XBOOL XDom::Select(XU32 id)
{
	XDomItem*p=GetActiveCard();
	if(p==XNULL) return XFALSE;
	XU32 nIndex=0;
	XDomItem*pi=p->FindByIndex(id,nIndex,0);
	if(pi==XNULL)
	{

		return XTRUE;
	}
	if(pi->GetStyle()&XDomItem::DOM_SELECTABLE)
		SetActive(&m_pSelect,pi);
	else return XFALSE;
	return XTRUE;
}

XU32 XDom::OnFocus()
{
	if(m_pSelect!=XNULL)
		return m_pSelect->OnFocus(&m_data);
	else return 0;
}

XU32 XDom::OnBlur()
{
	if(m_pSelect!=XNULL)
		return m_pSelect->OnBlur(&m_data);
	else return 0;
}

XU32 XDom::Refresh()
{
	if(IsEmpty()) return 0;
	return GetActiveCard()->OnRefresh(&m_data);
}

XPCTSTR XDom::IsScriptItem()
{
   if(m_pItem==XNULL) return XNULL;
   if(m_pItem->GetStyle()&XDomItem::DOM_SCRIPT)
   {
	   return XDomPhaseXML::GetToken(m_pItem->GetID());
	   //return XTRUE;
   }
   return XNULL;
}

XBOOL XDom::FinishToken(XBOOL bEnd)
{
   if(m_pItem==XNULL) return XFALSE;
   m_pItem->Start(m_pDraw,&m_data);
   if(bEnd&&!(m_pItem->GetStyle()&XDomItem::DOM_NODE))
	   Parent();
   return XTRUE;
}

//DEL void XDom::AddToList(XDomList &list, XDomItem *pItem)
//DEL {
//DEL 	for(XU32 i=0;i<list.GetSize();i++)
//DEL 		if(list[i]==pItem) return;
//DEL 	list.Add(pItem);
//DEL }

//DEL XBOOL XDom::UpdateDoms(XDomList &list)
//DEL {
//DEL 	if(list.GetSize()<=0) return XFALSE;
//DEL 	for(XU32 i=0;i<list.GetSize();i++)
//DEL 	{
//DEL 		list[i]->Handle(XDO_UPDATE_IMAGE,(XU32)m_pDraw,(XU32)&m_data);
//DEL 		//SetLayerout();
//DEL 	}
//DEL 	return XTRUE;
//DEL }

XBOOL XDom::IsStartItem()
{
	if(m_pItem==XNULL) return XFALSE;
	if(m_pItem->GetID()==XDOM_WML||
	   m_pItem->GetID()==XDOM_HTML) return XTRUE;
	return XFALSE;
}

XPCTSTR XDom::GetID(XDomItem *pItem)
{
	XVar var(XEAB::ID,0);
	if(!pItem->GetAttrib(var)) return XNULL;
	return m_css.GetName((XU16)var.iData);
}

XPCTSTR XDom::GetClass(XDomItem *pItem)
{
	XVar var(XEAB::CLASS,0);
	if(!pItem->GetAttrib(var)) return XNULL;
	return m_css.GetName((XU16)var.iData);
}

//DEL void XDom::DoCSS()
//DEL {
//DEL     if(m_nLoadStatus
//DEL }

void XDom::SetAttribs(XVarList &list,XU16Array&ids)
{
	if(ids.GetSize()>0)
	{
		XDomList items;
		m_doms.GetListByID(items,ids[0]);
		if(items.GetSize()>0)
		{
			for(XU32 i=0;i<items.GetSize();i++)
				items[i]->SetAttribs(list,ids,1,this);
			return;
		}

		else
		{
			//return;
			for(XU32 i=0;i<list.GetSize();i++)
			{

			}
			//int id=ids[
		}
	}

}

XBOOL XDom::CanAdd(XU16 ID)
{
	if(m_pItem==XNULL) return XFALSE;
	if(!(m_pItem->GetStyle()&XDomItem::DOM_NODE))
		return XFALSE;
	XU16* pChilds=XDomPhaseXML::GetTokenNChilds(m_pItem->GetID());
	if(pChilds!=XNULL)
	{
		int id=0;
		while(pChilds[id]!=0)
		{if(pChilds[id]==ID) return XFALSE;id++;}
	}
	pChilds=XDomPhaseXML::GetTokenChilds(m_pItem->GetID());
	if(pChilds==XNULL) return XTRUE;
	int id=0;
	while(pChilds[id]!=0)
	{
		if(pChilds[id]==ID) return XTRUE;
		id++;
	}
	return XFALSE;
}

//DEL XBOOL XDom::FinalImage(XIMAGEDATA *pImage)
//DEL {
//DEL 	return pImage->Final(this);
//DEL }

//DEL XBOOL XDom::TransVar(XVar &var, XU16 id)
//DEL {
//DEL 	switch(list[i].nID)
//DEL 				{
//DEL 				case XEAB::COLOR:
//DEL 					 switch(ids[0])
//DEL 					 {
//DEL 					 case XCSS::XI_ATEXT:
//DEL 						  m_pDraw->DCLINK=(XU32)list[i].iData;
//DEL 						  break;
//DEL 					 case XCSS::XI_AACTIVE:
//DEL 						  m_pDraw->DCACTIVE=(XU32)list[i].iData;break;
//DEL 					 //case XCSS::XI_ASELECT:
//DEL 					//	  m_pDraw->DCSELECTTEXT=(XU32)list[i].iData;break;
//DEL 					// case XCSS::XI_ABACK:
//DEL 					//	  m_pDraw->DCSELECTBACK=(XU32)list[i].iData;break;
//DEL 					 //case XCSS::XI_AABACK:
//DEL 					//	  break;
//DEL 					 }
//DEL 					 break;
//DEL 				}
//DEL
//DEL }

void XDom::GetText(XString16 &strTxt)
{
	XDomItem*pItem=GetActiveCard();
	if(pItem)
		pItem->GetAllText(strTxt);
}


void XDom::AppendDom(XDom *pDom,int s)
{
	//return;
	if(pDom->m_nStatus!=XHTTP_STATUS_OK)
		return;
	int nSize=m_cardList.GetSize()-1;
	if(nSize<0) return;
	XDomItem*pItem=pDom->GetActiveCard();
	if(pItem==XNULL) return;
	if(!m_css.IsEmpty())
	{
//	    XU32 t1=GetTickCount();
		GetActiveCard()->SetCSS(&m_css);
//		XU32 t2=GetTickCount()-t1;
		m_css.Clear();

	}
	//m_imageList.
	XDomItem*pRoot=m_cardList[nSize];
	//m_pItem=pRoot;
    XDomList*pList=pItem->GetChilds();
	XDomList*pDst=pRoot->GetChilds();
	if(pList&&pDst)
	{
		int i,head=XTRUE,nCount=pList->GetSize();
		if(s==XAP_TAIL) head=XFALSE;
		for(i=0;i<nCount;i++)
		{
			XDomItem*pItem=(*pList)[i];
			XU16 id=pItem->GetID();
			if(head)
			{
				pDst->InsertAt(i,pItem);
				if(s==XAP_DIV&&id==XDOM_DIV)
					head=XFALSE;
			}
			else
				pDst->Add(pItem);
		}
	}
	SetLayerout();
}
