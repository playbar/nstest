/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc.
****************************************************************************/

#ifndef SDISPLAY_INCLUDED
#define SDISPLAY_INCLUDED


#ifndef SOBJECT_INCLUDED
#include "sobject.h"
#endif
#ifndef STAGS_INCLUDED
#include "stags.h"
#endif

#include "XDrawDib.h"
//class XXVARLIST;
#include "XXVar.h"
#include "XXStack.h"
#include "Raster_gl.h"
#include "Raster.h"
struct _XSObject;
struct _XSCharacter;
class ScriptPlayer;
class ScriptThread;
class EditText;

// #ifdef SPRITE_XTRA
// //  Display transform data
// //  All points should be in standard twips coordinates
//
// typedef struct TDisplayXForm
// {
//     SFIXED			scale;
//     SFIXED          rotation;
//     SCOORD          originH;
//     SCOORD          originV;
//     SFIXED          viewScale;
//     SFIXED          viewRotation;
//     SCOORD          viewH;
//     SCOORD          viewV;
//     SRECT	        docRect;
// 	ColorTransform	cxform;
// 	SFIXED			skew;
// 	SCOORD			width;
// 	SCOORD			height;
// 	BOOL			bFlipX;
// 	BOOL			bFlipY;
// } TDisplayXForm, * PTDisplayXForm;
//
// #endif // ..SPRITE_XTRA

typedef struct _XPlaceInfo : STransform {
	int flags,flags3;
	SCharacter* character;
	int depth;
	U16 ratio;
	bool bAdd;
    int clipDepth;
    U8 puppet;
	XXVar name;
	XXVar className;
	U8 blendMode;
	U8 cacheMode;
	U32 nEventFlags;
	U8 bClone,bImage;
	_XFilter*pFilter;
	_XClipAction*pAction;
    _XPlaceInfo()
    {
		nEventFlags=0;
		blendMode=0;
		bImage=0;
		cxform.nBlendMode=0;
		cacheMode=0;
		pFilter=XNULL;
		pAction=XNULL;
		bClone=XFALSE;
		name=_strEmpty;
		className=_strEmpty;
		//cxform.nForeColor=0;
	    clipDepth = ratio = depth = flags = 0;
        puppet = 0;
	    character = 0;
	    name = 0;
		className=0;
		bAdd=false;
    }
}PlaceInfo;

// Button states
enum {
	bsNull = 0,		// the button is not initialized
	bsIdle,			// the button is idle
	bsOverUp,		// the mouse is up and over the button
	bsOverDown,		// the mouse is down and over the button
	bsOutDown,		// the mouse is down and outside the button
	bsHitTest
};

class XActionList
{
public:
	struct XActionEntry
	{
#pragma pack(1)
		SObject *actionBase;
		XXObject*actionThis;
		XU8		*actionEntry;
		XU32	actionFlag;
		XXVARLIST*actionArgs;
	//	XU32	actionLength;
#pragma pack()
	};
public:
	// Current Action list
	enum { maxActionsLimit = 4096 };
	enum {XA_TYPEMASK=0x6,
		  XA_TOP=0x01,
		  XA_CODE=0,XA_FUNCTION=0x02,
		  XA_BLOCK=0x10
		};
    U32 maxActions;
    U32 actionMask;
    void FreeAll();
	void FreeOne(XU32 i,XBOOL bForce);
	XActionEntry* entries;
	//U8 ** actionList;//,**actionEnd;
	//U8 *  actionFlags;
	//ScriptThread** actionThread;
	//SObject**actionObject;
	//XXObject**actionThis;
	U32 actionBase;
	U32 actionTop;

	XActionList();
	~XActionList();
    void PushAction(U8 * a,SObject* t,XXObject*pThis,XBOOL bTop,XXVARLIST*pArgs);
	void RemoveObject(SObject*pObject);
};
typedef XActionList ActionList;

typedef struct _XActionContext
{
	ActionList		*actionList;
	ScriptThread	*targetThread;
	BOOL			 useTarget;
}ActionContext;

