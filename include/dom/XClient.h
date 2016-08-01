// XClient.h: interface for the XClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCLIENT_H__9C255BD7_D73D_4629_856F_6D80DE22232F__INCLUDED_)
#define AFX_XCLIENT_H__9C255BD7_D73D_4629_856F_6D80DE22232F__INCLUDED_

#include "XCatch.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XEnumData.h"
#include "XURL.h"
#include "XConnect.h"

class XClientApply;

#define ICSIZE 128

enum {XAPP_HTTP,XAPP_HTTPS,XAPP_FTP,XAPP_WDP,XAPP_WTP,XAPP_FILE,XAPP_RES,XAPP_SOCKET};


class _XEXT_CLASS XClient// : public XBase  
{
	friend class XConnect;
	friend class XClientApply;

public:
	/*void SetFilter(XDOMFILTER*filter)
	{
		if(m_pCurFilter)
			m_pReferFilter=m_pCurFilter;
		m_pCurFilter=filter;
	}
	XDOMFILTER* GetFilter()
	{
		if(m_pCurFilter) return m_pCurFilter;
		return m_pReferFilter;
	}
	struct XDOMFILTER* m_pReferFilter,*m_pCurFilter;*/
	enum {FAILED,CONNECTOK,DATAOK};
	enum {WAITED,CONNECTED,STARTED,RECIVE,FINISHED,CANCELED};
	XU8 Connect(CONNECTREQ&data,XU8 nCharset,XU8 bSync=XFALSE);
	bool AddHeader(XPCTSTR strKey,XPCTSTR strValue);
	bool SendRequest(XU8Array&data);
	
	XBOOL Cancel(); 
	XPCTSTR GetURL()
	{
		return m_con.URL.m_strURL;
	}
	XBOOL Create(_CLIENTINFO&info,XU32 nID,XPCTSTR strPath);
	
	XBOOL IsConnecting() {return m_hConnect.IsEvent(0);}
	bool IsUsing()
	{
		return m_hUsing.IsEvent(0)!=0;
	}
	void SetUsing(bool b)
	{
		if(b)
			m_hUsing.SetEvent();
		else
			m_hUsing.ResetEvent();
	}
	XU32  GetErrorCode() {return m_res.nErrorCode;}
	void  SetErrorCode(XU32 nCode);

	RESPONSEINFO* GetResponseInfo(){return &m_res;}
	CONNECTINFO*  GetConnectInfo(){return &m_con;}
	XU8	GetStatus()
	{
		if(m_pConnect==XNULL) return XPROC_BEGIN;
		else			  	  return m_pConnect->GetStatus();
	}
	XBOOL IsCanceled(){return m_hCancel.IsEvent(0);}
	void SetPostOnly(bool bPost=true){m_bPostOnly=bPost;}
	bool IsPostOnly(){return m_bPostOnly;}
public:
	void StartCatch();
	void Catch(void*pData,int nSize)
	{
		if(m_con.URL.m_nProto==XPROTO_SOCKET)
			return;
		if(m_pConnect!=&m_catch)
			m_catch.Catch(pData,nSize);
	}
protected:
	XBOOL Release();
	virtual ~XClient();
	XClient(XClientApply*pApply);
	
	XBOOL Handle(XU32 nID);
	
public:
	void PostOK();
	void Sleep(int v);
	XBOOL IsStoped();
	XBOOL IsFinished();
	XU32 CallBack(XU32 id);
	XU8 m_nProtoType;
	XU8* GetDataI(){return m_iStream;}
	XU32 GetNotifyID(){return m_nNotifyID;}
	void SetNotifyID(XU32 nID){m_nNotifyID=nID;}
	void SetCallID(XU32 nID);
	XU32 Get_CallID();
	void SetWindow(XWindow*pWnd,XU32 nID,XU32 nNotifyID);
	
	//dushixiong --start
	XConnect *GetConnect(){ return m_pConnect; }
	//dushixiong --end
public:
	XU8Array m_userData;
	XU32 m_nUserData;
	void SetAlive(bool b){m_bKeepAlive=b;}
	bool IsAlive(){return m_bKeepAlive;}
	bool IsNeedCache(){return m_bNeedCache;}
	void SetNeedCache(bool b){m_bNeedCache=b;}
protected:
	bool	 m_bPostOnly;
	bool	 m_bKeepAlive;
	bool	 m_bNeedCache;
	XCatch   m_catch;
	XU32	 m_nCallID,m_nCmd,m_nNotifyID;
	XWindow* m_pWnd;
	XConnect* m_pConnect,*m_pConnectOld;
	XClient* m_pNext;
	
	XU8 GetAppProto();
//	virtual XUINT CallBack(XU32 nID);
	XClientApply*m_pApply;
	CLIENTINFO* m_pInfo;
	RESPONSEINFO m_res;
	CONNECTINFO m_con;
	//XSOCKET m_hSocket;
	XEvent m_hConnect,m_hCancel,m_hPosted,m_hUsing;
//	XThread m_thread;
	XU8		m_iStream[ICSIZE];
	//XU8	     m_nStatus;
	//XString8Array m_strHeaders;
};

typedef XVector<XClient*> CLIENTLIST;




#endif // !defined(AFX_XCLIENT_H__9C255BD7_D73D_4629_856F_6D80DE22232F__INCLUDED_)
