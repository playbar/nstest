// XClient.cpp: implementation of the XClient class.
//menuwebgame
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XClient.h"
#include "XConnectFile.h"
#include "XConnectRes.h"
#include "XConnectTCP.h"
#include "XClientApply.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XClient::XClient(XClientApply*pApply)
{
	m_pApply=pApply;
	m_catch.m_pClient=this;
	//Modify FILTER
	//m_pReferFilter=NULL;
	//m_pCurFilter=NULL;
	//m_pWnd=XNULL;
	m_nCallID=0;
	m_nNotifyID=0;
	m_nCmd=WAITED;
	m_res.Reset(XCHARSET_GB2312);

	m_con.Reset(XTRUE);
	m_pConnect=XNULL;
	m_pConnectOld=XNULL;
	m_pWnd=XNULL;
	m_nUserData=0;
	m_nProtoType=XAPP_FILE;
	memset(m_iStream,0,ICSIZE);
	m_bKeepAlive=false;
	m_bNeedCache=true;
	m_bPostOnly=false;
}

XClient::~XClient()
{
	Release();
}

XU32 XClient::Get_CallID()
{
	return m_nCallID;
}

void XClient::SetCallID(XU32 nID)
{
	m_nCallID=nID;
}

void XClient::SetWindow(XWindow*pWnd,XU32 nID,XU32 nNotifyID)
{
	m_nNotifyID=nNotifyID;
	m_pWnd=pWnd;
	m_nCallID=nID;
}

XBOOL XClient::Release()
{
	m_catch.EndCatch(m_res.nErrorCode);
	m_con.Reset(XFALSE);
	m_hCancel.ResetEvent();
	m_hConnect.ResetEvent();
	m_hPosted.ResetEvent();
	m_catch.Release();
	m_hConnect.Release();
	m_hUsing.Release();
	m_hCancel.Release();
	m_hPosted.Release();
	m_res.Reset(XCHARSET_GB2312);
	m_con.Reset(XTRUE);
#ifdef _SYMBIAN
#else
	if(m_pConnect!=XNULL&&m_pConnect!=&m_catch)
		delete m_pConnect;
	if(m_pConnectOld!=XNULL&&m_pConnectOld!=&m_catch)
		delete m_pConnectOld;
#endif
	m_pConnectOld=XNULL;
	m_pConnect=XNULL;

	return XTRUE;
}
//extern XClient* _sClient;
XBOOL XClient::Handle(XU32 nID)
{
	//if(nID==FINISHED&&this==_sClient)
	//	int v=0;
	if(IsPostOnly())
		return XTRUE;
	if(m_pWnd)
	{
		//LOGWHERE();
		m_hPosted.SetEvent();
		if(!m_pWnd->Handle(XWM_SOCKET,nID,(XU32)this))
		{
			m_nCmd=nID;
			m_hPosted.ResetEvent();
			return XFALSE;
		}
		else
		{
			m_nCmd=WAITED;
			return XTRUE;
		}
	}
	else
	{
		return XFALSE;
	}
}

XBOOL XClient::Create(_CLIENTINFO &info,XU32 nID,XPCTSTR strPath)
{
	Release();
	m_bPostOnly=false;
	m_catch.Create(strPath);
	m_pInfo=&info;

	m_hPosted.Create();
	m_hConnect.Create();
	m_hCancel.Create();
	m_hUsing.Create();
	memset(this->m_iStream,0,ICSIZE);
	return XTRUE;
}

void  XClient::SetErrorCode(XU32 nCode) 
{
	if ( nCode == 7 )
	{
		int i = 0;
	}
	m_res.nErrorCode=nCode;
}

XBOOL XClient::IsFinished()
{
	//if(m_pConnect&&m_pConnect->GetProStatus()==XPROC_FINISHED)
	//	return XFALSE;
	if(m_hConnect.IsEvent(0))
	{
		return XFALSE;
	}
	if(m_hPosted.IsEvent(0))
	{
		return XFALSE;
	}
	//LOGWHERE();
	return XTRUE;
}


