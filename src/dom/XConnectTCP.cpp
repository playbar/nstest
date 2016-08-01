
#include "StdAfxggdom.h"
#include "XClient.h"
#include "XConnectTCP.h"

#ifdef _WINEMU
	#include "XMainWnd.h"
#endif

XConnectTCP::XConnectTCP(XClient *pClient) : XConnect(pClient)
{

}

int XConnectTCP::Connect(XBOOL bRetry)
{
	CONNECTINFO*pInfo=GetConnectInfo();

	if(bRetry)
	{
		m_socket.Close();
		switch(m_socket.ConnectTCP(pInfo->URL.m_strHost,pInfo->URL.m_uPort))
		{
		case XSocket::XC_OK:
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
			return CWAIT_OK;
		case XSocket::CONNECTTIMEOUT:
			return CWAIT_TIMEOUT;
	}

	return CWAIT_TIMEOUT;

}

int XConnectTCP::SendData(XBOOL bRetry)
{
	if(!m_socket.IsValid()) 
		return CWAIT_ERROR;

	XUINT nIndex=0,nSize=m_outStream.GetSize()-m_outPos;
	if(nSize<=0) 
		return CWAIT_OK;
	if(nSize>BLOCKSIZE) nSize=BLOCKSIZE;
    int nv=m_socket.Send((char*)m_outStream.GetData()+m_outPos,nSize);
	if(nv==0)
		return CWAIT_TIMEOUT;
	else if(nv==XSOCKET_ERROR) 
	{
		return CWAIT_ERROR;
	}

	else m_outPos+=nv;
	return CWAIT_OK;	

}

void XConnectTCP::Write(XU8*pData,int l)
{
	this->m_outStream.WriteData(pData,l);
	m_bNeedSend=true;
}

int XConnectTCP::GetData()
{
	//Throw error ?

	switch(m_socket.WaitForReply(0))
	{
	case XSocket::REVERROR:
	case XSocket::NOTCONNECTED:
		return CWAIT_ERROR;

	case XSocket::REVTIMEOUT:
		if(m_bNeedSend)
		{
			m_bNeedSend=false;
			m_nProcStatus=XPROC_SENDDATA;
		}
		return CWAIT_CONTINUE;

	case XSocket::REVOK:
		{
			int nv=m_socket.Receive((char*)m_inData.GetData(),MAX_PACK);
			if(nv==0)
			{				
				if(m_bNeedSend)
				{
					m_bNeedSend=false;
					m_nProcStatus=XPROC_SENDDATA;
				}
				return CWAIT_CONTINUE;
			}
			else if(nv==XSOCKET_ERROR)
				return CWAIT_ERROR;
			else
			{
#ifdef _WINEMU
				XWindow*pWnd=XWindow::m_pMainWnd;
				if(pWnd)
					pWnd->Handle(XCMD_UPDATEPROFILE,XCMD_UPDATE_NETRATE,nv);
#endif
				RESPONSEINFO *pInfo = GetResponseInfo();
				m_inData.SetSize(nv);
				return CWAIT_OK;
			}
		}break;
	}
	return CWAIT_CONTINUE;
}

void XConnectTCP::Begin()
{
	this->GetResponseInfo()->nErrorCode=XHTTP_STATUS_OK;
	m_inData.SetSize(MAX_PACK,XFALSE);
	return;
}

int XConnectTCP::ProcData(XU8*pData,int& nSize)
{
	if(nSize<=0) 
	{
		return XClient::WAITED;
	}
	else
	{		
		this->ProcContent(pData,nSize);
		nSize = 0;

		return XClient::RECIVE;
	}

}

void XConnectTCP::DisConnect()
{

	m_socket.Close();
	XConnect::DisConnect();

}
