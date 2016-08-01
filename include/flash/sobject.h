/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc.
****************************************************************************/

// The object is responsible for generating edges and colors in a format
//	that the scan converter can use.

#ifndef SOBJECT_INCLUDED
#define SOBJECT_INCLUDED

#include "XXVar.h"
#include "BwShapeRecord.h"
//#ifndef RASTER_INCLUDED
//#endif
#include "Raster_gl.h"
#include "sbitmap.h"
#include "curve.h"


struct AS3ButtonInfomation
{
	public:
		enum {BI_MOUSEDOWN =0x0001,
			  BI_MOUSEUP   =0x0002,
			  BI_MOUSECLICK=0x0004};
		struct _XSObject*	button;
		XU32		nLength;
		XU32		nImportant;
		XU32		nFlag;
		int			nDeltaX;
		int			nDeltaY;
		SRECT		buttonBounds;
		AS3ButtonInfomation* pNext;
};

#define GETBOUNDS(CH,EDIT)\
	(EDIT&&(CH->type==editTextChar||CH->type==editTextEmptyChar||CH->type==textLineChar)?EDIT->editRect:CH->rbounds)
/*	{
		switch(character->type)
		{
		case editTextChar:
		case editTextEmptyChar:
			 return editText->editRect;
		default:
			 return character->bounds;
		}
	}*/


class DisplayList;
class ScriptPlayer;
class ScriptThread;
struct _XSCharacter;

//
// The Character Maintains the unmapped edges of an object
//



//struct BitsCharInfo {
//	SBitmapCore bm;
//};
class XSWFCONTEXT;
typedef struct _XButtonCharInfo {
	U8 * soundData;
	U8 * cxformData;
	U8 trackAsMenu;		// set if we should track as a menu instead of as a push button
	//XS32 length;
}ButtonCharInfo;

enum { textUnknown, textDeviceOK, textNoDevice, textDync };
#define XDOT_FONT16 0x8000
#define XDOT_FONT12 0x4000
#define XDOT_FONT8  0x2000
#define XDOT_MASK   0xf000

typedef struct _XFontCharInfo {
	U8 * infoData;
	int deviceState;
	U16 nGlyphs;
	U16 flags;
	GLTranglesOrLines** shapeData;
//	GLColor**			shapeColor;
	U8  fLen;
	char* strFontName;
// [gsg] nDataOffset now used by player code (edit text) as well as authoring tool
//#ifdef _CELANIMATOR
	S32  nDataOffset;			// used for font2 and import
	XU16* GetCodes(){return ((XU16*)infoData);}
	XU16* GetAdvance(){return ((XU16*)infoData)+nGlyphs;}
	XU32* GetOffset(){return ((XU32*)infoData)+nGlyphs;}
	XU32* GetAddress(){return (XU32*)(infoData+nGlyphs*8+4);}
//#endif
}FontCharInfo;


typedef struct _XTextCharInfo {
	int deviceState;
}TextCharInfo;

typedef struct _XSpriteCharInfo {
	int numFrames;
	U32 length;
}SpriteCharInfo;

typedef struct _XMorphCharInfo {
	SRECT bounds2;
}MorphCharInfo;

#ifdef EDITTEXT
class EditText;
#endif

enum {
	shapeChar		    = 0,
	bitsChar		    = 1,
	buttonChar		    = 2,
	fontChar		    = 3,
	textChar		    = 4,
	soundChar		    = 5,
	spriteChar		    = 6,
	morphShapeChar	    = 7,
	commandChar		    = 8,			// used for the Flash Generator
	editTextChar	    = 9,
	videoChar			= 10,
	spriteEmptyChar		= 11,
	editTextEmptyChar	= 12,
	buttonEmptyChar		= 13,
	shapeEmptyChar		= 14,
	bitmapEmptyChar		= 15,
	binaryDataChar		= 16,
	playerChar			= 17,
	videoEmptyChar		= 18,
	loaderChar			= 19,
	loaderImageChar		= 20,
	scrollChar			= 21,
	loaderSWFChar		= 22,
	rootCloneChar		= 23,
	textLineChar		= 24,
//#if (__CORE_VERSION__>=0x02073000)
	fontNameChar		= 25,
//#endif
	//grid9Char		    = 22,

	//drawChar			= 95,
	bitmapChar			= 96,
    spriteExternalChar  = 97,
	rootChar		    = 98,
	lostChar		    = 99
};

typedef struct _XShapeCharInfo
{
	XU8 flags;
	SRECT edgeRect;
}ShapeCharInfo;

typedef struct _XCOMMAND
{
	XU8		nCommand;
//	int     tmp[4];
	XU32	nAttachData;
	int		nLineData;
	union
	{
		P_CURVE pCurve;
		GLColor* pColor;
	};
	_XCOMMAND*pNext;
}XCOMMAND;

