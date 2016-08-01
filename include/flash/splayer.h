/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc.
****************************************************************************/

#ifndef XSWFPlayer_INCLUDED
#define XSWFPlayer_INCLUDED

// Define dragbuttons for now...
#define DRAGBUTTONS
#define ACTIONSCRIPT

#include "XXStack.h"
#include "PROPITEM.h"


#include "splay.h"
//#include "Bitbuf.h"
//#include <set>


#define MAXDELTA 32

#define FULLFONT
#define DOTFONT

#define _LOWSPEED

#define MaxExecActions 2000000//200000

#define XCMD_PROCFLASHTEXT 2000
#define XCMD_GETFLASHURL   2001
#define XCMD_FLASHCOMMAND  2002
#define XCMD_FLASHMODECMD  2003
#define XCMD_FLASHSCORE	   2004
#define XCMD_GETFLASHHTML  2005
#define XCMD_CLOSECLIENT   2006
#define XCMD_REMOVECLIENT  2007
#define XCMD_UPDATEOBJ	   2008
#define XCMD_UPDATECLICKOPT 2009
#define XCMD_FLASHERROR	   2010
#define XCMD_FLASHSHOWMODE 2011

#define XFLASH_QUIT		   1
#define XFLASH_CHANGEMODE  2
#define XFLASH_REFLASH	   3
//#if (__CORE_VERSION__>=0x02078000)
#define XFLASH_RELOAD	   4
#define XFLASH_POSTDATA    5
#define XFLASH_POSTID	  0xFFFFFF00
//#endif
#include "genericfonts.h"

#define ADLEVEL 999999

#ifdef EDITTEXT
#include "edittext.h"
#endif
#include "XConfig.h"	// Added by ClassView
#include "flashglobal.h"	// Added by ClassView

//dushixiong 2011-4-19 --start
#include "XNetWindow.h"
//dushixiong 2011-4-19 --end

int HexChar(int v);
int ParseHexChar(char c);

enum {
	streamProtocolHttp,
	streamProtocolFtp,
	streamProtocolLocalFile
};

/*! MenuStatus is filled and passed to the Native code in order to specify
 *	which menus should be displayed. Flash will make this call whenever the
 *	menu state changes. The first group (starting with view100percent) are
 *  boolean true if the option should be enabled on the menu. The second group
 *	(starting with view100Check) are true if the item should be checked.
 */
typedef struct _XMenuStatus
{
	BOOL view100percent;	//!< true if the movie can be set to 1:1 zoom
	BOOL viewAll;			//!< true if the movie can be stretched to fit
	BOOL zoomIn;			//!< true if the user can zoom in
	BOOL zoomOut;			//!< true if the user can zoom out
	BOOL play;				//!< true if the user can play a movie
	BOOL rewind;			//!< true if the current movie can be rewound
	BOOL forward;			//!< true if the user can step forward one frame
	BOOL back;				//!< true if the user can step backward one frame
	BOOL loop;				//!< true if the current movie can be looped

	BOOL view100Check;		//!< true if the movie is at 1:1 zoom
	BOOL viewAllCheck;		//!< true if the movie is stretched to fit the window
	BOOL highQChecked;		//!< true if high quality is on
	BOOL loopCheck;			//!< true if looping is on
	BOOL playCheck;			//!< true if playing
}MenuStatus;

/*! The structure EditMenuStatus is used to pass the state of an
	editable text field.
 */
typedef struct _XEditMenuStatus
{
	BOOL selectAll;		//!< true if the "Select All" menu item is enabled
	BOOL copy;			//!< true if the "Copy" menu item is enabled
	BOOL cut;			//!< true if the "Cut" menu item is enabled
	BOOL paste;			//!< true if the "Paste" menu item is enabled
	BOOL clear;			//!< true if the "Clear" menu item is enabled
	BOOL undo;			//!< Not enabled. Always false.
}EditMenuStatus;

struct SButton {
	_XSObject*	button;
	SRECT		buttonBounds;

	SButton() : button(NULL)
	{
		memset(&buttonBounds, 0, sizeof(buttonBounds));
	}

	SButton& operator=(const SButton& b)
	{
		button		 = b.button;
		buttonBounds = b.buttonBounds;
		return(*this);
	}

	SButton& operator=(const SButton* b)
	{
		if ( b ) {
			button		 = b->button;
			buttonBounds = b->buttonBounds;
		}
		return(*this);
	}
};

typedef struct _XKeyboard
{
public:
	enum {
		ID_KEY_LEFT        = 1,
		ID_KEY_RIGHT       = 2,
		ID_KEY_HOME        = 3,
		ID_KEY_END         = 4,
		ID_KEY_INSERT      = 5,
		ID_KEY_DELETE      = 6,
		ID_KEY_CLEAR       = 7,
		ID_KEY_BACKSPACE   = 8,
		ID_KEY_CUT		   = 9,
		ID_KEY_COPY		   = 10,
		ID_KEY_PASTE	   = 11,
		ID_KEY_SELECT_ALL  = 12,
		ID_KEY_ENTER	   = 13,
		ID_KEY_UP		   = 14,
		ID_KEY_DOWN		   = 15,
		ID_KEY_PAGE_UP	   = 16,
		ID_KEY_PAGE_DOWN   = 17,
		ID_KEY_TAB		   = 18,
		ID_KEY_ESCAPE	   = 19
	};
	enum {
		ID_KEY_SHIFT	   = 0x01,
		ID_KEY_CTRL		   = 0x02,
		ID_KEY_ALT		   = 0x04
	};
}Keyboard;

enum {
	CURSOR_TYPE_ARROW,		// the arrow
	CURSOR_TYPE_HAND,		// a picture of a hand
	CURSOR_TYPE_BUTTON,		// a hand with a button finger extended
	CURSOR_TYPE_BEAM,		// a bar for text editing
	CURSOR_TYPE_HIDE,
};

/*! The parent class of NativePlayerWnd, XSWFPlayer deals with events (mouse, keyboard), drawing,
 *	cursors, etc. at a "higher" OS-independant level. It aggregates the ScriptPlayer, DisplayList,
 *	and CBitBuffer, acting as a sort of "main" object.
 */
/*struct _XKeyword
{
	char* strKey;
	XU32  nHash;
	_XKeyword* pNext;
};
#define XKEYS_NUMS 0x100
#define XKEYS_MASK 0xff*/
#define MAXDEPTH 1024
class XXObject;
class XSortMemName;
class XActionScript;
//#ifdef _MultiThread_
class XDibSrv;
//#endif
#define CLIPTHREAD(OBJ) (OBJ?(OBJ->m_pObject?OBJ->m_pObject->thread:XNULL):XNULL)
#define CLIPOFOBJECT(T) (XXObjectClip*)(T->thread->rootObject->pObject)
#define CLIPOFTHREAD(T) (XXObjectClip*)(T->rootObject->pObject)
#define THREADOF(OBJ)   (OBJ->thread->rootObject)
#define CLIPOF(OBJ) (XXObjectClip*)(OBJ->thread->rootObject->pObject)
#define OBJOFTHREAD(T)  (XXObjectClip*)(T?T->rootObject->pObject:XNULL)
//#define TOSOBJECT(OBJ) (OBJ->m_pObject?)(OBJ->m_pObject:XNULL)

typedef XVectorFast<XPCTSTR> XCONSTVECTOR;


struct _VARCACHE
{
	XXObject*pObject;
	XXObject*pObjectThis;
	struct XXTOKEN *pToken;
	XU32	 nRefID;
	int		 nEventID;
};

namespace avmshell
{
	class EventDispatcherObject;
	class StageObject;
};
using namespace avmshell;

