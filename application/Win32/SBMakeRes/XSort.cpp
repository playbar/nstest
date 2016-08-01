// XSort.cpp: implementation of the XSortObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XSort.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XINT XSort::CompareString8(XPCTSTR str,XPCTSTR strValue,XU8 nMode)
{
	XString8 strKey((XPCTSTR)str,STRING_ATTACH);
	//XPCTSTR strValue=(XPCTSTR)strd;
	XINT rs=0;
	if(strValue==XNULL) rs=1;
	else
	{
		switch(nMode)
		{
		case XSS_FIRST:rs=strKey[0]-strValue[0];break;
		case XSS_NOCASE:rs=strKey.Compare(strValue,XTRUE);					 
			 break;
		default:
		case XSS_NORMAL:rs=strKey.Compare(strValue,XFALSE);
		}
	}
	if(rs>0) rs=1;
	else if(rs<0) rs=-1;
	strKey.EmptyNoFree();
	return rs;
}

XINT XSort::CompareString16(XPCWSTR str,XPCWSTR strValue,XU8 nMode)
{
	XString16 strKey((XPCWSTR)str,STRING_ATTACH);
	//XPCWSTR strValue=(XPCWSTR)strd;
	XINT rs=0;
	if(strValue==XNULL) rs=1;
	else
	{
		switch(nMode)
		{
		case XSS_FIRST:rs=((XPCWSTR)str)[0]-strValue[0];break;
		case XSS_NOCASE:rs=strKey.Compare(strValue,XTRUE);break;
		default:
		case XSS_NORMAL:rs=strKey.Compare(strValue,XFALSE);
		}
	}
	strKey.EmptyNoFree();
	if(rs>0) rs=1;
	else if(rs<0) rs=-1;
	return rs;
}

XINT XSort::Index(void*pKey, XINT &mi,XU8 nMode)
{
   XU32 nCount=GetCount();
   if(nCount<=0)
   {
	   mi=0;
	   return -1;
   }
   else 
   {
 	  int bi=0,ei=nCount-1;
	  mi=(bi+ei)>>1;
	  int nResult=Compare(pKey,GetData(mi),nMode);
	  //FORMATV(nResult);
	  while(ei-bi>1)
	  {	

		if(nResult==0) return mi;
		else if(nResult<0)
			ei=mi;
		else
			bi=mi;
		mi=(bi+ei)>>1;
		nResult=Compare(pKey,GetData(mi),nMode);
		//FORMATV(nResult);
	  }
	  switch(nResult)
	  {
	  case 0:return mi;
	  case 1:
		  {
			  nResult=Compare(pKey,GetData(ei),nMode);
			  //FORMATV(nResult);
			  switch(nResult)
			  {
			  case 0:mi=ei;return mi;
			  case 1:mi=ei+1;
				   return -1;
			  default:
				   mi=ei;
				   return -1;
			  }
		  }break;
	  default:
	  case -1:
		    return -1;
	  }
   }
}