typedef struct _XCommandInfo
{
	_XCOMMAND* head;
	_XCOMMAND* tail;
}XCommandInfo;

struct XVideo
{
	U16 numFrames;
	U16 curFrame;
	U16 width;
	U16 height;
	U8	flag;
	U8  codecID;
};


typedef struct _XSCharacter {
	_XSCharacter* next;
	ScriptPlayer* splayer;
	U16 tag;
	U8 type;
	U8 tagCode;			// the stag code from the define tag
	U8 * data;	// this must be first
	XS32 length;
	//XU32 useCount;
	XU32 placeCount;
	char* className;
	void* classEntry;
	char* gridRect;
	SRECT rbounds;
	BwShapeRecord record;
#ifdef _WINEMU
	bool bFilters;
#endif
	union {
		ShapeCharInfo shape;
		ButtonCharInfo button;
		SBitmapCore bits;
		//FrameTexture* frame;
		FontCharInfo font;
		TextCharInfo text;
		SpriteCharInfo sprite;
		MorphCharInfo morph;
		XCommandInfo cmds;
#ifdef SOUND
		XSound sound;
#endif
		XVideo video;
	};
}SCharacter;

/*typedef struct _InitAction
{
	XU8* pCode;
	XU32 nLength;
	XU8  bInit;
	_InitAction*pNext;
}InitAction;*/
//
// The Object keeps track of a set of colors and edges
// 	That have been mapped to the render device
//

typedef struct _XSTransform {
	MATRIX mat;
	ColorTransform cxform;
	//XU32 nForeColor,nBackColor;
	void Concat(_XSTransform* child) {
		MatrixConcat(&child->mat, &mat, &mat);
		cxform.Concat(&child->cxform);
	}
	void Clear() {
		MatrixIdentity(&mat);
		cxform.Clear();
	}
}STransform;

inline BOOL Equal(STransform* a, STransform* b)
{
	return MatrixEqual(&a->mat, &b->mat, 0) && Equal(&a->cxform, &b->cxform);
}

enum
{
	CLIPKEYUP=0x80,
	  CLIPKEYDOWN=0x40,
	  CLIPPOINTUP=0x20,
	  CLIPPOINTDOWN=0x10,
	  CLIPPOINTMOVE=0x08,
	  CLIPUNLOAD=0x04,
	  CLIPENTERFRAME=0x02,
	  CLIPLOAD=0x01,
	  CLIPDRAGOVER=0x8000,
	  CLIPROLLOUT=0x4000,
	  CLIPROLLOVER=0x2000,
	  CLIPRELEASEOUTSIDE=0x1000,
	  CLIPRELEASE=0x0800,
	  CLIPPRESS=0x0400,
	  CLIPINIT=0x0200,
	  CLIPEVENTDATA=0x0100,
	  CLIPCONSTRUCT=0x040000,
	  CLIPKEYPRESS= 0x020000,
	  CLIPDRAGOUT=0x010000,
};

#define CLIPMOUSEEVENT (CLIPPOINTMOVE|CLIPPOINTDOWN|CLIPPOINTUP|CLIPDRAGOUT|CLIPDRAGOVER|CLIPPRESS|CLIPRELEASE|CLIPRELEASEOUTSIDE|CLIPROLLOUT|CLIPROLLOVER)
#define CLIPKEYEVENT (CLIPKEYPRESS|CLIPKEYUP|CLIPKEYDOWN)

typedef struct _XClipAction
{
	XU8* pCode;
	XU32 nCodeLength;
	XU32 nEventFlags;
	XU8 nKeyCode;
	_XClipAction* pNext;
}XClipAction;
//
// Display list update model
//
//  place - put the object on the display, mark as dirty
//	move - change transform, free cache mark as dirty
//	remove - if drawn, add inval rect
//	update - if dirty, calc devMat and devBounds, inval rect
//	freeCache - free cache
struct _XFilter;
struct _XClipAction;
class XXObject;
class XSWFPlayer;
class XXVar;

#define OBJECTOFS(SS) (SS?SS->pObject:XNULL)