class XSWFCONTEXT
{
public:
	/*XXObject* GetBlock(XBOOL bThis=XTRUE)
	{
		XXObject* p=blocks.GetLast();
		//if(!p)
		//	p=locals.GetLast();
		if(bThis&&!p)
		{
			p=contexts.GetLast()->rootObject->pObject;//GetThis();
			//if(!p)
			//	p=GetContaint();
		}
		//if(pBlock) return pBlock;
		return p;
	}*/
	/*XXObject* GetLocal()
	{
		return m_contexts
	}*/
	int nCallDepth;
	XCONSTVECTOR constants;
#ifdef _VARCACHE_
	//XVectorFast<XU16> caches;
	_VARCACHE		  varCache;
#endif
	//XXStack	   constants
	XXStack	   regObjs;
	//XVector<XXObject*> blocks;
	class XSWFPlayer* pPlayer;
	enum { maxURLs = 32 };
	int nDepth;
	//char* actionURLs[maxURLs];
	//char* actionTargets[maxURLs];
	//char* actionTargets[maxURLs];
	//char* actionPostData[maxURLs];
	//int   actionVars[maxURLs];
	//XXObject*actionObjs[maxURLs];
	//ActionContext* context;
	XBOOL /*enteringCall,exitingCall,*/actionOverflow;
	int actionCount;
	//XXStack* pStack;
	XXStack	  stacks,*pStack;
	XXObject* pOwer,*pToken;
	XXObject* pWith;
	XXObject*baseThread,*thread;
	XXObject*pThis,*pObject;
	XActionScript*pCaller;
	XSWFCONTEXT():regObjs(16)
	{
		pStack=&stacks;
	}
	void InitContextA(XActionScript*ppCaller,
					 XXObject*ppBaseThread,
					 XXObject*ppThread,
					 XXObject*ppThis,
					 XXObject*ppObject,
					 XXObject*ppWith)
	{
		pWith=ppWith;
		baseThread=ppBaseThread;
		thread=ppThread;
		pThis=ppThis;//(XXObject*)(ppThis?ppThis:thread);//->rootObject->pObject;
		pObject=ppObject;
		pCaller=ppCaller;
	}

	//XVector<XXObject*> thises;
	//XVector<XXObject*> withs;
	//XVector<XXObject*> locals;
	//XVector<XXObject*> varSeekPath;
	//XVector<ScriptThread*> contexts;
	//ScriptThread*	pTarget;
	//ScriptThread* GetTarget()
	//{
	//	if(pTarget) return pTarget;
	//	return contexts.GetLast();
//	}
//	XXObject* GetTargetObj()
//	{
//		return GetTarget()->rootObject->pObject;
//	}
/*	ScriptThread* SetContext(ScriptThread*pThread,XXObject*pThis)
	{
		contexts.Add(pThread);
		thises.Add(pThis);
		pTarget=XNULL;
	}
	void RestoreContext(ScriptThread*p)
	{
		contexts.RemoveLast();
		thises.RemoveLast();
		pTarget=p;
	}*/
//	ActionList*	   actionList;
public:
	SObject* GetWithObject();
	//XBOOL NeedLowCase();
	//SObject* GetWithThread();
	void MakeConstant(XPCTSTR pBuf, XU16 nCount);
	XBOOL SaveRegister(int id, const XXVar&var)//XXObject*pObject)
	{
		if(id<(int)regObjs.GetSize())
			regObjs[id]=var;
		else return XFALSE;
		return XTRUE;
		//SETOBJ(regObjs[id],pObject);
	}
	XBOOL LoadRegister(int id,XXVar&var)
	{
		if(id<(int)regObjs.GetSize())
			var=regObjs[id];
		else
			return XFALSE;
		return XTRUE;
		//return regObjs[id]->REF();
	}
	//XXObject* GetThis();
};

#define ButtonLock()
#define ButtonUnlock()
class XXObjectFunction;

extern void DecodeQueryString(XSWFCONTEXT&cnt,XXObject *thread, char *str);
//const XPCTSTR _strQuality[]={"LOW","MEDIUM","HIGH"};
typedef XVector<XXObjectFunction*> XFUNLIST;
class XActionScript;
extern int Number(XPCTSTR name);

#ifdef _WINEMU
extern XSWFPlayer* _tracePlayer;
#endif


struct _BUTTONDRAWINFO
{
	XDib*  pDraw;
	XPOINT pt;
	XRECT  rect;
	XDib*  pLight;
	float  alpha;
};
struct _SHAPEIMAGES
{
	SCharacter* ch;
	_SHAPEIMAGES* next;
};
typedef XArray<_BUTTONDRAWINFO> _BUTTONS;
class XSWFPlayer
{
public:
	int m_nDelayShow;
	_SHAPEIMAGES* m_pShapeImages;
	//_SHAPECACHES* m_pShapeCaches;
	//void AddShapeCache(_XSObject*obj)
	//{
	//	_SHAPEIMAGES* s=(_SHAPEIMAGES*)XXVar::_AllocData(sizeof(_SHAPEIMAGES));
	//	s->ch = (SCharacter*)obj;
	//	s->next = m_pShapeCaches;
	//	m_pShapeCaches = s;
	//}
	//void ReleaseAllCaches()
	//{
	//	_SHAPEIMAGES* pImages=m_pShapeCaches;
	//	while(pImages)
	//	{
	//		_SHAPEIMAGES*pNext = pImages->next;
	//		//((_XSObject*)pImages->ch)->FreeCacheBmp();
	//		XXVar::_FreeData(pImages);
	//		pImages = pNext;
	//	}
	//	m_pShapeImages = NULL;
	//}
	void AddShapeImage(SCharacter*ch)
	{
		_SHAPEIMAGES* s=(_SHAPEIMAGES*)XXVar::_AllocData(sizeof(_SHAPEIMAGES));
		s->ch = ch;
		s->next = m_pShapeImages;
		m_pShapeImages = s;
	}
	void ReleaseAllShapeImages()
	{
		_SHAPEIMAGES* pImages=m_pShapeImages;
		while(pImages)
		{
			_SHAPEIMAGES*pNext = pImages->next;
			//if(pImages->ch&&pImages->ch->record.m_pImage)
			//	pImages->ch->record.ReleaseAll(true);
			XXVar::_FreeData(pImages);
			pImages = pNext;
		}
		m_pShapeImages = NULL;
	}
public:
	XArray<ScriptPlayer*> m_newDatas;
	bool m_bDataProc;
	void SetDataFlag(){if(m_newDatas.GetSize())m_bDataProc=true;}
	void DataStart(int nMax);
	void DataSet(int nProc);
	void DataEnd();
	_BUTTONS m_buttons;
	void ResetButtons()
	{
		m_buttons.RemoveAll();
	}
	void AddPaintButton(XDib*pDraw,XPOINT&pt,XRECT&rt,float alpha,XDib*pLight)
	{
		_BUTTONDRAWINFO info=
		{
			pDraw,pt,rt,pLight,alpha
		};
		m_buttons.Add(info);
	}
	void Repaint()
	{
		display.ModifyCamera(false);
	}
	bool Reload();
	bool IsJQBar(XPCTSTR strURL);
	bool RunSWF(XPCTSTR name);
//#ifdef _MultiThread_
public:
	//XDibSrv* m_pDibSrv;
	bool GetObjBitmap(SObject*pObj,XDrawDib&dib,bool bAS);
//#endif
public:
	//void FullScreen();
	XBOOL m_bDataOnly;

#ifdef _WINEMU
public:
	void GetPlayerList(XU32Array&list);
#endif

private:
	XBOOL m_bNeedEnter;
	XU32 m_iEnterCount;

