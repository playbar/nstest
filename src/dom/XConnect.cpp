// XConnect.cpp: implementation of the XConnect class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XConnect.h"
#include "XClient.h"
#include "XFile.h"
//#include "openssl/md5.h"

#ifdef _WINEMU
#include "XMainWnd.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static XPCTSTR _strCacheCmd[]={"no-cache","no-store","max-age","max-stale","private"};

XPCTSTR XConnect::_strHttpVersion="HTTP/1.1";
XPCTSTR XConnect::_strKeepAlive[]={"keep-alive","Close"};

//int XConnectHttp::mnDataSize = 0;
//int XConnectHttp::mnConCount = 0;

//XU32 XConnectHttp::mTimeStart =XTime::GetTimeMSec();
//XU32 XConnectHttp::mTimeEnd = XTime::GetTimeMSec();

XConnect::XConnect(XClient*pClient)
{
	m_pClient=pClient;
	m_nStatus=XPROC_BEGIN;
	m_nProcStatus=XPROC_START;
	m_bFirst=true;
	m_nTime=0;
	m_nRetries=0;
	m_inPos=0;
	m_outPos=0;
	m_inData.SetSize(MAX_PACK,XFALSE);
}

XConnect::~XConnect()
{
}



CONNECTINFO* XConnect::GetConnectInfo(){return &m_pClient->m_con;}
RESPONSEINFO* XConnect::GetResponseInfo(){return &m_pClient->m_res;}
CLIENTINFO* XConnect::GetClientInfo(){return m_pClient->m_pInfo;}

void XConnect::SetErrorCode(XU32 nCode)
{
	m_pClient->m_res.nErrorCode=nCode;
}

//extern XClient* _sClient;

