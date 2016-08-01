// XConnectRes.cpp: implementation of the XConnectRes class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XConnectRes.h"
#include "XConnectFile.h"
#include "XClient.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XConnectRes::XConnectRes(XClient*pClient):XConnect(pClient)
{

}

XConnectRes::~XConnectRes()
{

}

int XConnectRes::Connect(XBOOL bRetry)
{
	GetResponseInfo()->Reset(XCHARSET_GB2312);
	//m_strSubPath.Empty();
	
	return XTRUE;
}

int XConnectRes::SendData(XBOOL bRetry)
{
	 XURL*p=&GetConnectInfo()->URL;
	 RESPONSEINFO*pInfo=GetResponseInfo();
	 //XU8Array data;
	 if(!XResource::LoadData(m_inData,p->m_strFile))
		 return CWAIT_ERROR;
		 //SetErrorCode(XHTTP_STATUS_NOT_FOUND);
	 else
	 {
		 pInfo->nLength=m_inData.GetSize();
		 m_nStatus=XPROC_CONTENT;
		 pInfo->nTotalSize=0;
		 int id=-1;
		 switch(m_inData[0])
		 {
		 case 'g':
		 case 'G':
			 id=XConnectFile::TYPE_GIF;break;
		 case 'F':
		 case 'f':
		 case 'c':
		 case 'C':
			 id=XConnectFile::TYPE_SWF;break;
		 }
		 if(id<0) 
			pInfo->type.SetType(XCNT::CANY,XCNT::TANY);
		 else
			 pInfo->type.SetType(XConnectFile::_fileTypes[id].nClass,XConnectFile::_fileTypes[id].nType,XCHARSET_GB2312);
		 Handle(XClient::STARTED);
	 }
	 return CWAIT_OK;
}

int XConnectRes::GetData()
{
	GetResponseInfo()->nTotalSize=
		GetResponseInfo()->nLength;
	return CWAIT_FINISHED;
}

void XConnectRes::Begin()
{

}