#define MaxActionCallStack 256
class XSWFPlayer;

extern void FreeCache(SObject*parent);
//#define antiShift 2

class DisplayList {
//public:
//	bool m_bIsLocked;
//	void Lock(){m_bIsLocked=true;}
//	void UnLock(){m_bIsLocked=false;}
public:
	float	   mfPadW,mfPadH;
	XFastAlloc cmdAlloc;
	XFastAlloc infoAlloc;
	AS3ButtonInfomation* AllocInfo()
	{
		return (AS3ButtonInfomation*)infoAlloc.Alloc();
	}
	void FreeInfo(AS3ButtonInfomation*info)
	{
		while(info)
		{
			AS3ButtonInfomation*next=info->pNext;
			infoAlloc.Free(info);
			info=next;
		}
	}
	enum {XCMD_PLAY,XCMD_STOP,XCMD_GOTOANDSTOP,XCMD_GOTOANDPLAY,XCMD_GOTOANDKEEP};
	struct XCTRLCMD
	{
		int nCmd;
		int nFrame;
		XCTRLCMD*pNext;
	};
	XCTRLCMD* allocCmd(int nCmd,int nFrame)
	{
		XCTRLCMD* cmd=(XCTRLCMD*)cmdAlloc.Alloc();
		cmd->nCmd=nCmd;
		cmd->nFrame=nFrame;
		return cmd;
	}
	void freeCmd(XCTRLCMD*cmd)
	{
		cmdAlloc.Free(cmd);
	}
public:
	SObject* AS3CreateObject(PlaceInfo*info);
	void AS3SetStateObjects(SObject*obj,void*pInstance,int nState);
	void AS3PlaceObject(SObject*parent,SObject*object,int nDepth);
	void AS3ToTop(SObject*parent,SObject*object);
	bool AS3RemoveObject(SObject*parent,SObject*object);
	SObject* AS3RemoveObject(SObject*parent,int i);
	bool AS3SetChildIndex(SObject*parent,SObject*obj,int i);
	bool AS3SwapChild(SObject*parent,SObject*obj1,SObject*obj2);
	bool AS3SwapChild(SObject*parent,int i1,int i2);

public:
	// The Object List
	STransform camera;
	_XSObject root;
	
	// The Frame Buffer
	SRGB backgroundColor;
	int backgroundColorPriority;
//	XU8	 antiShift;
//	CBitBuffer* bits;		// the context for drawing into
//////////////////////////////////////////////////////////////////
//  for ANTI1
	//int antiShift;
	//BOOL antialias_b;
	BOOL faster;
	BOOL useDeviceFont;
	BOOL hasDeviceEditText;
	XSWFPlayer*player;
	BOOL dirty;
	SRECT devViewRect;		// the visible area of the bitmap in device coords
	SRECT screenDirtyRgn;	// the area of the bits that needs to be copied to the screen
	SRECT devDirtyRgn;		// the union of the dirty areas
	enum { maxDirtyAreas = 4 };
	int nDirty,nAspect;
	S32 devDirtyArea[maxDirtyAreas];	// the portion of the bitmap that needs to be redrawn in device coords
	SRECT devDirtyRect[maxDirtyAreas];	// the portion of the bitmap that needs to be redrawn in device coords

	RasterGL *mpRasterGL;
	CRaster raster;
	//CRaster raster,*pRaster;

	_XSObject* holdList;
	_XSObject* holdParent;

	// The Thread List
	ScriptThread* threads;

	// Current button info
	_XSObject* button;
	bool m_bInvalidate;
	int buttonState;//,threadState;//,threadState;			// these state fields are used to maintain a buttons state through a place/remove tag
	SRECT tabFocusRect;
	BOOL useFocusRect;
	_XSObject* buttonParent;
	SCharacter* buttonCharacter;
	int buttonDepth;
	int buttonRatio;


#ifdef EDITTEXT
	// Text editing
	BOOL iBeam;
#endif
	bool bFocusRect;
	char actionURL[512];