	void* m_pDownTarget;
	void* m_pUpTarget;

public:
	XBOOL m_bDoFrame;
	XBOOL m_bResetMode;
	void ResetStageMode()
	{
		m_bResetMode=XTRUE;
	}
	void ReleaseInfo(void*info);
	XBOOL	   m_bEnterFrame;
	void SetEnterFrame()
	{
		if(!m_pDomFilter||!m_pDomFilter->bDispatchLater) return;
		m_bEnterFrame=XTRUE;
	}
	void ClearEnterFrame(){m_bEnterFrame=XFALSE;}
	_XSObject* m_pUpObject;
	_XSObject* m_pUpRemovedObject;
public:
	
	void UpdateDragDib();
	void Interrupt();
	bool InterruptIfTimeout();
	static XSWFPlayer* m_pInstance;
	void SetMoveDirect();
	int	m_nFrameWidth,m_nFrameHeight;
	ScriptThread* m_pNextDoThread;
public:
	XFastAlloc charAlloc;
	XFastAlloc eventAlloc;
	XFastAlloc filterAlloc;
	_XFilter* CreateFilter(){return (_XFilter*)filterAlloc.Alloc();}
	void FreeFilter(_XFilter*filter){filterAlloc.Free(filter);}
	inline _XSCharacter* CreateCharacter()
	{
		_XSCharacter*ch=(_XSCharacter*)charAlloc.Alloc();
		//ch->useCount = 0;
		ch->placeCount = 0;
		return ch;
	}
	void FreeCharacter(_XSCharacter*ch);
	void ReleaseData(XU8*&data){}
	bool m_bCancelNext;
	XU32 m_nRunTime;
	SBitmapCore* m_bitmaps;
	XXVar m_strWorkCache;
	bool AS3CreateSObject(_XSObject*&obj,int type,void*pInstance,SCharacter*ch,ScriptPlayer*player);
	void InitCachePath()
	{
		m_strWorkCache=m_strWorkPath;
		m_strWorkCache.StringAdd("/Memory/");
		XFile::CreateFolder(m_strWorkCache.strTxt);
	}
	inline void AddBitmap(SBitmapCore*bitmap)
	{
		SBitmapCore*bits=m_bitmaps;
		//bitmap->m_nLockTime=m_nRunTime;
		bitmap->m_pNext=m_bitmaps;
		m_bitmaps=bitmap;
	}
	inline void RemoveBitmap(SBitmapCore*bitmap)
	{
		SBitmapCore**bits=&m_bitmaps;
		for(;;)
		{
			SBitmapCore*b=*bits;
			if(!b) break;
			if(b==bitmap)
			{
				*bits=b->m_pNext;
				return;
			}
			else
				bits=&b->m_pNext;
		}
	}
	enum {MV_NONE,MV_LEFT,MV_TOPLEFT,MV_DOWNLEFT,MV_RIGHT,MV_TOPRIGHT,MV_DOWNRIGHT,MV_TOP,MV_DOWN,MV_RET};
	enum {LK_NONE,LK_SELECT,LK_OBJECT};
	int    m_nDestX,m_nDestY,m_nMidX,m_nMidY,m_nMode;
	int    m_nLimitX,m_nLimitY,m_nStageWidth,m_nStageHeight;
	int    m_nLockMode;
	double m_fScaleX,m_fScaleY;
	void SetMoveMode(int nMode);
	bool StopMove();
	void DoMoveMode();
public://for LocalConnect
	XU32 m_nLoopCount;
	bool m_bUpdateCommand;
	inline void AS3UpdateCommand(){m_bUpdateCommand=true;}
	inline void AS3ClearCommand(){m_bUpdateCommand=false;}
	inline void AS3DoCommand();
	int	m_nCollectCount;
#ifdef _ANALY
	XEvent		 m_getEvent;
	AS3PROPITEM* m_pReturnItem;
	void*		 m_pRetObject;
	XSortMemName*m_pMemInfo;
	XSeekItem*	 m_pSeekItem;
	void GetClassTree(XString8&strTree,void*pObject);
	AS3PROPITEM* AS3GetChildren(void*pASObject,bool bSuper=false);
	void*		 AS3FindClass(const char*strClass,void*pInstance);
	_DIBINFO*	 AS3GetDibInfo();
	void		 AS3GetMemInfo(XSortMemName*pInfo);
	void		 GetMemInfo(XSortMemName*pInfo);
	void		 GetDibInfo(_DIBINFO*pInfo);
	void		 GetGLDib(_DIBINFO*ch);
	void		 GetGLDibInfo(XDrawDib*pDib,_XSCharacter*ch);
	void		 AS3SeekItem(XSeekItem*pItem);
	void		 SeekItem(XSeekItem*pItem);
	_DIBINFO*	 m_pReturnInfo;
#endif
	ScriptPlayer* pAS3Player;
	void AS3PushPlayer(ScriptPlayer*p)
	{
		p->pNextPlayer=pAS3Player;
		pAS3Player=p;
	}
	void AS3ReleasePlayer(ScriptPlayer*pp)
	{
		if(pp==this->player)
		{
			return;

		}
		if(!pAS3Player) return;
		ScriptPlayer**p=&pAS3Player;
		ScriptPlayer*pl=*p;
		while(pl)
		{
			pl=*p;
			if(pl==pp)
			{
				*p=pp->pNextPlayer;
				break;
			}
			p=&pl->pNextPlayer;
		}
		delete pp;
	}
	void AS3ReleasePlayer()
	{
		while(pAS3Player)
		{
			ScriptPlayer*next=pAS3Player->pNextPlayer;
			delete pAS3Player;
			pAS3Player=next;
		}
	}
	/*SObject* pAS3Objects;
	void AS3ReleaseObjects()
	{
		while(pAS3Objects)
		{
			SObject*pNext=pAS3Objects->pNext;
			pAS3Objects->FreeChildren();
			pAS3Objects->FreeContext();
			display.FreeObject(pAS3Objects);
			pAS3Objects=pNext;
		}
	}

	void AS3RemoveAndClear(SObject*obj)
	{
		if(!pAS3Objects) return;
		if(!AS3Remove(obj)) return;
		obj->FreeChildren();
		obj->FreeContext();
		display.FreeObject(obj);
	}

	void AS3Push(SObject*obj)
	{
		if(pAS3Objects)
			pAS3Objects->pPrev=obj;
		obj->pNext=pAS3Objects;
		obj->pPrev=NULL;
		pAS3Objects=obj;
	}
	bool AS3Remove(SObject*obj)
	{
		if(pAS3Objects==NULL) return false;


		//if(!obj->above) return false;
		if(obj->pPrev==NULL)
		{//first
			pAS3Objects=obj->pNext;
			obj->pNext=NULL;
			obj->pPrev=NULL;
			if(pAS3Objects)
				pAS3Objects->pPrev=NULL;
		}
		else
		{
			obj->pPrev->pNext=obj->pNext;
			if(obj->pNext)
				obj->pNext->pPrev=obj->pPrev;
			obj->pPrev=NULL;
			obj->pNext=NULL;
		}


		return true;
	}*/
	struct _CONNECTION
	{
		XXVar strDomain;
		XXVar strName;
		int	  nType;
		void* pClass;
		//ScriptPlayer*root;
		void* pContext;
	};
	XArray<_CONNECTION*> m_connections;
	bool AS3Connect(const char*name,void*p,int t,void*pContext);
	bool AS3Close(void*p);
	void AS3Release();
	bool AS3Send(const char*name,const char*method,void*args);
public:
	void*	m_pLoaderInfo;
	//StageObject*m_pStage;
	_XLoadInfo* pLoader,*pLastLoader;
	_XLoadInfo* AddLoader(const char*url,const char*target,
				   int nType=0,XXObject*pObj=NULL,EventDispatcherObject*pASObject=NULL,const char*context=NULL);
//#if (__CORE_VERSION__>=0x02075000)
	bool RemoveLoader(XU32 nID);
//#endif
	XBOOL	    RemoveLoader(const char*url);
	avmshell::XAVM2* m_pAVM2;
	_XSObject* m_pActiveObject,*m_pActiveTarget;
	void AS3SetKeyFocus(SObject*,SObject*,int nCode);
	inline void AS3InitMouseData(MOUSEEVENTDATA&data,SObject*pObj,bool bDown,void*pTarget,void*pRel,_XSObject*pStop);
	inline void AS3InitKeyData(KEYEVENTDATA&data,int nChar,int nKey,int nLocation,SObject*pObj);
	//void* AS3CreateMouseEvent(int id,_XSObject*pObj,bool bDown,void*pTarget);
	//void* AS3CreateKeyboardEvent(int id,int nChar,int nKey,int nLocaltion,void*pTarget);
	//void* AS3CreateFocusEvent(int id,int nKey,void*r,void*pTarget);

