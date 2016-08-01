// XConnect.h: interface for the XConnect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCONNECT_H__65A90DAA_D4AB_4202_A5D6_1E557CEFEC74__INCLUDED_)
#define AFX_XCONNECT_H__65A90DAA_D4AB_4202_A5D6_1E557CEFEC74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// sjj
#ifdef __ANDROID__
#else
//#include "XCom.h"
#endif

#include "XURL.h"
#include "XEnumData.h"
#include "XContentType.h"
class XClient;

#define MAX_PACK 256000
#define RECBUFFER 256000

enum 
{
	XPROC_START,XPROC_CONNECT,XPROC_SENDDATA,
	XPROC_GETDATA,XPROC_PROCDATA,XPROC_PROCDATAEND,
	XPROC_BEGIN,XPROC_HEADER,XPROC_CHUNKED,XPROC_CONTENT,XPROC_FINISHED,
	XPROC_DONE, XPROC_DISCONNECT,XPROC_WAIT
};

enum {XPROTO_TCP,XPROTO_UDP};
enum {XTCD_ID=0,XTCD_CHUNKED,XTCD_GZIP,XTCD_DEFLATE};



typedef struct _CONNECTINFO
{
	XURL	    URL;
	XString8	strRefer,strContentType;
	XU8			nMethod,nStatus;
	XU8Array	outData;
	XStream		stream;
	XU32		nSessionID;

	XString8Array strHeaders;
	void Reset(XBOOL bSession)
	{	
		strHeaders.RemoveAll();
		nMethod=XEM::GET;
		nStatus=XPROC_BEGIN;
		stream.Reset();
		outData.RemoveAll();
		if(bSession) 
			nSessionID=0;
	};
}CONNECTINFO;

typedef struct _RESPONSEINFO
{
	XString8 strVersion,strLocation;
	XString8 strConfigURL,strClientID,strFilterURL;
	XString8 strServer,strAcceptRanges,strPower,strETag;
	XU32 nErrorCode;
	XRingBuffer recBuf;
	XU32 nTotalSize,nLength;
	XString8Array strResHeaders;
	XString8Array strCookies;
	XContentType  type;
	XTime time;
	XU32  nMaxAge;
	XU8	  nCacheMode,nEncode;

	bool  bNoData;
    XU8   nShowBar;
    XU8   nShowAD;
    XU8   nOnSale;
	XU8   nGType;
	XU8   nStype;
	XU8   nAppStatus;
	XString8 nNewApp;
	XString8 mStrAppDesc;
    XU8   nOutLogin;	// zhengjl 2012.03


	_RESPONSEINFO():recBuf(RECBUFFER)
	{
		Reset(XCHARSET_GB2312);
	}
	void Reset(XU8 nCharset)
	{
		bNoData=false;
        nShowBar=2;
        nShowAD = 2;
		nAppStatus = 0;
		nNewApp.Empty();
        nOnSale=2;
		nGType = 2;
		nStype = 2;
        nOutLogin=2;
		mStrAppDesc.Empty();

		nCacheMode=0;
		nEncode=XTCD_ID;
		strLocation.Empty();
		strResHeaders.RemoveAll();
		strCookies.RemoveAll();
		strServer.Empty();
		strPower.Empty();
		strETag.Empty();
		strAcceptRanges.Empty();
		time.SetCurrentTime();
		nTotalSize=0;
		nMaxAge=7200;
		type.Reset(0,0,nCharset);
		nLength=0;
		recBuf.Reset();
		nErrorCode=1;
		strClientID.Empty();
		strConfigURL.Empty();
		strFilterURL.Empty();
	}
}RESPONSEINFO;



typedef struct _CLIENTINFO
{
   XString8 strGateway;
   XString8 strUserAgent;
   XString8 strMAC;
   XString8 strEip;
   XString8 strVCODE;
    XString8 strIDFV;
   XString8 strChannelID;
   XString8 strMobileType;
   XString8 strClientID;
   XString8 strXToken;
    XString8 strSysVer;
   //XU8 m_bModifyToken;
   XString8Array strAccepts;
   XString8Array strHeaders;
   XString8 strProfile;
   XU8		nProtocol,bKeepAlive;
   XU8	    nRetries;
   XU8	    nTimeout;
   XU16	    uPort;
   XU32		 nSDUSize;
   XU8Array	 nLanguages;
   XU8Array	 nCharsets;
  
   void operator=(const _CLIENTINFO&info)
   {
	   bKeepAlive=info.bKeepAlive;
	   strGateway=info.strGateway;
	   strUserAgent=info.strUserAgent;
	   strAccepts.Append(info.strAccepts);
	   strHeaders.Append(info.strHeaders);
	   strProfile=info.strProfile;
	   nProtocol=info.nProtocol;
	   nRetries=info.nRetries;
	   nTimeout=info.nTimeout;
	   nSDUSize=info.nSDUSize;
	   nLanguages.Append(info.nLanguages);
	   nCharsets.Append(info.nCharsets);
   }
   void Reset()
   {
	   bKeepAlive=XTRUE;//XFALSE;
	   nProtocol=XPROTO_UDP;
	   uPort=0;
	   nRetries=1;
	   nTimeout=20;
	   nSDUSize=0;
	   strAccepts.RemoveAll();
	   strHeaders.RemoveAll();
	   nLanguages.RemoveAll();
	   nCharsets.RemoveAll();
   }
}CLIENTINFO;