XU32 XClient::CallBack(XU32 id)
{
	if(!m_hConnect.IsEvent(0)||m_hPosted.IsEvent(0))
	{
		return 1;//Waiting for message result
	}
	else if(m_nCmd)					 //Post message again
	{
		Handle(m_nCmd);
		return 1;
	}
	else if(m_hCancel.IsEvent(0))
	{
		if(m_pConnect)
		{
			m_pConnect->DisConnect();
		}
		this->m_catch.EndCatch(m_res.nLength&&m_res.nLength<=m_res.nTotalSize?m_res.nErrorCode:0);
		m_hConnect.ResetEvent();
		Handle(CANCELED);
		return 0;
	}
	else							 //Procing Connecting...
	{
		//if(m_pConnect==XNULL)
		//{
		//	return 0;
		//	//if(this==_sClient)
		//	//	int v=0;
		//	SetErrorCode(XERR_BAD_PROTO);
		//	m_hCancel.SetEvent();
		//	Handle(FINISHED);
		//	return 0;
		//}
		if ( m_pConnect != NULL )
		{
			XU32 nID=m_pConnect->CallBack();

			if(m_pConnect != &m_catch && m_res.nErrorCode==XHTTP_STATUS_NOT_MODIFIED)
			{
				m_res.nErrorCode=XHTTP_STATUS_OK;
				m_pConnectOld=m_pConnect;
				m_pConnect=&m_catch;
				m_catch.ResetTime();
				//m_catch.ResetTime();
			}
			else if( nID )
			{
				//if(nID==FINISHED&&this==_sClient)
				//	int v=0;
				if(nID==FINISHED&&!IsAlive())
				{
					m_pConnect->DisConnect();
					m_hConnect.ResetEvent();
					m_catch.EndCatch(m_res.nErrorCode);
				}
				Handle(nID);
			}
		}
		else
		{
			//SetErrorCode(XERR_BAD_PROTO);
			//m_hCancel.SetEvent();
			//Handle(FINISHED);
			XU8 nNew=GetAppProto();
			m_nProtoType = nNew;
			switch(m_nProtoType)
			{
			case XAPP_HTTP:
				m_pConnect=new XConnectHttp(this);
				break;

			case XAPP_FILE:
				m_pConnect=new XConnectFile(this);
				break;
			case XAPP_RES:
				m_pConnect=new XConnectRes(this);
				break;
			case XAPP_SOCKET:
				m_pConnect=new XConnectTCP(this);
				break;
			}

			if(m_pConnect!=XNULL)
				m_pConnect->SetStatus(XPROC_BEGIN);
		}

	}

	return 1;
}

void XClient::StartCatch()
{
	if(m_con.URL.m_nProto==XPROTO_SOCKET||!IsNeedCache())
		return;
	if(m_res.nErrorCode==XHTTP_STATUS_OK)
		m_catch.StartCatch(m_con,m_res);
	if(m_res.nErrorCode==XHTTP_STATUS_NOT_MODIFIED)
	{
		if(m_pConnect!=&m_catch)
			m_catch.EndCatch(XFALSE);
	}
}

XU8 XClient::Connect(CONNECTREQ &data,XU8 nCharset,XU8 bSync)
{
	if(m_hConnect.IsEvent(0))
		return 0;//XFALSE;

	XU8 nApp = GetAppProto();
	m_nProtoType = nApp;
	if(m_pInfo->nProtocol != XPROTO_UDP && m_pConnect!=XNULL )
	{
		m_pConnect->DisConnect();
	}
	m_con.Reset(XFALSE);
	if(data.data.GetSize())
	{
		XU8Array&out=m_con.outData;
		out.Append(data.data);
		if(data.strContentType.IsEmpty())
		{
			XContentType type(XCNT::CAPPLICATION,XCNT::TX_WWW_FORM_URLENCODED);
			m_con.strContentType=type.GetTypeString();//strCntTypeURL;
		}
		else
		{
			m_con.strContentType=data.strContentType;
		}
	}
	m_con.URL=data.URL;
	m_con.strRefer=data.strRefer;
	m_con.nMethod=data.nMethod;
	m_con.strHeaders.Append(data.strHeaders);

	//m_res.Reset(nCharset);
	//{//for DEBUG
	//	CONNECTINFO*pCon=GetConnectInfo();
	//	RESPONSEINFO*pInfo=GetResponseInfo();
	//	if(strcmp(m_con.URL.m_strURL,"C:/game.dat") == 0)
     //       int a = 0;//pInfo->strFilterURL="D:\\WorkAS3\\debug.flt";
	//}
	XTime time;
	XU32 l=0;
	XBOOL bFilter=XFALSE;
	if(!IsNeedCache()) bFilter=true;
	bool bStatic=false;
	XU8 nType = bFilter?XCatch::XC_NONE:m_catch.CatchDetect(m_con,time,l,data.bForward,bStatic);
	if(m_con.nMethod!=XEM::GET)
		bStatic=false;
	if(nType==XCatch::XC_OK)
	{
		if(bSync&&l<RECBUFFER&&m_catch.GetCatchData())
		{
			return DATAOK;
		} 
		else if(m_pConnect!=&m_catch)
		{
			XTime nowTime;
			nowTime.SetCurrentTime();
			if(!bStatic||nowTime.GetDay()-time.GetDay()>4)
			{
				XString8 strTime;
				time.GetTime(strTime);
				XEH h;
				XSortString8::ESetValue(m_con.strHeaders,h[XEH::IF_MODIFIED_SINCE],strTime);
			}
			else
			{
				m_pConnectOld=m_pConnect;
				m_pConnect=&m_catch;
				nApp=GetAppProto();
			}
		}
	}
	m_res.Reset(nCharset);
	XU8 nNew=GetAppProto();
	m_nProtoType=nNew;
	if(nApp!=nNew||m_pConnect==XNULL)//||bNewPort)
	{
		if(m_pConnect!=XNULL&&m_pConnect!=&m_catch) 
			delete m_pConnect;
		m_pConnect=XNULL;
		switch(nNew)
		{
		case XAPP_HTTP:
			//sjj
#ifdef _USERSSL_
			m_pConnect=new XConnectHttp(this, XFALSE);
            break;
        case XAPP_HTTPS:
            m_pConnect=new XConnectHttp(this, XTRUE);
#else
			m_pConnect=new XConnectHttp(this);
#endif
            break;
                
		case XAPP_FILE:
			m_pConnect=new XConnectFile(this);
			break;
		case XAPP_RES:
			m_pConnect=new XConnectRes(this);
			break;
		case XAPP_SOCKET:
			m_pConnect=new XConnectTCP(this);
			break;
		}
	}

	if(m_pConnect!=XNULL)
		m_pConnect->SetStatus(XPROC_BEGIN);
	m_hPosted.ResetEvent();
	m_hCancel.ResetEvent();
	m_hConnect.SetEvent();

	return 1;
}