	//enum {XLS_KEY,XLS_MOUSE,XLS_STAGE};
	XConfig* GetConfig(){return&m_config;}
	XDOMFILTER*GetFilter(){return m_pDomFilter;}
	void SetFilter(XDOMFILTER*pFilter){m_pDomFilter=pFilter;}
	XDOMFILTER* m_pDomFilter;
	void*		m_pDom;
//#ifdef _DEBUG
//	static XU32 _actionTime[0xffff];
//	static XU32	_actionCount[0xffff];
//#endif
protected:

/*	struct XHandler
	{
		XXObject*		pFunction;
		XXObjectClip*	pThread;
		XXObject*		pThis;
		XXObject*		pCaller;
		XHandler*		pNext;
		XXVARLIST		args;
	};
	XHandler* m_handlers;*/
	//XFUNLIST m_listeners[XLS_STAGE+1];
//	_XKeyword* m_keys[XKEYS_NUMS];
	struct TimeCaller
	{
		XU32 nTimer;
		XU32 nLastCall;
		XXObject* pFunction;
		XXObject* pBase;
		XXVar     strMethodName;
		void*	  pArguments;
		void*	  pContext;
//#if (__CORE_VERSION__>=0x02077000)
		XU32	  nInit;
//#endif
		//XDTSpriteDisp*pContext;
		//ScriptThread*pThread;
		XU32 nID;
		int nType;
		XXVARLIST args;
		TimeCaller*pNext;
		TimeCaller():args(16)
		{
			nType=0;
		}
	};
	TimeCaller*m_callList;
	TimeCaller*m_freeList;
	//XVector<TimeCaller*> m_callList;
	//XVector<TimeCaller*> m_freeList;
	void FreeCalls();
	void ReleaseCallers();
public:
	XU8   nSkipCount,nSkipsNow,nCacheCount;
	XBOOL PostScore()
	{
		if(!pWindow) return XFALSE;
		return pWindow->PostMsg(XCMD_FLASHSCORE,0,0,XMS_THIS);
	}
	//inline XPCTSTR GetQuality()
	//{
	//	return _strQuality[nQuality];
	//}
	void CalcLimit();
	void SetQuality(XFDOUBLE v)
	{
		//if(m_config.m_nQuality!=XConfig::AUTO) return;
		//if(v>=3) nQuality=XConfig::HIGH;
		//else if(v>0) nQuality=XConfig::MID;
		//else nQuality=XConfig::LOW;
		//bestDither=v>1.5;
		//autoQuality = XFALSE;
		//lastQualitySwitch=0;
		SetCamera(updateLazy);
		CalcLimit();
	}
	void SetQuality(XPCTSTR q)
	{
		//if(m_config.m_nQuality!=XConfig::AUTO) return;
		switch(q[0])
		{
		case 'h':
		case 'H':
		case 'm':
		case 'M':
			SetQuality(1.0F);break;
		default:
			SetQuality(0.0F);break;
		}
	}
	XWindow* GetWindow(){return pWindow;}
	/*XXObjectClip* GetObject()
	{
		return player->GetObject();
	}*/
	//XFUNLIST& GetListeners(XU8 nMode)
	//{
	//	return m_listeners[nMode];
	//}
	enum {LOAD_OK,LOAD_AGAIN,LOAD_FAILED,LOAD_END,LOAD_BREAK,LOAD_INITOK};
	enum {PLAY_OK,PLAY_BREAK,PLAY_END,PLAY_WAIT,PLAY_WAITSHORT,PLAY_HITNONE};
	void GetShowSize(XSIZE&sz)
	{
		sz.cx=m_winSize.cx;
		sz.cy=m_winSize.cy;
		//if(bits.IsEmpty()) return;
		//sz.cx=bits.width();
		//sz.cy=bits.height();
	}
	/*XU32 MakeHash(char* str,XBOOL bLowCase)
	{
		XU32 nHash=0;
		int id=0;
		while(str[id])
		{
			if(!bLowCase)
				nHash+=(str[id]<<id);
			else
			{
				char c=str[id];
				if(c>='A'&&c<='Z') c+='a'-'A';
				nHash+=c;
			}
			id++;
		}
		return nHash;
	}*/
   	XBOOL IsStopEvent()
	{	return m_hStop.IsEvent(0);	}
	void Stop()
	{	//StopPlay();
		m_hStop.SetEvent();	}
	// Our player Objects
	ScriptPlayer* player;		// note that this is always layer #0 and the root of our layer list
	DisplayList display;
	//XBitBuffer bits;
	XBOOL IsEmpty()
	{
		return !this->player->m_bInitOK;
	}
#ifdef SOUND
		#ifdef ONE_SOUND_OBJECT
			static XSoundMix theSoundMix;		// there will only be one NativeSoundMix
		#else
			XSoundMix theSoundMix;				// one NativeSoundMix per XSWFPlayer
		#endif
#endif

	//BOOL highQuality;
	int nQuality;
	BOOL autoQuality;
	int lastQualitySwitch;
	//BOOL bestDither;
	BOOL bAdJustWin;
//	BOOL showMenu;

	// Sound prebuffer property
	int nSoundBufferTime;

	// Counters for autoquality settings
	int nTotalCost,nTotalCount;
	int nTotalNeed;
//	int nTooSlow;
//	int nTooFast;
//	int nTotal;
 	//int starved;

	// Streaming (netscape) code
	BOOL firstStream;	// true if we have not gotten a NewStream callback yet

	int mode;
	//char* url;
	//char* urlBase;
	//char* name;
	XXVar strURL;
	XXVar strURLBase;
	XXVar strBaseVar;
	XXVar strName;
//	SObject* hitObj;
	// View Control
	//int scaleMode;
	//SRECT zoomRect;
	//int	  m_nMaxPecent;
	//int  mnZoomPecent;  // from 1 - 2

	SPOINT scrollPt,cameraPt,downPt;
	XU32 scrollTime,nDownTime;
	BOOL scrolling,downling,isMoveScroll;
	BOOL destructing;


	//BOOL actionOverflow;

#ifdef EDITTEXT
	// Text editing
	_XSObject* focus;
	SaveFocus saveFocus;
	int  cursorTime;
	BOOL cursorBlink,cursorShow,menuShow;
	BOOL IsSelecting();
#endif
	GLColor fontColor;
#if defined(EDITTEXT)

#ifdef FULLFONT
	_XSCharacter buildInFont;
#ifdef DOTFONT
	//	,dotFont16,dotFont12;
	//XBOOL bDotFontOK;
#else
#endif
	BOOL LoadBuiltInFonts();
	//XBOOL LoadFont(XU16*text);
	//XResource	 m_fontFile;
#else
	ScriptPlayer* builtInFontsPlayer;
	enum {
	  kBuiltInFontsNotLoaded,
	  kBuiltInFontsLoaded,
	  kBuiltInFontsError
	} builtInFontsState;
	BOOL LoadBuiltInFonts();
#endif
#endif

#ifdef DRAGBUTTONS
	_XSObject* dragObject;
	BOOL	 dragStarted;
	SPOINT   dragPoint;
#endif