	int instanceNameCount;

#ifdef SOUND
	// Initialized be the Splayer if we are using sound.
	XSoundMix* theSoundMix;
#endif



public:
	// Memory Allocators
	//XChunkAlloc edgeAlloc;
	//XChunkAlloc objectAlloc;
	//XChunkAlloc colorAlloc;
	//XChunkAlloc commandAlloc;
	//XChunkAlloc	curveAlloc;
	XFastAlloc edgeAlloc;
	XFastAlloc objectAlloc;
	XFastAlloc colorAlloc;
	XFastAlloc commandAlloc;
	XFastAlloc curveAlloc;
	XFastAlloc formatAlloc;
	XFastAlloc loadAlloc;
	XFastAlloc editAlloc;
	XFastAlloc threadAlloc;
	XFastAlloc rectAlloc;
	XFastAlloc matrixAlloc;

	MATRIX*	CreateMatrix(){return (MATRIX*)matrixAlloc.Alloc();}
	void FreeMatrix(MATRIX*m){matrixAlloc.Free(m);}
	inline SRECT* CreateRect(){return (SRECT*)rectAlloc.Alloc();}
	inline void   FreeRect(SRECT*rect){rectAlloc.Free(rect);}
	inline class ScriptThread* CreateThread(){return (ScriptThread*)threadAlloc.Alloc();}
	inline void FreeThread(ScriptThread*thread){threadAlloc.Free(thread);}
	inline EditText* CreateEditText(){return (EditText*)editAlloc.Alloc();}
	inline void FreeEditText(EditText*thread){editAlloc.Free(thread);}
	inline struct _XLoadInfo* CreateLoad(){return (_XLoadInfo*)loadAlloc.Alloc();}
	inline void FreeLoad(struct _XLoadInfo*p){return loadAlloc.Free(p);}
	inline struct _XTEXTFORMAT* CreateFormat(){return (_XTEXTFORMAT*)formatAlloc.Alloc();}
	inline void FreeFormat(struct _XTEXTFORMAT*p){formatAlloc.Free(p);}
	inline XCOMMAND* CreateCommand(){return (XCOMMAND*)commandAlloc.Alloc();}
	inline P_CURVE CreateCurve(){return (P_CURVE)curveAlloc.Alloc();}
	inline void FreeCommand(XCOMMAND*c){commandAlloc.Free(c);}
	inline void FreeCurve(P_CURVE c){curveAlloc.Free(c);}
	inline RColor* CreateColor() { return (RColor*)(colorAlloc.Alloc());}
	inline void FreeColor(RColor* c) { c->FreeCache(); colorAlloc.Free(c); }

	inline REdge* CreateEdge() { return (REdge*)(edgeAlloc.Alloc()); }
	inline void FreeEdge(REdge* e) { edgeAlloc.Free(e); }

	_XSObject* CreateObject(ScriptPlayer*player,XU32 t);
	//void FreeRoot(_XSObject*o);
	void FreeObject(_XSObject* o);
	inline bool IsFreeObject(_XSObject*o)
	{
		XFastAlloc::XBLOCK*p=((XFastAlloc::XBLOCK*)o)-1;
		XFastAlloc::XBLOCK*pBlock=objectAlloc.m_pFreeList;
		while(pBlock)
		{
			if(pBlock==p) return true;
			pBlock=pBlock->pNext;
		}
		return false;
	}
	/*inline void FreeObjs()
	{
		SObject*p=freeObjs;
		freeObjs=XNULL;
		while(p)
		{
			SObject*next=p->pLink;
			p->FreeContext();
			objectAlloc.Free(p);
			p=next;
		}
	}*/

	BOOL MergeDirtyList(BOOL forceMerge);
	void DecomposeDirtyList();

	void UpdateList(_XSObject* parent, BOOL matChanged);

	void AddThread(ScriptThread* thread);
	void RemoveThread(ScriptThread* thread);