XU32 XConnect::CallBack()
{
	//LOGWHERE();
	
	CLIENTINFO*pInfo=GetClientInfo();
	XUINT nRetries=pInfo->nRetries;
	XU32  nTimeout=pInfo->nTimeout*1000;
	if(nTimeout<500)
		nTimeout=500;
	XU32  nTime=XTime::GetTimeMSec();
	XU32  nCmd=XClient::WAITED;
	XBOOL bRetry=XFALSE;
	//if(GetResponseInfo()->bNoData)
	//	int v=0;
	switch(m_nProcStatus)
	{
	case XPROC_START://Begin Connect
		Begin();
		m_nTime=nTime;
		m_nProcStatus=XPROC_CONNECT;
		m_nRetries=0;
		break;
	case XPROC_CONNECT:	   //Connectting...
		bRetry=nTime-m_nTime>nTimeout*5||m_nRetries==0;
		if(bRetry&&m_nRetries>nRetries)
		{	  //Timeout
			SetErrorCode(XERR_CANT_CONNECT);
			m_nProcStatus=XPROC_FINISHED;
			return nCmd;
		}
		else
		{
			if(bRetry)
			{
				m_nTime=nTime;
			}
			switch(Connect(bRetry))
			{//Conect OK
			case CWAIT_CONTINUE:
				m_nTime=nTime;
				return nCmd;//break;
			case CWAIT_OK:
				m_nProcStatus=XPROC_SENDDATA;
				nCmd=XClient::CONNECTED;
				m_nRetries=0;
				break;
			case CWAIT_TIMEOUT:
				if(bRetry)
					m_nRetries++;
				return nCmd;break;
			case CWAIT_ERROR:
				SetErrorCode(XERR_CANT_CONNECT);
				m_nProcStatus=XPROC_FINISHED;
				return nCmd;//break;
			}
		}
		break;
	case XPROC_SENDDATA://Send Data

		bRetry=nTime-m_nTime>nTimeout||m_nRetries==0;
		if(bRetry&&m_nRetries>nRetries)
		{
			SetErrorCode(XERR_SEND_ERROR);
			m_nProcStatus=XPROC_FINISHED;
			return nCmd;
		}
		else
		{
			if(bRetry)
			{
				m_nTime=nTime;
				//m_nRetries++;
			}
			switch(SendData(bRetry))
			{
			case CWAIT_CONTINUE:
				m_nTime=nTime;
				return nCmd;//break;
			case CWAIT_FINISHED:
			case CWAIT_OK:
				m_nProcStatus=XPROC_GETDATA;
				m_nTime=nTime;
				m_nRetries=0;
				if(this->m_pClient->IsPostOnly())
				{
					m_nProcStatus=XPROC_FINISHED;
					return nCmd;
				}
				break;
			case CWAIT_ERROR:
				SetErrorCode(XERR_SEND_ERROR);
				nCmd=XClient::FINISHED;
				//if(this->m_pClient==_sClient)
				//	int v=0;
				m_nProcStatus=XPROC_FINISHED;
				return nCmd;//break;
			case CWAIT_TIMEOUT:
				if(bRetry)
					m_nRetries++;
				return nCmd;//break;
			}
		}
		break;
	case XPROC_GETDATA:
		bRetry=nTime-m_nTime>nTimeout*5||m_nRetries==0;
		if(bRetry&&m_nRetries>nRetries)
		{
			if(GetResponseInfo()->bNoData)
				SetErrorCode(XERR_OK);
			else
				SetErrorCode(XERR_REV_ERROR);
			nCmd=XClient::FINISHED;
			//if(this->m_pClient==_sClient)
			//	int v=0;
			m_nProcStatus=XPROC_FINISHED;
			return nCmd;
		}
		else
		{
			if(bRetry)
				m_nTime=nTime;
			int ty=GetData();
			switch(ty)
			{
			case CWAIT_CONTINUE:
				m_nTime=nTime;
				return nCmd;//break;
			case CWAIT_FINISHED:
			case CWAIT_OK:
				if(m_inData.GetSize()==0)
				{
					if(m_nStatus!=XPROC_CONTENT)
						SetErrorCode(XERR_NO_RESPONSE);
					m_nProcStatus=XPROC_FINISHED;
					return nCmd;
				}
				else
				{
					m_nProcStatus=ty==CWAIT_OK?XPROC_PROCDATA:XPROC_PROCDATAEND;
					m_nTime=nTime;
					m_nRetries=0;
					m_inPos=0;
				}
				break;
			case CWAIT_ERROR:
				SetErrorCode(XERR_SEND_ERROR);
				nCmd=XClient::FINISHED;
				//if(this->m_pClient==_sClient)
				//	int v=0;
				m_nProcStatus=XPROC_FINISHED;
				return nCmd;//break;
			case CWAIT_TIMEOUT:
				if(GetResponseInfo()->bNoData)
				{
					m_nProcStatus=XPROC_FINISHED;
					nCmd=XClient::FINISHED;
				}
				if(bRetry)
					m_nRetries++;
				return nCmd;//break;
			}
		}break;
	case XPROC_PROCDATAEND:
	case XPROC_PROCDATA:
		{
			int nSize=m_inData.GetSize()-m_inPos;
			nCmd=ProcData(m_inData.GetData()+m_inPos,nSize);
			if(m_nStatus==XPROC_FINISHED)
				m_nProcStatus=XPROC_FINISHED;
			else if(nSize<=0)
			{
				//if(m_inData.GetSize()<MAX_PACK)
				m_inData.SetSize(0);
				if(this->GetResponseInfo()->nErrorCode!=XHTTP_STATUS_OK)
					m_nProcStatus=XPROC_FINISHED;
				else
					m_nProcStatus=m_nProcStatus==XPROC_PROCDATA?XPROC_GETDATA:XPROC_FINISHED;
			}
			else
			{
				m_inPos=m_inData.GetSize()-nSize;
			}break;
		}
	case XPROC_WAIT:
		break;
	case XPROC_FINISHED:
		if(m_pClient->IsAlive()&&m_pClient->GetErrorCode()==XHTTP_STATUS_OK)
		{
			//Modify for RPC Call
			nCmd=XClient::FINISHED;
			//if(this->m_pClient==_sClient)
			//	int v=0;
			m_nProcStatus=XPROC_WAIT;
		}
		else
		{
			DisConnect();
			nCmd=XClient::FINISHED;
			//if(this->m_pClient==_sClient)
			//	int v=0;
			m_nProcStatus=XPROC_DONE;
		}
		break;
	}
	return nCmd;
}


