// XConfig.h: interface for the XConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCONFIG_H__9AEBC847_6FA8_43E0_A44E_57CF1AC205B7__INCLUDED_)
#define AFX_XCONFIG_H__9AEBC847_6FA8_43E0_A44E_57CF1AC205B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#pragma pack(1)
struct XPREVFILTER
{
	XString8 strHost;
	XString8 strDest;
	int		 nMethod;
	XString8 strPost;
	XString8 strPostDest;
	int		 nType;
	int		 nReplace;
	bool	 bPost;
	XPREVFILTER*pNext;
	enum {_SELF,_POPUP, _FILTER_FULL, _FILTER_HALT,  _NULL};
};
struct XRETGO
{
	XString8Array strOptions;
	XString8Array strURLs;
	XU8Array	  nMethods;
	XString8Array strPosts;
};
struct XDOMFILTER
{
	enum {AD_TOPLEFT,AD_TOP,AD_TOPRIGHT,AD_LEFT,AD_CENTER,AD_RIGHT,AD_LEFTBOTTOM,AD_BOTTOM,AD_RIGHTBOTTOM};
	XString8 strHost;
	XString8Array strBegins;
	XString8Array strEnds;
	XString8 strAutoLocation;
	XString8Array strParams;
	XString8Array strAutoHeaders;
	XString8Array strExtFuns;
	XString8Array strExtFuns2;
//#if (__CORE_VERSION__>=0x02079000)
	XString8Array strExtFuns3;
	XString8Array strExtFuns4;
//#endif
//#if (__CORE_VERSION__>=0x02080000)
	XVector<XU8Array> repSrc;
	XVector<XU8Array> repDst;
//#endif
	XString8Array strLoaders;
	XString8Array strAllows;
	XString8Array strVars;
	XString8Array strClassLock;
	XString8Array strFormDatas;
	XString8Array strReplaceLibs;
	XString8 strCookies;
	//XString8Array strHoldList;
	XString8 strAutoGo;
	XString8 strAutoPost;
	XString8 strRefeshURL;
	XString8 strPayURL;
    //XString8 strOfferWallPayURL;
	XString8 strADURL;
	XString8 strEndURL,strContentID,strWaitFor;
	XRETGO*		  pRetGO;
	XPREVFILTER*  pRevFilter;
	int		      nMatchID,nMethod,nADMode;
	int			  nFontBase,nFontZoom,nFlex,nWaitID;
	//XBOOL		  bAutoGo,bAutoFrame,
	int			  nAutoFrame,nAutoForm,nAutoObject;
	XBOOL		  bFullMatch,bModifyDomain,bLoadScript,bEnuFilter,bMouseShow,bAdvanceGC,bReplaceSWZ,bHold,bDispatchLater,bEnuBlendMode;
	//int			  nContentMode;
	//int				nQuality;
	int			  nCacheImage,nCacheBitmap,nCacheObject;
	int			  nCacheMemory,nImgDiv;
	int			  nFrameWidth,nFrameHeight,nMaxPecent;
	int			  nAutoCap;
	int			  nEnd;
	XBOOL		  bBegin;
	XU32		  nBackColor;
	XDOMFILTER*   pNext;
	XBOOL		  IsAutoMode()
	{
		return (!strAutoGo.IsEmpty())||nAutoFrame||nAutoForm||nAutoObject;
	}
//#if (__CORE_VERSION__>=0x02070200)
	XBOOL		  bExternAPI,bDotAdjust;
//#endif
//#if (__CORE_VERSION__>=0x02075000)
	XU32 m_nBlockWidth,m_nBlockHeight;
//#endif
	XString8 strLoadingBG;
};

struct _XKEYMAP
{
	XU16 nKeyCode;
	XU16 nKeyMap;
	XU8  bLongPress;
	XU8  bLock;
	XU16 nLongLockTime;
	XU32 nPressTime;
};

enum {KLK_RELEASE,KLK_PRESSED,KLK_LOCKED};

struct _XBUTTONMAP
{	
	enum {TOPLEFT,TOPRIGHT,BOTTOMLEFT,BOTTOMRIGHT};
	XU8 nButtonID;
	XS16 nButtonPosX;
	XS16 nButtonPosY;
	XU8 bLongPress;
	XU8 bLock,nOrg,nHitPos;
	XU16 nKeyMap;
	XU16 nLongLockTime;
	XU16 nShowTime;
	XDib* pDib;
	XU32 nPressTime;
	
	//SBitmapCore* pBitmap;
};



struct _XMOUSEACT
{
	XU16 nKeyCode;
	XU8  nAct;
	XU8  nTimes;
};

//#pragma pack()

typedef XVector<_XMOUSEACT> MOUSEACTS;
struct _XMOUSEMODE
{
	enum {A,B,DOWN,LEFT,LEFT_KEY,POSX,POSY,RIGHT,RIGHT_KEY,UP};
	enum {XNORMAL,XTABLE};
	XString8 strName;
	XString16 strName16;
	XU8 nMode;
	MOUSEACTS acts;
};
typedef XVector<_XMOUSEMODE*> MOUSEMODES;

