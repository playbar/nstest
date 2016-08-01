#ifndef _XCLIENTAPPLY
#define _XCLIENTAPPLY

#include "XClient.h"
#include "XWAPText.h"
#include "XDomItem.h"
#ifdef _SYMBIAN
#include "XThreadPool.h"
#endif

#ifdef __APPLE__
#define BWIOSUA "IOSiPad;03000000;IOS3.2;BWCSZY/2.9.43"
#define BWIOSCID "10002000"
#endif


#define MAXNEARBY 10
#define MAXBOOKMARKS 40
#define MAXBOOKCLASS 10

#define MAXCLIENTS 20
typedef struct _BOOKMARKER
{
	XString8  strURL;
	XString16 strTitle;
	XString8  strHost;
	XU32	  nPicks;
}BOOKMARKER;
typedef XVector<BOOKMARKER*>MARKLIST;
typedef struct _MARKCLASS
{
	XString16 strClass;
	XU32	  nPicks;
	MARKLIST  lMarks;
}MARKCLASS;



class XClientApply:public XBase
{
	friend class XClient;
public:
	enum {DEC_NEEDDATA=-1,
		  DEC_ERROR=-2,
		  DEC_NOCOMPRESS=0,
		  DEC_OK=1};
	bool DecClientID(XString8&strCnt,XString8&strHost);
	bool EncClientID(XString8&strTime);
	static void MakePassword(XU8Array&data,XU8*lastPd);
	static int  BinEncode(XU8*pData,int l,XU8Array&data);
	static int  BinDecode(XU8*pData,int l,XString8&outStr);
	enum {	SET_ACCEPTS,
			SET_CID,
			SET_COLORS,
			SET_COOKIE,
			SET_DATE,
			SET_DAYS,
			SET_DELTAX,
			SET_DELTAY,
			SET_GATEWAY,
			SET_HELP,
			SET_HOME,			
			SET_HOST,
			SET_LOCALPATH,
			SET_PORT,
			SET_PROFILE,
			SET_PROTOCOL,
			SET_RETRIES,
			SET_SEARCH,
			SET_TIMEOUT,
			SET_UA,
			SET_VERSION,
			SET_XID,
			SET_XTOKEN,
			SET_XTYPE,
	};
public:
	void Stop()
		{
		#ifdef _SYMBIAN
		XThreadPool *pool = XThreadPool::GetInstance();
		pool->FreeThread(m_pThread);
		#else
		m_thread.Stop();
		#endif
		}
	static XClientApply* m_pInstance;
	static XClientApply* GetInstance(){return m_pInstance;}

	void Catch(XClient*){}
	XBOOL NeedStop()
		{
		#ifdef _SYMBIAN
		if(m_pThread == XNULL) return XTRUE;
		return m_pThread->NeedFree();
		#else
		return m_thread.NeedStop();
		#endif
		}
	XClientApply();
	~XClientApply();
//	CLIENTLIST m_clients;
	XBOOL IsModified(){return m_bModify;}
	void SetModify(XU8 b){m_bModify=b;}
public:
	CLIENTINFO m_info;
	XClient* CreateClient(CONNECTREQ&req,XU32 nID,XU32 nNotifyID,XWindow*pWnd);
	void Release();
	void AddFilters(XPCTSTR strTxt,XDomView*pView);
	int GetContent(XPCTSTR strTxt,XString8&txt,XString8&strKey);
	int GetLine(XPCTSTR strTxt,XString8&strKey,XString8&strValue);
	XRETGO* ParserRetGO(XPCTSTR strTxt,int&l);
	int	ParserExtern(XPCTSTR strTxt,XString8&strName,XString8&strValue);
	XPREVFILTER* ParserFilter(XPCTSTR strTxt,int&l);
	void DropFilters();
protected:
	
	XUINT CallBack(XU32 nID);
	XClient*m_pClients,*m_pFreeClients;
#ifdef _SYMBIAN
	XThread *m_pThread;
#else
	XThread m_thread;
#endif
	XMutex	m_dataLock;
	XString8 m_strXID; //,m_strCID;
	void MakeConfigFile(XString8&strFile);
	void InitClient(XClient&client,XU32 nID,XU32 nNotifyID,XWindow*pWnd);
public:
	XU32 m_nXIDTime;
public:
	XString8Array m_strDomainList;
	XTime m_time;
	//XString8	  m_strToken;
	void ResetConfig(XBOOL bDefault,DRAWCONTEXT*pDraw);
	XString8 m_strWorkPath;
	XString8 m_strFilePath;
	XString8 m_strBootFile;
	XPCTSTR GetCID(){return m_info.strChannelID;}
	XPCTSTR GetXID(){return m_strXID;}
	void SetEip( XPCTSTR strEip );
	void SetCID(XPCTSTR strCID);
    void setIDFV( XPCTSTR strIdfv );
    void setSystemVersion( XPCTSTR strSysVer );
	XBOOL IsEmptyCID();		//add by hgl
	BOOKMARKER* FindMarker(XPCWSTR strClass,XPCWSTR strTitle,XPCTSTR strURL,XINT&iClass,XINT&iMarker);
	XBOOL AddMarker(XPCWSTR strClass,XPCWSTR strTitle,XPCTSTR strURL,XU32 nPicks=0,XBOOL bFirst=XTRUE);
	void FromWAP(XString8&strTxt,XU8 nCharset,DRAWCONTEXT*pDraw);
	MARKLIST m_lNearest;
	XVector<MARKCLASS*> m_marks;
public:
	XDOMFILTER* GetFilter(XString8& strURL,XBOOL bFull=XFALSE);
	void UpdateClient();
	void ReleaseClient();
	XBOOL Create();
	void RemoveAllClick();
	void RemoveAllMark();
	void GetLocalPath(XString8&strPath);
	//XBOOL SetXID(XString8&strKey,XPCTSTR strValue);
	//XBOOL SetXID(_RESPONSEINFO*pInfo);
	void  ResetXID(XString8&strHost);
	XBOOL SetToken(XString8&strToken);
	XBOOL SetXID(XString8&strHost,XPCTSTR strValue,bool bEnc);
	void SetMobileType(XString8&strType);
	int AddNearby(XURL&url,XPCWSTR strTitle,XU32 nPicks,XBOOL bSameHost);
	void Click(XURL&url,XPCWSTR strTitle,XBOOL bSameHost);
	void MakeWAPText(XWAPText&wap);
	void SaveConfig();
	XU32 m_nVersion;
	XString8 m_strDays,m_strHost,m_strHelp,m_strHome,m_strSearch; //,m_strType;
	XString8 m_strLocalPath;
	static XPCTSTR _strSetKeys[];
	XBOOL SetValue(XDomItem*p);	
	XU8 m_bCookie,m_bModify; //,m_bModifyToken;
	XDOMFILTER* m_pFilters;
	int	m_nDeltaX;
	int m_nDeltaY;
//#ifdef __APPLE__
#if defined(__APPLE__) || defined(__ANDROID__)	// zhengjl 2012.03
    bool Enc91UID(XString8& strCnt);
    XU32 m_n91UIDTime;
#endif
};

#endif