void XConnect::Handle(XU8 nHandle)
{
	m_pClient->Handle(nHandle);
}

void XConnect::DisConnect()
{
	//LOGWHERE();
    m_inData.RemoveAll(XTRUE);
	m_outStream.RemoveAll(XTRUE);
	m_nProcStatus=XPROC_START;
	m_nStatus=XPROC_BEGIN;
	m_inPos=m_outPos=0;
	m_bFirst=true;
	m_bChunkEnd=XFALSE;
}


int XConnect::ProcContent(XU8 *pData, XU32 nSize)
{
	RESPONSEINFO*pInfo=m_pClient->GetResponseInfo();
	XLock lock;
	if(!pInfo->recBuf.LockWrite(lock,0)) 
		return 0;
	int nSkip=0;
	if(m_bFirst&&nSize>=3)
	{
		bool bText=false;
		if(pInfo->type.m_nClass==XCNT::CTEXT)
			bText=true;
		else if(pInfo->type.m_nClass==XCNT::CAPPLICATION)
		{
			switch(pInfo->type.m_nType)
			{
			case XCNT::THTML:
			case XCNT::TVND_WAP_WML:
			case XCNT::TVND_WAP_WMLC:
			case XCNT::TVND_WAP_WMLSCRIPT:
			case XCNT::TVND_WAP_WMLSCRIPTC:
			case XCNT::TVND_WAP_WTLS_USER_CERTIFICATE:
			case XCNT::TVND_WAP_XHTML_XML:
			case XCNT::TX_HDML:
			case XCNT::TX_HDMLC:
			case XCNT::TX_TTML:
			case XCNT::TXHTML_XML:
			case XCNT::TXML:
			case XCNT::TX_JAVASCRIPT:
				bText=true;
			}
		}
	}

	m_bFirst=false;
	XU32 ns=XMIN(pInfo->recBuf.GetFreeSize(),nSize);

	if(pInfo->nEncode==XTCD_CHUNKED)
	{
		ns=XMIN(ns,pInfo->nLength-pInfo->nTotalSize);
	}
	if(ns>0)
	{
		pInfo->nTotalSize+=ns;//nSize;
		pInfo->recBuf.Write(pData,ns);//nSize);
		m_pClient->Catch(pData,ns);
	}
	lock.UnLock();
	//if(ns>0) Handle(XClient::RECIVE);
	if(pInfo->nLength>0&&pInfo->nTotalSize>=pInfo->nLength)
	{
		if(pInfo->nEncode==XTCD_CHUNKED)
		{
			m_bChunkEnd=XTRUE;
			m_nStatus=XPROC_CHUNKED;
		}
		else
			m_nStatus=XPROC_FINISHED;
	}
	return ns+nSkip;
}


int XConnect::ProcData(XU8 *pData, int& nSize)
{
	if(nSize<=0) 
		return XClient::WAITED;
	int nv = 0;
	int nCmd=XClient::WAITED;
	switch(m_nStatus)
	{
	case XPROC_FINISHED:
		nv=nSize;
		m_nProcStatus=XPROC_FINISHED;
		break;
	case XPROC_CHUNKED:
		nv=ProcChunked(pData,nSize);
		//if(m_nStatus==XPROC_FINISHED)
		//	 nCmd=XClient::FINISHED;
		break;
	case XPROC_CONTENT:
		nv=ProcContent(pData,nSize);
		if(nv)
		{
			nCmd=XClient::RECIVE;
		}
		break;
	case XPROC_HEADER:
		nv=ProcHeader(pData,nSize);
		if(m_nStatus!=XPROC_HEADER)
		{
			nCmd=XClient::STARTED;
			//if(this->GetResponseInfo()->bNoData)
			//	m_nStatus=XPROC_FINISHED;
		}
		break;
	case XPROC_BEGIN:
		nv=ProcBegin(pData,nSize);
		break;
	default:
		nv=nSize;
		break;
	}
	if(nv>0)
		nSize-=nv;
	return nCmd;
}

