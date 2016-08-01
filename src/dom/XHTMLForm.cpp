// XHTMLForm.cpp: implementation of the XHTMLForm class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XHTMLForm.h"
#include "XEnumData.h"
#include "XContentType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XSHA1::XSHA1()
{
	i=0;
	g=8;
}

XSHA1::~XSHA1()
{

}

void XSHA1::Final(const char*pData,int len,void*md)
{	
	//Max length 512
	if(len>512)
		len=512;
	int nl=(len*g)>>5;
	//int*output=new int[nl];
	int output[512];
	memset(output,0,512*sizeof(int));
	f((char*)pData,len,output);
	int key[5];
	b(output,nl,len*g,key);
	h(key,5,(char*)md);
	//delete output;
	//h(b(f(j),len*g),md);
}


void XSHA1::b(int*A,int length,int r,int*output)
{
	A[r>>5]|=128<<(24-r%32);
	int ml=(((r+64)>>9)<<4)+15;
	A[ml]=r;
	if(length<ml+1) length=ml+1;
	//var B=Array(80);
	int B[80];
	int z=1732584193;
	int y=-271733879;
	int v=-1732584194;
	int u=271733878;
	int s=-1009589776;
	for(int o=0;o<length;o+=16)
	{
		int q=z;
		int p=y;
		int n=v;
		int m=u;
		int k=s;
		for(int l=0;l<80;l++)
		{
			if(l<16)
			{
				B[l]=A[o+l];
			}
			else
			{
				B[l]=d(B[l-3]^B[l-8]^B[l-14]^B[l-16],1);
			}
			int C=e(e(d(z,5),a(l,y,v,u)),
				  e(e(s,B[l]),c(l)));
			s=u;
			u=v;
			v=d(y,30);
			y=z;
			z=C;
		}
		z=e(z,q);y=e(y,p);v=e(v,n);u=e(u,m);s=e(s,k);
	}
	output[0]=z;
	output[1]=y;
	output[2]=v;
	output[3]=u;
	output[4]=s;
	//return Array(z,y,v,u,s)
}
int XSHA1::a(int k,int j,int m,int l)
{
	if(k<20)
	{
		return(j&m)|((~j)&l);
	}
	if(k<40)
	{
		return j^m^l;
	}
	if(k<60)
	{
		return(j&m)|(j&l)|(m&l);
	}
	return j^m^l;
}


XU32 XHTMLForm::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{	 
	case XDO_LAYEROUT_CELLS:
	case XDO_LAYEROUT_CELL:
		{
			if(!IsVisiable()) return 0;
		   DRAWCONTEXT*pDraw=(DRAWCONTEXT*)pData1;
		   CELLDATA*pData=(CELLDATA*)pData2;
		   XU8 bCells=nOpera==XDO_LAYEROUT_CELLS;
		   //if(!pData->bNewLine)
			//   NewRow(0,pDraw,pData,pDraw->IND);
		   XDomNode::Handle(nOpera,pData1,pData2);
		   //if(!pData->bNewLine)///Return)
			//   NewRow(0,pDraw,pData,pDraw->IND);
		}return 0;
	case XDO_RESETINPUT:
		 return XDomNode::Handle(XDO_SETINPUT,pData1,pData2);
	case XDO_RESETRADIO:
		 return XDomNode::Handle(XDO_SETRADIO,pData1,pData2);
	case XDO_SUBMIT:
		 return DoSubmit((XEVENTDATA*)pData2);
/*	case XDO_ADD_ATTR:
	case XDO_SET_ATTR:
		{
			XVar*p=(XVar*)pData1;
			switch(p->nID)
			{
			case XEAB::METHOD:
				if(p->nType==VSTRING)
				  {
					  XString8 str(p->strData,STRING_ATTACH);
					  p->strData=XNULL;
					  p->nType=VINT;
					  *p=XEM::Index(str);
				  }
				break;
			}
		};*/
	}
	return XDomNode::Handle(nOpera,pData1,pData2);
}

XU32 XHTMLForm::DoSubmit(XEVENTDATA *pData2)
{
    XU32 s=0;
	XPCTSTR strURL=FindAttrib(XEAB::ACTION,(XPCTSTR)XNULL);
	if(strURL==XNULL) return s;
	s|=XRCONNECT;
	//pData2->tempURL.SetURL(strURL);
	XU8 m=FindAttrib(XEAB::METHOD,XEM::GET);
	if(m==XEM::POST)
	{
		XEH h;
		XContentType type(XCNT::CAPPLICATION,XCNT::TX_WWW_FORM_URLENCODED);
		//XU8 c=FindAttrib(XEA::ACCEPT_CHARSET,XCHARSET_UTF8);
		XSortString8::ESetValue(pData2->strHeaders,
								h[XEH::CONTENT_TYPE],
								type.GetTypeString());
	}
	pData2->SetURL(strURL,m);
	//pData->SetURL(strURL,FindAttrib(XEAB::METHOD,XEM::GET));
	s|=XDomNode::Handle(XDO_CONNECT,(XU32)pData2,2);
	XPCTSTR strInterface=FindAttrib(XEAB::XINTERFACE,(XPCTSTR)XNULL);
	if(strInterface)
	{
		if(strcmp(strInterface,"sinaweibo")==0)
		{
			MakeWeiboInterface(pData2);
		}
	}
	return s;
}
void XHTMLForm::MakeNonce(XString8&strNonce,int len)
{
	const char strTmp[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	for(int i=0;i<len;i++)
	{
		int iRand=XGlobal::RandInt()%sizeof(strTmp);
		strNonce+=strTmp[iRand];
		//str+=x.charAt(Math.ceil(Math.random()*1000000)%x.length)}
	}
}

void XHTMLForm::MakeWeiboInterface(XEVENTDATA*pData)
{
	//XString8 strPassword;
	//XSortString8 sort(pData->strDatas,2);
	//int mi=0;
	//int id=sort.Index((void*)"password",mi,XSS_NOCASE);
	//if(id<0) return;
	int id=-1;
	for(int i=0;i<pData->strDatas.GetSize();i+=2)
	{
		if(pData->strDatas[i].Compare("password",XTRUE)==0)
		{
			id=i;
			break;
		}
	}
	if(id<0) return;
	id++;
	XString8 strNonce,strTime;
	MakeNonce(strNonce,6);
	XTime time;
	time.SetCurrentTime();
	//SortString8::SSetValue(pData->strDatas,"nonce",strNonce);
	pData->strDatas.Add("nonce");
	pData->strDatas.Add(strNonce);
	strTime.FromInt(time.GetTime());
	//XSortString8::SSetValue(pData->strDatas,"servertime",strTime);
	pData->strDatas.Add("servertime");
	pData->strDatas.Add(strTime);
	XSHA1 shaEnc;
	char sha[41];
	XString8&strPassword=pData->strDatas[id];
	shaEnc.Final(strPassword,strPassword.GetLength(),sha);
	shaEnc.Final(sha,40,sha);
	XString8 strCnt=sha;
	strCnt+=strTime;
	strCnt+=strNonce;
	shaEnc.Final(strCnt,strCnt.GetLength(),sha);
	pData->strDatas[id]=sha;
	//XSortString8::SSetValue(pData->strDatas,"password",sha);
	/*{
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
	return XTRUE;*/
}