	void UpdateButton(_XSObject* obj, int newState,bool bAS3=false);
	void DoButtonAction(_XSObject* target, int transition);
	void DoButtonStateTransition(_XSObject* newButton, int transition);

	void DoRemove(_XSObject**,bool bEvent,bool bFinal);

public:
	void GetCameraMatrix(MATRIX&mat);
	ActionList actionList;

//	ActionContext actionCallStack[MaxActionCallStack];
//	int actionCallStackTop;
//	SObject* freeObjs;
public:
	//XBOOL HasAnyEvent(_XSObject*target);
	void ShutDownAlloc();
	void FreeAll();
	void RemoveAllThread(ScriptPlayer*pl);
	DisplayList();
	~DisplayList();

	void Reset();
	void Invalidate()
	{
		InvalidateRect(&devViewRect); 
	} // set the dirty area to the whole view
	void InvalidateRect(SRECT*);

	void FreeCache();	// free the cached data to reduce memory load

	//enum 
	//{ 
	//	showAll=0, noBorder=1, exactFit=2, noScale=3, fitDev=4,fit50=5,fit100=6,fitFull=7,autoFit=8,scaleMask = 0x0F,
	//	alignLeft = 0x10, alignRight = 0x20, alignTop = 0x40, alignBottom = 0x80,
	//	zoom100Scroll = 0x1000 // this is a special mode used by the editor only
	//};

// #ifdef SPRITE_XTRA		// !!sprite_xtra
// 	void SetCamera(SRECT* frame, SRECT* viewRct, BOOL smooth, int scaleMode=showAll, PTDisplayXForm pXform = NULL);
// #else
	void TransMat( float x, float y );
	void ScaleMat( float fa, float fd );
	void ScaleTranMat( float fa, float fd, SPOINT pos );
	void ZoomMat( SRECT *rect1, SRECT *rect2 );
	void UpdateMat();
	void SetCamera(SRECT* frame );
	MATRIX mControlMat;
// #endif
	void ModifyCamera(bool bModify);
	void UpdateDevViewRect();
	void SetBackgroundColor(SRGB, int priority = 1);

	_XSObject* PlaceObject2(_XSObject* parent, PlaceInfo* info,XSWFCONTEXT*pCnt,bool bInstance,ScriptPlayer*player=NULL);//XBOOL bUpdate);
	_XSObject* PlaceObject3(_XSObject* parent, PlaceInfo* info,void*pCnt);//XBOOL bUpdate);
	void RemoveObject(_XSObject* parent, int depth);
	void RemoveObject(_XSObject* parent, _XSObject*obj);
	_XSObject* MoveObject(_XSObject* parent, PlaceInfo* info);

	void BeginHold(_XSObject* parent);	// hold the place/removes until we finish seeking
	void FinishHold(bool bAVM2);

	void CalcUpdate();
	void UpdateRect(SRECT*);	// rebuild a specific area of the offscreen buffer
	void Update();	// rebuild the offscreen buffer
	void CalcBitsDirty(SRECT* devDirty, SRECT* bitsDirtyRgn);	// convert a dev area to a bits area
	//void CalcBitsDirty(SRECT* r) { CalcUpdate(); CalcBitsDirty(&devDirtyRgn, r); }	// calc the offsreen area that needs to be rebuilt

	//_XSObject* HitButton(_XSObject*&hitClip,SPOINT*);
	BOOL SetButton(_XSObject* newButton, BOOL mouseIsDown);

	_XSObject* HitObject(SPOINT*pt,_XSObject**hitObj,XU32 t);
	//_XSObject* HitTest(SPOINT*);
    void PushAction(U8 * a, SObject* t,XXObject*pThis,XBOOL bTop=XFALSE,XXVARLIST*pArgs=XNULL)
	{ actionList.PushAction(a, t,pThis,bTop,pArgs); }

// #ifdef SPRITE_XTRA
//    void SetImageEnabled(BOOL bEnabled) { fbImageEnabled = bEnabled; }
// #endif
};

extern S32 RectArea(SRECT* area);

#endif
