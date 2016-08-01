// XSocket.cpp: implementation of the XSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#include "XSocket.h"
#include "XMutex.h"

#include <errno.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define T_RETRIES 3
#define T_TIMEOUT 500
//int XSocket::mnDataSize = 0;
//int XSocket::mnConCount = 0;

XSocket::XSocket()
{
   m_hSocket=XNULL;
}

XSocket::~XSocket()
{

}

int XSocket::ConnectTCP(XPCTSTR strHost, XU16 uPort)
{
	if(m_bConnected)
		return XC_OK;
//	mnConCount++;
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

//		int flags=fcntl(m_hSocket,F_GETFL,0);
//		fcntl(m_hSocket,F_SETFL,flags|O_NONBLOCK);
	}
	m_bConnected=XFALSE;
	int nRet=connect(m_hSocket,(struct sockaddr*)&m_destAddr,sizeof(m_destAddr));
	if( nRet == 0 )
	{
		m_bConnected=XTRUE;
		return XC_OK;
	}
	else if(nRet == -1)
	{
		return XC_ERROR;
		return XC_WAIT;
	}
	else
	{
		return XC_ERROR;
	}
	return XC_WAIT;

}

XBOOL XSocket::Close()
{
	if(m_hSocket!=XNULL) close(m_hSocket);
	m_hSocket=XNULL;
	m_bConnected=XFALSE;
	//mnConCount--;
	return XTRUE;
}

XINT XSocket::WaitForReply(XU8 nTime)
{
	if(m_hSocket==XNULL)
		return NOTCONNECTED;

	//struct timeval Timeout={0,nTime?nTime:100};
	struct timeval Timeout={0,nTime?nTime:2000};
	//struct timeval Timeout={nTime?nTime:20,0};

	if(!m_bConnected)
	{
		fd_set mask;
		FD_ZERO(&mask);
		FD_SET(m_hSocket,&mask);

		int nv = select(m_hSocket + 1, 0, &mask, 0, &Timeout);
		switch (nv)
		{
			case -1:
				close(m_hSocket);
				m_hSocket=XNULL;
				return CONNECTERROR;
			case 0:
				return CONNECTTIMEOUT;
			default:
				m_bConnected=XTRUE;
				return CONNECTOK;
		}
	}

	fd_set readfds;
//	if(nTime<1)
//	{
//		Timeout.tv_sec=0;
//		Timeout.tv_usec=100;
//		nTime=1;
//	}

	FD_ZERO(&readfds);
	FD_SET(m_hSocket,&readfds);
	int nv=select(m_hSocket + 1, &readfds, NULL, NULL, &Timeout);
	if(nv>0)
	{
		return REVOK;
	}
	else if( nv == 0 )
	{
		return REVTIMEOUT;
	}
	else
	{
		return REVERROR;
	}

}

int XSocket::Send(void *pData, int nSize)
{
	 try {
	        int v=sendto(m_hSocket,(char*)pData,nSize,0,(struct sockaddr*)&m_destAddr,sizeof(m_destAddr));
	        return v;
	    } catch (...) {
	        return SENDERROR;
	    }
		return SENDERROR;
}

int XSocket::Receive(void *pData, int nSize)
{
	sockaddr addr;
	 int n=sizeof(addr);
	   int len = recvfrom(m_hSocket,(char*)pData,nSize,0,&addr,&n);
	  // mnDataSize += len;
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