	// Action control
	XU8	m_align;
	int actionDepth;
	MATRIX m_lockMatrix;
	// Background playing control
	//BOOL syncToClock;	//	set if we should be skipping frames to keep the animation tightly synchronized to the clock
	S32 nextFrameTime;	// the tick value to display the next frame

	BOOL loaded,bError;		// set when the first frame is loaded
	BOOL running;		// set if the player is running

	// cached mouse info
	int mouseState,gmouseState;			// -1 = no inited, 0 = mouse up, 1 = mouse down
	SPOINT mousePt,gmousePt;
	//int nKeyCode[256],nKeyStatus;
	XU8 nKeyStatus[256];
	int nLastKey;
	S32 mTimerOffset;
	XBOOL IsKeyDown(int k)
	{
		if(k<0||k>=256) return 0;
		return nKeyStatus[k];
	}
public:
	MATRIX m_beginMat;
	bool   m_bZooming;
	SRECT  m_startArea;
	void   StartZoom(SRECT&r);
	int	   SetZoom(SRECT&r);
	void   EndZoom();
	void   UpdateAll();
public:
//#ifdef _LOWSPEED
//	BOOL LockZoomBitmap();
//	BOOL UnlockZoomBitmap(bool bUpdate);
//	int		 m_nMulti0;
//	XDrawDib m_dragDib;
//	SPOINT   m_dragOffset;
//	bool	 m_bDragOrZoom,m_bUpdateDrag,m_bNoScript;
//#endif
	int m_nDomSize;
	XString8Array m_argList;
	XBOOL OnAS3Event(void*pTarget,void*pEvent,void*);	
	XBOOL AddEvent(int type,int x,int y,int s,int t=0,bool bFirst=false);
	XBOOL ModifyEvent(int type,int x,int y,int s);
	XBOOL RemoveEvent(int type,int x,int y,int s);
	//XBOOL PushHandler(XXObjectFunction*pFunction,XXObjectClip*pThread,XXObject*pThis,XXObject*pCaller,XXVARLIST*pAags);
	XBOOL RemoveEditText(XSWFCONTEXT&cnt,char* strName,EditText*pEdit);
	int TableIndex(SObject*pObj);
	XBOOL RemoveVariable(XSWFCONTEXT&cnt,XPCTSTR strName);
	int GetStageHeight();
	int GetStageWidth();
	int GetPecentBase();
	void ShowMenu(XBOOL bShow);
	void SetScaleMode(XPCTSTR strMode);
	XPCTSTR GetScaleMode(int sm=-1);
	void ShowCursor(XBOOL bShow);
	ScriptThread* ThreadOf(XSWFCONTEXT&cnt,SObject*thisObj,XXVar*pVar,ScriptThread*thisThread);
	SObject*	  ObjectOf(XSWFCONTEXT&cnt,SObject*thisObj,XXVar*pVar,ScriptThread*thisThread);
	XBOOL GetURL2(XSWFCONTEXT&cnt,ScriptThread*thread,char* baseURL,char*pTarget,XU8 method);
	XBOOL GetURL3(XSWFCONTEXT&cnt,char*baseURL,char*pTarget,XU8 method,XXObject*pObj,char*postData);
	XBOOL SetGlobal(XPCTSTR name,XXVar&var,XBOOL bLowCase);
	int	  GetGlobal(XPCTSTR name,XXObject*pFun,XXObject*pThis,XXObject*pObject,XXVar&var);
	XXObject* ResolveVariableByDot(XSWFCONTEXT&cnt,const XXVar&name,XXObject*base,XXVar&varName,XBOOL&bFirst);
	//void ReleaseListeners();
	//void RemoveListener(XXObject*pObject,XU8 nMode);
	//void AddListener(XXObject*pObject,XU8 nMode);
	XU32 AddCaller(XXObject*pBase,XXObject*pFunction,const XXVar&method,XU32 nTimer,XXVARLIST&list,int nType=0);
	XU32 AS3AddCaller(void*pFunction,XU32 nTimer,void*pArgument,int nType,void*pContext);
	inline bool AS3PostEvent(int id,int type,bool bMask);
	XBOOL RemoveCaller(XU32 id);
	void StopDrag();
	void StartDrag(SObject*pObject,SRECT&rect,XBOOL bCenter,XBOOL bRect);
	int GetMousePos(SObject*pObj,double&x,double&y);
	void CallFrame(XSWFCONTEXT&cnt,ScriptThread*thread,XXVar*pVar,int pos);
	//XBOOL SetZoom(XINT nZoom,XSIZE&sz);
	//XBOOL SetZoomPecent(int nPecent,XSIZE&sz,SPOINT&center,int nLockCenter=0);//bool bLockCenter=false);
	//XBOOL LockCenter(SPOINT&center,MATRIX&mat);
	//XBOOL NewCenter(SPOINT&center);
	//XBOOL Paint(XGraphics&g,int dx,int dy,XRECT&clip);
	//char* KeywordOfIndex(XINT id);
	//XINT IndexOfKeyword(char* strKey,XBOOL bAdd,XBOOL bLowCase);
	//void AdjustWindow(int w,int h);
	//void InvalidateScreenArea(const XRect&rect);
	//void Paint(void*,int dx,int dy,int cx,int cy,int ox,int oy);
	void InitPlayer(XSIZE&winSize, XWindow*pWin);
	void CloseStream(StreamData*pStream);
	bool CreateIfEmpty(SCharacter*ch,bool bTexture=true);
	XSWFPlayer(int nDeltaX,int nDeltaY);
	virtual ~XSWFPlayer();

	void SetVersionVariable( ScriptPlayer *player );
	XU32 GetVersion(){return 10;}
	enum { updateNone, updateNow, updateLazy };
	void SetCamera(int update);
	void FreeLayers();
	void ClearScript();
	void GotoFrame(int);
	int CurrentFrame() { return player->GetFrame(); }
	void OnString(XPCWSTR buf)
	{
		AddEvent(XSTRING,(int)buf,0,0);
	}
	void OnChar(XWCHAR c)
	{
		AddEvent(XCHAR,c,0,0);
	}
	void OnKeyDown(int nKeyCode,int nCharCode,int nLocation)
	{
		/*if(key>=0&&key<=0xff)
		{
			if(nKeyStatus[key])
				return;
		} */
		AddEvent(XKEYDOWN,nKeyCode,nCharCode,nLocation);
	}
	void OnKeyUp(int nKeyCode,int nCharCode,int nLocation)
	{
		AddEvent(XKEYUP,nKeyCode,nCharCode,nLocation);
	}
	void OnMouseMove(int x,int y,int iPos,int s);
	//{
	//	AddEvent(XMOUSEMOVE,x,y,s|(iPos<<16));
	//}
	void OnMouseDown(int x,int y,int iPos)
	{
		//if(y<100)
		//	int v=0;
		AddEvent(XMOUSEDOWN,x,y,iPos);
	}
	void OnActivate(){AddEvent(XACTIVE,0,0,0);}
	void OnDeactivate(){AddEvent(XDEACTIVE,0,0,0);}
	void OnMouseUp(int x,int y,int iPos)
	{
		AddEvent(XMOUSEUP,x,y,iPos);
	}
	void OnClick(int x,int y,int iPos,int t)
	{
		AddEvent(XMOUSECLICK,x,y,iPos,t);
	}
	void OnDoubleClick(int x,int y,int iPos)
	{
		AddEvent(XMOUSEDOUBLECLICK,x,y,iPos);
	}

protected:
	#ifdef EDITTEXT
	BOOL TabKeyDown(int key, int modifiers);
	void BlinkCursor(S32 time);
	BOOL HandleKeyDown(XU32 t,int key, int nChar,int nLocation,int modifiers,XBOOL bChar=XFALSE);
	void HandleKeyUp(XU32 t,int key, int nChar,int nLocation,int modifiers);
	#endif
public:

	/*! Deletes the bitmap backing the window, invalidates the display. Flash will
	 *  rebuild the bitmap with the current screen depth and palette.
	 */
	void FreeBuffer();
	BOOL UpdateBuffer(BOOL render = true);
//	void UnlockBuffer() { bits.UnlockBits(); }

	ScriptPlayer* OpenLayer(int layerDepth, BOOL loadVars=false);
    ScriptPlayer* OpenSprite(ScriptThread *targetThread);
	ScriptPlayer* OpenVariableLoader(int layer,XXVar&var);
	void ClearLayer(int layerDepth);

	XBOOL DoActions();


	//void Play(BOOL rewind=true) { player->Play(rewind); }
	//void StopPlay() { player->StopPlay(); }

	void GCLeave();

	XINT DoPlay(XRect&rect,BOOL wait);	// a routine to call repeatedly in response to timer messages or null events
	_XSObject* SetMyFucos(_XSObject*target);
	_XSObject* DoButton(_XSObject*&target,SPOINT* pt, int mouseIsDown/*0-up,1-down,2-release*/, XU32 t,BOOL updateScreen = true);

	/*! MouseMove is a method provided for use by the native code. <br>
	 *  MouseMove should be called whenever the mouse position needs to be updated.
	 *	(The actual frequency should be often enough to update the mouse, but not so often as to
	 *	clog the system.) It should be called irrespective of whether the mouse button is down.
	 */
protected:
	void MouseMove(XU32 t, int x, int y, BOOL mouseIsDown,BOOL isRepeat);

	/*! MouseDown is a method provided for use by the native code. <br>
	 *	MouseDown should be called when the mouse button is pressed.
	 */
	bool MouseDown(XU32 t, int x, int y );

	/*! MouseUp is a method provided for use by the native code. <br>
	 *	MouseUp should be called when the mouse button is released.
	 */
//#if (__CORE_VERSION__>=0x02070100)
	void MouseUp(XU32 t, int x, int y,XBOOL bWithClick );
//#else
//	void MouseUp(XU32 t, int x, int y );
//#endif
	void MouseClick(XU32 t,int x,int y);
	void MouseDoubleClick(XU32 t,int x,int y);
public:
	void SetFilePath(XPCTSTR strPath);
	void AS3SetFocus(_XSObject*focus);
	static void ReleaseGlobal();
	SObject* GetRootObject();
	void GetEditTexts(SObject*root,XSObjectArray&list);
	void Pause(XBOOL bPause);
	XXVar m_strWorkPath;
	XString8 m_strFilePath;

	//XString8 m_strAppName;
	//void Restart();
	XBOOL MoveView(XPOINT&delta);
	BOOL TabDirect(int key);
	void SetAutoFit(int w,int h);
	void UpdateConfig();
	bool NeedOriginal();
	bool NeedMove();
	//void SetConfig(XConfig*pConfig);
	void SetDevSize(XSIZE&sz,int nZoom);
	/*! CancelCapture is a method provided for use by the native code. <br>
	 *	CancelCapture should be called if the mouse capture needs to be aborted for
	 *	some reason. outsideWindow is true if the mouse has left the window. (Elvis
	 *	has left the building.)
	 */
	void CancelCapture(BOOL outsideWindow = true);

	//void Home();
	//void Zoom(SPOINT size, SPOINT center);
	//void ZoomF(SFIXED factor, SPOINT* center = 0);
	////void Zoom(BOOL enlarge, SPOINT* center) { ZoomF(enlarge ? fixed1/2 : fixed1*2, center); }
	//void Zoom(SRECT* r);
	//void Zoom100();
	//void ZoomScale(SFIXED scale);

	// Action Helpers
	//void Rewind() {GotoFrame(0);}
	//void Forward() {GotoFrame(CurrentFrame()+1);}
	//void Back() {GotoFrame(CurrentFrame()-1);}

	// The Scripting API
	enum { panPixels = 0, panPercent = 1 };
	/*void Pan(long x, long y, int mode);
	void Zoom(int factor);
	void SetZoomRect(long left, long top, long right, long bottom);*/
	int TotalFrames() { return player->numFrames; }
	int PercentLoaded() { return (int)(player->len*100/player->scriptLen); }
	BOOL FrameLoaded(int frameNum) { return player->FrameComplete(frameNum); }
	BOOL IsPlaying() { return player->playing; }

	XXObject *ResolveVariable(const XXVar&name,XSWFCONTEXT&cnt,XXVar&varName,XBOOL&bFirst);
	ScriptThread *ResolveFrameNum(XXVar&var,XSWFCONTEXT&cnt, int *frameNum);
	XBOOL SetVariable(XSWFCONTEXT&cnt,const XXVar&name,XXVar&var,EditText*pEdit);// BOOL updateFocus);
	XBOOL GetVariable(XSWFCONTEXT&cnt,char*name,XXVar&var,XBOOL bInit);
	SObject* FindTarget(_XSObject* base, char*name,_XSObject*with);
	XXObject* FindTargetA(XXObject* base, char*name,XXObject*with);
	//XXObject* FindTarget(XXObject*base,char*name,XXObject*with);
	static XBOOL GetTargetPath(_XSObject* base,XXVar&var);

#ifdef ACTIONSCRIPT
	BOOL CallFrame(ScriptThread *thread, int frameNum);
#endif

	XFDOUBLE GetProperty(ScriptThread *thread, int propType);
	XBOOL   GetPropertyStr(ScriptThread *thread, int propType,XXVar&var);

	void SetProperty(ScriptThread *thread, int propType,XXVar&var);// XFDOUBLE value);
	void SetPropertyStr(ScriptThread *thread, int propType, XXVar&var);


	/*! OnDraw is a method provided for use by the native code. <br>
	 *  OnDraw should be called when the play timer fires, or when
	 *	the screen should be rendered.
	 */

	/*! OnDraw is a method provided for use by the native code. <br>
	 *	It is used to request a repaint of a screen rectangle. Usefull
	 *	for responding to re-paint requests from the OS.
	 */
	void Repaint( SRECT* rect );

	void CheckUpdate();		// generate an InvalidateRect if the screen needs redrawn

	//virtual BOOL StartTimer( int playTimerInterval, int cursorTimerInterval ) = 0;
	//virtual void StopTimer() = 0;
	//virtual void InvalidateScreenArea( SRECT* ) = 0;
	//virtual void ClientRect(SRECT*) = 0;
    //virtual void AdjustWindow( int width, int height ) = 0;

	// Internet, loading, and streaming functions
	//char* BuildRelative(char* base, char* url);
	//char* NSResolveURL(char* actionURL);
	void BuildRelative(XXVar&base,XXVar&url);
	void NSResolveURL(XXVar&url);//char* actionURL);


	void NSGetURL(_XLoadInfo*pInfo);//char* actionURL, char* window, char* postData,XBOOL bPost);
	void NetscapeDoURL(LoadInfo*info);
	void LoadLayer(	LoadInfo*info);

	/*! GetURL provides support for all URL retrieval. It is not generally used by Native code.
	 */
	void GetURL(_XLoadInfo*pInfo);//char* url, char* target = "", char* postData = "", int loadMethod = 0,XXObject*pObj=XNULL,void*pASObject=NULL);