void XConnectHttp::Begin()
{
	//m_inString.Empty();
	m_inData.SetSize(MAX_PACK,XFALSE);
	CreateRequest();
	//m_bChunkEnd=XFALSE;
}

void XConnectHttp::DisConnect()
{
	//if ( mbConnect )
	//{
	//	mTimeEnd = XTime::GetTimeMSec();
	//	mnConCount--;
	//	mbConnect = false;
	//	if ( mnConCount < 0 )
	//	{
	//		mnConCount = 0;
	//	}
	//	else if ( mnConCount == 0 && ((mTimeEnd - mTimeStart) > 1000 ))
	//	{
	//		int speed  = mnDataSize / (mTimeEnd - mTimeStart);
	//		if( speed < 15  )
	//		{
	//			static XU32 timeCou = 0;
	//			if (  XTime::GetTimeMSec() - timeCou > 300000  )
	//			{
	//				timeCou = XTime::GetTimeMSec();
	//				XWindow*pWnd=XWindow::m_pMainWnd;
	//				if(pWnd)
	//					pWnd->Handle(XWM_LOWNETSPEED, 0, 0 );

	//			}
	//		}
	//		mnDataSize = 0;
	//	}

	//}
	m_socket.Close();
	XConnect::DisConnect();
	//m_inString.Empty();

}

int XConnectHttp::GetData()
{
	//int recTotal = 0;
	switch(m_socket.WaitForReply(0))
	{
	case XSocket::REVERROR:
	case XSocket::NOTCONNECTED:
		return CWAIT_ERROR;
	case XSocket::REVTIMEOUT:
		return CWAIT_TIMEOUT;
	case XSocket::REVOK:
		{
			int nv=m_socket.Receive((char*)m_inData.GetData(),MAX_PACK);
			//mnDataSize += nv;
			//LOGWHEREVAL(mnDataSize);
			if(nv==0)
				return CWAIT_FINISHED;
			else if(nv==XSOCKET_ERROR)
				return CWAIT_ERROR;
			else
			{
				//recTotal += nv;
#ifdef _WINEMU
				XWindow*pWnd=XWindow::m_pMainWnd;
				if(pWnd)
					pWnd->Handle(XCMD_UPDATEPROFILE,XCMD_UPDATE_NETRATE,nv);
#endif
				RESPONSEINFO*pInfo=GetResponseInfo();
				m_inData.SetSize(nv);
				if(pInfo->nLength<=pInfo->nTotalSize && pInfo->nLength)
				 {
					 RESPONSEINFO*pInfo=m_pClient->GetResponseInfo();
					 if(pInfo->nEncode==XTCD_CHUNKED)
						 return CWAIT_OK;
					 //mnConCount--;
					 return CWAIT_FINISHED;
				 }
				else //if( !pInfo->nLength || recTotal == MAX_PACK || recTotal + pInfo->nTotalSize >= pInfo->nLength )
				 {
					 return CWAIT_OK;
				 }
				//else{
				//	goto startWait;
				//}
			}
		}break;
	}
	return CWAIT_TIMEOUT;
}


int XConnectHttp::SendData(XBOOL bRetry)
{
	if(!m_socket.IsValid())
		return CWAIT_ERROR;
	XUINT nIndex=0;
	XUINT nSize=m_outStream.GetSize()-m_outPos;
	if(nSize<=0)
		return CWAIT_OK;
	//if(nSize>BLOCKSIZE)
	//	nSize=BLOCKSIZE;
	void *pData = (char*)m_outStream.GetData()+m_outPos;
	int nv=m_socket.Send( pData,nSize);
	if(nv==0)
	{
		return CWAIT_TIMEOUT;
	}
	else if(nv==XSOCKET_ERROR)
	{
		return CWAIT_ERROR;
	}
	else m_outPos+=nv;
	return CWAIT_OK;
	return XTRUE;
}