#define AS3DECLAREEVENT(NAME) XBOOL AS3_On##NAME()
#define AS3ONEVENT(CLASS,NAME)\
XBOOL CLASS::AS3_On##NAME()\
{\
	if(pASObject)\
	{\
		return display->player->m_pAVM2->OnEvent(avmshell::XAS3_EVENT_##NAME,false,pASObject,GetContext() );\
	}\
	return false;\
}

/*#define AS3DECLAREEVENTSTRING(NAME) XBOOL AS3_On##NAME(U16*,int l)
#define AS3ONEVENTSTRING(CLASS,NAME)\
XBOOL CLASS::AS3_On##NAME(U16* p,int l)\
{\
	if(pASObject)\
	{\
		avmshell::XAVM2*pAVM2=character->player->m_pAVM2;\
		//pEvent=pAVM2->CreateEventObject(avmshell::XAS3_EVENT_##NAME,avmshell::XAS3EVENT_TEXT,false,l,(int*)p);\
		//if(pEvent)\
		//	return pAVM2->DispatchEvent(pEvent,pASObject);\
	}\
	return false;\
}*/
#ifdef _WINEMU
extern int _nDebugTimes;
#endif


typedef struct _XCONTROL
{
	SFIXED rotX,rotY;
	SFIXED scaleX;
	SFIXED scaleY;
	//XFDOUBLE
	XFDOUBLE sx;
	XFDOUBLE sy;
	XFDOUBLE rotate;
	//XFDOUBLE ry;
}XCONTROL;

typedef struct _XSObject
{
	enum 
	{
		OB_VISIABLE=0x01,
		  OB_ENABLE=0x02,
		  OB_OBJECT=0x04,
		  OB_DRAW=0x03,
		  OB_CLONEOBJ=0x08,
		  OB_AS3FLAG=0x10,
		};
/*	struct _NamedObject
	{
		char*	  name;
		_XSObject* pObject;
	};*/
	/*class XSortObject:public XSort
	{
	public:
		XU32 GetCount(){return m_objs.GetSize();}
		XINT Compare(void*iSrt,void*iDst,XU8 nMode)
		{
			return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nMode);
		}
		void* GetData(XU32 iData){return (void*)m_objs[iData]->name;}
		XVector<_XSObject*> m_objs;
		void Release()
		{
			m_objs.RemoveAll();
		}
		void Add(_XSObject*pObject,XBOOL bLowCase)
		{
			int im=0;
			int id=Index((void*)pObject,im,bLowCase?XSS_NOCASE:XSS_NORMAL);
			if(id>=0) m_objs[id]=pObject;
			else
			{
//				_NamedObject obj={name,pObject};
				m_objs.InsertAt(im,pObject);
			}
		}
		void Remove(char* name,XBOOL bLowCase)
		{
			int id=Index((void*)name,bLowCase?XSS_NOCASE:XSS_NORMAL);
			if(id>=0)
				m_objs.RemoveAt(id);
		}
		_XSObject* Find(char* name,XBOOL bLowCase)
		{
			int id=Index((void*)name,bLowCase?XSS_NOCASE:XSS_NORMAL);
			if(id>=0)
				return m_objs[id];
			else
				return XNULL;
		}
	};
	XSortObject* sorts;*/
	//////////////////////////////////////////////////

	DisplayList* display;	// the display tree
	_XSObject* pLink,*pLinkBy;
	//_XSObject* pPrev,*pNext;

	ScriptPlayer*CreatePlayer;
	_XSObject*   parent;
	_XSObject*   above;
	_XSObject*   clipObject;
	_XSObject*   bottomChild;
	//_XSObject*	 scrollObject;
	_XFilter*    pFilter;
	SRECT*		 scrollRect;
	SRECT*		 gridRect;
	//_XSObject*	 gridObjects;
	XXObject*	 pObject;
	void*		 pASObject;
	void*		 pSuper;
	void GetName(XString16&name);
//	_XClipAction*pAction;
	SCharacter* character;
	int depth,oldDepth;			// the z order where this object should be drawn
	U16 ratio;
	XXVar name;
	//XString8 strName;

	ScriptThread* thread;	// the thread that owns this object
	STransform xform;

	SRECT devBounds;

    U8 puppet;              // set to true if the user controlls the xform
	U8 state;				// the button state for a button
	U8 drawn;				// true if this object has been drawn and needs to be erased
	U8 dirty;
	U8 dirty1;
   //U8 visible,enable;             // if set to zero we dont draw or hit test the object
	U8 flags;
	XU32 exflags;
	enum 
	{
		XEN_DOUBLECLICKED=0x01,
		  XEN_FOCUSRECT=0x02,
		  XEN_MOUSEDENABLED=0x04,
		  XEN_TABENABLED=0x08,
		  XEN_MOUSECHIDREN=0x10,
		  XEN_TABCHILDREN=0x20,
		  XEN_NOTBUTTONMODE=0x40,
		  XEN_HANDCURSOR=0x80,
		  XEN_TRACKASMENU=0x100,
		  XEN_MOUSEWHEEL=0x200,
		  XEN_NOTINSTANCE=0x400,
		  XEN_NOTINITCHILD=0x800,
		  XEN_ISAS3NAMED=0x1000,
		  XEN_CALLFRAMEOK=0x2000,
		  XEN_NOTCALLFRAME=0x4000,
		  XEN_NOTINITAS2  =0x8000,
		  XEN_ORGBITMAP=0x10000,
		  XEN_FILTEROBJ=  0x20000,
//#if (__CORE_VERSION__>=0x02077000)
		  XEN_FOCUSENABLE=0x40000,
//#endif
		  //XEN_INDISPLAY=0x8000,
		  //XEN_BITMAPDISPOSE=0x80000,
		  XEN_ALL=0xffffffff
	};
	//inline void AS3SetDispose(){exflags&=~XEN_BITMAPDISPOSE;}
	//inline void AS3ClearDispose(){exflags|=XEN_BITMAPDISPOSE;}
	//inline void AS3IsBitmapDispose(){ return (exflags&XEN_BITMAPDISPOSE)==0;}

	inline void AS3SetFilterRenter(){exflags&=~XEN_FILTEROBJ;}
	inline void AS3ClearFilterRender(){exflags|=XEN_FILTEROBJ;}
	inline bool AS3IsFilterRender(){return (exflags&XEN_FILTEROBJ)==0;}
	inline void AS3SetReplace(){exflags&=~XEN_ORGBITMAP;}
	inline bool AS3IsReplace(){return (exflags&XEN_ORGBITMAP)==0;}

	inline void AS2SetInit(){ exflags|=XEN_NOTINITAS2;}
	inline void AS2ClearInit(){ exflags&=~XEN_NOTINITAS2;}
	inline bool AS2NeedInit(){ return (exflags&XEN_NOTINITAS2)!=0;}

	inline bool AS3IsCalling(){ return (exflags&XEN_NOTCALLFRAME)==0;}
	inline void AS3SetCalling(){ exflags&=~XEN_NOTCALLFRAME;}
	inline void AS3ClearCalling(){ exflags|=XEN_NOTCALLFRAME;}
	inline bool AS3NeedCallFrame(){ return (exflags&XEN_CALLFRAMEOK)==0;}
	inline void AS3ClearCallFrame(){ exflags|=XEN_CALLFRAMEOK;}
	inline void AS3SetCallFrame(){ exflags&=~XEN_CALLFRAMEOK;}
	inline bool AS3IsNamed(){return (exflags&XEN_ISAS3NAMED)!=0;}
	inline void AS3SetNoName(){ exflags&=~XEN_ISAS3NAMED; }
	inline bool IsNotInitChild(){return (exflags&XEN_NOTINITCHILD)!=0;}
	inline void SetInitChild(){exflags&=~XEN_NOTINITCHILD;}
	inline bool IsNotInstance(){return (exflags&XEN_NOTINSTANCE)!=0;}
	inline void SetInstance(){ exflags&=~XEN_NOTINSTANCE;}
	inline void ClearInstance() {exflags|=XEN_NOTINSTANCE;}
	inline bool IsMouseWheel(){return (exflags&XEN_MOUSEWHEEL)!=0;}
	inline bool IsTrackAsMenu(){return (exflags&XEN_TRACKASMENU)!=0;}
	inline bool IsHandCursor(){return (exflags&XEN_HANDCURSOR)!=0;}
	inline bool IsTabChildren(){return (exflags&XEN_TABCHILDREN)!=0;}
	inline bool IsMouseChildren(){return (exflags&XEN_MOUSECHIDREN)!=0;}
	inline bool IsDoubleClicked(){return (exflags&XEN_DOUBLECLICKED)!=0;}
	inline bool IsFocusRect(){return (exflags&XEN_FOCUSRECT)!=0;}
	inline bool IsMouseEnabled(){return (exflags&XEN_MOUSEDENABLED)!=0;}
	inline bool IsTabEnabled(){return (exflags&XEN_TABENABLED)!=0;}
	inline bool IsButtonMode(){return (exflags&XEN_NOTBUTTONMODE)==0;}
	//void AS3SetLoaderInfo(ScriptPlayer*player);
	void SetGridRect(SRECT*rect);
	SRECT* GetGridRect();
	_XSObject();
	bool IsFocusAble()
	{
		//if((exflags&XEN_FOCUSENABLE)!=0)
		//	return true;
		if(!character) return false;
		switch(character->type)
		{
		case textLineChar:
		case editTextChar:
		case editTextEmptyChar:
		case buttonEmptyChar:
		case buttonChar:
			 return true;
		}
		return false;
	}
	XBOOL IsEditText()
	{
		if(!character) return false;
		switch(character->type)
		{
		case editTextChar:
		case editTextEmptyChar:
			 return true;
		}
		return false;
	}

	bool IsRootObject()
	{
		if(!character) return false;
		switch(character->type)
		{
		//case playerChar:
		//case loaderChar:
		case bitmapEmptyChar:
			 if(character->tag>65500)
				 return true;
			 break;
		case rootChar:
		case loaderSWFChar:
		case loaderImageChar:
			 return true;
		}
		return false;
	}
	ScriptPlayer* GetPlayer()
	{
		if(character) 
			return character->splayer;
		return NULL;
	}
	void* GetContext();
	int	GetDrawCmds();
	inline bool IsDisplayObject()
	{
		if(character==NULL||character->type!=bitsChar)
			return true;
		return false;
	}
	bool IsCreateBefore(XU32 nTime)
	{
		return nCreateTime<nTime;
	}
	int getChildCount()
	{
		_XSObject*obj=bottomChild;
		int iCount=0;
		while(obj)
		{
			iCount++;
			obj=obj->above;
		}
		return iCount;
	}

	bool AS3Instance(bool bAdd,void*pObj);
	void AS3Construct();

	inline void SetMouseWheel(bool b)
	{
		if(b)
			exflags|=XEN_MOUSEWHEEL;
		else
			exflags&=~XEN_MOUSEWHEEL;
	}
	inline void SetTrackAsMenu(bool b)
	{
		if(b)
			exflags|=XEN_TRACKASMENU;
		else
			exflags&=~XEN_TRACKASMENU;
	}
	inline void SetHandCursor(bool b)
	{
		if(b)
			exflags|=XEN_HANDCURSOR;
		else
			exflags&=~XEN_HANDCURSOR;
	}
	inline void SetButtonMode(bool b)
	{
		if(!b)
			exflags|=XEN_NOTBUTTONMODE;
		else
			exflags&=~XEN_NOTBUTTONMODE;
	}
	inline void SetTabChildren(bool b)
	{
		if(b)
			exflags|=XEN_TABCHILDREN;
		else
			exflags&=~XEN_TABCHILDREN;
	}
	inline void SetMouseChildren(bool b)
	{
		if(b&&character&&character->tag==21)
			int v=0;
		if(b)
			exflags|=XEN_MOUSECHIDREN;
		else
			exflags&=~XEN_MOUSECHIDREN;
	}
	inline void SetDoubleClicked(bool b)
	{
		if(b) exflags|=XEN_DOUBLECLICKED;
		else  exflags&=~XEN_DOUBLECLICKED;
	}
	inline void SetFocusRect(bool b)
	{
		if(b) exflags|=XEN_FOCUSRECT;
		else  exflags&=~XEN_FOCUSRECT;
	}
	inline void SetMouseEnabled(bool b)
	{
		if(b) exflags|=XEN_MOUSEDENABLED;
		else  exflags&=~XEN_MOUSEDENABLED;
	}
	inline void SetTabEnabled(bool b)
	{
		if(b) exflags|=XEN_TABENABLED;
		else  exflags&=~XEN_TABENABLED;
	}
	int clipDepth,tabIndex;
	U8 blendMode;
	U8 cacheMode;

	//for Bitmap Cache
	void FreeCacheBmp();
	bool CacheIfNeed(MATRIX&mat);
	FrameTexture* pCacheBmp;
	//XDrawDib* pCacheBmp;
	MATRIX*   pCacheMat;
	//End Bitmap Cache


	int tagType;
	XU32 nForeColor,nBackColor,nCreateTime;
	void SetBackColor(XU32 nColor);
	//U32 nEventFlags;
	//_XClipAction*pActions;

	// Drag and drop
	XBOOL dragCenter;
	SRECT dragConstraint;
	//char *dropTarget;
	//XXVar dropTarget;
	_XSObject* dropTarget;

	//_XREdge* edges;
	//_XRColor* colors;
	XCONTROL control;
	GLShapeListData* mpShapeList;
	BwShapeRecord mShapeRecord;
#ifdef EDITTEXT
	EditText* editText;
	EditText* GetEdit()
	{
		if(!character) return XNULL;
		if(character->type!=editTextChar)
			return XNULL;
		return editText;
	}
#endif

	XBOOL IsEmptyThread()
	{
		return character&&character->type==spriteEmptyChar;//rootChar
	}
	void LinkBy(_XSObject*p)
	{
		if(p==XNULL)
		{
			if(pLinkBy)
				pLinkBy->pLink=XNULL;
			pLinkBy=XNULL;
			
		}
		else
		{
			p->pLink=this;
			pLinkBy=p;
		}
	}
	void SetPosX(XFDOUBLE value)
	{
		//if(character->tag==65534)//140&&value>1000)
		//	int v=0;
		SCOORD tx = (SCOORD) (value * 20); // turn into twips
		if ( xform.mat.tx != tx )
		{
			xform.mat.tx = tx;
			if(!pCacheBmp)
				Modify();
			else
				Modify1();
		}
		puppet = XTRUE;
	}
	//void GetSpriteBound(P_SRECT rect);
	void SetPosY(XFDOUBLE value)
	{
//		return;
		//if(character&&character->tag==951&&value>1000)
		//	int v=0;
		//if(character&&character->type==rootChar)
		//	int v=0;
		SCOORD ty = (SCOORD) (value * 20); // turn into twips
		//if(ty<-10000)
		//	int v=0;
		if ( xform.mat.ty != ty ) {
			xform.mat.ty = ty;
			if(!pCacheBmp)
				Modify();
			else
				Modify1();
		}
		puppet = XTRUE;
	}
	XFDOUBLE GetPosX()
	{
		//if(character&&character->tag==stag)
		//	int v=0;
		return xform.mat.tx/20.0;
	}
	XFDOUBLE GetPosY()
	{
		return xform.mat.ty/20.0;
	}
	void SetCloneFlag()
	{
		flags|=OB_CLONEOBJ;
	}
	void SetAS3Flag()
	{
		flags|=OB_AS3FLAG;
	}
	bool IsAS3Flag(){return (flags&OB_AS3FLAG)!=0;}
	bool CallFrame(int iFrame);
	XBOOL IsCloneObject(){return flags&OB_CLONEOBJ;}
	void CalcDevBounds(MATRIX*,SRECT*);
	void CalcButtonBounds(P_SRECT bounds);
	static void BuildRasterObj(_XSCharacter*character,MATRIX&mat,bool bSave);
	void BuildEdges(STransform*,_XSObject*pClip=XNULL,bool bClip=false);//,_GRIDINFO*info,_XSObject*pClip=XNULL,XU32 nFilterColor=0);//bool bFilterRender=false);
	void BuildChar(ScriptPlayer*player,XBOOL bTess);
	static GLTranglesOrLines* BuildFontChar(_XSCharacter*fontChar,int iCode,char*fontName,int nFontID,float fontSize);
	static void GLReleaseChar(GLChar*pChar);
	static void GLUseChar(GLChar*pChar);
	void FreeCache();
	void FreeCacheAll();
	void Release();
	void FreeContext(bool bFinal=false);
	void FreeShapeData();
	//_XSObject* DrawClipBracket(CRaster* raster, STransform x, RColor* clipColor,_XSObject*pClip=XNULL);
	void BuildClipEdges(STransform x,_XSObject*clipColor);
	//_XSObject* HitClipper(_XSObject*&hitObj,STransform x, SPOINT* pt);
	//XBOOL HitButtonClipper(_XSObject*&next,_XSObject*&hitObj,_XSObject*&hitClip,STransform x,SPOINT*pt);

	void FreeChildren(bool bFinal=false);		// free all the children of the object
	void Modify();				// mark the object as need draw
	void Modify1();


	bool CalcUpdate(MATRIX* m, SRECT*clipRect,BOOL forceDirty=false);	// calculate the update region based on the changes
	void Draw(RasterGL* raster, STransform x,bool bDraw,SRECT*rect,_XFilter*pFilter=NULL);//bool bFilterRender=false);
	void DrawCached(RasterGL* raster, STransform x,SRECT*rect);//bool bFilterRender=false);
#ifdef EDITTEXT
	void EditTextMouse(SPOINT* pt, int mouseIsDown);
	void DblClick(SPOINT*pt);
//	#ifdef _MAC
	BOOL DrawEditText(STransform* x);
//	#elif defined(_WINDOWS)
// 	BOOL DrawEditText(STransform* x, HDC dc);
//	#endif
	//void UpdateEditText(ScriptThread *thread, XPCTSTR name, XPCTSTR value, BOOL updateFocus);
#endif
	bool HitTestOneChild(STransform x,SPOINT*pt,bool bClip=false);
	BOOL HitTestOne(STransform* x, SPOINT* pt,XBOOL bTest=XFALSE,XBOOL bClip=false); // the point should be in device coordinates

	//void HitTest(_XSObject*& hitObj, STransform x, SPOINT* pt,XBOOL bTest);
	//XBOOL HitButton(_XSObject*& hitObj,_XSObject*&hitClip, STransform x, SPOINT* pt,SRECT*rect=XNULL);

public:
	void FreeFilter();
public:
	XBOOL IsVisibleAll();
	void UpdateControl();
	//void SwapDepth(SObject*pObj1,SObject*pObj2);
	XBOOL PostEvent(XU32 nTime,XSWFPlayer*player,XU32 id,int nCode);
	int AS3FillMembers(void*,void*);
	U16* AS3GetStaticText(int&l);
	/*void* AS3GetEventObject()
	{
		SObject*pObj=this;
		for(;;)
		{
			if(pObj->pASObject) return pObj->pASObject;
			pObj=pObj->parent;
			if(!pObj) return NULL;

		}
		return NULL;
	}*/
	//void AS3SetStateObjects(void*pInstance,int nState);
	//XBOOL AS3PostEvent(void*pEvent);
	//XBOOL AS3DoEvent(void*pEvent);
	XBOOL HasClipEvent(int type);
	void UpdateBounds(int x,int y,int w,int h,int nThick);
	bool IsOverBounds(int x,int y,int w,int h,int nThick);
	void UpdateBounds(int x,int y,int nThick);
	XBOOL IsThread();
	XBOOL OnEvent(XU32 nTime,XSWFPlayer*player,XU32 id,XXVar*pArgs=XNULL,int nCode=0);//XXVARLIST*pArgs);
	XBOOL SetText(XPCTSTR strBuf);
	//XBOOL AS3OnEvent(XSWFPlayer*,int id,int type,int argc,int* args);
	XBOOL AS3SetText(XPCWSTR strBuf,int l=0);
	XBOOL GetText(XXVar&var);//XString8&strTxt);
	void SetBoundBox(SRECT&rect,XBOOL bCamera);
	void SetMatrix(MATRIX&mat);
	void GetMatrix(MATRIX&mat);
	void SetCXForm(XCXForm&form);
	void GetCXForm(XCXForm&form);
	_XSObject* GetChildByIndex(XU32 index);
	void SwapDepth(_XSObject*pObject,XPCTSTR name);
	void SwapDepth(_XSObject*pObject,int nDepth);
	int GetMaxDepth();
	_XSObject* GetChildOf(int nDepth);
	void GetBoundBox(SRECT&rect,MATRIX*pMatrix,bool transSelf=true);
	void GetBoundRect(SRECT&rect,MATRIX*pMatrix,bool transSelf=true);
	void GetBoundBox(SRECT&rect,XBOOL bWithCamera);
//	XBOOL HitTest(int x,int y,XBOOL bAnti);
//	void GetBoundBox(SRECT&rect,XBOOL bWithCamera);
	XBOOL HitObject(_XSObject*pObject);
	void LocalToGlobal(XFDOUBLE&x,XFDOUBLE&y);
	void GlobalToLocal(XFDOUBLE&x,XFDOUBLE&y);
	ScriptThread* GetThread();
	_XSObject* GetChildOf(XPCTSTR strName,XBOOL bThread);
	void GetTotalMatrix(MATRIX&mat,XBOOL bWithCamera);
	void GetTotalCXForm(XCXForm&form);
	void Scale(XFDOUBLE sx,XFDOUBLE sy);
	void SetHeight(XFDOUBLE v);
	void SetWidth(XFDOUBLE v);
	XFDOUBLE GetHeight();
	XFDOUBLE GetWidth();
	XBOOL IsNamedThread();
	XFDOUBLE GetRotate();
	XFDOUBLE GetScaleY();
	XFDOUBLE GetScaleX();
	void SetRotate(XFDOUBLE v);
	bool IsVisible(){return (flags&OB_VISIABLE)!=0;}
	bool IsEnable(){return (flags&OB_ENABLE)!=0;}
	void SetEnable(XBOOL e)
	{
		if(e)
			flags|=OB_ENABLE;
		else
			flags&=~OB_ENABLE;
	}
	void SetVisible(XBOOL vis)
	{
		if(vis)
			flags|=OB_VISIABLE;
		else
		{
			flags&=~OB_VISIABLE;
			SetUpRemove();
		}
		Modify();
	}

	void SetUpRemove();
	void SetScaleY(XFDOUBLE value);
	void SetScaleX(XFDOUBLE v);
	void SetAlpha(XFDOUBLE value);
	void AS3SetBlendMode(U8 iMode)
	{
		if(blendMode==iMode) 
			return;
		blendMode=(U8)iMode;
	}
	void AS3SetAlpha(XFDOUBLE value)
	{
		S16 aa = (S16) (value * 256);
		if ( xform.cxform.aa == aa ) 
			return;
		{
			 xform.cxform.aa = aa;
			 xform.cxform.CalcFlags();
			 Modify();
		}
		puppet = XTRUE;
	}
	XFDOUBLE GetAlpha()
	{
		return (xform.cxform.aa * 100) / 256;
	}
	XFDOUBLE AS3GetAlpha()
	{
		return (XFDOUBLE)(xform.cxform.aa)/256;
	}

	void FreeObject(bool bFinal=false);
	void* AS3CreateInst(void*pSuper,XBOOL bForce);
	void* AS3ConstructInst(void*pSuper);
	bool AS3CreateAndConstruct(void*pSuper,XBOOL bForce)
	{
		pSuper=AS3CreateInst(pSuper,bForce);
		if(pSuper)
			AS3ConstructInst(pSuper);
		return pSuper!=XNULL;
	}
	XXObject* CreateObject(XXObject*pSuper,XSWFCONTEXT*pCnt);

public:
	inline _XSObject* GetCommonParent(_XSObject*obj)
	{
		if(!obj) return XNULL;
		_XSObject*p1=parent;
		while(p1)
		{
			_XSObject*p2=obj->parent;
			while(p2)
			{
				if(p1==p2) return p1;
				p2=p2->parent;
			}
			p1=p1->parent;
		}
		return XNULL;
	}
	inline bool IsParentOf(_XSObject*obj)
	{
		while(obj)
		{
			obj=obj->parent;
			if(obj==this) return true;
		}
		return false;
	}
	inline bool IsChildOf(_XSObject*obj)
	{
		if(!obj) return false;
		_XSObject*p=parent;
		while(p)
		{
			if(p==obj) return true;
			p=p->parent;
		}
		return false;
	}
					//bounds=pObject->devBounds;
	void GetDevBound(P_SRECT rect)
	{
		RectUnion(&devBounds,rect,rect);
		for (_XSObject* child = bottomChild; child; child = child->above)
		{	
			child->GetDevBound(rect);
		}
		return;
	}
	//void DrawDraw( Raster *raster,STransform x,GLColor*clipColor,_GRIDINFO*gridBy,XBOOL bTrans,XU32 nFilterColor,bool setDrawn=false);
	void CalcDrawUpdate(MATRIX*m,SRECT*clipRect,XBOOL bMat);
	void UpdateClass(XXVar&className,XXObject*pSuper,XSWFCONTEXT&cnt);
	void Constructor(XXObject*pSuper,XSWFCONTEXT*pCnt);
	void SetMask(_XSObject*pObj);
	_XSObject* AS3GetMask();
	SRECT* AS3GetScrollRect();
	void AS3SetScrollRect(SRECT*rect);
	XBOOL HasMouseEvent();
	bool FindObject(_XSObject*pObj)
	{
		if(pObj==this)
			return true;
		_XSObject*o=this->bottomChild;
		while(o)
		{
			if(o->FindObject(pObj)) 
				return true;
			o=o->above;
		}
		return false;
	}
	void AS3RemoveChild();
	void AS3AddChild();
	bool AS3HasMouseEvent();
	bool hasPressEvent();
protected:
	inline _XSObject* AS3HitObjectClipper(_XSObject*&next,STransform x,SPOINT*pt);
	bool AS3HitTest(STransform x,SPOINT*pt,bool bTest);
	bool AS3HitTestPixel(STransform x,SPOINT*pt);
	//SBitmapCore* GetBits();
	inline bool AS3HitTestClipper(_XSObject*&next,STransform x,SPOINT*pt,bool bTest);
	
	bool AS3HasMouseEventAt(_XSObject*pObj,bool bAS3);
	bool AS3HasKeyEvent();
public:
	AS3ButtonInfomation* CreateBI(XBOOL&bParent,SRECT&rect,SPOINT*pt);
	XBOOL HitBI(STransform x,SPOINT*pt,int nCap,SRECT&rect,XBOOL bEnable);
	XBOOL HasClickEvent();
public:
	_XSObject* GetItemParent(_XSObject*obj,_XSObject*parent);
	bool IsAS3();
	_XSObject* AS3HitObject(STransform x,SPOINT*pt,_XSObject**hitObj,XU32 t,bool bButton=false);
	bool	   AS3HitTest(int x,int y,XBOOL bAnti);
	_XSObject* AS3GetTargetObject(bool bMouse);
	_XSObject* AS3GetEventObject(bool bMouse)
	{
		_XSObject*pObj=this;
		while(pObj)
		{
			if( !pObj->parent || !pObj->parent->IsMouseEnabled())
			   return pObj;
			if(bMouse)
			{
				if(pObj->AS3HasMouseEvent())
					return pObj;
			}
			else if(pObj->AS3HasKeyEvent())
				return pObj;

			pObj=pObj->parent;
		}
		return XNULL;
	}
	//bool AS3HasKeyEvent();

	//////////////////////////////////////////////////////////////////

	AS3DECLAREEVENT(enterFrame);
	AS3DECLAREEVENT(added);
	AS3DECLAREEVENT(removed);
	AS3DECLAREEVENT(render);
	AS3DECLAREEVENT(addedToStage);
	AS3DECLAREEVENT(removedFromStage);
	AS3DECLAREEVENT(change);
	AS3DECLAREEVENT(scroll);
	AS3DECLAREEVENT(open);
	bool AS3_OnLink(U16*,U8*);
	bool AS3_OnTextinput(U16*,U8*);

	/////////////////////////////////////////////////////
	static GLTranglesOrLines* CreateFrame(SRECT&rect,float*color,bool bFill);
	static GLShapeData* CreateShapeData(MATRIX*pMatrix,float*color,GLTranglesOrLines*pFL=NULL);
	static GLShapeListData* CreateShapeList(STransform* x,_XSObject*pObj,bool bMatrix=true);
	static void AddFrame(SRECT&rect,MATRIX&mat,float*color,GLShapeListData*pList,BwShapeRecord*pRec,bool bFill,GLShapeData**pLast);
	static void GLSetShapeListData(GLShapeListData*pList,GLShapeData*pData,GLShapeData**pLast);
	static void GLSetShapeData(GLShapeData* pData,GLTranglesOrLines*pFL,GLTranglesOrLines**pLast);
}SObject;

typedef XVector<SObject*> XSObjectArray;

void GetBoundingBox(SObject* obj, P_SRECT bounds);
void GetBoundingBox(SObject* obj,MATRIX*, P_SRECT bounds,bool transSelf=true,bool setall=true);
void GetBoundingRect(SObject*obj, P_SRECT bounds);
#endif
