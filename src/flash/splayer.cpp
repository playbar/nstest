/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc.
****************************************************************************/
//	990325	mnk	"fixed1" -> "fixed_1"
#include "StdAfxflash.h"
#include <math.h>
#include "splayer.h"
#include "XWindow.h"
#include "XDomView.h"
#include "stags.h"
#include "sobject.h"
#include "flashmemcop.h"
#include "edittext.h"
#include "splay.h"
#include "XXObject.h"
#include "ObjectCreator.h"
#include "XActionScript.h"
#include "XXObjectLoadVars.h"
#include "avm2.h"
#include "avmplus.h"
#include "XFilter.h"

using namespace avmplus;
#include "ShellCore.h"
using namespace avmshell;
#include "AS3Loader.h"
#include "AS3LoaderInfo.h"
#include "AS3EventDispatcher.h"
#include "AS3Security.h"
#include "AS3Timer.h"
#include "AS3LocalConnection.h"
#include "AS3SoundChannel.h"
#include "AS3Event.h"
//#ifdef _MultiThread_
#include "BwShapeRecord.h"
#include "BwShapeToGL.h"

//#endif
#define __STOPRUN__
#ifdef __STOPRUN__
XBOOL _bStopRun=XFALSE;
#endif

#ifdef _WINEMU
extern bool _analyFunction;
#endif

//#define _FULLEDIT
XSWFPlayer* XSWFPlayer::m_pInstance=NULL;
//#if (__CORE_VERSION__>=0x02075000)
static XU8 _stockEditData[]={												  /*0x7f,0xfd,0x85,0x43,0x7f,0xf6,
							0x0b,0x42,0x80,*/0xed/*ed*/,0x22,0x00/*01*/,0x00,0x04,0x01/*0x01*/,0x00,0x33,0x66,0xff,0x00,0x00,0x00,
							0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x70,0x20,0x61,0x6c,0x69,0x67,0x6e,0x3d,
							0x22,0x6c,0x65,0x66,0x74,0x22,0x3e,0x3c,0x2f,0x70,0x3e,0x00};//Default size=12*20 but 14*20
//#else
//static XU8 _stockEditData[]={												  /*0x7f,0xfd,0x85,0x43,0x7f,0xf6,
//							0x0b,0x42,0x80,*/0xed/*ed*/,0x22,0x01,0x00,0x04,0x01,0x00,0x33,0x66,0xff,0x00,0x00,0x00,
//							0x00,0x00,0x00,0x00,0x28,0x00,0x00,0x3c,0x70,0x20,0x61,0x6c,0x69,0x67,0x6e,0x3d,
//							0x22,0x6c,0x65,0x66,0x74,0x22,0x3e,0x3c,0x2f,0x70,0x3e,0x00};//Default size=12*20 but 14*20
//#endif
char *ConvertIntegerToString(int value);

static XU8 _advanceTable[512]={
		0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xa,0x0,0xa,0x0,0xfa,0x2,0xfa,0x2,0xa,0x0,0xfa,0x2,0xfa,0x2,0xfa,0x2,
		0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xfa,0x2,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x1,
		0x2e,0x1,0x2e,0x2,0x40,0x2,0x31,0x2,0x54,0x4,0xab,0x3,0x4,0x1,0x85,0x1,0x63,0x1,0x14,0x2,0x92,0x2,0xa,0x1,0x76,0x1,0xfd,0x0,0x54,0x1,0x2c,0x2,
		0xfc,0x1,0x1e,0x2,0x13,0x2,0x27,0x2,0x2b,0x2,0x33,0x2,0x3c,0x2,0x2e,0x2,0x2c,0x2,0x2f,0x1,0x3b,0x1,0x95,0x2,0x95,0x2,0x95,0x2,0x15,0x2,0x37,0x4,
		0x4e,0x3,0xf2,0x2,0x1b,0x3,0x2b,0x3,0xe6,0x2,0xb6,0x2,0x92,0x3,0x89,0x3,0xb8,0x1,0x50,0x2,0xaa,0x3,0xf1,0x2,0x4c,0x4,0x3f,0x3,0x74,0x3,0xb5,0x2,
		0x75,0x3,0x67,0x3,0x6a,0x2,0xf0,0x2,0x47,0x3,0x4f,0x3,0x99,0x4,0x55,0x3,0x4f,0x3,0x7,0x3,0x6c,0x1,0x54,0x1,0x18,0x1,0x62,0x2,0x62,0x2,0x10,0x1,
		0x44,0x2,0x66,0x2,0xf8,0x1,0x7e,0x2,0xf8,0x1,0xe8,0x1,0x42,0x2,0x77,0x2,0x46,0x1,0x2e,0x1,0xa4,0x2,0x46,0x1,0xc0,0x3,0x77,0x2,0x2c,0x2,0x66,0x2,
		0x81,0x2,0x2,0x2,0xa9,0x1,0x87,0x1,0x73,0x2,0x4c,0x2,0x50,0x3,0x42,0x2,0x4c,0x2,0x0,0x2,0xa9,0x1,0xaf,0x0,0x7d,0x1,0x83,0x2,0x4a,0x1,0xa,0x0,
		0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,
		0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,0xa,0x0,
		0x2e,0x1,0x17,0x2,0x3f,0x2,0x4c,0x2,0x56,0x2,0xaf,0x0,0x22,0x2,0x87,0x1,0x5b,0x3,0x74,0x1,0x40,0x2,0x95,0x2,0x76,0x1,0x5b,0x3,0x61,0x2,0xba,0x1,
		0x87,0x2,0x50,0x1,0x49,0x1,0x7d,0x1,0x9b,0x2,0x88,0x2,0x1a,0x1,0x20,0x1,0x3b,0x1,0x72,0x1,0x42,0x2,0x66,0x3,0x5f,0x3,0x66,0x3,0xf,0x2,0x4e,0x3,
		0x4e,0x3,0x4e,0x3,0x4e,0x3,0x4e,0x3,0x4e,0x3,0x6e,0x4,0x1b,0x3,0xe6,0x2,0xe6,0x2,0xe6,0x2,0xe6,0x2,0xb8,0x1,0xb8,0x1,0xb8,0x1,0xb8,0x1,0x30,0x3,
		0x3f,0x3,0x74,0x3,0x74,0x3,0x74,0x3,0x74,0x3,0x74,0x3,0x54,0x2,0x73,0x3,0x47,0x3,0x47,0x3,0x47,0x3,0x47,0x3,0x4f,0x3,0xb1,0x2,0x5e,0x2,0x44,0x2,
		0x44,0x2,0x44,0x2,0x44,0x2,0x44,0x2,0x44,0x2,0x3a,0x3,0xf8,0x1,0xf8,0x1,0xf8,0x1,0xf8,0x1,0xf8,0x1,0x46,0x1,0x5c,0x1,0x5e,0x1,0x69,0x1,0x2c,0x2,
		0x77,0x2,0x2c,0x2,0x2c,0x2,0x2c,0x2,0x2c,0x2,0x2c,0x2,0x87,0x2,0x30,0x2,0x73,0x2,0x73,0x2,0x73,0x2,0x73,0x2,0x4c,0x2,0x66,0x2,0x4c,0x2,0x4e,0x3};


#define GCENTER\
	MMgc::GCAutoEnter gcEnter(NULL);\
	if(m_pAVM2){ m_pAVM2->GetCore()->setStackLimit();gcEnter.Enter(m_pAVM2->GetGC());\
		m_nCollectCount++;\
		gcEnter.doCollect=(m_nCollectCount&0x1ff)==1;\
		if(m_nCollectCount==(m_pDomFilter?m_pDomFilter->nCacheObject:0x1ff))\
		{gcEnter.doCollect=true;m_pAVM2->GetGC()->policy.queueFullCollection();m_nCollectCount=0;}}


#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#define VK_BACK           0x08
#define VK_TAB            0x09

#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14

#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19

#define VK_ESCAPE         0x1B

#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F


// end add

int Number(XPCTSTR name)
{
	int nNum=0;
	XPCTSTR t=name;
	for(;;)
	{
		switch(*t)
		{
		case 0:return nNum;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			  nNum=nNum*10+(*t-'0');
			  break;
		default:
			return -1;
		}
		t++;
	}
	return nNum;
}

inline XBOOL GetMember(XSWFCONTEXT&cnt,XPCTSTR name,XXVar&var,XBOOL bInc,
					   XXObject*pObj0,XXObject*pObj1=XNULL,XXObject*pObj2=XNULL)
{
	if(pObj1==pObj0) pObj1=XNULL;
	if(pObj2==pObj0||pObj2==pObj1) pObj2=XNULL;
	int nNum=Number(name);
	if(nNum>=0)
	{
		if(pObj0&&pObj0->GetMember(nNum,var,XNULL)) return XTRUE;
		if(pObj1&&
           pObj1->GetMember(nNum,var,XNULL)) return XTRUE;
		if(pObj2&&
		   pObj2->GetMember(nNum,var,XNULL)) return XTRUE;
	}
	if(pObj0&&pObj0->GetMember(cnt,name,var,bInc)) return XTRUE;
	if(pObj1&&pObj1->GetMember(cnt,name,var,bInc)) return XTRUE;
	if(pObj2&&pObj2->GetMember(cnt,name,var,bInc)) return XTRUE;
	return XFALSE;
}

inline XXObject* GetMember(char* name,
					   XXObject*pObj0,XXObject*pObj1=XNULL,XXObject*pObj2=XNULL)
{
	if(pObj1==pObj0) pObj1=XNULL;
	if(pObj2==pObj0||pObj2==pObj1) pObj2=XNULL;
	int nNum=Number(name);
	XXVar var;
	if(nNum>=0)
	{
		XBOOL bOK=pObj0&&pObj0->GetMember(nNum,var,XNULL);//) //return XTRUE;
		if(!bOK&&pObj1)
			bOK=pObj1->GetMember(nNum,var,XNULL);
		if(!bOK&&pObj2)
		    bOK=pObj2->GetMember(nNum,var,XNULL);
		if(bOK&&var.IsObject()) return var.pObject;
	}
	XXObject*pObj=XNULL;
	if(pObj0)
	{
		pObj0->GetMember(name);//cnt,name,var,bInc)) return XTRUE;
		if(pObj) return pObj;
	}
	if(pObj1)
	{
		pObj=pObj1->GetMember(name);//cnt,name,var,bInc)) return XTRUE;
		if(pObj) return pObj;
	}
	if(pObj2)
	{
		return pObj2->GetMember(name);//cnt,name,var,bInc)) return XTRUE;

	}
	return XNULL;
}

/*-------------------------------------------------------------------------*/

#ifdef SOUND
	#ifdef ONE_SOUND_OBJECT
	// If there is only one sound object, initialize it.
	XSoundMix XSWFPlayer::theSoundMix;
	#endif
#endif





#ifdef _WINEMU
XSWFPlayer* _tracePlayer;
#endif
XSWFPlayer::XSWFPlayer(int nDeltaX,int nDeltaY):
		charAlloc(sizeof(_XSCharacter),64),//:builtInFontsPlayer(XFALSE)
		eventAlloc(sizeof(XEVENT),32),
		filterAlloc(sizeof(_XFilter),32),
		m_winSize(RasterGL::sharedRasterGL()->m_WinSize)
{
	m_nDelayShow = 0;
	m_pShapeImages = NULL;
	m_bDataProc = false;
	m_bZooming = false;
	fontColor.colorType = glColorFont;
//#if (__CORE_VERSION__>=0x02074000)
	m_pDownTarget=NULL;
	m_pUpTarget=NULL;
	m_bDataOnly=XFALSE;
//#endif
//#if (__CORE_VERSION__>=0x02071000)
	m_bNeedEnter=XFALSE;
//#endif
//#if (__CORE_VERSION__>=0x02070200)
	m_iEnterCount=0;
//#endif
	m_nCallTimerID=1;
	m_bDoFrame=XFALSE;
	m_bResetMode=XFALSE;
	strBaseVar=_strEmpty;
	m_bEnterFrame=XFALSE;
	m_nErrorCode=0;
	m_pUpObject=NULL;
#ifdef DOTFONT
//	bDotFontOK=XFALSE;
#endif
	m_pUpRemovedObject=NULL;
	isMoveScroll=XFALSE;
	m_pFocus=NULL;
	nDownTime=0;
	m_nClickOffset=0;
	m_nClickDeltaX=nDeltaX;
	m_nClickDeltaY=nDeltaY;
	m_bPreLoad=false;
	for(int i=0;i<MAXDELTA;i++)
	{
		m_nClickDelta[i<<1]=nDeltaX;
		m_nClickDelta[(i<<1)+1]=nDeltaY;
	}
//#ifdef _DEBUG
//	memset(_actionTime,0,sizeof(_actionTime));
//	memset(_actionCount,0,sizeof(_actionCount));
//#endif
	//XDrawDib dib;
	//dib.Create(120,100);
	//dib.FillRect(XRect(10,10,100,80),0xff00ffff,0);
	//dib.SaveFile("E:\\abc.bmp");
	m_nLockOpt=0;
	::RectSetEmpty(&m_objRect);
	m_optObject=NULL;
	XSWFPlayer::m_pInstance=this;
	//m_nMaxPecent=100;
	//mnZoomPecent = 1;
	m_pDom=NULL;
//#ifdef _LOWSPEED
//	m_bDragOrZoom=false;
//	m_bUpdateDrag=false;
//	m_bNoScript=false;
//#endif
#ifdef _WINEMU
	_tracePlayer=this;
#endif
	m_nFrameWidth=0;
	m_nFrameHeight=0;
	m_nDomSize=0;
	m_pLoaderInfo=NULL;
	m_pADPlayer=NULL;
	this->m_pNextDoThread=NULL;
	m_nRunTime=XTime::GetTimeMSec();
	SBitmapCore::m_pPlayer=this;
	m_bitmaps=XNULL;
	this->nCacheCount=0;
	avmshell::m_bDestroying=false;
	m_pDomFilter=XNULL;
	m_nMode=MV_NONE;
	m_nLockMode=LK_NONE;
	m_nLimitX=0;
	m_nLimitY=0;
	m_fScaleX=1;
	m_fScaleY=1;
	downling=XFALSE;
	this->m_bADLanuched=XFALSE;
	m_nCollectCount=0;
	m_nLoopCount=1;
	m_pAVM2=XNULL;//avmshell::InitAVM2(this);
	//pAS3Objects=XNULL;
	pAS3Player=XNULL;
	m_bCancelNext=false;
//	m_pStage=NULL;
	pLoader=NULL;
	pLastLoader=NULL;
#ifdef SOUND
	display.theSoundMix = &theSoundMix;
#endif
	player=new ScriptPlayer(XTRUE);
#ifdef FULLFONT
	buildInFont.font.infoData=_advanceTable;
	buildInFont.font.shapeData=XNULL;
	//buildInFont.font.shapeColor=XNULL;
	buildInFont.data=XNULL;
#ifdef DOTFONT
	//dotFont16.font.infoData=XNULL;
	//dotFont16.data=XNULL;
	//dotFont16.font.shapeData=XNULL;
	////dotFont16.font.shapeColor=XNULL;
	//dotFont12.font.infoData=XNULL;
	//dotFont12.data=XNULL;
	//dotFont12.font.shapeData=XNULL;
	////dotFont12.font.shapeColor=XNULL;
	////dotFont8.font.infoData=XNULL;
	////dotFont8.data=XNULL;
#endif
#else
	builtInFontsPlayer=new ScriptPlayer(XFALSE);
	builtInFontsPlayer->m_pGlobal=player->m_pGlobal;
#endif
	bError=XFALSE;
	display.root.CreatePlayer=player;

	m_callList=XNULL;
	m_freeList=XNULL;
//	m_pConfig=XNULL;
//	m_handlers=XNULL;
	m_hStop.Create();
//	hitObj=XNULL;
	m_nStageWidth=1000;
	m_nStageHeight=800;
	mousePt.x=0;
	mousePt.y=0;
	gmousePt.x=0;
	gmousePt.y=0;
	gmouseState=-1;
	nSkipCount=0;
	nSkipsNow=0;
	//nKeyCode=0;
//	for(int i=0;i<XKEYS_NUMS;i++)
//		m_keys[i]=XNULL;
	bAdJustWin=XFALSE;
	m_pEvents=XNULL;
	// Our Player
	display.player=this;
	player->SetDisplay(&display,true);	// the player draws into the display list
	player->splayer = this;

	nQuality=XConfig::MID;//MID;
    //highQuality = XTRUE;
	autoQuality = XFALSE;
//	bestDither = XFALSE;
	lastQualitySwitch = 1000;

	//m_nScreenDirect=2;
    // set the version variable
    //SetVersionVariable( &player );

	// Counters for auto quality settings
	//nTooSlow = 0;
	//nTooFast = 0;
	//nTotal = 0;
	nTotalCount=0;
	nTotalCost=0;
	nTotalNeed=0;
	nLastKey=0;
	memset(nKeyStatus,0,sizeof(nKeyStatus));
	//starved = XFALSE;

	// # of seconds to prebuffer sound
	nSoundBufferTime = 5;

	// Offset for GetTimer
	mTimerOffset = m_nRunTime;//XTime::GetTimeMSec();

//	showMenu = XTRUE;

	//syncToClock = XFALSE;

#ifdef EDITTEXT
	// Text editing
	focus = 0;
	cursorBlink = XFALSE;
#ifndef FULLFONT
	builtInFontsState = kBuiltInFontsNotLoaded;
#endif
#endif
	cursorShow=XTRUE;
	menuShow=XTRUE;
	m_align=stextAlignLeft;
	// View Control
	//scaleMode = DisplayList::showAll;
	//RectSetEmpty(&zoomRect);

	actionDepth = 0;

//	actionOverflow = XFALSE;

	scrolling = XFALSE;

	// Play Control
	loaded = XFALSE;
	running = XFALSE;
	nextFrameTime = 0;

	mouseState = -1;

#ifdef ACTIONSCRIPT
//	stack = NULL;
//	stackPos = 0;
//	stackSize = 0;
#endif

//#ifdef DRAGBUTTONS
#if defined(DRAGBUTTONS) && !defined(GENERATOR)
	dragObject = 0;
	dragStarted = XFALSE;
#endif

	// Button initialization
	buttonArray		= NULL;
	buttonIndex		= 0;
	arraySize		= 0;

	// streaming (netscape) initialization
	mode = 0;
	strURL=_strEmpty;//url = 0;
	strURLBase = _strEmpty;//0;
	strName=_strEmpty;//name = 0;

	firstStream = XTRUE;
	destructing = XFALSE;
	m_pActiveTarget=XNULL;
	m_pActiveObject=XNULL;

#ifdef SOUND
	theSoundMix.Construct();
#endif

	//dushixiong 2011-4-19 --start
	m_pNetWindow = new XNetWindow(this);
	//dushixiong 2011-4-19 --end

//	SetVersionVariable( player );
    bRotaionEnd = true;
}

XSWFPlayer::~XSWFPlayer()
{
//	FLASHOUTPUT( "Deleting: XSWFPlayer\n" );
	m_bDestroying=true;
	m_bRunning=false;
	Stop();

#ifdef SOUND
	theSoundMix.CloseDevice();
#endif

	SBitmapCore*bitmap=m_bitmaps;
	while(bitmap)
	{
		SBitmapCore*next=bitmap->m_pNext;
		bitmap->PIFree();
		bitmap=next;
	}
	
	{
		_SHAPEIMAGES*pImages=m_pShapeImages;
		while(pImages)
		{
			//_SHAPEIMAGES*p=*pImages;
			//_SHAPEIMAGES**next=&pImages->next;
			_SHAPEIMAGES* pNext = pImages->next;
			_ShapeImage*pImage=pImages->ch->record.m_pImage;
			
			pImages->ch->record.ReleaseAll(true);//ReleaseImage();
			XXVar::_FreeData(pImages);
			
			pImages=pNext;
		}
		m_pShapeImages = NULL;
	}
	

	GCENTER
	//MMgc::GCAutoEnter gcEnter(NULL);\
	///if(m_pAVM2&&!bError){ gcEnter.Enter(m_pAVM2->GetGC());gcEnter.doCollect=true;}
	//Trace(XString16(":Destroy"));
	if(m_pAVM2)
	{
		m_pAVM2->ReleaseData();
		m_pAVM2->GetToplevel()->ClearDeadImage(true);

	}
	//this->AS3ReleaseObjects();

	while(pLoader)
	{
		_XLoadInfo*pNext=pLoader->pNext;
		display.FreeLoad(pLoader);
		pLoader=pNext;
	}

	XXObject::DestructAll();

	destructing = XTRUE;		// We cannon call NativePlayerWnd functions any more - since NPW has
							// been deleted. Set this flag so NPW functions can be wrapped in
							// if ( !destructing ) calls.
	//Trace(XString16(":Stop"));
	ClearScript();

	FreeLayers();

	display.FreeAll();

	XEVENT*p=m_pEvents;
	while(p)
	{
		XEVENT*pNext=p->pNext;
		//delete p;
		eventAlloc.Free(p);
		p=pNext;
	}
	m_pEvents=XNULL;
	this->AS3ReleasePlayer();
	AS3Release();

	if(display.root.pASObject)
		m_pAVM2->ReleaseInstance((void**)&display.root.pASObject,XNULL,true,NULL);

#ifdef ACTIONSCRIPT
	// Delete the action stack
	//while ( stackPos > 0 )
	//{
	//	FreeStr(Pop());
	//}
	//delete [] stack;
	//runstack.Release();
#endif

	RemoveAllButtons();
	strURL=_strEmpty;//.Release();
	strURLBase=_strEmpty;
	strName=_strEmpty;


	//delete url;
	//url = 0;

#ifdef SOUND
	theSoundMix.Destruct();
#endif
	//display.FreeObjs();

	XXObject::FreeObjs();

	XXObject::DestroyAll();

	display.ShutDownAlloc();

	delete player;
#ifdef FULLFONT
	//if(buildInFont.font.infoData)
	//{

	//	XU32*pAddr=buildInFont.font.GetAddress();
	//	for(int i=0;i<buildInFont.font.nGlyphs;i++)
	//	{
	//		if(pAddr[i])
	//			delete (XU32*)pAddr[i];
	//	}
	//	delete buildInFont.font.infoData;
	//	//delete []chars;
	//}
	if(buildInFont.data)
		delete buildInFont.data;
	BwShapeRecord::ReleaseFontData(buildInFont.font.shapeData,buildInFont.font.nGlyphs);
	//BwShapeRecord::ReleaseFontColor(buildInFont.font.shapeColor,buildInFont.font.nGlyphs)
//	m_fontFile.Close();
#ifdef DOTFONT
	//if(dotFont16.font.infoData)
	//	delete dotFont16.font.infoData;
	//BwShapeRecord::ReleaseFontData(dotFont16.font.shapeData,dotFont16.font.nGlyphs);
	////BwShapeRecord::ReleaseFontColor(dotFont16.font.shapeColor,dotFont16.font.nGlyphs)
	//if(dotFont12.font.infoData)
	//	delete dotFont12.font.infoData;
	//BwShapeRecord::ReleaseFontData(dotFont12.font.shapeData,dotFont12.font.nGlyphs);
	//BwShapeRecord::ReleaseFontColor(dotFont12.font.shapeColor,dotFont12.font.nGlyphs)
#endif
#else
	delete 	builtInFontsPlayer;
#endif


	if(m_pAVM2)
	{
		if(m_pLoaderInfo)
			m_pAVM2->RemoveObject(m_pLoaderInfo);
		m_pAVM2->CheckRCObjects(&display.root);

		//m_pAVM2->GetGC()->FinalizeRC();
		//Trace(XString16(":Destroy"));
		//Trace(XString16("Destroy"));
		avmshell::ReleaseAVM2(m_pAVM2);
		//Trace(XString16("Destroy OK"));
	}
	XXObject::FreeAll();


	XFileFinder finder;
	XXVar path=m_strWorkCache;
	path.StringAdd("*");

	if(finder.FindFirst(path.strTxt))
	{
		for(;;)
		{
			//XString8 strFile,strName;
			//finder.GetFileName(strFile,strName);
			if(!finder.IsDirectory())
			{
				XXVar strFile=m_strWorkCache;
				//strFile.ingAdd("\\");
				strFile.StringAdd(finder.cFileName);
				XFile::RemoveFile(strFile.strTxt);
			}
			if(!finder.FindNext()) break;
		}
	}


	delete m_pNetWindow;
	this->ReleaseAllShapeImages();
	//this->ReleaseAllCaches();

	ShapeAlloc::Release();
	TessPloy::Release();
	//RasterGL::ClearCache();
}


void XSWFPlayer::FreeCharacter(SCharacter* c)
{
	//if(c->tag==55)
	//	int v=0;
	c->record.ReleaseAll();
	ReleaseData(c->data);
	switch ( c->type ) {
		//case shapeChar: {
		//} break;
		case bitmapEmptyChar:
		case loaderImageChar:
		case videoEmptyChar:
		case bitsChar: {
			// Free the uncompressed bitmap
// 			#ifdef _CELANIMATOR
// 			if ( c->data != 0 )	// don't free pointer bitmaps
// 			#endif

			c->bits.PIFree();
		} break;

		case buttonChar: {
			ReleaseData(c->button.soundData);
			ReleaseData(c->button.cxformData);
		} break;

		case fontChar: {
			ReleaseData(c->font.infoData);
			BwShapeRecord::ReleaseFontData(c->font.shapeData,c->font.nGlyphs);
			c->font.shapeData = NULL;
			//BwShapeRecord::ReleaseFontColor(c->font.shapeColor,c->font.nGlyphs);
			//if(c->font.shapeData)
			//	delete c->font.shapeData;
		} break;

#ifdef SOUND
		case soundChar:
			theSoundMix.FreeSound(&c->sound);
			break;
#endif
	}

	charAlloc.Free(c);
}

void XSWFPlayer::SetVersionVariable( ScriptPlayer *player )
{
    //char version[64];


	//sprintf( version, "%s %s", PLATFORM_STRING, VERSION_STRING );
    //SetVariable(player, "$version", XXVar("jqc 10"), XFALSE);
	//player->rootObject->pObject->
	//	AddMember("$version", XXVar("jqc 10"), XFALSE);
}


_XSObject* XSWFPlayer::SetMyFucos(_XSObject *hitBtn)
{
	_XSObject*old=display.button;
#ifdef EDITTEXT
	if (hitBtn && hitBtn->IsEditText()) {
		{
			//if (!IsSelecting())
			{
				XSetCapture();
				if (focus != hitBtn) {
					if (focus) {
						cursorBlink = XFALSE;
						//if(focus->OnEvent(this,XOBJEVENT_KILLFOCUS,hitBtn))
						//	bAction=XTRUE;
						//focus->editText->ClearSelection();
						focus->Modify();
					}
					old=focus;
					focus = hitBtn;
					//if(focus -> OnEvent(this,XOBJEVENT_SETFOCUS,hitBtn))
					//	bAction=XTRUE;
				}
				//focus->EditTextMouse(pt, XTRUE);
				focus->Modify();
			}
		}
		//display.iBeam = ((hitBtn->editText->m_flags & seditTextFlagsNoSelect) == 0);
		//hitBtn = 0;
	} else {
		if ( focus ) {
			cursorBlink = XFALSE;
			//focus->editText->ClearSelection();
			//if(focus->OnEvent(this,XOBJEVENT_KILLFOCUS,hitBtn))
			//	bAction=XTRUE;
			focus->Modify();
			old=focus;
			focus = NULL;
		}
	}
#endif

	if ( hitBtn ) {
		// Check object is really a button
		//FLASHASSERT(hitBtn->character->type == buttonChar);
		if ( !hitBtn->character ) {
			FLASHASSERT(hitBtn->character);
			return old;
		}


	}

	FLASHASSERT(!hitBtn || hitBtn->character->type == buttonChar || hitBtn->HasMouseEvent());

	if ( display.SetButton(hitBtn, false) ) {

		if (!RectIsEmpty(&display.tabFocusRect)) {
			display.InvalidateRect(&display.tabFocusRect);
		}
		if (hitBtn) {
			hitBtn->CalcButtonBounds(&display.tabFocusRect);
			display.InvalidateRect(&display.tabFocusRect);
		} else {
			RectSetEmpty(&display.tabFocusRect);
		}

	}
	return old;
}

void XSWFPlayer::DoButton(SObject* hitBtn, BOOL mouseIsDown, BOOL updateScreen)
{
	XBOOL bAction=XFALSE;


	bAction=SetFocus(hitBtn);

	//if(!mouseIsDown) return;
#ifdef EDITTEXT
	/*if (hitBtn) {
		if (hitBtn->character->type == editTextChar) {
			if (focus != hitBtn) {
				if (focus) {
					cursorBlink = XFALSE;
					//if(focus->OnEvent(this,XOBJEVENT_KILLFOCUS,hitBtn))
					//	bAction=XTRUE;
					//if(hitBtn->OnEvent(this,XOBJEVENT_SETFOCUS,focus))
					//	bAction=XTRUE;
					focus->editText->ClearSelection();
					focus->Modify();
				}
				//if(hitBtn->OnEvent(this,XOBJEVENT_SETFOCUS,focus))
				//	bAction=XTRUE;
				focus = hitBtn;
				EditText* editText = focus->editText;
				editText->m_selectionStart = 0;
				editText->m_selectionEnd = editText->m_length;
				focus->Modify();
				display.iBeam = XTRUE;
			}
			return;
		}
	}
	display.iBeam = XFALSE;
	if (mouseIsDown && focus && !focus->editText->m_selecting) {
		cursorBlink = XFALSE;
		//if(focus->OnEvent(this,XOBJEVENT_KILLFOCUS,hitBtn))
		//	bAction=XTRUE;
		focus->editText->ClearSelection();
		focus->Modify();
		focus = 0;
	}*/
	if (hitBtn && hitBtn->IsEditText() ) {
		if (mouseIsDown) {
			if (!IsSelecting()) {
				XSetCapture();
				if (focus != hitBtn) {
					if (focus) {
						cursorBlink = XFALSE;
						//if(focus->OnEvent(this,XOBJEVENT_KILLFOCUS,hitBtn))
						//	bAction=XTRUE;
						focus->editText->ClearSelection();
						focus->Modify();
					}
					focus = hitBtn;
					//if(focus -> OnEvent(this,XOBJEVENT_SETFOCUS,hitBtn))
					//	bAction=XTRUE;
				}
				//focus->EditTextMouse(pt, XTRUE);
				focus->Modify();
			}
		} else {
			XReleaseCapture();
		}
		display.iBeam = ((hitBtn->editText->m_flags & seditTextFlagsNoSelect) == 0);
		//hitBtn = 0;
	} else {
		display.iBeam = XFALSE;
		if (mouseIsDown && focus && !focus->editText->m_selecting) {
			cursorBlink = XFALSE;
			focus->editText->ClearSelection();
			//if(focus->OnEvent(this,XOBJEVENT_KILLFOCUS,hitBtn))
			//	bAction=XTRUE;
			focus->Modify();
			focus = NULL;
		}
	}
#endif

	if ( hitBtn ) {
		// Check object is really a button
		//FLASHASSERT(hitBtn->character->type == buttonChar);
		if ( !hitBtn->character ) {
			FLASHASSERT(hitBtn->character);
			return;
		}

		if ( hitBtn->character->type != buttonChar&&
			 !hitBtn->HasMouseEvent() ) {

			FLASHASSERT(hitBtn->character->type == buttonChar);
			return;
		}

	}

	//FLASHASSERT(!hitBtn || hitBtn->character->type == buttonChar || hitBtn->HasMouseEvent());

	if ( display.SetButton(hitBtn, mouseIsDown) ) {

		if(hitBtn&&mouseIsDown)
		{
			hitBtn->PostEvent(0,this,XOBJEVENT_MOUSEDOWN,0);
			hitBtn->PostEvent(0,this,XOBJEVENT_MOUSEUP,0);
		}

		if (!RectIsEmpty(&display.tabFocusRect)) {
			display.InvalidateRect(&display.tabFocusRect);
		}
		if (hitBtn) {
			hitBtn->CalcButtonBounds(&display.tabFocusRect);
			display.InvalidateRect(&display.tabFocusRect);
		} else {
			RectSetEmpty(&display.tabFocusRect);
		}

		bAction=XTRUE;

	}


	if(bAction) DoActions();

	mouseState = mouseIsDown;
}

//#include <stdafx.h>
SObject* XSWFPlayer::DoButton(_XSObject*&target,SPOINT* pt,int mouseIsDown, XU32 t,BOOL updateScreen)
{

	//SObject* hitClip=XNULL;
	SObject*hitObj=NULL;
	SObject* hitBtn = display.HitObject(pt,&hitObj,0);//display.HitButton(hitClip,pt);
	target=hitBtn;
	//SObject* ret=hitBtn?hitBtn:hitClip;
	//if(hitBtn) hitClip=XNULL;
	if(hitBtn)
	{
		//_XSObject*lo=hitBtn;
		//if(hitBtn&&hitBtn!=&display.root)
		//	hitObj=hitBtn;
		// Mybe error!!!
		target=target->AS3GetTargetObject(true);
		if(target)
		{
			hitBtn=target->AS3GetEventObject(true);
				//hitBtn->AS3GetEventObject(true);
//#if (__CORE_VERSION__>=0x02076000)
			if(hitBtn&&(/*hitBtn->IsButtonMode()||*/!hitBtn->IsMouseChildren()))
				target=hitBtn;
			else if(hitObj&&hitObj!=target&&hitObj->pLink==hitBtn)
				target=hitObj->AS3GetTargetObject(true);
//#else
//			if(hitObj&&hitObj!=target&&hitObj->pLink==hitBtn)
//				target=hitObj->AS3GetTargetObject(true);
//#endif
		}
		else
			hitBtn=XNULL;
	}

	if(!updateScreen) 
		return hitBtn;

	XBOOL bAction=SetFocus(hitBtn);
//	hitClip=XNULL;

	//if(hitBtn==XNULL) hitBtn=hitClip;

#ifdef EDITTEXT
	 if(hitBtn&&hitBtn->IsEditText())
	 {
		   nDownTime=0;
		   //if(hitBtn->editText->IsEditAble())
			 //  Trace(XString16("Edit Start"));
#ifdef _FULLEDIT
		   if(mouseIsDown==2&&pWindow&&hitBtn->editText->IsEditAble())
			{
				hitBtn->editText->SelectAll();
				pWindow->PostMsg(XWM_STARTEDIT,hitBtn->editText->GetEditStyle(),
								 (XU32)hitBtn->editText->m_buffer,XMS_TOPMOST);
			}
#endif
		if (hitBtn->editText->IsSelectable()) {
			if (mouseIsDown==1) {
				if (!IsSelecting()) {
					XSetCapture();
					if (focus != hitBtn) {
						if (focus) {
							cursorBlink = XFALSE;
							//if(focus->OnEvent(this,XOBJEVENT_KILLFOCUS,hitBtn))
							//	bAction=XTRUE;
							focus->editText->ClearSelection();
							focus->Modify();
						}
						focus = hitBtn;
						//if(focus -> OnEvent(this,XOBJEVENT_SETFOCUS,hitBtn))
						//	bAction=XTRUE;
					}
					focus->EditTextMouse(pt, XTRUE);
					focus->Modify();
				}
			} else {
				XReleaseCapture();
			}
			display.iBeam = ((hitBtn->editText->m_flags & seditTextFlagsNoSelect) == 0);

			//hitBtn = 0;
		} else {
			display.iBeam = XFALSE;
			if (mouseIsDown==1 && focus && !focus->editText->m_selecting) {
				cursorBlink = XFALSE;
				focus->editText->ClearSelection();
				//if(focus->OnEvent(this,XOBJEVENT_KILLFOCUS,hitBtn))
				//	bAction=XTRUE;
				focus->Modify();
				focus = NULL;
			}
		}
	}
	 else
	 {
			display.iBeam = XFALSE;
			if (mouseIsDown==1 && focus && !focus->editText->m_selecting) {
				cursorBlink = XFALSE;
				focus->editText->ClearSelection();
				//if(focus->OnEvent(this,XOBJEVENT_KILLFOCUS,hitBtn))
				//	bAction=XTRUE;
				focus->Modify();
				focus = NULL;
			}
	 }
#endif

	/*if(hitBtn) hitClip=XNULL;
	if(display.SetClip(hitClip,mouseIsDown))
	{
		bAction=XTRUE;
		if (!RectIsEmpty(&display.tabFocusRect)) {
			display.InvalidateRect(&display.tabFocusRect);
			RectSetEmpty(&display.tabFocusRect);
		}
	}*/
	/*if(hitBtn==XNULL)
	{
		hitBtn=hitClip;
	}*/

	XBOOL bSet=display.SetButton(hitBtn, mouseIsDown==1);

	//hitObj=mouseIsDown?hitBtn:XNULL;
	//if(display.SetButton(hitBtn,mouseIsDown,1))
	//	bSet=XTRUE;

	if ( bSet ) {
		if (!RectIsEmpty(&display.tabFocusRect)) {
			display.InvalidateRect(&display.tabFocusRect);
			RectSetEmpty(&display.tabFocusRect);
		}
		//if ( updateScreen )
		//	UpdateScreen();
		//UpdateCursor();
		bAction=XTRUE;

		if ( (display.button) && mouseIsDown==1 ) {
			XSetCapture();
		} else {
			XReleaseCapture();
		}
	}

	SPOINT dpt = *pt;

	if ( !RectPointIn(&display.devViewRect, &dpt) ) {
		mouseState = -1;
	} else {
		mouseState = mouseIsDown==1;
		mousePt = *pt;
	}
	if(bAction) DoActions();
	return hitBtn;
}

//#ifdef DRAGBUTTONS
#if defined(DRAGBUTTONS) && !defined(GENERATOR)
void XSWFPlayer::UpdateDragObject(SPOINT *pt)
{
	if (dragObject) {
		SObject *parentObject = dragObject->thread->rootObject;
		//
		// Let's figure out where we need to position this
		// sprite.
		//
		MATRIX m, invMat;
		SObject *current;

		MatrixIdentity(&m);
		current = parentObject->parent;
		while ( current && (current != &display.root) ) {
			MatrixConcat( &m, &current->xform.mat, &m );
			current = current->parent;
		}
		MATRIX viewmat;
		
		viewmat = display.camera.mat;
		
		MatrixConcat(&m, &viewmat, &m);

		MatrixInvert(&m, &invMat);

		SPOINT delta, xformedDelta, newPt;

		// If this drag is of the lock-to-center variety...
		if (dragObject->dragCenter) {
			MatrixTransformPoint(&invMat, pt, &newPt);
		} else {
			delta.x = dragPoint.x - pt->x;
			delta.y = dragPoint.y - pt->y;
			MatrixDeltaTransformPoint(&invMat, &delta, &xformedDelta);

			newPt.x = parentObject->xform.mat.tx - xformedDelta.x;
			newPt.y = parentObject->xform.mat.ty - xformedDelta.y;
		}

		SRECT* constraint = &parentObject->dragConstraint;
		if (!RectIsEmpty(constraint)) {
			if (newPt.x < constraint->xmin) {
				newPt.x = constraint->xmin;
			} else if (newPt.x > constraint->xmax) {
				newPt.x = constraint->xmax;
			}
			if (newPt.y < constraint->ymin) {
				newPt.y = constraint->ymin;
			} else if (newPt.y > constraint->ymax) {
				newPt.y = constraint->ymax;
			}

			if (dragObject->dragCenter) {
				MatrixTransformPoint(&m, &newPt, pt);
			} else {
				xformedDelta.x = parentObject->xform.mat.tx - newPt.x;
				xformedDelta.y = parentObject->xform.mat.ty - newPt.y;

				MatrixDeltaTransformPoint(&m, &xformedDelta, &delta);

				pt->x = dragPoint.x - delta.x;
				pt->y = dragPoint.y - delta.y;
			}
		}

		parentObject->xform.mat.tx = newPt.x;
		parentObject->xform.mat.ty = newPt.y;

		parentObject->Modify();

		dragPoint = *pt;

		UpdateDropTarget();
	}
}
#endif

void XSWFPlayer::MouseMove(XU32 t, int x, int y, BOOL mouseIsDown,BOOL isRepeat)
{
	
	SPOINT pt;
	pt.x = x;
	pt.y = y;
	/*static bool _isFirst = true;
	if(!_isFirst) return;
	_isFirst = false;*/

	if(downling&&m_nMode==MV_NONE)
	{
		int dx=x-scrollPt.x;
		int dy=y-scrollPt.y;
		if(dx<0) dx=-dx;
		if(dy<0) dy=-dy;
		if(dx>10||dy>10)
			downling=false;
	}
	// If we're doing a partial first frame, don't accept mouse downs yet
	if (!loaded||downling||m_nMode!=MV_NONE) {
		return;
	}
	

//	XLock lock(&m_dataLock);
//	if(!lock.Lock(XWAIT_TIME)) return;

	gmousePt.x=x;
	gmousePt.y=y;

	if(m_nLockMode!=LK_SELECT&&m_nMode==MV_NONE
//#ifdef _LOWSPEED
//		&&!m_bNoScript
//#endif
		)
	{
		XBOOL bOK=m_pAVM2?XFALSE:display.root.PostEvent(t,this,XOBJEVENT_MOUSEMOVE,0);

		if(HandleListener(XOBJEVENT_MOUSEMOVE))
			bOK=XTRUE;
		if(bOK)
			DoActions();

	//#ifdef DRAGBUTTONS
	#if defined(DRAGBUTTONS) && !defined(GENERATOR)
		if (dragObject) {
//#if (__CORE_VERSION__>=0x02074000)
			SPOINT pt1=pt;
			UpdateDragObject(&pt1);
//#else
//			UpdateDragObject(&pt);
//#endif
		}
	#endif

	#ifdef EDITTEXT
		if (IsSelecting()) {
			focus->EditTextMouse(&pt, mouseIsDown);
			focus->Modify();
	//		lock.UnLock();
			return;
		}
	#endif
	}

	if(false)//mouseIsDown&&nDownTime&&!dragObject)
	{//Need Scrolling
		/*if(display.button)
			nDownTime=0;
		else*/
		{
			int dx=x-downPt.x;
			int dy=y-downPt.y;
			if(dx<0) dx=-dx;
			if(dy<0) dy=-dy;
			if(dx>16||dy>16)
			{
				nDownTime=0;
				bool bEvent=false;
				if(m_pFocus)//display.button)
				{
					if(m_pFocus->pASObject)//display.button->pASObject)
					{
						DisplayObjectObject*pObj=(DisplayObjectObject*)m_pFocus->pASObject;//display.button->pASObject;
						if(pObj->hasEvent(XAS3_EVENT_mouseMove)||
						   pObj->hasEvent(XAS3_EVENT_rollOver)||
						   pObj->hasEvent(XAS3_EVENT_rollOut)||
						   pObj->hasEvent(XAS3_EVENT_mouseOver)||
						   pObj->hasEvent(XAS3_EVENT_mouseOut))
							bEvent=true;
					}
					else if(display.button->HasMouseEvent())
						bEvent=true;
				}
				if(!bEvent)//&&scaleMode!=display.noBorder)
				{
//#ifdef _LOWSPEED
//					this->LockZoomBitmap();
//#endif
					scrollPt.x=downPt.x;// = pt;
					scrollPt.y=downPt.y;
					cameraPt.x=display.mControlMat.tx;
					cameraPt.y=display.mControlMat.ty;
					scrolling = XTRUE;
					downling = XFALSE;
					scrollTime=m_nRunTime;
					isMoveScroll=XTRUE;
					XSetCapture();
				}
			}
		}
	}

	if ( scrolling ) {
		// Track scrolling when zoomed in
		/*FLASHASSERT(mouseIsDown);
		MATRIX m, invMat;
		if ( display.antialias_b ) {
			switch(display.antiShift)
			{
			case 2:
				MatrixScale(fixed_1/4, fixed_1/4, &m);
				break;
			case 1:
				MatrixScale(fixed_1/2, fixed_1/2, &m);
				break;
			case 3:
				MatrixScale(fixed_1/8, fixed_1/8, &m);
				break;
			}
			//MatrixScale(fixed_1/4, fixed_1/4, &m);
			MatrixConcat(&display.camera.mat, &m ,&m);
		} else {
			m = display.camera.mat;
		}
		MatrixInvert(&m, &invMat);

		SRECT viewRect, docRect;

		//viewRect.xmin=0;
		//viewRect.ymin=0;
		//viewRect.xmax=devSize.cx;
		//viewRect.ymax=devSize.cy;

		ClientRect(viewRect);
		MatrixTransformRect(&m, &player->frame, &docRect);

		// Calculate the scroll limits
		SRECT limit;
		limit.xmin = docRect.xmin - viewRect.xmin;
		if ( limit.xmin > 0 ) limit.xmin = 0;
		limit.xmax = docRect.xmax - viewRect.xmax;
		if ( limit.xmax < 0 ) limit.xmax = 0;
		limit.ymin = docRect.ymin - viewRect.ymin;
		if ( limit.ymin > 0 ) limit.ymin = 0;
		limit.ymax = docRect.ymax - viewRect.ymax;
		if ( limit.ymax < 0 ) limit.ymax = 0;

		SPOINT delta;
		delta.x = scrollPt.x - pt.x;
		delta.y = scrollPt.y - pt.y;
		if ( delta.x < limit.xmin )
			delta.x = limit.xmin;
		else if ( delta.x > limit.xmax )
			delta.x = limit.xmax;

		if ( delta.y < limit.ymin )
			delta.y = limit.ymin;
		else if ( delta.y > limit.ymax )
			delta.y = limit.ymax;

		// Show the updated view after a 1/2 second pause
		zoomRect = viewRect;
		RectOffset(delta.x, delta.y, &zoomRect);
		MatrixTransformRect(&invMat, &zoomRect, &zoomRect);
		SetCamera(updateNow);*/

		//UpdateScreen();
		int dx=pt.x-scrollPt.x;
		int dy=pt.y-scrollPt.y;

		//dx=dx*20;//m_fScaleX;
		//dy=dy*20;//m_fScaleY;
		MATRIX&mat=display.mControlMat;//camera.mat;
		int ox=mat.tx;
		int oy=mat.ty;
		mat.tx=cameraPt.x+dx;
		mat.ty=cameraPt.y+dy;

		if(mat.tx>0) mat.tx=0;
		if(mat.ty>0) mat.ty=0;
		if(mat.tx<-m_nLimitX) mat.tx=-m_nLimitX;
		if(mat.ty<-m_nLimitY) mat.ty=-m_nLimitY;

//#ifdef _LOWSPEED
//		this->m_bUpdateDrag=true;
//		UpdateDragDib();
//#else
		if(ox!=mat.tx||oy!=mat.ty)
			display.UpdateMat();
			//display.ModifyCamera(false);
//#endif
		//scrollPt = pt;
	} else if(m_nLockMode!=LK_SELECT
//#ifdef _LOWSPEED
//		&&!m_bNoScript
//#endif
		){
		//if(mouseIsDown) return;
		//_XSObject* old=display.button;

		//SPOINT local=pt;
		_XSObject*target=XNULL;
		_XSObject*newBut=DoButton(target,&pt, mouseIsDown,!mouseIsDown);
		bool bSet = false;
		if(m_pAVM2)
		{
			//_XSObject*newBut=display.button;
			MOUSEEVENTDATA data;
			if(m_pActiveObject&&m_pActiveObject->pASObject)
			{
				//if(!m_pActiveTarget&&m_pActiveTarget->pASObject)//Object->IsParentOf(newBut))
				{
					//MOUSEEVENTDATA data;
					void*pContext=m_pActiveObject->GetContext();
					//ScriptPlayer*player=m_pActiveObject->GetPlayer();
					//if(player)
					//	pContext=player->m_pContext;
					if(m_pActiveTarget!=target)
					{
						//MOUSEEVENTDATA data;
						AS3InitMouseData(data,target,mouseIsDown!=0,m_pActiveTarget?m_pActiveTarget->pASObject:XNULL,XNULL,XNULL);//newBut?newBut->pASObject:XNULL);
                        if (m_pActiveObject->character && m_pActiveObject->character->type != 1 ) {
                            m_pAVM2->OnMouseEvent(XAS3_EVENT_mouseOut,data,m_pActiveObject->pASObject,pContext);
							bSet = true;
                        }
                    }
					if(m_pActiveObject!=newBut&&!m_pActiveObject->IsParentOf(newBut))
					{
						//MOUSEEVENTDATA data;
						//data.pStop=newBut?newBut->pASObject:XNULL;
						//data.pTarget=m_pActiveObject->pASObject;
//#if (__CORE_VERSION__>=0x02070200) //Roll out Event send method error!modify
						XVector<_XSObject*> objs;
						_XSObject*obj=m_pActiveObject;
						while(obj)
						{
							if(obj==newBut||obj->IsParentOf(newBut)) break;
							if(obj->pASObject&&m_pAVM2->HasEvent(obj->pASObject,XAS3_EVENT_rollOut))
								objs.Add(obj);
							obj=obj->parent;
						}
						int i,nCount=objs.GetSize()-1;
						for(i=0;i<=nCount;i++)//nCount;i>=0;i--)
						{
							obj=objs[i];
							AS3InitMouseData(data,obj,mouseIsDown!=0,obj->pASObject,XNULL,(_XSObject*)2);
							//[Note X]
							data.bWithTarget=true;
							m_pAVM2->OnMouseEvent(XAS3_EVENT_rollOut,data,obj->pASObject,pContext);
						}
//#else
//						AS3InitMouseData(data,m_pActiveTarget,mouseIsDown!=0,m_pActiveTarget->pASObject,XNULL,m_pActiveObject->GetCommonParent(newBut));
//						//[Note X]
//						data.bWithTarget=true;
//						m_pAVM2->OnMouseEvent(XAS3_EVENT_rollOut,data,m_pActiveObject->pASObject,pContext);
//#endif
					}
					//int lx,ly;
					//this->GetMousePos(m_pActiveObject,lx,ly);
					//void*pEvent=AS3CreateMouseEvent(avmshell::XAS3_EVENT_mouseOut,m_pActiveObject,mouseIsDown!=0,
					//	m_pActiveObject->pASObject);
					//m_pAVM2->DispatchEvent(pEvent,m_pActiveObject->pASObject);
					//m_pAVM2->ResetEvent(pEvent,avmshell::XAS3_EVENT_rollOut,m_pActiveObject->pASObject);
					//m_pAVM2->DispatchEvent(pEvent,m_pActiveObject->pASObject);
				}

			}
			if(newBut&&newBut->pASObject)
			{
				
				void*pContext=newBut->GetContext();//NULL;
				AS3InitMouseData(data,target,mouseIsDown!=0,target->pASObject,XNULL,XNULL);
				if(!isRepeat)
				{
					//
					m_pAVM2->OnMouseEvent(avmshell::XAS3_EVENT_mouseMove,data,newBut->pASObject,pContext);
				}
				if(newBut==m_pActiveObject&&target!=m_pActiveTarget)
				{
					if(!newBut->IsParentOf(target))
					{
						//m_pActiveObject=NULL;
						//m_pActiveTarget=NULL;
						//AddEvent(XMOUSEMOVE,x,y,(mouseIsDown?1:0)|0x40000,0,true);
						//_XSObject*target1=XNULL;
						_XSObject*target1 = NULL;
						_XSObject*newBut1=DoButton(target1,&pt, mouseIsDown,!mouseIsDown);
						if(newBut1!=newBut)
						{
							m_pActiveObject=NULL;
							m_pActiveTarget=NULL;
							AddEvent(XMOUSEMOVE,x,y,(mouseIsDown?1:0)|0x40000);
							return;
						}
						//return;
					}
				}
				if(target!=m_pActiveTarget)
				{
					//AS3InitMouseData(data,target,mouseIsDown!=0,target->pASObject,XNULL,XNULL);
					m_pAVM2->OnMouseEvent(avmshell::XAS3_EVENT_mouseOver,data,newBut->pASObject,pContext);
					bSet = true;
					//AddEvent(XMOUSEMOVE,x,y,(mouseIsDown?1:0)|0x40000);
				}
				
				if(newBut!=m_pActiveObject&&!newBut->IsParentOf(m_pActiveObject))
				{
					AS3InitMouseData(data,target,mouseIsDown!=0,target->pASObject,XNULL,newBut->GetCommonParent(m_pActiveObject));
					//[Note X]
					data.bWithTarget=true;
					m_pAVM2->OnMouseEvent(XAS3_EVENT_rollOver,data,newBut->pASObject,pContext);
				}
			}
			else
			{	void*pASObject=display.root.pASObject;
				if(pASObject)
				{
					//MOUSEEVENTDATA data;
					AS3InitMouseData(data,target,mouseIsDown!=0,display.root.pASObject,XNULL,XNULL);
					m_pAVM2->OnMouseEvent(XAS3_EVENT_mouseMove,data,pASObject,player->m_pLoaderInfo->m_codeContext);//m_pContext);
				}
			}

		}
		m_pActiveObject=newBut;
		m_pActiveTarget=target;
		/*if(bSet)
		{
			_XSObject*target1 = NULL;
			_XSObject*newBut1=DoButton(target1,&pt, mouseIsDown,!mouseIsDown);
			if(target1!=target)
				AddEvent(XMOUSEMOVE,x/20,y/20,(mouseIsDown?1:0)|0x40000);
		}*/
	}
	//XBOOL bAction=XFALSE;
	//SObject*fc=focus?focus:display.buttonThread;
	//if(fc)
	//{
	//	if(fc->OnEvent(this,XOBJEVENT_MOUSEMOVE,XNULL))
	//		bAction=XTRUE;
	//}
	//if(HandleListener(XOBJEVENT_MOUSEMOVE))
	//	bAction=XTRUE;
	//if(bAction)
	//	DoActions();

//	lock.UnLock();
}

//enum {LEFT,TOPLEFT,DOWNLEFT,RIGHT,TOPRIGHT,DOWNRIGHT,TOP,DOWN};
//	int  m_nDestX,m_nDestY,m_nMode;
//	void SetMoveMode(int nMode);

#define MOVE_OFFSET 200

bool XSWFPlayer::StopMove()
{
	//#ifdef _LOWSPEED
	//	this->UnlockZoomBitmap(false);
	//#endif
	if(m_nMode==MV_NONE) return false;

	bool bModify=false;
	downling = XFALSE;
	scrolling = XFALSE;
	MATRIX&mat=display.mControlMat;//.camera.mat;
	if(mat.tx>0)
	{
		mat.tx=0;
		bModify=true;
	}
	if(mat.ty>0)
	{
		mat.ty=0;
		bModify=true;
	}
	if(mat.tx<-m_nLimitX)
	{
		mat.tx=-m_nLimitX;
		bModify=true;
	}
	if(mat.ty<-m_nLimitY)
	{
		mat.ty=-m_nLimitY;
		bModify=true;
	}
	m_nMode=MV_NONE;
	if(bModify)
	{
		display.UpdateMat();
		//display.ModifyCamera(false);
	}
	return true;
}

void XSWFPlayer::SetMoveMode(int nMode)
{

	m_nMode=nMode;
	if(m_nMode==MV_NONE)
	{
//#ifdef _LOWSPEED
//		this->UnlockZoomBitmap(false);
//#endif
		return;
	}
	downling = XFALSE;
//#ifdef _LOWSPEED
//	this->LockZoomBitmap();
//#endif
	// Calculate the scroll limits
	MATRIX&mat=display.camera.mat;
	switch(nMode)
	{
	case MV_LEFT:
		 m_nDestX=0;
		 m_nDestY=mat.ty;
		 m_nMidX=m_nDestX+MOVE_OFFSET;
		 m_nMidY=m_nDestY;
		 break;
	case MV_TOPLEFT:
		 m_nDestX=0;
		 m_nDestY=0;
		 m_nMidX=m_nDestX+MOVE_OFFSET;
		 m_nMidY=m_nDestY+MOVE_OFFSET;
		 break;
	case MV_DOWNLEFT:
		 m_nDestX=0;
		 m_nDestY=-m_nLimitY;
		 m_nMidX=m_nDestX+MOVE_OFFSET;
		 m_nMidY=m_nDestY-MOVE_OFFSET;
		 break;
	case MV_TOP:
		 m_nDestX=mat.tx;
		 m_nDestY=0;
		 m_nMidX=m_nDestX;
		 m_nMidY=m_nDestY+MOVE_OFFSET;
		 break;
	case MV_TOPRIGHT:
		 m_nDestX=-m_nLimitX;
		 m_nDestY=0;
		 m_nMidX=m_nDestX-MOVE_OFFSET;
		 m_nMidY=m_nDestY+MOVE_OFFSET;
	case MV_RIGHT:
		 m_nDestX=-m_nLimitX;
		 m_nDestY=mat.ty;
		 m_nMidX=m_nDestX-MOVE_OFFSET;
		 m_nMidY=m_nDestY;//-MOVE_OFFSET;
		 break;
	case MV_DOWNRIGHT:
		 m_nDestX=-m_nLimitX;
		 m_nDestY=-m_nLimitY;
		 m_nMidX=m_nDestX-MOVE_OFFSET;
		 m_nMidY=m_nDestY-MOVE_OFFSET;
		 break;
	case MV_DOWN:
		 m_nDestX=mat.tx;
		 m_nDestY=-m_nLimitY;
		 m_nMidX=m_nDestX;//+MOVE_OFFSET;
		 m_nMidY=m_nDestY-MOVE_OFFSET;
	}
	/*SRECT limit;
	limit.xmin = docRect.xmin - viewRect.xmin;
	if ( limit.xmin > 0 ) limit.xmin = 0;
	limit.xmax = docRect.xmax - viewRect.xmax;
	if ( limit.xmax < 0 ) limit.xmax = 0;
	limit.ymin = docRect.ymin - viewRect.ymin;
	if ( limit.ymin > 0 ) limit.ymin = 0;
	limit.ymax = docRect.ymax - viewRect.ymax;
	if ( limit.ymax < 0 ) limit.ymax = 0;*/
}

//#define _PREVSHOW

//#ifdef _LOWSPEED
//BOOL XSWFPlayer::LockZoomBitmap()
//{
//	//XLock lock(&display.raster.dibLock);//m_dataLock);
//	//if(!lock.Lock(XNORMALTIME)) return XFALSE;
//	return XFALSE;
//	/*if(!display.raster.dibLock.Lock(XNORMALTIME)) return XFALSE;
//	if(m_bDragOrZoom)
//	{
//		display.raster.dibLock.UnLock();
//		return XFALSE;
//	}
//
//#ifndef _PREVSHOW
//	MATRIX*mat=&display.camera.mat;
//	m_lockMatrix=*mat;
//	m_bDragOrZoom=true;
//	int w=bits.width(),h=bits.height();
//	if(m_dragDib.IsEmpty())
//		m_dragDib.Create(w,h,32);
//	else
//	{
//		int l=w*h*4;
//		if(l>(int)m_dragDib.m_nDataLength)
//		{
//			m_dragDib.Release();
//			m_dragDib.Create(w,h,32);
//		}
//		else
//		{
//			XSIZE sz={w,h};
//			m_dragDib.Resize(sz);
//		}
//	}
//	{
//		XRect rect(0,0,bits.width(),bits.height());
//
//		m_dragDib.Copy(*bits.bitmap,0,0,rect);
//
//	}
//	m_nMulti0=mat->a;
//	int b=display.antialias_b?display.antiShift:0;
//	int half=b>1?(1<<(b-1)):0;
//	int tx=((-mat->tx)+half)>>b;
//	int ty=((-mat->ty)+half)>>b;
//	this->m_dragOffset.x=tx;
//	this->m_dragOffset.y=ty;
//#else
//	int w=RectWidth(&player->frame);
//	int h=RectHeight(&player->frame);
//	MATRIX*mat=&display.camera.mat;
//	m_bDragOrZoom=true;
//	int b=display.antiShift;
//	int half=b>1?(1<<(b-1)):0;
//	w=(FixedMul(w,mat->a)+half)>>b;
//	h=(FixedMul(h,mat->d)+half)>>b;
//
//	if(m_dragDib.IsEmpty()||m_dragDib.Width()!=w||m_dragDib.Height()!=h)
//	{
//		if(!m_dragDib.HasBits()||w*h*4>m_dragDib.m_nDataLength)
//		{
//			m_dragDib.Release();
//			m_dragDib.Create(w,h,32);
//		}
//		else
//		{
//			XSIZE sz={w,h};
//			m_dragDib.Resize(sz);
//		}
//		//m_dragDib.Reset();
//	}
//	XDrawDib*pDib=bits.bitmap;
//	m_nMulti0=display.camera.mat.a;
//
//	if(w==pDib->Width()&&h==pDib->Height())
//	{
//		XRect rect(0,0,w,h);
//		m_dragDib.Copy(*pDib,0,0,rect);
//	}
//	else
//	{
//
//
//
//		int lw=bits.width();
//
//
//		//lw=lw/4;
//		MATRIX oldMat=display.camera.mat;
//
//		XBitBuffer bitBuf;
//		bitBuf.bitmap=&m_dragDib;
//		mat->tx=0;
//		mat->ty=0;
//		mat->a>>=b;
//		mat->d>>=b;
//		//display.antialias_b
//		//display.root.Modify();
//		display.antialias_b=0;
//		display.antiShift=0;
//		display.ModifyCamera();
//		display.bits=&bitBuf;
//		display.CalcUpdate();
//
//		{
//			SRECT rect={0,w,0,h};
//			display.UpdateRect(&rect);
//		}
//		
//
//		display.antiShift=b?b:0;
//		display.antialias_b=b?XTRUE:XFALSE;
//		display.bits=&bits;
//		display.camera.mat=oldMat;
//		bitBuf.bitmap=NULL;
//		::FreeCache(&display.root);
//		display.ModifyCamera();
//
//		int sw=lw;
//		int tx=mat->tx;
//		int ty=mat->ty;
//		int ox=(-tx)>>b;
//		int oy=(-ty)>>b;
//		XU32*pSrt=(XU32*)bits.bitmap->GetBuffer();
//		if(ox<0)
//		{
//			pSrt-=ox;
//			lw+=ox;
//			ox=0;
//		}
//
//		int lh=bits.height();
//		if(oy<0)
//		{
//			pSrt-=(oy*sw);
//			lh+=oy;
//			oy=0;
//		}
//
//		if(ox+lw>w) lw=w-ox;
//		if(oy+lh>h) lh=h-oy;
//		int x,y;
//		lw=lw*4;
//
//		XU32*pDst=((XU32*)m_dragDib.GetBuffer())+oy*w+ox;
//		for(y=0;y<lh;y++)
//		{
//			XGlobal::Memcpy(pDst,pSrt,lw);
//			pDst+=w;
//			pSrt+=sw;
//		}
//
//	}
//#endif
//	//lock.UnLock();
//	display.raster.dibLock.UnLock();
//	return XTRUE;*/
//}
//BOOL XSWFPlayer::UnlockZoomBitmap(bool bUpdate)
//{
//	return XFALSE;
//	/*if(!display.raster.dibLock.Lock(XNORMALTIME)) return XFALSE;
//	if(!m_bDragOrZoom)
//	{
//		display.raster.dibLock.UnLock();
//		return XFALSE;
//	}
//	m_dragDib.Release();
//	m_bDragOrZoom=false;
//	display.raster.dibLock.UnLock();
//	//if NO memory
//	
//
//	if(bUpdate)
//	{	//SetCamera(updateNow);
//		GCENTER
//		display.ModifyCamera();
//		display.Update();
//		m_bUpdateRedraw=XTRUE;
//
//	}
//	else
//	{
//		display.ModifyCamera();
//
//	}
//	return XTRUE;*/
//}
//#endif

//XBOOL XSWFPlayer::LockCenter(SPOINT&center,MATRIX&mat)
//{
//	MATRIX invMat;
//	
//	::MatrixInvert(&mat,&invMat);
//	SPOINT centerView;
//	::MatrixTransformPoint(&invMat,&center,&centerView);
//	::MatrixTransformPoint(&display.camera.mat,&centerView,&centerView);
//	int tx=display.camera.mat.tx+(center.x-centerView.x);
//	int ty=display.camera.mat.ty+(center.y-centerView.y);
//	if(tx>0) tx=0;
//	if(ty>0) ty=0;
//	if(tx<-m_nLimitX) tx=-m_nLimitX;
//	if(ty<-m_nLimitY) ty=-m_nLimitY;
//	if(tx==display.camera.mat.tx&&
//		ty==display.camera.mat.ty) return XFALSE;
//	display.camera.mat.tx=tx;
//	display.camera.mat.ty=ty;
//
//	//display.ModifyCamera();
//	return XTRUE;
//}

//XBOOL XSWFPlayer::NewCenter(SPOINT&center)
//{
//	//if(!LockCenter(center,display.camera.mat)) return XFALSE;
//
//	MATRIX&mat=display.camera.mat;
//	int oldx=mat.tx;
//	int oldy=mat.ty;
//	mat.tx+=center.x;
//	mat.ty+=center.y;
//	if(mat.tx>0) mat.tx=0;
//	if(mat.ty>0) mat.ty=0;
//	if(mat.tx<-m_nLimitX) mat.tx=-m_nLimitX;
//	if(mat.ty<-m_nLimitY) mat.ty=-m_nLimitY;
//
//	if(oldx==mat.tx&&oldy==mat.ty)
//		return XTRUE;
//
//	display.ModifyCamera();
//
//	if(!m_bDragOrZoom)
//	{
//		display.Update();
//		//m_bUpdateRedraw=XTRUE;
//	}
//	else
//	{
//		m_bUpdateDrag=true;
//		UpdateDragDib();
//	}
//	return XTRUE;
//}

//void XSWFPlayer::UpdateDragDib()
//{
//
//		/*_ANALYENTER("UpdateDragDib");
//#ifdef _LOWSPEED
//		if(m_bDragOrZoom)
//		{
//			if(display.raster.dibLock.Lock(XSHORTTIME))
//			{
//			//bits.bitmap->Reset();
//#ifndef _PREVSHOW
//			int l=0;//bits.width()*bits.height()*4;
//			//memset(bits.bitmap->m_pDib,0,l);
//			XRect rect(0,0,bits.width(),bits.height());
//			bits.bitmap->FillRect( rect, 0xff000000,0);
//			MATRIX*mat=&display.camera.mat;
//			int b=display.antiShift;
//			int nMulti=mat->a;
//			int nOffX=((-mat->tx)>>b);
//			int nOffY=((-mat->ty)>>b);
//			//if(nOffX<0) nOffX=0;
//			//if(nOffY<0) nOffY=0;
//			if(m_nMulti0==nMulti)
//			{
//				nOffX-=m_dragOffset.x;
//				nOffY-=m_dragOffset.y;
//				int w=bits.width();
//				int dw=w;
//				int h=bits.height();
//				int sw=m_dragDib.Width();
//				//int lw=sw-nOffX;
//				//int lh=m_dragDib.Height()-nOffY;
//				//if(w>lw) w=lw;
//				//if(h>lh) h=lh;
//				XU32*pDst=(XU32*)bits.bitmap->GetBuffer();
//				if(nOffX<0)
//				{
//					pDst-=nOffX;
//					w+=nOffX;
//					nOffX=0;
//				}
//				if(nOffY<0)
//				{
//					pDst-=(nOffY*sw);
//					h+=nOffY;
//					nOffY=0;
//					if(h<=0)
//					{
//						display.raster.dibLock.UnLock();
//						return;
//					}
//				}
//				if(nOffX+w>sw) w=sw-nOffX;
//				if(nOffY+h>(int)m_dragDib.Height())
//					h=m_dragDib.Height()-nOffY;
//
//				XU32*pSrt=((XU32*)m_dragDib.GetBuffer())+nOffY*sw+nOffX;
//				if(w>0)
//				{
//					int l=w*4;
//					int y;
//					for(y=0;y<h;y++)
//					{
//						//_ANALYENTER("");
//						XGlobal::Memcpy(pDst,pSrt,l);
//						//XGlobal::Memset(pDst,0xff,l);
//						pSrt+=sw;
//						pDst+=dw;
//					}
//				}
//			}
//			else //if(0)
//			{
//				if(nOffX)
//					nOffX=nOffX*m_nMulti0/nMulti;
//				if(nOffY)
//					nOffY=nOffY*m_nMulti0/nMulti;
//				nOffX-=m_dragOffset.x;
//				nOffY-=m_dragOffset.y;
//				//XRect rect(0,0,bits.width(),bits.height());
//				//bits.bitmap->Clear(rect,0xffff0000);
//				int deltax=(m_nMulti0<<XBITMAPOFF)/nMulti;
//				int deltay=(m_nMulti0<<XBITMAPOFF)/nMulti;
//				int w=bits.width();
//				int h=bits.height();
//				int dw=bits.width();
//				int sw=m_dragDib.Width();
//				XU32*pDst=(XU32*)bits.bitmap->GetBuffer();
//				if(nOffX<0)
//				{
//					int dd=(-nOffX<<XBITMAPOFF)/deltax;
//					w-=dd;
//					pDst+=dd;
//					nOffX=0;
//				}
//				if(nOffY<0)
//				{
//					int dd=(-nOffY<<XBITMAPOFF)/deltay;//)>>XBITMAPOFF;
//					h-=dd;
//					pDst+=(dd*dw);
//					nOffY=0;
//				}
//				int lw=((((sw-nOffX)<<XBITMAPOFF)/deltax));
//				int lh=((((m_dragDib.Height()-nOffY)<<XBITMAPOFF)/deltay));
//				if(w>lw) w=lw;
//				if(h>lh) h=lh;
//
//				XU32*pSrt=((XU32*)m_dragDib.GetBuffer())+nOffY*sw+nOffX;
//				int x,y,oy=0;//,ty=nOffY;//,ty=nOffY;
//				int delta=deltay/fixed_1;
//				deltay-=delta*fixed_1;
//				delta*=sw;
//				for(y=0;y<h;y++)
//				{
//					int ox=0;
//					for(x=0;x<w;x++)
//					{
//						pDst[x]=pSrt[ox>>XBITMAPOFF];
//						ox+=deltax;
//					}
//					pDst+=dw;
//					oy+=deltay;
//					pSrt+=delta;
//					if(oy>fixed_1)
//					{
//						oy-=fixed_1;
//						pSrt+=sw;
//						//ty++;
//						//if(ty>=m_dragDib.Height())
//						//	int v=0;
//					}
//				}
//			}
//#else
//			//bits.bitmap->Reset();
//			int l=bits.width()*bits.height()*4;
//			memset(bits.bitmap->m_pDib,0xff,l);
//			MATRIX*mat=&display.camera.mat;
//			int b=display.antiShift;
//			int nMulti=mat->a;
//			int nOffX=((-mat->tx)>>b);
//			int nOffY=((-mat->ty)>>b);
//			if(nOffX<0) nOffX=0;
//			if(nOffY<0) nOffY=0;
//			if(m_nMulti0==nMulti)
//			{
//				int w=bits.width();
//				int dw=w;
//				int h=bits.height();
//				int sw=m_dragDib.Width();
//				//int lw=sw-nOffX;
//				//int lh=m_dragDib.Height()-nOffY;
//				//if(w>lw) w=lw;
//				//if(h>lh) h=lh;
//				XU32*pDst=(XU32*)bits.bitmap->GetBuffer();
//				if(nOffX+w>sw) w=sw-nOffX;
//				if(nOffY+h>m_dragDib.Height())
//					h=m_dragDib.Height()-nOffY;
//
//				XU32*pSrt=((XU32*)m_dragDib.GetBuffer())+nOffY*sw+nOffX;
//				int l=w*4;
//				int x,y;
//				for(y=0;y<h;y++)
//				{
//					XGlobal::Memcpy(pDst,pSrt,l);
//					//XGlobal::Memset(pDst,0xff,l);
//					pSrt+=sw;
//					pDst+=dw;
//				}
//			}
//			else// if(0)
//			{
//				if(nOffX)
//					nOffX=nOffX*m_nMulti0/nMulti;
//				if(nOffY)
//					nOffY=nOffY*m_nMulti0/nMulti;
//				//XRect rect(0,0,bits.width(),bits.height());
//				//bits.bitmap->Clear(rect,0xffff0000);
//				int deltax=(m_nMulti0<<XBITMAPOFF)/nMulti;
//				int deltay=(m_nMulti0<<XBITMAPOFF)/nMulti;
//				int w=bits.width();
//				int h=bits.height();
//				int dw=bits.width();
//				int sw=m_dragDib.Width();
//				int lw=(((sw<<XBITMAPOFF)/deltax))-nOffX;
//				int lh=(((m_dragDib.Height()<<XBITMAPOFF)/deltay))-nOffY;
//				if(w>lw) w=lw;
//				if(h>lh) h=lh;
//				XU32*pDst=(XU32*)bits.bitmap->GetBuffer();
//				XU32*pSrt=((XU32*)m_dragDib.GetBuffer())+nOffY*sw+nOffX;
//				int x,y,oy=0;//,ty=nOffY;
//				for(y=0;y<h;y++)
//				{
//					int ox=0;
//					for(x=0;x<w;x++)
//					{
//						pDst[x]=pSrt[ox>>XBITMAPOFF];
//						ox+=deltax;
//					}
//					pDst+=dw;
//					oy+=deltay;
//					while(oy>fixed_1)
//					{
//						oy-=fixed_1;
//						pSrt+=sw;
//					}
//				}
//			}
//#endif
//
//			display.raster.dibLock.UnLock();
//			}
//		}
//#endif*/
//}

XBOOL XSWFPlayer::AutoObject(XSIZE&sz)
{
	return XFALSE;
	/*if(!m_optObject) return XFALSE;

	if(RectIsEmpty(&m_objRect))
	{
		m_optObject->GetBoundBox(m_objRect,XTRUE);
		//display.ToPixel(m_objRect);
	}

	if(RectIsEmpty(&m_objRect)) return XFALSE;
	int ox=m_devSize.cx;
	int oy=m_devSize.cy;
	int nx=m_objRect.xmax-m_objRect.xmin;
	int ny=m_objRect.ymax-m_objRect.ymin;
	if(ox==oy&&nx==ny)
	{
		//sz=m_devSize;
		return XFALSE;
	}
	MATRIX&mat=display.camera.mat;
	int fx=90*ox/nx;
	int fy=90*oy/ny;
	int ff=fx<fy?fx:fy;
	int f0=ff;
	//int f0=ff;
	int a=mat.a*ff;
	//int d=mat.d*ff;
	ff=((a)<<display.antiShift)/fixed_1;
	if(ff>m_nMaxPecent) 
		ff=m_nMaxPecent;
	SPOINT center;

	SRECT rect;
	MATRIX mat0;
	::MatrixIdentity(&mat0);
	m_optObject->GetBoundBox(rect,&mat0);
	center.x=(rect.xmin+rect.xmax)/2;
	center.y=(rect.ymin+rect.ymax)/2;
	this->SetZoomPecent(ff,sz,center,2);
	m_optObject=NULL;
	m_nLockMode=LK_NONE;
	if(pWindow)
	{
		pWindow->Handle(XCMD_UPDATEOBJ,ff+2,0);
		pWindow->Handle(XCMD_UPDATEOBJ,0,0);
	}
	return XTRUE;*/
}

void XSWFPlayer::SetZoomPecent( float zoomScale, SPOINT&pos )
{
	if ( zoomScale < 1 || zoomScale > 5 )
	{
		return;
	}

	display.ScaleTranMat( zoomScale, zoomScale,pos );
	//float fa = zoomScale;
	//float fd = zoomScale;
	//MATRIX&mControlMat = display.mControlMat;
	////center.x = center.x / 2;
	//float tx0 = (pos.x*20-mControlMat.tx)*65536.0/mControlMat.a;
	//float ty0 = (pos.y*20-mControlMat.ty)*65536.0/mControlMat.d;
	//mControlMat.a = fa * fixed_1;
	//mControlMat.d = fd * fixed_1;
	////mControlMat.tx = 
	//mControlMat.tx = pos.x*20-tx0*fa;
	//mControlMat.ty = pos.y*20-ty0*fd;
	CalcLimit();
	MATRIX&mControlMat = display.mControlMat;
	if(mControlMat.tx>0) mControlMat.tx=0;
	if(mControlMat.ty>0) mControlMat.ty=0;
	if(mControlMat.tx<-m_nLimitX) mControlMat.tx=-m_nLimitX;
	if(mControlMat.ty<-m_nLimitY) mControlMat.ty=-m_nLimitY;
	display.UpdateMat();
	//display.ScaleTranMat( zoomScale, zoomScale, center );
	//display.ModifyCamera(false);
	
	return;

}

//XBOOL XSWFPlayer::SetZoomPecent(int nPecent,XSIZE&sz,SPOINT&center,int nLockCenter)//bool bLockCenter)
//{
////	LOGWHEREVAL(m_nMaxPecent );
//	GCENTER;
//
//	if ( m_nMaxPecent >0 )
//	{
//		nPecent = m_nMaxPecent * nPecent / 100;
//	}
//	//LOGWHEREVAL(nPecent);
//	//LOGWHEREVAL( m_nMaxPecent );
//	//else
//	//{
//	//	int minPecent = m_nMaxPecent / 2;
//	//	nPecent = minPecent * nPecent / 100 + minPecent;
//	//}
//	MATRIX mat=m_bDragOrZoom?m_lockMatrix:display.camera.mat;
//	if(!nLockCenter)
//	{
//		center.x=this->m_winSize.cx/2;
//		center.y=this->m_winSize.cy/2;
//	}
//	if(nPecent==0)
//	{
//		SetZoom(0,sz);
//		m_nLimitX=0;
//		m_nLimitY=0;
//	}
//	else
//	{
//		////m_winSize=sz;
//		//if(nPecent)
//		//	scaleMode=DisplayList::fit100;
//		//else
//		//{
//		//	scaleMode=DisplayList::fitFull+1;
//		//}
//		int w=RectWidth(&player->frame)/20;
//		int h=RectHeight(&player->frame)/20;
//		SFIXED fx=FixedDivS(m_winSize.cx,w);
//		SFIXED fy=FixedDivS(m_winSize.cy,h);
//		SFIXED f;
//		int cx=w,cy=h;
//		if(nPecent<=100)
//		{
//			f=XMIN(XMIN(fx,fy),fixed_1);
//			f=f+(fixed_1-f)*nPecent/100;
//		}
//		else
//		{
//			f=fixed_1*nPecent/100;
//		}
//		cx=FixedMul(cx,f);
//		cy=FixedMul(cy,f);
//		//m_winSize.cx=XMIN(cx,sz.cx);
//		//m_winSize.cy=XMIN(cy,sz.cy);
//
//		 fx=FixedDivS(m_winSize.cx,cx);
//		 fy=FixedDivS(m_winSize.cy,cy);
//		 int zx=FixedMul(w,fx)*20;
//		 int zy=FixedMul(h,fy)*20;
//	
//		 if(RectIsEmpty(&zoomRect))
//		 {
//			zoomRect.xmin=0;
//			zoomRect.ymin=0;
//			zoomRect.xmax=zx;
//			zoomRect.ymax=zy;
//		 }
//		 else
//		 {
//			 w=w*20;
//			 h=h*20;
//			 int cx=(zoomRect.xmin+zoomRect.xmax)>>1;
//			 int cy=(zoomRect.ymin+zoomRect.ymax)>>1;
//			 zoomRect.xmax=cx+(zx>>1);
//			 if(zoomRect.xmax>w) zoomRect.xmax=w;
//			 zoomRect.xmin=zoomRect.xmax-zx;
//			 if(zoomRect.xmin<0) {zoomRect.xmin=0;zoomRect.xmax=zx;}
//			 zoomRect.ymax=cy+(zy>>1);
//			 if(zoomRect.ymax>h) zoomRect.ymax=h;
//			 zoomRect.ymin=zoomRect.ymax-zy;
//			 if(zoomRect.ymin<0)
//			 {
//				 zoomRect.ymin=0;zoomRect.ymax=zy;
//			 }
//		 }
//
//		 if(player->gotHeader) UpdateBuffer(XTRUE);
//
//		 SetCamera(updateNone);
//		 this->CalcLimit();
//		 if(nLockCenter==2)
//		 {
//			//MATRIX invMat;
//			MATRIX&mat =display.camera.mat;
//			//::MatrixInvert(&mat,&invMat);
//			SPOINT centerView;
//			::MatrixTransformPoint(&mat,&center,&centerView);//&invMat,&center,&centerView);
//			int cx=m_winSize.cx/2;
//			int cy=m_winSize.cy/2;
//			
//			int tx=mat.tx+cx-centerView.x;//display.camera.mat.tx+(center.x-centerView.x);
//			int ty=mat.ty+cy-centerView.y;//display.camera.mat.ty+(center.y-centerView.y);
//			if(tx>0) tx=0;
//			if(ty>0) ty=0;
//			if(tx<-m_nLimitX) tx=-m_nLimitX;
//			if(ty<-m_nLimitY) ty=-m_nLimitY;
//			display.camera.mat.tx=tx;
//			display.camera.mat.ty=ty;
//			//display.ModifyCamera();
//
//		 }
//		 else
//			 LockCenter(center,mat);
//		 //if(LockCenter(center,mat))
//		 display.ModifyCamera();
//		 if(!m_bDragOrZoom)
//		 {
//			display.Update();
//			//m_bUpdateRedraw=XTRUE;
//		 }
//	}
//	sz=m_winSize;
//
//	UpdateDragDib();
//	return XTRUE;
//}

//void XSWFPlayer::FullScreen()
//{
//	return;
// //   if(scaleMode==DisplayList::noBorder)
// //       return;
//	//scaleMode=DisplayList::fitFull+1;
//
//	int w=RectWidth(&player->frame)/20;
//	int h=RectHeight(&player->frame)/20;
//	SFIXED fx=FixedDivS(m_winSize.cx,w);
//	SFIXED fy=FixedDivS(m_winSize.cy,h);		
//	int cx=w,cy=h;
//
//	cx=FixedMul(cx,fx);
//	cy=FixedMul(cy,fy);
//
//	//m_winSize.cx=XMIN(cx,sz.cx);
//	//m_winSize.cy=XMIN(cy,sz.cy);
//
//	fx=FixedDivS(m_winSize.cx,cx);
//	fy=FixedDivS(m_winSize.cy,cy);
//	int zx=FixedMul(w,fx)*20;
//	int zy=FixedMul(h,fy)*20;
//	{
//		zoomRect.xmin=0;
//		zoomRect.ymin=0;
//		zoomRect.xmax=zx;
//		zoomRect.ymax=zy;
//	}
//	SetCamera(updateNone);
//	this->CalcLimit();
//	display.ModifyCamera();
//
//
//}




void XSWFPlayer::AS3SetFocus(_XSObject*focus)
{
	if(m_pFocus==focus)
		return;
	int nKey=0;
	bool bShift=nKeyStatus[VK_SHIFT]!=0;
	if(m_pFocus&&m_pFocus->pASObject&&m_pFocus->IsFocusAble())
	{
		void*pContext=m_pFocus->GetContext();//NULL;
		m_pAVM2->OnFocusEvent(XAS3_EVENT_mouseFocusChange,m_pFocus?m_pFocus->pASObject:XNULL,m_pFocus->pASObject,nKey,bShift,pContext);
		m_pAVM2->OnFocusEvent(XAS3_EVENT_focusOut,m_pFocus?m_pFocus->pASObject:XNULL,m_pFocus->pASObject,nKey,bShift,pContext);
	}
//#if (__CORE_VERSION__>=0x02077000)
	if(focus&&focus->pASObject&&focus->IsFocusAble())
//#else
//	if(focus&&focus->pASObject&&m_pFocus->IsFocusAble())
//#endif
	{
		void*pContext=focus->GetContext();
		m_pAVM2->OnFocusEvent(XAS3_EVENT_focusIn,m_pFocus?m_pFocus->pASObject:XNULL,focus->pASObject,nKey,bShift,pContext);
		m_pAVM2->OnFocusEvent(XAS3_EVENT_mouseFocusChange,m_pFocus?m_pFocus->pASObject:XNULL,focus->pASObject,nKey,bShift,pContext);
	}
	m_pFocus=focus;
}

bool XSWFPlayer::MouseDown(XU32 t, int x, int y )
{
	SPOINT pt;
	pt.x = x;
	pt.y = y;
	downPt=pt;
	//downling = XTRUE;
	//lockDown=XTRUE;
	// If we're doing a partial first frame, don't accept mouse downs yet
	if (!loaded)
	{
		return false;
	}
	//if ( scaleMode!=DisplayList::showAll && scaleMode!=DisplayList::fitFull&& !RectIsEmpty(&zoomRect) )
	if(this->m_nLimitX>0||this->m_nLimitY>0)
		nDownTime=this->m_nRunTime;

	if(m_nLockMode!=LK_SELECT&&!StopMove()
//#ifdef _LOWSPEED
//		&&!m_bNoScript
//#endif
		)
	{
		nKeyStatus[1]=XTRUE;
	//	XLock lock(&m_dataLock);
	//	if(!lock.Lock(XWAIT_TIME)) return;

		gmouseState=1;

		//SPOINT local=pt;
		SObject*pOld=m_pFocus;//display.button?display.button:focus;
		SObject*target=XNULL;
		SObject*pObj=DoButton(target,&pt,XTRUE,t);
//#if (__CORE_VERSION__>=0x02074000)
//#if (__CORE_VERSION__>=0x02076000)
		m_pDownTarget=pObj;
//#else
//		m_pDownTarget=target;
//#endif
		m_pUpTarget=NULL;
//#endif
		if(m_pAVM2)
		{

			MOUSEEVENTDATA data;
			if(pObj&&pObj->pASObject)
			{
				//if(0)//pObj->IsCreateBefore(t))
				{
					void*pContext=pObj->GetContext();//NULL;
					//ScriptPlayer*player=pObj->pObj->GetPlayer();
					//if(player)
					//	pContext=player->m_pContext;
					//if(pObj->character)
					//	pContext=pObj->character->player->m_pContext;
					AS3InitMouseData(data,target,true,target->pASObject,XNULL,XNULL);
					LockOpt(XTRUE);
					m_pAVM2->OnMouseEvent(XAS3_EVENT_mouseDown,data,pObj->pASObject,pContext);
					UnLockOpt();
				}
				//void*pEvent=AS3CreateMouseEvent(avmshell::XAS3_EVENT_mouseDown,pObj,true,pObj->pASObject);
				//m_pAVM2->DispatchEvent(pEvent,pObj->pASObject);
			}
			else
			{	void*pASObject=display.root.pASObject;
				if(pASObject)
				{
					MOUSEEVENTDATA data;
					AS3InitMouseData(data,target,true,display.root.pASObject,XNULL,XNULL);
					LockOpt(XTRUE);
					m_pAVM2->OnMouseEvent(XAS3_EVENT_mouseDown,data,pASObject,player->m_pLoaderInfo->m_codeContext);//m_pContext);
					UnLockOpt();
				}
			}
			//pObj=display.button?display.button:focus;
			if(pOld!=pObj)
			{
				m_pFocus=pObj;
				int nKey=0;
				bool bShift=nKeyStatus[VK_SHIFT]!=0;
				if(pOld&&pOld->pASObject&&pOld->IsFocusAble())
				{
					void*pContext=pOld->GetContext();//NULL;
					//ScriptPlayer*player=pOld->GetPlayer();
					//if(player)
					//	pContext=player->m_pContext;
					//if(pOld->character)
					//	pContext=pOld->character->player->m_pContext;
					m_pAVM2->OnFocusEvent(XAS3_EVENT_mouseFocusChange,pObj?pObj->pASObject:XNULL,pOld->pASObject,nKey,bShift,pContext);
					//if(!pOld->IsParentOf(pObj))
					m_pAVM2->OnFocusEvent(XAS3_EVENT_focusOut,pObj?pObj->pASObject:XNULL,pOld->pASObject,nKey,bShift,pContext);
					//void*pEvent=AS3CreateFocusEvent(avmshell::XAS3_EVENT_mouseFocusChange,
					//	0,pObj?pObj->pASObject:XNULL,pOld->pASObject);
					//m_pAVM2->DispatchEvent(pEvent,pOld->pASObject);
					//m_pAVM2->ResetEvent(pEvent,avmshell::XAS3_EVENT_focusOut,pOld->pASObject);
					//m_pAVM2->DispatchEvent(pEvent,pOld->pASObject);
				}
				if(pObj&&pObj->pASObject&&pObj->IsFocusAble())
				{
					void*pContext=pObj->GetContext();
					//ScriptPlayer*player=pObj->GetPlayer();
					//if(player)
					//	pContext=player->m_pContext;
					//if(pObj->character)
					//	pContext=pObj->character->player->m_pContext;
					//if(!pObj->IsParentOf(pOld))
					m_pAVM2->OnFocusEvent(XAS3_EVENT_focusIn,pOld?pOld->pASObject:XNULL,pObj->pASObject,nKey,bShift,pContext);
					m_pAVM2->OnFocusEvent(XAS3_EVENT_mouseFocusChange,pOld?pOld->pASObject:XNULL,pObj->pASObject,nKey,bShift,pContext);
					//m_pAVM2->OnEvent(XAS3_EVENT_change,true,pObj->pASObject,pContext);
					//m_pAVM2->OnFocusEvent(
					//void*pEvent=AS3CreateFocusEvent(avmshell::XAS3_EVENT_focusIn,0,pOld?pOld->pASObject:XNULL,pObj->pASObject);
					//m_pAVM2->DispatchEvent(pEvent,pObj->pASObject);
					//m_pAVM2->ResetEvent(pEvent,avmshell::XAS3_EVENT_focusOut);
					//m_pAVM2->DispatchEvent(pEvent,pOld->pASObject);
				}
			}

			//MouseFocus(pOld,display.button);
		}

		/*SObject*fc=focus?focus:display.buttonThread;
		XBOOL bAction=XFALSE;
		if(fc)
		{
			if(fc->OnEvent(this,XOBJEVENT_MOUSEDOWN,XNULL))
				bAction=XTRUE;
				//DoActions();
		}*/
		XBOOL bOK=m_pAVM2?XFALSE:
				display.root.PostEvent(t,this,XOBJEVENT_MOUSEDOWN,0);

		if(HandleListener(XOBJEVENT_MOUSEDOWN))
			bOK=XTRUE;
		if(bOK)
		//	bAction=XTRUE;
		//if(bAction)
			DoActions();

		if ( display.button){//&&display.buttonThread->character->type==buttonChar ) {
	//		lock.UnLock();
			return false;
		}
	#ifdef EDITTEXT
		if (IsSelecting()) {
	//		lock.UnLock();
			nDownTime=0;
			return false;
		}
	#endif
		}

	// Just incase, release the capture
	XReleaseCapture();
	return false;
}

#ifdef DRAGBUTTONS
void XSWFPlayer::UpdateDropTarget()
{
	//int saveVisible = dragObject->visible;
	XU8 saveFlags=dragObject->flags;

	//dragObject->visible = XFALSE;
	dragObject->flags=0;
	_XSObject*hitObj=NULL;
	SObject *dropObject = display.HitObject(&mousePt,&hitObj,0);
		//dragObject->display->HitTest(&mousePt);
	dragObject->flags=saveFlags;//visible = saveVisible;

	dragObject->dropTarget=NULL;//.Release();
	//FreeStr(dragObject->dropTarget);

	if (dropObject) {
		//dragObject->dropTarget =
		//XXVar var;
		//GetTargetPath(dropObject->thread->rootObject,dragObject->dropTarget);
		dragObject->dropTarget=dropObject;
	} else {
		dragObject->dropTarget = NULL;
	}
}
#endif


void XSWFPlayer::MouseClick(XU32 t,int x,int y)
{
	if(m_bCancelNext)
	{
		m_bCancelNext=false;
		return;
	}
	downling = XFALSE;
	if(m_nLockMode==LK_SELECT&&StopMove()) return;
	if(m_pAVM2)
	{
		SPOINT pt={x,y};
		//SObject* hitClip=XNULL;
		SObject*ret=XNULL;
		_XSObject*hitObj=NULL;
		SObject* target = NULL;
		if(m_pUpRemovedObject)
			target=m_pUpRemovedObject;
		else
			target=display.HitObject(&pt,&hitObj,t);
        
		if(m_pDownTarget!=m_pUpTarget)
		{
			m_pDownTarget=NULL;
			m_pUpTarget=NULL;
			return;
		}
		m_pDownTarget=NULL;
		m_pUpTarget=NULL;


		if(m_pDomFilter&&m_pDomFilter->nAutoCap)
		{
			int nCap=(m_pDomFilter->nAutoCap*128)/100;
			int scale=display.camera.mat.a*20;
			//if(display.antialias_b)
			//	scale>>=display.antialias_b;
			nCap=nCap*fixed_1/scale;
			if(nCap&&(!target||!target->HasClickEvent()))
			{
				AS3ButtonInfomation*pInfo=NULL;
				pt.x=x;
				pt.y=y;
				//if(display.antialias_b)
				//{
				//	pt.x<<=display.antialias_b;
				//	pt.y<<=display.antialias_b;
				//}
				//STransform x,SPOINT*pt,AS3ButtonInfomation**pInfo,int nCap,SRECT&devRect,XBOOL bEnable)
				
				SRECT rect;
				RectSetEmpty(&rect);
				m_pBIInfo=&pInfo;
				display.root.HitBI(display.camera,&pt,nCap,rect,XTRUE);
				
				if(pInfo)
				{
					AS3ButtonInfomation*pImp=pInfo;
					AS3ButtonInfomation*p=pInfo->pNext;
					while(p)
					{
						if(pImp->nImportant<p->nImportant)
							pImp=p;
						else if(pImp->nImportant==p->nImportant)
						{
							if(pImp->button->IsParentOf(p->button))
								pImp=p;
						}
						p=p->pNext;
					}
					if(target!=pImp->button)
					{
						pt.x=(pImp->buttonBounds.xmin+pImp->buttonBounds.xmax)/2;
						pt.y=(pImp->buttonBounds.ymin+pImp->buttonBounds.ymax)/2;
						SObject*autoHit=NULL;
						SObject* autoObj = display.root.AS3HitObject(display.camera,&pt,&autoHit,t);
							//display.HitObject(&pt,&autoHit,t);
						if(autoObj&&(pImp->button==autoObj||pImp->button->IsParentOf(autoObj)))
						{
							//target=pImp->button;
							hitObj=autoHit;
							target=autoObj;
							pt.x=pImp->nDeltaX;
							pt.y=pImp->nDeltaY;
							//if(display.antialias_b)
							//{
							//	pt.x>>=display.antialias_b;
							//	pt.y>>=display.antialias_b;
							//	AddDelta(pt.x,pt.y);
							//}
						}

					}
					display.FreeInfo(pInfo);
				}
				
			}
		}

		if(target)
		{
			//Adjust camera for auto center
			//SObject* lo=target;
			_XSObject*tmp=target;
			target=/*hitObj?hitObj->AS3GetTargetObject(true):*/target->AS3GetTargetObject(true);
			if(target)
			{
				ret=target->AS3GetEventObject(true);
//#if (__CORE_VERSION__>=0x02076000)
			if(ret&&(/*ret->IsButtonMode()||*/!ret->IsMouseChildren()))
				target=ret;
			else if(hitObj&&hitObj!=target&&hitObj->pLink==tmp)
					target=hitObj->AS3GetTargetObject(true);
//#else
//				if(hitObj&&hitObj!=target&&hitObj->pLink==tmp)
//					target=hitObj->AS3GetTargetObject(true);
//#endif
			}
			else
				ret=XNULL;


			//ret=target->AS3GetEventObject(true);
			//target=target->AS3GetTargetObject(true);
		}
			//display.HitButton(hitClip,&pt);
		//SObject* ret=hitBtn?hitBtn:hitClip;
//#if (__CORE_VERSION__>=0x02070100)
		if(ret&&ret->pASObject&&ret->IsCreateBefore(t))
		{
			//if(ret->creat			
			{
				
//#else
//		if(ret&&ret->pASObject)
//		{
//			if(ret->IsCreateBefore(t))
//			{
//				if(ret->IsEditText())
//				{
//					ret->editText->DoClick(x,y);
//					//Trace(XString16("Click"));
//				}
//#endif			
				void*pContext=ret->GetContext();//NULL;
				//ScriptPlayer*player=ret->GetContext();//ret->GetPlayer();
				//if(player)
				//		pContext=player->m_pContext;
				//if(ret->character)
				//	pContext=ret->character->player->m_pContext;

				MOUSEEVENTDATA data;
				AS3InitMouseData(data,target,false,target->pASObject,XNULL,XNULL);
				LockOpt(XFALSE);
				m_pAVM2->OnMouseEvent(XAS3_EVENT_click,data,ret->pASObject,pContext);

				UnLockOpt();
			}
			//void*pEvent=AS3CreateMouseEvent(avmshell::XAS3_EVENT_click,ret,false,ret->pASObject);
			//m_pAVM2->DispatchEvent(pEvent,ret->pASObject);
		}
		else if(display.root.pASObject)
		{
			void*pContext=player->m_pLoaderInfo->m_codeContext;//m_pContext;
			MOUSEEVENTDATA data;
			AS3InitMouseData(data,target,false,display.root.pASObject,XNULL,XNULL);
			LockOpt(XFALSE);
			m_pAVM2->OnMouseEvent(XAS3_EVENT_click,data,display.root.pASObject,pContext);
			UnLockOpt();
		}

	}
}



void XSWFPlayer::MouseDoubleClick(XU32 t,int x,int y)
{
	downling = XFALSE;
	if(m_nLockMode==LK_SELECT&&StopMove()) return;
	if(m_pAVM2)
	{
		SPOINT pt={x,y};
		//SObject* hitClip=XNULL;
		SObject*ret=XNULL;
		_XSObject*hitObj=NULL;
		SObject*target = display.HitObject(&pt,&hitObj,t);
		if(target)
		{
			_XSObject*tmp=target;
			target=target->AS3GetTargetObject(true);
			if(target)
			{
				ret=target->AS3GetEventObject(true);
//#if (__CORE_VERSION__>=0x02076000)
			if(ret&&(/*ret->IsButtonMode()||*/!ret->IsMouseChildren()))
				target=ret;
			else if(hitObj&&hitObj!=target&&hitObj->pLink==tmp)
					target=hitObj->AS3GetTargetObject(true);
//#else
//				if(hitObj&&hitObj!=target&&hitObj->pLink==tmp)
//					target=hitObj->AS3GetTargetObject(true);
//#endif
			}
			else
				ret=XNULL;
			//ret=target->AS3GetEventObject(true);
			//target=target->AS3GetTargetObject(true);
		}
			//display.HitButton(hitClip,&pt);
		//SObject* ret=hitBtn?hitBtn:hitClip;

		//if(ret) ret->DblClick(&pt);
		if(ret&&ret->pASObject)

		{
			//if(ret->IsCreateBefore(t))
			{
				void*pContext=ret->GetContext();//NULL;
				//ScriptPlayer*player=ret->GetContext();//ret->GetPlayer();

				//if(player)
				//		pContext=player->m_pContext;
				//if(ret->character)
				//	pContext=ret->character->player->m_pContext;
				MOUSEEVENTDATA data;
				AS3InitMouseData(data,target,false,target->pASObject,XNULL,XNULL);
				m_pAVM2->OnMouseEvent(XAS3_EVENT_doubleClick,data,ret->pASObject,pContext);
			}
			//void*pEvent=AS3CreateMouseEvent(avmshell::XAS3_EVENT_doubleClick,ret,false,ret->pASObject);
			//m_pAVM2->DispatchEvent(pEvent,ret->pASObject);
		}
		else if(display.root.pASObject)
		{
			void*pContext=player->m_pLoaderInfo->m_codeContext;//m_pContext;
			MOUSEEVENTDATA data;
			AS3InitMouseData(data,target,false,display.root.pASObject,XNULL,XNULL);

			m_pAVM2->OnMouseEvent(XAS3_EVENT_doubleClick,data,display.root.pASObject,pContext);
		}
	}
}

//#if (__CORE_VERSION__>=0x02070100)
void XSWFPlayer::MouseUp(XU32 t, int x, int y,XBOOL bWithClick )
//#else
//void XSWFPlayer::MouseUp(XU32 t, int x, int y )
//#endif
{
	SPOINT pt;
	pt.x = x;
	pt.y = y;
	//lockDown=XFALSE;
	nDownTime=0;
	// If we're doing a partial first frame, don't accept mouse downs yet
	if (!loaded) {
		return;
	}

	nKeyStatus[1]=XFALSE;
//	XLock lock(&m_dataLock);
//	if(!lock.Lock(XWAIT_TIME)) return;

	gmouseState=0;

	//if(StopMove()) return;

#ifdef EDITTEXT
	if (IsSelecting()) {
		focus->EditTextMouse(&pt, XFALSE);
		focus->Modify();
#ifdef _FULLEDIT
		if(pWindow&&focus->editText->IsEditAble())
		{
			focus->editText->SelectAll();
			pWindow->PostMsg(XWM_STARTEDIT,focus->editText->GetEditStyle(),
								 (XU32)focus->editText->m_buffer,XMS_TOPMOST);
		}
#endif
//		lock.UnLock();
		if(focus->editText->m_selectionEnd!=focus->editText->m_selectionStart)
			return;
		//return;
	}
#endif
	if(isMoveScroll)
	{
		isMoveScroll=XFALSE;
		StopMove();
		m_bCancelNext=false;
		scrolling = XFALSE;
		downling = XFALSE;
		//if(m_bNoScript)
		{
			this->OnMouseUp(x,y,0);
			return;
		}
	}

	if ( scrolling || downling ) {
		StopMove();
		m_bCancelNext=true;
		scrolling = XFALSE;
		downling = XFALSE;
	} else if(m_nLockMode!=LK_SELECT&&m_nMode==MV_NONE
//#ifdef _LOWSPEED
//		&&!m_bNoScript
//#endif
		){
		//SObject*pOld=
		SObject*target=XNULL;
		SObject*pObj=DoButton(target,&pt,2,t);
		m_pUpObject=pObj;
//#if (__CORE_VERSION__>=0x02074000)
//#if (__CORE_VERSION__>=0x02076000)
		m_pUpTarget=pObj;
//#else
//		m_pUpTarget=target;
//#endif
//#endif
		if(m_pAVM2)
		{
//#if (__CORE_VERSION__>=0x02070100)
			if(pObj&&pObj->pASObject&&pObj->IsCreateBefore(t))
			{
				//if(pObj->IsCreateBefore(t))				
					if(bWithClick&&pObj->IsEditText())
					{
						pObj->editText->DoClick(x,y);
						//Trace(XString16("Click"));
					}
//#else
//			if(pObj&&pObj->pASObject)
//			{
//				if(pObj->IsCreateBefore(t))
//#endif
				{
					void*pContext=pObj->GetContext();//NULL;
					//ScriptPlayer*player=pObj->GetPlayer();
					//if(player)
					//	pContext=player->m_pContext;
					//if(pObj->character)
					//	pContext=pObj->character->player->m_pContext;
					MOUSEEVENTDATA data;
					AS3InitMouseData(data,target,false,target->pASObject,XNULL,XNULL);
					LockOpt(XFALSE);
					m_pAVM2->OnMouseEvent(XAS3_EVENT_mouseUp,data,pObj->pASObject,pContext);
					UnLockOpt();
				}
				//void*pEvent=AS3CreateMouseEvent(avmshell::XAS3_EVENT_mouseUp,pObj,true,pObj->pASObject);
				//m_pAVM2->DispatchEvent(pEvent,pObj->pASObject);
			}
			else
			{	void*pASObject=display.root.pASObject;
				if(pASObject)
				{
					MOUSEEVENTDATA data;
					AS3InitMouseData(data,target,false,display.root.pASObject,XNULL,XNULL);
					LockOpt(XFALSE);
					m_pAVM2->OnMouseEvent(XAS3_EVENT_mouseUp,data,pASObject,player->m_pLoaderInfo->m_codeContext);//m_pContext);
					UnLockOpt();
				}
			}
			/*pObj=display.button?display.button:focus;
			if(pOld!=pObj)
			{
				if(pOld&&pOld->pASObject)
				{
					void*pEvent=AS3CreateFocusEvent(avmshell::XAS3_EVENT_mouseFocusChange,
						0,pObj?pObj->pASObject:XNULL,pOld->pASObject);
					m_pAVM2->DispatchEvent(pEvent,pOld->pASObject);
					m_pAVM2->ResetEvent(pEvent,avmshell::XAS3_EVENT_focusOut);
					m_pAVM2->DispatchEvent(pEvent,pOld->pASObject);
				}
				if(pObj&&pObj->pASObject)
				{
					void*pEvent=AS3CreateFocusEvent(avmshell::XAS3_EVENT_focusIn,0,pOld?pOld->pASObject:XNULL,pObj->pASObject);
					m_pAVM2->DispatchEvent(pEvent,pObj->pASObject);
					//m_pAVM2->ResetEvent(pEvent,avmshell::XAS3_EVENT_focusOut);
					//m_pAVM2->DispatchEvent(pEvent,pOld->pASObject);
				}
			}*/
			//MouseFocus(pOld,display.button);
		}
	}

	XBOOL bOK=m_pAVM2?XFALSE:
			display.root.PostEvent(t,this,XOBJEVENT_MOUSEUP,0);

	if(HandleListener(XOBJEVENT_MOUSEUP))
		bOK=XTRUE;
	if(bOK)
		DoActions();

	//XBOOL bAction=XFALSE;
	/*SObject*fc=focus?focus:display.buttonThread;
	if(fc)
	{
		if(fc->OnEvent(this,XOBJEVENT_MOUSEUP,XNULL))
			bAction=XTRUE;
			//DoActions();
	}*/
	//if(HandleListener(XOBJEVENT_MOUSEUP))
	//	bAction=XTRUE;
	//if(bAction)
	//	DoActions();

	XReleaseCapture();
//	lock.UnLock();
}

void XSWFPlayer::CancelCapture(BOOL outsideWindow)
{
	// We lost the capture
//	XLock lock(&m_dataLock);
//	if(!lock.Lock(XWAIT_MONMENT)) return;
	scrolling = XFALSE;
	if ( outsideWindow ) {
		// We lost the mouse entirely
		mouseState = -1;
		XBOOL bSet=display.SetButton(0, XFALSE);
		//if(display.SetButton(0,XFALSE,1))
		//	bSet=XTRUE;
		if (  bSet ) {
			DoActions();
			//UpdateScreen();
		}
	} else {
		// We just lost the mousedown capture
		mouseState = 0;
	}
//	lock.UnLock();
	//UpdateCursor();
}

ScriptPlayer* FindLayer(ScriptPlayer* layer, int layerDepth)
{
	while ( layer ) {
		if ( layer->layerDepth == layerDepth )
			return layer;
		layer = layer->nextLayer;
	}
	return 0;
}

void XSWFPlayer::ClearLayer(int layerDepth)
{
	if ( layerDepth == 0 ) {
		// For layer 0, reset everything
		ClearScript();
	} else {
		// For all other layers, delete the player
		ScriptPlayer** link = &player->nextLayer;
		while ( XTRUE ) {
			ScriptPlayer* layer = *link;
			if ( !layer ) break;
			if ( layer->layerDepth == layerDepth ) {
				*link = layer->nextLayer;
				delete layer;
				return;
			}
			link = &layer->nextLayer;
		}
	}
}

ScriptPlayer* XSWFPlayer::OpenVariableLoader(int layer,XXVar&var)
{
	ScriptPlayer *player = new ScriptPlayer(XFALSE);
	player->display=&display;
	ScriptPlayer *root=FindLayer(player,layer);
	player->m_pGlobal=root?root->m_pGlobal:player->m_pGlobal;
	player->splayer = this;
	player->variableLoader = XTRUE;
	player->variableLayer = layer;
	player->variableSprite = var;//CreateStr((char *)spriteName);
	return player;
}

/////////////////////////////////////////
// create a place holder for the
// load external sprite call
//
ScriptPlayer* XSWFPlayer::OpenSprite(ScriptThread *targetThread)
{
    ScriptPlayer    *sprite = 0;
    SCharacter*     ch = 0;
    PlaceInfo       info;
    BOOL            success = XFALSE;
    SObject         *rootObject = 0;
    SObject         *srcObject = 0;

	if (!targetThread)
        goto exit_gracefully;

    srcObject = targetThread->rootObject;

    sprite = new ScriptPlayer(XTRUE);
	//sprite->InitRoot();


	if (!sprite)
        goto exit_gracefully;
	sprite->splayer = this;
	sprite->m_pGlobal->Init(sprite->NeedLowCase());
    // don't use regular character alloc
    // on this. Use new intead. We treat this
    // ch differently since it's external
	ch = CreateCharacter();//new SCharacter;
	if ( ch )
    {
        // create a unique character becuase
        // the player will be different than the
        // main movie's player
		ch->record.Init();
		ch->next = 0;;
		ch->className=XNULL;
		ch->gridRect=XNULL;
		ch->classEntry=XNULL;
		ch->splayer = sprite;
		ch->tag = ctagExternalThreadRoot;
		ch->tagCode = 0;
		ch->type = spriteExternalChar;
		ch->data = 0;
		ch->rbounds = srcObject->character->rbounds;
	}
    else
        goto exit_gracefully;

	info.flags = splaceCharacter | splaceMatrix;
	info.depth = srcObject->depth;
    info.mat = srcObject->xform.mat;
    info.puppet = srcObject->puppet;
	info.character = ch;

    if (srcObject->xform.cxform.HasTransform())
    {
        info.flags |= splaceColorTransform;
        info.cxform = srcObject->xform.cxform;
    }
    else
		info.cxform.Clear();

    // we inherit the ratio of the object we are trying
    // replace
    info.ratio = srcObject->ratio;

    info.name = srcObject->name;//CreateStr(srcObject->name);
    if (!info.name.IsNull())
        info.flags |= splaceName;

    if (srcObject->clipDepth > 0)
    {
        info.flags |= splaceDefineClip;
		info.clipDepth = srcObject->clipDepth;
    }
    else
		info.clipDepth = 0;

    rootObject = srcObject->parent;
    rootObject->thread->RemoveSprite(targetThread, XFALSE);
	sprite->rootObject = display.PlaceObject2(rootObject, &info, XNULL,true);//XNULL,XTRUE);
	sprite->rootObject->thread = sprite;

    success = XTRUE;

exit_gracefully:

    if (!success)
    {
        if (ch) {
			FreeCharacter(ch);
	        //sprite->characterAlloc.Free(ch);
		}

        delete sprite;
        sprite = 0;
    }

	return sprite;
}

ScriptPlayer* XSWFPlayer::OpenLayer(int layerDepth, BOOL loadVars)
{
	if ( layerDepth == 0 && player->script && !loadVars)
		ClearScript();			// For layer 0, reset everything

	// See if the layer already exists
	ScriptPlayer* layer = FindLayer(player, layerDepth);
	if ( !layer ) {
		// Create a new layer
		FLASHASSERT(layerDepth != 0);
		layer = new ScriptPlayer(XTRUE);
		if ( !layer )
			return 0;
		layer->nextLayer = player->nextLayer;
		player->nextLayer = layer;
		layer->SetDisplay(&display,true);
		layer->splayer = this;
		layer->layerDepth = layerDepth;
	} else {
		// Empty an existing layer
		if ( (layer->script || layerDepth != 0) && !loadVars )
			layer->ClearScript();
	}

    // set the version variable
    SetVersionVariable( layer );

	return layer;
}

//void XSWFPlayer::Zoom(SPOINT size, SPOINT center)
//{
//	//if ( scaleMode == DisplayList::noScale )
//	//	scaleMode = DisplayList::showAll;
//
//	SRECT viewRect;
//	ClientRect(viewRect);
//
//	// Never more than 1:1 enlargement
//	if ( size.x < RectWidth(&viewRect) )
//		size.x = RectWidth(&viewRect);
//
//	if ( size.y < RectHeight(&viewRect) )
//		size.y = RectHeight(&viewRect);
//
//	zoomRect.xmin = center.x - size.x/2;
//	zoomRect.ymin = center.y - size.y/2;
//	zoomRect.xmax = center.x + size.x/2;
//	zoomRect.ymax = center.y + size.y/2;
//
//	SCOORD adjust = zoomRect.xmin - player->frame.xmin;
//	if ( adjust < 0 ) {
//		zoomRect.xmin -= adjust;
//		zoomRect.xmax -= adjust;
//	}
//
//	adjust = zoomRect.xmax - player->frame.xmax;
//	if ( adjust > 0 ) {
//		zoomRect.xmin -= adjust;
//		zoomRect.xmax -= adjust;
//	}
//
//	adjust = zoomRect.ymin - player->frame.ymin;
//	if ( adjust < 0 ) {
//		zoomRect.ymin -= adjust;
//		zoomRect.ymax -= adjust;
//	}
//
//	adjust = zoomRect.ymax - player->frame.ymax;
//	if ( adjust > 0 ) {
//		zoomRect.ymin -= adjust;
//		zoomRect.ymax -= adjust;
//	}
//
//	RectIntersect(&player->frame, &zoomRect, &zoomRect);
//	if ( RectIsEmpty(&zoomRect) ||
//		(size.x >= RectWidth(&player->frame) &&
//		 size.y >= RectHeight(&player->frame)) ) {
//		// We are zoomed out past the maximum
//		Home();
//	} else {
//		//RectIntersect(&zoomRect, &player->frame, &zoomRect);
//		SetCamera(updateLazy);
//	}
//}
//
//void XSWFPlayer::ZoomF(SFIXED factor, SPOINT* pt)
//{
//	MATRIX m, invMat;
//	/*if ( display.antialias_b ) {
//		switch(display.antiShift)
//			{
//			case 2:
//				MatrixScale(fixed_1/4, fixed_1/4, &m);
//				break;
//			case 1:
//				MatrixScale(fixed_1/2, fixed_1/2, &m);
//				break;
//			case 3:
//				MatrixScale(fixed_1/8, fixed_1/8, &m);
//				break;
//			}
//		//MatrixScale(fixed_1/4, fixed_1/4, &m);
//		MatrixConcat(&display.camera.mat, &m ,&m);
//	} else */
//	{
//		m = display.camera.mat;
//	}
//	MatrixInvert(&m, &invMat);
//
//	SRECT viewRect;
//	ClientRect(viewRect);
//	MatrixTransformRect(&invMat, &viewRect, &zoomRect);
//
//	SPOINT size, center;
//	size.x = RectWidth(&zoomRect);
//	size.y = RectHeight(&zoomRect);
//
//	if ( pt ) {
//		MatrixTransformPoint(&invMat, pt, &center);
//	} else {
//		center.x = (zoomRect.xmin + zoomRect.xmax)/2;
//		center.y = (zoomRect.ymin + zoomRect.ymax)/2;
//	}
//
//	size.x = FixedMul(factor, size.x);
//	size.y = FixedMul(factor, size.y);
//
//	Zoom(size, center);
//}
//
//void XSWFPlayer::Zoom(SRECT* r)
//{
//	SPOINT size, center;
//	size.x = RectWidth(r);
//	size.y = RectHeight(r);
//
//	center.x = (r->xmin + r->xmax)/2;
//	center.y = (r->ymin + r->ymax)/2;
//
//	Zoom(size, center);
//}
//
//
//
//
//void XSWFPlayer::Zoom100()
//{
//	//if ( scaleMode & DisplayList::zoom100Scroll ) 
//	//{
//	//	SRECT vr;
//	//	ClientRect(vr);
//	//	zoomRect.xmin = zoomRect.ymin = 0;
//	//	zoomRect.xmax = 20*RectWidth(&vr);
//	//	zoomRect.ymax = 20*RectHeight(&vr);
//	//} else {
//	//	RectSetEmpty(&zoomRect);
//	//	scaleMode = DisplayList::noScale;
//	//}
//	SetCamera(updateLazy);
//}
//
//void XSWFPlayer::ZoomScale(SFIXED scale)
//{
//	/*
//	MATRIX m, invMat;
//	if ( display.antialias ) {
//		MatrixScale(fixed_1/4, fixed_1/4, &m);
//		MatrixConcat(&display.camera.mat, &m ,&m);
//	} else {
//		m = display.camera.mat;
//	}
//	MatrixInvert(&m, &invMat);
//
//	SRECT viewRect;
//	ClientRect(&viewRect);
//	MatrixTransformRect(&invMat, &viewRect, &zoomRect);
//	*/
//	/*
//	SRECT viewRect;
//	ClientRect(&viewRect);
//	SPOINT size, center;
//	size.x = 20*RectWidth(&viewRect);
//	size.y = 20*RectHeight(&viewRect);
//
//	center.x = (zoomRect.xmin + zoomRect.xmax)/2;
//	center.y = (zoomRect.ymin + zoomRect.ymax)/2;
//
//	size.x = FixedDiv(size.x, scale);
//	size.y = FixedDiv(size.y, scale);
//
//	Zoom(size, center);
//	*/
//}
//
//void XSWFPlayer::Home()
//{
//	//if ( scaleMode == DisplayList::noScale )
//	//	scaleMode = DisplayList::showAll;
//
//	RectSetEmpty(&zoomRect);
//	SetCamera(updateLazy);
//}

void XSWFPlayer::SetCamera(int update)
// Set the camera mat so that the picture frame fills the window
{
	//GCENTER
//#ifdef _LOWSPEED
//	if(m_bDragOrZoom)
//	{
//		//m_bUpdateDrag=true;
//		//return;
//		update=updateNone;
//	}
//#endif
	//SRECT vr;
	//ClientRect(vr);
	display.faster = true;//player->numFrames>1&&nQuality<XConfig::HIGH;
		//player->numFrames > 1 && !bestDither&&nQuality==XConfig::HIGH;
	//if(scaleMode == DisplayList::noBorder)
	//{
	//	vr.xmin=0;
	//	vr.ymin=0;
	//	vr.xmax=player->frame.xmax/20;
	//	vr.ymax=player->frame.ymax/20;
	//	display.SetCamera(&player->frame, &vr, scaleMode);
	//	display.ModifyCamera();
	//}
	//else 
	//display.SetCamera(RectIsEmpty(&zoomRect) ? &player->frame : &zoomRect );
	display.SetCamera( &player->frame );
	//switch ( update ) {
	//	default:
	//	case updateLazy:
	//		CheckUpdate();
	//		break;
	//	//case updateNow:
	//	//	UpdateScreen();
	//	//	break;
	//}
	//if(player->gotHeader)
	//{
	//	UpdateBuffer(update!=updateNone);
	//}
}

void XSWFPlayer::FreeLayers()
{
	// Note layer[0] is special and should not be deleted
	while (XTRUE) {
		ScriptPlayer* layer = player->nextLayer;
		if ( !layer )
			break;
		player->nextLayer = layer->nextLayer;
		delete layer;
	}

	player->ClearScript();
}




void XSWFPlayer::ClearScript()
// Clear all of the layers in the movie
{
	//Suspend();
	m_hStop.ResetEvent();

//	ClearHandlers();

	FreeLayers();

	//RectSetEmpty(&zoomRect);
	loaded = XFALSE;

#ifdef DRAGBUTTONS
	dragObject = 0;
#endif
#ifdef EDITTEXT
	focus = 0;
	saveFocus.Clear();
#endif
	cursorShow=XTRUE;
	menuShow=XTRUE;
	// This is a spagetti situation:
	// SetCamera will call
	//		NativePlayerWnd::ClientRect(&vr)
	// which is already destroyed. So we set a
	// flag to suppress this call.
	if ( !destructing )
	{
		SetCamera(updateLazy);
	}

	ReleaseCallers();
	FreeCalls();

	XXObject::FreeObjs();
/*	for(int i=0;i<XKEYS_NUMS;i++)
	{
		_XKeyword*pKey=m_keys[i];
		while(pKey)
		{
			_XKeyword*pNext=pKey->pNext;
			delete pKey;
			pKey=pNext;
		}
		m_keys[i]=XNULL;
	}	*/
//#ifdef SOUND
//	theSoundMix.Destruct();
//#endif
}

void XSWFPlayer::GotoFrame(int f)
{
	//FLASHOUTPUT("Goto %i\n", f);
	GCENTER
	//player->Seek(f,true,ScriptPlayer::SK_KEEP);
	player->CMDGotoAndStop(f);
	player->doCommand(true);
	if ( actionDepth == 0 ) {
		DoActions();
		CheckUpdate();
	}
}

void XSWFPlayer::FreeBuffer()
{
	//XLock lock(&display.raster.dibLock);   // modify by hgl
	/*if(display.raster.dibLock.Lock(XSHORTTIME))
	{
		bits.FreeBits();
		display.raster.dibLock.UnLock();
	}*/
	display.Invalidate();

}


SObject* XSWFPlayer::FindTarget(SObject* base, char* path,SObject*with)
// Path notation
//
// "/fred"			- the object named fred that is a top level object of the movie
// ".."				- the thread's parent
// "fred/bob"		- the object named fred that is a top level object of the movie
// "fred"			- the object named fred that is a child of this thread
// "../../fred"		- the object named fred that is a child of this threads parent's parent
// "" - self		- blank refers to the current thread
// "_flash1/bob"	- the object named bob at the top level of layer1
{
	//return XNULL;
	if(path==XNULL) return base;
	FLASHASSERT(base);

	BOOL top = XTRUE,bDot=XTRUE;

	if ( path[0] == '/' ) {
		// Pop up to the root
		bDot=XFALSE;
		if(with)
		{
			base=with;
			with=XNULL;
		}
		while ( base->parent && base->parent != &display.root ) {
			base = base->parent;
		}
		path++;
		top = XFALSE;
	}

	// Parse the path elements
	while ( XTRUE ) {
		char name[128];

		// We have parsed the path, return the object
		if ( *path == 0 )
			return base;

		{// Pull off a name
			int i = 0;
			while ( *path && i < 127 ) {
				if ( *path == '/' ) {
					path++;
					bDot=XFALSE;
					break;
				}
				else if(bDot&&*path=='.')//&&*(path+1)!='.')
				{
					if(*(path+1)=='.')
						bDot=XFALSE;
					else
					{
						path++;
						break;
					}
				}
				name[i++] = *path++;
			}
			name[i] = 0;
		}

		// See if they reference a layer directly (only check at the top element)
		if ( top ) {
			top = XFALSE;
			if(XString8::Compare(name,"_root",0,XTRUE)==0)
			{
				if(with)
				{
					base=with;
					with=XNULL;
				}
				while ( base->parent && base->parent != &display.root ) {
					base = base->parent;
				}
				//path++;
				//top = XFALSE;
				continue;
			}
			else
			{
				int layerNum = LayerNum(name);
				if ( layerNum >= 0 ) {
					ScriptPlayer* l = FindLayer(player, layerNum);
					if ( l && l->rootObject ) {
						base = l->rootObject;
						continue;
					} else {
						return 0;
					}
				}
			}
		}

		if ( StrEqual(name, "..") ) {
			// Visit the parent
			if(with)
			{
				base=with;
				with=XNULL;
			}
			base = base->parent;
			if ( !base )
				return 0;
		} else {
			// Find an object that matches the name
			SObject*pObj=XNULL;
			if(with)
			{
				pObj=with->GetChildOf(name,XTRUE);
				with=XNULL;
			}
			if(!pObj)
				pObj=base->GetChildOf(name,XTRUE);
			if(pObj)
			{
				base=pObj;
				goto found;
			}

			/*for ( SObject* obj = base->bottomChild; obj; obj = obj->above ) {
				// Does this object match?
				if ((obj->character->type == spriteChar || obj->character->type == spriteExternalChar)
                    && obj->name && StrEqual(obj->name, name) ) {
					base = obj;
					goto found;
				}
			}*/
			return 0;

		  found:
			;
		}
	}
}

XXObject* XSWFPlayer::FindTargetA(XXObject* base, char* path,XXObject*with)
// Path notation
//
// "/fred"			- the object named fred that is a top level object of the movie
// ".."				- the thread's parent
// "fred/bob"		- the object named fred that is a top level object of the movie
// "fred"			- the object named fred that is a child of this thread
// "../../fred"		- the object named fred that is a child of this threads parent's parent
// "" - self		- blank refers to the current thread
// "_flash1/bob"	- the object named bob at the top level of layer1
{
	//return XNULL;
	if(path==XNULL) return base;
	FLASHASSERT(base);

	BOOL top = XTRUE,bDot=XTRUE;

	if ( path[0] == '/' ) {
		// Pop up to the root
		bDot=XFALSE;
		if(with)
		{
			base=with;
			with=XNULL;
		}
		//while ( base->parent && base->parent != &display.root ) {
		//	base = base->parent;
		//}
		base=base->m_pRoot->rootObject->pObject;
		path++;
		top = XFALSE;
	}

	// Parse the path elements
	while ( XTRUE ) {
		char name[128];

		// We have parsed the path, return the object
		if ( *path == 0 )
			return base;

		{// Pull off a name
			int i = 0;
			while ( *path && i < 127 ) {
				/*XBOOL bBreak=XFALSE;
				switch(*path)
				{
				case '/':bBreak
				case
				}*/
				if ( *path == '/' ) {

					path++;
					break;
				}
				else if(bDot&&*path=='.')//&&*(path+1)!='.')
				{
					if(*(path+1)=='.')
						bDot=XFALSE;
					else
					{
						path++;
						break;
					}
				}
				name[i++] = *path++;
			}
			name[i] = 0;
		}

		// See if they reference a layer directly (only check at the top element)
		if ( top ) {
			top = XFALSE;
			if(XString8::Compare(name,"_root",0,XTRUE)==0)
			{
				if(with)
				{
					base=with;
					with=XNULL;
				}
				//while ( base->parent && base->parent != &display.root ) {
				//	base = base->parent;
				//}
				base=base->m_pRoot->rootObject->pObject;
				//path++;
				//top = XFALSE;
				continue;
			}
			else
			{
				int layerNum = LayerNum(name);
				if ( layerNum >= 0 ) {
					ScriptPlayer* l = FindLayer(player, layerNum);
					if ( l && l->rootObject ) {
						base = l->rootObject->pObject;
						continue;
					} else {
						return 0;
					}
				}
			}
		}

		if ( StrEqual(name, "..") ) {
			// Visit the parent
			if(with)
			{
				base=with;
				with=XNULL;
			}
			base = base->GetParent();
			if ( !base )
				return 0;
		} else {
			// Find an object that matches the name
			XXObject*pObj=GetMember(name,with,base);
			/*if(pObj)
				XNULL;

				//XNULL;
			if(with)
			{
				pObj=with->GetMember(name);//GetChildOf(name,XTRUE);
				with=XNULL;
			}
			if(!pObj)
				pObj=base->GetMember(name);//GetChildOf(name,XTRUE);*/
			if(pObj)
			{
				base=pObj;
				goto found;
			}

			/*for ( SObject* obj = base->bottomChild; obj; obj = obj->above ) {
				// Does this object match?
				if ((obj->character->type == spriteChar || obj->character->type == spriteExternalChar)
                    && obj->name && StrEqual(obj->name, name) ) {
					base = obj;
					goto found;
				}
			}*/
			return 0;

		  found:
			;
		}
	}
}


XBOOL XSWFPlayer::GetTargetPath(SObject* base,XXVar&var)
{
	enum { MAX_TARGET_DEPTH = 256 };

	SObject* stack[MAX_TARGET_DEPTH];
	int stackPtr = 0;

	SObject* obj = base;

	// Find the path to the root
	while (obj && obj->character&&obj->character->type != rootChar) {
		stack[stackPtr++] = obj;
		if (stackPtr == MAX_TARGET_DEPTH) {
			break;
		}
		obj = obj->parent;
	}

	//URLBuilder ub;
	var.SetString(XNULL,0);

	int depth = obj->thread?obj->thread->player->layerDepth:0;
	if (depth>=0) {
		//ub.AppendString("_level");
		//ub.AppendInt(depth);
		var.StringAdd("_level");
		var.AppendInt(depth);
	}

	// Copy the names of the target paths
	while (stackPtr) {
		XXVar name = stack[--stackPtr]->name;
		var.AddChar('/');
		if(name.IsNull()) var.AddChar('?');
		else var.StringAdd(name);
		//ub.AppendChar('/');
		//ub.AppendChar('.');
		//var.StringAdd(name.IsNull()"?":name);//(name ? name : "?"));
		//ub.AppendString((char*)(name ? name : "?"));
	}

	return XTRUE;

	//if (!ub.len) {
	//	ub.AppendChar('/');
	//}
	//var.SetString(ub.buf,XString8::SafeStrlen(
	//return CreateStr(ub.buf);
}

// ConvertStringToInteger: Converts an ASCII string to an integer
//

BOOL ConvertStringToInteger( const char *s, int *value)
{
	BOOL negate = XFALSE;
	int result = 0;

	// Check sign
	if (*s == '-') {
		negate = XTRUE;
		s++;
	} else if (*s == '+') {
		s++;
	}

	// Make sure there actually are digits
	if (!*s) {
		return XFALSE;
	}

	// Read the digits
	while (*s) {
		if (*s >= '0' && *s <= '9') {
			result = result * 10 + *s - '0';
		} else {
			return XFALSE;
		}
		s++;
	}

	if (negate) {
		result = -result;
	}
	*value = result;
	return XTRUE;
}

// convertdoubletostring: Converts a XFDOUBLE-precision floating-point number
// to its ASCII representation
//

#define LOG2_10 0.301029995663981

static int NextDigit(XFDOUBLE *value)
{
  int digit;
  digit = (int) *value;
  *value -= digit;
  *value *= 10;
  return digit;
}

static int RoundInt(XFDOUBLE x)
{
	if (x < 0) {
		return (int) (x - 0.5);
	} else {
		return (int) (x + 0.5);
	}
}

XFDOUBLE PowerOfTen(int exponent)
{
	XFDOUBLE base = 10.0F;

	if (exponent < 0) {
		base = 0.1F;
		exponent = -exponent;
	}

	XFDOUBLE result = 1.0;

	while (exponent) {
		if (exponent & 1) {
			result *= base;
		}
		exponent >>= 1;
		base *= base;
	}

	return result;
}

char *ConvertDoubleToString(XFDOUBLE value)
{
	int i;
	int prec = 15; // base 10 precision

	char buffer[256];
	char *s = buffer;

	// Deal with negative numbers
	if (value < 0) {
		value = -value;
		*s++ = '-';
	}

	// Determine base 2 exponent
	int exp2;
	frexp(value, &exp2);
	exp2--;

	// Convert to base 10 exponent
	int exp10 = (int) RoundInt((XFDOUBLE)exp2 * LOG2_10);

	// Obtain base 10 mantissa
	XFDOUBLE pow10 = PowerOfTen(exp10);
	XFDOUBLE f = value / pow10;

	// Adjust for round-off error on exponent
	if ((int) f == 0) {
		exp10--;
		pow10 = PowerOfTen(exp10);
		f = value / pow10;
	}

	// Sentinel is used for rounding
	char *sentinel = s;

	if (exp10 < 0 && exp10 > -6) {
		// Number is of form 0.######
		if (exp10 < -prec) {
			exp10 = -prec-1;
		}
		sentinel = s;
		*s++ = '0'; // Sentinel
		*s++ = '0';
		*s++ = '.';

		// Write out leading zeros
		for (i=exp10; i<-1; i++) {
			*s++ = '0';
		}
		// Write out significand
		for (i=0; i<prec; i++) {
			*s++ = NextDigit(&f) + '0';
		}
		exp10 = 0;
	} else if (exp10 >= prec) {
		*s++ = NextDigit(&f) + '0';
		*s++ = '.';
		for (i=0; i<prec-1; i++) {
			*s++ = NextDigit(&f) + '0';
		}
	} else {
		int digits = 0;
		sentinel = s;
		*s++ = '0';
		i = NextDigit(&f);
		if (i) {
			*s++ = i + '0';
		}
		while (exp10 > 0) {
			*s++ = NextDigit(&f) + '0';
			exp10--;
			digits++;
		}
		*s++ = '.';
		for (; digits < prec-1; digits++) {
			*s++ = NextDigit(&f) + '0';
		}
    }

	// Rounding
	i = NextDigit(&f);
	if (i > 4) {
		char *ptr = s-1;
		while (ptr >= buffer) {
			if (*ptr < '0') {
				ptr--;
				continue;
			}
			(*ptr)++;
			if (*ptr != 0x3A) {
				break;
			}
			*ptr-- = '0';
		}
    }

	/* Remove trailing zeros */
	while (*(s-1) == '0') {
		s--;
	}
	if (*(s-1) == '.') {
		s--;
	}

	if(exp10) {
		*s++ = 'e';
		char *expstr = ConvertIntegerToString(exp10);
		char *t = expstr;
		while (*t) { *s++ = *t++; }
		FreeStr(expstr);
	}

	*s = '\0';

	if (sentinel && sentinel[0] == '0' && sentinel[1] != '.') {
		char *s = sentinel;
		char *t = sentinel+1;
		while ((*s++ = *t++) != 0);
	}

	char *result = new char[strlen(buffer) + 1];
	strcpy(result, buffer);
	return result;
}



// ConvertStringToDouble: Converts an ASCII string in the form
//     [+-]######.######e[+-]###
// to a XFDOUBLE-precision floating-point number
//

BOOL ConvertStringToDouble( const char *s, XFDOUBLE *value)
{
	BOOL negate = XFALSE;
	int exp10 = 0;
	XFDOUBLE result = 0;

	// Handle sign
	if (*s == '-') {
		negate = XTRUE;
		s++;
	} else if (*s == '+') {
		s++;
	}
	// Make sure it's not - and + and nothing else
	if (!*s) {
		return XFALSE;
	}

	// Read digits to left of decimal point
	while (*s) {
		if (*s >= '0' && *s <= '9') {
			result = result * 10 + *s++ - '0';
		} else {
			break;
		}
	}

	// If decimal point encountered, read
	// digits to right of decimal point
	if (*s == '.') {
		while (*++s) {
			if (*s >= '0' && *s <= '9') {
				result = result * 10 + *s - '0';
				exp10--;
			} else {
				break;
			}
		}
	}

	// Handle exponent
	if (*s == 'e' || *s == 'E') {
		int num;
		if (!ConvertStringToInteger(++s, &num)) {
			return XFALSE;
		}
		exp10 += num;
	} else if (*s) {
		// Unknown character encountered
		return XFALSE;
	}

	if (negate) {
		result = -result;
	}
	*value = result * PowerOfTen(exp10);
	return XTRUE;
}

BOOL IsTrue( const char* c)
{
	XFDOUBLE numval;
	if ( ConvertStringToDouble(c, &numval) )
	{
		return numval != 0;
	}
	return StrEqual(c, "true") || StrEqual(c, "yes");
}
// ConvertIntegerToString: Converts a signed integer to its ASCII representation

char *ConvertIntegerToString(int value)
{
	URLBuilder ub;
	if (value < 0) {
		ub.AppendChar('-');
		value = -value;
	}
	ub.AppendInt(value);
	return CreateStr(ub.buf);
}

#ifdef _CELANIMATOR
// Generate an error message
void ShowTargetError(XSWFPlayer* player, SObject*, char*);

// Send a string to the trace window
void TraceString(const char *message);
void ExecutionLimitError();
void CallLimitError();
#endif



////////////////////////////////////////////////////////////
// Clone
//      Create a new sprite that is a copy of the current one.
//      NOTE: State information (current frame, variables, etc.)
//              is not cloned.
//
// inputs:
//      thread  - the sprite we want to clone
//      name    - the name of the new sprite that we are about to
//                  create. space for this name has been allocated
//                  no need to copy.
//      depth   - what layer we want to place the cloned sprite in
/*void CloneSprite(ScriptThread *thread, char *name, int depth)
{
    if (thread)
    {
        // don't allow duplication of main timeline
        if (thread == thread->player)
            return;

        thread->rootObject->parent->thread->CloneSprite(thread, name, depth);
    }
}*/

void XSWFPlayer::RemoveSprite(ScriptThread *thread, BOOL createPlaceholder)
{
    if (thread)
    {
        if (thread->rootObject->parent == &display.root)
		{
			if(thread->layerDepth)
				ClearLayer(thread->layerDepth);
		}
        else
            thread->rootObject->parent->thread->RemoveSprite(thread, createPlaceholder);
    }
}


void XSWFPlayer::SetProperty(ScriptThread *thread, int propType, XXVar&var)//XFDOUBLE value)
{
    if (thread == 0||var.IsNaN())
        return;

    SObject *object = thread->rootObject;

    switch (propType)
    {
		// Sprite properties
		case kSpriteWidth:
			 object->SetWidth(var.ToFloat());
			 break;
		case kSpriteHeight:
			 object->SetHeight(var.ToFloat());
			 break;
        case kSpritePosX:
			 object->SetPosX(var.ToFloat());
			/*{
				SCOORD tx = (SCOORD) (value * 20); // turn into twips
				if ( object->xform.mat.tx != tx ) {
					object->xform.mat.tx = tx;
					object->Modify();
				}
				object->puppet = XTRUE;
			} */break;
        case kSpritePosY: /*{
				SCOORD ty = (SCOORD) (value * 20); // turn into twips
				if ( object->xform.mat.ty != ty ) {
					object->xform.mat.ty = ty;
					object->Modify();
				}
				object->puppet = XTRUE;
            } */
			object->SetPosY(var.ToFloat());//value);
			break;
        case kSpriteScaleX: /*{
				SFIXED scalex = DoubleToFixed(value/100.0);
				SPOINT pt;
				pt.x = object->xform.mat.a;
				pt.y = object->xform.mat.b;
				SFIXED curScalex = PointLength(&pt);
				if ( !FixedEqual(scalex, curScalex, 0x10) ) {
					SFIXED rotX = FixedAtan2(object->xform.mat.b, object->xform.mat.a);
					object->xform.mat.a =  FixedMul(scalex, FixedCos(rotX));
					object->xform.mat.b =  FixedMul(scalex, FixedSin(rotX));
					object->Modify();
				}
				object->puppet = XTRUE;
            }*/
			object->SetScaleX(var.ToFloat());//value);
			break;
        case kSpriteScaleY:/*{
				SFIXED scaley = DoubleToFixed(value/100.0);
				SPOINT pt;
				pt.x = object->xform.mat.c;
				pt.y = object->xform.mat.d;
				SFIXED curScaley = PointLength(&pt);
				if ( !FixedEqual(scaley, curScaley, 0x10) ) {
					SFIXED rotY = FixedAtan2(-object->xform.mat.c, object->xform.mat.d);
					object->xform.mat.c = -FixedMul(scaley, FixedSin(rotY));
					object->xform.mat.d =  FixedMul(scaley, FixedCos(rotY));
					object->Modify();
				}
				object->puppet = XTRUE;
            } */
			object->SetScaleY(var.ToFloat());//value);
			break;
        case kSpriteAlpha: {
				// (a value between 0 and 100 %)
				/*S16 aa = (S16) (value * (256.0 / 100.0));
				if ( object->xform.cxform.aa != aa ) {
					object->xform.cxform.aa = aa;
					object->xform.cxform.CalcFlags();
					object->Modify();
				}
				object->puppet = XTRUE;*/
				object->SetAlpha(var.ToFloat());//value);
            } break;
        case kSpriteVisible: {
				// (if zero this means we don't hit test the object)
				int vis = var.ToLogic();//(value != 0.0f) ? 1 : 0;
				object->SetVisible(vis);
				/*if ( object->visible != vis ) {
					object->visible = vis;
					object->Modify();
				}*/
			} break;
        case kSpriteRotate:
            /*{
				MATRIX mat = object->xform.mat;
				SPOINT vec;
				vec.x = mat.a;
				vec.y = mat.b;
				SFIXED scalex = PointLength(&vec);

				vec.x = mat.c;
				vec.y = mat.d;
				SFIXED scaley = PointLength(&vec);

				SFIXED rotX = FixedAtan2(mat.b, mat.a);
				SFIXED rotY = FixedAtan2(-mat.c, mat.d);
				SFIXED newRot = DoubleToFixed(value);

				mat.a =  FixedMul(scalex, FixedCos(newRot));
				mat.b =  FixedMul(scalex, FixedSin(newRot));
				mat.c = -FixedMul(scaley, FixedSin(rotY-rotX + newRot));
				mat.d =  FixedMul(scaley, FixedCos(rotY-rotX + newRot));

				if ( !MatrixEqual(&mat, &object->xform.mat, 0x10) ) {	// note that we don't change the tx or ty values ever...
					object->xform.mat = mat;
					object->Modify();
				} else {
					//FLASHOUTPUT("skip\n");
				}

                object->puppet = XTRUE;
            }*/
			object->SetRotate(var.ToFloat());
            break;

		// Global properties
		case kSpriteHighQuality:
			{
			XFDOUBLE value=var.ToFloat();
			SetQuality(value);
			//highQuality = (value != 0.0);
			//if(value>=3.0) nQuality=XConfig::HIGH;
			//else if(value>=1.0) nQuality=XConfig::MID;
			//else nQuality=XConfig::LOW;
			//bestDither = (value > 1.5);	// really should be 2.0 but be flexible...
			}
			break;
		case kSpriteFocusRect:
			display.useFocusRect = var.ToLogic();//(value != 0.0);
			if (!RectIsEmpty(&display.tabFocusRect)) {
				display.InvalidateRect(&display.tabFocusRect);
			}
			break;
		case kSpriteSoundBufferTime:
			nSoundBufferTime = var.ToInt();//(int) value;
			break;
		case kSpriteQulity:
			break;
        default:
            // we should never get here
            FLASHASSERT(0);
    }
}

void XSWFPlayer::SetPropertyStr(ScriptThread *thread, int propType,XXVar&var)// char *value)
{
    if (thread == 0)
        return;
    SObject *object = thread->rootObject;

	switch (propType) {
	case kSpriteName:
		//FreeStr(object->name);
		var.ToString(XFALSE);
		object->name=var;//CreateStr(var.strTxt);
		//var.ToString();
		//object->name = var.StringTo();
		//var.nType=XODT_INT;
			//CreateStr();
		break;
	case kSpriteTarget:
	case kSpriteURL:
	case kSpriteDropTarget:
		// (can only get, not set)
		break;
	default:
		{
			SetProperty(thread,propType,var);
			/*XFDOUBLE num;
			if (ConvertStringToDouble(value, &num)) {
				SetProperty(thread, propType, num);
			}*/
		}
		break;
	}
}



XXObject *XSWFPlayer::ResolveVariable(const XXVar&name,XSWFCONTEXT&cnt,XXVar&varName,XBOOL&bFirst)
{

	char *colon = strrchr(name.strTxt,':');
	if (colon) {
		char* endStr=colon+name.GetLength();
		XXObject*obj=cnt.thread;//->m_pObject;
		if (!obj) {
			// Use the root
			obj = player->rootObject->pObject;
		}
		//XXObject*po=obj;
		//obj=THREADOF(obj);


		bFirst=XFALSE;
		XXVar targetName;
		targetName.SetString(name.strTxt,colon-name.strTxt);
		//char *targetName = CreateStr(name.strTxt, colon-name);

		//if(strcmp(targetName,"_level0/Pileinfo01/")==0)
		//	int v=0;

		XXObject* result = FindTargetA(obj, targetName.strTxt,cnt.pWith?cnt.pWith:XNULL);
//		FreeStr(targetName);
		//if(*colon==':') colon++;
		//*varName = colon+1;
		varName.SetString(colon+1,endStr-colon-1);
		//if(!result) return XNULL;
		return result;//->pObject;
	} else {
		varName = name;
		return cnt.pThis;
	}
}

//DEL XXObject* XSWFPlayer::SeekVariable(XSWFCONTEXT &cnt, char *name, XXObject*base,XXVar &var)
//DEL {
//DEL 	/*XXObject*pObj=cnt.withs.GetLast();
//DEL 	if(pObj&&pObj!=base)
//DEL 	{
//DEL 		if(pObj->GetMember(cnt,name,var,XTRUE))
//DEL 			return pObj;
//DEL 	}
//DEL 	pObj=cnt.blocks.GetLast();
//DEL 	if(pObj&&pObj!=base)
//DEL 	{
//DEL 		if(pObj->GetMember(cnt,name,var,XTRUE))
//DEL 			return pObj;
//DEL 	}
//DEL 	pObj=cnt.locals.GetLast();
//DEL 	if(pObj&&pObj!=base)
//DEL 	{
//DEL 		if(pObj->GetMember(cnt,name,var,XTRUE))
//DEL 			return pObj;
//DEL 	}*/
//DEL 	/*int nSize=cnt.seekPaths.GetSize()-1;
//DEL 	for(int i=nSize;i>=0;i--)
//DEL 	{
//DEL 		XXObject*pObj=cnt.seekPath[i];
//DEL 		if(pObj!=base)
//DEL 		{
//DEL 			if(pObj->GetMember(cnt,name,var,XTRUE)) return pObj;
//DEL 		}
//DEL 	}*/
//DEL 	if(cnt.pWith&&cnt.pWith!=base)
//DEL 	{
//DEL 		if(cnt.pWith->GetMember(cnt,name,var,XTRUE)) return cnt.pWith;
//DEL 	}
//DEL 	if(base->GetMember(cnt,name,var,XTRUE)) return base;
//DEL 	return XNULL;
//DEL }


XXObject* XSWFPlayer::ResolveVariableByDot(XSWFCONTEXT&cnt,const XXVar&name, XXObject *base,XXVar&varName,XBOOL& bFirst)
{

	char *lpText=name.strTxt;
	char *colon = strchr(lpText, '.');
	//char *endStr=name.strTxt+name.GetLength();
	varName=name;
	XBOOL bSet=XFALSE;
	//if(!colon||!base) return;
	while(colon&&base)
	{
		bSet=XTRUE;
		//char*objName=CreateStr(*varName,colon-*varName);
		XXVar objName;
		objName.SetString(lpText,colon-lpText);
		XXVar var;
		//XBOOL bOK=GetGlobal(cnt,objName,base,var);
		XBOOL bOK=XFALSE;
		if(bFirst)
		{
			bOK=GetGlobal(objName.strTxt,cnt.thread,cnt.pThis,cnt.pObject,var);
			if(!bOK)
				bOK=GetMember(cnt,objName.strTxt,var,XTRUE,cnt.pWith,cnt.thread,cnt.thread->m_pRoot->m_pGlobal);
			/*	&&cnt.pWith&&cnt.pWith!=cnt.thread)
				bOK=cnt.pWith->GetMember(cnt,objName,var,XTRUE);
			if(!bOK)
				bOK=cnt.thread->GetMember(cnt,objName,var,XTRUE);

			//bOK=SeekVariable(cnt,objName,base,var)!=XNULL;
			if(!bOK)
			{
				bOK=cnt.thread->m_pRoot->m_pGlobal->GetMember(cnt,objName,var,XTRUE);
			}*/
		}
		else
		{
			bOK=GetMember(cnt,objName.strTxt,var,XTRUE,base);
				//base->GetMember(cnt,objName,var,XTRUE);
			if(!bOK)
				bOK=GetGlobal(objName.strTxt,base,base,base,var);
		}
		//if(!bOK)
		//	bOK=GetGlobal(cnt,objName,base,var);
		if(bOK)
		{
			if(var.IsObject())
				base=var.pObject;
			else
				base=XNULL;
		}
		else
			base=XNULL;
		lpText=colon+1;
		//varName.SetString(colon+1,XString8::SafeStrlen(colon+1));
		colon=strchr(lpText,'.');
		bFirst=XFALSE;
	}
	if(bSet)
		varName.SetString(lpText,name.strTxt-lpText+name.GetLength());

	return base;
}

static XPCTSTR _strGlobalAttr[]=
{

	_XXKEY("."),
	_XXKEY(".."),
	_XXKEY("_accProps"),
	_XXKEY("_focusRect"),
	_XXKEY("_global"),
	_XXKEY("_highquality"),
	/*_XXKEY("_level0"),
	_XXKEY("_level1"),
	_XXKEY("_level2"),
	_XXKEY("_level3"),
	_XXKEY("_level4"),
	_XXKEY("_level5"),
	_XXKEY("_level6"),
	_XXKEY("_level7"),
	_XXKEY("_level8"),
	_XXKEY("_level9"),*/

	_XXKEY("_parent"),
	_XXKEY("_quality"),
	_XXKEY("_root"),
	_XXKEY("_soundbuftime"),
	//_XXKEY("length"),
	//_XXKEY("maxscroll"),
	//_XXKEY("scroll"),
	_XXKEY("flash"),
	_XXKEY("super"),
	_XXKEY("this"),
};

enum {

	  XGA_PPOINT=0,
	  XGA_PPPOINT=1,
	  XGA_ACCPROPS,
      XGA_FOCUSRECT,
	  XGA_GLOBAL,
	  XGA_HIGHQUALITY,
	  /*XGA_LEVEL0,
	  XGA_LEVEL1,
	  XGA_LEVEL2,
	  XGA_LEVEL3,
	  XGA_LEVEL4,
	  XGA_LEVEL5,
	  XGA_LEVEL6,
	  XGA_LEVEL7,
	  XGA_LEVEL8,
	  XGA_LEVEL9,*/
	  XGA_PARENT,
	  XGA_QUALITY,
	  XGA_ROOT,
	  XGA_SUNDBUFTIME,
	  //XGA_LENGTH,
	  //XGA_MAXSCROLL,
	  //XGA_SCROLL,
	  XGA_FLASH,
	  XGA_SUPER,
	  XGA_THIS};

//const XPCTSTR _strQuality[]={"LOW","MEDIUM","HIGH"};
//XXObject* SeekVariable(char*name,
//XXObject* GetRoot(XXObject
#define ROOTOF(OBJ) (OBJ->m_pRoot->rootObject->pObject)
/*XXObject* XSWFPlayer::ResolveVariable(char *name,XSWFCONTEXT&cnt, char **varName,XBOOL&bFirst)
{
	XXObject*base=cnt.pThis?cnt.pThis:cnt.thread;
	if(name[0]=='/')
	{
		bFirst=XFALSE;
		name++;
		base=ROOTOF(cnt.thread);
	}
	*varName=name;
	//while(*name)
	//if(name[0]=='/')
	char*path=XNULL;
	//XBOOL bPath=XFALSE;
	XXVar var;
	int nc=0;
	XBOOL bOK=XFALSE;
	for(;;name++)
	{
		//char* path=XNULL;

		switch(*name)
		{
		case 0:return base;
		case '/':
			if(!name[1])
			{
				*name=0;
				bFirst=XFALSE;
				return base;
			}
		case ':':
		case '.':
			if(!name[1]) return XNULL;
			bOK=XFALSE;
			nc=name-*varName;
			if(!nc) break;//return XNULL;
			path=CreateStr(*varName,nc);
			*varName=name+1;
			if(!bFirst)
			{
				bOK=base->GetMember(cnt,path,var,XTRUE);
				if(!bOK)
					bOK=GetGlobal(path,base,base,var);
			}
			else
			{
				bFirst=XFALSE;
				bOK=GetGlobal(path,cnt.thread,base,var);
				if(!bOK&&cnt.pWith)
					bOK=cnt.pWith->GetMember(cnt,path,var,XTRUE);
				if(!bOK)
					bOK=base->GetMember(cnt,path,var,XTRUE);
				//if(!bOK)
				//	bOK=GetGlobal(path,base,cnt.pThis?cnt.pThis:base,var);
			}

			FreeStr(path);
			if(!bOK||!var.IsObject()) return XNULL;
			base=var.pObject;
		}
	}

	return base;
};*/

ScriptThread* XSWFPlayer::ResolveFrameNum(XXVar&var,XSWFCONTEXT&cnt, int *frameNum)
{
	ScriptThread*thread=CLIPTHREAD(cnt.thread);
	if (!thread) {
		// Use the root
		thread = player;
	}

	//char *funcName = NULL;
	XXVar funcName(_strEmpty);
	XBOOL bFirst=XTRUE;
	ScriptThread *targetThread = XNULL;
	if(var.IsString2())//var.IsNum3())
	{
		var.ToString(XFALSE);
		targetThread=thread;

		*frameNum=Number(var.strTxt)-1;//var.ToInt()-1;
		if(*frameNum>=0)
			return targetThread;
		//if(*frameNum<0) return XNULL;
	}
		//ResolveVariable(name, thread, &funcName,with,bFirst);
	//if (targetThread)
	//else
	{
		var.ToString(XFALSE);
		XXObject*obj=ResolveVariable(var, cnt, funcName,bFirst);
		targetThread=(obj&&obj->m_pObject)?obj->m_pObject->thread:XNULL;
		if(targetThread)
		{
			// Convert function name/number to frame number
			if (ConvertStringToInteger(funcName.strTxt, frameNum)) {
				(*frameNum)--;
			} else {
				*frameNum = targetThread->FindLabel(funcName.strTxt);
			}
			// Fail if label not found, or if frame number is negative
			if (*frameNum < 0) {
				return targetThread;
			}
		}
	}
	return targetThread;
}

XBOOL XSWFPlayer::SetGlobal(XPCTSTR name, XXVar &var,XBOOL bLowCase)
{
	XSortConstStr sort(_strGlobalAttr,sizeof(_strGlobalAttr)/sizeof(XPCTSTR));
	int id=sort.Index((void*)name,bLowCase?XSS_NOCASE:XSS_NORMAL);
	if(id<0) return XFALSE;
	//tmpvar.ToError(XXObject::XSC_UNDEFINE);
	switch(id)
	{
	case XGA_ROOT:break;
	case XGA_SUNDBUFTIME:
	//case XGA_MAXSCROLL:
	//case XGA_SCROLL:
		nSoundBufferTime=var.ToInt();
		var=nSoundBufferTime;
		break;
	case XGA_HIGHQUALITY:
		{
			XFDOUBLE v=var.ToFloat();
			SetQuality(v);
		}
		//var=highQuality;
		//var.ToLogic();
		//highQuality=var.ToLogic();
		//SetCamera(updateLazy);
		break;
	case XGA_QUALITY:
		/*if(highQuality)
			var="HIGH";
		else
			var="LOW";*/
		var.ToString(XFALSE);
		SetQuality(var.strTxt);
		//SetCamera(updateLazy);
		break;
	case XGA_SUPER:

		 break;
	case XGA_FOCUSRECT:
		 display.useFocusRect=var.ToLogic();
		  break;
		 //return XTRUE;
	default:
		 return XFALSE;
	}
	return XTRUE;
}

int XSWFPlayer::GetGlobal(XPCTSTR name, XXObject *pFun,XXObject*pThis,XXObject*pObject,XXVar &var)
{
	//if(!pFun) return XNULL;
	int layer=LayerNum((char*)name);
	if(layer>=0)
	{
		ScriptPlayer*p=FindLayer(player,layer);
		if(p)
			var=p->rootObject->pObject;
		else
			var.ToConst(XOCT_UNDEFINE);
		return XTRUE;
	}
	XSortConstStr sort(_strGlobalAttr,sizeof(_strGlobalAttr)/sizeof(XPCTSTR));
	int id=sort.Index((void*)name,pFun->m_pRoot->NeedLowCase()?XSS_NOCASE:XSS_NORMAL);
	if(id<0) {return XFALSE;}
	//tmpvar.ToError(XXObject::XSC_UNDEFINE);
	switch(id)
	{
	case XGA_ACCPROPS:
		 var.ToConst(XOCT_UNDEFINE);
		 return XTRUE;
		 //return XXObjectCreator::CreateConst(XOCT_UNDEFINE);
//	case XGA_LENGTH:
//		{
//			pFun->Length(var);
//			return XTRUE;
		  //XXObject*p=pFun->Length();
		  //pFun->FREE();
		  //return p;
//		}
		 //return &tmpvar;
		 //break;
	case XGA_SUNDBUFTIME:
	//case XGA_MAXSCROLL:
	//case XGA_SCROLL:
		var=nSoundBufferTime;
		break;
	case XGA_PPPOINT:
		var=pFun->GetParent();
		break;
	case XGA_PPOINT:
		var=pFun;
		break;
	//case XGA_HIGHQUALITY:
	//	var=nQuality;//highQuality;
	//	//var.ToLogic();
	//	break;
	case XGA_QUALITY:
		/*if(bestDither)
			var="HIGH";
		else if(highQuality)
			var="MEDIUM";
		else
			var="LOW";*/
		var="HIGH";
		break;
	case XGA_PARENT:
		{
			XXObject*p=pThis->GetParent();
			if(p)
				var=p;
			else
				var.ToConst(XNULL);
		}
		 break;
	case XGA_SUPER:
		{
		 XXObject*p=pObject?pObject->GetSuper():pFun->GetSuperObject();

		 if(p) var=p;
		 else var.ToConst(XOCT_UNDEFINE);
		}
		 break;
	case XGA_FOCUSRECT:
		 var=display.useFocusRect;
		  break;
		 //return XTRUE;
	case XGA_FLASH:
	case XGA_GLOBAL:

		 var=pFun->GetGlobal();
			 //XXObjectCreator::GetGlobal();
		 return XTRUE;
	case XGA_ROOT:
		{
			ScriptThread*p=pFun->GetRoot();
			if(p)
				var=p->rootObject->pObject;
			else
				var.ToConst(XOCT_UNDEFINE);
		}
		 break;
/*	case XGA_LEVEL0:
	case XGA_LEVEL1:
	case XGA_LEVEL2:
	case XGA_LEVEL3:
	case XGA_LEVEL4:
	case XGA_LEVEL5:
	case XGA_LEVEL6:
	case XGA_LEVEL7:
	case XGA_LEVEL8:
	case XGA_LEVEL9:
		{
			ScriptPlayer*p=FindLayer(&player,id-XGA_LEVEL0);
			if(p)
				var=p->rootObject->pObject;
			else
				var.ToConst(XOCT_UNDEFINE);
		}*/
		 //var=contexts.GetFirst()->GetObject();
		 return XTRUE;
		 //return contexts.GetFirst()->GetObject();
	case XGA_THIS:
		 var=pThis;
		 return XTRUE;
	default:
		return XFALSE;
	}

	return XTRUE;
}


XBOOL XSWFPlayer::RemoveVariable(XSWFCONTEXT &cnt,XPCTSTR strName)
{
	//char *varName = NULL;
	XXVar varName(_strEmpty);
	XBOOL bFirst=XTRUE;
	//XXObject* targetObj = ResolveVariable((char*)strName,cnt, &varName,bFirst);
	XXVar tempVar(strName);
	XXObject* base = ResolveVariable(tempVar,cnt, varName,bFirst);
	//if(bFirst) targetObj=obj;
	if(base==XNULL) return XFALSE;
	//XXObject*object=targetObj->pObject;//targetThread->rootObject->pObject;
	//XBOOL bFirst=targetThread==thread;
	//XXObject*
	XXVar name=varName;
	base=ResolveVariableByDot(cnt,name,base,varName,bFirst);
	if(!base) return XFALSE;
	if(!bFirst)
	{
		return base->RemoveMember(varName.strTxt);//GetMember(cnt,varName,var,XTRUE);
	}
	else
	{
		if(cnt.pWith&&cnt.pWith!=base)
		{
			if(cnt.pWith->RemoveMember(varName.strTxt)) return XTRUE;
		}

		return cnt.thread->RemoveMember(varName.strTxt);
		/*if(base->RemoveMember(varName)) return XTRUE;

		if(base!=cnt.thread)
		{
			//XXObject*pObj=thread->rootObject->pObject;
			//if(pObj!=base)
				return cnt.thread->RemoveMember(strName);
		}*/
		//return base->RemoveMember(varName);

		return XFALSE;
		//if(base->RemoveMember(varName)) return XTRUE;

		//if(base->thread->rootObject!=base)
		//	return base->thread->rootObject->pObject->RemoveMember

		//return XFALSE;
		//if(base->GetMember(cnt,name,var,XTRUE)) return base;
		//return XNULL;
		//return GetGlobal(cnt,varName,targetObject,var)) return XTRUE;
		//if(SeekVariable(cnt,varName,targetObject,var)!=XNULL)
		//	return XTRUE;
		//if(GetGlobal(cnt,varName,targetObject,var)) return XTRUE;
		//return XXObjectCreator::_global.GetMember(cnt,varName,var,XFALSE);
	}

}


XBOOL XSWFPlayer::SetVariable(XSWFCONTEXT&cnt,const XXVar&name, XXVar&var,EditText*pEdit)//BOOL updateFocus)
{
	//if(strcmp(name.strTxt,"Game")==0) //return XFALSE;
	//	int v=0;
	XXVar varName(_strEmpty);
	XBOOL bFirst=XTRUE;
	XXObject* base = ResolveVariable(name, cnt, varName,bFirst);
	if(base==XNULL)
	{
		return XFALSE;
	}
	XXVar newName=varName;
	base=ResolveVariableByDot(cnt,newName,base,varName,bFirst);
	if(!base)
	{
#ifdef _VARCACHE_
		cnt.pObject=XNULL;
#endif
		return XFALSE;
	}
	if(!bFirst)
	{
		return base->SetMember(cnt,varName,var,pEdit);
	}
	else
	{
		if(SetGlobal(varName.strTxt,var,base->m_pRoot->NeedLowCase()))
		{
#ifdef _VARCACHE_
			cnt.pObject=XNULL;
#endif
			return XTRUE;
		}

		if(cnt.pWith&&cnt.pWith!=cnt.thread)
		{
			if(cnt.pWith->SetMemberOnly(cnt,name.strTxt,var,pEdit))
			{
				return XTRUE;
			}
		}

		cnt.thread->SetMember(cnt,name,var,pEdit);

		return XTRUE;
	}

}




XBOOL XSWFPlayer::GetVariable(XSWFCONTEXT&cnt,char *name,XXVar&var,XBOOL bInit)
{
	if(!player||!player->rootObject) return XFALSE;
//#if (__CORE_VERSION__>=0x02075000)
	if(name==NULL)
		return XFALSE;
//#endif
//	if(strcmp(name,"engine")==0) //return XFALSE;
//		int v=0;
//		int v=0;
//	//return XFALSE;
		//int v=0;
	//	tt=var.iData32;
		//int v=0;
	//char *result = NULL;
	//XBOOL bOK=XFALSE;
//	return XFALSE;
//	if(strcmp(name,"muted")==0)
//		int v=0;
	XXVar varName(_strEmpty);// = _strEmpty;//NULL;
//	if(obj==XNULL) obj=base;
//	if(obj==XNULL) return XFALSE;
	/*SObject*with=XNULL;
	XXObject*pObj=cnt.withs.GetLast();
	if(pObj)
	{
		ScriptThread*p=pObj->GetTarget(&cnt);
		if(p) with=p->rootObject;
	}*/
	//int v=sizeof(XXObjectClip);

//	if(strcmp(name,"mcBackground")==0)//"clickPlay")==0)
//		int v=0;

	XBOOL bFirst=XTRUE;
	XXVar tempVar(name);
	XXObject* targetObject = ResolveVariable(tempVar, cnt, varName,bFirst);
	if(targetObject==XNULL)
	{
//		_ANALYLEAVE();
		return XFALSE;
	}
	//XXObject*object=targetThread->pObject;//rootObject->pObject;
	//XBOOL bFirst=targetThread==thread;
	//XXObject*
	XXVar newName(varName);
	targetObject=ResolveVariableByDot(cnt,newName,targetObject,varName,bFirst);
	if(!targetObject)
	{
#ifdef _VARCACHE_
		cnt.pObject=XNULL;
#endif
		return XFALSE;
	}

	if(!bFirst)
	{
		return GetMember(cnt,varName.strTxt,var,XTRUE,targetObject);
//		_ANALYLEAVE();
//		return bOK;
		//return targetObject->GetMember(cnt,varName,var,XTRUE);
	}
	else
	{
		//return GetGlobal(cnt,varName,targetObject,var)) return XTRUE;

		if(GetGlobal(varName.strTxt,cnt.thread,cnt.pThis?cnt.pThis:cnt.thread,cnt.pObject,var))

		{
#ifdef _VARCACHE_
			cnt.varCache.pObject=cnt.thread;
			cnt.varCache.pObjectThis=XNULL;
			cnt.varCache.nRefID=0;
#endif
			return XTRUE;
		}
		if(bInit)
			return GetMember(cnt,varName.strTxt,var,XTRUE,targetObject->GetGlobal(),cnt.pWith,cnt.thread);
		else
			return GetMember(cnt,varName.strTxt,var,XTRUE,cnt.pWith,cnt.thread,targetObject->GetGlobal());
//		_ANALYLEAVE();
		//return bOK;
		/*if(cnt.pWith&&cnt.pWith!=cnt.thread)
		{
			if(cnt.pWith->GetMember(cnt,varName,var,XTRUE)) return XTRUE;
		}

		if(cnt.thread->GetMember(cnt,varName,var,XTRUE)) return XTRUE;


		return targetObject->GetGlobal()->GetMember(cnt,varName,var,XFALSE);*/
	}

	//if(targetThread!=thread)


	/*XXObject*object=targetThread->rootObject->pObject;
	XXObject*targetObject=ResolveVariable(cnt,name,object,&varName);
	if(targetObject==XNULL) return XFALSE;

	if(GetGlobal(cnt,varName,targetObject,var)) return XTRUE;

	if(targetObject!=object||
	   )
	{
		return targetObject->GetMember(cnt,varName,var,XTRUE);
	}
	if(targetThread!=thread)
	{

	}*/
/*

	if (targetThread) {
#ifdef EDITTEXT
		char *dot = strchr(varName, '.');
		if (dot) {
			// Suffix encountered.  It must be scroll or maxscroll.
			int operation;
			if (StrEqual(dot, ".scroll")) {
				operation = editTextGetScroll;
			} else if (StrEqual(dot, ".maxscroll")) {
				operation = editTextGetMaxScroll;
			} else {
				// Invalid suffix, return nothing.
				return NULL;
			}
			// Construct name without suffix
			char *editTextName = CreateStr(varName, dot-varName);
			if (editTextName) {
				int intval;
				if (display.root.ScrollEditText(targetThread, editTextName, operation, intval)) {
					//result = ConvertIntegerToString(intval);
					var=intval;
					bOK=XTRUE;
				}
				FreeStr(editTextName);
			}
		} else
#endif
		{
			return targetThread->GetVariable(cnt,varName,var);
			//ScriptVariable* var = targetThread->FindVariable(varName);

			//if (var) {
				//result = CreateStr(var->value);
			//}
		}
	}*/

	//return result;
}

XBOOL XSWFPlayer::RemoveEditText(XSWFCONTEXT&cnt,char *name, EditText*pEdit)
{
	XXVar varName(_strEmpty);
	//XXObject*pWithObj=cnt.withs.GetLast();
	XBOOL bFirst=XTRUE;
	XXVar tempVar(name);
	XXObject* targetObject = ResolveVariable(tempVar, cnt, varName,bFirst);
	if(targetObject==XNULL) return XFALSE;
//	XXObject*object=targetThread->pObject;
	//XXObject*
	XXVar newName(varName);
	targetObject=ResolveVariableByDot(cnt,newName,targetObject,varName,bFirst);
	if(!targetObject) return XFALSE;
	if(!bFirst)
		return targetObject->RemoveEditText(cnt,varName.strTxt,pEdit);
	else
	{
		if(cnt.pWith&&cnt.pWith!=targetObject)
		{
			if(cnt.pWith->RemoveEditText(cnt,name,pEdit)) return XTRUE;
		}
	    //if(targetObject->RemoveEditText(cnt,name,pEdit)) return XTRUE;
		//if(targetObject!=cnt.baseThread)
		{
			return cnt.thread->RemoveEditText(cnt,name,pEdit);
		}
		return XFALSE;
		//return XTRUE;
	}

}


void GetBoundingBox(SObject* obj, P_SRECT bounds);

XFDOUBLE XSWFPlayer::GetProperty(ScriptThread *thread, int propType)
{

    XFDOUBLE result = 0.0;
    SObject *object = 0;

    if (thread == 0)
        goto exit_gracefully;

    object = thread->rootObject;

    switch (propType)
    {
        case kSpritePosX:
            //result = (XFDOUBLE) object->xform.mat.tx;
            //result /= 20; // convet twips into pixels.
			result=object->GetPosX();
            break;
        case kSpritePosY:
            //result = (XFDOUBLE) object->xform.mat.ty;
            //result /= 20; // convet twips into pixels.
			result=object->GetPosY();
            break;
        case kSpriteScaleX: /*{
			SPOINT vec;
			vec.x = object->xform.mat.a;
			vec.y = object->xform.mat.b;
			result = FixedToDouble(PointLength(&vec)*100);
			}*/
			result=object->GetScaleX();
			break;
        case kSpriteScaleY: /*{
			SPOINT vec;
			vec.x = object->xform.mat.c;
			vec.y = object->xform.mat.d;
			result = FixedToDouble(PointLength(&vec)*100);
			}*/
			result=object->GetScaleY();
			break;
        case kSpriteCurFrame:
            result = (XFDOUBLE) (thread->curFrame+1);
            break;
        case kSpriteTotalframes:
            result = (XFDOUBLE) thread->numFrames;
            break;
        case kSpriteAlpha:
            result = object->GetAlpha();//(XFDOUBLE) (object->xform.cxform.aa * 100) / 256;
            break;
        case kSpriteVisible:
            result = object->IsVisible();//visible;
            break;
        case kSpriteWidth:
			/*{
				SRECT bounds;
				GetBoundingBox(object, &bounds);
				result = (XFDOUBLE) (bounds.xmax - bounds.xmin) / 20;
			}*/
			result=object->GetWidth();
            break;
        case kSpriteHeight:
			/*{
				SRECT bounds;
				GetBoundingBox(object, &bounds);
				result = (XFDOUBLE) (bounds.ymax - bounds.ymin) / 20;
			}*/
			result=object->GetHeight();
            break;
        case kSpriteLastFrameLoaded:
            {
                if (thread == thread->player) {
                    // this is the main movie
					if (thread->player->numFramesComplete < thread->numFrames) {
						result = (XFDOUBLE) thread->player->numFramesComplete;
					} else {
						result = (XFDOUBLE) thread->numFrames;
					}
				} else {
                    // movie sprites are alwayes fully loaded
                    // before they start playing
                    result = (XFDOUBLE) thread->numFrames;
				}
            }
            break;
        case kSpriteRotate:
            /*{
				SFIXED rotX = FixedAtan2(object->xform.mat.b, object->xform.mat.a);
				//if ( rotX < 0 )
				//	rotX += FC(360);
				result = FixedToDouble(rotX);
            }*/
			result=object->GetRotate();
            break;
		case kSpriteHighQuality:
			result = 1;
			//result=nQuality;
			/*if (highQuality) {
				result = bestDither ? 2.0 : 1.0;
			} else {
				result = 0.0;
			}*/
			break;
		case kSpriteFocusRect:
			result = display.useFocusRect ? 1.0 : 0.0;
			break;
		case kSpriteSoundBufferTime:
			result = (XFDOUBLE) nSoundBufferTime;
			break;
		case kSpriteXMouse:
			{
				double x,y;
				GetMousePos(thread->rootObject,x,y);
				result=x;
			}break;
		case kSpriteYMouse:
			{
				double x,y;
				GetMousePos(thread->rootObject,x,y);
				result=y;
			}break;
        default:
            // we should never get here
            FLASHASSERT(0);
    }

exit_gracefully:
    return result;
}

XBOOL XSWFPlayer::GetPropertyStr(ScriptThread *thread, int propType,XXVar&var)
{
	//BOOL makeCopy = XTRUE;
    char *result = NULL;
    SObject *object = 0;

    if (thread == 0) return XFALSE;
    //    goto exit_gracefully;

    object = thread->rootObject;

    switch (propType)
    {
		case kSpriteDropTarget:
			//result = object->dropTarget;
			if(object->dropTarget)
				var=object->dropTarget->pObject;
			else
				var=(XXObject*)NULL;
			break;
		case kSpriteTarget:
			GetTargetPath(object,var);
			//makeCopy = XFALSE;
			break;
		case kSpriteName:
			var = object->name;
			break;
		case kSpriteURL:
			if(object->GetPlayer())
			var = object->GetPlayer()->m_url;//character->player->m_url;
			break;
        default:
			{
				var=GetProperty(thread, propType);
				//result = ConvertDoubleToString(value);
				//makeCopy = XFALSE;
			}
			break;
    }

//exit_gracefully:
    return XTRUE;
}

BOOL IsMachineNameURL(const char *url)
{
	// An URL that contains only a machine name looks like
	//    protocol://machinename

	// Search for :// sequence
	const char *s = url;
	while (*s) {
		if (s[0] == ':' && s[1] == '/' && s[2] == '/')  {
			// Found it!
			// The remaining text must contain no slashes.
			s += 3;
			while (*s) {
				if (*s == '/') {
					return XFALSE;
				}
				s++;
			}
			return XTRUE;
		}
		s++;
	}
	return XFALSE;
}

//
// Multibyte support
//
// WARNING: The multibyte support here will only work for Japanese on the Macintosh.
//

// #ifdef _MAC
// static int gDoubleByteSystem = -1;
// static char gDoubleByteParseTable[256];
// #elif defined(_WINDOWS)
// static UINT gACPCache = 0;
// static BOOL gIsMultiByte = XFALSE;
// #endif

// PlayerIsMultiByte returns XTRUE if the system is multibyte
BOOL PlayerIsMultiByte()
{
// #ifdef _MAC
// 	if (gDoubleByteSystem == -1) {
// 		gDoubleByteSystem = (::GetEnvirons(smDoubleByte) != 0);
// 		if (gDoubleByteSystem) {
// 			// Initialize the parse table
// 			gDoubleByteSystem = FillParseTable(gDoubleByteParseTable, smSystemScript);
// 		}
// 	}
// 	return gDoubleByteSystem;
// #elif defined(_WINDOWS)
// 	UINT acp = GetACP();
// 	if (acp != gACPCache) {
// 		CPINFO cpinfo;
// 		GetCPInfo(acp, &cpinfo);
// 		gACPCache = acp;
// 		gIsMultiByte = cpinfo.MaxCharSize > 0;
// 	}
// 	return gIsMultiByte;
// #else
	// Multibyte support only available for Windows and Mac
	return XFALSE;
// #endif
}

// PlayerIsLeadByte returns XTRUE if ch is a lead byte
BOOL PlayerIsLeadByte(U8 ch)
{
// #ifdef _MAC
// 	if (!PlayerIsMultiByte()) {
// 		return XFALSE;
// 	}
// 	return gDoubleByteParseTable[ch] != 0;
// #elif defined(_WINDOWS)
// 	if (!PlayerIsMultiByte()) {
// 		return XFALSE;
// 	}
// 	return IsDBCSLeadByte(ch);
// #else
	return XFALSE;
// #endif
}

//
// String Length
//

int PlayerStringLength(char *string, BOOL doMultiByte)
{
	if (!doMultiByte || !PlayerIsMultiByte()) {
		return strlen(string);
	} else {
		int count = 0;
		while (*string) {
			count++;
			if (PlayerIsLeadByte(*string)) {
				string++;
				if (!*string) { break; }
			}
			string++;
		}
		return count;
	}
}

//
// Helper function for StringExtract
// This function is for multibyte use only!
// Returns pointer to n'th character in a multibyte string
// n is assumed to be 0 <= n <= StringLength(string)
//

char *PlayerStringIndex(char *string, int index)
{
	while (index--) {
		if (PlayerIsLeadByte(*string)) {
			string++;
			if (!*string) { break; }
		}
		string++;
	}
	return string;
}

//
// String Extract
//

char *PlayerStringExtract(char *string, int index, int count, BOOL doMultiByte)
{
	int len = PlayerStringLength(string, doMultiByte);
	if (index < 0) {
		index = 0;
	} else if (index > len) {
		index = len;
	}
	if (count < 0 || count > len-index) {
		count = len-index;
	}
	if (!doMultiByte || !PlayerIsMultiByte()) {
		return CreateStr(string+index, count);
	} else {
		char *start = PlayerStringIndex(string, index);
		char *end   = PlayerStringIndex(start, count);
		return CreateStr(start, end-start);
	}
}



void XSWFPlayer::InitContext(XSWFCONTEXT &cnt)
{
	cnt.nCallDepth=0;
	cnt.pPlayer=this;
	cnt.pThis=XNULL;
	cnt.pObject=XNULL;
	//cnt.pLoader=XNULL;
	//cnt.pLastLoader=XNULL;
	cnt.baseThread=player->rootObject?player->rootObject->pObject:XNULL;
	cnt.thread=cnt.baseThread;
	cnt.pCaller=XNULL;
//	cnt.urlCount=0;
//	cnt.context=XNULL;
//	cnt.enteringCall=XFALSE;
//	cnt.exitingCall=XFALSE;
	cnt.actionCount=0;
	cnt.actionOverflow=XFALSE;
	cnt.nDepth=0;
	//cnt.pStack=&runstack;
	cnt.regObjs.SetSize(16);
//	cnt.thread=&player;
	cnt.pWith=XNULL;
	if(player->rootObject)
	{
		XXObject*pBase=player->rootObject->pObject;
		cnt.InitContextA(XNULL,pBase,pBase,pBase,pBase,XNULL);
	}
//	cnt.pTarget=XNULL;
//	cnt.contexts.Add(&player);
}

void XSWFPlayer::ReleaseContext(XSWFCONTEXT&cnt)
{
}

void XSWFPlayer::LoadURLs()
{

	m_loadLock.Lock();
	_XLoadInfo*p=pLoader;
	pLoader=pLastLoader=XNULL;
	while(p)
	{
		_XLoadInfo*pNext=p->pNext;
		if(p->pLoader&&(p->nType&kHttpLoadData)!=0)
		{
			p->pLoader->LoadBytes();
			XReleaseINFO(p);
		}
		else if(!p->strURL.IsNull())
		{
			GetURL(p);
		}
		else
		{
			XReleaseINFO(p);
		}
		//display.FreeLoad(p);
		p=pNext;
	}
	m_loadLock.UnLock();
	/*for ( int i = 0; i < cnt.urlCount; i++ ) {
	 if ( cnt.actionURLs[i] ) {
		GetURL(cnt.actionURLs[i],
			   cnt.actionTargets[i],
			   cnt.actionPostData[i],
			   cnt.actionVars[i],
			   cnt.actionObjs[i]);
		//int v=sizeof(LoadInfo);
	}

	FreeStr(cnt.actionURLs[i]);
	FreeStr(cnt.actionTargets[i]);
	FreeStr(cnt.actionPostData[i]);
	FREEOBJECT(cnt.actionObjs[i]);
	}*/
}


//static int iActCount=0;

XBOOL XSWFPlayer::DoActions()
{
	// If we're doing a partial first frame, don't execute actions yet.
	if (!loaded||!player->rootObject/*||!display.raster.bits*/) {
		return XFALSE;
	}

	// If we've overflowed, clear the action stack and do nothing
//	if (actionOverflow) {
//		display.actionList.actionBase = display.actionList.actionTop = 0;
//		return;
//	}

	// Always let the outermost loop process the actions
	if ( actionDepth > 0 )
		return XFALSE;

	XSWFCONTEXT cnt;
	InitContext(cnt);

	actionDepth++;

	//XXVar name("abc.def.tt.bc");

	// Build a list of GetURLs so we can defer loading to avoid reentrancy problems


//	display.actionCallStackTop = 1;
//	display.actionCallStack[0].actionList = &display.actionList;

	//BOOL enteringCall = XFALSE, exitingCall = XFALSE;
	//int actionCount = 0;

//	while (display.actionCallStackTop)
//#if (__CORE_VERSION__>=0x02071000)
//#else
//	bool bMouse=false;
//	this->DoEvent(bMouse);
//#endif
	CallTimer(cnt);
	{

//		cnt.enteringCall = XFALSE;

//		cnt.context = &display.actionCallStack[display.actionCallStackTop-1];
		ActionList *actionList = &display.actionList;//cnt.context->actionList;

		//cnt.contexts.Add(cnt.context->


	while ( actionList->actionBase < actionList->actionTop ) {
		// Do the action
		cnt.pStack->Release();
		int i = actionList->actionBase & actionList->actionMask;
		//if(i==11)
		//	iActCount++;
			//int v=0;
		XActionList::XActionEntry*pEntry=&actionList->entries[i];
		if ( pEntry->actionEntry ) {
			//if(i==11&&iActCount==9)
			//	int v=0;
			SObject*pBase=pEntry->actionBase;
//			if(pBase->character->tag==240)
//				int v=0;
			//XXObject*baseObj=pBase->pObject;
			XXObject*thisObj=pEntry->actionThis;
			if(thisObj==XNULL) thisObj=pBase->pObject;
			if(thisObj)
			{
				XXObject*pClip=pBase->thread->rootObject->pObject;

			//	FLASHASSERT(!(pEntry->actionFlag&XActionList::XA_TOP));
				XBOOL bInit=(pEntry->actionFlag&XActionList::XA_TOP);
				pEntry->actionFlag|=XActionList::XA_TOP;
				ScriptPlayer*pl=pBase->GetPlayer();
				//if(player)
				//		pContext=player->m_pContext;
				//if(pBase->character)
				if(pl)
					XXVar::m_nVersion=pl->version;//pBase->character->player->version;
				else
					XXVar::m_nVersion=player->version;
				switch(pEntry->actionFlag&XActionList::XA_TYPEMASK)
				{
				case XActionList::XA_CODE:
					{
						//if((pEntry->actionFlag>>16)==488)
						//	int v=0;
						XActionScript action(pEntry->actionEntry);
						action.InitContext(pClip,pClip,thisObj,thisObj,XNULL,XNULL);
						action.DoAction(cnt,(pEntry->actionFlag&XActionList::XA_BLOCK)?XNULL:thisObj,
										(pEntry->actionFlag&XActionList::XA_BLOCK),
										bInit);//&local);
					}break;
				case XActionList::XA_FUNCTION:
					{
						XXObject*pObj=(XXObject*)pEntry->actionEntry;
						REFOBJECT(pObj);
						XXVARLIST*list=pEntry->actionArgs;
						XU8 vars[sizeof(XXVar)*2];
						XXVARLIST args(vars,2);
						if(!list) list=&args;
						cnt.InitContextA(XNULL,pClip,pClip,thisObj,thisObj,XNULL);
						pObj->Function(&cnt,*list,thisObj);
						FREEOBJECT(pObj);
					};
				}
			}
		}

		if (cnt.actionOverflow) {

			cnt.actionOverflow=XFALSE;
			cnt.actionCount=0;
			//break;
		}
		actionList->FreeOne(i,XTRUE);
		actionList->actionBase++;
		}

	}


	//CallHandlers(cnt);

	actionDepth--;

	if (cnt.actionOverflow) {
		display.actionList.actionBase = display.actionList.actionTop = 0;
		//return;
	}
	// Load the URLs now
	ReleaseContext(cnt);
//	display.FreeObjs();
	FreeCalls();
	XXObject::FreeObjs();
	return XTRUE;
}

SObject* XSWFPlayer::GetRootObject()
{
    return &display.root;
}


inline void XSWFPlayer::AS3DoCommand()
{
	if(!this->m_bUpdateCommand) return;

	for ( ScriptThread* t = display.threads; t; t = t->next )
	{
		t->doCommand(false);
		// Advance a single frame
		//		t->DoFrame();
	}
	m_bUpdateCommand=false;
}


void XSWFPlayer::DoMoveMode()
{
	MATRIX&mat=display.mControlMat;//.camera.mat;
	int nStep=MOVE_OFFSET/5;
	if(m_nMode!=MV_RET)
	{
		int dx=m_nDestX-mat.tx;
		int dy=m_nDestY-mat.ty;
		int ddx=dx>=0?dx:-dx;
		int ddy=dy>=0?dy:-dy;
		if(ddx<nStep&&ddy<nStep)
			m_nMode=MV_RET;
		else
		{
			if(ddx>nStep)
				ddx=dx*30/ddx;
			else
				ddx=dx;
			if(ddy>nStep)
				ddy=dy*30/ddy;
			else
				ddy=dy;
			mat.tx+=ddx;
			mat.ty+=ddy;
		}

	}
	else
	{
		int dx=m_nDestX-mat.tx;
		int dy=m_nDestY-mat.ty;
		//if(dx==0&&dy==0) return;
		int ddx=dx>=0?dx:-dx;
		int ddy=dy>=0?dy:-dy;
		if(ddx<nStep&&ddy<nStep)
		{
			mat.tx=m_nDestX;
			mat.ty=m_nDestY;
			m_nMode=MV_NONE;
		}
		else
		{
			if(ddx>nStep)
				ddx=dx*30/ddx;
			else
				ddx=dx;
			if(ddy>nStep)
				ddy=dy*30/ddy;
			else
				ddy=dy;
			mat.tx+=ddx;
			mat.ty+=ddy;
		}

	}
//#ifdef _LOWSPEED
//	this->m_bUpdateDrag=true;
//	UpdateDragDib();
//#else
	display.UpdateMat();
	//display.ModifyCamera(false);
//#endif
}

void XSWFPlayer::SetMoveDirect()
{
	int cx=m_winSize.cx/2;
	int cy=m_winSize.cy/2;
	int dx=scrollPt.x-cx;
	int dy=scrollPt.y-cy;
	int ddx=dx>=0?dx:-dx;
	int ddy=dy>=0?dy:-dy;
	if(ddx>ddy*4) dy=0;
	else if(ddy>ddx*4) dx=0;
	if(ddx>cx/4||ddy>cy/4)
	{
		if(dx>0)
		{
			if(dy>0)
				SetMoveMode(MV_DOWNRIGHT);
			else if(dy<0)
				SetMoveMode(MV_TOPRIGHT);
			else
				SetMoveMode(MV_RIGHT);
		}
		else if(dx<0)
		{
			if(dy>0)
				SetMoveMode(MV_DOWNLEFT);
			else if(dy<0)
				SetMoveMode(MV_TOPLEFT);
			else
				SetMoveMode(MV_LEFT);
		}
		else
		{
			if(dy>0)
				SetMoveMode(MV_DOWN);
			else
				SetMoveMode(MV_TOP);
		}
	}
}

void XSWFPlayer::Interrupt()
{
	if(m_pAVM2)
		m_pAVM2->Interrupt(true);
}

bool XSWFPlayer::InterruptIfTimeout()
{
	//return false;
#ifdef _DEBUG
	return false;
#endif
	if(!m_pAVM2) return false;
	XU32 t=XTime::GetTimeMSec();
//#ifdef _SYMBIAN
//	return false;
//	if(t<this->m_nRunTime+30000) return false;
//#elif defined(__APPLE__)
//	if(t<this->m_nRunTime+60000) return false;	// Modified by handong 2011.10.28(iPad1 can't launch YongHengTaFang)
//#elif defined( __ANDROID__ )
//	if(t<this->m_nRunTime+60000) return false;
//#else
	if(t<this->m_nRunTime+60000) return false;
//#endif
	m_pAVM2->Interrupt(false);
	return true;
}

XINT XSWFPlayer::DoPlay(XRect&rect,BOOL wait)
{
	_ANAGRAPH("DoPlay",20);

	_ANALYENTER("DoPlay");
	//if(m_bDataProc)
	//{
	//	int nTotals=0,i,j,m;
	//	for(i=0;i<m_newDatas.GetSize();i++)
	//		nTotals+=m_newDatas[i]->m_nShapes;
	//	DataStart(nTotals);
	//	int nNow = 0;
	//	//XU32 nMaxID = 0;
	//	//XU32 nMaxTime = 0;
	//	MATRIX mat;
	//	//display.GetCameraMatrix(mat);
	//	::MatrixIdentity(&mat);
	//	for(i=0;i<m_newDatas.GetSize();i++)
	//	{
	//		ScriptPlayer*p=m_newDatas[i];
	//		//SCharacter* ch = charIndex[tag & charIndexMask];
	//		for(j=0;j<ScriptPlayer::charIndexSize;j++)
	//		{
	//			SCharacter* ch = p->charIndex[j];
	//			while(ch)
	//			{
	//				switch(ch->type)
	//				{
	//				case shapeChar:
	//					if(ch->record.pFirst==NULL)
	//					{
	//						/*SCharacterParser parser(p, ch->data, 0, &ch->record);
	//						if((ch->shape.flags)&0x04)
	//							parser.useWinding=XTRUE;

	//						//if(character->tag==249)
	//						//	int v=0;
	//						parser.getAlpha = 
	//							ch->tagCode >= stagDefineShape3;
	//						parser.lineStyle2 =
	//							ch->tagCode >= stagDefineShape4;
	//						//XU32 t=XTime::GetTickCount();
	//						parser.BuildEdges(XTRUE);*/
	//						if(ch->record.m_nSampleType!=2&&
	//						   !ch->record.m_pImage)
	//						   _XSObject::BuildRasterObj(ch,mat,true);

	//						nNow++;
	//						DataSet(nNow);
	//						//ch->record.ReleaseTexture();
	//					}
	//					break;
	//				//case fontChar:
	//				//	if(ch->font.nGlyphs>0)
	//				//	{
	//				//		for(m=0;m<ch->font.nGlyphs;m++)
	//				//		{
	//				//			//XU32 t=XTime::GetTickCount();
	//				//			_XSObject::BuildFontChar(ch,m,NULL,0,0);
	//				//			nNow++;
	//				//			DataSet(nNow);
	//				//		}
	//				//	}
	//				//	 break;
	//				}
	//				ch = ch->next;
	//			}
	//		}
	//		//DataSet(nNow);
	//	}
	//	m_bDataProc = false;
	//	m_newDatas.RemoveAll();
	//	DataEnd();
	//}
    if(!bRotaionEnd)
        CheckOrientation();
	if(m_nDelayShow)
	{
		m_nDelayShow--;
		if(!m_nDelayShow)
			display.root.Modify();
	}
	m_nRunTime=XTime::GetTimeMSec();

	bool bMouse=false;

	nCacheCount++;

	{
		_ANALYENTER("Free Timeout Images");
		/*if(nCacheCount==80)
		{
			_SHAPEIMAGES**pImages=&m_pShapeCaches;
			while(*pImages)
			{
				_SHAPEIMAGES*p=*pImages;
				_XSObject* obj=(_XSObject*)p->ch;
				if(!obj->pCacheBmp||!display.root.IsParentOf(obj))
				{
					*pImages=p->next;
					obj->FreeCacheBmp();
					XXVar::_FreeData(p);
				}
				else pImages=&p->next;
			}
		}*/

		if(nCacheCount==150)
		{
			_SHAPEIMAGES**pImages=&m_pShapeImages;
			while(*pImages)
			{
				_SHAPEIMAGES*p=*pImages;
				//_SHAPEIMAGES**next=&pImages->next;
				_ShapeImage*pImage=p->ch->record.m_pImage;
				if(!pImage||!pImage->id)
				{
					*pImages=p->next;
					XXVar::_FreeData(p);
				}
				else if(!RasterGL::IsLocked(pImage->id)&&m_nRunTime-RasterGL::GetLastTime(pImage->id)>2000)//if(pImage->useCount<=0&&(this->m_nRunTime-pImage->nTimer)>=2000)
				{
					p->ch->record.ReleaseAll(false);//ReleaseImage();
					*pImages=p->next;
					XXVar::_FreeData(p);
				}
				else pImages=&p->next;
				//pImages=next;
			}
		}
		if(nCacheCount==100)
		{
			//int nMaxTime=this->m_pDomFilter?m_pDomFilter->nCacheImage:8000;
			SBitmapCore*bitmap=m_bitmaps;
			while(bitmap)
			{
				SBitmapCore*next=bitmap->m_pNext;
				if(!bitmap->bitmapID)
					bitmap->PIFree();
				else if(!RasterGL::IsLocked(bitmap->bitmapID))//!bitmap->bitmapID->nLockCount)
				{
					_TEXTUREID*id = bitmap->bitmapID;
					XU32 dt=m_nRunTime-id->nLastTime;
					if(dt>MAXCACHET)
					{
						//bitmap->m_nLockTime=m_nRunTime;
						bitmap->PIFree();
					}
				}
				bitmap=next;
			}
		}
		if(nCacheCount<100&&buildInFont.font.shapeData)
		{
			int i;
			GLTranglesOrLines**pData=buildInFont.font.shapeData;
			int nCount=buildInFont.font.nGlyphs/100+1;
			int nBase=nCount*nCacheCount;
			int nMax=nBase+nCount;
			if(nMax>buildInFont.font.nGlyphs)
				nMax=buildInFont.font.nGlyphs;
			for(i=nBase;i<nMax;i++)
			{
				if(!pData[i]) continue;
				GLTranglesOrLines** pList = &pData[i];
				while(*pList)
				{
					GLTranglesOrLines*pTL = *pList;
					GLTranglesOrLines**pNext = &pTL->pNext;
					_TEXTUREID*tID = pTL->pChar->id;
					if(!RasterGL::IsLocked(tID)&&m_nRunTime-RasterGL::GetLastTime(tID)>MAXCACHET)//pTL->pChar->nCount<=0&&this->m_nRunTime-pTL->pChar->nLastT>MAXCACHET)
					{
						*pList = pTL->pNext;
						pTL->pNext = NULL;
						BwShapeRecord::ReleaseShapeList(pTL);
					}
					else pList = pNext;
				}
			}
		}
	}



	GCENTER;

	if(m_pAVM2&&m_pDomFilter&&m_pDomFilter->bAdvanceGC)
	{
		XU32 tMax = MAXCACHET*3;
		if(this->m_pDomFilter)
			tMax = m_pDomFilter->nCacheBitmap;
		m_pAVM2->GetToplevel()->ClearDeadImage(nCacheCount,m_nRunTime,tMax);
	}


	if(bError)
	{
		return PLAY_WAIT;
	}

	if(m_nLockOpt)
		m_nLockOpt--;

	if (!loaded) {
		//_ANALYLEAVE();
		return PLAY_WAIT;
	}



	m_pNetWindow->CallBack();


	if(this->m_pAVM2)
	{
		m_pAVM2->GetToplevel()->CallLaterObjects();
	}

    // Note that DoActions() can cause this to get called again (when the browser is loading an URL) which is not properly supported
	if ( actionDepth )
	{
		//_ANALYLEAVE();
		return PLAY_WAITSHORT;
	}


	

	bool bNoHit=DoEvent(bMouse);



	/*if(m_nMode!=MV_NONE)
	{
		DoMoveMode();
		display.Update();

		CheckUpdate();
		return PLAY_OK;
	}

	if(downling)
	{
		XU32 t=m_nRunTime-scrollTime;
		if(t>400)
		{
			//scrolling = XFALSE;
			//downling = XFALSE;
			SetMoveDirect();
			return PLAY_OK;
		}
	}*/



	/*if(this->scrolling)
	{
		display.Update();

		CheckUpdate();

		return PLAY_OK;
	}*/



	this->AS3ClearCommand();
//	XLock lock(&m_dataLock);
//	if(!lock.Lock(XWAIT_TIME))
//	{
		//_ANALYLEAVE();
//		return PLAY_WAITSHORT;
//	}

	S32 time = XTime::GetTimeMSec();
#ifdef __STOPRUN__
	if(!_bStopRun) //return PLAY_OK;
	{
		_ANALYENTER("Process MovieClips and Sounds");
#endif
	BlinkCursor(time);

	DoActions();

	int nFramesToAdvance = 1;

#ifdef SOUND
	ScriptThread* syncThread = 0;
	m_pNextDoThread = NULL;
	{
		_ANALYENTER("Sounds");
		ScriptThread* t = display.threads;
		while(t)
		{
			m_pNextDoThread=t->next;
			XSoundChannel* c = t->sndChannel;
			if(!c)
			{
				t=m_pNextDoThread;
				continue;
			}
			if ( c->IsPlaying() && !c->streamStalled ) {
				// Use the streaming sound as the clock...
				nFramesToAdvance = c->ElapsedFrames() - t->GetFrame();
				syncThread = t;
				if ( nFramesToAdvance <= 0 )
				{
//					lock.UnLock();
					//_ANALYLEAVE();
					//GCLeave();
					this->AS3DoCommand();
					return bNoHit?PLAY_HITNONE:PLAY_WAITSHORT;
				}
				goto Advance;
			}
			t=m_pNextDoThread;
		}
	}
#endif

	// Is it time to show the next frame yet???

  Advance:
	if ( wait && time < nextFrameTime )
	{
//		lock.UnLock();
//		_ANALYLEAVE();
//#if (__CORE_VERSION__>=0x02071000)
		if(m_bNeedEnter&&m_pAVM2)
		{
			m_bNeedEnter=XFALSE;
			_ANALYENTER("Enter Frame");
			//m_bEnterFrame=XTRUE;
			//if(m_iEnterCount<2)//wait 2 circle then start send ENTER_FRAME event
			//	m_iEnterCount++;
			//else
			this->AS3PostEvent(XAS3_EVENT_enterFrame,XAS3EVENT_EVENT,false);
//#if (__CORE_VERSION__>=0x02076000)
			this->AS3PostEvent(XAS3_EVENT_exitFrame,XAS3EVENT_EVENT,false);
//#endif
			//m_bEnterFrame=XFALSE;
		}
//#endif
		LoadURLs();
		if(RasterGL::sharedRasterGL()->mbFresh)
            if(bRotaionEnd&&RasterGL::sharedRasterGL()->mbInit)
                display.Update();
		//GCLeave();
		return bNoHit?PLAY_HITNONE:PLAY_WAITSHORT;
	}
	// Always try to maintain the frame delay and slow down if we can't keep up
	nextFrameTime = time + player->frameDelay;


	{
		_ANALYENTER("Do Threads");
		while ( nFramesToAdvance-- ) {
			//display.root.AS3OnenterFrame();
			/*void*pStage=display.root.pASObject;
			if(pStage)
			{
				m_pAVM2->OnEvent(XAS3_EVENT_enterFrame,false,pStage,player->m_pContext);
			}*/
			//if(m_pAVM2)

			// Process the timeline for all threads
			ScriptThread* t = display.threads;
			//for ( ScriptThread* t = display.threads; t; t = t->next )
			m_bDoFrame=XTRUE;
			while(t)
			{
				m_pNextDoThread = t->next;
	#ifdef SOUND
				if ( t->sndChannel && t != syncThread && t->sndChannel->IsPlaying() )
				{
					if ( t->sndChannel->ElapsedFrames() >= t->GetFrame() || t->sndAtEnd )
					{
						// Don't get ahead of a sound channel
						t->DoFrame();
					}
				}
				else
	#endif
				{
					// Advance a single frame
					t->DoFrame();
				}
				t=m_pNextDoThread;
			}		

			m_bDoFrame=XFALSE;

			//[Note X]
			t = display.threads;		
			while(t)
			{
				m_pNextDoThread = t->next;
				t->doCommand(false);
				t=m_pNextDoThread;
			}//[Note X End]
		}
		m_pNextDoThread = NULL;

		// Handle the action list
		DoActions();

		if(m_pAVM2)
		{
			if(m_bResetMode)
			{
				if(display.root.pASObject)
				{
					//m_pAVM2->OnEvent(XAS3_EVENT_change,true,pObj->pASObject,pContext);

					m_pAVM2->OnEvent(XAS3_EVENT_resize,false,display.root.pASObject,player->m_pCodeContext);
				}
				m_bResetMode=false;
			}
			_ANALYENTER("Enter Frame");
			//m_bEnterFrame=XTRUE;
			//if(m_iEnterCount<2)//wait 2 circle then start send ENTER_FRAME event
			//	m_iEnterCount++;
			//else
			if(!pLoader)
			{
				this->AS3PostEvent(XAS3_EVENT_enterFrame,XAS3EVENT_EVENT,false);

				this->AS3PostEvent(XAS3_EVENT_exitFrame,XAS3EVENT_EVENT,false);
			}

			//m_bEnterFrame=XFALSE;
		}

	}

	if(pLoader)
	{
		LoadURLs();
		//return bNoHit?PLAY_HITNONE:PLAY_WAITSHORT;
	}
// NOTE: commented out for windows
// #if defined(DRAGBUTTONS) && defined(_WINDOWS) && !defined(GENERATOR)
// 	// When a drag is started on Windows, the object doesn't get moved to
// 	// the current mouse position until the first WM_MOUSEMOVE.
// 	// This fixes that.
// 	if (dragStarted) {
// 		UpdateDragObject(&mousePt);
// 		dragStarted = XFALSE;
// 	}
// #endif
#ifdef __STOPRUN__
	}
#endif
  #ifdef EDITTEXT
	if ( !display.iBeam ) {
  #endif
		// Update the buttons in case a button has moved under the mouse
		//if ( !scrolling && mouseState >= 0 )
		//	DoButton(&mousePt, ( mouseState != 0), XFALSE);
  #ifdef EDITTEXT
	}
  #endif

	//UpdateScreen();

	// See if the quality setting is appropriate for this movie


	if ( autoQuality ) {

		if ( nTotalCount>20 )
		{
			if(nTotalCost>(nTotalNeed<<2)/3)
			{
				if(nQuality)
				{
					nQuality--;
					//SetCamera(updateLazy);
				}
			}
			else if(nTotalCost<(nTotalNeed>>1))//if(nQuality<XConfig::HIGH&&nTooFast*3>nTotal)
			{
				if(nQuality<XConfig::HIGH)
				{
					nQuality++;
					//SetCamera(updateLazy);
				}
			}
			nTotalCount=0;
			nTotalCost=0;
			nTotalNeed=0;
		}
		//lastQualitySwitch++;
		/*if(!m_bUpdateRedraw)
		{
			display.Update();
			m_bUpdateRedraw=XTRUE;
			CheckUpdate();

		}
		else
		{
			CheckUpdate();

		}*/
	}
	else
	{
		if ( nTotalCount>20 )
		{
			if(nTotalCost>(nTotalNeed<<2)/3)
			{
				//lastQualitySwitch=0;
				//nTooFast=0;
//#ifdef __APPLE__
//                if(nSkipCount<1)
//					nSkipCount++;
//#else
				if(nSkipCount<2)
					nSkipCount++;
//#endif
				//SetCamera(updateLazy);
			}
			else if(nTotalCost<nTotalNeed>>1)//if(nTooFast*2>=lastQualitySwitch)
			{
				//lastQualitySwitch=0;
				if(nSkipCount>0)
					nSkipCount--;
				GCLeave();
				//SetCamera(updateLazy);
			}
			nTotalCount=0;
			nTotalCost=0;
			nTotalNeed=0;
		}
//#if (__CORE_VERSION__>=0x02077000)
//		if(nSkipsNow<nSkipCount&&!display.m_bInvalidate)
//#else
		if(nSkipsNow<nSkipCount)
//#endif
			nSkipsNow++;
		else //if(!m_bUpdateRedraw)
		{
			nSkipsNow=0;
            if(bRotaionEnd&&RasterGL::sharedRasterGL()->mbInit)
                display.Update();
			//m_bUpdateRedraw=XTRUE;
			//CheckUpdate();
			//rect|=m_updateRect;
			//m_updateRect.SetRectEmpty();
		}
		/*else
		{
			CheckUpdate();
			if(!m_updateRect.IsRectEmpty())
			{
				rect|=m_updateRect;
				m_updateRect.SetRectEmpty();
			}
			else m_bUpdateRedraw=XFALSE;
		}*/
		//lastQualitySwitch++;
	}


	XXObject::FreeObjs();

	S32 elapsed = XTime::GetTimeMSec() - time;

	nTotalCount++;
	nTotalCost+=elapsed;
	nTotalNeed+=player->frameDelay;

	/*if ( elapsed > player->frameDelay || nFramesToAdvance > 1 )
		nTooSlow++;
	else if ( elapsed < player->frameDelay/4 )
		nTooFast++;
	nTotal++;*/

//	display.FreeObjs();
//	lock.UnLock();
	//_ANALYLEAVE();
	//RectSetEmpty(&display.screenDirtyRgn);
	//rect=(XRECT&)display.screenDirtyRgn;
	return bNoHit?PLAY_HITNONE:PLAY_OK;
}


#ifdef EDITTEXT
void XSWFPlayer::GetEditMenuState( EditMenuStatus* editMenuStatus )
{
	int flags = 0;
	memset( editMenuStatus, 0, sizeof( EditMenuStatus ) );

	if (focus)
	{
		focus->editText->GetEnabledCommands(flags);
	}

	editMenuStatus->cut			= flags & EditText::kCutEnabled			? XTRUE : XFALSE;
	editMenuStatus->copy		= flags & EditText::kCopyEnabled		? XTRUE : XFALSE;
	editMenuStatus->paste		= flags & EditText::kPasteEnabled		? XTRUE : XFALSE;
	editMenuStatus->clear		= flags & EditText::kClearEnabled		? XTRUE : XFALSE;
	editMenuStatus->selectAll	= flags & EditText::kSelectAllEnabled	? XTRUE : XFALSE;
}
#endif


//void XSWFPlayer::Control100()
//{
//	Zoom100();
//	//SetMenuState();
//}

//void XSWFPlayer::ControlViewAll()
//{
//	Home();
//	//SetMenuState();
//}

//void XSWFPlayer::ControlZoomIn()
//{
//	ZoomF(fixed_1/2);
//	//SetMenuState();
//}

//void XSWFPlayer::ControlZoomContext( int x, int y )
//{
//	SPOINT pt;
//
//	pt.x = x;
//	pt.y = y;
//
//	ZoomF( fixed_1/2, &pt );
//	//SetMenuState();
//}
//
//void XSWFPlayer::ControlZoomOut()
//{
//	ZoomF(fixed_1*2);
//	//SetMenuState();
//}
//
void XSWFPlayer::ControlHighQuality()
{
	////highQuality = !highQuality;
	if(nQuality<XConfig::HIGH)
		nQuality++;
	else
		nQuality=XConfig::HIGH;
	autoQuality = XFALSE;
	SetCamera(updateLazy);
	//SetMenuState();
}
//
//void XSWFPlayer::Pan(long x, long y, int mode)
//{
//	if ( RectIsEmpty(&zoomRect) ) return;
//
//	// Adjust the scroll distance based on the mode
//	switch ( mode ) {
//		case panPixels: // absolute pixels
//			break;
//		case panPercent:{// percent of the window size
//			SRECT viewRect;
//			ClientRect(viewRect);
//			x = (RectWidth(&viewRect)*x)/100;
//			y = (RectHeight(&viewRect)*y)/100;
//		} break;
//	}
//
//	{// scroll x number of pixels
//		MATRIX m, invMat;
//		/*if ( display.antialias_b ) {
//			switch(display.antiShift)
//			{
//			case 2:
//				MatrixScale(fixed_1/4, fixed_1/4, &m);
//				break;
//			case 1:
//				MatrixScale(fixed_1/2, fixed_1/2, &m);
//				break;
//			case 3:
//				MatrixScale(fixed_1/8, fixed_1/8, &m);
//				break;
//			}
//			//MatrixScale(fixed_1/4, fixed_1/4, &m);
//			MatrixConcat(&display.camera.mat, &m ,&m);
//		} else */
//		{
//			m = display.camera.mat;
//		}
//		MatrixInvert(&m, &invMat);
//
//		SRECT viewRect, docRect;
//		ClientRect(viewRect);
//		MatrixTransformRect(&m, &player->frame, &docRect);
//
//		// Calculate the scroll limits
//		SRECT limit;
//		limit.xmin = docRect.xmin - viewRect.xmin;
//		if ( limit.xmin > 0 ) limit.xmin = 0;
//		limit.xmax = docRect.xmax - viewRect.xmax;
//		if ( limit.xmax < 0 ) limit.xmax = 0;
//		limit.ymin = docRect.ymin - viewRect.ymin;
//		if ( limit.ymin > 0 ) limit.ymin = 0;
//		limit.ymax = docRect.ymax - viewRect.ymax;
//		if ( limit.ymax < 0 ) limit.ymax = 0;
//
//		SPOINT delta;
//		delta.x = x;
//		delta.y = y;
//		if ( delta.x < limit.xmin )
//			delta.x = limit.xmin;
//		else if ( delta.x > limit.xmax )
//			delta.x = limit.xmax;
//
//		if ( delta.y < limit.ymin )
//			delta.y = limit.ymin;
//		else if ( delta.y > limit.ymax )
//			delta.y = limit.ymax;
//
//		// Do the scroll
//		RectOffset(delta.x, delta.y, &viewRect);
//		MatrixTransformRect(&invMat, &viewRect, &zoomRect);
//		SetCamera(updateLazy);
//	}
//}
//
//void XSWFPlayer::Zoom(int factor)
//{
//	if ( factor == 0 ) {
//		Home();
//	} else {
//		ZoomF(FC(factor)/100);
//	}
//}
//
//void XSWFPlayer::SetZoomRect(long left, long top, long right, long bottom)
//{
//	if ( left >= right || top >= bottom ) {
//		Home();
//	} else {
//		zoomRect.xmin = left;
//		zoomRect.ymin = top;
//		zoomRect.xmax = right;
//		zoomRect.ymax = bottom;
//		SetCamera(XSWFPlayer::updateLazy);
//	}
//}

//////////////////////////////////////////////////////////////////////////
// Internet download routines for the standalone Flash Player

// void XSWFPlayer::LoadFile( URLWrapper* url,
// 						int layer,
// 						char *dstSprite,
// 						char *postData,
// 						BOOL loadVars
// 					  )
// {
//     ScriptThread    *thread = 0;
//
//     // see if we are loading a sprite instead
//     // of a movie layer. If so find the target
//     // thead that is going to get the new script
//     if (dstSprite)
//     {
// 		if (!FindTarget(player->rootObject, dstSprite)) {
// 			// if can't find the target thread
// 			// just return.
//             return;
// 		}
//     }
//
// 	// Put the data into the script
// 	DownloadURL( url, postData, dstSprite, layer, loadVars );
// }
//

// String Utils
//

int LayerNum(char* s)
{
	if(!s) return -1;
	const char* layer = SkipPrefix(s, "_flash");  // this is obsolete but we include it for compatibility with a beta version
	if ( !layer )
		layer = SkipPrefix(s, "_level");	// allow this also since it is easier for users
	if ( layer )
		return ToInt(layer);
	else
		return -1;
}


//
// A Helper Object that combines variables into a form suitable for URLs or a POST operation
//

void URLBuilder::AppendChar(char ch)
{
	if ( len >= size-1 ) {
		// enlarge the buffer
		size += 256;
		char* x = new char[size];
		if ( !x ) return;
		if ( buf )
			memcpy(x, buf, len+1);
		FreeStr(buf);
		buf = x;
	}

	buf[len++] = ch;
	buf[len] = 0;	// keep the string null terminated
}

void URLBuilder::AppendInt(int i)
{
	if (!i) {
		AppendChar('0');
	} else {
		int n = 0;
		char buf[10];
		while (i > 0) {
			buf[n++] = (i % 10)+'0';
			i /= 10;
		}
		char* c = buf+n;
		while ( n-- ) {
			AppendChar(*--c);
		}
	}
}

int HexChar(int v)
{
	v &= 0xF;
	return v < 10 ? v + '0' : v + 'A' - 10;
}

void URLBuilder::EncodeString(char* ch)
{
	while ( XTRUE ) {
		char c = *ch;
		if ( !c ) break;
		if ( (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ) {
			AppendChar(c);		// leave alpha numeric characters alone
		} else if ( c == ' ' ) {
			AppendChar('+');	// convert a space to a "+"
		} else {
			// Encode all other characters
			AppendChar('%');
			AppendChar((char)HexChar(c>>4));
			AppendChar((char)HexChar(c));
		}

		ch++;
	}
}

void URLBuilder::AppendString(char* ch)
{
	while ( XTRUE ) {
		char c = *ch;
		if ( !c ) break;
		AppendChar(c);
		ch++;
	}
}

void URLBuilder::EncodeVariables(XSWFCONTEXT&cnt,XXObject*pObj)//ScriptVariable* first)
{
	//ScriptVariable* var = first;
	XXTOKENS*pTokens=pObj->GetTokens();
	if(!pTokens) return;
	//while ( var )
	for(int i=0;i<(int)pTokens->GetSize();i++)
	{
		XXTOKEN*t=(*pTokens)[i];
		if(!t->member.IsObject())
		//if ( var->name && var->value )
		{
			if ( i )//var != first )
				AppendChar('&');
			EncodeString(t->strKey.strTxt);//var->name);
			AppendChar('=');
			XXVar var;
			pObj->GetValueOf(cnt,t,var,XNULL);
			var.ToString(XFALSE);
			EncodeString(var.strTxt);
			//EncodeString(var->value);
		}

		//var = var->next;
	}
}


//
// Extract variable values from a URL
//

int ParseHexChar(char c)
{
	if ( '0' <= c && c <= '9' )
		return c-'0';
	if ( 'A' <= c && c <= 'F' )
		return c-'A'+10;
	if ( 'a' <= c && c <= 'f' )
		return c-'a'+10;
	return -1;
}

char* ExtractCharacter(char* ch, URLBuilder* ub)
{
	FLASHASSERT(ch && *ch);
	char c = *ch;
	if ( c == '%' ) {
		// Parse out a hex code
		ch++;	// skip the %

		int v = ParseHexChar(*ch++);	// get first digit
		if ( v < 0 ) return ch;
		int v2 = ParseHexChar(*ch++);	// get second digit
		if ( v2 < 0 ) return ch;

		ub->AppendChar((v << 4) | v2);

	} else if ( c == '+' ) {
		// Map a '+' to a ' '
		ub->AppendChar(' ');
		ch++;

	} else {
		// Handle raw characters
		ub->AppendChar(c);
		ch++;
	}
	return ch;
}

void DecodeQueryString(XSWFCONTEXT&cnt,XXObject* thread, char *str)
{
	char *ch = str;
	if(!thread) return;
	// Iterate through the variable pairs...
	while ( XTRUE ) {
		URLBuilder name;
		URLBuilder value;

		// Pull off the name
		while ( XTRUE ) {
			char c = *ch;
			if ( !c ) break;
			if ( c == '=' ) {
				ch++;
				break;
			}
			if ( c == '&' ) {
				break;
			}
			ch = ExtractCharacter(ch, &name);
		}

		// Pull off the value
		while ( XTRUE ) {
			char c = *ch;
			if ( !c ) break;
			if ( c == '&' ) {
				ch++;
				break;
			}
			ch = ExtractCharacter(ch, &value);
		}

		if ( name.buf )
		{
			//XXObject*pObj=thread->rootObject->pObject;
			//if(pObj)
			XXVar var,txt(_strEmpty);
			if(value.buf)
				txt.SetString(value.buf,value.len);
			var.SetString(name.buf,name.len);
				thread->SetMember(cnt,var,txt,XNULL);
			//thread->SetVariable(cnt,name.buf, XXVar(value.buf ? value.buf : ""),XNULL);
		}

		if ( *ch == 0 ) break;
	}
}

void ExtractVariables(XSWFCONTEXT&cnt,XXObject* thread, char* url)
{
	if(!thread) return;
	char* ch = url;

	// Find the '?' delimiter
	while ( XTRUE ) {
		if ( !*ch ) return;	// there are no variables...
		if ( *ch++ == '?' ) break;
	}

	DecodeQueryString(cnt,thread, ch);
}


#ifdef EDITTEXT

void XSWFPlayer::BlinkCursor(S32 time)
{
	if (!focus||!focus->IsEditText()) {
		return;
	}

	if(!focus->editText->IsEditAble())
		return;

	if(time-cursorTime<400) return;

	cursorTime=time;

	cursorBlink = !cursorBlink;

	focus->Modify();
}

BOOL XSWFPlayer::TabDirect(int key)
{
//	XLock lock(&m_dataLock);
//	if(!lock.Lock(XWAIT_TIME)) return XFALSE;

	if(!currentButton.button||RectIsEmpty(&display.tabFocusRect))
	{
		//currentButton=&m_lastButton;
		//if(!currentButton.button)
		{
			switch(key)
			{
				case _XMOUSEMODE::A:
				case _XMOUSEMODE::B:

					 break;
				case _XMOUSEMODE::LEFT_KEY:
					 TabKeyDown(Keyboard::ID_KEY_ENTER,0);
					 /*if(currentButton.button)
					 {
						ButtonFocusSet(currentButton.button);
					 }*/
					 break;
			   case _XMOUSEMODE::UP:
			   case _XMOUSEMODE::LEFT:
				    ButtonTabPrevious();
					break;
			   case _XMOUSEMODE::DOWN:
			   case _XMOUSEMODE::RIGHT:
			   		ButtonTabNext();
					break;
			}
//			lock.UnLock();
			return XTRUE;
			//ButtonTabNext();
		}
		//else

	}
	//ButtonFocusSet(currentButton.button);

	XBOOL bOK=XFALSE;
	switch(key)
	{
	case _XMOUSEMODE::A:
	case _XMOUSEMODE::B:
	case _XMOUSEMODE::LEFT_KEY:
		 bOK=TabKeyDown(Keyboard::ID_KEY_ENTER,0);
		 if(currentButton.button)
		 {
			ButtonFocusSet(currentButton.button);
		 }
		 break;

	case _XMOUSEMODE::DOWN:
		 bOK=TabKeyDown(Keyboard::ID_KEY_DOWN,0);
		 break;
	case _XMOUSEMODE::LEFT:
		 bOK=TabKeyDown(Keyboard::ID_KEY_LEFT,0);
		 break;
	case _XMOUSEMODE::RIGHT:
		 bOK=TabKeyDown(Keyboard::ID_KEY_RIGHT,0);
		 break;
	case _XMOUSEMODE::RIGHT_KEY:
		 bOK=TabKeyDown(Keyboard::ID_KEY_ESCAPE,0);
		 break;
	case _XMOUSEMODE::UP:
		 bOK=TabKeyDown(Keyboard::ID_KEY_UP,0);
		 break;
	}
//	lock.UnLock();

	return bOK;
}

BOOL XSWFPlayer::TabKeyDown(int key, int modifiers)
{
	SObject*old=this->currentButton.button;
	if (key == Keyboard::ID_KEY_TAB) {
		// If there is no current button, we must reset the
		// current button to the last button visited so that
		// navigation can continue where it left off.
		if (!currentButton.button) {
			currentButton = &m_lastButton;
		}
		// Save current button
		m_lastButton = &currentButton;
		// Do the tab
		if (modifiers & Keyboard::ID_KEY_SHIFT) {
			ButtonTabPrevious();
		} else {
			ButtonTabNext();
		}
		// Notify the container that we handled the navigation
		// only if we found a valid button to navigate to.
		AS3SetKeyFocus(old,currentButton.button,key);
		return currentButton.button != 0;
	}

	// The arrow keys and Enter are only handled if there
	// is already a current button AND if there is no edit text
	// focused.
	if (!currentButton.button || focus != 0 || (RectIsEmpty(&display.tabFocusRect)&&key!=Keyboard::ID_KEY_ENTER)) {
		return XFALSE;
	}

	// Save current button
	m_lastButton = &currentButton;

	switch (key) {
    case Keyboard::ID_KEY_RIGHT:
        ButtonGoDirection(RIGHT);
        break;
    case Keyboard::ID_KEY_LEFT:
        ButtonGoDirection(LEFT);
        break;
    case Keyboard::ID_KEY_UP:
        ButtonGoDirection(UP);
        break;
    case Keyboard::ID_KEY_DOWN:
        ButtonGoDirection(DOWN);
        break;
    case Keyboard::ID_KEY_ENTER:
	case ' ':
        // process an action here if required
		ButtonEnter();

		// Check if the button has changed/moved
		SButton* button = &currentButton;
		SButton  newButton;
		ButtonFind(button, &newButton);

		if ( button->button != newButton.button ) {
			button = &newButton;
			if (!newButton.button) {
				ButtonTabNext();
			}
		}
		// Return that we handled this
		AS3SetKeyFocus(old,currentButton.button,key);
		return XTRUE;
	}

	// Notify the container that we handled the navigation
	// only if we found a valid button to navigate to.
	AS3SetKeyFocus(old,currentButton.button,key);
	return currentButton.button != 0;
}


int TranslateKeyCode(int key)
{
	static XU8 vkCodes[] =
	{
		VK_LEFT,	//ID_KEY_LEFT
		VK_RIGHT,	//ID_KEY_RIGHT
		VK_HOME,	//ID_KEY_HOME
		VK_END,		//ID_KEY_END
		VK_INSERT,	//ID_KEY_INSERT
		VK_DELETE,	//ID_KEY_DELETE
		0,			//ID_KEY_CLEAR
		VK_BACK,	//ID_KEY_BACKSPACE
		0,			//ID_KEY_CUT
		0,			//ID_KEY_COPY
		0,			//ID_KEY_PASTE
		0,			//ID_KEY_SELECT_ALL
		VK_RETURN,	//ID_KEY_ENTER
		VK_UP,		//ID_KEY_UP
		VK_DOWN,	//ID_KEY_DOWN
		VK_PRIOR,	//ID_KEY_PAGE_UP
		VK_NEXT,	//ID_KEY_PAGE_DOWN
		VK_TAB,		//ID_KEY_TAB
		VK_ESCAPE	//ID_KEY_ESCAPE
	};
	for(int i=0;i<sizeof(vkCodes);i++)
	{
		if(key==vkCodes[i]) 
		{
			return i+1;
		}
	}
	return key;
}
#endif

//extern void* _DebugWindow;

BOOL XSWFPlayer::HandleKeyDown(XU32 t,int key, int nChar,int nLocation,int modifiers,XBOOL bChar)
{
//	XLock lock(&m_dataLock);
//	if(!lock.Lock(XWAIT_MONMENT))
//		return XFALSE;
#ifdef _WINEMU
	if(key=='F')
		_analyFunction = !_analyFunction;
#endif
#ifdef __STOPRUN__
	if(key=='P')
		_bStopRun=XTRUE;
	if(key=='M')
		_bStopRun=XFALSE;
#endif
	if(!bChar)
	{
	if(key>=0&&key<=255)
	{
		nKeyStatus[key]=XTRUE;
		if(key>='a'&&key<='z')
		{
			//key=key+'A'-'a';
			nKeyStatus[key+'A'-'a']=XTRUE;
		}
	}

	//if(key=='S'&&_DebugWindow)
	//	_DebugWindow=(void*)(((int)_DebugWindow)|1);
	//if(key=='A')
	//	player->Play();
		//int v=0;
//	if(key== 0x28)
//	{
//		LOGWHERE();
//		bits.bitmap->SaveFile("/sdcard/test.bmp");
//	}
	
	nLastKey=key;
	//nKeyCode=key;
	//nKeyStatus=1;
	if(!bChar&&!focus&&m_pAVM2&&currentButton.button==XNULL)
	{
		m_pAVM2->OnKeyDown(key);
		SObject*f=focus?focus:display.button;
		if(f&&f->pASObject)
		{
			void*pContext=f->GetContext();//NULL;
			//ScriptPlayer*player=f->GetPlayer();
			//if(player)
			//			pContext=player->m_pContext;
			//if(f->character)
			//		pContext=f->character->player->m_pContext;
			KEYEVENTDATA data;
			AS3InitKeyData(data,nChar,key,nLocation,f);
			m_pAVM2->OnKeyEvent(XAS3_EVENT_keyDown,data,f->pASObject,pContext);
			//void*pEvent=this->AS3CreateKeyboardEvent(avmshell::XAS3_EVENT_keyDown,nChar,key,nLocation,f->pASObject);
			//m_pAVM2->DispatchEvent(pEvent,f->pASObject);
			//if(nChar==key)
			//int v=0;
			/*while(f)
			{
				if(f->pASObject)
					m_pAVM2->DispatchEvent(pEvent,f->pASObject);
				f=f->parent;
			}*/
		}
		if(f!=&display.root)
		//else
		{
			void*pStage=display.root.pASObject;
			if(pStage)
			{

				KEYEVENTDATA data;
				AS3InitKeyData(data,nChar,key,nLocation,&display.root);
				m_pAVM2->OnKeyEvent(XAS3_EVENT_keyDown,data,pStage,player->m_pLoaderInfo->m_codeContext);//m_pContext);
			}
		}
	}
	key=TranslateKeyCode(key);

	/*SObject*fc=focus?focus:display.buttonThread;
	XBOOL bAction=XFALSE;
	if(fc)
	{
		if(fc->OnEvent(this,XOBJEVENT_KEYDOWN,XNULL))
			bAction=XTRUE;
			//DoActions();
	}*/
	XBOOL bAction=(m_pAVM2||currentButton.button)?XFALSE:display.root.PostEvent(t,this,XOBJEVENT_KEYDOWN,key);
		//bAction=XTRUE;

	if(HandleListener(XOBJEVENT_KEYDOWN))
		bAction=XTRUE;
	//	bAction=XTRUE;
	//if(bAction)
	if(bAction)
	{
		DoActions();
	}
	{
		ButtonLock();

		RemoveAllButtons();

		int iNumButtons = NumButtons(&display.root);

		// Check for any buttons that match this key
		for (int i=0; i<iNumButtons; i++) {
			SObject* obj = buttonArray[i];
			SCharacter* ch = obj->character;

			// Does this button have a keypress state?
			if ( ch->tagCode == stagDefineButton2 ) {
				SParser parser;
				parser.Attach(ch->data, 0);
				BOOL first = XTRUE;
				for (;;) {
					S32 link = parser.pos;
					int offset = parser.GetWord();
					if ( !first ) {
						int code = parser.GetWord();
						code = (code >> 9) & 0x7F;

						if (code != 0 && code == key) {
							ButtonUnlock();

							// Execute the actions for the key condition
							display.PushAction(parser.script+parser.pos, obj,XNULL,XActionList::XA_BLOCK);
							DoActions();

//							UpdateScreen();
//							lock.UnLock();
							return XTRUE;
						}
					}
					if ( !offset ) break;
					parser.pos = link + offset;
					first = XFALSE;
				}
			}
		}

		ButtonUnlock();
	}

	if (TabKeyDown(key, modifiers)) {
//		lock.UnLock();
		return XTRUE;
	}
	}


	if (focus&&(bChar||key<' ')) {
		ScriptPlayer* player = focus->GetPlayer();//character->player;

		EditText* editText = focus->editText;
		// BACKCAST
		BOOL changed=editText->HandleKeyDown( key, modifiers,bChar!=0);//, (NativePlayerWnd*)this );

		// Did the edit text's value actually change?
		//XXVar varName  = editText->m_variable;
		//if (!varName.IsNull())
		if(!m_pAVM2&&changed)
		{
			//char* newValue = editText->GetBuffer();
			XXVar vars(focus->pObject);
			//XXVARLIST args(&vars,1);
			BOOL bAction=focus->OnEvent(0,this,XOBJEVENT_ONCHANGED,&vars);
			XXVar varName  = editText->m_variable;
			if(!varName.IsNull())
			//if (newValue)
			{
				XXVar newv;
				editText->GetBuffer(newv);
				// oldValue may legitimately be NULL if variable isn't set
				//char* oldValue = GetVariable(focus->thread, varName);
				BOOL changed = editText->UpdateVariable(newv);//XFALSE;

				if (changed) {
					//if(focus->pASObject)
					//	focus->AS3Onchange();
						//XBOOL bAction=XTRUE;
						// Call the changed handler.
						const char *changedSuffix = "_changed";
						char *handlerName = new char[strlen(changedSuffix)+varName.GetLength()+1];
						if (handlerName) {
							strcpy(handlerName, varName.strTxt);
							strcat(handlerName, changedSuffix);
							XXVar var(handlerName);
							int frameNum;
							XSWFCONTEXT cnt;
							InitContext(cnt);
							XXObject*pObj=focus->thread->rootObject->pObject;
							cnt.InitContextA(XNULL,pObj,pObj,focus->pObject,focus->pObject,XNULL);
							ScriptThread* targetThread = ResolveFrameNum(var,cnt, &frameNum);
							if (targetThread&&frameNum>=0) {
								//bAction=XFALSE;
								CallFrame(targetThread, frameNum);
							}
							else
								bAction=XTRUE;
							ReleaseContext(cnt);
							delete [] handlerName;
						}

					}
				//delete [] newValue;
			}
			if(bAction)
			{
				DoActions();
			}
		}

		focus->Modify();
//		lock.UnLock();
		return XTRUE;
	}
//	lock.UnLock();
	return XFALSE;
}

void XSWFPlayer::HandleKeyUp(XU32 t,int key,int nChar,int nLocation,int modifiers)
{
	//nKeyStatus=0;

	if(key>=0&&key<=255)
	{
		nKeyStatus[key]=XFALSE;
		if(key>='a'&&key<='z')
		{
			//key=key+'A'-'a';
			nKeyStatus[key+'A'-'a']=XFALSE;
		}
	}
	nLastKey=key;
	if(m_pAVM2&&currentButton.button==XNULL)
	{
		SObject*f=focus?focus:display.button;
//#if (__CORE_VERSION__>=0x02075000)
		XBOOL bEdit=XFALSE;
		if(f&&f->IsEditText()&&f->editText->IsEditAble())
			bEdit=XTRUE;
		if(!bEdit)
			m_pAVM2->OnKeyUp(key);
/*#else
		m_pAVM2->OnKeyUp(key);
#endif*/		

		if(f&&f->pASObject)
		{
			void*pContext=f->GetContext();//NULL;
			//ScriptPlayer*player=f->GetPlayer();
			//if(player)
			//			pContext=player->m_pContext;
			//if(f->character)
			//		pContext=f->character->player->m_pContext;
			KEYEVENTDATA data;
			AS3InitKeyData(data,nChar,key,nLocation,f);
			m_pAVM2->OnKeyEvent(XAS3_EVENT_keyUp,data,f->pASObject,pContext);
			/*void*pEvent=this->AS3CreateKeyboardEvent(avmshell::XAS3_EVENT_keyUp,nChar,key,nLocation,f->pASObject);
			while(f)
			{
				if(f->pASObject)
					m_pAVM2->DispatchEvent(pEvent,f->pASObject);
				f=f->parent;
			}*/
		}
//#if (__CORE_VERSION__>=0x02075000)
		if(!bEdit)
//#endif
		//else
		if(f!=&display.root)
		{
			void*pStage=display.root.pASObject;
			if(pStage)
			{

				KEYEVENTDATA data;
				AS3InitKeyData(data,nChar,key,nLocation,&display.root);
				m_pAVM2->OnKeyEvent(XAS3_EVENT_keyUp,data,pStage,player->m_pLoaderInfo->m_codeContext);//m_pContext);
			}
		}
	}
//	XLock lock(&m_dataLock);
//	if(!lock.Lock(XWAIT_TIME))
//		return;

	/*SObject*fc=focus?focus:display.buttonThread;
	XBOOL bAction=XFALSE;
	if(fc)
	{
		if(fc->OnEvent(this,XOBJEVENT_KEYDOWN,XNULL))
			//DoActions();
			bAction=XTRUE;
	}*/
	XBOOL bAction=(m_pAVM2||currentButton.button)?XFALSE:display.root.PostEvent(t,this,XOBJEVENT_KEYUP,key);

	if(HandleListener(XOBJEVENT_KEYUP))
		bAction=XTRUE;
	//	bAction=XTRUE;
	//if(bAction)
	if(bAction)
		DoActions();

//	lock.UnLock();
}

BOOL XSWFPlayer::IsSelecting()
{
	return focus && focus->editText->m_selecting;
}


#ifdef ACTIONSCRIPT

//DEL void XSWFPlayer::Push(char *value)
//DEL {
//DEL 	if (stackPos >= stackSize) {
//DEL 		if (stackSize) {
//DEL 			stackSize *= 2;
//DEL 		} else {
//DEL 			stackSize = 128;
//DEL 		}
//DEL 		char **newStack = new char* [stackSize];
//DEL 		memcpy(newStack, stack, sizeof(char*) * stackPos);
//DEL 		delete[] stack;
//DEL 		stack = newStack;
//DEL 	}
//DEL 	stack[stackPos++] = CreateStr(value);
//DEL }

//DEL char *XSWFPlayer::Pop()
//DEL {
//DEL 	if (stackPos == 0) {
//DEL 		// Stack underflow
//DEL 		return CreateStr("");
//DEL 	}
//DEL 	char *value = stack[--stackPos];
//DEL 	return value;
//DEL }

//DEL XFDOUBLE XSWFPlayer::PopNum()
//DEL {
//DEL 	char *str = Pop();
//DEL 	XFDOUBLE value;
//DEL 	if (!ConvertStringToDouble(str, &value)) {
//DEL 		value = 0;
//DEL 	}
//DEL 	FreeStr(str);
//DEL 	return value;
//DEL }

//DEL void XSWFPlayer::PushNum(XFDOUBLE value)
//DEL {
//DEL 	char *s = ConvertDoubleToString(value);
//DEL 	Push(s);
//DEL 	FreeStr(s);
//DEL }

BOOL XSWFPlayer::CallFrame(ScriptThread *thread, int frameNum)
{
	if (frameNum < 0) {
		return XFALSE;
	}

	if (thread) {
		// Seek to the frame
		SParser parser;
		parser.Attach(thread->script, thread->startPos);
		int code, framesLeft = frameNum, len = thread->len;
		if (framesLeft) {
			while ((code = parser.GetTag(len)) != -1) {
				parser.pos = parser.tagEnd;
				if (code == stagShowFrame) {
					if (!--framesLeft) {
						break;
					}
				}
			}
			if (framesLeft) {
				// No such frame
				return XFALSE;
			}
		}

		// Scan for DoAction tags in the frame
		while ((code = parser.GetTag(len)) != -1) {
			if (code == stagDoAction) {
				thread->display->PushAction(parser.script+parser.pos, thread->rootObject,XNULL,0);
			}
			if (code == stagShowFrame) {
				break;
			}
			parser.pos = parser.tagEnd;
		}

		// Do the actions!
		DoActions();
		//UpdateScreen();

		return XTRUE;
	}

	return XFALSE;
}

#endif


//
// Button support
//

//
// Button interface methods
//

void XSWFPlayer::ButtonGoDirection(int dir)
{
	ButtonLock();

	SButton oldButton = currentButton;

	// Remove focus from current button
	ButtonFocusRemove(currentButton.button);

	// Construct button ordering around current button
	buttonOrder.Reset();
	ButtonOrdering(&oldButton, &buttonOrder);

	if (buttonOrder.button[dir]) {
		// Set button
		currentButton.button	   = buttonOrder.button[dir];
		currentButton.buttonBounds = buttonOrder.bounds[dir];

		// Set focus to new button
		ButtonFocusSet(currentButton.button);
	} else {
		// Restore old focus
		currentButton = oldButton;
		ButtonFocusSet(oldButton.button);
	}

	ButtonUnlock();
}

void XSWFPlayer::ButtonEnter()
{
	SButton* button = &currentButton;




	if (button->button &&
		button->button->character &&
		(button->button->character->type == buttonChar||button->button->HasMouseEvent()))
	{
		// Take action on button

		// Simulate mouse down
		DoButton(button->button, XTRUE);

		// Simulate mouse up
		DoButton(button->button, XFALSE);
	}

	//XReleaseCapture();
}

int XSWFPlayer::TableIndex(SObject *pObj)
{
	int id=-1;
	ButtonLock();


	ButtonWeight* buttonTabArray = BuildButtonTabMap();
	if (!buttonTabArray) {
		//button->button = NULL;
		ButtonUnlock();
		return id;
	}

	// Find the first weight greater than this one
	int index = 0;
	while (index < buttonIndex) {
		if (buttonTabArray[index].button==pObj)//buttonTabArray[index].weight > weight)
		{
			id=index;
			break;
			//break;
		}
		index++;
	}
	if (index >= buttonIndex) {
		index = 0;
	}


	delete [] buttonTabArray;

	ButtonUnlock();

	return id;
}

void XSWFPlayer::ButtonTabNext()
{
	ButtonLock();

	SButton* button = &currentButton;

#ifdef EDITTEXT
	if (focus) {
		button->button = focus;
		focus->CalcButtonBounds(&button->buttonBounds);
	}
#endif

	int weight = -1;
	if (button->button) {
		weight = CalcButtonWeight(&button->buttonBounds);
	}

	// Remove focus from current button
	ButtonFocusRemove(button->button);

	ButtonWeight* buttonTabArray = BuildButtonTabMap();
	if (!buttonTabArray) {
		button->button = NULL;
		ButtonUnlock();
		return;
	}

	// Find the first weight greater than this one
	int index = 0;
	while (index < buttonIndex) {
		if (buttonTabArray[index].weight > weight) {
			break;
		}
		index++;
	}
	if (index >= buttonIndex) {
		index = 0;
	}

	button->button		 = buttonTabArray[index].button;
	button->buttonBounds = buttonTabArray[index].buttonBounds;

	// Set focus to new button
	ButtonFocusSet(button->button);

	delete [] buttonTabArray;

	ButtonUnlock();
}

void XSWFPlayer::ButtonTabPrevious()
{
	ButtonLock();

	SButton* button = &currentButton;

#ifdef EDITTEXT
	if (focus) {
		button->button = focus;
		focus->CalcButtonBounds(&button->buttonBounds);
	}
#endif

	int weight = -1;
	if (button->button) {
		weight = CalcButtonWeight(&button->buttonBounds);
	}

	// Remove focus from current button
	ButtonFocusRemove(button->button);

	ButtonWeight* buttonTabArray = BuildButtonTabMap();
	if (!buttonTabArray) {
		button->button = NULL;
		ButtonUnlock();
		return;
	}

	// Find the first button weight less than this one
	int index = buttonIndex - 1;
	if (weight != -1) {
		while (index >= 0) {
			if (buttonTabArray[index].weight < weight) {
				break;
			}
			index--;
		}
		if (index < 0) {
			index = buttonIndex - 1;
		}
	}

	button->button		 = buttonTabArray[index].button;
	button->buttonBounds = buttonTabArray[index].buttonBounds;

	// Set focus to new button
	ButtonFocusSet(button->button);

	delete [] buttonTabArray;

	ButtonUnlock();
}

//
// Button implementation
//

void XSWFPlayer::ButtonOrdering(SButton* button, ButtonOrder* order)
{
	FLASHASSERT(button);
	FLASHASSERT(order);

	RemoveAllButtons();

	FLASHASSERT(&display.root);
	int iNumButtons = NumButtons(&display.root);
	if ( iNumButtons == 0 )
		return;	// no buttons in this control

	SObject* dummyButton = NULL;
	SObject* buttonObj   = NULL;
	SRECT    buttonBounds;

	if ( button->button ) {
		// Use current button and previously calculated boundary
		buttonObj	 = button->button;
		buttonBounds = button->buttonBounds;
		//buttonObj->CalcButtonBounds(&buttonBounds);
	}
	else {
		dummyButton = display.CreateObject(player,m_nLoopCount);
		FLASHASSERT(dummyButton);

		SRECT originRect = {0, 0, 0, 0};
		buttonObj		 = dummyButton;
		buttonBounds	 = originRect;
	}

	// Arrange button ordering
	for ( int i=0; i<iNumButtons; i++ ) {
		SObject* buttoni = buttonArray[i];
		SRECT    buttoniBounds;

		if ( buttoni == buttonObj ) {
			// Skip current button
			continue;
		}

		buttoni->CalcButtonBounds(&buttoniBounds);

		SObject* buttonChosen;

		if ( IsButtonUp(&buttonBounds, &buttoniBounds) ) {
			buttonChosen = ButtonChoose(buttonObj, &buttonBounds, UP, buttoni, &buttoniBounds, order->button[UP], &order->bounds[UP]);

			if ( buttonChosen == buttoni ) {
				order->button[UP] = buttoni;
				order->bounds[UP] = buttoniBounds;
			}
		}

		if ( IsButtonDown(&buttonBounds, &buttoniBounds) ) {
			buttonChosen = ButtonChoose(buttonObj, &buttonBounds, DOWN, buttoni, &buttoniBounds, order->button[DOWN], &order->bounds[DOWN]);

			if ( buttonChosen == buttoni ) {
				order->button[DOWN] = buttoni;
				order->bounds[DOWN] = buttoniBounds;
			}
		}

		if ( IsButtonRight(&buttonBounds, &buttoniBounds) ) {
			buttonChosen = ButtonChoose(buttonObj, &buttonBounds, RIGHT, buttoni, &buttoniBounds, order->button[RIGHT], &order->bounds[RIGHT]);

			if ( buttonChosen == buttoni ) {
				order->button[RIGHT] = buttoni;
				order->bounds[RIGHT] = buttoniBounds;
			}
		}

		if ( IsButtonLeft(&buttonBounds, &buttoniBounds) ) {
			buttonChosen = ButtonChoose(buttonObj, &buttonBounds, LEFT, buttoni, &buttoniBounds, order->button[LEFT], &order->bounds[LEFT]);

			if ( buttonChosen == buttoni ) {
				order->button[LEFT] = buttoni;
				order->bounds[LEFT] = buttoniBounds;
			}
		}
	}

	if ( dummyButton ) {
		display.FreeObject(dummyButton);
	}
}

SObject* XSWFPlayer::ButtonChoose(
						SObject* button, 	  //old Button
						P_SRECT  buttonBounds, //old Bounds
						int		 direction,
						SObject* button1, 		//New Direction Button
						P_SRECT  button1Bounds,
						SObject* button2,	     //Old Direction Button
						P_SRECT  button2Bounds)
// Choose which button is closest to the current button in the direction specified
{
	FLASHASSERT(button);
	FLASHASSERT(button1 || button2);

	if ( !button1 ) {
		return(button2);
	}
	else if ( !button2 ) {
		return(button1);
	}

	FLASHASSERT(button1 && button2);

	BOOL bStraight1 = IsButtonAligned(buttonBounds, direction, button1Bounds);
	BOOL bStraight2 = IsButtonAligned(buttonBounds, direction, button2Bounds);

	if ( bStraight1 && bStraight2 ) {
		// Both buttons are aligned in the specified direction,
		// so take the closer one.
		P_SRECT buttonCloser =  ButtonCloser(buttonBounds, button1Bounds, button2Bounds,direction);
		return ( buttonCloser == button1Bounds ) ? button1 : button2;
	}
	else if ( bStraight1 ) {
		return button1;
	}
	else if ( bStraight2 ) {
		return button2;
	}

	// Neither of the buttons are aligned with the current button


	BOOL bOverlap1  = IsButtonOverlapped(buttonBounds, direction, button1Bounds);
	BOOL bOverlap2  = IsButtonOverlapped(buttonBounds, direction, button2Bounds);

	if ( bOverlap1 && bOverlap2 ) {
		// Both buttons overlap, so take the closer one
		P_SRECT buttonCloser =  ButtonCloser(buttonBounds, button1Bounds, button2Bounds,direction);
		return ( buttonCloser == button1Bounds ) ? button1 : button2;
	}
	else if ( bOverlap1 ) {
		return button2;
	}
	else if ( bOverlap2 ) {
		return button1;
	}
	else {
		// Neither button overlaps, so take the closer one
		P_SRECT buttonCloser =  ButtonCloser(buttonBounds, button1Bounds, button2Bounds,direction);
		return ( buttonCloser == button1Bounds ) ? button1 : button2;
	}

}

BOOL XSWFPlayer::IsButtonUp(P_SRECT origin, P_SRECT button)
{
	return (button->ymax < origin->ymin || button->ymax < origin->ymax);
}

BOOL XSWFPlayer::IsButtonDown(P_SRECT origin, P_SRECT button)
{
	return (button->ymin > origin->ymax || button->ymax > origin->ymax);
}

BOOL XSWFPlayer::IsButtonRight(P_SRECT origin, P_SRECT button)
{
	return (button->xmin > origin->xmax || button->xmax > origin->xmax);
}

BOOL XSWFPlayer::IsButtonLeft(P_SRECT origin, P_SRECT button)
{
	return (button->xmax < origin->xmin || button->xmin < origin->xmin);
}

BOOL XSWFPlayer::IsButtonAligned(P_SRECT origin, int direction, P_SRECT button)
{
	FLASHASSERT(origin);
	FLASHASSERT(button);

	if(direction == DOWN || direction == UP)
	{
		if( ( origin->xmax < button->xmin ) ||
		    ( origin->xmin > button->xmax ) ) {
			return XFALSE;
		}
		else {
			return XTRUE;
		}
	}
	else if ( direction == RIGHT || direction == LEFT ) {
		if( ( origin->ymax < button->ymin ) ||
		    ( origin->ymin > button->ymax ) ) {
			return XFALSE;
		}
		else {
			return XTRUE;
		}
	}

	FLASHASSERT(0);
	return XFALSE;
}

BOOL XSWFPlayer::IsButtonOverlapped(P_SRECT origin, int direction, P_SRECT button)
{
	FLASHASSERT(origin);
	FLASHASSERT(button);

	if ( direction == DOWN ) {
		FLASHASSERT(IsButtonDown(origin, button));
		return ( button->ymin > origin->ymax );
	}

	return XFALSE;
}

static XINT DeltaV(int s0, int s1, int d0, int d1)
{
	if(s0<=d0)
	{
		if(s1>=d1) return 0;
		else if(s1>=d0) return 1;//1;
	}
	else
	{
		if(s1<=d1) return 0;
		else if(s0<=d1) return 1;//1;
	}
	return (XABS((s0-d0))>>4)+2;
}

P_SRECT XSWFPlayer::ButtonCloser(P_SRECT origin, P_SRECT button1, P_SRECT button2,int direction)
{
	FLASHASSERT(origin);
	FLASHASSERT(button1);
	FLASHASSERT(button2);

	int l1=0,d1=0;
	int l2=0,d2=0;

	switch(direction)
	{
	default:
		{
			SCOORD d1 = RectDistance(origin, button1);
			SCOORD d2 = RectDistance(origin, button2);
			return (d1 < d2) ? button1 : button2;
		}break;
	case LEFT:
		 l1=(origin->xmin-button1->xmax)>>4;
		 l2=(origin->xmin-button2->xmax)>>4;
		 d1=DeltaV(button1->ymin,button1->ymax,origin->ymin,origin->ymax);
		 d2=DeltaV(button2->ymin,button2->ymax,origin->ymin,origin->ymax);
		 //nLength=(m_select.left-rect.right)>>4;
		 //nDelta=Delta(rect.top,rect.bottom,m_select.top,m_select.bottom);//  //XABS((rect.top-m_select.top));
		 break;
	case UP:
		 l1=(origin->ymin-button1->ymax)>>4;
		 l2=(origin->ymin-button2->ymax)>>4;
		 d1=DeltaV(button1->xmin,button1->xmax,origin->xmin,origin->xmax);
		 d2=DeltaV(button2->xmin,button2->xmax,origin->xmin,origin->xmax);
		 break;
	case RIGHT:
		 l1=(button1->xmin-origin->xmax)>>4;
		 l2=(button2->xmin-origin->xmax)>>4;
		 d1=DeltaV(button1->ymin,button1->ymax,origin->ymin,origin->ymax);
		 d2=DeltaV(button2->ymin,button2->ymax,origin->ymin,origin->ymax);
		 break;
	case DOWN:
		 l1=(button1->ymin-origin->ymax)>>4;
		 l2=(button2->ymin-origin->ymax)>>4;
		 d1=DeltaV(button1->xmin,button1->xmax,origin->xmin,origin->xmax);
		 d2=DeltaV(button2->xmin,button2->xmax,origin->xmin,origin->xmax);
		 break;
	}

	if(l1==l2)
	{
		return d1<d2?button1:button2;
	}
	else return l1<l2?button1:button2;



	//return XTRUE
}

BOOL XSWFPlayer::ButtonFind(SButton* removed, SButton* current)
// Find the current button that (most closely) matches the removed button.
{
	ButtonLock();

	FLASHASSERT(removed);
	FLASHASSERT(current);

	RemoveAllButtons();

	int iNumButtons = NumButtons(&display.root);
	if ( iNumButtons == 0 ) {
		ButtonUnlock();
		return XFALSE;
	}

	for ( int i=0; i<iNumButtons; i++ ) {
		SObject* buttoni = buttonArray[i];
		FLASHASSERT(buttoni);

		SRECT buttoniBounds;
		buttoni->CalcButtonBounds(&buttoniBounds);

		// Check if button sizes are equal
		if ( RectEqual(&buttoniBounds, &removed->buttonBounds, 0) ) {
			current->button		  = buttoni;
			current->buttonBounds = buttoniBounds;

			ButtonUnlock();
			return XTRUE;
		}
	}

	ButtonUnlock();
	return XFALSE;
}


int XSWFPlayer::NumButtons(SObject* obj)
{
	FLASHASSERT(obj);

	if(!obj->IsTabChildren()) return 0;

	int iNumButtons = 0;

	obj = obj->bottomChild;
	while ( obj ) {

		BOOL isButton = XFALSE;

		if(obj->IsVisible()&&obj->IsEnable())//obj->visible&&obj->enable)
		{

		switch(obj->character->type)
			{
#ifdef EDITTEXT
		case editTextEmptyChar:
		case editTextChar:
			 if(!(obj->editText->m_flags & seditTextFlagsReadOnly))
				 isButton=XTRUE;
			 break;
#endif
		case buttonEmptyChar:
		case buttonChar:
			 isButton=XTRUE;
		case spriteChar:
		case spriteEmptyChar:
		case spriteExternalChar:
			 if(obj->HasMouseEvent())
				 isButton=XTRUE;
			 break;
			}

		if (isButton) {
			iNumButtons += 1;
			AddButton(obj);
		}

		iNumButtons += NumButtons(obj);
		}
		obj = obj->above;
	}

	return iNumButtons;
}

void XSWFPlayer::ButtonFocusSet(SObject* button)
{
#ifdef EDITTEXT
	// Clear the edit text focus
	if (focus) {
		focus->editText->ClearSelection();
		focus->Modify();
		focus = NULL;
	}
#endif

	DoButton(button, XFALSE);

	// Set mouseState to -1 so the current button won't
	// change until the mouse is moved.
	mouseState = -1;
}

void XSWFPlayer::ButtonFocusRemove(SObject* button)
{
	DoButton((SObject*)NULL, XFALSE);
}

//
// Methods used to help enumerate buttons
//

static int		ADDITIONAL_BUTTONS = 50;

void XSWFPlayer::AddButton(SObject* obj)
{
	FLASHASSERT(obj);

	if ( buttonIndex == arraySize ) {
		// Dynamically resize array
		SObject** newArray = new SObject*[arraySize + ADDITIONAL_BUTTONS];
		FLASHASSERT(newArray);

		// Copy old array into new array
		memcpy(newArray, buttonArray, arraySize * sizeof(SObject*));

		// Delete old array
		delete [] buttonArray;

		// Initialize new array
		buttonArray = newArray;
		arraySize  += ADDITIONAL_BUTTONS;
	}

	FLASHASSERT(buttonIndex < arraySize);

	// Add button to end of button array.
	FLASHASSERT(buttonArray);
	buttonArray[buttonIndex++] = obj;
}

void XSWFPlayer::RemoveAllButtons()
{
	buttonIndex = 0;
	delete [] buttonArray;
	buttonArray = NULL;
	arraySize   = 0;
}

//
// Tabbing implementation
//

XSWFPlayer::ButtonWeight* XSWFPlayer::BuildButtonTabMap()
{
	ButtonWeight* buttonTabArray = NULL;

	RemoveAllButtons();

	int iNumButtons = NumButtons(&display.root);

	if ( iNumButtons == 0 ) {
		return NULL;
	}

	buttonTabArray = new ButtonWeight[iNumButtons+1];
	FLASHASSERT(buttonTabArray);

	SRECT originRect = {0, 0, 0, 0};

	for ( int i=0; i<iNumButtons; i++ ) {
		SObject* button = buttonArray[i];
		FLASHASSERT(button);

		buttonTabArray[i].button = button;
		button->CalcButtonBounds(&buttonTabArray[i].buttonBounds);

		// Use an algorithm to weight each button.
		buttonTabArray[i].weight = CalcButtonWeight(&buttonTabArray[i].buttonBounds);
	}

	// Sort buttons based on weight
	ButtonSort(buttonTabArray, iNumButtons-1);

	return buttonTabArray;
}

S32 XSWFPlayer::CalcButtonWeight(P_SRECT bounds)
// Weight button by distance from origin, giving more weight to
// vertical direction than horizontal.
{
	// Weight factors
	const int xWeightFactor = 3;
	const int yWeightFactor = 2;
	return (bounds->xmin/xWeightFactor) + (bounds->ymin*yWeightFactor);
}

void XSWFPlayer::ButtonSort(ButtonWeight* array, int size)
// Sort buttons based on distance
{
	int i,j;
	ButtonWeight t;

	for ( i=size; i>=0; i-- ) {
		for ( j=1; j<=i; j++ ) {
			if( array[j-1].weight > array[j].weight ) {
				t = array[j-1];
				array[j-1] = array[j];
				array[j]   = t;
			}
		}
	}
}

void XSWFPlayer::CreateIdealPalette( SColorTable* ctab )
{
	const U8 colorRamp6[] = { 0, 0x33, 0x66, 0x99, 0xCC, 0xff };

	// Set up the a Palette of colors that we would like to be available.
	// Fill in the color ramp - 6 levels of each color
	int i = 0;
	for ( int b = 0; b <= 5; b++ ) {
		for ( int g = 0; g <= 5; g++ ) {
			for ( int r = 0; r <= 5; r++ ) {
				if ( r == g && g == b )
					continue;// don't do any grays here
#ifdef _ARGB_PIXEL_
				ctab->colors[i].red   = colorRamp6[r];
				ctab->colors[i].green = colorRamp6[g];
				ctab->colors[i].blue  = colorRamp6[b];
//#elif __ANDROID__
//				ctab->colors[i].red   = colorRamp6[b];
//				ctab->colors[i].green = colorRamp6[g];
//				ctab->colors[i].blue  = colorRamp6[r];
#else
				ctab->colors[i].red   = colorRamp6[r];
				ctab->colors[i].green = colorRamp6[g];
				ctab->colors[i].blue  = colorRamp6[b];
#endif
				ctab->colors[i].alpha = 0;
				i++;
			}
		}
	}
	ctab->n = i;
}

// update the buffer and blt to the screen given a DC
//DEL void XSWFPlayer::OnDraw()
//DEL {
//DEL 	// BACKCAST
//DEL 	NativePlayerWnd* native = (NativePlayerWnd*)(this);
//DEL
//DEL 	if ( UpdateBuffer() && bits.LockBits() )
//DEL 	{
//DEL 		if ( !RectIsEmpty( &display.screenDirtyRgn ) )
//DEL 		{
//DEL 			// simply blt the buffer to the screen
//DEL 			bits.bltToScreen(	native,
//DEL 								display.screenDirtyRgn.xmin, display.screenDirtyRgn.ymin,
//DEL  								RectWidth(&display.screenDirtyRgn), RectHeight(&display.screenDirtyRgn),
//DEL 								display.screenDirtyRgn.xmin, display.screenDirtyRgn.ymin
//DEL 							);
//DEL 		}
//DEL
//DEL 		UnlockBuffer();
//DEL
//DEL 		bits.UnlockBits();
//DEL
//DEL 	} else {
//DEL 		bits.clearScreen( native );
//DEL 	}
//DEL }

void XSWFPlayer::Repaint( SRECT* rect )
{
	SRECT dev = *rect;

	/*if ( display.antialias_b ) {
		//dev.xmin *= 4;
		//dev.ymin *= 4;
		//dev.xmax *= 4;
		//dev.ymax *= 4;
		display.ToAnti(dev);
	}*/

	display.InvalidateRect( &dev );

}

//DEL void XSWFPlayer::UpdateScreen()
//DEL {
//DEL 	// Copy to the screen
//DEL 	SRECT dirty;
//DEL
//DEL 	display.CalcBitsDirty(&dirty);
//DEL 	RectUnion(&dirty, &display.screenDirtyRgn, &dirty);
//DEL
//DEL  	if ( !RectIsEmpty(&dirty) )
//DEL 	{
//DEL 		OnDraw();
//DEL 	}
//DEL 	RectSetEmpty(&display.screenDirtyRgn);
//DEL }

BOOL XSWFPlayer::UpdateBuffer(BOOL render)
{
	//return XTRUE;
	// Check to create the offscreen bitmap

	if(player->resize)
		return XFALSE;
	XSIZE sz;
	sz.cx=m_winSize.cx;
	sz.cy=m_winSize.cy;//cy;
	//UpdateBuffer(XSize(cx,cy),XFALSE);

	//XLock lock(&display.raster.dibLock);   // modify by hgl
 	/*if(display.raster.dibLock.Lock(XSHORTTIME))
	{
		if ( bits.LockBits()||bits.IsEmpty())
		{

			if(bits.IsEmpty()||
			   (sz.cx!=bits.width()||sz.cy!=bits.height()))
			{
				bits.setAllowPurge( XTRUE );

				bits.CreateScreenBits(sz,this->m_maxSize);// native, &cr );
				display.UpdateDevViewRect();
				InvalidateScreenArea(XRect(0,0,sz.cx,sz.cy));
			}
		}

		bits.UnlockBits();
		display.raster.dibLock.UnLock();
	}

	if ( render ) {
		// Be sure the bits are up to date
#ifdef _LOWSPEED
		if(!m_bDragOrZoom)
#endif
		display.Update();
		m_bUpdateRedraw=XTRUE;
	}*/



	return XTRUE;
}

void XSWFPlayer::CheckUpdate()
{
	/*if(display.m_bIsLocked)//!directRect.IsRectEmpty())
	{
		m_updateRect=directRect;
		directRect.SetRectEmpty();
		return;
	}
	//Trace(XString16("Update"));
	//if(display.newOffsetBGY>display.offsetBGY)
	//	int v=0;
	SRECT dirty;
	display.CalcBitsDirty(&dirty);
	//if(dirty.xmin>0)
	//	int vv=0;
	RectUnion(&dirty, &display.screenDirtyRgn, &dirty);
	//if(!directRect.IsEmpty())
	
	if ( !RectIsEmpty(&dirty) )
	{
		dirty.xmin=0;
		dirty.ymin=0;
		dirty.xmax=this->m_winSize.cx;
		dirty.ymax=this->m_winSize.cy;
        InvalidateScreenArea( XRect(dirty.xmin,dirty.ymin,dirty.xmax,dirty.ymax));
			//&dirty );
		RectSetEmpty(&display.screenDirtyRgn);
	}*/
	//display.Update();
}

bool XSWFPlayer::NeedMove()
{
	//if(scaleMode!=DisplayList::showAll&&scaleMode!=DisplayList::fitFull)//m_config.m_nZoomMode)// !RectIsEmpty(&zoomRect) )
	//{
	//	return true;
	//}
	return false;
}

int XSWFPlayer::GetCursorType()
{
	int type = CURSOR_TYPE_ARROW;
	if(!cursorShow) return CURSOR_TYPE_HIDE;
	if ( display.button&&display.button->AS3HasMouseEvent())//&&display.buttonThread->character->type==buttonChar )
	{
		type = CURSOR_TYPE_BUTTON;
	}
	//else if(scaleMode!=DisplayList::showAll&&scaleMode!=DisplayList::fitFull)//m_config.m_nZoomMode)// !RectIsEmpty(&zoomRect) )
	//{
	//	//if(NeedOriginal())
	//		//zoomRect.xmax>=player->frame.xmax&&
 //          //zoomRect.ymax>=player->frame.ymax&&
	//	   //zoomRect.xmin<=player->frame.xmin&&
	//	   //zoomRect.ymin<=player->frame.ymin)
	//	  // type=CURSOR_TYPE_ARROW;
	//	//else
	//		type = CURSOR_TYPE_ARROW;//CURSOR_TYPE_HAND;
	//}

	#ifdef EDITTEXT
	if ( display.iBeam&&focus )
	{
		type = CURSOR_TYPE_BEAM;
#if defined(__TOUCHPAD__)||defined(__POINT__)
		if(focus->editText->m_bURL)
			type=CURSOR_TYPE_BUTTON;
#endif
	}
	#endif

	return type;
}

void XSWFPlayer::BuildRelative(XXVar&baseV,XXVar&urlV)//char* base, char* url)
{
	//int maxLen = baseV.GetLength()+urlV.GetLength()+10;//strlen(base)+strlen(url)+10;
	//char*base=baseV.strTxt;
	//char*url=urlV.strTxt;
	//char* c = url;
	char* c = urlV.strTxt;
	while ( *c ) {
		if ( *c == '/' ) break;
		if ( *c == ':' ) {
			// It's already an absolute URL
			//strcpy(urlBuf, url);
			//urlV=urlBufV;
			return;
		}
		c++;
	}

	//if(strBaseVar.IsNull())
	{
		XURL setURL;
		setURL.SetURL(baseV.strTxt);
		if(!strBaseVar.IsNull())
			setURL.SetURL(strBaseVar.strTxt);
		setURL.SetURL(urlV.strTxt);
		urlV=setURL.m_strURL;
		urlV.ToString(XTRUE);
	}
	/*else
	{
		XURL setURL;
		setURL.SetURL(baseV.strTxt);
		setURL.SetURL(urlV.strTxt);
		urlV=setURL.m_strProtocol;
		urlV.StringAdd(setURL.m_strHost);
		if(strBaseVar.strTxt[0]!='/')
			urlV.StringAdd("/");
		urlV.StringAdd(strBaseVar);
		urlV.StringAdd(setURL.m_strPath);
		urlV.StringAdd(setURL.m_strFile);
	
	}*/

	/*XXVar urlBufV=_strEmpty;
	urlBufV.ToString(XTRUE);
	urlBufV.SetLength(maxLen,false);

	char*urlBuf=urlBufV.strTxt;


	// Build a relative URL
	strcpy(urlBuf, base);	//

	// Extract just the machine name from the base
	char* m = urlBuf;
	while ( XTRUE ) {
		if ( !*m ) {
			m = urlBuf;	// we did not find a machine name
			goto NoMachine;
		}
		if ( m[0] == '/' && m[1] == '/' ) {
			m += 2;
			break;
		}
		m++;
	}
	while ( XTRUE ) {
		if ( !*m ) {
			// Add a trailing / to the machine name
			*m++ = '/';
			*m = 0;
			break;
		}
		if ( m[0] == '/' ) {
			m++;
			break;
		}
		m++;
	}

	// m should be pointing to the first path
NoMachine:
	if ( url[0] == '/' ) {
		// A root (machine) relative url
		*m = 0;	// strip any path info

		strcat(urlBuf, url+1);
		urlV=urlBufV;
		urlV.AdJustLength();//SetLength(strlen(urlV.strTxt));
		return;// urlBuf;

	} else {
		// Remove "../" from the url
		do {
			// Remove a directory (or file on first pass) from the base URL
			char* tail = urlBuf + strlen(urlBuf)-1;
			if ( *tail == '/' && tail >= m ) *tail-- = 0;
			while ( *tail && tail >= m ) {
				if ( *tail == '/' ) break;
				*tail-- = 0;
			}

			char* u = SkipPrefix(url, "../");
			if ( !u ) break;
			url = u; // Strip the prefix
		} while ( XTRUE );

		strcat(urlBuf, url);
		urlV=urlBufV;
		urlV.AdJustLength();//SetLength(strlen(urlV.strTxt));
		return;
	}*/
}

void XSWFPlayer::NSResolveURL(XXVar&url)//char* actionURL)
{
	if ( !strURLBase.IsNull() ) {
		// Generate URLs relative to a given path
		char* ub = strURLBase.strTxt;////=//urlBase;
		if ( ub[0] == '.' && ub[1] == 0 ) {
			// Relative to the .spl file
			//ub = url;
			BuildRelative(strURL,url);
			return;
		}
		BuildRelative(strURLBase,url);
		//return BuildRelative(ub, actionURL);
	}
	// Let Netscape resolve the URL
}

void XSWFPlayer::NSGetURL(_XLoadInfo*pInfo)//char* actionURL, char* window, char* postData,XBOOL bPost)
{
	// Check for the special FSCommand URL
	//return;
	if ( pInfo->strTarget.IsNull() )
		pInfo->strTarget = "_self";

	//LoadInfo*pInfo=new LoadInfo;// info;//pInfo=new LoadInfo;
	//if(!pInfo) return;

	//pInfo->layer=-1;
	//pInfo->pASObject=NULL;
	//pInfo->pObject=NULL;
	//pInfo->spriteName=XNULL;

	NSResolveURL(pInfo->strURL);
	//char* resolvedURL = NSResolveURL(actionURL);
	if ( !pInfo->strURL.IsNull())//resolvedURL )
	{
		//pInfo->url=resolvedURL;
		//pInfo->window=window;

		NetscapeDoURL(pInfo );
		/*if ( resolvedURL != actionURL )
		{
			delete resolvedURL;
		}*/
	}
	else
	{
		//if(pInfo->pLoader)
		//	pInfo->pLoader->DecrementRef();
		//pInfo->Release();
		//FREEOBJECT(pInfo->pObject);
		//pInfo->strURL.Release();
		//pInfo->strTarget.Release();
		//pInfo->str
		//display.FreeLoad(pInfo);
		XReleaseINFO(pInfo);
	}

		//delete pInfo;
}

void XSWFPlayer::NetscapeDoURL(LoadInfo* info)
// Handle a URL operation for Netscape based on whether we want a notify and or a POST operation
// Note that this routine will do a Notify if the window is NULL, otherwise, it will do a
{
  //if ( info->postData.GetSize() )
  //{
		// Build up the headers that Navigator wants for a POST...
		//URLBuilder ub;
	    /*const char* strCntTypeURL="Content-type: application/x-www-form-urlencoded";
		const char* strCntType="Content-type: ";
		const char* strCntLength="\nContent-length: ";
	    XU8Array ub;
		if(info->strContext.IsNull())
			ub.Append((XU8*)strCntTypeURL,strlen(strCntTypeURL));
		else
		{
			ub.Append((XU8*)strCntType,strlen(strCntType));
			ub.Append(info->strContext.strTxt,info->strContext.GetLength());
		}
		ub.AppendString((XU8*)strCntLength,strlen(strCntLength));
		XXVar s=_strEmpty;
		s.AppendInt(info->postData.GetSize());
		ub.Append(s.strTxt,s.GetLength());
		ub.Append("\n\n",2);
		ub.Append(info->postData.GetData());
		ub.Append("\n",1);

// 		notifyData = notify;
		info->postData.RemoveAll();
		info->postData.Append((XU8*)ub.buf,ub.len+1);*/
			//.Append((XU8*)ub.buf,ub.len+1);
		//bPost=XTRUE;
		//StreamPostURLNotify( url, window, ub.len+1, ub.buf, (void*)info );
	//}
	//else
	{
		// Just do a GET
// 		notifyData = notify;
	//	info->url=url;
	//	info->window=window;
		StreamGetURLNotify(info);
	}
}

void XSWFPlayer::LoadLayer( LoadInfo*pInfo/* char* url,
						  int layer,
						  char* postData,
						  char *target,
						  int loadMethod,
						  XXObject*pObj,
						  void*pASObject*/)
{
	if((pInfo->nType&kHttpLoadData)==0)
	{
		bool            loadVars = (pInfo->nType&kHttpLoadVariables)!=0;//loadMethod & kHttpLoadVariables;
		bool            loadTarget = (pInfo->nType&kHttpLoadTarget)!=0;
		ScriptThread    *thread = 0;
		XBOOL bRoot=XFALSE;
		//XBOOL bPost=(loadMethod&kHttpSendUsePost)?XTRUE:XFALSE;
		// find target
		if (loadTarget)
		{
			SObject*obj = NULL;
			if(pInfo->pLoader)
			{
				//obj=((DisplayObjectObject*)pInfo->pLoader)->m_pObject;
				NSResolveURL(pInfo->strURL);

				NetscapeDoURL(pInfo);
				return;
			}
			else obj=XSWFPlayer::FindTarget(player->rootObject,pInfo->strTarget.strTxt,XNULL);
			thread=obj?obj->thread:XNULL;
			if (!thread)
			{
				//XReleaseINFO(pInfo);
				//pInfo->layer=-1;
				return;
			}

			// unload original sprite before anything else
			if (!loadVars)
			{
				if(obj->parent == &display.root) bRoot=XTRUE;
				RemoveSprite(thread, XTRUE);
			}
		}

		if ( pInfo->strURL.IsNull()&& !loadVars && !loadTarget && pInfo->layer >= 0)
		{
			ClearLayer(pInfo->layer);
			//if(pInfo->pLoader)
			//	pInfo->pLoader->DecrementRef();
				//m_pAVM2->ReleaseInstance(&pInfo->pLoader,false);
			//pInfo->strTarget.Release();
			//pInfo->strURL.Release();
			//FREEOBJECT(pInfo->pObject);
			//display.FreeLoad(pInfo);
			XReleaseINFO(pInfo);
			return;
		}


		// see if we are loading a sprite instead
		// of a movie layer. If so find the target
		// thead that is going to get the new script
		if (loadTarget)
		{
			if(!bRoot)
				pInfo->layer = -1;
		}
		else if(!loadVars)
		{
			if(pInfo->layer>=0)
				ClearLayer(pInfo->layer); 	// clear out the old contents of the layer immediately??
		}
	}
    // NOTE: we don't remove sprites if we can't resolve the url.
    //          The user can use remove sprite action instead

	// Start loading a new stream
	//char* resolvedURL = NSResolveURL(url);
	//LoadInfo info;
	//loadInfo->url=resolvedURL;
	//loadInfo->window=XNULL;
	NSResolveURL(pInfo->strURL);
	NetscapeDoURL(pInfo);
	/*if ( resolvedURL )
	{
		NetscapeDoURL(  postData, loadInfo,bPost );
		if ( resolvedURL != url )
		{
			delete resolvedURL;
		}
	}*/
}


static bool FilterReplace(_XLoadInfo*pInfo,XPREVFILTER*pFilter,XSWFPlayer*player)
{
		if(!pFilter)
			return false;
	
		if(pFilter->nType!=XPREVFILTER::_SELF|| !pInfo->strTarget.IsNull()||pInfo->pLoader!=NULL )
		{
#ifdef _WINEMU
			XString16 s("[Note] Replace URL:");
			s+=XString16(pInfo->strURL.strTxt);
			s+=XString16(" To ");
			s+=XString16(pFilter->strDest);
			player->Trace(s);
#endif

			XString8 strTmp = pFilter->strDest;
			if ( pFilter->nReplace == XPREVFILTER::_FILTER_HALT )
			{
				char *pTmp = NULL;
				pTmp = strstr( pInfo->strURL.strTxt, "?");
				if ( pTmp != NULL )
				{
					strTmp += (pTmp + 1);
				}
			}
			pInfo->strURL.SetString(strTmp.GetData(), strTmp.GetLength() );
			pInfo->nTarget=pFilter->nType;
			pInfo->nType&=~(kHttpSendUsePost|kHttpSendUseGet);
			if(pFilter->nMethod)
				pInfo->nType|=kHttpSendUsePost;
			else
				pInfo->nType|=kHttpSendUseGet;
			if(!pFilter->strPost.IsEmpty())
			{
				if(pFilter->bPost)
				{
					pInfo->postData.SetSize(0);//.FreeAll();
					pInfo->postData.Append((XU8*)pFilter->strPostDest.GetData(),pFilter->strPostDest.GetLength());
				}
				else
				{
					if(pInfo->postData.GetSize())
						pInfo->postData.Add((XU8)'&');
					pInfo->postData.Append((XU8*)pFilter->strPost.GetData(),pFilter->strPost.GetLength());
				}
			}
			if(pFilter->strDest.GetLength()<=0) return false;
			return true;
		}
		return false;
}

static bool FilterReplaceDomain(_XLoadInfo*pInfo,XDOMFILTER*pDomFilter,XSWFPlayer*player,bool bDefault,bool bPage)
{
	if(!pDomFilter)
		return false;
	//if(strstr(pInfo->strURL.strTxt,"NewGuide")!=NULL)
	//	int v=0;
	if(bPage)
	{
		int i,iCount=pDomFilter->strAllows.GetSize();
		for(i=0;i<iCount;i++)
		{
			XString8&strAllow=pDomFilter->strAllows[i];
			int l=pInfo->strURL.GetLength();
			if((int)strAllow.GetLength()>l) continue;
			if(strAllow.Compare(pInfo->strURL.strTxt,XTRUE,strAllow.GetLength())==0)
				return true;
		}
	}
	//LOGWHEREMSG(pInfo->strURL.strTxt);
	//if(strstr( pInfo->strURL.strTxt, "resource/cn/swf/chat/chatui.swf") != NULL )
	//{
	//	int i = 0;
	//}
	XPREVFILTER*pFilter=pDomFilter->pRevFilter,*pReplace=NULL;
	while(pFilter)
	{
		if(bDefault&&pFilter->strHost=="*"&&!pReplace)
			pReplace=pFilter;
		if(pFilter->strHost.Compare(pInfo->strURL.strTxt,XTRUE,pFilter->strHost.GetLength())==0)
		{
			if(!pFilter->bPost&&pFilter->strPost.GetLength()>0)
			{
				XPCTSTR str=(XPCTSTR)pInfo->postData.GetData();
				int l=XMIN(pInfo->postData.GetSize(),pFilter->strPost.GetLength());
				if(pFilter->strPost.Compare(str,XTRUE,l)==0)
				{
					pReplace=pFilter;
				}
			}
			else
				pReplace=pFilter;
		}
		else if(pFilter->bPost)
		{
			if(pFilter->strHost.Compare(pInfo->strURL.strTxt,XTRUE,pInfo->strURL.GetLength())==0)
			{
				XPCTSTR str=(XPCTSTR)pInfo->postData.GetData();
				int l=XMIN(pInfo->postData.GetSize(),pFilter->strPost.GetLength());
				if(pFilter->strPost.Compare(str,XTRUE,l)==0)
				{
					pReplace=pFilter;
				}
			}
		}
		pFilter=pFilter->pNext;
	}
	if(pReplace)
	  return FilterReplace(pInfo,pReplace,player);
	return false;
}

void XSWFPlayer::GetURL(_XLoadInfo*pInfo)// char* actionURL, char* window, char* postData, int loadMethod,XXObject*pObj,void*pASObject)
{
	//return;
	char* cmd = SkipPrefix(pInfo->strURL.strTxt, "FSCommand:");
	if(!cmd)
	{
		cmd=SkipPrefix(pInfo->strURL.strTxt,"Javascript:");
	}
	if ( cmd )
	{

		//if(pInfo->pLoader)
			//pInfo->pLoader->DecrementRef();
		//pInfo->strURL.Release();
		//pInfo->strTarget.Release();
		//display.FreeLoad(pInfo);
		//FREEOBJECT(pInfo->pObject);
		XReleaseINFO(pInfo);
		ProcessFSCommand( cmd, pInfo->strTarget.strTxt );
	}
	else
	{
        // Check to load a layer
		char *target = pInfo->strTarget.strTxt;//window;
//		if(!target)
//			int v=0;
		int layerNum = LayerNum(target);
		if (layerNum > 0 || (pInfo->nType & kHttpLoadTarget) || (pInfo->nType&kHttpLoadContext))
        {
            // see if we are loading into a target
            if (!(pInfo->nType & kHttpLoadTarget)) target = 0;
			// LoadLayer moved from native to core.
			pInfo->layer=layerNum;
			NSResolveURL(pInfo->strURL);
			FilterReplaceDomain(pInfo,m_pDomFilter,this,false,false);			
			//pInfo->strTarget=_strEmpty;
			LoadLayer(pInfo);
			if(!pInfo->IsTargetLoader()&&pInfo->layer<0)
				XReleaseINFO(pInfo);
			return;
        }
		else
		{
			NSResolveURL(pInfo->strURL);
			//NetscapeDoURL(pInfo);
			if(FilterReplaceDomain(pInfo,m_pDomFilter,this,true,true)||IsJQBar(pInfo->strURL.strTxt))
			{
				pInfo->layer=-1;
				NetscapeDoURL(pInfo);
				if(!pInfo->IsTargetLoader())
					XReleaseINFO(pInfo);
				return;
			}
			/*if(m_pDomFilter)
			{
				XPREVFILTER*pFilter=m_pDomFilter->pRevFilter,*pReplace=NULL;
				while(pFilter)
				{
					if(pFilter->strHost=="*"&&!pReplace)
						pReplace=pFilter;
					if(pFilter->strHost.Compare(pInfo->strURL.strTxt,XTRUE,pFilter->strHost.GetLength())==0)
					{
						pReplace=pFilter;
					}
					pFilter=pFilter->pNext;
				}
				if(pReplace)
				{
					if(FilterReplace(pInfo,pReplace,this))
					{
						pInfo->layer=-1;
						NetscapeDoURL(pInfo);
						if(pInfo->IsTargetLoader())
							XReleaseINFO(pInfo);
						return;
					}
				}
			}*/
			/*if(target)
			{
				if(XString8::Compare(target,"_self",0,XTRUE)==0||
				   XString8::Compare(target,"self",0,XTRUE)==0)
				{
					pInfo->layer=-1;
					NetscapeDoURL(pInfo);
					if(pInfo->IsTargetLoader())
							XReleaseINFO(pInfo);
					return;
				}
			}*/
#ifdef _WINEMU
			XString16 s("[Note] Refuse Connect URL:");
			s+=XString16(pInfo->strURL.strTxt);
			Trace(s);
#endif
			//if(pInfo->pLoader)
			//	pInfo->pLoader->DecrementRef();
			//pInfo->strTarget.Release();
			//pInfo->strURL.Release();
			//FREEOBJECT(pInfo->pObject);
			//display.FreeLoad(pInfo);
			XReleaseINFO(pInfo);
			/*_XLoadInfo*INFO=pInfo;
			INFO->strURL.Release();
			INFO->strTarget.Release();
			INFO->strContext.Release();
			INFO->strHeaders.RemoveAll();
			INFO->strRefer.Release();
			INFO->postData.RemoveAll(XTRUE);
			if(INFO->pLoader) m_pAVM2->RemoveObject(INFO->pLoader);
			if(INFO->pObject)
				INFO->pObject->FreeObject();
			display.FreeLoad(INFO);*/
			// NSGetURL moved from native to core
			//return;
			//NSGetURL(actionURL, window, postData,loadMethod&kHttpSendUsePost);
		}
	}
}

void XSWFPlayer::ReleaseInfo(void*info)
{
	//return;
	LoadInfo* loadInfo = (LoadInfo*)info;
	m_loadLock.Lock();
	XReleaseINFO(loadInfo);
	m_loadLock.UnLock();
}

void XSWFPlayer::StreamInNew( StreamData* streamData, char* streamUrl,char*urlBase, char* contentType,
							 void* notifyData,XString8Array&headList,int nCode,void*pClient )
{
	//FLASHOUTPUT( "StreamInNew %x\n", streamData );
	m_nRunTime=XTime::GetTimeMSec();
	GCENTER
	int i=0;
	while(streamUrl[i])
	{
		if(streamUrl[i]=='\\')
			streamUrl[i]='/';
		i++;
	}

	/*if(firstStream)
	{
		const char*ss=::strstr(streamUrl,"::");
		if(ss-streamUrl>=3)
			SecurityClass::m_nSandType=SecurityClass::XAS3_REMOTE;
		else
			SecurityClass::m_nSandType=SecurityClass::XAS3_LOCAL_TRUSTED;
	}*/
	if(strURLBase.IsNull())
	{
		strURLBase=urlBase;//streamUrl;
		strURLBase.ToString(XTRUE);
		/*int i,nSize=headList.GetSize();
		for(i=0;i<nSize;i+=2)
		{
			if(headList[i].Compare("width",XTRUE)==0)
				m_nFrameWidth=headList[i+1].ToInt();
			else if(headList[i].Compare("hight",XTRUE)==0)
				m_nFrameHeight=headList[i+1].ToInt();
		}*/
		if(m_pDomFilter)
		{
			SRGB c;
			c.all=m_pDomFilter->nBackColor;
			display.SetBackgroundColor(c);
		}
		int iCount = headList.GetSize();
		for(i=0;i<iCount;i+=2)
		{
			XString8&strTmp=headList[i];
			if(strTmp.Compare("base",true)==0)
			{
				strURLBase=headList[i+1];
				strURLBase.ToString(XTRUE);
			}
		}
	}

	//bError=XFALSE;
	// copy the url to stream data, since the client should not modify streamData
//	XLock lock(&m_dataLock);
//	if(!lock.Lock(XSHORTTIME)) return;
	streamData->url = streamUrl;//CreateStr( streamUrl );
	streamData->url.ToString(XTRUE);
	streamData->loadInfo=NULL;
	//XFileFinder::GetFileName(streamURL,m_strAppName);

	// Open up a layer for the the stream
	S32 layer;
	if ( firstStream )//m_strAppName
	{
		layer = 0;				// force the first stream to load to layer 0
		streamData->resize = XTRUE;
		//XString8 strTmp(streamUrl,STRING_ATTACH);
		//m_strAppName.SetString(XNULL,
		//XFileFinder::GetFileName(strTmp,m_strAppName,XFALSE);
		//strTmp.DataTo();
		if(player&&m_pDomFilter)
		{
			if(!m_pDomFilter->strWaitFor.IsEmpty())
			{
				player->SetWaitClass(m_pDomFilter->strWaitFor,m_pDomFilter->nWaitID);
			}
		}
	}
   	else
	{
		layer = (S32)notifyData;		// If layer is -1, then this is just a notice from the browser.
										// If it is positive, then it is actually a pointer to a LoadInfo struct
		streamData->resize = XFALSE;
	}

	LoadInfo* loadInfo = (LoadInfo*) layer;

	if ( layer >= 0 )
	{
        ScriptPlayer* p = 0;


        if (layer == 0)
        {
		    p = OpenLayer(0);
			p->AS3_OnOpen();
        }
        else
        {
			// This stream is for a Load Sprite or a Load Variable action.
			if( loadInfo->pLoader)
			{
				streamData->scriptPlayer=XNULL;
				streamData->pLoader=loadInfo->pLoader;
				m_pAVM2->PushObject(loadInfo->pLoader);
				m_pAVM2->StreamInNew(loadInfo->pLoader,streamData,streamUrl,contentType,headList,nCode,pClient,loadInfo);
				if(loadInfo->pObject&&pClient)
				{
					XClient*pc=(XClient*)pClient;
					pc->SetCallID((XU32)loadInfo->pObject);
				}

				//loadInfo->pLoader->StreamInNew(streamData,streamUrl,contentType,headList,nCode);
				XClient*pc=(XClient*)pClient;
				if(!pc||!pc->IsAlive()||!pc->IsNeedCache())
				{
					m_loadLock.Lock();
					XReleaseINFO(loadInfo);
					m_loadLock.UnLock();
				}
					

				return;
				/*if(loadInfo->nType&kHttpLoadData)
					p=NULL;
				else
				{
					SObject*pObj=((DisplayObjectObject*)loadInfo->pLoader)->m_pObject;
					if(pObj)
						p=pObj->character->player;
					else
						p=NULL;
				}
				if(p)
					p->AS3OnOpen();*/
			}
			else if (loadInfo->nType&kHttpLoadVariables)//loadVars)
			{

				p = OpenVariableLoader(loadInfo->layer, loadInfo->strTarget);//strSpriteName);
			}
			else
			{
				if (!loadInfo->strTarget.IsNull()&&loadInfo->layer<=0)//spriteName)
				{
					// Loaded to a target path
					SObject *obj = XNULL;
					//if(display.root.bottomChild)
					obj=FindTarget( player->rootObject,
									//display.root.bottomChild->bottomChild,
									loadInfo->strTarget.strTxt,XNULL);
					if(!obj)
					{
						if(player->rootObject)
							goto exit_gracefully;
						p=OpenLayer(0);
						streamData->resize=XTRUE;
					}
					else
					{
						//obj=
						obj->SetCloneFlag();
						ScriptThread*thread=obj?obj->thread:XNULL;
						if (!thread)
						{
							goto exit_gracefully;
						}
						p = OpenSprite(thread);
					}
				} else {
					// Loaded to a layer number
					p = OpenLayer(loadInfo->layer);
					if(loadInfo->layer==ADLEVEL)
						this->m_pADPlayer=p;
				}
				if(p)
					p->AS3_OnOpen();
			}

            // cleanup

        }

//         *stype = NP_ASFILE;	// this forces the data to be cached even if the file is large
		//*stype = NP_NORMAL;

		// Parse the variables from the URL and add to the base thread
		/*if (!p->variableLoader&&p->rootObject) {
			XSWFCONTEXT cnt;
			InitContext(cnt);
			ExtractVariables(cnt, p, streamData->url);
		}*/

		p->stream = streamData;	// link the objects
		streamData->scriptPlayer = p;
		//FreeStr(p->url);
		p->m_url = streamData->url;//CreateStr(streamData->url);
		p->m_url.ToString(XTRUE);
		p->m_loadUrl=p->m_url;
		p->m_contentType=contentType;
		p->m_contentType.ToString(XTRUE);

		// Do special stuff for the first stream we get
		if ( firstStream ) {
			firstStream = XFALSE;
			// Save the base URL
			//FreeStr(url);
			strURL=streamData->url;
			streamData->url.Release();
			//url = CreateStr(streamData->url);
		}
#ifdef		_WINEMU_DEBUGFILE
		//else
		if(strstr(streamUrl,"://")!=NULL)
		{
			int ls=0;
			const char* appName=p->GetAppName(ls);
			if(appName)
			{
				XString8 strFile=this->m_strWorkPath.strTxt;
				strFile+="Download\\";
				XFile::CreateFolder(strFile);
				XString8 strName(appName,ls);
				strFile+=strName;
				strFile+=".swf";
				streamData->m_downFile.Open(strFile,XFile::XWRITE|XFile::XCREATE|XFile::XBINARY);
			}
		}
#endif



	} else {
		// If layer < 0, this is just a notify from a browser navigate operation and we don't care
	}

	if(streamData->scriptPlayer)
	{	ScriptPlayer*p=streamData->scriptPlayer;
		if(loadInfo)
		{

			if(loadInfo->pObject)
			{
				p->SetEvent(loadInfo->pObject);
				if(loadInfo->pObject->IsObject(XXOBJ_LOADVARS))
				{
					XXObjectLoadVars*p=(XXObjectLoadVars*)loadInfo->pObject;
					p->SetTotal(streamData->nTotalBytes);
					p->SetLoaded(streamData->nLoadedBytes);
				}
				XU8 vars[sizeof(XXVar)*2];
				XXVARLIST list(vars,2);
				list.Push(nCode);//Add(new XXVar(nCode));

				//list.FreeAll();
				for(int i=0;i<(int)headList.GetSize()-1;i+=2)
				{
					XXVar name,txt;
					name.SetString(headList[i],headList[i].GetLength());
					txt.SetString(headList[i+1],headList[i+1].GetLength());
					loadInfo->pObject->AddMember(name,txt);
				}
				if(loadInfo->pObject->HandlerEvent(this,XNULL,XOBJEVENT_HTTPSTATUS,&list))
					DoActions();
			}
			/*else
			{
				for(int i=0;i<(int)headList.GetSize()-1;i+=2)
				{
					XXVar name,txt;
					name.SetString(headList[i],headList[i].GetLength());
					txt.SetString(headList[i+1],headList[i+1].GetLength());

					p->m_heads.Add(name);
					p->m_heads.Add(txt);
					//loadInfo->pObject->AddMember(name,txt);
				}
			}*/

		}

		for(int i=0;i<(int)headList.GetSize()-1;i+=2)
		{
			XXVar name,txt;
			name.SetString(headList[i],headList[i].GetLength());
			txt.SetString(headList[i+1],headList[i+1].GetLength());

			p->m_heads.Add(name);
			p->m_heads.Add(txt);
			//loadInfo->pObject->AddMember(name,txt);
		}

		if (p->variableLoader)
		{
			SObject*obj = FindTarget(display.root.bottomChild->bottomChild, p->variableSprite.strTxt,XNULL);
			ScriptThread*thread=obj?obj->thread:XNULL;
			if (thread)
			{
				XXVar arg(nCode);
				if(thread->rootObject->OnEvent(0,this,XOBJEVENT_HTTPSTATUS,&arg))
					DoActions();
			}
		}
		else
		{
			if(p->rootObject)
			{
				XXVar arg(nCode);
				if(p->rootObject->OnEvent(0,this,XOBJEVENT_HTTPSTATUS,&arg))
					DoActions();
			}
		}
		//p->CreateLoader();
		p->AS3_OnHttpStatus(nCode);
	}



exit_gracefully:
	if(loadInfo &&layer >= 0 )
	{
		//FREEOBJECT(loadInfo->pObject);
		//if(loadInfo->pLoader)
		//	loadInfo->pLoader->DecrementRef();
		//loadInfo->strURL.Release();
		//loadInfo->strTarget.Release();
		//FreeStr(loadInfo->spriteName);
		//delete loadInfo;
		//display.FreeLoad(loadInfo);
		m_loadLock.Lock();
		XReleaseINFO(loadInfo);
		m_loadLock.UnLock();
	}
//	lock.UnLock();
	return;
}

void XSWFPlayer::SetErrorCode(int nErrorCode)
{
	m_nErrorCode=nErrorCode;
	if(nErrorCode&&pWindow)
	{
		pWindow->Handle(XCMD_FLASHERROR,nErrorCode,0);
	}
}

bool XSWFPlayer::CloseClient(void*pClient)
{
	if(this->pWindow)
		return pWindow->Handle(XCMD_CLOSECLIENT,(XU32)pClient,0)!=0;
	return false;
}

static XU8 bsr[]={0xf1,0x74,0x33,0x22,0xa9,0x10,0x00,0x01,0x78,0x45,0x90,0xa8,0xb0,0xb7,0xc8,0x53,0xd2,0x52,
				0xe9,0xf6,0xe2,0x18,0x28,0x42,0x30,0x02,0x09,0x21,0x44,0x44,0x12,0x21,0x90,0x56,0xb2,0xd3};

int XSWFPlayer::StreamInWrite( StreamData*	streamData,
							void* buffer,
							int	  length,void*pClient )
{
// 	FLASHOUTPUT( "StreamInWrite %x\n", streamData );
	//return 0;
	///////////////////////////////////////////////////////////////
	XU8*pBuf=(XU8*)buffer;
	if(!streamData->notFirst)
	{
		XU8 b1=pBuf[0]^bsr[0];
		XU8 b2=pBuf[1]^bsr[1];
		XU8 b3=pBuf[2]^bsr[2];
		streamData->notFirst=true;
		if((b2=='w'||b2=='W')&&
		   (b3=='s'||b3=='S')&&
		   (b1=='C'||b1=='F'||b1=='c'||b1=='f'))
		{
			streamData->isBDT=true;
			streamData->nOffset=0;
		}
	}

	if(streamData->isBDT)
	{
		int iOff=streamData->nOffset;
		for(int i=0;i<length;i++)
		{
			pBuf[i]=pBuf[i]^bsr[iOff%sizeof(bsr)];
			iOff++;
		}
		streamData->nOffset=iOff;
	}

	//


	m_nRunTime=XTime::GetTimeMSec();
	GCENTER
	if(streamData->pLoader)
	{
		//return ((avmshell::EventDispatcherObject*)streamData->pLoader)->StreamInWrite(buffer,length,&gcEnter);
		return m_pAVM2->StreamInWrite((avmshell::EventDispatcherObject*)streamData->pLoader,buffer,length,&gcEnter,pClient);
		//	return LOAD_OK;
		//return LOAD_BREAK;
	}
#ifdef _WINEMU_DEBUGFILE
	if(streamData->m_downFile.IsValid())
		streamData->m_downFile.Write(buffer,length);
#endif
	ScriptPlayer* p = streamData->scriptPlayer;
	XBOOL bInit=XFALSE;
//	XLock lock(&m_dataLock);
//	if(!lock.Lock(XSHORTTIME)) return LOAD_AGAIN;
	if ( p&&!bError ) {
		p->resize=streamData->resize;
		if(p->m_pEvent&&p->m_pEvent->IsObject(XXOBJ_LOADVARS))
		{
			XXObjectLoadVars*pe=(XXObjectLoadVars*)p->m_pEvent;
			pe->SetTotal(streamData->nTotalBytes);
			pe->SetLoaded(streamData->nLoadedBytes);
		}

		int nCode=p->PushData((U8*)buffer, length,&gcEnter);
		// see if we can size the window - added lee@middlesoft
		if ( streamData->resize && p->gotHeader )
		{
			int nMaxW=this->m_nFrameWidth;
			int nMaxH=this->m_nFrameHeight;
			if(this->m_pDomFilter)
			{
				nMaxW=XMAX(nMaxW,m_pDomFilter->nFrameWidth);
				nMaxH=XMAX(nMaxH,m_pDomFilter->nFrameHeight);
				//
			}
			nMaxW*=20;
			nMaxH*=20;
			//if(p->frame.xmax<nMaxW)
			if(nMaxW)
				p->frame.xmax=nMaxW;
			if(nMaxH)
			//if(p->frame.ymax<nMaxH)
				p->frame.ymax=nMaxH;
			p->resize=XFALSE;
			int width = (int)RectWidth( &p->frame ) / 20;
			int height = (int)RectHeight( &p->frame ) / 20;
			//::RectSetEmpty(&zoomRect);
			//if(m_nScreenDirect==2)
			ChangeMode(width,height);
			//AdjustWindow( width, height );

			bInit=XTRUE;
			running=XTRUE;
			streamData->resize = XFALSE;
		}


		if(p==this->m_pADPlayer&&p->m_bInitOK&&p->rootObject)
		{
			int aw=RectWidth(&p->frame);
			int ah=RectHeight(&p->frame);
			int sw=RectWidth(&player->frame);
			int sh=RectHeight(&player->frame);
			int nMode=XDOMFILTER::AD_TOP;
			if(m_pDomFilter&&!m_pDomFilter->strADURL.IsEmpty())
				nMode=m_pDomFilter->nADMode;
			else
				nMode=m_config.m_nADMode;
			MATRIX&mat=p->rootObject->xform.mat;
			switch(nMode)
			{
			case XDOMFILTER::AD_TOP:
				 mat.ty=0;
				 mat.tx=(sw-aw)/2;
				 break;
			case XDOMFILTER::AD_BOTTOM:
				 mat.ty=(sh-ah);
				 mat.tx=(sw-aw)/2;
				 break;
			case XDOMFILTER::AD_LEFT:
				 mat.tx=0;
				 mat.ty=(sh-ah)/2;
				 break;
			case XDOMFILTER::AD_RIGHT:
				 mat.tx=sw-aw;
				 mat.ty=(sh-ah)/2;
				 break;
			}
			p->rootObject->Modify();
		}


		//return length;
		if(nCode==ScriptThread::notSupport||nCode==ScriptThread::as3ScriptError)
		{
			streamData->scriptPlayer=XNULL;
			if(p==player)
			{
				bError=XTRUE;
				//Trace(XString16(":ER1"));
				return LOAD_BREAK;
			}
		}
		p->AS3_OnProgress();
		if(p->len>=p->scriptLen)
			p->AS3_OnComplete();
		if(bInit)
		{
//			lock.UnLock();
			return LOAD_INITOK;
		}

		if(p->m_bCanceled)
			return LOAD_BREAK;
	}

//	lock.UnLock();
	return LOAD_OK;	// we want to abort the stream...
}

void XSWFPlayer::StreamInDestroy( StreamData* streamData,XBOOL bOK,const char*msg,void*pClient)
{
	//if(!bOK)
	//	int v=0;
	//char * pTmp = streamData->url.strTxt;
	//LOGWHEREMSG( pTmp );
	//Trace(XString16(":Destroy"));
	//if(bError) return;
	//XString16 s("GC");
	//s.FromInt((int)m_pAVM2->GetGC(),true);
	//Trace(s);
	m_nRunTime=XTime::GetTimeMSec();
	if(!bOK)
		SetErrorCode(0x10000000);
#ifdef _WINEMU
	if(bOK)
	{
		XString16 s(">>>Load OK:");
		XString16 ss;
		ss.ConvertFrom(streamData->url.strTxt,XCHARSET_GB2312);
		s+=ss;//XString16(streamData->url.strTxt);
		Trace(s);
	//	if(strstr(streamData->url.strTxt,"preload/TDYX.swf"))
	//		int v=0;
	}
	else
	{
		XString16 s(">>>[Note]Load Failed(");
		if(pClient)
		{
			XString8& strURL=((XClient*)pClient)->GetConnectInfo()->URL.m_strURL;
			XString16 ss;
			ss.ConvertFrom(strURL,XCHARSET_GB2312);
			s+=ss;////XString16(strURL);
			s+=XString16(" ");
		}
		s+=XString16(msg);
		s+=XString16("):");
		s+=XString16(streamData->url.strTxt);
		Trace(s);
	}
#endif
	if(bError) return;

	GCENTER


	//FLASHOUTPUT( "StreamInDestroy %x\n", streamData );

	if(streamData->pLoader)
	{
		//((avmshell::EventDispatcherObject*)streamData->pLoader)->StreamInDestroy(bOK,msg);
		//if(strstr(streamData->url.strTxt,"lan_text.tsze")!=NULL)
		//		int v=0;
		if(m_pAVM2->StreamInDestroy((avmshell::EventDispatcherObject*)streamData->pLoader,bOK,msg,pClient))
		{
			m_pAVM2->RemoveObject(streamData->pLoader);
			streamData->url.Release();
		}
		//if(m_pAVM2)
		//{
		//
		//	DoEvent();
		//}
		return;
	}

#ifdef _WINEMU_DEBUGFILE
	if(streamData->m_downFile.IsValid())
		streamData->m_downFile.Close();
#endif

    ScriptPlayer* p = streamData->scriptPlayer;
	if ( p ) {
		p->stream = 0;	// disconnect the stream
		if(p==this->player)
			this->LanuchAD();
		if(p->m_pEvent&&p->m_pEvent->IsObject(XXOBJ_LOADVARS))
		{
			XXObjectLoadVars*pe=(XXObjectLoadVars*)p->m_pEvent;
			pe->SetTotal(streamData->nTotalBytes);
			pe->SetLoaded(streamData->nLoadedBytes);
		}
		//// Resize the window if it has not happened yet.
		//if ( streamData->resize )
		//{
		//	int width = (int)RectWidth( &p->frame ) / 20;
		//	int height = (int)RectHeight( &p->frame ) / 20;
		//	//bAdJustWin=XTRUE;
		//	AdjustWindow( width, height );
		//}
		XSWFCONTEXT cnt;
		InitContext(cnt);
		p->SetDataComplete(cnt,bOK);
		streamData->scriptPlayer=XNULL;
		//if(bOK)
		//	p->AS3_OnComplete();
		//else
		if(!bOK)
			p->AS3_OnIOError(msg);
	}
	if(streamData->loadInfo)
	{
		LoadInfo* loadInfo = streamData->loadInfo;
		if(loadInfo->pLoader)
		{
			if(m_pAVM2->StreamInDestroy((avmshell::EventDispatcherObject*)loadInfo->pLoader,bOK,msg,pClient))
			{
				m_pAVM2->RemoveObject(loadInfo->pLoader);//streamData->pLoader);
				streamData->url.Release();
			}
			//return;
		}	
		if(loadInfo->pObject&&loadInfo->pObject->IsObject(XXOBJ_LOADVARS))
		{
			XXObjectLoadVars*pObj=(XXObjectLoadVars*)loadInfo->pObject;
			XXObject*pEvent=pObj;
			if(pObj->m_target.IsObject())
				pEvent=pObj->m_target.pObject;

			pEvent->AddMember(_SYSVAR(loaded),XXVar(bOK));
			XU8 vars[sizeof(XXVar)*2];
			XXVARLIST list(vars,2);
			list.PushBool(bOK);//Add(new XXVar(bOK));
			//m_pEvent->HandlerEvent(splayer,OBJOFTHREAD(thread),XOBJEVENT_LOAD,&list);
			pEvent->HandlerEvent(this,OBJOFTHREAD(player),XOBJEVENT_LOAD,&list);
			pObj->m_target.Release();
				//splayer->DoActions();
		}
		m_loadLock.Lock();
		XReleaseINFO(loadInfo);
		m_loadLock.UnLock();
	}

	if(m_pAVM2)
	{
		//if(strstr(streamData->url.strTxt,"lan_text.tsze")!=NULL)
		//	int v=0;
		bool bMouse=false;
		DoEvent(bMouse);
	}
	// clean up the url memory
	//if ( streamData->url )
	//{
	//	FreeStr( streamData->url );
	//	streamData->url = 0;
	//}
	streamData->url.Release();
	display.Invalidate();

}

int GetUnixFontData(char **data);

#ifdef EDITTEXT
BOOL XSWFPlayer::LoadBuiltInFonts()
{
#ifdef FULLFONT
  if(buildInFont.font.shapeData) return XTRUE;

    //printf("---->%s", m_strWorkPath.strTxt);
  //XString8 strFile=m_strWorkPath.strTxt;
  //XResource::TransResPath(strFile);
 // strFile+="fonts.fnt";
  //if(!m_fontFile.Open(strFile)) return XFALSE;

  buildInFont.splayer=player;
  buildInFont.tag=1;
  buildInFont.type=fontChar;
  buildInFont.tagCode=1;
  buildInFont.data=0;
  buildInFont.length=0;
  buildInFont.className=XNULL;
  buildInFont.gridRect=XNULL;
  buildInFont.classEntry=XNULL;
  buildInFont.font.nGlyphs=0;

  XU16 nCount=65534;
  //m_fontFile.Read(&nCount,sizeof(XU16));
  buildInFont.font.deviceState=textDync;
  buildInFont.font.flags=0x9c;	   //sfontFlagsWideCodes
  buildInFont.font.infoData=_advanceTable;
  //int nc=buildInFont.font.nGlyphs;
  //XU8*pData=new XU8[nCount*4+nCount*4+4+nCount*4];
  //*((XU16*)pData)=nCount;
  buildInFont.font.nGlyphs=nCount;
  //if(pData==XNULL) {m_fontFile.Close();return XFALSE;}
  //m_fontFile.Read(pData,nCount*4+nCount*4+4);
  //buildInFont.font.infoData=pData;
  buildInFont.font.shapeData=
	  (GLTranglesOrLines**)new XU8[nCount*sizeof(GLTranglesOrLines*)];
  memset(buildInFont.font.shapeData,0,nCount*sizeof(GLTranglesOrLines*));
 // XU32*pAddr=(XU32*)(pData+nCount*4+nCount*4+4);
 // for(int i=0;i<nCount;i++)
//	  pAddr[i]=0;
///*
//#ifdef DOTFONT
////  bDotFontOK=XTRUE;
//  strFile=m_strWorkPath.strTxt;
//  strFile+="hz16.dot";
//  XFile file;
//#ifdef __APPLE__
//  if(file.Open(strFile,XFile::XREAD|XFile::XBINARY,XTRUE))
//#else
//  if(file.Open(strFile,XFile::XREAD|XFile::XBINARY))
//#endif
//  {
//	 dotFont16.splayer=player;
//	 dotFont16.tag=1;
//	 dotFont16.type=fontChar;
//	 dotFont16.tagCode=2;
//	 dotFont16.data=0;
//	 dotFont16.length=0;
//	 dotFont16.className=XNULL;
//	 dotFont16.gridRect=XNULL;
//	 dotFont16.classEntry=XNULL;
//	 dotFont16.font.nGlyphs=0;
//     int l=file.GetLength()-2;
//	 file.Read(&nCount,sizeof(XU16));
//	 dotFont16.font.nGlyphs=nCount;
//	 dotFont16.font.deviceState=textDync;
//	 dotFont16.font.flags=0x09c|XDOT_FONT16;
//	 dotFont16.font.infoData=XNULL;
//	 dotFont16.font.nDataOffset=nCount*3;
//	 dotFont16.font.shapeData=
//	  (GLTranglesOrLines**)new XU8[nCount*sizeof(GLTranglesOrLines*)];
//	 memset(dotFont16.font.shapeData,0,nCount*sizeof(GLTranglesOrLines*));
//	 if(l>0)
//	 {
//		XU8*pData=new XU8[l];
//		if(pData)
//		{
//			file.Read(pData,l);
//			dotFont16.font.nDataOffset=l;
//			dotFont16.font.infoData=pData;
//		}
//	 }
//	 file.Close();
//  }
//  else
//	  bDotFontOK=XFALSE;
//  bDotFontOK=XTRUE;
//  strFile=m_strWorkPath.strTxt;
//  strFile+="hz12.dot";
//  //XFile file;
//#ifdef __APPLE__
//  if(file.Open(strFile,XFile::XREAD|XFile::XBINARY,XTRUE))
//#else
//  if(file.Open(strFile,XFile::XREAD|XFile::XBINARY))
//#endif
//  {
//	 dotFont12.splayer=player;
//	 dotFont12.tag=1;
//	 dotFont12.type=fontChar;
//	 dotFont12.tagCode=3;
//	 dotFont12.data=0;
//	 dotFont12.length=0;
//	 dotFont12.className=XNULL;
//	 dotFont12.gridRect=XNULL;
//	 dotFont12.classEntry=XNULL;
//	 dotFont12.font.nGlyphs=0;
//     int l=file.GetLength()-2;
//	 file.Read(&nCount,sizeof(XU16));
//	 dotFont12.font.nGlyphs=nCount;
//	 dotFont12.font.deviceState=textDync;
//	 dotFont12.font.flags=0x09c|XDOT_FONT12;
//	 dotFont12.font.infoData=XNULL;
//	 dotFont12.font.shapeData=
//	  (GLTranglesOrLines**)new XU8[nCount*sizeof(GLTranglesOrLines*)];
//	 memset(dotFont12.font.shapeData,0,nCount*sizeof(GLTranglesOrLines*));
//	 if(l>0)
//	 {
//		XU8*pData=new XU8[l];
//		if(pData)
//		{
//			file.Read(pData,l);
//			dotFont12.font.nDataOffset=l;
//			dotFont12.font.infoData=pData;
//		}
//	 }
//	 file.Close();
//  }
//  else
//	  bDotFontOK=XFALSE;
//    
//#endif
 

  return XTRUE;
#else
  if (builtInFontsState != kBuiltInFontsNotLoaded) {
    return (builtInFontsState == kBuiltInFontsLoaded);
  }

  char *fontScript = 0;
  int fontScriptLen = GetUnixFontData(&fontScript);
  if (fontScriptLen <= 0) {
    builtInFontsState = kBuiltInFontsError;
    return XFALSE;
  }

  builtInFontsPlayer->SetDisplay(&display);
  builtInFontsPlayer->splayer = this;
  builtInFontsPlayer->layerDepth = 16000;

  builtInFontsPlayer->PushData((U8*) fontScript, fontScriptLen);

  builtInFontsPlayer->DoTags(0);

  builtInFontsPlayer->rootObject->SetVisible(XFALSE);

  delete [] fontScript;

  builtInFontsState = kBuiltInFontsLoaded;
  return XTRUE;
#endif
}
#endif

void XSWFPlayer::ClientRect(SRECT &rect)
{
	rect.xmin=0;
	rect.ymin=0;
	rect.xmax=m_winSize.cx;
	rect.ymax=m_winSize.cy;
}
////////////////////////////////////////////////////////////////////////////////////////////////
// ������Ϣ
void XSWFPlayer::SetFilePath(XPCTSTR strPath)
{
	m_strFilePath = strPath;
	int i;
	for(i=0;i<m_strFilePath.GetLength();i++)
	{
		char c=m_strFilePath[i];
		if(c=='\\')
			m_strFilePath[i]='/';
	}
	m_strFilePath+='/';
}

void XSWFPlayer::StreamGetURLNotify(LoadInfo*notifyData)
{
//		if(pWin->Handle(XCMD_PROCFLASHTEXT,(XU32)strTxt,(XU32)&dst))
//				return XTRUE;
	//if(!notifyData) return;
#ifdef _WINEMU
			XString16 s("--->Connect URL:");
			XString8 s8;
			s8.ConvertFrom(notifyData->strURL.strTxt,XCHARSET_UTF8);
			s+=XString16(s8);
			if(notifyData->postData)
			{
				//FILE *pFile = fopen("D://temp.dat", "wb" );
				//fwrite( notifyData->postData.GetData(), notifyData->postData.GetSize(), 1, pFile );
				//fclose( pFile );
				s+=XString16("(");
				XString8 ss((char*)notifyData->postData.GetData(),notifyData->postData.GetSize());
				s+=XString16(ss);
				s+=XString16(")");
			}
			Trace(s);
#endif
	if(pWindow)
	{
		(pWindow->Handle(XCMD_GETFLASHURL,(XU32)notifyData,(XU32)&strURL));
//#if (__CORE_VERSION__>=0x02077000)
		if(notifyData->pClient&&notifyData->pLoader)
			notifyData->pLoader->SetClient(notifyData->pClient);
//#endif
		//{
			//FREEOBJECT(notifyData->pObject);
			//FreeStr(notifyData->spriteName);
			//delete notifyData;
		//}
	}
	else
	{
		//FreeStr(notifyData->spriteName);
		//FREEOBJECT(notifyData->pObject);
		//if(notifyData->pLoader)
		//	notifyData->pLoader->DecrementRef();
		//notifyData->strTarget.Release();
		//notifyData->strURL.Release();
		//display.FreeLoad(notifyData);
		//delete notifyData;
		XReleaseINFO(notifyData);
	}
}

void XSWFPlayer::CloseStream(StreamData *pStream)
{
	pStream->scriptPlayer=XNULL;
}

const char* XSWFPlayer::ProcessFSCommand(char *cmd, char *args)
{
	if(!cmd) return NULL;
	if(this->m_pDomFilter)
	{
		XString8Array&list=m_pDomFilter->strExtFuns;
		int i,nc=list.GetSize();
		const char*strDefault=NULL;
		for(i=0;i<nc;i+=2)
		{
			if(list[i]==(XPCTSTR)cmd)
				return list[i+1];
			else if(list[i].IsEmpty())
				strDefault=list[i+1];
		}
		if(strDefault)
			return strDefault;
		//list=m_pDomFilter->strExtFuns2;
		nc=m_pDomFilter->strExtFuns2.GetSize();
		strDefault=NULL;
		for(i=0;i<nc;i+=2)
		{
			if(m_pDomFilter->strExtFuns2[i].IsEmpty())
				strDefault=m_pDomFilter->strExtFuns2[i+1];
			else if(strstr(cmd,m_pDomFilter->strExtFuns2[i])!=NULL)
				return m_pDomFilter->strExtFuns2[i+1];
			//else if(list[i].IsEmpty())
				
		}
//#if (__CORE_VERSION__>=0x02079000)
		nc=m_pDomFilter->strExtFuns4.GetSize();
		for(i=0;i<nc;i+=2)
		{
			if(m_pDomFilter->strExtFuns4[i]==(XPCTSTR)cmd)
			{
				//m_pDomFilter->strExtFuns2[i+1];
				if(pWindow)
					pWindow->Handle(XCMD_FLASHCOMMAND,XFLASH_RELOAD,(XU32)m_pDomFilter->strExtFuns4[i+1].GetData());
				return NULL;
			}
		}
//#endif
		if(strDefault)
			return strDefault;
	}
	if(!pWindow) return NULL;
	switch(cmd[0])
	{
	case 'q':
	case 'Q':
		pWindow->PostMsg(XCMD_FLASHCOMMAND,XFLASH_QUIT,0);
		break;
	default:
		if(strstr(cmd,"reload()")!=NULL)
			pWindow->PostMsg(XCMD_FLASHCOMMAND,XFLASH_REFLASH,0);
		break;
	}
	return NULL;
}

void XSWFPlayer::InitPlayer(XSIZE &winSize,XWindow *pWin)
{
	//m_winSize=winSize;
	pWindow=pWin;
	if(player)
	{
		player->frame.xmax=winSize.cx*20;
		player->frame.ymax=winSize.cy*20;
		display.SetCamera(&player->frame);
	}
}


//XBOOL XSWFPlayer::Paint(XGraphics &g, int dx, int dy, XRECT &clip)
//{
//	return XTRUE;
//}


void XSWFPlayer::CallFrame(XSWFCONTEXT &cnt,ScriptThread*thread, XXVar *pVar,int pos)
{
		//if (display.actionCallStackTop < MaxActionCallStack)
		{
		int frameNum;
		ScriptThread* targetThread;
		if(pVar->IsNum3())
		{
			targetThread=CLIPTHREAD(cnt.thread);//GetTarget();
			frameNum=pVar->ToInt();
		}
		else
		{
//			if(strcmp(pVar->strData,"/:NewDeal")==0)
//				int v=0;
//			if(strcmp(pVar->strData,"CheckValues")==0)
//						int v=0;
			targetThread=ResolveFrameNum(*pVar,cnt, &frameNum);
		}
		if (targetThread&&frameNum>=0) {

			//ActionList *actionList = cnt.context->actionList;
			// Save position in old action list
			//int i = actionList->actionBase & actionList->actionMask;
			//actionList->actionList[i] += pos;

			SParser parser;
			parser.Attach(targetThread->script,
						  targetThread->startPos);
			int code;
			int curFrame = 0;
			int len = targetThread->len;
			// Seek to the frame
			if (frameNum) {
				while ((code = parser.GetTag(len)) != -1) {
					parser.pos = parser.tagEnd;
					if (code == stagShowFrame) {
						if (++curFrame == frameNum) {
							break;
						}
					}
				}
			}

			while ((code = parser.GetTag(len)) != -1) {
				if (code == stagDoAction) {
					XActionScript action(parser.script+parser.pos);
					XXObjectClip*pClip=(XXObjectClip*)targetThread->rootObject->pObject;
					action.InitContext(pClip,pClip,pClip,pClip,XNULL,cnt.pCaller);
						//		  	   targetThread,XNULL);

					XCONSTVECTOR consts;
					consts.Copy(cnt.constants);
					action.DoAction(cnt,pClip,XFALSE);
					cnt.constants.Copy(consts);
					//newActions->PushAction(parser.script+parser.pos, targetThread);
				}
				if (code == stagShowFrame) {
					break;
				}
				parser.pos = parser.tagEnd;
			}

		}
	}
#ifdef _CELANIMATOR
	else {
		CallLimitError();
	}
#endif
}

int XSWFPlayer::GetMousePos(SObject *pObj, double &x, double &y)
{
	SPOINT pt;
	pt.x=gmousePt.x;
	pt.y=gmousePt.y;
	
	MATRIX mat,mat0;
	if(!pObj)//==XNULL||pObj->parent==XNULL)
		//mat=display.camera.mat;
		display.GetCameraMatrix(mat);
	else
		pObj->GetTotalMatrix(mat,XTRUE);
	//if(pObj&&pObj->parent)
	{
		//pObj->parent->GetTotalMatrix(mat,XTRUE);
		//mat0=mat;
		::MatrixInvert(&mat,&mat0);
		//::MatrixConcat(&mat,&mat0,&mat0);
		::MatrixTransformPoint(&mat0,&pt,&pt);

		//int v=0;
	}

	x=pt.x/20;
	y=pt.y/20;
	/*if(display.antialias)
	{
		x=pt.x>>2;
		y=pt.y>>2;
	}*/
	return mouseState;
}

void XSWFPlayer::StartDrag(SObject *pObject,SRECT &rect,XBOOL bCenter,XBOOL bRect)
{

	if (pObject)
	{
		dragObject = pObject;
		dragStarted = XTRUE;
        dragObject->puppet = XTRUE;
		dragPoint = gmousePt;
		dragObject->dragCenter = bCenter;
		if (bRect) {
			dragObject->dragConstraint=rect;
			//RectSet(x1*20, y1*20, x2*20, y2*20, &dragObject->dragConstraint);
		} else {
			RectSetEmpty(&dragObject->dragConstraint);
		}
	}
}

void XSWFPlayer::StopDrag()
{
	if (dragObject) {
		UpdateDropTarget();
		dragObject = NULL;
		dragStarted = XFALSE;
	}
}


//DEL void XSWFPlayer::RemoveThread(ScriptThread *thread)
//DEL {
//DEL 	/*TimeCaller**pp=&m_callList;
//DEL 	for(;;)
//DEL 	{
//DEL 		TimeCaller*p=*pp;
//DEL 		if(!p) break;
//DEL 		if()
//DEL 		{
//DEL 			delete *pp;
//DEL 			*pp=p->pNext;
//DEL 		}
//DEL 		else
//DEL 			pp=&p->pNext;
//DEL 	}*/
//DEL
//DEL }

XBOOL XSWFPlayer::RemoveCaller(XU32 id)
{
	TimeCaller**pp=&m_callList;
	for(;;)
	{
		TimeCaller*p=*pp;
		if(!p) break;
		if(p->nID==id)
		{
			//delete *pp;
			*pp=p->pNext;
			p->pNext=m_freeList;
			m_freeList=p;
			//delete p;
		}
		else
			pp=&p->pNext;
	}
	return XTRUE;
	/*for(XU32 i=0;i<m_callList.GetSize();i++)
	{
		if(m_callList[i]->nID==id)
		{
			m_freeList.Add(m_callList[i]);
			m_callList.RemoveAt(i);

			return XTRUE;
		}
	}
	return XFALSE;*/
}

/*_XSObject* XSWFPlayer::GetFocus()
{
	SObject* f=focus?focus:display.buttonThread;
	return f;
}*/



XU32 XSWFPlayer::AddCaller(XXObject*pBase,XXObject *pFunction,const XXVar&method, XU32 nTimer, XXVARLIST &list,int nType)
{
	TimeCaller*pCaller=new TimeCaller;
	if(pCaller==XNULL) return 0;
	pCaller->pArguments=XNULL;
	//if(strMethod)
//#if (__CORE_VERSION__>=0x02077000)
	pCaller->nInit=0;//;nTimer/32+1;
	//if(pCaller->nInit>200)
	//	pCaller->nInit=200;
//#endif
	pCaller->strMethodName=method;//strMethod;
	pCaller->nTimer=nTimer;
	pCaller->nLastCall=XTime::GetTimeMSec();
	pCaller->pFunction=pFunction->RefObject();
	pCaller->pBase=pBase->RefObject();
	pCaller->nType=nType;
	//pCaller->pContext=pDisp;
	pCaller->args.Append(list);
	list.Release();//RemoveAll();
	//[Note X]

	pCaller->nID=m_nCallTimerID;
	if(m_nCallTimerID<0xfffffffe)
		m_nCallTimerID++;
	else
		m_nCallTimerID=1;

	/*if(m_callList==XNULL)
		pCaller->nID=1;
	else
		pCaller->nID=m_callList->nID+1;*/
	pCaller->pNext=m_callList;
	m_callList=pCaller;
	//m_callList.Add(pCaller);
	//XU32 ID=m_callList.Add(pCaller);
	return pCaller->nID;
}


XU32 XSWFPlayer::AS3AddCaller(void*pFunction,XU32 nTimer,void*pArguments,int nType,void*pContext)
{
	//if(nType==3&&nTimer==10)
	//	int v=0;
	//if(nTimer==8000) return 0;
	//	nTimer=24000;
	TimeCaller*pCaller=new TimeCaller;
	if(pCaller==XNULL) return 0;
//#if (__CORE_VERSION__>=0x02077000)
	pCaller->nInit=0;//nTimer/32+1;
	//if(pCaller->nInit>200)
		pCaller->nInit=200;
//#endif
	pCaller->pArguments=pArguments;
	pCaller->pContext=pContext;
	//if(strMethod)
	pCaller->nTimer=nTimer;

	pCaller->nLastCall=XTime::GetTimeMSec();
	pCaller->pFunction=(XXObject*)pFunction;
	pCaller->pBase=(XXObject*)nType;

	//pCaller->pContext=pDisp;
	//pCaller->args.Append(list);
	//list.Release();//RemoveAll();
	//[Note X]
	pCaller->nID=m_nCallTimerID;
	if(m_nCallTimerID<0xfffffffe)
		m_nCallTimerID++;
	else
		m_nCallTimerID=1;
	/*if(m_callList==XNULL)
		pCaller->nID=1;
	else
		pCaller->nID=m_callList->nID+1;*/
//#if (__CORE_VERSION__>=0x02070200)
	TimeCaller**pLink=&m_callList;
	pCaller->pNext=NULL;
	for(;;)
	{
		TimeCaller*p=*pLink;
		if(p==NULL)
		{
			*pLink=pCaller;
			break;
		}
		pLink=&p->pNext;
	}
//#else
//	pCaller->pNext=m_callList;
//	m_callList=pCaller;
//#endif
	//m_callList.Add(pCaller);
	//XU32 ID=m_callList.Add(pCaller);
	return pCaller->nID;
}

void XSWFPlayer::FreeCalls()
	{
		TimeCaller*p=m_freeList;
		while(p)
		{
			TimeCaller*pNext=p->pNext;
			switch((int)p->pBase)
			{
			case 3:break;
			case 1:
			case 2:
				m_pAVM2->ReleaseInstance((void**)&p->pFunction,p->pContext,false,NULL);
				m_pAVM2->ReleaseInstance(&p->pArguments,p->pContext,false,NULL);
				break;
			default:
				p->pFunction->FreeObject();
				p->pBase->FreeObject();
				p->args.Release();//FreeAll();
				break;
			}
			delete p;
			p=pNext;
		}
		m_freeList=XNULL;
		//m_freeList.FreeAll();
	}
void XSWFPlayer::ReleaseCallers()
	{
		TimeCaller*p=m_callList;
		while(p)
		{
			TimeCaller*pNext=p->pNext;
			switch((int)p->pBase)
			{
			case 3:break;
			case 1:
			case 2:
				m_pAVM2->ReleaseInstance((void**)&p->pFunction,p->pContext,false,NULL);
				m_pAVM2->ReleaseInstance(&p->pArguments,p->pContext,false,NULL);
				break;
			default:
				p->pFunction->FreeObject();
				p->pBase->FreeObject();
				p->args.Release();//FreeAll();
			}
			delete p;
			p=pNext;
		}
		m_callList=XNULL;
		/*for(XU32 i=0;i<m_callList.GetSize();i++)
		{
			if(m_callList[i]->pFunction->IsObject(XXOBJ_FUNCTION))
				((XXObjectFunction*)m_callList[i]->pFunction)->
					Destroy();
		    else
				m_callList[i]->pFunction->Destroy();
			m_callList[i]->args.FreeAll();
			delete m_callList[i];
			//m_callList.RemoveAt(i);

		}
		m_callList.RemoveAll();*/
	}

SObject* XSWFCONTEXT::GetWithObject()
{
    if(!pWith)
        return XNULL;
    if(pWith->m_pObject)
        return pWith->m_pObject;
    return XNULL;
}

XBOOL	XSWFPlayer::RemoveLoader(const char*url)
{
		m_loadLock.Lock();
		_XLoadInfo**pRoot=&pLoader;
		for(;;)
		{
			_XLoadInfo*p=*pRoot;
			if(!p) break;
			if(XString8::Compare(url,p->strURL.strTxt,0,XTRUE)==0)
			{
				*pRoot=p->pNext;
				XReleaseINFO(p);
				m_loadLock.UnLock();
				return XTRUE;
			}
			pRoot=&p->pNext;
		}
		m_loadLock.UnLock();
		return XFALSE;
}

//void* _pDebugP=NULL;
//#if (__CORE_VERSION__>=0x02075000)
static XU32 _nLoadID=1;

bool XSWFPlayer::RemoveLoader(XU32 nID)
{
	bool bOK=false;
	m_loadLock.Lock();
	_XLoadInfo**pInfo=&this->pLoader;
	_XLoadInfo*pPrev=NULL;
	for(;;)
	{
		_XLoadInfo*p=*pInfo;
		if(!p) 
			break;
		if(p->nLoadID==nID)
		{
			*pInfo=p->pNext;
			if(pLastLoader==p)
				pLastLoader=pPrev;
			display.FreeLoad(p);
			bOK=true;
			break;
		}
		pPrev=p;
		pInfo=&p->pNext;
	}
	m_loadLock.UnLock();
	return bOK;
}
//#endif

_XLoadInfo* XSWFPlayer::AddLoader(const char*url,const char*target,
								   int nType,XXObject*pObj,EventDispatcherObject*pASObject,const char*strContext)
{
//#ifdef _WINEMU
//	if(strstr(url,"m.do")!=NULL)
//	{
//		int v=0;
//	}
//#endif
	//LOGWHEREMSG( url );
	XString8 strUrl;

	//if ( strstr( url, "resource/cn/swf/chat/chatui.swf" ) != NULL )
	//{
	//	int i = 0;
	//}

	if ( strstr(url, "file://{localpath}") != NULL )
	{
#ifndef _WIN32
		strUrl +="c:";
#endif
		strUrl += m_strWorkPath.strTxt;
		strUrl += (url + 19 );
	}
	else
	{
		strUrl = url;
	}
	//LOGWHEREMSG( strUrl.GetData() );
	m_loadLock.Lock();
	_XLoadInfo*pNew=display.CreateLoad();
	if(!pNew) 
	{
		m_loadLock.UnLock();
		return XNULL;
	}
//#if (__CORE_VERSION__>=0x02075000)
	pNew->nLoadID=_nLoadID;
	_nLoadID++;
	if(_nLoadID==0)
		_nLoadID=1;
//#endif
	//if(pASObject)
	//	pASObject->IncrementRef();
	//if(m_nLockOpt&&m_nLockOpt>40)
	pNew->pNext=NULL;
	{
		if(pWindow)
			pWindow->Handle(XCMD_UPDATECLICKOPT,0,0);
	}
	XString8 str;
	if(strContext&&strchr(strContext,'/')==XNULL)
	{
		str="application/";
		str+=strContext;
		strContext=str;
	}
	pNew->Init(-1,strUrl.GetData(),target,pObj,pASObject,nType,strContext);
	if(pASObject)
		m_pAVM2->PushObject(pASObject);
	if(!pLoader)
	{
		pLoader=pLastLoader=pNew;
	}
	else
	{
		pLastLoader->pNext=pNew;
		pLastLoader=pNew;
	}
	if(m_pDomFilter&&m_pDomFilter->bReplaceSWZ&&pNew->strURL.strTxt)
	{
		char*url=pNew->strURL.strTxt;
		//int ids=strchr(url,'.')-url;
		char*swz=strstr(url,".swz");
		if(swz)
		{
			if(swz[4]=='?'||swz[4]==0)
			{
				swz[3]='f';
			}
		}
	}
	if(m_pDomFilter&&m_pDomFilter->strReplaceLibs.GetSize())
	{
		XString8 url=pNew->strURL.strTxt;
		int i,nc=m_pDomFilter->strReplaceLibs.GetSize();
		url.MakeLower();
		for(i=0;i<nc;i+=2)
		{
			if(strstr(url,m_pDomFilter->strReplaceLibs[i])!=NULL)
			{
				pNew->strURL=m_pDomFilter->strReplaceLibs[i+1];
				pNew->strURL.ToString(XTRUE);
				break;
			}
			//f(XString8::Compare(url,m_pDomFilter->strReplaceLibs[i],0,XTRUE)
		}
	}
	m_loadLock.UnLock();
	return pNew;

}

void XSWFCONTEXT::MakeConstant(XPCTSTR pBuf, XU16 nCount)
{
	//constants.RemoveAll();
	//XPCTSTR pBuf=(XPCTSTR)stream.ReadAddr();
	//XPCTSTR pMax=pBuf+stream.GetDataSize();
	constants.SetSize(nCount);
	for(XU16 i=0;i<nCount;i++)
	{
		constants[i]=pBuf;
		//constants.Add(pBuf);
		int l=XString8::SafeStrlen(pBuf);
		pBuf+=l+1;
		//if(pBuf>=pMax) break;
	}
}

/*void XSWFPlayer::AddListener(XXObject *pObject, XU8 nMode)
{
	if(pObject==XNULL) return;
		XFUNLIST&list=m_listeners[nMode];//bKey?m_keyListeners:m_ptListeners;
		for(XU32 i=0;i<list.GetSize();i++)
			if(list[i]==pObject) return;
		list.Add((XXObject*)pObject->RefObject());
}*/

/*void XSWFPlayer::RemoveListener(XXObjectFunction *pObj, XU8 nMode)
{
		XFUNLIST&list=m_listeners[nMode];///bKey?m_keyListeners:m_ptListeners;
		for(XU32 i=0;i<list.GetSize();i++)
		{
			if(list[i]==pObj)
			{
				FREEOBJECT(list[i]);
				list.RemoveAt(i);
				return;
			}
		}
}*

void XSWFPlayer::ReleaseListeners()
{
		for(int j=0;j<=XLS_STAGE;j++)
		{
			for(XU32 i=0;i<m_listeners[j].GetSize();j++)
				FREEOBJECT(m_listeners[j][i]);
			m_listeners[j].RemoveAll();
		}
}*/

/*SObject* XSWFCONTEXT::GetWithThread()
{
	//XXObject*pObj=withs.GetLast();
	if(!pWith) return XFALSE;
	//if(!pObj) return XNULL;
	ScriptThread*pThread=pWith->GetTarget();
	if(pThread)
		return pThread->rootObject;
	return XNULL;
}*/

XBOOL XSWFPlayer::GetURL3(XSWFCONTEXT&cnt,char*baseURL,char*pTarget,XU8 method,XXObject*pObj,char*postData)
{
	//return XFALSE;
	//return XTRUE;
	/*if(cnt.urlCount>=XSWFCONTEXT::maxURLs) return XFALSE;
	int id=cnt.urlCount;
	cnt.actionVars[id]=method;
	cnt.actionURLs[id]=CreateStr(baseURL);
	cnt.actionObjs[id]=REFOBJECT(pObj);
	cnt.actionTargets[id]=CreateStr(pTarget);
	cnt.actionPostData[id]=CreateStr(postData);
	cnt.urlCount++;*/
	_XLoadInfo*pLoader=AddLoader(baseURL,pTarget,method,pObj);
	if(pLoader&&postData)
	{
		pLoader->postData.Append((XU8*)postData,XString8::SafeStrlen(postData));
	}
	return XTRUE;
}

XBOOL XSWFPlayer::GetURL2(XSWFCONTEXT &cnt, ScriptThread*thread,char* baseURL,char*target,
						  XU8 method)
{
		//return XFALSE;
		//if(cnt.urlCount>=XSWFCONTEXT::maxURLs) return XFALSE;
		//FreeStr(target);
		//return XFALSE;

		//int v=sizeof(XXObjectLoadVars);
		//pObj=XNULL;
		//cnt.actionVars[cnt.urlCount] = method;
		//cnt.actionObjs[cnt.urlCount] = XNULL;//(XXObjectEvent*)REFOBJECT(pObj);
		//cnt.actionPostData[cnt.urlCount] = thread?XNULL:CreateStr(postData);
		//if(pTarget)
		//XU8 nType=method;
		//XXVar targetStr=target;
		//XXVar urlStr=baseURL;
	    _XLoadInfo*pInfo=AddLoader(baseURL,target,method);
		//cnt.actionTargets[cnt.urlCount]= CreateStr(target);//*pTarget;
		if (method & kHttpLoadVariables)
			method &= ~kHttpLoadVariables;

		if (method & kHttpLoadTarget)
        {
            //+
            // we are loading into a target
            //-

			method &= ~kHttpLoadTarget;

            // see if target resolves to a _levelXX
            //char            *targetTmp = target;
            BOOL            loadTarget = XTRUE;
    		ScriptThread*   targetThread = thread;

			if(!target)
			{
				loadTarget=XFALSE;
				method &= ~kHttpLoadTarget;
			}
			else
			{
				//if(pTarget->IsObject(XXOBJ_CLIP))
				//	targetThread=pTarget->pObject->GetTarget();
				//else
				{
					//pTarget->ToString();
					char*targetTmp=target;//pTarget->strData;
					//char*target=targetTmp;
					if (targetTmp&&targetTmp[0])
					{
						// find the target we want
						SObject*obj = FindTarget(thread ? thread->rootObject : player->rootObject,target,cnt.GetWithObject());
						targetThread=obj?obj->thread:XNULL;
					}

					// see if target resolves to a _levelXX
					if (LayerNum(targetTmp) >= 0)
					{
						// skip past the _levelxx or _levelxx/ string
						for (; *targetTmp != 0; targetTmp += 1)
						{
							if (*targetTmp == '/')
							{
								targetTmp += 1;
								break;
							}
						}

						if (*targetTmp == 0)
						{
							// we are dealing _level not target
                			method &= ~kHttpLoadTarget;
							loadTarget = XFALSE;
						}
					}
					else if (target && target[0] == '/' && target[1] == 0)
					{
						// we are dealing _level not target
						method &= ~kHttpLoadTarget;
						loadTarget = XFALSE;

						if (targetThread && targetThread->layerDepth > 0)
						{
							//cnt.actionTargets[cnt.urlCount]=targetThread->rootObject->pObject;
							// will resolve to "_levelXX/" or "/"

							//char *fullPath =

							XXVar var;
							if (GetTargetPath(targetThread->rootObject,var))//;fullPath)
							{
								pInfo->strURL=var;
								//FreeStr(cnt.actionTargets[cnt.urlCount]);
								//cnt.actionTargets[cnt.urlCount] = CreateStr(var.strTxt);//fullPath;
							}
						}
						else
						{
							// special case: root of main movie
							//FreeStr(cnt.actionTargets[cnt.urlCount]);
							pInfo->strTarget="_level0";//NULL;
							//cnt.actionTargets[cnt.urlCount] = CreateStr("_level0");
						}
					}
				}
			}

        }

        if (method == kHttpSendUseGet) {

                // Send the variables with a GET
//			if(!postData&&!pObj)
			{
				URLBuilder ub;
				ub.AppendString(baseURL);
				XXObject*pObj=thread->GetClipObject();
                // thread could have been deleted
				if ( thread && pObj && pObj->GetVarCount() ) {
					if (IsMachineNameURL(baseURL)) {
						ub.AppendChar('/');
					}
					ub.AppendChar('?');
					ub.EncodeVariables(cnt,pObj);
				}
				pInfo->strURL=ub.buf;
				pInfo->strURL.ToString(XTRUE);
				//cnt.actionURLs[cnt.urlCount] = CreateStr(ub.buf);

                //cnt.actionPostData[cnt.urlCount] = 0;
			}
		} else if (method == kHttpSendUsePost) {

            // Send the variables with a POST
//			if(!postData&&!pObj)
			{
				//urlStr=
				//cnt.actionURLs[cnt.urlCount] = CreateStr(baseURL);
				XXObject*pObj=thread->GetClipObject();
				// thread could have been deleted
				if (thread&&pObj&&pObj->GetVarCount()) {
					URLBuilder ub;
					ub.EncodeVariables(cnt,pObj);
					pInfo->postData.Append((XU8*)ub.buf,ub.len);

					//cnt.actionPostData[cnt.urlCount] = CreateStr(ub.buf);
				}
    			//else
				//	cnt.actionPostData[cnt.urlCount] = 0;
			}
		} else {
			/* normal fetch */
			//cnt.actionURLs[cnt.urlCount] = CreateStr(baseURL);
//			if(!postData&&!pObj)
				//cnt.actionPostData[cnt.urlCount] = 0;
		}

		//cnt.urlCount++;
		return XTRUE;
		//FreeStr(baseURL);
}


ScriptThread* XSWFPlayer::ThreadOf(XSWFCONTEXT &cnt, SObject *thisObject, XXVar *pVar,ScriptThread*thisThread)
{
	if(pVar->IsObject(XXOBJ_CLIP))
	{
		SObject*p=((XXObjectClip*)pVar->pObject)->m_pObject;
		if(p) return p->thread;
		else return thisThread;
	}
	if(pVar->IsString2())
	{

		pVar->ToString(XFALSE);
		if(pVar->strTxt[0])
		{
			if(thisObject==XNULL)
			{
				switch(pVar->strTxt[0])
				{
				case '/':
					thisObject=cnt.thread->m_pRoot->rootObject;
					break;
				case '_':
					if(SkipPrefix(pVar->strTxt,"_level")==XNULL)
						return thisThread;
					thisObject=cnt.thread->m_pRoot->rootObject;
					break;
				default:
					return thisThread;
				}
				//return
				//	cnt.thread->m_pRoot;
			}
			SObject*obj=FindTarget(thisObject, pVar->strTxt,cnt.GetWithObject());
			if(obj)
			{
				return obj->thread;
			}
			else
			{
				XXVar var;
				if(GetVariable(cnt,pVar->strTxt,var,XFALSE)&&
					(var.IsObject(XXOBJ_CLIP)||var.IsObject(XXOBJ_BUTTON)))
				{
					//if(var.pObject->m_pObject)
					return CLIPTHREAD(var.pObject);
				}
				//	return var.pObject->m_pObject->thread;
			}
			return XNULL;
			/*XXVar var;
			if(GetVariable(cnt,pVar->strData,var)&&var.IsObject())
			{
				if(var.pObject->m_pObject)
				{
					return var.pObject->m_pObject->thread;
				}
				//if(obj) return obj->thread;
			}*/
		}
		else
		{
			return thisObject?thisObject->thread:thisThread;
		}
			//return thisObject->thread;
	}
	return thisThread;
}

SObject* XSWFPlayer::ObjectOf(XSWFCONTEXT &cnt, SObject *thisObject, XXVar *pVar,ScriptThread*thisThread)
{
	if(pVar->IsObject(XXOBJ_CLIP))
	{
		SObject*p=((XXObjectClip*)pVar->pObject)->m_pObject;
		if(p) return p;
		else return thisObject;
	}
	if(pVar->IsString2())
	{

		pVar->ToString(XFALSE);
		if(pVar->strTxt[0])
		{
			if(thisObject==XNULL)
			{
				switch(pVar->strTxt[0])
				{
				case '/':
					thisObject=cnt.thread->m_pRoot->rootObject;
					break;
				case '_':
					if(SkipPrefix(pVar->strTxt,"_level")==XNULL)
					{
						if(SkipPrefix(pVar->strTxt,"_root")==XNULL)
							return cnt.thread->m_pRoot->rootObject;
					}
						//thisThread->rootObject;
					thisObject=cnt.thread->m_pRoot->rootObject;
					break;
				default:
					return thisThread->rootObject;
				}
				//return
				//	cnt.thread->m_pRoot;
			}
			SObject*obj=FindTarget(thisObject, pVar->strTxt,cnt.GetWithObject());
			if(obj)
			{
				return obj;
			}
			else
			{
				XXVar var;
				if(GetVariable(cnt,pVar->strTxt,var,XFALSE)&&
					(var.IsObject(XXOBJ_CLIP)||var.IsObject(XXOBJ_BUTTON)))
				{
					//if(var.pObject->m_pObject)
					return var.pObject->m_pObject;
					//return CLIPTHREAD(var.pObject);
				}
				//	return var.pObject->m_pObject->thread;
			}
			return XNULL;
			/*XXVar var;
			if(GetVariable(cnt,pVar->strData,var)&&var.IsObject())
			{
				if(var.pObject->m_pObject)
				{
					return var.pObject->m_pObject->thread;
				}
				//if(obj) return obj->thread;
			}*/
		}
		else
		{
			return thisObject?thisObject:thisThread->rootObject;
		}
			//return thisObject->thread;
	}
	return thisObject;
}

void XSWFPlayer::ShowCursor(XBOOL bShow)
{
	cursorShow=bShow;
}

//const XPCTSTR _strScaleModes[]=
//{	"showAll","noBorder","exactFit","noScale","fitDev","fit50","fit100","fitFull","autoFit"};

XPCTSTR XSWFPlayer::GetScaleMode(int sm)
{
	return "showAll";
	//if(sm<0)
	//	sm=scaleMode&DisplayList::scaleMask;
	//switch(sm)
	//{
	//case DisplayList::showAll:
	//case DisplayList::noBorder:
	//case DisplayList::exactFit:
	//case DisplayList::noScale:
	//	 return _strScaleModes[sm];
	//	 break;
	//case DisplayList::autoFit:
	//	 return _strScaleModes[sm];
	//default:
	//	return _strScaleModes[DisplayList::exactFit];
	//}
}

void XSWFPlayer::SetScaleMode(XPCTSTR strMode)
{
	/*for(int i=0;i<4;i++)
	{
		if(strcmp(strMode,_strScaleModes[i])==0)
		{
			scaleMode&=~DisplayList::scaleMask;
			scaleMode|=i;
			SetCamera(updateLazy);
			return;
		}
	}*/
	return;
}

void XSWFPlayer::ShowMenu(XBOOL bShow)
{
	menuShow=bShow;
}

int XSWFPlayer::GetPecentBase()
{
	int fw=RectWidth(&player->frame);
	int fh=RectHeight(&player->frame);
	if(fw<=0||fh<=0) 
		return 100;
	int fx=this->m_winSize.cx*2000/fw;
	int fy=this->m_winSize.cy*2000/fh;
	return XMIN(fx,fy);
}


int XSWFPlayer::GetStageWidth()
{
	int fw=RectWidth(&player->frame)/20;
	if(m_pDomFilter)
	{
		if(fw<m_pDomFilter->nFrameWidth)
			fw=m_pDomFilter->nFrameWidth;
	}
	else if(m_nFrameWidth)
		fw=m_nFrameWidth;
	return fw;
}

int XSWFPlayer::GetStageHeight()
{
	int fh=RectHeight(&player->frame)/20;
	if(m_pDomFilter)
	{
		if(fh<m_pDomFilter->nFrameHeight)
			fh=m_pDomFilter->nFrameHeight;
	}
	else if(m_nFrameHeight)
		fh=m_nFrameHeight;
	return fh;
}

XBOOL XSWFPlayer::HandleListener(XU32 type)
{
	if(m_pAVM2) return false;
	ScriptPlayer*p=player;//(XXObjectClip*)player->rootObject->pObject;
	XBOOL bOK=XFALSE;
	while(p)
	{
		XXObjectCreator*pGlobal=p->m_pGlobal;
		if(pGlobal)
		{
			switch(type)
			{
			case XOBJEVENT_MOUSEMOVE:
			case XOBJEVENT_MOUSEDOWN:
			case XOBJEVENT_MOUSEUP:
				 if(pGlobal->m_pMouse)
					bOK=pGlobal->m_pMouse->HandlerEvent(this,CLIPOFTHREAD(p),type,XNULL);
				 break;
			case XOBJEVENT_KEYUP:
			case XOBJEVENT_KEYDOWN:
				 if(pGlobal->m_pKey)
				 {
					 XU8 vars[sizeof(XXVar)*2];
					 XXVARLIST list(vars,2);
					 list.Push(nLastKey);
					 bOK=pGlobal->m_pKey->HandlerEvent(this,CLIPOFTHREAD(p),type,&list);
					 list.Release();
					 //list.FreeAll();
				 }
				 break;
			}
		}
		p=p->nextLayer;
	}
	return bOK;
}

XBOOL XSWFPlayer::SetFocus(SObject *hitBtn)
{
	if(hitBtn==display.button) return XFALSE;
	//if(m_pFocus==hitBtn) return XFALSE;
	//m_pFocus=hitBtn;
	XBOOL bOK=XFALSE;
	if(display.button)
	{
		//SObject*pNew=hitClip?hitClip:hitBtn;
		XXVar arg(OBJECTOFS(hitBtn));
		if(display.button->OnEvent(0,this,XOBJEVENT_KILLFOCUS,&arg))
				bOK=XTRUE;
	}
	if(hitBtn)
	{
			SObject*pOld=display.button;
			XXVar arg(OBJECTOFS(pOld));
			if(hitBtn->OnEvent(0,this,XOBJEVENT_KILLFOCUS,&arg))
				bOK=XTRUE;
	}
	/*if(hitClip)
	{
		if(hitClip!=display.button[1])
		{
			SObject*pOld=display.button[1]?display.button[1]:display.button[0];
			if(hitClip->OnEvent(this,XOBJEVENT_KILLFOCUS,&XXVar(OBJECTOFS(pOld))))
				bOK=XTRUE;
		}
	}
	if(hitBtn)
	{
		if(hitBtn!=display.button[0])
		{
			SObject*pOld=display.button[0]?display.button[0]:display.button[1];
			if(hitBtn->OnEvent(this,XOBJEVENT_KILLFOCUS,&XXVar(OBJECTOFS(pOld))))
				bOK=XTRUE;
		}
	}*/
	return bOK;
	/*SObject* oldFocus=focus?focus:display.buttonThread;
	SObject* newFocus=hitBtn?hitBtn:hitClip;
	if(oldFocus!=newFocus)
	{
		if(oldFocus)
		{
			if(oldFocus->OnEvent(this,XOBJEVENT_KILLFOCUS,&XXVar(OBJECTOFS(newFocus))))
				bAction=XTRUE;
		}
		if(newFocus)
		{
			if(newFocus->OnEvent(this,XOBJEVENT_SETFOCUS,&XXVar(OBJECTOFS(oldFocus))))
				bAction=XTRUE;
		}
	}*/
}

void XSWFPlayer::OnMouseMove(int x,int y,int iPos,int s)
{
	//return;

	if(m_eventLock.Lock(XWAIT_MONMENT))
	{
		XEVENT*pEvent=m_pEvents;
		XEVENT*pMoveEvent=XNULL;
		while(pEvent)
		{
			switch(pEvent->type)
			{
			case XMOUSEMOVE:
				 pMoveEvent=pEvent;
				 break;
			case XMOUSECLICK:
			case XMOUSEDOUBLECLICK:
			case XMOUSEUP:
			case XMOUSEDOWN:
				 pMoveEvent=XNULL;
				 break;
			}
			pEvent=pEvent->pNext;
		}
		if(pMoveEvent)
		{
			pMoveEvent->x=x;
			pMoveEvent->y=y;
			//pMoveEvent->iPos=0;
			pMoveEvent->state=s;
			m_nLoopCount++;
			pMoveEvent->nTime=m_nLoopCount;
			m_eventLock.UnLock();
			return;
		}
		m_eventLock.UnLock();
	}

	AddEvent(XMOUSEMOVE,x,y,s|(iPos<<16));

}

XBOOL XSWFPlayer::ModifyEvent(int type,int x,int y,int s)
{
	if(!m_eventLock.Lock(XWAIT_MONMENT))
	{
		//delete pEvent;
		//eventAlloc.Free(pEvent);
		return XFALSE;
	}
	XEVENT*pEvent=m_pEvents;
	XBOOL bOK=XFALSE;
	while(pEvent)
	{
		if(pEvent->type==type&&
		   pEvent->x==x)
		{
			bOK=XTRUE;
			pEvent->state=s;
		}
		pEvent=pEvent->pNext;
	}
	m_eventLock.UnLock();
	return bOK;
}

XBOOL XSWFPlayer::AddEvent(int type, int x, int y,int s,int t,bool bFirst)
{
	//return XTRUE;
	//if(type==XAS3EVENT)
	//{
	//	ScriptObject*pObj=(ScriptObject*)y;
	//	//if(!pObj->vtable)
	//		int v=0;
	//	//int v=0;
	//}
	//XLock lock(&m_eventLock);       // modify by hgl
	if(!m_eventLock.Lock(XWAIT_MONMENT))
	{
		//delete pEvent;
		//eventAlloc.Free(pEvent);
		return XFALSE;
	}
	XEVENT*pEvent=(XEVENT*)eventAlloc.Alloc();
		//new XEVENT;
	if(!pEvent) 
	{
		m_eventLock.UnLock();
		return XFALSE;
	}
	pEvent->pNext=XNULL;
	pEvent->type=type;
	pEvent->x=x;
	pEvent->y=y;
	m_nLoopCount++;
	pEvent->nTime=t?t:m_nLoopCount;

	pEvent->state=s;
	if(bFirst)
	{
		pEvent->pNext = m_pEvents;
		m_pEvents = pEvent;
	}
	else
	{
		XEVENT**pp=&m_pEvents;
		for(;;)
		{
			XEVENT*p=*pp;
			if(!p)
			{
				*pp=pEvent;
				break;
			}
			else
				pp=&p->pNext;
		}
	}
	m_eventLock.UnLock();
	return XTRUE;
}


#ifdef _ANALY
	bool _isExist(AS3PROPITEM*root,Stringp str)
	{
		StUTF16String s(str);
		while(root)
		{

#ifdef __ANDROID__
			if (gg_wcscmp(s.c_str(), root->name)==0)
				return true;
#elif defined(__APPLE__)
            if (gg_wcscmp(s.c_str(), root->name)==0)
				return true;
#else
			if(::wcscmp(s.c_str(),root->name)==0)
				return true;
#endif
			root=root->next;
		}
		return false;
	}

	static AS3PROPITEM* _CreateItem(AvmCore*core,Stringp name,Atom atom,int type,const char* strName=NULL)
	{
		AS3PROPITEM* item=new AS3PROPITEM;
		if(name)
		{
			StUTF16String sn(name);
			item->name=sn.c_str();
		}
		else
		{
			item->name=XString16(strName);
		}
		item->nData=atom;

		Stringp s=core->string(atom);
		StUTF16String sv(s);
		item->value=sv.c_str();

		item->next=NULL;

		if(type>=0)
			item->nType=type;
		else
		{
			switch(atomKind(atom))
			{
			case kObjectType:
				 item->nType=AS3PROPITEM::OBJECT;break;
			case kStringType:
				 item->nType=AS3PROPITEM::STRING;break;
			case kNamespaceType:
				 item->nType=AS3PROPITEM::NAMESPACE;break;
			default:
			case kSpecialType:
				 item->nType=AS3PROPITEM::OTHER;break;
			case kBooleanType:
				 item->nType=AS3PROPITEM::LOGIC;break;
			case kIntptrType:
			case kDoubleType:
				 item->nType=AS3PROPITEM::NUMBER;break;
			}
		}
		return item;
	}



	AS3PROPITEM* _GetChildrenT(AS3PROPITEM*root,ScriptObject*obj,AvmCore*core,VTable*vtable,bool bSuper)
	{
		//AS3PROPITEM*item=root;
		TraitsBindingsp b=vtable->traits->getTraitsBindings();
		int index=0,iSet=0;
		for(;;)
		{
			index=b->next(index);
			if(index<=0) break;
			Stringp str=b->keyAt(index);
			if(bSuper&&_isExist(root,str)) str=NULL;
			if(str)
			{
				Binding bd=b->valueAt(index);
				if(bd==BIND_NONE) continue;
				Atom atom=0;
				switch (AvmCore::bindingKind(bd))
				{
					case BKIND_METHOD:
						 break;
					case BKIND_VAR:
					case BKIND_CONST:
						 {
						   atom=obj->getSlotAtom(AvmCore::bindingToSlotId(bd));
						   AS3PROPITEM* newItem=_CreateItem(core,str,atom,-1);
						   ADDITEM(newItem);
						 }
						 break;
					case BKIND_GET:
					case BKIND_GETSET:
						{
							int m = AvmCore::bindingToGetterId(bd);
							MethodEnv *f = vtable->methods[m];//vtable->method[m];
							Atom atomv=obj->atom();
							atom = f->coerceEnter(atomv);
							AS3PROPITEM*newItem=_CreateItem(core,str,atom,-1);
							ADDITEM(newItem);
						}break;

				}
			}
		}
		if(obj->vtable->traits->getHashtableOffset()!=0)
		{
			InlineHashtable*b=obj->getTable();
			int index=0;
			for(;;)
			{
				index=b->next(index);
				if(index<=0) break;
				Stringp str=core->string(b->keyAt(index));
				if(bSuper&&_isExist(root,str)) str=NULL;
				if(!str) continue;

				Atom atom=b->valueAt(index);
				AS3PROPITEM*newItem=_CreateItem(core,str,atom,-1);
				ADDITEM(newItem);
			}
		}
		if(ShellCore::IsTypeClass(obj,obj->toplevel()->arrayClass))
		{
			ArrayObject*ar=(ArrayObject*)obj;
			int nSize=ar->getDenseLength();//get_length();
			for(int i=0;i<nSize;i++)
			{
				Atom atom=ar->getIntProperty(i);
				XXVar name("[");
				name.StringAdd(XXVar(i));
				name.StringAdd("(");
				XString8 ss;
				ss.FromInt((int)atom,XTRUE);
				name.StringAdd(ss);
				name.StringAdd(")]");
				AS3PROPITEM*newItem=_CreateItem(core,NULL,atom,-1,name.strTxt);
				ADDITEM(newItem);
			}
		}
		return root;
	}

	AS3PROPITEM* _GetChildren(AvmCore*core,ScriptObject*obj,bool bSuper)
	{
		AS3PROPITEM*root=NULL;
		VTable*vtable=obj->vtable;
		while(vtable)
		{
			root=_GetChildrenT(root,obj,core,vtable,bSuper);
			if(!bSuper) break;
			vtable=vtable->base;
		}
		return root;
	}
#endif

XBOOL XSWFPlayer::OnAS3Event(void*pTarget,void*pEvent,void*pt)
{
	m_pAVM2->PushObject(pTarget);
	m_pAVM2->PushObject(pEvent);
	AddEvent(XAS3EVENT,(int)pTarget,(int)pEvent,(int)pt);
	return XTRUE;
}

XBOOL XSWFPlayer::RemoveEvent(int type,int x,int y,int s)
{
	XBOOL bRemove=XFALSE;
	//XLock lock(&m_eventLock);           // modify by hgl
	if(!m_eventLock.Lock(XWAIT_MONMENT))
		return bRemove;
	XEVENT**pRoot=&m_pEvents;
	for(;;)
	{
		XEVENT*pEvent=*pRoot;
		if(!pEvent) break;
		if(pEvent->type==type&&
		   pEvent->x==x&&
		   pEvent->y==y&&
		   pEvent->state==s)
		{
			*pRoot=pEvent->pNext;
			eventAlloc.Free(pEvent);
			//delete pEvent;
		}
		else
			pRoot=&pEvent->pNext;
	}
	m_eventLock.UnLock();
	return bRemove;
}



bool XSWFPlayer::DoEvent(bool& bMouse)
{
	m_pUpObject=NULL;
	m_pUpRemovedObject=NULL;
	bool bHitNone=false;
	XEVENT*pEvents=XNULL;
	//XLock lock(&m_eventLock);        // modify by hgl
	if(!m_eventLock.Lock(XWAIT_MONMENT))
		return bHitNone;
	pEvents=m_pEvents;
	m_pEvents=XNULL;
	m_eventLock.UnLock();

	while(pEvents)
	{
		XEVENT*pNext=pEvents->pNext;
		switch(pEvents->type)
		{
		case XAS3EVENT:
			/*if(this->m_bNoScript)
			{
				AddEvent(pEvents->type,pEvents->x,pEvents->y,pEvents->state,pEvents->nTime);
			}
			else*/
			{
				//m_pAVM2->
				//m_bEnterFrame=XTRUE;
				EventObject*pEvent=(EventObject*)pEvents->y;
				ScriptObject*pTarget=pEvent->pTarget;
				pEvent->pTarget=(ScriptObject*)pEvents->state;
				m_pAVM2->DispatchEvent((void*)pEvents->x,(void*)pEvents->y);
				pEvent->pTarget=pTarget;
				m_pAVM2->RemoveObject((void*)pEvents->x);
				m_pAVM2->RemoveObject((void*)pEvents->y);
				//m_bEnterFrame=XFALSE;
			}break;
		case XMOVIECLIP0:
			/*if(this->m_bNoScript)
			{
				AddEvent(pEvents->type,pEvents->x,pEvents->y,pEvents->state,pEvents->nTime);
			}
			else*/
			{
				_XSObject*obj=(_XSObject*)pEvents->x;
				m_pAVM2->RemoveObject(obj->pASObject);
				if(obj->thread&&obj->thread->m_bAS3Frame0)
				{
					obj->thread->m_bAS3Frame0=0;
					obj->thread->m_bNeedCallFrame=false;
					obj->CallFrame(obj->thread->curFrame);
					if(pEvents->state==1)
						obj->thread->playing=true;
					else if(pEvents->state==2)
						obj->thread->playing=false;
				}
			}
			break;
		case XACTIVITY:
			/*if(this->m_bNoScript)
			{
				AddEvent(pEvents->type,pEvents->x,pEvents->y,pEvents->state,pEvents->nTime);
			}
			else*/
			{
				//EventDispatcherObject*pObj=(EventDispatch
				EventDispatcherObject* obj=(EventDispatcherObject*)pEvents->x;
				obj->OnActivityEvent(pEvents->state!=0);
				//obj->PlayEnd(pEvents->state!=0);
				//m_pAVM2->OnEvent(XAS3_EVENT_soundComplete,false,(void*)pEvents->x,(void*)pEvents->y,pEvents->);
				m_pAVM2->RemoveObject(obj);
			}
			break;
		case XSOUNDCOMPLETE:
			/*if(this->m_bNoScript)
			{
				AddEvent(pEvents->type,pEvents->x,pEvents->y,pEvents->state,pEvents->nTime);
			}
			else*/
			{
				//EventDispatcherObject*pObj=(EventDispatch
				SoundChannelObject* obj=(SoundChannelObject*)pEvents->x;
				obj->PlayEnd(pEvents->state!=0);
				//m_pAVM2->OnEvent(XAS3_EVENT_soundComplete,false,(void*)pEvents->x,(void*)pEvents->y,pEvents->);
				m_pAVM2->RemoveObject(obj);
			}break;
		case XACTIVE:
			 this->Activate();
			 break;
		case XDEACTIVE:
			 this->Deactivate();
			 break;
		case XMOUSECLICK:
			 MouseClick(pEvents->nTime,pEvents->x*20,pEvents->y*20);
			 bHitNone=false;
			 bMouse=true;
			 break;
		case XMOUSEDOUBLECLICK:
			 MouseDoubleClick(pEvents->nTime,pEvents->x* 20,pEvents->y* 20);
			 bHitNone=false;
			 bMouse=true;
			 break;
		case XGETCLASSTREE:
			{
			//add by zhangchen because m_getEvent.SetEvent can't be compiled
				XString8*pStr=(XString8*)pEvents->x;
				m_pAVM2->GetClassTree(*pStr,(void*)pEvents->y);
#ifdef _ANALY
				m_getEvent.SetEvent();
#endif
			}break;
		case XFINDCLASS:
			if(m_pAVM2)
			{
#ifdef _ANALY
				if(pEvents->y==0)
					m_pRetObject=m_pAVM2->FindClass((const char*)pEvents->x,NULL);
				else
				{
					ScriptObject*obj=(ScriptObject*)pEvents->y;
					Stringp name=m_pAVM2->GetCore()->internStringUTF8((const char*)pEvents->x);
					m_pRetObject=AvmCore::atomToScriptObject(obj->getStringProperty(name));
				}
				m_getEvent.SetEvent();
#endif
			}break;
#ifdef _ANALY
		case XSEEKITEM:
			SeekItem(m_pSeekItem);
			m_getEvent.SetEvent();
			break;
		case XGETDIBINFO:
			GetDibInfo(m_pReturnInfo);
			m_getEvent.SetEvent();
			break;
		case XGETGLDIB:
			GetGLDib(m_pReturnInfo);
			m_getEvent.SetEvent();
			break;
		case XGETMEMINFO:
			GetMemInfo(m_pMemInfo);
			m_getEvent.SetEvent();
			break;
#endif
		case XGETCHILDREN:
			if(m_pAVM2)
			{
#ifdef _ANALY

				EventDispatcherClass*pClass=m_pAVM2->GetToplevel()->getEventDispatcherClass();
				//if(ShellCore::IsProtoTypeOf(pClass,(ScriptObject*)pEvents->x))
				if(ShellCore::IsTypeClass((ScriptObject*)pEvents->x,pClass))
				{
					m_pReturnItem=((EventDispatcherObject*)pEvents->x)->GetChildren();
					if(pEvents->y)
					{
						ScriptObject*obj=(ScriptObject*)pEvents->x;
						VTable*vtable=obj->vtable->base;
						while(vtable)
						{
							m_pReturnItem=_GetChildrenT(m_pReturnItem,obj,m_pAVM2->GetCore(),vtable,true);
							vtable=vtable->base;
						}
					}
				}
				else
				{
					m_pReturnItem=_GetChildren(m_pAVM2->GetCore(),(ScriptObject*)pEvents->x,pEvents->y!=0);
				}
				m_getEvent.SetEvent();
#endif
			};break;
		case XMOUSEDOWN:
			 MouseDown(pEvents->nTime,pEvents->x * 20,pEvents->y * 20);
			 bMouse=true;
			 if(m_eventLock.Lock(XWAIT_MONMENT))
			 {
				 //Need ENTER_FRAME bethween MOUSE_DOWN and MOUSE_UP
				 eventAlloc.Free(pEvents);
				 if(pNext)
				 {
					 if(m_pEvents)
					 {
						XEVENT**pRoot=&pNext->pNext;
						for(;;)
						{
							XEVENT*p=*pRoot;
							if(p==NULL)
							{
								*pRoot=m_pEvents;
								break;
							}
							pRoot=&p->pNext;
						}
					 }
					 m_pEvents=pNext;
				 }
				 m_eventLock.UnLock();
				 m_bNeedEnter=XTRUE;
				 return bHitNone;
			 }
			 break;
		case XMOUSEUP:
			 MouseUp(pEvents->nTime,pEvents->x * 20,pEvents->y * 20,(pEvents->state&0x80000000)!=0);
			 bHitNone=false;
			 bMouse=true;

			 break;
		case XMOUSEMOVE:
			MouseMove(pEvents->nTime,pEvents->x * 20,pEvents->y * 20,(pEvents->state&1),(pEvents->state&0x4000)!=0);
			 bMouse=true;
			 break;
		case XSTRING:
			 if(focus&&focus->IsEditText())//focus->character->type==editTextChar)
				focus->editText->SetBuffer((XPCWSTR)pEvents->x,WideStrLen((XPCWSTR)pEvents->x),true);
			break;
		case XCHAR:
			 HandleKeyDown(pEvents->nTime,pEvents->x,pEvents->y,pEvents->state,0,XTRUE);
			 bMouse=true;
			 break;
		case XKEYDOWN:
			 HandleKeyDown(pEvents->nTime,pEvents->x,pEvents->y,pEvents->state,0);
			 bMouse=true;
			 break;
		case XKEYUP:
			 HandleKeyUp(pEvents->nTime,pEvents->x,pEvents->y,pEvents->state,0);
			 bMouse=true;
			 break;
		}
		//delete pEvents;
		if(m_eventLock.Lock(XWAIT_MONMENT))
		{
			eventAlloc.Free(pEvents);
			m_eventLock.UnLock();
		}
		pEvents=pNext;
	}
	return bHitNone;
//	_ANALYLEAVE();
}

void XSWFPlayer::CalcLimit()
{
	SRECT viewRect, docRect;
	ClientRect(viewRect);
	int w=RectWidth(&viewRect);
	int h=RectHeight(&viewRect);

	MATRIX mat;
	display.GetCameraMatrix(mat);
	//MATRIX&mat=display.camera.mat;

	m_fScaleX=display.mControlMat.a;//(double)w/m_winSize.cx;
	m_fScaleY=display.mControlMat.d;//
		//65536.0/(m.a);
	//m_fScaleY=(double)h/m_winSize.cy;
		//65536.0/(m.d);

	m_nStageWidth=w;
	m_nStageHeight=h;
	MatrixTransformRect(&mat, &player->frame, &docRect);
	//MatrixTransformRect(&invMat,
	m_nLimitX=RectWidth(&docRect)-w*20;
	//MATRIX&cm=display.camera.mat;
	if(mat.tx>display.mControlMat.tx)
		m_nLimitX-=(display.mControlMat.tx-mat.tx)*2;
	//if(cm.tx>0)
	
	if(m_nLimitX<0) m_nLimitX = 0;
	m_nLimitY=RectHeight(&docRect)-h*20;
	if(mat.ty>display.mControlMat.ty)
		m_nLimitY-=(display.mControlMat.ty-mat.ty)*2;
		//docRect.xmax-viewRect.xmax;
	
	if(m_nLimitY<0) m_nLimitY = 0;
}

void XSWFPlayer::SetDevSize(XSIZE &sz,int nZoom)
{
	//m_winSize=sz;
	//if(nZoom>=0)
	//	SetZoom(nZoom,sz);
	this->m_winSize=sz;

	CalcLimit();
		//docRect.ymax-viewRect.ymax;
}

void XSWFPlayer::ChangeMode(int w,int h)
{
	if(!pWindow) 
		return;
    if(w > h)
        scrOrien = SCRENNRight;
    else
        scrOrien = SCRENNPortrait;
	if ( (w < h && m_winSize.cx > m_winSize.cy) ||(w > h && m_winSize.cx < m_winSize.cy) )
	{
//		XS32 temp;
//		temp = m_winSize.cx;
//		m_winSize.cx = m_winSize.cy;
//		m_winSize.cy = temp;
        bRotaionEnd = false;
		pWindow->PostMsg(XCMD_FLASHMODECMD,-1,0,XMS_TOPMOST);
	}
	else
		display.SetCamera(&player->frame);
}

bool XSWFPlayer::NeedOriginal()
{
	int width = GetStageWidth();
	int height = GetStageHeight();
	return width>m_winSize.cx||height>m_winSize.cy;
}


void XSWFPlayer::SetAutoFit(int w,int h)
{
	//scaleMode = DisplayList::noBorder;
	bool bSet = false;
	if(w<=0&&h<=0)
	{
		bSet = true;		
	}
	if(w<=0)
		w=RectWidth(&player->frame);
	if(h<=0)
		h=RectHeight(&player->frame);
	bool bS1 = w>h;
	bool bS2 = m_winSize.cx>m_winSize.cy;
	if(bS1==bS2||!bSet)
	{
		m_nFrameWidth = m_winSize.cx;
		m_nFrameHeight = m_winSize.cy;
	}
	else
	{
		m_nFrameWidth = m_winSize.cy;
		m_nFrameHeight = m_winSize.cx;
		if(bSet)
			pWindow->PostMsg(XCMD_FLASHMODECMD,-1,0,XMS_TOPMOST);
        bSet = false;
	}
	if(bSet)
		pWindow->Handle(XCMD_FLASHSHOWMODE,0,0);
}

void XSWFPlayer::UpdateConfig()
{
	if(this->m_pDomFilter)
	{
		//if(m_pDomFilter->nQuality)
		//{
		//	m_config.m_nQuality=m_pDomFilter->nQuality-1;
		//}
		m_nFrameWidth=m_pDomFilter->nFrameWidth;
		m_nFrameHeight=m_pDomFilter->nFrameHeight;
		display.SetCamera(&player->frame);
		//if(m_pDomFilter->
	}

	display.nAspect=0;
	m_config.InitConfig();
	if(m_config.m_pActiveMode==XNULL)
	{
		if(m_config.m_mouseModes)
			m_config.m_pActiveMode=m_config.m_mouseModes[0];
	}
	if(!pWindow) return;
	//if(nQuality!=m_config.m_nQuality)
	//{
	//	if(m_config.m_nQuality==XConfig::AUTO)
	//		nQuality=XConfig::MID;
	//	else
	//	{
	//		nQuality=m_config.m_nQuality;
	//		SetCamera(updateLazy);
	//	}
	//}
	//m_nScreenDirect=m_config.m_nScreenDirect;
	//switch(m_config.m_nScreenDirect)
	//{
	//case 1:
	//	if(m_devSize.cx<m_devSize.cy)
 //       {
 //           pWindow->PostMsg(XCMD_FLASHMODECMD,-1,0,XMS_TOPMOST);
 //           XSIZE sz;
 //           sz.cx = m_devSize.cx < m_devSize.cy ? m_devSize.cx : m_devSize.cy;
 //           sz.cy = m_devSize.cx > m_devSize.cy ? m_devSize.cx : m_devSize.cy;
 //           m_devSize=sz;
 //           m_winSize=sz;
 //           //m_maxSize = sz;
 //          // break;

 //       }break;
	//case 0:
	//	if(m_devSize.cx>m_devSize.cy)
 //       {
 //           pWindow->PostMsg(XCMD_FLASHMODECMD,-1,0,XMS_TOPMOST);
 //           XSIZE sz;
 //           sz.cx = m_devSize.cx > m_devSize.cy ? m_devSize.cx : m_devSize.cy;
 //           sz.cy = m_devSize.cx < m_devSize.cy ? m_devSize.cx : m_devSize.cy;
 //           m_devSize=sz;
 //           m_winSize=sz;
 //           //m_maxSize = sz;
 //           
 //       }
	//	break;
	//case 2:
	//    if(!loaded&&player->gotHeader)
	//	{
	//		int width = this->GetStageWidth();//(int)RectWidth( &player->frame ) / 20;
	//		int height = this->GetStageHeight();//(int)RectHeight( &player->frame ) / 20;

	//		ChangeMode(width,height);
	//		AdjustWindow( width, height );
	//	}
	//	break;
	//}
	SetMaxPecent( );
	m_config.m_bConfigOK=XTRUE;
	if(!m_config.m_strWaitFor.IsEmpty())
		player->SetWaitClass(m_config.m_strWaitFor,m_config.m_nWaitID);
	if(!loaded&&player->gotHeader&&player->numFramesComplete>=16000)
	{
		GCENTER
		player->PushDataComplete();
		SetCamera(SPlayer::updateLazy);
	}
}

XBOOL XSWFPlayer::MoveView(XPOINT &delta)
{
	//if(scaleMode==DisplayList::showAll) 
	//	return XFALSE;

	XBOOL bOK=XFALSE;

	MATRIX m, invMat;
	/*if ( display.antialias_b )
	{
		MatrixScale(fixed_1>>display.antiShift, fixed_1>>display.antiShift, &m);
		MatrixConcat(&display.camera.mat, &m ,&m);
	} else */
	{
		m = display.mControlMat;//.camera.mat;
	}
	MatrixInvert(&m, &invMat);

	SRECT viewRect, docRect;

	//viewRect.xmin=0;
	//viewRect.ymin=0;
	//viewRect.xmax=devSize.cx;
	//viewRect.ymax=devSize.cy;

	ClientRect(viewRect);
	MatrixTransformRect(&m, &player->frame, &docRect);

	// Calculate the scroll limits
	SRECT limit;
	limit.xmin = docRect.xmin - viewRect.xmin;
	if ( limit.xmin > 0 ) limit.xmin = 0;
	limit.xmax = docRect.xmax - viewRect.xmax;
	if ( limit.xmax < 0 ) limit.xmax = 0;
	limit.ymin = docRect.ymin - viewRect.ymin;
	if ( limit.ymin > 0 ) limit.ymin = 0;
	limit.ymax = docRect.ymax - viewRect.ymax;
	if ( limit.ymax < 0 ) limit.ymax = 0;

	if ( delta.x < limit.xmin )
		delta.x = limit.xmin;
	else if ( delta.x > limit.xmax )
		delta.x = limit.xmax;

	if ( delta.y < limit.ymin )
		delta.y = limit.ymin;
	else if ( delta.y > limit.ymax )
		delta.y = limit.ymax;

	// Show the updated view after a 1/2 second pause
	//zoomRect = viewRect;
	bOK=delta.x!=0||delta.y!=0;
	if(bOK)
	{
		m.tx += delta.x;
		m.ty += delta.y;
		//display.ModifyCamera(false);
		display.UpdateMat();
		//RectOffset(delta.x, delta.y, &zoomRect);
		//MatrixTransformRect(&invMat, &zoomRect, &zoomRect);
		//SetCamera(updateNow);
	}
//	lock.UnLock();


	return bOK;
}




//DEL void XSWFPlayer::StartSound()
//DEL {
//DEL 	theSoundMix.OpenDevice();
//DEL }

void XSWFPlayer::Pause(XBOOL bPause)
{
    if (bPause)
    {
        SetMyFucos(NULL);
    }
	/*if(bPause)
	{
		form=display.camera.cxform;
		display.camera.cxform.ra=96;
		display.camera.cxform.ga=96;
		display.camera.cxform.ba=96;
		display.camera.cxform.flags=XCXForm::needA;
	}
	else
	{
		display.camera.cxform=form;
	}

	SetCamera(updateNow);
	theSoundMix.SetPause(bPause);*/
	//display.Invalidate();
	//if(bPause)
	//	display.Update();
	if(!bPause)
	{
		nextFrameTime=XTime::GetTimeMSec();
	}
#ifdef SOUND
	theSoundMix.SetPause(bPause);
#endif
}

void XSWFPlayer::GetEditTexts(SObject*obj,XSObjectArray &list)
{

	if(obj==XNULL) obj=&display.root;

	//int iNumButtons = 0;

	obj = obj->bottomChild;
	while ( obj ) {

		BOOL isButton = XFALSE;

		if(obj->IsVisible()&&obj->IsEnable())//obj->visible&&obj->enable)
		{

		switch(obj->character->type)
			{
#ifdef EDITTEXT
		case editTextEmptyChar:
		case editTextChar:
			 //if(!(obj->editText->m_flags & seditTextFlagsReadOnly))
			 isButton=XTRUE;
			 break;
#endif
			}

		if (isButton) {
			list.Add(obj);
		}

		GetEditTexts(obj,list);
		}
		obj = obj->above;
	}
}


void XSWFPlayer::ReleaseGlobal()
{
	XXVar::ReleaseData();
	XXSortToken::alloc.ReleaseData();
}

#ifdef FULLFONT



//XBOOL XSWFPlayer::LoadFont(XU16 *text)
//{
//	//XU16Array codes;
//	//return XFALSE;
//	if(!buildInFont.font.infoData) return XFALSE;
//	int nCount=buildInFont.font.nGlyphs;
//
//	//DYNCCHAR*chars=(DYNCCHAR*)buildInFont.font.infoData;
//	XSortConstU16 sort(buildInFont.font.GetCodes(),nCount);
//	//XU32 nOffset=0;
//	XU32*pAddr=buildInFont.font.GetAddress();
//	XU32*offset=buildInFont.font.GetOffset();
//	while(*text)
//	{
//		int id=sort.Index((void*)(XU32)*text);
//		if(id>=0&&pAddr[id]==0)
//		{
//			int iLen=offset[id+1]-offset[id];
//			//chars[id].pData=new XU8[iLen];
//			if(iLen)
//			{
//				XU8*pData=new XU8[iLen];
//				pAddr[id]=(XU32)pData;
//				if(pAddr[id])
//				{
//					//if(nOffset==0)
//					m_fontFile.Seek(offset[id],XFile::XSEEK_SET);
//					//else if(offset[id]>nOffset)
//					//	m_fontFile.Seek(offset[id]-nOffset,XFile::XSEEK_CUR);
//					m_fontFile.Read(pData,iLen);
//					//nOffset=offset[id+1];
//				}
//			}
//		}
//		text++;
//	}
//	return XTRUE;
//	/**((XU16*)buildInFont.font.infoData);//buildInFont.font.nGlyphs;
//	XU16* codes=(XU16*)(buildInFont.font.infoData+2);
//	XU16* advance=codes+nCount;
//	XU32* offset=(XU32*)(advance+nCount);
//
//	XBOOL bModify=XFALSE;
//	XSortConstU16 tcode(codes,nCount);
//	XVectorFast<XU16> newCodes,newIndex;
//	XSortU16Fast  sort(&newCodes);
//	XU16*p=text;
//	while(*p)
//	{
//		int nc=buildInFont.font.nGlyphs;
//		int idd=tcode.Index((void*)*p);
//		if(idd)
//		{
//			int mi;
//			if(sort.Index((void*)*p,mi)<0)
//			{
//				newCodes.InsertAt(mi,*p);
//				newIndex.InsertAt(mi,idd);
//			}
//		}
//		p++;
//	}
//
//	if(buildInFont.data)
//	{
//		XBOOL bModify=XFALSE;
//		int nc=buildInFont.font.nGlyphs;
//		if((int)newCodes.GetSize()!=nc)
//			bModify=XTRUE;
//		if(!bModify) return XTRUE;
//		else
//		{
//			delete buildInFont.data;
//			buildInFont.data=XNULL;
//			//buildInFont.font.nGlphs=0;
//		}
//
//	}
//
//	int i,nc=newCodes.GetSize();
//	if(!nc) return XFALSE;
//	buildInFont.length=4*(nc+1)+6+2*nc+2*nc;
//	int nLength=0;
//	for(i=0;i<nc;i++)
//	{
//		int iIndex=newIndex[i];
//		nLength+=offset[iIndex+1]-offset[iIndex];
//		//buildInFont.length+=offset[iIndex+1]-offset[iIndex];
//	}
//	buildInFont.font.nGlyphs=nc;
//	buildInFont.length+=nLength;
//	nLength+=(nc+1)*4;
//	buildInFont.data=new XU8[buildInFont.length];
//	if(buildInFont.data==XNULL) return XFALSE;
//	XU32* soff=(XU32*)buildInFont.data;
//	XU8*  glyph=(XU8*)(soff+nc+1);
//	XU16* scode=(XU16*)(buildInFont.data+nLength);
//	XU16* sadvance=(XU16*)(scode+3+nc);
//	int iOffset=(nc+1)*4;
//	for(i=0;i<nc;i++)
//	{
//		soff[i]=iOffset;
//		int iIndex=newIndex[i];
//		int iLen=offset[iIndex+1]-offset[iIndex];
//		m_fontFile.Seek(offset[iIndex],XFile::XSEEK_SET);
//		m_fontFile.Read(glyph,iLen);
//		glyph+=iLen;
//		iOffset+=iLen;
//		scode[i]=codes[iIndex];
//		sadvance[i]=advance[iIndex];
//	}
//	soff[nc]=iOffset;
//	scode[nc]=912;
//	scode[nc+1]=222;
//	scode[nc+2]=110;
//	//buildInFont.font.
//	//if(
//
//	return XTRUE;*/
//}
#endif
bool XSWFPlayer::CreateIfEmpty(SCharacter*ch,bool bTexture)
{
	//if(ch->bits.HasBits()) return true;
	bool bAdd=true;
	//if(!ch->data) return true;
	if(ch->bits.bitmapID&&bTexture) return true;
	if(!ch->bits.HasBits())
	{
		if(ch->type==loaderImageChar||ch->type==loaderChar)
		{
			if(!ch->data&&ch->length)
					m_pAVM2->SetActive(ch);
			if(ch->data)
			{
				ch->bits.PICreate(ch->data,ch->length);
				ch->rbounds.xmin=0;
				ch->rbounds.ymin=0;
				ch->rbounds.xmax=ch->bits.width*20;
				ch->rbounds.ymax=ch->bits.height*20;
				
				ch->bits.CalcTrans();
			}
			bAdd = false;
		}
		else
		{
			ch->splayer->BuildBits(ch);
			ch->rbounds.xmin = 0;
			ch->rbounds.ymin = 0;
			ch->rbounds.xmax = ch->bits.width*20;
			ch->rbounds.ymax = ch->bits.height*20;
		}
		}
	if(bTexture)
	{
		ch->bits.CreateTexture();//bAdd);
		ch->bits.ReleaseData();
		if(ch->bits.bitmapID)
			ch->bits.bitmapID->nLastTime = m_nRunTime;
	}
	return true;
}

void XSWFPlayer::GCLeave()
{
	/*ScriptPlayer*p=player;
	while(p)
	{
		avmshell::XAVM2*pAVM2=p->m_pAVM2;
		if(pAVM2)
			pAVM2->GCLeave();
		p=p->nextLayer;
	}*/
	//if(m_pAVM2)
	//	m_pAVM2->GCLeave();
}

void XSWFPlayer::Activate()
{
	/*if(!m_pAVM2) return;
	void*pEvent=m_pAVM2->CreateEventObject(avmshell::XAS3_EVENT_activate,avmshell::XAS3EVENT_EVENT,0,NULL);
	if(pEvent)
	{
		ScriptPlayer*p=player;
	}*/
	GCENTER
	AS3PostEvent(avmshell::XAS3_EVENT_activate,avmshell::XAS3EVENT_EVENT,false);
	//	display.root.AS3PostEvent(pEvent);
}
void XSWFPlayer::Deactivate()
{
	GCENTER
	AS3PostEvent(avmshell::XAS3_EVENT_deactivate,avmshell::XAS3EVENT_EVENT,false);
	/*if(!m_pAVM2) return;
	void*pEvent=m_pAVM2->CreateEventObject(avmshell::XAS3_EVENT_deactivate,avmshell::XAS3EVENT_EVENT,0,NULL);
	if(pEvent)
		display.root.AS3PostEvent(pEvent);*/
}

inline bool XSWFPlayer::AS3PostEvent(int id, int type,bool bMask)
{
	if(!m_pAVM2) return false;
	bool ret=false;
	void*pEvent=m_pAVM2->CreatePostEventObject(id,type,false,0,XNULL);
	if(pEvent)
	{
		/*ScriptPlayer*p=player;
		while(p)
		{
			if(p->m_pContext)
			{
				if(m_pAVM2->PostEvent(pEvent,p->m_pContext,bMask))
					ret=true;
			}
			p=p->nextLayer;
		}*/
		//m_bEnterFrame=XTRUE;
		m_pAVM2->PostEvent(pEvent,XNULL,bMask);
		//m_bEnterFrame=XFALSE;
	}
	return ret;
}

inline void XSWFPlayer::AS3InitKeyData(KEYEVENTDATA&data,int nChar,int nKey,int nLocation,SObject*pObj)//void* XSWFPlayer::AS3CreateKeyboardEvent(int id,int nChar,int nKey,int nLocation,void*pTarget)
{
	data.bAlt=0;
	data.bControl=nKeyStatus[VK_CONTROL]!=0;
	data.bShift=nKeyStatus[VK_SHIFT]!=0;
	data.nChar=nChar;
	data.nKey=nKey;
	data.nLocaltion=nLocation;
	data.pTarget=pObj->pASObject;
	/*int datas[]=
	{
		nChar,
		nKey,
		nLocation,
		nKeyStatus[VK_CONTROL],
		0,
		nKeyStatus[VK_SHIFT],
		(int)pTarget
	};
	return m_pAVM2->CreateEventObject(id,avmshell::XAS3EVENT_KEYBOARD,true,7,datas);*/
}

inline void XSWFPlayer::AS3InitMouseData(MOUSEEVENTDATA&data,SObject*pObj,bool bDown,void*pTarget,void*pRel,_XSObject*pStop)//void* XSWFPlayer::AS3CreateMouseEvent(int id,_XSObject*pObj,bool bDown,void*pTarget)
{
	//int lx,ly;
	//GetMousePos(pObj,lx,ly);
	GetMousePos(pObj,data.localX,data.localY);
	data.buttonDown=bDown;
	data.ctrlKey=nKeyStatus[VK_CONTROL]!=0;
	data.delta=10;
	//data.localX=lx;
	//data.localY=ly;
	data.pRelatedObject=pRel;//pTarget->pASObject;
	data.pTarget=pTarget;
	data.bWithTarget=false;
//#if (__CORE_VERSION__>=0x02070200)
	if(pStop==(_XSObject*)2)
		data.pStop=pStop;
	else
//#endif
	data.pStop=pStop?pStop->pASObject:XNULL;

	data.shiftKey=nKeyStatus[VK_SHIFT]!=0;

	GetMousePos(XNULL,data.stageX,data.stageY);
	//data.stageX=gmousePt.x;
	//data.stageY=gmousePt.y;


	/*MOUSEEVENTDATA data=
	{	nKeyStatus[VK_CONTROL]!=0,
		bDown,
		10,
		lx,
		ly,
		pTarget,
		nKeyStatus[VK_SHIFT]!=0,
		mousePt.x,
		mousePt.y};
	return m_pAVM2->CreateEventObject(id,avmshell::XAS3EVENT_MOUSE,true,1,(int*)&data);	*/
}

/*inline void* XSWFPlayer::AS3CreateFocusEvent(int id,int nKey,void*r,void*pTarget)
{
	int datas[]={nKey,(int)r,nKey?nKeyStatus[VK_SHIFT]!=0:false,(int)pTarget};
	return m_pAVM2->CreateEventObject(id,avmshell::XAS3EVENT_FOCUS,true,4,datas);
}*/

void XSWFPlayer::AS3SetKeyFocus(SObject*pOld,SObject*pObj,int nKey)
{
	if(pOld==pObj) return;
	{
		bool bShift=nKeyStatus[VK_SHIFT]!=0;
		if(pOld&&pOld->pASObject&&pOld->IsFocusAble())
		{
			void*pContext=pOld->GetContext();//NULL;
			//ScriptPlayer*player=pOld->GetPlayer();
			//if(player)
			//		pContext=player->m_pContext;
			//if(pOld->character)
			//		pContext=pOld->character->player->m_pContext;
			m_pAVM2->OnFocusEvent(XAS3_EVENT_keyFocusChange,pObj?pObj->pASObject:XNULL,pOld->pASObject,nKey,bShift,pContext);
			//if(!pOld->IsParentOf(pObj))
			m_pAVM2->OnFocusEvent(XAS3_EVENT_focusOut,pObj?pObj->pASObject:XNULL,pOld->pASObject,nKey,bShift,pContext);
			//void*pEvent=AS3CreateFocusEvent(avmshell::XAS3_EVENT_keyFocusChange,
			//	nKey,pObj?pObj->pASObject:XNULL,pOld->pASObject);
			//m_pAVM2->DispatchEvent(pEvent,pOld->pASObject);
			//m_pAVM2->ResetEvent(pEvent,avmshell::XAS3_EVENT_focusOut,pOld->pASObject);
			//m_pAVM2->DispatchEvent(pEvent,pOld->pASObject);
		}
		if(pObj&&pObj->pASObject&&pObj->IsFocusAble())
		{
			void*pContext=pObj->GetContext();//NULL;
			//ScriptPlayer*player=pOld->GetPlayer();
			//		if(player)
			//			pContext=player->m_pContext;
			//if(pObj->character)
			//		pContext=pObj->character->player->m_pContext;
			m_pAVM2->OnFocusEvent(XAS3_EVENT_keyFocusChange,pOld?pObj->pASObject:XNULL,pObj->pASObject,nKey,bShift,pContext);
			//if(!pObj->IsParentOf(pOld))
			m_pAVM2->OnFocusEvent(XAS3_EVENT_focusIn,pOld?pOld->pASObject:XNULL,pObj->pASObject,nKey,bShift,pContext);
			//void*pEvent=AS3CreateFocusEvent(avmshell::XAS3_EVENT_focusIn,nKey,pOld?pOld->pASObject:XNULL,pObj->pASObject);
			//m_pAVM2->DispatchEvent(pEvent,pObj->pASObject);
			//m_pAVM2->ResetEvent(pEvent,avmshell::XAS3_EVENT_focusOut);
			//m_pAVM2->DispatchEvent(pEvent,pOld->pASObject);
		}
	}
}


XBOOL XSWFPlayer::CallTimer(XSWFCONTEXT &cnt)
{
	FreeCalls();
	if(m_callList==XNULL) return XFALSE;
	XU32 nTime=XTime::GetTimeMSec();
	XU8 bSet=0;
	TimeCaller**pRoot=&m_callList;
	XXObject*pCallObj=player->rootObject->pObject;//pCaller?pCaller->rootObject->pObject:XNULL;
	XXObjectClip*pClip=(XXObjectClip*)player->rootObject->pObject;
	//while(pCaller)
	for(;;)
	{
		TimeCaller*pCaller=*pRoot;
		if(!pCaller) break;
		TimeCaller**pNext=&pCaller->pNext;
		int iType=(int)pCaller->pBase;
//#if (__CORE_VERSION__>=0x02077000)		
		if(pCaller->nInit<10000)
			pCaller->nInit++;
//#endif
		switch(iType)
		{
		case 3:
		case 2:
		case 1:
//#if (__CORE_VERSION__>=0x02077000)
//			if(pCaller->nInit) break;
//#endif
			if(nTime>=pCaller->nLastCall+pCaller->nTimer)
			{
				pCaller->nLastCall=nTime;
				if(iType==2)
				{
					*pRoot=pCaller->pNext;
					pCaller->pNext=m_freeList;
					m_freeList=pCaller;
				}
				if(iType==3)
				{
					//((avmshell::TimerObject*)pCaller->pFunction)->OnTimer(nTime);
					m_pAVM2->OnTimer(nTime,pCaller->pFunction,pCaller->pContext);
					break;
				}
				m_pAVM2->Call(pCaller->pFunction,pCaller->pArguments,pCaller->pContext);

			}break;
		default:
			{
			XXObject*pBase=pCaller->pBase;//pFunction->GetBase();
			if(pBase->m_pObject==XNULL)
			{
				*pRoot=pCaller->pNext;
				pCaller->pNext=m_freeList;
				m_freeList=pCaller;
				continue;
			}
//#if (__CORE_VERSION__>=0x02077000)
//			if(pCaller->nInit) break;
//#endif
			if(nTime>=pCaller->nLastCall+pCaller->nTimer)
			{
				pCaller->nLastCall=nTime;
				XU8 vars[sizeof(XXVar)*32];
				XXVARLIST list(vars,32);//pCaller->args.GetSize());
				for(int i=0;i<(int)pCaller->args.GetSize();i++)
				{
					list.Push(pCaller->args[i]);
				}
				if(pCaller->strMethodName.IsNull())//IsEmpty())
				{
					cnt.InitContextA(XNULL,pBase,pBase,pBase,pBase,XNULL);

					pCaller->pFunction->Function(&cnt,list,pCallObj);

					cnt.pStack->FreePop();
				}
				else //if(0)
				{
					//if(pCaller->strMethod=="goLiftUp")
					//	int v=0;
					cnt.InitContextA(XNULL,pBase,pBase,pCaller->pFunction,pCaller->pFunction,XNULL);

					pCaller->pFunction->CallMethod(&cnt,pCaller->strMethodName.strTxt,list,XFALSE);

					cnt.pStack->FreePop();
				}
				list.Release();//FreeAll();
				if(pCaller->nType==1)
				{
					*pRoot=pCaller->pNext;
					pCaller->pNext=m_freeList;
					m_freeList=pCaller;
				}
			}
			break;
			}
		}
		pRoot=pNext;
		//cnt.context.RemoveLast();
	}
	//cnt.pTarget=pTarget;
	//cnt.pContext=XNULL;
	//cnt.contexts.RemoveLast();
	return bSet;
}

bool XSWFPlayer::AS3CreateSObject(_XSObject*&obj,int type,void*pInstance,SCharacter*ch,ScriptPlayer*player)
{
		bool bNew=false,bRoot=false;
		//if(type==15)
		//	int v=0;
		if(ch==XNULL)
		{
			bNew=true;
			ch=CreateCharacter();//new SCharacter;
			if (!ch) return XNULL;
			ch->record.Init();
			ch->data=XNULL;
			ch->className=XNULL;
			ch->gridRect=XNULL;
			ch->gridRect=XNULL;
			ch->splayer=player;
			ch->sprite.length=0;
			ch->sprite.numFrames=0;
			ch->classEntry=NULL;
			//ch->className=NULL;
			//ch->type=spriteEmptyChar;
			::RectSetEmpty(&ch->rbounds);
			ch->tag=0; //spriteEmptyChar
			ch->type=(U8)type;
		}
		else if(ch->type==rootChar)
		{
			bNew=true;
			bRoot=true;
			SCharacter*chn=CreateCharacter();//new SCharacter;
			ScriptPlayer*pl=ch->splayer;
			if (!chn) return XNULL;
			chn->record.Init();
			chn->data=pl->script+pl->startPos;//ch->data;
			chn->className=ch->className;
			chn->gridRect=ch->gridRect;			
			chn->splayer=ch->splayer;
			chn->sprite.length=pl->scriptLen-pl->startPos;//ch->sprite.length;
			chn->sprite.numFrames=pl->numFrames;//ch->sprite.numFrames;
			chn->classEntry=ch->classEntry;
			//ch->className=ch->className;
			//ch->type=spriteEmptyChar;
			//::RectSetEmpty(&chn->rbounds);
			chn->rbounds=ch->rbounds;
			chn->tag=ch->tag; //spriteEmptyChar
			chn->type=rootCloneChar;
			ch=chn;
		}
		//if(ch->tag==351)
		//	int v=0;
		obj = display.CreateObject(player,m_nLoopCount);
		if ( !obj ) {if(bNew) FreeCharacter(ch);return false;}
		//obj->nCreateTime=m_nLoopCount;
		::RectSetEmpty(&obj->devBounds);


		obj->parent = XNULL;
		obj->above = XNULL;

		obj->pFilter=XNULL;
		obj->pLink=XNULL;
		obj->pLinkBy=XNULL;
		obj->nForeColor=obj->nBackColor=0;
		obj->blendMode=10;
		obj->cacheMode=0;
		obj->clipObject=NULL;
		obj->exflags=SObject::XEN_ALL;
		obj->tagType=ch->type;


		obj->character = ch;
		obj->pSuper=XNULL;
		::MatrixIdentity(&obj->xform.mat);// = thread->rootObject->xform.mat;
		obj->xform.cxform.Clear();

		obj->depth = 0;
		obj->ratio = 0;
		obj->puppet = 0;
		obj->drawn = XFALSE;
		obj->flags=SObject::OB_VISIABLE|SObject::OB_ENABLE;
		obj->dragCenter = XFALSE;
		RectSetEmpty(&obj->dragConstraint);
		obj->dropTarget = NULL;
		obj->display = &display;
		obj->bottomChild = 0;
		//obj->pNext=NULL;
		//obj->pPrev=NULL;
//		obj->edges = 0;
//		obj->colors = 0;
		obj->blendMode=0;
		obj->cacheMode=0;
		obj->state = 0;
		obj->pObject=XNULL;
		obj->UpdateControl();
		obj->SetInstance();
		obj->SetInitChild();
#ifdef EDITTEXT
		obj->editText = 0;
#endif

		obj->thread = 0;
		obj->clipDepth = 0;
		obj->Modify();

		obj->pASObject=pInstance;
		if(obj->character->gridRect)
		{
			//if(obj->character->tag==154)
			//	int v=0;
			obj->gridRect=display.CreateRect();
			SParser parser;
			parser.Attach((U8*)obj->character->gridRect, 0);
			parser.GetRect(obj->gridRect);
			obj->gridRect->xmax/=20;
			obj->gridRect->ymax/=20;
			obj->gridRect->xmin/=20;
			obj->gridRect->ymin/=20;
		}
		//obj->AS3RemoveFromStage();

		switch ( ch->type ) {
		case rootChar:
			/*{
			if(obj->name.IsNull())
			{
				XXVar var;
				var.SetString(_strInstance,32);
				//URLBuilder ub;
				//ub.AppendString("instance");
				//ub.AppendInt(++instanceNameCount);
				XXVar varc(++display.instanceNameCount);
				var.StringAdd(varc);
				//obj->strName=ub.buf;
				obj->name = var;//CreateStr(ub.buf);
				obj->AS3SetNoName();
			}
			// Create a sprite thread
			ScriptThread* thread = display.CreateThread();//new ScriptThread();
			thread->InitThread();
			if ( thread ) {
				//if(ch->tag==104)
				//	int v=0;

				thread->nEventFlags=0;//info->nEventFlags;
				thread->pActions=XNULL;
				thread->ClearState();
				//thread->playing=false;
				obj->thread = thread;//obj->GetPlayer();
				thread->player = obj->character->splayer;//GetPlayer()->player;
				thread->splayer= this;//this->splayer;
				thread->layerDepth = obj->depth;
				thread->rootObject = obj;
				thread->SetDisplay(obj->display,false);
				thread->Attach(obj->character->data, 0);

				thread->startPos = 0;
				//if(obj->character->data)
				thread->len = obj->character->sprite.length;
				thread->numFrames = obj->character->sprite.numFrames;
				if(ch->type==spriteChar)
				{
					//thread->AS3BuildSecnesAndFrames(obj->pASObject);
					thread->m_bAS3Frame0=true;
					thread->DrawFrame(0, XFALSE, true);
					//obj->AS3FillMembers(pInstance,m_pAVM2);
				}
			}
			}*/
			break;
		case bitsChar:
			ch->splayer->BuildBits(ch);
			if(ch->bits.HasBits())
			{
				ch->rbounds.xmin=0;
				ch->rbounds.ymin=0;
				ch->rbounds.xmax=ch->bits.width*20;
				ch->rbounds.ymax=ch->bits.height*20;
				ch->bits.CreateTexture();
				ch->bits.ReleaseData();
			}
			break;
		case buttonChar: {
			// Set up the button state
			display.AS3SetStateObjects(obj,obj->pASObject,0xff);
			obj->state=bsNull;
			int state = bsIdle;
			display.UpdateButton(obj, state,XTRUE);

			//display->UpdateButton(obj,state,bAS3);
		} break;

#ifdef EDITTEXT
		case textLineChar:
			{
				//int v=0;
			}
		case editTextEmptyChar:		
		case editTextChar:
			{
				bool noSize=false;
				if(ch->data==NULL)
				{
					//XSWFPlayer*player=this->splayer;
					ch->data=_stockEditData;
					ch->rbounds.xmin=ch->rbounds.ymin=0;
					ch->rbounds.xmax=100*20;
					ch->rbounds.ymax=100*20;
					ch->length=sizeof(_stockEditData);
					noSize=true;
				}
				obj->editText = this->display.CreateEditText();//new EditText(obj);
				obj->editText->Init(obj);
				obj->editText->m_bNoSize=noSize;
				if(noSize)
				{
					obj->editText->m_flags&=~seditTextFlagsMultiline;
					obj->editText->m_flags&=~seditTextFlagsWordWrap;
					//obj->editText->m_flags
				}
			}
			break;
#endif
		case bitmapEmptyChar:
			{
				/*XDrawDib*pDib=m_pAVM2->GetDrawDib(pInstance,NULL);
				if(pDib)
				{
					ch->rbounds.xmin=0;
					ch->rbounds.ymin=0;
					ch->rbounds.xmax=pDib->Width()*20;
					ch->rbounds.ymax=pDib->Height()*20;
				}*/
			}break;
		case rootCloneChar:
		case spriteChar:
		case spriteEmptyChar:
  			{
			if(obj->name.IsNull())
			{
				XXVar var;
				var.SetString(_strInstance,32);
				//URLBuilder ub;
				//ub.AppendString("instance");
				//ub.AppendInt(++instanceNameCount);
				XXVar varc(++display.instanceNameCount);
				var.StringAdd(varc);
				//obj->strName=ub.buf;
				obj->name = var;//CreateStr(ub.buf);
				obj->AS3SetNoName();
			}
			// Create a sprite thread
			ScriptThread* thread = display.CreateThread();//new ScriptThread();
			thread->InitThread();
			if ( thread ) {
				//if(ch->tag==104)
				//	int v=0;

				thread->nEventFlags=0;//info->nEventFlags;
				thread->pActions=XNULL;
				thread->ClearState();
				//thread->playing=false;
				obj->thread = thread;//obj->GetPlayer();
				thread->player = obj->character->splayer;//GetPlayer()->player;
				thread->splayer= this;//this->splayer;
				thread->layerDepth = obj->depth;
				thread->rootObject = obj;
				thread->SetDisplay(obj->display,false);
				thread->Attach(obj->character->data, 0);

				thread->startPos = 0;
				//if(obj->character->data)
				thread->len = obj->character->sprite.length;
				thread->numFrames = obj->character->sprite.numFrames;
				if(ch->type==spriteChar||bRoot)
				{
					//thread->AS3BuildSecnesAndFrames(obj->pASObject);
					thread->m_bAS3Frame0=true;
					//if(ch->tag==757)
					//	thread->DrawFrame(1, XFALSE, true);
					//else
					thread->DrawFrame(0, XFALSE, true);
					//obj->AS3FillMembers(pInstance,m_pAVM2);
				}
//#if (__CORE_VERSION__>=0x02074000)
				if(bNew)
				{
					thread->playing=false;
					if(thread->numFrames<=0)
						thread->numFrames=1;
				}
//#endif
			}
		} break;
	}

	return true;
}



void XSWFPlayer::SetLockClass(void*codeContext)
{
	if(this->m_pDomFilter&&m_pDomFilter->strClassLock.GetSize())
	{
		int nSize=m_pDomFilter->strClassLock.GetSize()-1;
		while(nSize>=0)
		{
			XString8&strClass=m_pDomFilter->strClassLock[nSize];
			void*pClass=m_pAVM2->FindClass(strClass,codeContext);
			if(pClass)
			{
				m_pDomFilter->strClassLock.RemoveAt(nSize);
				m_lockClasses.Add((XU32)pClass);
			}
			nSize--;
		}
	}
}

void XSWFPlayer::SetMaxPecent( )
{
	/*int nStageW = GetStageWidth();
	int nStageH = GetStageHeight();
	if(nStageW<=0||nStageH<=0)
		return;
	int w = m_winSize.cx;
	int h = m_winSize.cy - 60;
	int pecentx = w * 100 / nStageW;
	int pecenty = h * 100 / nStageH;
	if( pecentx < pecenty )
	{
		m_nMaxPecent = pecentx * 2;
	}
	else if( pecenty < pecentx )
	{
		m_nMaxPecent = pecenty * 2;
	}*/
}

void XSWFPlayer::LockOpt(XBOOL bClear)
{
	if(!m_lockClasses.GetSize()) return;
	if(bClear)
	{
		::RectSetEmpty(&m_objRect);
		m_optObject=NULL;
		if(this->pWindow)
			pWindow->Handle(XCMD_UPDATEOBJ,0,0);
	}
	m_nLockOpt=100;

}
void XSWFPlayer::ClearOpt(_XSObject*obj)
{
	if(obj)
	{
		if(m_optObject!=obj) return;
	}
	::RectSetEmpty(&m_objRect);
	m_nLockOpt=0;
	if(this->pWindow)
		pWindow->Handle(XCMD_UPDATEOBJ,0,0);
	m_optObject=NULL;
}
void XSWFPlayer::UnLockOpt()
{
	/*if(m_optObject)
	{
		m_optObject->GetBoundBox(m_objRect,XTRUE);
	}
	m_optObject=NULL;
	m_bLockOpt=false;*/
}
XBOOL XSWFPlayer::GetObjectRect(XRect&rect)
{
	if(RectIsEmpty(&m_objRect)) return false;
	rect.left=m_objRect.xmin;
	rect.right=m_objRect.xmax;
	rect.top=m_objRect.ymin;
	rect.bottom=m_objRect.ymax;
	return XTRUE;
}

XBOOL XSWFPlayer::AddOptObject(_XSObject*obj)
{
	if(!m_nLockOpt) return XFALSE;
	if(!obj->pASObject||!m_lockClasses.GetSize()) return XFALSE;

	int i,nc=m_lockClasses.GetSize();
	bool bSet=false;
	for(i=0;i<nc;i++)
	{
		ClassClosure*pClass=(ClassClosure*)m_lockClasses[i];
		ScriptObject*pObj=(ScriptObject*)obj->pASObject;
		if(ShellCore::IsTypeClass(pObj,pClass))
		{
			bSet=true;
			break;
		}
	}
	if(!bSet) return XFALSE;

	if(m_optObject)
	{
		if(m_optObject->IsParentOf(obj))
			return XFALSE;
		/*SRECT rect;
		obj->GetBoundBox(rect,XTRUE);
		int sx=rect.xmax-rect.xmin;
		int sy=rect.ymax-rect.ymin;
		int ox=m_objRect.xmax-m_objRect.xmin;
		int oy=m_objRect.ymax-m_objRect.ymin;
		if(sx*sy<=ox*oy) return XFALSE;
		m_objRect=rect;*/
		m_optObject=obj;
	}
	else
	{
		m_optObject=obj;
		//obj->GetBoundBox(m_objRect,XTRUE);
	}
	if(this->pWindow)
		pWindow->Handle(XCMD_UPDATEOBJ,1,0);
	return XTRUE;
}

bool XSWFPlayer::AS3Connect(const char*name,void*p,int t,void*pContext)
	{
		const char*id=strchr(name,':');
		XXVar strDomain,strName;
		if(id)
		{
			strDomain.SetString(name,id-name);
			strName.SetString(id+1,strlen(id+1));
		}
		else
		{
			strDomain=_strEmpty;
			strName=name;
		}
		strDomain.ToString(XTRUE);
		strName.ToString(XTRUE);
		int i,nCount=m_connections.GetSize();
		for(i=0;i<nCount;i++)
		{
			_CONNECTION*p=m_connections[i];
			if(XString8::Compare(strName.strTxt,p->strName.strTxt,strName.GetLength(),XTRUE)==0)
				return false;
		}
		_CONNECTION*pc=new _CONNECTION;
		pc->strDomain=strDomain;
		pc->nType=t;
		pc->pContext=pContext;
		//pc-=root;
		pc->strName=strName;
		pc->pClass=p;
		//((ScriptObject*)p)->IncrementRef();
		m_pAVM2->GetToplevel()->PushObject((ScriptObject*)p);
		m_connections.Add(pc);
		return true;
	}
bool XSWFPlayer::AS3Close(void*p)
{
	int i,iCount=m_connections.GetSize();
	bool bOK=false;
	for(i=iCount-1;i>=0;i--)
	{
		_CONNECTION*pc=m_connections[i];
		if(pc->pClass==p)
		{
			//((ScriptObject*)p)->DecrementRef();
			m_pAVM2->ReleaseInstance(&p,pc->pContext,false,NULL);
			delete pc;
			m_connections.RemoveAt(i);
			bOK=true;
		}
	}
	return bOK;
}

XBOOL XSWFPlayer::LanuchAD()
{
	if(this->m_bADLanuched) return XFALSE;
	this->m_bADLanuched=XTRUE;
	const char*strURL=NULL;
	if(m_pDomFilter&&!m_pDomFilter->strADURL.IsEmpty())
		strURL=m_pDomFilter->strADURL;
	else if(!m_config.m_strADURL.IsEmpty())
		strURL=m_config.m_strADURL;

	if(!strURL) return XFALSE;
	AddLoader(strURL,"_level999999");
	return XTRUE;
}

void XSWFPlayer::AS3Release()
{
	int i,nCount=m_connections.GetSize();
	for(i=0;i<nCount;i++)
	{
		//((ScriptObject*)m_connections[i]->pClass)->DecrementRef();
		m_pAVM2->ReleaseInstance(&m_connections[i]->pClass,XNULL,false,NULL);
		delete m_connections[i];
	}
	m_connections.RemoveAll();
}
bool XSWFPlayer::AS3Send(const char*name,const char*method,void*args)
{
	int i,iCount=m_connections.GetSize();
	bool bOK=false;
	const char*id=strchr(name,':');
	XXVar strDomain,strName;
	if(id)
	{
		strDomain.SetString(name,id-name);
		strName.SetString(id+1,strlen(id+1));
	}
	else
	{
		strDomain=_strEmpty;
		strName=name;
	}
	for(i=iCount-1;i>=0;i--)
	{
		_CONNECTION*pc=m_connections[i];
		if(XString8::Compare(strName.strTxt,pc->strName.strTxt,pc->strName.GetLength(),XTRUE)==0)
		{
//			LocalConnectionObject*pObj=(LocalConnectionObject*)pc->pClass;
			if(m_pAVM2->SendMethod(pc->pClass,strName.strTxt,strDomain.strTxt,method,args,pc->nType,pc->pContext))
				bOK=true;
		}
	}
	return bOK;
}
#ifdef _ANALY
void	XSWFPlayer::GetClassTree(XString8&strTree,void*pASObject)
{
	if(!m_pAVM2) return;
	m_pRetObject=NULL;
	m_getEvent.Create();
	AddEvent(XGETCLASSTREE,(int)&strTree,(int)pASObject,0);
	int nTimes=0;
	while(!m_getEvent.IsEvent(2000))
	{
		nTimes++;
		if(nTimes>10) break;
	}
#ifdef _WINENU
	if(!strTree.IsEmpty())
	{
		XString16 s(strTree);
		Trace(s);
	}
#endif
	m_getEvent.Release();
}
void*		 XSWFPlayer::AS3FindClass(const char*strClass,void*pASObject)
{
	if(!m_pAVM2) return NULL;
	m_pRetObject=NULL;
	m_getEvent.Create();
	AddEvent(XFINDCLASS,(int)strClass,(int)pASObject,0);
	int nTimes=0;
	while(!m_getEvent.IsEvent(2000))
	{
		nTimes++;
		if(nTimes>10) break;
	}
	m_getEvent.Release();
	return m_pRetObject;
}
AS3PROPITEM* XSWFPlayer::AS3GetChildren(void*pASObject,bool bSuper)
{
	if(!m_pAVM2) return NULL;
	//if(!m_pAVM2->GetCore()->IsProtoTypeOf
	//	(m_pAVM2->GetToplevel()->getEventDispatcherClass()->prototype,(EventDispatcherObject*)pASObject)) return NULL;
	m_pReturnItem=NULL;
	m_getEvent.Create();
	AddEvent(XGETCHILDREN,(int)pASObject,(int)bSuper,0);
	int nTimes=0;
	while(!m_getEvent.IsEvent(2000))
	{
		nTimes++;
		if(nTimes>10) break;
	}
	m_getEvent.Release();
	return m_pReturnItem;
	//GCENTER
	//return ((EventDispatcherObject*)pASObject)->GetChildren();
}

static inline _DIBINFO* _AddInfo(_DIBINFO*pRoot,int nType)
{
	_DIBINFO*pInfo=new _DIBINFO();
	pInfo->pNext=NULL;
	pInfo->pDib=0;
	pInfo->isAttach=false;
	pInfo->nType=nType;
	pInfo->nCacheTime=0;
	pInfo->nLastTime=0;
	pInfo->nLockCount=0;
	pInfo->nUseTime=0;
	if(pRoot)
		pRoot->pNext=pInfo;
	return pInfo;
}

void XSWFPlayer::GetDibInfo(_DIBINFO*pInfo)
{
	RasterGL* gl = RasterGL::sharedRasterGL();
	int i,iCount=RasterGL::GetTextureCount();
	SBitmapCore*bitmap=NULL;
	FrameTexture*frame=NULL;
	for(i=0;i<iCount;i++)
	{
		_TEXTUREID*id = RasterGL::GetTextureByID(i);
		if(!id) continue;
		pInfo = _AddInfo(pInfo,0);
		memset(pInfo,0,sizeof(_DIBINFO));
		XDrawDib* pDib = new XDrawDib();
		gl->GetTexture(id,*pDib);
		pInfo->pDib = pDib;
		pInfo->nType = id->type;
		pInfo->isAttach = id->type==1;
		pInfo->nLastTime = id->nLastTime;
		pInfo->nLockCount = id->nUseCount;
		pInfo->nCacheTime = id->nDisplayCount;
		pInfo->nUseTime = m_nRunTime - id->nLastTime;
		if(id->type==2)
		{
			RGB8*pRGB=(RGB8*)pDib->GetBuffer();
			int iCount = pDib->Width()*pDib->Height();
			int im;
			for(im=0;im<iCount;im++)
			{
				pRGB[im].red=pRGB[im].alpha;
				pRGB[im].green=pRGB[im].alpha;
				pRGB[im].blue=pRGB[im].alpha;
			}
		}
	}
	iCount = SBitmapCore::GetBitmapCount();
	for(i=0;i<iCount;i++)
	{
		SBitmapCore*bmp=SBitmapCore::GetBitmapByID(i);
		pInfo = _AddInfo(pInfo,0);
		memset(pInfo,0,sizeof(_DIBINFO));
		XDrawDib* pDib = new XDrawDib();
		pDib->CreateFrom(bmp,false);
		pInfo->pDib = pDib;
		pInfo->nType = 4;
		pInfo->isAttach = false;
		pInfo->nLastTime = bmp->m_nLockTime;
		pInfo->nLockCount = bmp->useCount;
		pInfo->nCacheTime = bmp->m_pCH==NULL?0:1;
		pInfo->nUseTime = m_nRunTime - pInfo->nLastTime;
	}
	/*SBitmapCore*bitmap=m_bitmaps;
	while(bitmap)
	{
		SBitmapCore*next=bitmap->m_pNext;
		if(bitmap->HasBits())
		{
			//if(bitmap->width==2140&&bitmap->height==900)
			//	int v=0;
			pInfo = _AddInfo(pInfo,0);
			pInfo->isAttach = bitmap->attachDib==TRUE;
			pInfo->nCacheTime = bitmap->m_nMaxCacheTime;
			pInfo->nLastTime = bitmap->m_nLockTime;
			pInfo->nLockCount = bitmap->lockCount;
			pInfo->nUseTime = this->m_nRunTime - bitmap->m_nLockTime;
			XDrawDib*pDib = new XDrawDib();
			pInfo->pDib = pDib;
			pDib->CreateFrom(bitmap,false);
		}
		bitmap=next;
	}
	if(m_pAVM2)
	{
		m_pAVM2->GetToplevel()->GetDibInfo(pInfo,this->m_nRunTime);
	}*/
}

static inline void AddFast(XPCTSTR name,XSortMemName*pInfo,XFastAlloc*pAlloc)
{
	int nSize = 0;//pAlloc->m_nMemSize;
	int nCount = pAlloc->m_nAllocs;
	XFastAlloc::XDATA*pData=pAlloc->m_pData;
	while(pData)
	{
		nSize += pAlloc->m_nMemSize;
		pData=pData->pNext;
	}
	pInfo->Add(name,nSize,nCount);
}

void XSWFPlayer::GetMemInfo(XSortMemName*pInfo)
{
//	AddFast("Active Edge",pInfo,&display.raster.activeEdgeAlloc);
//	AddFast("Run(Raster)",pInfo,&display.raster.runAlloc);
//	AddFast("Ramp(Raster)",pInfo,&display.raster.rampAlloc);
//	AddFast("Map(Raster)",pInfo,&display.raster.mapAlloc);
	AddFast("Cmd(Display)",pInfo,&display.cmdAlloc);
	AddFast("Button Info(Display)",pInfo,&display.infoAlloc);
//	AddFast("Edge(Display)",pInfo,&display.edgeAlloc);
	AddFast("Commond(Display)",pInfo,&display.commandAlloc);
	AddFast("Curve(Display)",pInfo,&display.curveAlloc);
	AddFast("Format(Display)",pInfo,&display.formatAlloc);
	AddFast("Load(Display)",pInfo,&display.loadAlloc);
	AddFast("Edit(Display)",pInfo,&display.editAlloc);
	AddFast("Thread(Display)",pInfo,&display.threadAlloc);
	AddFast("Rect(Display)",pInfo,&display.rectAlloc);
	AddFast("Matrix(Display)",pInfo,&display.matrixAlloc);
	AddFast("Char(Player)",pInfo,&this->charAlloc);
	AddFast("Event(Player)",pInfo,&this->eventAlloc);
	AddFast("Filters(Player)",pInfo,&this->filterAlloc);
	AddFast("Block64",pInfo,&XXVar::alloc64);
	AddFast("Block128",pInfo,&XXVar::alloc128);
	AddFast("Block256",pInfo,&XXVar::alloc256);
	AddFast("Block512",pInfo,&XXVar::alloc512);
	AddFast("Block1024",pInfo,&XXVar::alloc1024);
	AddFast("Block2048",pInfo,&XXVar::alloc2048);
	AddFast("Block4096",pInfo,&XXVar::alloc4096);
	AddFast("Block16384",pInfo,&XXVar::alloc16384);
	TessPloy* tess=TessPloy::m_pInstance;
	if(tess)
	{
		AddFast("OpenGL Path(Tess)",pInfo,&tess->allocPath);
		AddFast("OpenGL Edges(Tess)",pInfo,&tess->allocEdge);
		AddFast("OpenGL Edges2(Tess)",pInfo,&tess->allocEdge2);
	}
	ShapeAlloc* pAlloc = ShapeAlloc::GetInst();
	AddFast("OpenGL Edge(ShapeAlloc)",pInfo,&pAlloc->edgeAlloc);
	AddFast("OpenGL Path(ShapeAlloc)",pInfo,&pAlloc->pathAlloc);
	AddFast("OpenGL color(ShapeAlloc)",pInfo,&pAlloc->colorAlloc);
	AddFast("OpenGL lineStyle(ShapeAlloc)",pInfo,&pAlloc->lineStyleAlloc);
	AddFast("OpenGL point(ShapeAlloc)",pInfo,&pAlloc->pointAlloc);
	AddFast("OpenGL trangle(ShapeAlloc)",pInfo,&pAlloc->trangleAlloc);
	AddFast("OpenGL trangle or lines(ShapeAlloc)",pInfo,&pAlloc->tranglesOrLinesAlloc);
	AddFast("OpenGL ramp(ShapeAlloc)",pInfo,&pAlloc->rampAlloc);
	AddFast("OpenGL rectangle(ShapeAlloc)",pInfo,&pAlloc->rectangleAlloc);
	AddFast("OpenGL simpleRect(ShapeAlloc)",pInfo,&pAlloc->simpleRectAlloc);
	AddFast("OpenGL char(ShapeAlloc)",pInfo,&pAlloc->charAlloc);
	AddFast("OpenGL image(ShapeAlloc)",pInfo,&pAlloc->imageAlloc);
	if(m_pAVM2)
	{
		m_pAVM2->GetGC()->Report(pInfo);
	}
}

void XSWFPlayer::AS3SeekItem(XSeekItem*pItem)
{
	m_pSeekItem = pItem;
	m_getEvent.Create();
	AddEvent(XSEEKITEM,0,0,0);
	int nTimes=0;
	while(!m_getEvent.IsEvent(2000))
	{
		nTimes++;
		if(nTimes>20) break;
	}
	m_getEvent.Release();

}
void XSWFPlayer::SeekItem(XSeekItem*pItem)
{
	if(m_pAVM2)
	{
		m_pAVM2->GetGC()->SeekItem(pItem);
	}
}

_DIBINFO* XSWFPlayer::AS3GetDibInfo()
{
	m_pReturnInfo = _AddInfo(NULL,0);
	
	m_getEvent.Create();
	AddEvent(XGETDIBINFO,0,0,0);
	int nTimes=0;
	while(!m_getEvent.IsEvent(2000))
	{
		nTimes++;
		if(nTimes>20) break;
	}
	m_getEvent.Release();
	return m_pReturnInfo;
}

void XSWFPlayer::AS3GetMemInfo(XSortMemName*pInfo)
{
	m_pMemInfo = pInfo;
	
	m_getEvent.Create();
	AddEvent(XGETMEMINFO,0,0,0);
	int nTimes=0;
	while(!m_getEvent.IsEvent(2000))
	{
		nTimes++;
		if(nTimes>20) break;
	}
	m_getEvent.Release();	
}


void XSWFPlayer::GetGLDib(_DIBINFO*pInfo)
{
	if(!pInfo->ch||!pInfo->pDib) return;
	if(pInfo->ch->bits.HasBits())
	{
		SBitmapCore* b=&pInfo->ch->bits;
		pInfo->pDib->Create(b->width,b->height,32);
		memcpy(pInfo->pDib->GetBuffer(),b->baseAddr,b->rowBytes*b->height);
		return;
	}
	_TEXTUREID* id = pInfo->ch->bits.bitmapID;
	if(!id) return;
	RasterGL::sharedRasterGL()->GetTexture(id,*pInfo->pDib);
}

void XSWFPlayer::GetGLDibInfo(XDrawDib*pDib,_XSCharacter*ch)
{
	_DIBINFO info;
	info.ch = ch;
	info.pDib = pDib;
	m_pReturnInfo = &info;
	m_getEvent.Create();
	AddEvent(XGETGLDIB,0,0,0);
	int nTimes=0;
	while(!m_getEvent.IsEvent(2000))
	{
		nTimes++;
		if(nTimes>20) break;
	}
	m_getEvent.Release();
	m_pReturnInfo = NULL;
}

#endif

#ifdef _WINEMU
void XSWFPlayer::GetPlayerList(XU32Array&list)
{
	ScriptPlayer*p=pAS3Player;
	XBOOL bAddRoot=XTRUE;
	while(p)
	{
		list.Add((XU32)p);
		if(p==player)
			bAddRoot=XFALSE;
		p=p->pNextPlayer;
	}		
	if(bAddRoot)
		list.Add((XU32)player);
}
#endif


#include "AS3BitmapData.h"
#include "AS3Bitmap.h"

bool XSWFPlayer::GetObjBitmap(SObject*pObj,XDrawDib&dib,bool bAS)
{
	if(!pObj) return false;
	if(bAS)
	{
		if(!m_pAVM2) return false;
		ScriptObject*pASObject=(ScriptObject*)pObj;
		ShellCore*pCore=m_pAVM2->GetCore();
		ShellToplevel*top=(ShellToplevel*)pCore->GetToplevel();
		BitmapDataClass*pBDClass=top->getBitmapDataClass();
		BitmapClass*pBClass=top->getBitmapClass();
		if(pBClass!=XNULL&&pCore->IsTypeClass(pASObject,(avmplus::ClassClosure*)pBDClass))
		{
			BitmapDataObject*pBmd=(BitmapDataObject*)pObj;
			if(pBmd->m_pChar&&pBmd->m_pChar->bits.HasBits())
			{
				//dib.CreateFrom(&pBmd->m_pChar->bits,false);
				//pBmd->GetDib(dib);
#ifdef _ANALY
                GetGLDibInfo(&dib,pBmd->m_pChar);
#endif
				return true;
				//dib.Create(pBmd->m_char.bits.width,pBmd->m_pDib->Height(),pBmd->m_pDib->m_nBits);
				//memcpy(dib.GetBuffer(),pBmd->m_pDib->GetBuffer(),pBmd->m_pDib->m_nDataLength);
			}
			else return false;
			//pBmd->ExpandDummy(XTRUE);
			//dib.Create(pBmd->m_pDib->Width(),pBmd->m_pDib->Height(),pBmd->m_pDib->m_nBits);
			//memcpy(dib.GetBuffer(),pBmd->m_pDib->GetBuffer(),pBmd->m_pDib->m_nDataLength);
			return true;
		}
		else if(pBClass&&pCore->IsTypeClass(pASObject,(avmplus::ClassClosure*)pBClass))
		{
			BitmapObject*pBmp=(BitmapObject*)pObj;
			if(pBmp->m_pBitmapData)
			{
				BitmapDataObject*pBmd=pBmp->m_pBitmapData;
				if(pBmd->m_pChar)//&&pBmd->m_pChar->bits.HasBits())
				{
					//pBmd->GetDib(dib);
#ifdef _ANALY
					GetGLDibInfo(&dib,pBmd->m_pChar);
#endif
					//dib.CreateFrom(&pBmd->m_pChar->bits,false);
					return true;
					//dib.Create(pBmd->m_char.bits.width,pBmd->m_pDib->Height(),pBmd->m_pDib->m_nBits);
					//memcpy(dib.GetBuffer(),pBmd->m_pDib->GetBuffer(),pBmd->m_pDib->m_nDataLength);
				}
				else return false;
				//pBmd->ExpandDummy(XTRUE);
				//dib.Create(pBmd->m_pDib->Width(),pBmd->m_pDib->Height(),pBmd->m_pDib->m_nBits);
				//memcpy(dib.GetBuffer(),pBmd->m_pDib->GetBuffer(),pBmd->m_pDib->m_nDataLength);
				//return true;
			}
		}
				//m_pAVM2->m_pCore->toplevel();
		//if(m_pAVM2->m_pCore->IsTypeClass(
		return false;
	}
	if(!pObj->character) return false;
	switch(pObj->character->type)
	{
	default:
		/*if(pObj->pCacheBmp)
		{
			dib.Create(pObj->pCacheBmp->Width(),pObj->pCacheBmp->Height(),pObj->pCacheBmp->m_nBits);
			memcpy(dib.GetBuffer(),pObj->pCacheBmp->GetBuffer(),pObj->pCacheBmp->m_nDataLength);
			return true;
		}*/
		break;
	case bitmapChar:
		{
			if(pObj->character->data)
			{
			   XXObjectBitmap*pBitmap=(XXObjectBitmap*)(pObj->character->data);
			   //XDrawDib*pDib=pBitmap->GetBitmap();
			   if(pBitmap->m_pChar)
			   {
#ifdef _ANALY
				   GetGLDibInfo(&dib,pBitmap->m_pChar);
#endif
				   //dib.Create(pDib->Width(),pDib->Height(),pDib->m_nBits);
				   //memcpy(dib.GetBuffer(),pDib->GetBuffer(),pDib->m_nDataLength);
				   //dib.CreateFrom(&pBitmap->m_pChar->bits,false);
				   //pBitmap->GetDib(dib);
				   return true;
			   }
			   //if(pDib)
			   //{
				    
			   //}
			}
		}break;

	case bitsChar:	
		//if(pObj->character->bits.HasBits())
		{
			//dib.CreateFrom(&pObj->character->bits,false);
#ifdef _ANALY
			GetGLDibInfo(&dib,pObj->character);
#endif
			return true;
		}
		 break;
	case bitmapEmptyChar:
		{
			BitmapObject*pBmp = (BitmapObject*)pObj->pASObject;
			if(pBmp&&pBmp->m_pBitmapData)
			{
				//SBitmapCore* bits = &pBmp->m_pBitmapData->m_pChar->bits;
				//dib.CreateFrom(bits,false);
				//pBmp->m_pBitmapData->GetDib(dib);
#ifdef _ANALY
				GetGLDibInfo(&dib,pBmp->m_pBitmapData->m_pChar);
#endif
				return true;
			}
			   /*SBitmapCore* sbitmap=NULL;
			   XDrawDib*pDib=m_pAVM2->GetDrawDib(pObj->pASObject,(void**)&sbitmap);//pBitmap->GetBitmap();
			   if(pDib)
			   {
				    dib.Create(pDib->Width(),pDib->Height(),pDib->m_nBits);
				    memcpy(dib.GetBuffer(),pDib->GetBuffer(),pDib->m_nDataLength);
					return true;
			   }
			   else if(sbitmap)
			   {
					dib.CreateFrom(&pObj->character->bits,false);
					return true;
			   }*/
		}
		 break;
	}
	return false;
}

bool XSWFPlayer::Reload()
{
	if(!pWindow) return false;
	pWindow->PostMsg(XCMD_FLASHCOMMAND,XFLASH_REFLASH,0);
	return true;
}

bool XSWFPlayer::IsJQBar(XPCTSTR strURL)
{
	XString8 url(strURL);
	url.MakeLower();
	if(url.Find(".jqbar.")>0) return true;
	if(url.Find(".playbyone.")>0) return true;
	return false;
}

bool XSWFPlayer::RunSWF(XPCTSTR name)
{
	if(!pWindow) return false;
	//XString8 strFile = this->m_strFilePath;
	//strFile+=name;
	XString8 str(name);
	str.MakeLower();
	if(str.Find("http://")<0)
	{
		XFile file;
		if(!file.Open(name,XFile::XBINARY|XFile::XREAD)) return false;
		file.Close();
	}
	if(pWindow)
		pWindow->Handle(XCMD_FLASHCOMMAND,XFLASH_RELOAD,(XU32)name);
	return true;
}


//MATRIX m_beginMat;
//	bool   m_bZooming;
//	SRECT  m_startArea;
void  XSWFPlayer::StartZoom(SRECT&r)
{
	m_bZooming = true;
	m_startArea = r;
	m_beginMat = display.mControlMat;
}

int	 XSWFPlayer::SetZoom(SRECT&r)
{
    float dx0 = 0.0f;
    float dy0 = 0.0f;
    float dx1 = 0.0f;
    float dy1 = 0.0f;
    float cx0 = 0.0f;
    float cy0 = 0.0f;
    float cx1 = 0.0f;
    float cy1 = 0.0f;
    float fx = 0.0f;
    float fy = 0.0f;
    float f = 0.0f;
    MATRIX m;
    MATRIX&mat = display.mControlMat;
	if(!m_bZooming)
		goto EndSet;
	dx0=m_startArea.xmax-m_startArea.xmin;
	dy0=m_startArea.ymax-m_startArea.ymin;
	if(dx0<=0&&dy0<=0)
		goto EndSet;
	dx1=r.xmax-r.xmin;
	dy1=r.ymax-r.ymin;
	if(dx1<=0&&dy1<=0)
		goto EndSet;
	cx0=(m_startArea.xmin+m_startArea.xmax)/2.0;
	cy0=(m_startArea.ymin+m_startArea.ymax)/2.0;
	cx1=(r.xmin+r.xmax)/2.0;
	cy1=(r.ymin+r.ymax)/2.0;
	
	
	fx = dx1/dx0;
	fy = dy1/dy0;
	f=fx>fy?fx:fy;
	
	::MatrixIdentity(&m);
	m.a = m.d = f*fixed_1;
	::MatrixConcat(&m_beginMat,&m,&mat);
	if(mat.a<fixed_1) mat.a=fixed_1;
	else if(mat.a>fixed_1*5) mat.a=fixed_1*5;
	mat.d=mat.a;
	mat.tx = (cx1-cx0)*20+m_beginMat.tx;
	mat.ty = (cy1-cy0)*20+m_beginMat.ty;
	
	
	//display.ModifyCamera(false);
	
	CalcLimit();
	if(mat.tx>0) mat.tx=0;
	if(mat.ty>0) mat.ty=0;
	if(mat.tx<-m_nLimitX) mat.tx=-m_nLimitX;
	if(mat.ty<-m_nLimitY) mat.ty=-m_nLimitY;
	display.UpdateMat();
EndSet:
	return (int)(display.mControlMat.a*100/fixed_1);
}
	
void XSWFPlayer::EndZoom()
{
	m_bZooming = false;
	UpdateAll();
}

void XSWFPlayer::UpdateAll()
{
	//display.root.Modify();
	display.ModifyCamera(true);
}

void XSWFPlayer::DataStart(int nMax)
{
	if(!pWindow) return;
	pWindow->PostMsg(XWM_UPDATEMSG,XCMD_UPDATE_DATASTART,nMax,XMS_TOPMOST);
}
void XSWFPlayer::DataSet(int nProc)
{
	if(!pWindow) return;
   // pWindow->PostMsg(XWM_APPLE_MESSAGE, XCMD_UPDATE_SET, nProc, XMS_TOPMOST);
	pWindow->PostMsg(XWM_UPDATEMSG,XCMD_UPDATE_DATASET,nProc,XMS_TOPMOST);
}
void XSWFPlayer::DataEnd()
{
	if(!pWindow) return;

	pWindow->PostMsg(XWM_UPDATEMSG,XCMD_UPDATE_DATAEND,0,XMS_TOPMOST);

}