int XConnectHttp::Connect(XBOOL bRetry)
{
	CONNECTINFO*pInfo=GetConnectInfo();
	if(bRetry)
	{
		m_socket.Close();

		switch(m_socket.ConnectTCP(pInfo->URL.m_strHost,pInfo->URL.m_uPort))
		{
		case XSocket::XC_OK:
			//mnConCount++;
			//mTimeStart = XTime::GetTimeMSec();
			////LOGWHEREVAL(mTimeStart);
			//if ( mTimeStart - mTimeEnd  > 10000 )
			//{
			//	mnDataSize = 0;
			//	mnConCount = 0;
			//}
			//mbConnect = true;

			return CWAIT_OK;
		case XSocket::XC_WAIT:
			return CWAIT_TIMEOUT;
		case XSocket::XC_ERROR:
			return CWAIT_ERROR;
		}
	}
	else switch(m_socket.WaitForReply(0))
	{
		default:
			return CWAIT_ERROR;
		case XSocket::CONNECTOK:
			//mnConCount++;
			//mTimeStart = XTime::GetTimeMSec();
			////LOGWHEREVAL(mTimeStart);
			//if ( mTimeStart - mTimeEnd  > 10000 )
			//{
			//	mnDataSize = 0;
			//	mnConCount = 0;
			//}
			//mbConnect = true;

			return CWAIT_OK;
		case XSocket::CONNECTTIMEOUT:
			return CWAIT_TIMEOUT;
	}
	return CWAIT_TIMEOUT;
}


// sjj
#ifdef _USERSSL_//__ANDROID__
XConnectHttp::XConnectHttp(XClient *pClient, XBOOL bUseSSL):m_socket(bUseSSL), XConnect(pClient)
{

}
#else
XConnectHttp::XConnectHttp(XClient *pClient):XConnect(pClient)
{

}

#endif

int XConnectHttp::ProcBegin(XU8 *pData, XU32 nSize)
{
	for(XUINT i=0;i<nSize;i++)
	{
		switch(pData[i])
		{
		case XCR:break;
		case XCT:
			if(m_inString.GetLength()==0) break;
			else
			{
				XString8Array list;
				list.SplitString(m_inString);
				m_inString.Empty();
				if(list.GetSize()<2)
				{
					m_nStatus=XPROC_FINISHED;
					SetErrorCode(XERR_BAD_PROTO);
					return -1;
				}
				RESPONSEINFO*pInfo=GetResponseInfo();
				pInfo->strVersion=list[0];
				pInfo->nErrorCode=list[1].ToInt();
				m_nStatus=XPROC_HEADER;
				return i+1;
			}
			break;
		default:
			m_inString+=(XTCHAR)pData[i];
			break;
		}
	}
	return nSize;
}


int XConnectHttp::ProcChunked(XU8 *pData, XU32 nSize)
{
	//m_nStatus=XPROC_CONTENT;
	for(XUINT i=0;i<nSize;i++)
	{
		switch(pData[i])
		{
		case XCR:break;
		case XCT:
			if(!m_inString.IsEmpty())
			{
				int nv=m_inString.ToInt(16);
				m_inString.Empty();
				GetResponseInfo()->nLength+=nv;
				m_nStatus=nv>0?XPROC_CONTENT:XPROC_FINISHED;
				return i+1;
			}
			else
			{
//#if (__CORE_VERSION__>=0x02073000)
				if(m_bChunkEnd)
					m_bChunkEnd=XFALSE;
				else
					m_nStatus=XPROC_FINISHED;
//#else
//				m_nStatus=XPROC_FINISHED;
//#endif
			}
			break;
		default:
			m_inString+=(XTCHAR)pData[i];
			break;
		}
	}
	return nSize;

}

void XConnectHttp::ProcFinished()
{
	LOGWHERE();
	if(!GetClientInfo()->bKeepAlive)
		DisConnect();
}

