#ifndef _X_CONNECT_HTTP_H_
#define _X_CONNECT_HTTP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XConnect.h"
#include "XClient.h"



class XConnectTCP : public XConnect
{
public:
	XConnectTCP(XClient *pClient);
	virtual void Write(XU8*pData,int l);
	virtual int Connect(XBOOL bRetry);
//	int Connect(XPCTSTR strHost, XU16 uPort);
	virtual bool IsValid(){return m_socket.IsValid()!=0;}
	virtual void CreateRequest(){}
	virtual int SendData(XBOOL bRetry);

	virtual int GetData();

	virtual int ProcData(XU8*pData,int& nSize);
//	virtual int ProcContent(XU8*pData,XU32 nSize){ return 0; }

	virtual void DisConnect();
	//XBOOL	IsConnect(){ return m_socket.IsConnect(); }

	//int CreateSocket(XPCTSTR strHost, XU16 uPort);
	//XSocket GetSocket(){ return m_socket; }

public:
	unsigned int	GetBytesAvailable()
	{
		return m_inData.GetSize();
	}
	//XBOOL			IsConnected()
	//{
	//	return m_socket.IsConnect();
	//}

protected:
	virtual void Begin();

private:
	XSocket m_socket;
	bool	m_bNeedSend;
	//XClient *m_pClient;
};

#endif