bool XClient::AddHeader(XPCTSTR strKey,XPCTSTR strValue)
{
	int i,iCount=m_con.strHeaders.GetSize();
	bool bSet=false;
	for(i=0;i<iCount;i+=2)
	{
		XString8&key=m_con.strHeaders[i];
		XString8&value=m_con.strHeaders[i+1];
		if(key.Compare(strKey,true)==0)
		{
			bSet=true;
			value=(strValue);
			break;
		}
	}
	if(!bSet)
	{
		m_con.strHeaders.Add(strKey);
		m_con.strHeaders.Add(strValue);
	}
	return true;
}
bool XClient::SendRequest(XU8Array&data)
{
	if(this->m_pConnect==NULL || !this->m_pConnect->IsValid()) 
	   return false;
	m_con.outData.RemoveAll(false);
	m_con.outData.Append(data);
	m_pConnect->CreateRequest();
	m_pConnect->SetStatus(XPROC_BEGIN);
	m_pConnect->SetProcStatus(XPROC_SENDDATA);
	m_res.Reset(m_res.type.m_nCharset);
	return true;
}

XBOOL XClient::Cancel()
{
	if(!m_hConnect.IsEvent(0))
		return 0;
	m_hCancel.SetEvent();
	return 1;
}

XU8 XClient::GetAppProto()
{
	if(m_con.URL.m_nProto==XPROTO_FILE)
		return XAPP_FILE;
	else if(m_con.URL.m_nProto==XPROTO_RES)
		return XAPP_RES;
	if(m_pInfo->nProtocol==XPROTO_TCP && m_nProtoType != XPROTO_SOCKET)
	{
		switch(m_con.URL.m_nProto)
		{
		default:
		case XPROTO_SOCKET:
			return XAPP_SOCKET;
		case XPROTO_HTTP:
			return XAPP_HTTP;
		case XPROTO_HTTPS:
			return XAPP_HTTPS;
			break;
		case XPROTO_FTP:
			return XAPP_FTP;
			break;
		}
	}
	else if (m_pInfo->nProtocol == XPROTO_SOCKET)
	{
		return XPROTO_SOCKET;
	}
	else
	{
		if(m_pInfo->uPort=9200)
			return XAPP_WDP;
		else
			return XAPP_WTP;
	}
	return 0;
}

XBOOL XClient::IsStoped()
{
	if(m_pApply==XNULL)
		return XFALSE;
	return m_pApply->NeedStop();
}

void XClient::PostOK()
{
	//LOGWHERE();
	m_hPosted.ResetEvent();
}

void XClient::Sleep(int v)
{
	if(m_pApply==XNULL)
		return;
#ifdef _SYMBIAN
	m_pApply->m_pThread->Sleep(v);
#else
	m_pApply->m_thread.Sleep(v);
#endif
}
