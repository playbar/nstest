// XSocket.cpp: implementation of the XSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XSocket.h"
#include "XMutex.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XSocket::XSocket()
{
   m_hSocket=XNULL;
   m_bConnected=XFALSE;
}

XSocket::~XSocket()
{

}

#define T_RETRIES 3
#define T_TIMEOUT 500


int XSocket::ConnectTCP(XPCTSTR strHost, XU16 uPort)
{
	if(m_bConnected) 
		return XC_OK;
	if(m_hSocket==XNULL)
	{
		hostent *phostent=gethostbyname(strHost);
		if(phostent==XNULL)
			return XC_ERROR;
		
		struct in_addr ip_addr;
		memcpy(&ip_addr,phostent->h_addr_list[0],4);
		
		struct sockaddr_in& destAddr=(sockaddr_in&)m_destAddr;
		memset((void *)&destAddr,0,sizeof(destAddr)); 
		destAddr.sin_family=AF_INET;
		destAddr.sin_port=htons(uPort);
		destAddr.sin_addr=ip_addr;
		
		m_hSocket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);		
		unsigned   long   ul   =   1; 
		int ret = ioctlsocket(m_hSocket,   FIONBIO,   (unsigned   long*)&ul); 
		if(ret==SOCKET_ERROR)
		{
			closesocket(m_hSocket);
			m_hSocket=XNULL;
			return XC_ERROR;
		} 
	}
	m_bConnected=XFALSE;
	int nRet=connect(m_hSocket,(struct sockaddr*)&m_destAddr,sizeof(m_destAddr));
	if(nRet == -1)
	{
		return XC_WAIT;
	}
	else if(nRet!=0)
		return XC_ERROR;
	else
	{
		m_bConnected=XTRUE;
		return XC_OK;
	}
	return XC_WAIT;

}

XBOOL XSocket::Close()
{
	if(m_hSocket!=XNULL)
		closesocket(m_hSocket);
	m_hSocket=XNULL;
	m_bConnected=XFALSE;
	return XTRUE;
}

XINT XSocket::WaitForReply(XU8 nTime)
{
	if(m_hSocket==XNULL) 
		return NOTCONNECTED;

	struct timeval Timeout={ 0, nTime?nTime:100};

	if(!m_bConnected)
	{
		fd_set mask;
		FD_ZERO(&mask);
		FD_SET(m_hSocket,&mask);
				
		int nv = select(m_hSocket + 1, 0, &mask, 0, &Timeout);
		switch (nv) 
		{
			case -1:
				closesocket(m_hSocket);
				m_hSocket=XNULL;
				return CONNECTERROR;
			case 0:
				return CONNECTTIMEOUT;
			default: 			
				m_bConnected=XTRUE;
				return CONNECTOK;
		}
	}

	fd_set readfds,expfds;
	expfds.fd_count = 1;
	expfds.fd_array[0]=m_hSocket;
	//XU32 nTime=m_info.nTimeout;
	if(nTime<1) 
	{
		Timeout.tv_sec = 0;
		Timeout.tv_usec=100;
		nTime=1;
	}
	readfds.fd_count = 1;
	
	//readfds.fd_array[0] = m_hSocket;
	readfds.fd_array[0] = m_hSocket;//recSocket;
	
	int nv=select(1, &readfds, NULL,&expfds, &Timeout);
	if(nv>0) 
		return REVOK;
	else if(nv==SOCKET_ERROR)
		return REVERROR;

	return REVTIMEOUT;
}

int XSocket::Send(void *pData, int nSize)
{
   int v=sendto(m_hSocket,(char*)pData,nSize,0,(struct sockaddr*)&m_destAddr,sizeof(m_destAddr));
   return v;
}

int XSocket::Receive(void *pData, int nSize)
{
   sockaddr addr;
   int n=sizeof(addr);
   int len = recvfrom(m_hSocket,(char*)pData,nSize,0,&addr,&n);
   return len;
}


XBOOL XSocket::CheckSocket()
{
 	if(m_hSocket!=XNULL)
	{
		if(WaitForReply(0)>=XWAIT_OK)
		{
			XU32 data;
			if(Receive((char*)&data,sizeof(XU32))==0)
			{
				Close();
				return XFALSE;
			}
		}
	}
	return XTRUE;
}