int XConnectHttp::ProcHeader(XU8 *pData, XU32 nSize)
{
	//LOGWHERE();
	for(XUINT i=0;i<nSize;i++)
	{
		switch(pData[i])
		{
		case XCR:
			break;
		case XCT:
			if(m_inString.GetLength()<=0)
			{
				m_nStatus=GetResponseInfo()->nEncode==XTCD_ID?XPROC_CONTENT:XPROC_CHUNKED;
				m_pClient->StartCatch();
				return i+1;
			}
			else
			{
				RESPONSEINFO*pInfo=GetResponseInfo();
				XString8Array list;
				XEnumHeader hd;
				int id=m_inString.Find(XCOLON);
		
				if(id>0)
				{
					XString8 strKey;
					m_inString.Left(strKey,id);
					XString8 str1;
					m_inString.Mid(str1,id+1);
					strKey.TrimLeft();
					strKey.TrimRight();
					str1.TrimLeft();
					str1.TrimRight();
					
					int id=hd.Index(strKey,XSS_NOCASE);
					switch(id)
					{
					case XEnumHeader::CONNECTION:
						break;
					case XEnumHeader::USER_AGENT:
					case XEnumHeader::MOBILE_TYPE:
						if(!str1.IsEmpty())
						{
							GetClientInfo()->strUserAgent=str1;
						}
						break;
					case XEnumHeader::ETAG:
						pInfo->strETag=str1;
						break;
					case XEnumHeader::X_FILTER_URL:
						pInfo->strFilterURL=str1;
						break;
					case XEnumHeader::CONFIG_URL:
						pInfo->strConfigURL=str1;
						break;
					case XEnumHeader::X_POWERED_BY:
						pInfo->strPower=str1;
						break;
					case XEnumHeader::SERVER:
						pInfo->strServer=str1;
						break;
					case XEnumHeader::ACCEPT_RANGES:
						pInfo->strAcceptRanges=str1;
						break;
					case XEnumHeader::CLIENT_ID:
						pInfo->strClientID=str1;
						break;
					case XEnumHeader::TRANSFER_ENCODING:
						pInfo->nEncode=XEAB().Index(str1)==XEAB::CHUNKED?XTCD_CHUNKED:XTCD_ID;
						break;
					case XEnumHeader::SHOWBAR:
						pInfo->nShowBar=str1.GetFirst();
						break;
					case XEnumHeader::SHOWAD:
						pInfo->nShowAD=str1.GetFirst();
						break;
					case XEnumHeader::ONSALE:
						pInfo->nOnSale = str1.GetFirst();
						break;
					case XEnumHeader::GTYPE:
						pInfo->nGType = str1.GetFirst();
						break;

					case XEnumHeader::STYPE:
						pInfo->nStype = str1.GetFirst();
						break;

					case XEnumHeader::APPSTATUS:
						pInfo->nAppStatus=str1.GetFirst();
						break;
					case XEnumHeader::NEWAPP:
						pInfo->nNewApp = str1;
						break;
					case XEnumHeader::APPDESC:
						pInfo->mStrAppDesc = str1;
						break;
						// zhengjl 2012.03
					case XEnumHeader::OUTLOGIN:
						//LOGE("%s, %d-------->,%d", __FUNCTION__,__LINE__,str1.GetFirst());

						pInfo->nOutLogin = str1.GetFirst();
						break;

					case XEnumHeader::LOCATION:
						pInfo->strLocation=str1;
						break;
					case XEnumHeader::SET_COOKIE:
						{
							XSortString8::ESetValue(pInfo->strCookies,str1);
						}
						break;
					case XEnumHeader::CONTENT_LENGTH:
						pInfo->nLength=str1.ToInt();
						if(pInfo->nLength==0)
							pInfo->bNoData=true;
						break;
					case XEnumHeader::CONTENT_TYPE:
						pInfo->type.SetType(str1);
						break;
					case XEnumHeader::LAST_MODIFIED:
					case XEnumHeader::DATE:
						{
							XTime t;
							if(t.SetTime(str1)&&t>pInfo->time)
								pInfo->time=t;
						}
						break;
					case XEnumHeader::PRAGMA:
					case XEnumHeader::CACHE_CONTROL:
						ProcCache(pInfo,str1);
						break;
					case XEnumHeader::EXPIRES:
						if(pInfo->nCacheMode<2)
						{
							pInfo->nCacheMode=2;
							XTime t;
							if(t.SetTime(str1))
							{
								long tt=XMAX(0,t-pInfo->time);
								pInfo->nMaxAge=tt;
							}
						}
						break;

					default:
						pInfo->strResHeaders.Add(m_inString);
						break;

					}
				}
				m_inString.Empty();
			}
			break;
		default:
			m_inString+=(XTCHAR)pData[i];
			break;
		}
	}
	return nSize;

}

