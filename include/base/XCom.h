// XCom.h
//
//////////////////////////////////////////////////////////////////////
#ifndef _X_COM_H
#define _X_COM_H

#include "XString.h"
#include "XSocket.h"
#include "SSLAPI.h"

extern "C" void SSLLogCallback(const char * msg);

class _XEXT_CLASS XCom
{
protected:
	static XBOOL m_bSSLLibReady;
	static SSLCTXPTR m_pCTX;
	static XINT m_nTimeout;
	static int m_myDataIndex;

public:
	static XBOOL SSLLibStartup();

	static XBOOL SSLCTXSetup(XPCTSTR szUserX509File,
							 XPCTSTR szUserKeyFile,
							 XPCTSTR szUserKeyPass,
							 XPCTSTR szCaCertFile,
							 XPCTSTR szCaCertPath,
							 XINT nDefaultTimeout);
	
	static void SSLCTXCleanup();
	static XBOOL SSLIsReady();

public:
	enum {
		NOTCONNECTED = XSocket::NOTCONNECTED,
		CONNECTTIMEOUT = XSocket::CONNECTTIMEOUT,
		CONNECTERROR = XSocket::CONNECTERROR,
		CONNECTOK = XSocket::CONNECTOK,
		SENDTIMEOUT = XSocket::SENDTIMEOUT,
		SENDERROR = XSocket::SENDERROR,
		SENDOK = XSocket::SENDOK,
		REVTIMEOUT = XSocket::REVTIMEOUT,
		REVERROR = XSocket::REVERROR,
		REVOK = XSocket::REVOK
	};

public:

	XBOOL IsValid();
	XBOOL IsConnect();
	XBOOL CheckSocket();
	int Receive(void*pData,int nSize);
	int Send(void*pData,int nSize);
	XINT WaitForReply(XU8 nTime);
	XBOOL Close();
	enum {XC_WAIT,XC_ERROR,XC_OK};
	int ConnectTCP(XPCTSTR strHost,XU16 uPort);
	XCom(XBOOL bUseSSL);
	virtual ~XCom();

protected:
	// for SSL
	XBOOL		m_bUseSSL;
	SSLPTR		m_pSSL;
	XBOOL		m_bSocketConnected; // valid only when m_pSSL is not null
	char		m_prefetchBuffer[1];
	int			m_prefetchNum; // At most 1 in this implementation.
	XTCHAR		m_host[SSL_HOST_SIZE]; // The host name to which this XCom object connects

	// for non-SSL
	XSocket		m_xsocket;
};

#endif // _X_COM_H