	/*! A data stream is delivered to Flash in 3 steps:<br>
	 *	1) StreamInNew<br>
	 *	2) StreamInWrite<br>
	 *	3) StreamInDestroy<br>
	 *	Note that Flash has only one thread and these calls need to be on that thread; it may
	 *	be necessary to perform synchronization in native code.<br>
	 *	StreamInNew is either a response to a POST or GET,
	 *	or can be used to initiate streaming (and loading a movie) without
	 *  a request from Flash. If used to initiate streaming, it is possible
	 *  the incoming stream will confilct with the existing movie. There
	 *  are 2 solutions to this problem:<br>
	 *	1) only use StreamGetURLNotify to load the first movie - useful for plugins<br>
	 *  2) use ControlOpen to start streaming, which will clear existing movies and strings.<br>
	 *	StreamInNew is modeled after the netscape NPP_NewStream.
	 *	\param		streamData		Storage space for data -- used by Flash core.
	 *	\param		url				URL of the request, which was (usually) passed to the native code
									by StreamGetURLNotify.
	 *	\param		notifyData		The data passed in by StreamGetURLNotify -- null if the stream is
     *								initiated by the native code.
	 */
	void SetURLBase(const char*base)
	{
		strBaseVar=base;
		strBaseVar.ToString(XTRUE);
	}
	void StreamInNew(	_XStreamData* streamData,
						char*		  url,
						char*		  urlBase,
						char*		  contentType,
						void*		  notifyData,
						XString8Array&list,
						int nCode,void*pClient);

	/*!	StreamInWrite delivers a block of SWF data to the Flash core. Based on netscape NPP_Write.
	 *	Returns >= 0 to continue, return -1 if error
	 *	\param		streamData		Storage space for data -- used by Flash core.
	 *	\param		buffer			Pointer to a block o' SWF data.
	 *	\param		length			length of that block.
	 */
	bool CloseClient(void*pClient);
	int StreamInWrite(	_XStreamData*	streamData,
						void* buffer,
						int	  length,
						void* pClient);

	/*!	StreamInDestroy ends the delivery of SWF data to the Flash core.
	 *	Based on netscape NPP_DestroyStream. Deallocates memory stored by streamData.
	 *	\param		streamData		Storage space for data -- used by Flash core.
	 */
	void StreamInDestroy( _XStreamData* streamData,XBOOL bOK,const char*msg,void*pClient);

	/*! If a stream needs to be axed mid-way (after the Stream in new, but before completion)
	 *  this call kills it. The most basic functionality of this call should be to set
	 *	streamData->scriptPlayer to NULL. That way, future calls to StreamInWrite and
	 *	StreamInDestroy will not write SWF data. It is a good idea to call StreamInDestroy on the
	 *	closed stream to clean up memory.
	 */
	//virtual void CloseStream( StreamData* streamData )	= 0;

	/*! FSCommand sends a command from the movie to the player-> The ProcessFSCommand method call
	 * is made to the NativePlayerWnd when it needs to process an FSCommand.
	 * It has two parameters: Command and Argument. Since the FSCommand does something different
	 * on every host platform, there are a wide range of actions that FSCommand can cause. Please
	 * see the Flash 4 help file on FSCommand for a complete description.
	 */


	//virtual void EnableMenus( const MenuStatus& menuStatus ) = 0;

	/*! SetMenuState will determine the state of the menu options and call EnableMenus.
	 */

	/*! GetMenuState puts the status (enabled, disabled, checked) of the various menu items into
	 *  the MenuStatus structure. It has no side effects and can be called at any time.
	 */
	//void GetMenuState( MenuStatus* menuStatus );

	#ifdef EDITTEXT
	/*! GetEditMenuState puts the status (enabled, disabled) of the edit text popup
	 *	menu items into the EditMenuStatus structure. It has no side effects and can be
	 *  called at any time. If no field has the focus, all of the fields in the structure
	 *	will be false.
	 */
	void GetEditMenuState( EditMenuStatus* editMenuStatus );
	#endif

	enum {
		MENU_EDIT,
		MENU_NONE,
		MENU_NO_MOVIE,
		MENU_NORMAL,
		MENU_STATIC
	};
	/*!	Based on the current mouse cursor, GetPopupMenuState assumes a popup menu has
	 *	been requested and returns an enumerated for what type of menu it should be:
	 *	MENU_EDIT, MENU_NONE, MENU_NO_MOVIE, MENU_NORMAL, or MENU_STATIC. Please see
	 *	the design doc for a description of the various menu types.
	 */

	/*! The Control* methods are provided for use by the native code. Their
	 *  functionality (and names) reflects menu item choices.<br>
	 *	ControlOpen opens and plays a movie given a URL to a local or remote object.
	 */
	void ChangeMode(int width,int height);
	//void ControlPlay();							//!< Play or stop (toggle)
	//void ControlRewind();						//!< Rewind the current movie to the beginning
	//void ControlForward();						//!< Move the movie forward one frame
	//void ControlBack();							//!< Move the movie backward one frame
	//void ControlLoop();							//!< Sets the movie looping or non-looping
	void ControlViewAll();						//!< Sets the movie to fill the client window
	void Control100();							//!< Sets the movie to its actual size (100% zoom)
	void ControlZoomIn();						//!< Zoom in
	void ControlZoomContext( int x, int y );	//!< Zoom in, from context menu. An x and y coordinate to zoom to must be specified.
	void ControlZoomOut();						//!< Zoom out
	void ControlHighQuality();					//!< Toggles high quality on and off
	void InitContext(XSWFCONTEXT&cnt);

	/*! Currently not used. Writes an "ideal" palette into ctab. Just a suggestion.
	 */
	void CreateIdealPalette( SColorTable* ctab );

	void XSetCapture()			{}		// These should not be needed if the client window always captures
	void XReleaseCapture()		{}		// the mouse when it is down.
	//virtual BOOL UpdateCursor() = 0;

	/*! Query the current cursor type. Returns either
	 *  CURSOR_TYPE_ARROW, CURSOR_TYPE_HAND, CURSOR_TYPE_BUTTON, CURSOR_TYPE_BEAM
	 */
	int GetCursorType();				// Returns the type of cursor that should be displayed.
										// UpdateCursor() uses this.

//#endif

    void RemoveSprite(ScriptThread *thread, BOOL createPlaceholder = true);

#ifdef DRAGBUTTONS
	void UpdateDropTarget();
	void UpdateDragObject(SPOINT *pt);
#endif

	// Button interface

	void		ButtonGoDirection(int dir);
	void		ButtonTabNext();
	void		ButtonTabPrevious();

	void		ButtonEnter();

	BOOL		ButtonFind(SButton*, SButton*);

	// Button implementation

	class ButtonWeight {
	public:
		_XSObject*	button;
		SRECT		buttonBounds;
		S32			weight;

		ButtonWeight()
		: button(NULL), weight(0)
		{

		}
	};

	class ButtonOrder {
	public:
		_XSObject* button[4];
		SRECT bounds[4];

		ButtonOrder() { Reset(); }
		void Reset() { memset(this, 0, sizeof(ButtonOrder)); }
	};

	enum { UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3 };

	_XSObject*	ButtonChoose(_XSObject*, P_SRECT, int, _XSObject*, P_SRECT, _XSObject*, P_SRECT);

	void		DoButton(_XSObject* pt, BOOL mouseIsDown, BOOL updateScreen = true);
	int			NumButtons(_XSObject*);
	void		ButtonFocusSet(_XSObject*);
	void		ButtonFocusRemove(_XSObject*);

	void		ButtonOrdering(SButton*, ButtonOrder*);
	//int			ButtonDirection(P_SRECT,P_SRECT);
	BOOL		IsButtonUp(P_SRECT, P_SRECT);
	BOOL		IsButtonDown(P_SRECT, P_SRECT);
	BOOL		IsButtonLeft(P_SRECT, P_SRECT);
	BOOL		IsButtonRight(P_SRECT, P_SRECT);
	BOOL		IsButtonAligned(P_SRECT, int, P_SRECT);
	BOOL		IsButtonOverlapped(P_SRECT, int, P_SRECT);
	void ReleaseContext(XSWFCONTEXT&cnt);
	void LoadURLs();
	P_SRECT		ButtonCloser(P_SRECT, P_SRECT, P_SRECT,int nDirect);