void XConnectHttp::SetHeader(XPCTSTR strKey, XPCTSTR strValue, XU8 q)
{
	if(strKey!=XNULL)
	{
		if(m_strHeaders.Find(strKey,XTRUE)>=0) 
			return;
		m_strHeaders.Add(strKey);
	}
	XString8 str;
	if(strKey!=NULL)
	{
		str=strKey;
		str.MakeFirstUpper();
		str+=XCOLON;
		str+=XSPACE;
		str+=strValue;
		if(q>0)
		{
			str+=";q=0.";
			str+=(XINT)q;
		}
	}
	str+=XCRLF;
	m_outStream.WriteString(str);
	return;
}

void XConnectHttp::CreateRequest()
{
	m_outPos=0;
	m_inString.Empty();
	m_outStream.Reset();
	m_inData.RemoveAll(false);
	m_strHeaders.RemoveAll();
	CreateReqHeader();
	CreateClientHeaders();
	CreateConnectHeaders();
	m_strHeaders.RemoveAll();
	m_bChunkEnd=XFALSE;
}

void XConnectHttp::CreateReqHeader()
{
	CONNECTINFO*pInfo=GetConnectInfo();
	XString8 strTmp=XEM::Key(GetConnectInfo()->nMethod);
	strTmp+=XSPACE;
	strTmp+=pInfo->URL.m_strPath;
	strTmp+=pInfo->URL.m_strFile;
	strTmp+=XSPACE;
	strTmp+=_strHttpVersion;
	strTmp+=XCRLF;
	m_outStream.WriteString(strTmp);
}

void XConnectHttp::CreateClientHeaders()
{
	XUINT i;
	CLIENTINFO*pInfo=GetClientInfo();
	CONNECTINFO*pCon=GetConnectInfo();
	XEnumHeader hd;
	SetHeader(hd[hd.USER_AGENT],pInfo->strUserAgent);

	if(!pInfo->strMAC.IsEmpty())
		SetHeader("X-MAC",pInfo->strMAC);

	if ( !pInfo->strEip.IsEmpty() )
	{
		SetHeader( "EIP", pInfo->strEip  );
	}
    
    if ( !pInfo->strSysVer.IsEmpty() )
	{
		SetHeader( "SYSVERS", pInfo->strSysVer  );
	}
    
    SetHeader("client_id", pInfo->strClientID );

	XString8 strTmp;
	strTmp += pInfo->strUserAgent;
	strTmp += pInfo->strMAC;
	strTmp += pInfo->strChannelID;
	strTmp += pInfo->strMobileType;
	strTmp += pInfo->strClientID;

	//if ( ! pInfo->strXToken.IsEmpty())
	{
		SetHeader( "xtoken", pInfo->strXToken );
		strTmp += pInfo->strXToken;
	}

	strTmp += pInfo->strEip;
	strTmp += pInfo->strIDFV;
	strTmp += ">?Bw@4^8]<";
	 MD5 md5;
	 XU8 md5Buf[16];
	 char buf[33] = {'\0'};
	 char tmp[3] = {'\0'};
	 md5.MakeMD5((char*)strTmp.GetData(), strTmp.GetLength(),(char*)md5Buf);
	 pInfo->strVCODE = "";
	 for(i=0;i<16;i++)
	 {
		 XU8 c=md5Buf[i];
		 XU8 hi=c>>4;
		 XU8 lo=c&0xf;
		 if(hi>=10)
			  pInfo->strVCODE+=(char)('a'+hi-10);
		 else
			  pInfo->strVCODE+=(char)('0'+hi);
		 if(lo>=10)
			  pInfo->strVCODE+=(char)('a'+lo-10);
		 else
			  pInfo->strVCODE+=(char)('0'+lo);
	 }
	if ( !pInfo->strVCODE.IsEmpty() )
	{
		SetHeader( "VCODE", pInfo->strVCODE );
	}
//    if ( ! strTmp.IsEmpty() ) {
//         SetHeader( "VCODE_ALL", strTmp );
//    }
   
	
	

	//SetHeader("Accept-Encoding","gzip, deflate");
	//SetHeader("ACB","ACC");
	//XString8 strKey=XEH()[XEH::MOBILE_TYPE];
	//strKey.MakeUpper();
	//SetHeader(strKey,pInfo->strUserAgent);
	SetHeader(hd[hd.HOST],pCon->URL.m_strHost);
	//SetHeader(hd[hd.CONNECTION],KEEP
	for( i=0;i<pInfo->strAccepts.GetSize();i++)
		SetHeader(hd[hd.ACCEPT],pInfo->strAccepts[i]);
	for(i=0;i<pInfo->strHeaders.GetSize();i+=2)
		SetHeader(pInfo->strHeaders[i],pInfo->strHeaders[i+1]);
	if(pInfo->bKeepAlive)
		SetHeader(hd[hd.CONNECTION],_strKeepAlive[pInfo->bKeepAlive?0:1]);
	if(!pInfo->strProfile.IsEmpty())
		SetHeader(hd[hd.PROFILE],pInfo->strProfile);
	XEnumLan hl;
	for(i=0;i<pInfo->nLanguages.GetSize();i++)
	{
		XU8 nIndex=pInfo->nLanguages[i];
		SetHeader(hd[hd.ACCEPT_LANGUAGE],hl[nIndex]);
	}
	XEnumCharset ec;
	for(i=0;i<pInfo->nCharsets.GetSize();i+=2)
	{
		XU8 nIndex=pInfo->nCharsets[i];
		SetHeader(hd[hd.ACCEPT_CHARSET],
			ec.Key(nIndex),pInfo->nCharsets[i+1]);
	}
}

