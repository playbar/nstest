// XURL.h: interface for the XURL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XURL_H__7D889947_C29B_4701_A467_C03DA98EAEE4__INCLUDED_)
#define AFX_XURL_H__7D889947_C29B_4701_A467_C03DA98EAEE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XEnumData.h"

enum 
{
	XPROTO_HTTP,
	XPROTO_HTTPS,
	XPROTO_WEBB,
	XPROTO_WEBV,
	XPROTO_FTP,
	XPROTO_SOCKET,
	XPROTO_APP,
	XPROTO_EXT,
	XPROTO_WIN,
    XPROTO_IAPPAY,
	XPROTO_FILE,
	XPROTO_RES,
	XPROTO_SYSTEM
};

class XURL 
{
public:
	static XPCTSTR _strProtocol[];
public:
	static void GetArgList(const char*url,XString8Array&list);
	XURL(XPCTSTR str);
	XURL(XString8&str);
	virtual ~XURL();
	XURL()
	{Reset();}
	void operator=(const XURL&src)
	{
		m_strURL=src.m_strURL;
		m_uPort=src.m_uPort;
		m_strProtocol=src.m_strProtocol;
		m_strPath=src.m_strPath;
		m_strFile=src.m_strFile;
		m_strHost=src.m_strHost;
		m_nProto=src.m_nProto;
		m_strUser=src.m_strUser;
		m_strPasswd=src.m_strPasswd;
		m_strCard=src.m_strCard;
		m_bJustCard=src.m_bJustCard;
	}
	operator XPCTSTR(){return m_strURL;}
public:
	static void Decode(XString8&str);
	static void DecodeNum(XString8&str);
	static void Encode0(XString8&str);
	void MakeHomeURL(XString8&strURL);
	static void Encode(XString8&str);
	void SetURL(XPCTSTR strURL);
	void MakeURL();
	void Reset();
	XString8 m_strHost;
	XString8 m_strPath;
	XString8 m_strFile;
	XString8 m_strURL;
	XString8 m_strProtocol;
	XString8 m_strUser;
	XString8 m_strPasswd;
	XString8 m_strCard;
	XU16	 m_uPort;
	XU8		 m_nProto,m_bJustCard;
protected:
	void FormatPath();
};

enum {XCT_BACK,XCT_CONNECT,XCT_FORWARD,XCT_DIR};
class XDom;
typedef struct _CONNECTREQ
{
	XURL		  URL;
	XString8	  strRefer,strContentType;
	XU8			  nMethod,bForward,bAuto;
	XU32		  nSelect,nText,nStatus;
	XPoint		  offset;
	XString8Array strHeaders,strValues;
	XU8Array	  data;
	XDom*		  pDom;
	struct		  XDOMFILTER*pFilter;
//	XString16	  strTitle;
//	XTime		  time;
	XBOOL operator==(const _CONNECTREQ&r)
	{
		if(nMethod!=r.nMethod) return XFALSE;
		if(nMethod==XEM::POST)
		{
			if(data!=r.data) return XFALSE;
		}
		return URL.m_strURL==r.URL.m_strURL;
	}
	XBOOL operator!=(const _CONNECTREQ&r)
	{
		return !(*this==r);
	}
	void Copy(const _CONNECTREQ&r)
	{
		bAuto=r.bAuto;
		strValues.Copy(r.strValues);
//		time=r.time;
		nStatus=r.nStatus;
		URL=r.URL;
		offset=r.offset;
		nSelect=r.nSelect;
		nText=r.nText;
		strRefer=r.strRefer;
		strContentType=r.strContentType;
//		strTitle=r.strTitle;
		nMethod=r.nMethod;
		bForward=r.bForward;
		strHeaders.Copy(r.strHeaders);
		data.Copy(r.data);
	}
	void operator=(const _CONNECTREQ&r)
	{
		Copy(r);
	}
	
	_CONNECTREQ(XPCTSTR strURL=XNULL,XPCTSTR ref=XNULL)
	{
		pDom=XNULL;
		bAuto=XFALSE;
		if(strURL!=XNULL)
			URL.SetURL(strURL);
		strRefer=ref;
		nStatus=XHTTP_STATUS_OK;
		nMethod=XEM::GET;
		bForward=XCT_CONNECT;
		nSelect=0;
		nText=0;
		offset.x=0;
		offset.y=0;
		pFilter=NULL;
	}
}CONNECTREQ;

typedef XVector<CONNECTREQ*>REQARRAY;

#endif // !defined(AFX_XURL_H__7D889947_C29B_4701_A467_C03DA98EAEE4__INCLUDED_)
