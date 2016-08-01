/*
 ============================================================================
 Name		: GGBase.h
 Author	  : xie peifu
 Copyright   : Copyright @jqbar
 Description : Static lib header file
 ============================================================================
 */

#ifndef __GGBASE_H__
#define __GGBASE_H__


//#include "../BwVersion.h"

#define SOUND
#define EDITTEXT

#ifdef _SYMBIAN
#include <e32base.h>
#define _stdcall
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include "ImageThread.h"
//  Function Prototypes
extern TInt DummyFunction();

#elif defined (_WIN32)
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <ctype.h>
#elif defined (__APPLE__)
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
    #include "BaiWan_Config.h"
    #include "module.h"
#elif defined( __ANDROID__ )
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif

#include "GGType.h"
#include "XVector.h"
#include "XString.h"
#include "XMutex.h"
#include "XStream.h"
#include "XImage.h"
#include "XData.h"
#include "XGraphics.h"
#include "XWindow.h"
#include "XTabControl.h"
#include "XVScrollBar.h"
#include "XHScrollBar.h"
#include "XMenu.h"
#include "XDialog.h"
#include "XButton.h"
#include "XTime.h"
#include "XEdit.h"
#include "XSocket.h"
#include "XFile.h"
#include "XResource.h"
#include "XListBox.h"
#include "XSort.h"
#include "XZLib.h"
#include "XNativeSoundMix.h"
#include "XFastAlloc.h"

//#include "zlib.h"
//  Include Files

#ifdef __APPLE__
#include "IPhoneDeviceData.h"
#endif


//#pragma comment(lib,"GGBase.lib")
#ifdef _ANALY
struct _XCALLENTRY
{
	XString8 strFunction;
	XU32	 nTimes;
	float	 nPecent;
	XU32     bExtern;
	XS64	 nMaxTime;
	XS64	 nTotalTime;
	XS64	 nInTime;
	XS64	 nOutTime;
	_XCALLENTRY*pNext;
	_XCALLENTRY*pChild;
	_XCALLENTRY*pParent;
};
struct _XCALLGRAPH
{
	_XCALLGRAPH*pNext;
	XString8 strFunction;
	XU16	 nCount,nMaxCount;
	XU64	 nCurData,nEnterTime,nMaxTime;
	XU64	 *pDatas;
	XU32	 nPosition;
	XU32	 nDataLength;
};

extern XBOOL _ANGraphEnter(XPCTSTR strFunction,int nCount);
extern void  _ANGraphLeave();
extern _XCALLGRAPH* _ANGetGraph();
class _XCallGraphS
{
	XBOOL m_bPush;
public:
	_XCallGraphS(XPCTSTR function,int nCount){m_bPush=_ANGraphEnter(function,nCount);}
	~_XCallGraphS(){if(m_bPush) _ANGraphLeave();}
};
extern XBOOL _ANEntryFunction(XPCTSTR strFunction);
extern void _ANLeaveFunction();
extern _XCALLENTRY* _ANGetRoot();
extern XS64 _ANGetStartTime();
extern XS64 _ANGetNowTime();
extern XS64 _ANGetTotalTime();
extern XU32 _ANGetStartTick();
extern void _ANStop();
extern void _ANStart();
extern void _ANClear();
extern XBOOL _ANIsStarted();
extern void _ANDisable(XU32 id);
extern void _ANEnable(XU32 id);
extern XBOOL _ANIsDisabled(XU32 id);
class _XCallEntryS
{
	XBOOL m_bPush;
public:
	_XCallEntryS(XPCTSTR function){m_bPush=_ANEntryFunction(function);}
	~_XCallEntryS(){if(m_bPush) _ANLeaveFunction();}
};

#define _ANALYENTER(TOKEN) _XCallEntryS _analyentry(TOKEN)
#define _ANAGRAPH(TOKEN,COUNT) _XCallGraphS _analyGraph(TOKEN,COUNT)
//_ANEntryFunction(TOKEN)
//#define _ANALYLEAVE() _ANLeaveFunction()

#else
#define _ANALYENTER(TOKEN)
#define _ANAGRAPH(TOKEN,COUNT)
//#define _ANALYLEAVE()
#endif

#ifdef __cplusplus
extern "C" {
#endif

XBOOL STDCALL XInitGGBase();
void  STDCALL XReleaseGGBase();

#ifdef __cplusplus
}
#endif

#endif  // __GGBASE_H__