void XConnectHttp::CreateConnectHeaders()
{
	CONNECTINFO*pCon=GetConnectInfo();
	for(XU32 i=0;i<pCon->strHeaders.GetSize();i+=2)
	{
		SetHeader(pCon->strHeaders[i],pCon->strHeaders[i+1]);
	}
	XEnumHeader hd;
	if(!pCon->strRefer.IsEmpty())
		SetHeader(hd[hd.REFERER],pCon->strRefer);

	if((pCon->outData.GetSize()>0&&!pCon->strContentType.IsEmpty())||pCon->nMethod==XEM::POST)
	{
		XString8 str;
		str.FromInt(pCon->outData.GetSize());
		SetHeader(hd[hd.CONTENT_LENGTH],str);
		if(!pCon->strContentType.IsEmpty())
			SetHeader(hd[hd.CONTENT_TYPE],pCon->strContentType);
		else
			SetHeader(hd[hd.CONTENT_TYPE],XContentType::_strTypeList[XContentType::TX_WWW_FORM_URLENCODED]);

	}
	SetHeader(NULL,NULL);
	m_outStream.WriteData(pCon->outData,pCon->outData.GetSize());
	return;

}

void XConnectHttp::ProcCache(RESPONSEINFO *p, XString8 &str)
{
	if(p->nCacheMode>1) return;
	XString8Array list;
	list.SplitString(str,',');
	for(XU32 i=0;i<list.GetSize();i++)
	{
		int id=list[i].Find('=');
		XString8 strKey,strValue;
		if(id>0)
		{
			list[i].Left(strKey,id);
			list[i].Mid(strValue,id+1);
		}
		else strKey=list[i];
		id=strKey.IndexOf(_strCacheCmd,sizeof(_strCacheCmd)/sizeof(XPCTSTR));
		switch(id)
		{
			//case 4:
		case 0:
		case 1:
			p->nCacheMode=1;
			p->nMaxAge=0;break;
		case 2:
		case 3:
			p->nCacheMode=1;
			p->nMaxAge=strValue.ToInt();
			break;
		}
	}
}