	// Methods used for tabbing implementation

	ButtonWeight* BuildButtonTabMap();

	void		ButtonSort(ButtonWeight* array, int size);
	S32			CalcButtonWeight(P_SRECT bounds);

	// Methods used to help enumerate all buttons

	void		AddButton(_XSObject* obj);
	void		RemoveAllButtons();

	// Data members

	ButtonOrder		 buttonOrder;
	SButton			 currentButton;

	_XSObject**		buttonArray;
	int				buttonIndex;
	int				arraySize;

	// from ShockwaveFlash.h
	SButton			m_lastButton;
	int				m_Focus;
	//XU8			    m_nScreenDirect;
public:
	enum {XKEYDOWN,XKEYUP,XMOUSEDOWN,XMOUSEUP,XMOUSEMOVE,XCHAR,XMOUSECLICK,XMOUSEDOUBLECLICK,XSTRING,XACTIVE,XDEACTIVE,XGETCHILDREN,
		  XSOUNDCOMPLETE,XFINDCLASS,XAS3EVENT,XMOVIECLIP0,XGETCLASSTREE,XACTIVITY,XGETDIBINFO,XGETMEMINFO,XSEEKITEM,XGETGLDIB};
	ScriptPlayer*m_pADPlayer;
	SObject* m_pFocus;
protected:
	
	XBOOL LanuchAD();
	XBOOL m_bADLanuched;

	void Activate();
	void Deactivate();
	XCXForm form;
	XConfig m_config;
//	int nAspect;
//	XConfig m_config;

	bool DoEvent(bool&bMouse);

	struct XEVENT
	{
		int type;
		int x,y;
		int state;
		XU32 nTime;
		XEVENT*pNext;
	};
	XEVENT*m_pEvents;
	XBOOL SetFocus(SObject*hitBtn);
	XBOOL HandleListener(XU32 type);
	//_XSObject* GetFocus();
	//XBOOL CallHandlers(XSWFCONTEXT&cnt);
	//void ClearHandlers();
	XBOOL CallTimer(XSWFCONTEXT&cnt);
	//ScriptThread* ThreadOf(XSWFCONTEXT&cnt,ScriptThread*thread);


	//XMutex m_dataLock;
	XMutex m_eventLock;
	XMutex m_loadLock;
	//XMutex m_lock;
	XEvent m_hStop;
	//XRect  m_updateRect;
public:
	/*XRect directRect;
	void DirectDraw(XRect&r)
	{
		directRect|=r;
		//m_updateRect|=r;
	}*/
	void RemoveClient(void*pClient)
	{
		if(pWindow)
			pWindow->Handle(XCMD_REMOVECLIENT,(XU32)pClient,0);
	}
	const char* ProcessFSCommand(char* cmd,char* args);
protected:
	/*! StreamGetURLNotify is a call made to the Native code to start a Get.
	 *	It mimics the netscape call NPN_GetURLNotify (See the plug-in guide
	 *	at http://developer.netscape.com/docs/manuals/ )
	 *	\param		url		URL of the GET request, specified by the plug-in.
	 *	\param		window	Target window. For values, see NPN_GetURL.
	 *	\param		notifydata    Plug-in-private value	 */

	void StreamGetURLNotify(LoadInfo*notifyData);
	/*! StreamPostURLNotify is a call made to the Native code to start a Post.
	 *	It mimics the netscape call NPN_PostURLNotify. (See the plug-in guide
	 *	at http://developer.netscape.com/docs/manuals/ )
	 *	\param		url		URL of the POST request, specified by the plug-in.
	 *	\param		window	Target window. For values, see NPN_GetURL.
	 *	\param		len		Length of the buffer buf.
	 *	\param		buf		Path to local temporary file or data buffer that contains the data to post.
	 *	\param		notifydata    Plug-in-private value
	 */

	void ClientRect(SRECT&rect);
public:
	XSIZE &m_winSize;
	XWindow*pWindow;
	void Trace(const XU16*msg)
	{
		//if(msg[0]=='w'&&msg[1]=='a')
		//	int v=0;
#ifdef _WINEMU
		if(pWindow)
			pWindow->Trace(msg);
#endif
	}
private:
    enum ScrennOrientation
    {SCRENNRight, SCRENNPortrait};
    bool bRotaionEnd;
    ScrennOrientation scrOrien;
    void CheckOrientation()
    {
        switch(scrOrien)
        {
            case SCRENNRight:
                if(m_winSize.cx > m_winSize.cy)
                {
                    bRotaionEnd = true;
                    return;
                }
                break;
            case SCRENNPortrait:
                if(m_winSize.cx < m_winSize.cy)
                {
                    bRotaionEnd = true;
                    return;
                }
                break;
        }
        bRotaionEnd = false;
    }
// 	void  OnDraw( NativePlayerWnd* );
// 	void  LoadFile( URLWrapper *url, int layer, char *dstSprite = 0, char *postData = 0, BOOL loadVars = FALSE);

//dushixiong 2011-4-19 --start
public:
	bool	   m_bPreLoad;
	XNetWindow *m_pNetWindow;
	XClientApply *m_pClientApply;
	XBOOL GetObjectRect(XRect&rect);
	void LockOpt(XBOOL bClear);
	void ClearOpt(_XSObject*obj);
	void UnLockOpt();
public:
	XBOOL AddOptObject(_XSObject*obj);
	XBOOL AutoObject(XSIZE&sz);
	void SetLockClass(void*codeContext);
	void SetMaxPecent( );
	void SetZoomPecent( float zoomScale, SPOINT&center );
	XBOOL	   m_nLockOpt;
	AS3ButtonInfomation** m_pBIInfo;
public:
	void SetDeltaXY(int x,int y)
	{
		m_nClickDeltaX=x;
		m_nClickDeltaY=y;
	}
	void AddDelta(int x,int y)
	{
		m_nClickDelta[(m_nClickOffset<<1)]=x;
		m_nClickDelta[(m_nClickOffset<<1)+1]=y;
		if(m_nClickOffset+1<MAXDELTA)
			m_nClickOffset++;
		else
			m_nClickOffset=0;
		m_nClickDeltaX*=4;
		m_nClickDeltaY*=4;
		for(int i=0;i<MAXDELTA;i++)
		{
			m_nClickDeltaX+=m_nClickDelta[(i<<1)];
			m_nClickDeltaY+=m_nClickDelta[(i<<1)+1];
		}
		m_nClickDeltaX/=(MAXDELTA+4);
		m_nClickDeltaY/=(MAXDELTA+4);
	}
	int		   m_nClickDelta[MAXDELTA*2];
	int		   m_nClickOffset;
	int		   m_nClickDeltaX,m_nClickDeltaY;
	int		   m_nErrorCode;
	void	   SetErrorCode(int nErrorCode);
protected:
	XU32	   m_nCallTimerID;	

	XU32Array  m_lockClasses;
	
	SRECT	   m_objRect;
	_XSObject* m_optObject;
	//XBOOL      m_bUpdateRedraw;
//dushixiong 2011-4-19 --end
};

typedef XSWFPlayer SPlayer;
//#define SPlayer XSWFPlayer

int LayerNum(char* s);

//
// A Helper Object that combines variables into a form suitable for URLs or a POST operation
//

class URLBuilder {
public:
	char* buf;
	int len;	// the first empty spot
	int size;

public:
	URLBuilder() { buf = 0; len = 0; size = 0; }
	~URLBuilder() { FreeStr(buf); }

	void AppendChar(char);
	void AppendInt(int);
	void AppendString(char*);
	void EncodeString(char*);
	//void AppendData(XU8*pData,int l);

	void EncodeVariables(XSWFCONTEXT&cnt,XXObject*pObj);//ScriptVariable* var);

};

void ExtractVariables(XSWFCONTEXT&cnt,XXObject* thread, char* url);




#endif