struct _ACCMAP
{
	enum {X,RX,Y,RY,Z,RZ};
	int nDirect;
	int nValue;
	int nMapKey;
	int nMapDirect;
	int nMapStep;
	int nStatus;
	int nOnlyOne;
};

struct _TAGSHOW
{
	enum {TOPLEFT,TOPRIGHT,BOTTOMLEFT,BOTTOMRIGHT,CENTER};
	int nCode;
	int nPlace;
	int nZoom;
	//int align;
	int offsetx;
	int offsety;
};
typedef XVector<_TAGSHOW> SHOWMAP;

typedef XVector<_ACCMAP> ACCMAPS;
typedef XVector<_XKEYMAP> KEYMAPS;
typedef XVector<_XBUTTONMAP> BUTTONMAPS;
//enum {XCNT_MODE_LOCAL,XCNT_MODE_NET,XCNT_MODE_VIDEO};
class XConfig  
{
public:	   
	enum
	{
		LOW=0,
		MID=1,
		HIGH=2
	};
	class XSortMap:public XSort
	{
	public:
		XSortMap(SHOWMAP*pShow)
		{
			m_pShows=pShow;
		}
		void AddMap(_TAGSHOW&show)
		{
			int mi,id;
			id=Index((void*)((show.nPlace<<16)|show.nCode),mi);
			if(id<0)
				m_pShows->InsertAt(mi,show);
			else
				(*m_pShows)[id]=show;
		}
		inline int FindMap(int nCode,int nPlace)
		{
			return Index((void*)((nPlace<<16)|nCode));
			//return id;
		}
		int Compare(void*iSrt,void*iDst,XU8)
		{
			return CompareUInt((XU32)iSrt,(XU32)iDst);
		}
		XU32 GetCount(){return m_pShows->GetSize();}
		void* GetData(XU32 i){_TAGSHOW&show=(*m_pShows)[i];
							  return (void*)((show.nPlace<<16)|show.nCode);}
		SHOWMAP* m_pShows;
	};	
	SHOWMAP	 m_showMaps;
	void AddMap(_TAGSHOW&show)
	{
		XSortMap sort(&m_showMaps);
		sort.AddMap(show);
	}
	inline int FindShowMap(int nCode,int nPlace)
	{
		if(m_showMaps.GetSize()<=0) return -1;
		XSortMap sort(&m_showMaps);
		int id=sort.FindMap(nCode,nPlace);
		if(id<0)// return id;
		{
			if(nPlace)
				id=sort.FindMap(nCode,0);
		}
		
		return id;
	}
	//enum {LOW,MID,HIGH,AUTO};
    //enum {GAME,MEDIA};
	XString8 m_strContentID;
	XString8 m_strADURL;
	XString8 m_strEndURL;
    //XU8      m_nContentType;
	XU16	 m_nContentLevel;
	XU16	 m_nMouseStep;
	XU16	 m_nLongLockTime;
	XU16     m_nZoomHotKey;
	XU16	 m_nAutoTag;
	XU16	 m_nTagPlace,m_nWaitID;
	//XString8 m_strTagPlace;
	XU8		 m_nADMode;
	//XU8		 m_nScreenDirect;
	XU8		 m_nZoomMode;
	XU8		 m_bMute,m_bButtons,m_bConfigOK,m_bMouseShow;
	XU8		 m_nVolume,m_nScoreType;
	XU8		 m_nSoundDevice;
	//XU8		 m_nTouchSize;
	XU8		 m_bACCMap;
	//XU8		 m_nQuality;
	//XU8		 m;
	float	 m_fMaxScore;
	XString8 m_strScoreVar;
	XString8 m_strScoreURL;
	XString8 m_strWaitFor;
	_XMOUSEMODE*m_pActiveMode;
	KEYMAPS m_keyMaps;
	BUTTONMAPS m_buttonMaps;
	MOUSEMODES m_mouseModes;
	int		 m_nAutoCap;
	ACCMAPS m_ACCMaps;
	
public:
	XDib* m_pGreenDib,*m_pGrayDib; //*m_pOnDib,*m_pOffDib;
	//XDib* m_pButtonDib[6];
	enum {XCUR_ARROW=0,XCUR_HAND,XCUR_INPUT,XCUR_MOVE};
	enum {SCOREVAR=0,SCOREOBJ=1};
	XImage m_cursor;
	void InitConfig();
	void Reset();
	XBOOL SetMouseMode(XPCTSTR strMode,XBOOL bForce=XFALSE);
	void FreeExtra();
	XConfig();
	virtual ~XConfig();
	static int _nCurData[4][4];
protected:
	XDib* CreateDib(XPCTSTR strName);
};

#endif // !defined(AFX_XCONFIG_H__9AEBC847_6FA8_43E0_A44E_57CF1AC205B7__INCLUDED_)
