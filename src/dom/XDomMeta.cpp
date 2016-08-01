// XDomMeta.cpp: implementation of the XDomMeta class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDomMeta.h"
#include "XEnumData.h"
#include "XDom.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XU32 XDomMeta::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	 case XDO_EVENT_ONLOAD:
		  if(pData1)
		  {
			  XString8 strCmd;
			  if(XDomItem::GetAttrib(XEAB::HTTP_EQUIV,strCmd))
			  {
				strCmd.MakeLower();
				if(strCmd=="refresh")
				{
					XString8 strURL;
					if(XDomItem::GetAttrib(XEAB::SRC,strURL)&&!strURL.IsEmpty())
					{
						XEVENTDATA*pData=(XEVENTDATA*)pData1;
						//XU32 s=XRCONNECT;
						pData->bAuto=XTRUE;
						pData->SetURL(strURL);//URL.SetURL(strURL);
						pData->nMethod=XEM::GET;
						return XRCONNECT;
					}
				}
			  }
		  }
		  break;
	 case XDO_SET_FINAL:
		  return OnLoad((XEVENTDATA*)pData2);
	}
	return XDomLeaf::Handle(nOpera,pData1,pData2);
}

XU32 XDomMeta::OnLoad(XEVENTDATA *pData)
{
	XPCTSTR strHeader=FindAttrib(XEAB::HTTP_EQUIV,(XPCTSTR)XNULL);
	XPCTSTR strValue=FindAttrib(XEAB::CONTENT,"");
	XPCTSTR strName=FindAttrib(XEAB::NAME,(XPCTSTR)XNULL);
	if(strHeader!=XNULL)
	{
		/*if(XString8::Compare(strHeader,"refresh",0,XTRUE)==0)
		{
			//s|=XRCONNECT;
			XString8 strURL;
			if(XDomItem::GetAttrib(XEAB::SRC,strURL)&&!strURL.IsEmpty())
			{
				pData->bAuto=XTRUE;
				pData->SetURL(strURL);//URL.SetURL(strURL);
				pData->nMethod=XEM::GET;
				return XRCONNECT;
			}
		}*/
		/*int id=XEH().Index(strHeader);
		if(id==XEH::CONTENT_TYPE)
		{
			XString8 str(strValue,STRING_ATTACH);
			int is=str.Find(';');
			str.EmptyNoFree();
			if(is>0)
			{
				XString8Array list;
				XString8 cnt(strValue+is+1,STRING_ATTACH);
				list.SplitString(cnt,'=');
				cnt.EmptyNoFree();
				if(list.GetSize()>=2)
				{
					id=XEAB().Index(list[0]);
					if(id==XEAB::CHARSET)
					{
						id=XEC::Index(list[1]);
						if(id>=0)
							pData->pDom->m_nCharset=XEC::Index(list[1]);
					}
				}
			}
		}*/
		XSortString8::ESetValue(pData->strHeaders,strHeader,strValue);
		return XRSETHEADER;
	}
	else if(strName!=XNULL)
	{
		XSortString8::SSetValue(pData->strValues,strName,strValue);
		return XRSETVAR;
	}
	return XRNONE;
}

XDomItem* XDomItem::FindByAttrib(XVar &var, XU32 &nIndex, XU32 nID)
{
   if(nID==0||m_nID==nID)
   {
	 XVar v(var.nID,0);
	 if(GetAttrib(v)&&v==var) return this;
     nIndex++;
   }
   XU32 nCount=Handle(XDO_GET_CHILDCOUNT,0,0);
   for(XU32 i=0;i<nCount;i++)
   {
	  XDomItem*p=(XDomItem*)Handle(XDO_GET_CHILDBYINDEX,i,0);
	  if(p==XNULL) continue;
	  p=p->FindByAttrib(var,nIndex,nID);
	  if(p!=XNULL) return p;
   }
   return XNULL;
}

XDomItem* XDomItem::FindByIndex(XU32 ix, XU32 &nIndex, XU32 nID)
{
   if(nID==0||m_nID==nID)
   {
	 if(nIndex==ix) 
		 return this;
     nIndex++;
   }
   XU32 nCount=Handle(XDO_GET_CHILDCOUNT,0,0);
   for(XU32 i=0;i<nCount;i++)
   {
	  XDomItem*p=(XDomItem*)Handle(XDO_GET_CHILDBYINDEX,i,0);
	  if(p==XNULL) continue;
	  p=p->FindByIndex(ix,nIndex,nID);
	  if(p!=XNULL) return p;
   }
   return XNULL;
}

XU32 XDomItem::FindByItem(XDomItem*pi,XU32&nIndex)
{
	if(pi==this) 
		return nIndex;
	nIndex++;
	XU32 nCount=Handle(XDO_GET_CHILDCOUNT,0,0);
    for(XU32 i=0;i<nCount;i++)
    {
	  XDomItem*p=(XDomItem*)Handle(XDO_GET_CHILDBYINDEX,i,0);
	  if(p==XNULL) continue;
	  XU32 s=p->FindByItem(pi,nIndex);
	  if(s!=-1)
		  return s;
    }
	return -1;
}

XBOOL _XIMAGEDATA::Final(XDom*pDom,XU8 bUpdate)
{
		if(!bUpdate)
		{
			nStatus=XL_FAILED;
			if(nType==XIT_IMAGE)
			{
				if(!image.Final()||
					image.IsEmpty()) return XFALSE;
			}
			else if(data.GetSize()<=0) return XFALSE;
		}
		nStatus=XL_OK;
		for(XU32 i=0;i<list.GetSize();i++)
			list[i]->Handle(XDO_UPDATE_IMAGE,(XU32)pDom,0);
		return XTRUE;
}