/*#define FORMATV(VV) {if(VV>1) VV=1;else if(VV<-1) VV=-1;}




XSortObj::XSortObj()
{

}

XINT XSortObj::Index(XPCTSTR strKey, XINT &mi,XU8 nMode)
{
   XU32 nCount=GetCount();
   if(nCount<=0)
   {
	   mi=0;
	   return -1;
   }
   else 
   {
 	  int bi=0,ei=nCount-1;
	  mi=(bi+ei)>>1;
	  int nResult=Compare(strKey,GetData(mi),nMode);
	  FORMATV(nResult);
	  while(ei-bi>1)
	  {	

		if(nResult==0) return mi;
		else if(nResult<0)
			ei=mi;
		else
			bi=mi;
		mi=(bi+ei)>>1;
		nResult=Compare(strKey,GetData(mi),nMode);
		FORMATV(nResult);
	  }
	  switch(nResult)
	  {
	  case 0:return mi;
	  case 1:
		  {
			  nResult=Compare(strKey,GetData(ei),nMode);
			  FORMATV(nResult);
			  switch(nResult)
			  {
			  case 0:mi=ei;return mi;
			  case 1:mi=ei+1;
				   return -1;
			  default:
				   mi=ei;
				   return -1;
			  }
		  }break;
	  default:
	  case -1:
		    return -1;
	  }
   }
}

XINT XSortObj::IndexW(XPCWSTR strKey, XINT &mi,XU8 nMode)
{
   XU32 nCount=GetCount();
   if(nCount<=0)
   {
	   mi=0;
	   return -1;
   }
   else 
   {
 	  int bi=0,ei=nCount-1;
	  mi=(bi+ei)>>1;
	  int nResult=CompareW(strKey,GetWData(mi),nMode);
	  FORMATV(nResult);
	  while(ei-bi>1)
	  {	

		if(nResult==0) return mi;
		else if(nResult<0)
			ei=mi;
		else
			bi=mi;
		mi=(bi+ei)>>1;
		nResult=CompareW(strKey,GetWData(mi),nMode);
		FORMATV(nResult);
	  }
	  switch(nResult)
	  {
	  case 0:return mi;
	  case 1:
		  {
			  nResult=CompareW(strKey,GetWData(ei),nMode);
			  FORMATV(nResult);
			  switch(nResult)
			  {
			  case 0:mi=ei;return mi;
			  case 1:mi=ei+1;
				   return -1;
			  default:
				   mi=ei;
				   return -1;
			  }
		  }break;
	  default:
	  case -1:
		    return -1;
	  }
   }
}

XINT XSortObj::Index(XINT nData, XINT &mi)
{
   XU32 nCount=GetCount();
   if(nCount<=0)
   {
	   mi=0;
	   return -1;
   }
   else 
   {
 	  int bi=0,ei=nCount-1;
	  mi=(ei+bi)>>1;
	  int nResult=nData-GetIData(mi);
	  FORMATV(nResult);
	  while(ei-bi>1)
	  {	

		if(nResult==0) return mi;
		else if(nResult<0)
			ei=mi;
		else
			bi=mi;
		mi=(bi+ei)>>1;
		nResult=nData-GetIData(mi);
		FORMATV(nResult);
	  }
	  switch(nResult)
	  {
	  case 0:return mi;
	  case 1:
		  {
			  nResult=nData-GetIData(ei);
			  FORMATV(nResult);
			  switch(nResult)
			  {
			  case 0:mi=ei;return mi;
			  case 1:mi=ei+1;return -1;
			  default:
				   mi=ei;
				   return -1;
			  }
		  }break;
	  default:
	  case -1:return -1;
	  }
   }
}

XINT XSortObj::IndexF(float nData, XINT &mi)
{
   XU32 nCount=GetCount();
   if(nCount<=0)
   {
	   mi=0;
	   return -1;
   }
   else 
   {
 	  int bi=0,ei=nCount-1;
	  mi=(ei+bi)>>1;
	  float s=nData-GetFData(mi);
	  FORMATV(s);
	  int nResult=(int)s;
	  while(ei-bi>1)
	  {	

		if(nResult==0) return mi;
		else if(nResult<0)
			ei=mi;
		else
			bi=mi;
		mi=(bi+ei)>>1;
		s=nData-GetFData(mi);		
		FORMATV(s);
		nResult=(int)s;
	  }
	  switch(nResult)
	  {
	  case 0:return mi;
	  case 1:
		  {
			  s=nData-GetFData(ei);
			  FORMATV(s);
			  nResult=(int)s;
			  switch(nResult)
			  {
			  case 0:mi=ei;return mi;
			  case 1:mi=ei+1;return -1;
			  default:
				   mi=ei;
				   return -1;
			  }
		  }break;
	  default:
	  case -1:return -1;
	  }
   }
}*/

XBOOL XSortString8::SSetValue(XString8Array&list,XPCTSTR strKey,XPCTSTR strValue)
{
	XSortString8 sort(list,2);
	int mi=0;
	int id=sort.Index((void*)strKey,mi,XSS_NOCASE);
	if(id>=0)
	{
		if(list[(id<<1)+1]==strValue) return XFALSE;
		list[(id<<1)+1]=strValue;
		return XTRUE;
	}
	else
	{
		mi<<=1;
		list.InsertAt(mi,strKey);
		list.InsertAt(mi+1,strValue);
	}
	return XTRUE;
}
XBOOL XSortString8::SSetValue(XString8Array&list,XPCTSTR strKey)
{
	XSortString8 sort(list,1);
	int mi=0;
	int id=sort.Index((void*)strKey,mi,XSS_NOCASE);
	if(id>=0) return XFALSE;
	list.InsertAt(mi,strKey);
	return XTRUE;
}
XBOOL XSortString8::ESetValue(XString8Array&list,XPCTSTR strKey,XPCTSTR strValue)
{
	for(XU32 i=0;i<list.GetSize();i+=2)
	{
	   if(list[i].Compare(strKey,XTRUE)==0)
	   {
		   if(list[i+1]==strValue) return XFALSE;
		   list[i+1]=strValue;
		   return XTRUE;
	   }
	}
	list.Add(strKey);
	list.Add(strValue);
	return XTRUE;
}
XBOOL XSortString8::ESetValue(XString8Array&list,XPCTSTR strValue)
{
	for(XU32 i=0;i<list.GetSize();i++)
		if(list[i].Compare(strValue,XTRUE)==0) return XFALSE;
	list.Add(strValue);
	return XTRUE;
}

void XSortTable::Add(XU32 iKey, XU32 iValue)
{
	int mi;
	int id=Index((void*)iKey,mi);
	if(id>=0)
		m_tables[(id<<1)+1]=iValue;
	else
	{
		m_tables.Add(iKey);
		m_tables.Add(iValue);
		//return (m_tables.GetSize()>>1)-1;
	}
	//return id;
}

