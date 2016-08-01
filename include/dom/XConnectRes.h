// XConnectRes.h: interface for the XConnectRes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCONNECTRES_H__0BF6CB13_5974_413B_9615_20F108A20C6F__INCLUDED_)
#define AFX_XCONNECTRES_H__0BF6CB13_5974_413B_9615_20F108A20C6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XConnect.h"

class XConnectRes : public XConnect  
{
public:
	XBOOL GetResponse();
	int Connect(XBOOL bRetry);
	XConnectRes(XClient*pClient);
	virtual ~XConnectRes();
	virtual bool IsValid(){return true;}
	virtual void CreateRequest(){}
protected:
	void Begin();
	int GetData();
	int SendData(XBOOL bRetry);
};

#endif // !defined(AFX_XCONNECTRES_H__0BF6CB13_5974_413B_9615_20F108A20C6F__INCLUDED_)
