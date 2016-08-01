// XSocket.h: interface for the XSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSOCKET_H__6C0195A1_1611_4EEA_ACA7_1720C4411415__INCLUDED_)
#define AFX_XSOCKET_H__6C0195A1_1611_4EEA_ACA7_1720C4411415__INCLUDED_

#include "GGType.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {XWAIT_TIMEOUT=0,XWAIT_CANCEL=1,XWAIT_ERROR=2,XWAIT_OK=3};
#define XSOCKET_ERROR -1
class XBase;

typedef struct _XSOCKETADDR
{
        short   sin_family;
        XU16	sin_port;
        XS32    sin_addr;
        char    sin_zero[8];
}XSOCKETADDR;


class XConnectTCP;

class _XEXT_CLASS XSocket  
{
public:
	enum 
	{
		NOTCONNECTED,
		CONNECTTIMEOUT,CONNECTERROR,CONNECTOK,
		SENDTIMEOUT,SENDERROR,SENDOK,
		REVTIMEOUT,REVERROR,REVOK
	};
public:
	XBOOL IsConnect(){return m_bConnected;}
	XBOOL CheckSocket();

	XBOOL IsValid(){return m_hSocket!=XNULL;}
	int Receive(void*pData,int nSize);
	int Send(void*pData,int nSize);
	XINT WaitForReply(XU8 nTime);
	XBOOL Close();
	enum {XC_WAIT,XC_ERROR,XC_OK};
	int ConnectTCP(XPCTSTR strHost,XU16 uPort);
	XSocket();
	virtual ~XSocket();

protected:
	XBOOL		m_bConnected;
	XSOCKETADDR m_destAddr;
	XSOCKET		m_hSocket;
};

#endif // !defined(AFX_XSOCKET_H__6C0195A1_1611_4EEA_ACA7_1720C4411415__INCLUDED_)