class XConnect
{
public:
	void Handle(XU8 nHandle);
	virtual void Write(XU8*pData,int l){}
	virtual void DisConnect();
	enum {CWAIT_TIMEOUT=0,CWAIT_ERROR=1,CWAIT_OK=2,CWAIT_CONTINUE=3,CWAIT_FINISHED=4};
	virtual XU32 CallBack();
	XConnect(XClient*pClient);
	XU8 GetStatus(){return m_nStatus;}
	void SetStatus(XU8 nStatus){m_nStatus=nStatus;}
	void SetProcStatus(XU8 nStatus)
	{
		m_nProcStatus=nStatus;
	}
	virtual ~XConnect();
	virtual int Connect(XBOOL bRetry)=0;
	virtual int SendData(XBOOL bRetry)=0;
	virtual int GetData()=0;
	virtual bool IsValid()=0;
	virtual void CreateRequest()=0;

//dushixiong --start
	XStream *GetOutStream(){ return &m_outStream; }
	XU8		GetProStatus(){ return m_nProcStatus; }
//dushixiong --end

protected:
	virtual void Begin()=0;
	virtual int ProcChunked(XU8*pData,XU32 nSize){return nSize;}
	virtual int ProcHeader(XU8*pData,XU32 nSize){return nSize;}
	virtual int ProcBegin(XU8*pData,XU32 nSize){return nSize;}
	virtual int ProcContent(XU8*pData,XU32 nSize);
	virtual void ProcFinished(){m_nStatus=XPROC_DONE;}
	virtual int  ProcData(XU8*pData,int& nSize);
	//virtual XINT WaitForReply(XU8 nTime)=0;
	CONNECTINFO*  GetConnectInfo();
	RESPONSEINFO* GetResponseInfo();
	CLIENTINFO*	  GetClientInfo();
	XBOOL	NeedStop();
	void SetErrorCode(XU32 nCode);
	XClient*m_pClient;
//	XSocket	m_socket;
	bool	m_bFirst;
	XU8		m_nStatus,m_nProcStatus;
	XStream	m_outStream;
	XU8Array m_inData;
	XU32	m_nTime,m_nRetries,m_inPos,m_outPos;
	static XPCTSTR _strKeepAlive[],_strHttpVersion;
	XStream m_zipData;
	XBOOL m_bChunkEnd;
};

class XConnectHttp : public XConnect
{
//public:
//	static XU32  mTimeStart;
//	static XU32 mTimeEnd;
//	static int mnDataSize;
//	static int mnConCount;
//	bool mbConnect;
public:
	int GetData();
	int SendData(XBOOL bRetry);

	// sjj
#ifdef _USERSSL_//__ANDROID__
	XConnectHttp(XClient * pClient, XBOOL bUseSSL);	
	XCom m_socket;
#else
	XConnectHttp(XClient*pClient);
	XSocket m_socket;
#endif

protected:
	virtual bool IsValid(){return m_socket.IsValid()!=0;}
	void DisConnect();
	int Connect(XBOOL bRetry);
	void Begin();

	void ProcCache(RESPONSEINFO*p,XString8&str);
	void CreateConnectHeaders();
	void CreateClientHeaders();
	void CreateReqHeader();
	virtual void CreateRequest();
	void SetHeader(XPCTSTR strKey,XPCTSTR strValue,XU8 q=0);
	XString8 m_inString;
	XString8Array m_strHeaders;
	virtual void ProcFinished();
	int ProcHeader(XU8*pData,XU32 nSize);
	int ProcChunked(XU8*pData,XU32 nSize);
	int ProcBegin(XU8*pData,XU32 nSize);
};

#endif // !defined(AFX_XCONNECT_H__65A90DAA_D4AB_4202_A5D6_1E557CEFEC74__INCLUDED_)
