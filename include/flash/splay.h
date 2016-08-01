/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc.
****************************************************************************/

#ifndef SPLAY_INCLUDED
#define SPLAY_INCLUDED

#ifndef SDISPLAY_INCLUDED
#include "sdisplay.h"

#endif

#define USE_VIDEO
//
// Helpers for Parsing a Script
//
struct _XStreamData;

// The low level parser
class SParser {
public:
	U8 * script;
	S32 pos;

	// Bit Handling
	U32 bitBuf;
	int bitPos;

public:
	// Tag Parser
	S32 tagPos;		// the start pos of the tag
	S32 tagEnd;
	int  tagCode;

	int GetTag(S32 len=0x1FFFFFFF);	// return -1 if the tag is not yet loaded given len data
	XU8* ReadAddr(){return script+pos;}
public:
	void SkipU30(int nCount);
	XU32 GetU30();
	SParser(XU8* s,int l){script=s;pos=0;tagEnd=l;}
	SParser() { script = 0; pos = 0; };
	void Attach(U8 * s, S32 start, S32 e = (1L<<29)) { script = s; pos = start; tagEnd = e; };

	// Get data from the script
	U8 GetByte();
	U8 GetBitsByte();
	U16 GetWord();
	U32 GetDWord();
	GLfloat GetFixedtoFloat();
	GLfloat GetFixedtoFloat8();
	GLfloat GetFloat32();
	inline double GetDouble()
	{
		double data;
		// zhengjl 2011.08.29
		// this modification is for android release
		#if defined(__ANDROID__)
			char * t1 = (char*)(script+pos);
			char * t2 = (char*)&data;

			for (int i = 0; i < 4; i++)
			{
				*(t2 + i) = *(t1 + i + 4);
			}
			for (int i = 0; i < 4; i++)
			{
				*(t2 + i + 4) = *(t1 + i);
			}

		#else
			XU32*p=(XU32*)&data;
			XU32*ps=(XU32*)(script+pos);
			p[0]=ps[1];
			p[1]=ps[0];
			//data=*((double *)&m_pData[readPos]);
		#endif

		pos+=8;

		return data;
		//return XTRUE;
	}
	inline float GetFloat()
	{
		float* s =(float*)(script+pos);
		pos+=4;
		return *s;
	//if ( pos > end ) return 0;
		//return (U32)s[0] | ((U32)s[1]<<8) | ((U32)s[2]<<16) | ((U32)s[3]<<24);
	}

	void SkipBytes(int n) { pos += n; }
	inline S32  GetSize(){return tagEnd-pos;}
	S32 GetData(void * data, S32 len);	// returns the actual number of bytes read could
										//	be less than len if we hit the end of a tag

	void GetColor(RGBI*, BOOL alpha=false);
	SRGB GetColor(BOOL alpha=false);
	void GetRect(SRECT*);
	void GetBwMatrix( BwMatrix *mat );
	void GetMatrix(MATRIX*);
	void GetColorTransform(ColorTransform*, BOOL alpha=false);
	char* GetString();	// caller must free memory with a delete operator
	char* GetStringP();	// just return a pointer to the string data
#ifdef SOUND
	void GetSoundInfo(XSoundChannel*);
#endif

	// Routines for reading arbitrary sized bit fields from the stream
	//	always call start bits before gettings bits and do not intermix
	//	this calls with GetByte, etc...
	void InitBits();
	U32 GetBits(int n);		// get n bits from the stream
	S32 GetSBits(int n);	// extend the sign bit
};


// The Shape Edge parser
/*typedef struct _XSLine {
	RColor* color;
	int thickness;
	XU16	nFlags;
	XU16	nMiterFactor;
}SLine;*/

class ScriptPlayer;
class XSWFPlayer;

enum
{
	XLS_CAPSTYLEROUND=0x0000,
	XLS_CAPSTYLENONE=0x4000,
	XLS_CAPSTYLESQURE=0x8000,
	XLS_JOINSTYLEROUND=0x0000,
	XLS_JOINSTYLEBEVEL=0x1000,
	XLS_JOINSTYLEMITER=0x2000,
	XLS_FILLFLAG=0x0800,
	XLS_NOHSCALE=0x0400,
	XLS_NOVSCALE=0x0200,
	XLS_PIXELHINT=0x0100,
	XLS_NOCLOSE=0x04,
	XLS_ENDCAPSTYLE=0x03,

};

class SShapeParser : public SParser {
public:
	//_GRIDINFO* info;
	//DisplayList* display;
	bool m_bODD;
	ScriptPlayer* player;
	//RColor* localColors;	// if the colors are not kept, colorList points to this list
	//RColor** colorList;	   	// the colors are added to this list
	//MATRIX mat;				// the edges are transformed by this matrix - defaults to identity
	//ColorTransform cxform;	// the color transform
	S32 ratio;				// the morph ratio
	
	//RasterGL* rasterGL;
	//U32 layer;			// the base layer for the edges

	// Line and Fill Index
	int nLines, nFills;
	//int nFillBase,nLineBase;
	//RColor** fillIndex;
	//SLine* lineIndex;

	//RColor* fillIndexMem[256];
	//SLine lineIndexMem[256];

	int nFillBits, nLineBits;

	BOOL getAlpha,lineStyle2;

	// Current State
	int line, fill[2];
	SPOINT curPt, curPtX;

public:
	BwShapeRecord* pShapeRec;

public:
	SShapeParser(ScriptPlayer* p, U8 * data, S32 start,BwShapeRecord*pRec);
	~SShapeParser();
	BOOL GetBwStyles(bool bTess=true);
	XBOOL SkipColor();
	XBOOL ReadGLColor(GLColor*pColor);
	int GetBwEdge( BwEdge &edge );
	//BOOL ReadFillStyel(vector<BwFillStyle*> &fillStyles );
	
	int GetBwPaths(BOOL useWinding=XTRUE);


};

// lee@middlesoft moved this to the header - there was 2 copies, which created hell
// when they became slightly out of sync
//
// The Character Parser
//
// Takes the packed edge data and adds processed edges to the object.
typedef struct _XSLine {
	RColor* color;
	int thickness;
	XU16	nFlags;
	XU16	nMiterFactor;
}SLine;
class SCharacterParser : public SShapeParser {
public:
	RColor* fillIndexMem[256];
	SLine lineIndexMem[256];
	U32 layer;			// the base layer for the edges
	MATRIX mat;
	DisplayList* display;
	RColor** fillIndex;
	SLine* lineIndex;
	REdge* pEdges;
	RColor* pColors;
	BOOL useWinding;
	BOOL preclip;
	int fillRule;
	RColor* color1;
	RColor* color2;
	int depth;
	CRaster* raster;		// optional raster if you want the color caches built
	bool IsComplex();
	void GetFontBound(SRECT&r);
	BOOL GetStyles();
	BOOL GetNormalStyles();
	XBOOL ReadColor(_XRColor*pColor);
	
	int GetEdge(_XCURVE*);//,XINT div=0);	// return the edge flags
	int GetNormalEdge(_XCURVE*);//,XINT div=0);	// return the edge flags
	void SetupColor(RColor* color);
	void ReleaseData();
	//_XSObject* obj;

public:
	SCharacterParser(ScriptPlayer* p, U8* data, S32 start, BwShapeRecord* pRec) :
			SShapeParser(p, data, start, pRec) { fillIndex=fillIndexMem;lineIndex=lineIndexMem;preclip=false;useWinding = false; }

	void AddCurve(P_CURVE);
	void BuildRasterEdges(BOOL getStyles, SRECT* devBounds,CRaster*raster,MATRIX&m);
	//void MakeClipper();
	void BuildEdges(BOOL getStyles,bool bTess=true);
	void BuildMorphEdges();
	XBOOL ReadGLMorphColor(GLColor*pColor);
	void ReadMorphFillStyle( BwFillStyle* fillStyle, int ratio );
	BOOL GetBwMorphStyles( int ratio );
	int GetBwMorphPaths( SShapeParser *parser, int ratio );
};


/*class ScriptVariable {
public:
	ScriptVariable* next;
	char* name;
	char* value;
};*/


//
// The ScriptThread handles the flow of control and parsing for a timeline
//

struct _XClipAction;
class XXObject;
class XXObjectClip;
class XSWFCONTEXT;
class XXVar;
class XXObjectBitmap;
//class XXObject;
class XDrawObj;
class XActionScript;
class XXObjectSound;

class ScriptThread : public SParser {
public:
	U32 symbolPos;
	//int m_nClasses;
public:
	friend class XActionScript;
	friend class XXObjectClip;
	//int abc[100];
	enum {
			noErr = 0,
			badHeaderErr = -1, 	// The header tag was missing
			noMemErr = -2,		// There was not enough memory available
			noScriptErr = -3,	// There was not enough memory available
			notSupport = -4,
			playOK = 0, 		// The frame was drawn properly
			playNeedData = 1,	// The player needs more data to finish the frame
			playAtEnd = 2,		// The player is at the end of the script
			as3ScriptError = 3,
		 };

public:
	//bool m_bLoadedOK;
	//virtual bool IsLoaded(){return true;}
	int numFramesComplete;	// info for tracking the numbers of frames loaded
	void InstanceFrame(bool bAdd,bool seek,void*pObj,bool bCall);
	bool m_bAS3Frame0,m_bInDisplay,m_bNeedCallFrame;
	int m_nAS3SencePos,m_nAS3Frames;
	void AS3BuildSecnesAndFrames(void*);
	void* AS3GetDomainEnv();

	int AS3GetLabels(void*pArray);
	//XActionScript* m_pCalling;
	XXObjectClip* GetClipObject()
	{
		if(!rootObject) return XNULL;
		return (XXObjectClip*)rootObject->pObject;
		//return m_pClip;
	}
	U32 nEventFlags;
	_XClipAction*pActions;
	ScriptPlayer* player;		// The player that has the character dictionary for this thread
	ScriptThread* next;			// for the display list to use
 	XSWFPlayer* splayer;			// Needed to support multiple sound objects in ScriptThread - else
							// this could be deprectated to ScriptPlayer. lee@middlesoft

	int m_nCurX,m_nCurY;
	XU8 m_bFill;

	// The script stream/data
	S32 startPos;	// the first tag in the script data
	S32 len;		// the length of the data we have so far...
	_XCOMMAND*	m_cmds;
	BOOL atEnd;
	int curFrame;
	//BOOL needCallFrame;
	BOOL seeking;
    BOOL justSeeked;            // used for mp3 streaming
    BOOL justSeekedNegative;    // used for mp3 streaming

	BOOL playing;		// set if the movie should be playing
	BOOL loop,bNewLoader;			// set if the movie should loop at the end

	int numFrames;	// the number of frames
	int scriptErr;
	int layerDepth;	// the depth to add to all place/remove objects for multiple layer support

	char* currentLabel;			// a pointer to the most recent frame label encountered to track state information
	int	  labelFrame;
	DisplayList* display;		// The display list we are attached to
	_XSObject* rootObject;		// The root object we have created on the display list for this thread

#ifdef SOUND
	// Sound Stream State
	// Note the sound thread runs a bit ahead of the main draw thread to solve latency problems
	BOOL needBuffer;			// true if we should prebuffer some frames before starting the sound
	BOOL sndAtEnd;				// true if we hit the end of the movie for the stream
	int mixFormat;				// the suggested format for the global sound mixer
	XSound snd;					// the format of all streaming sound in the file
	XSoundChannel* sndChannel;	// the sound channel for the streaming sound, nil if not streaming...
	int sndFrame;				// the current frame number for the sound thread
	S32 sndPos;					// the position of the sound thread
#endif

public:
	int m_nShapes;
	//void SyncObject();
	//void SyncObject2();
	//void SyncObject3();
	void PlaceObject();
	void PlaceObject2();
    _XSObject* CloneSprite(XSWFCONTEXT&cnt,ScriptThread *thread,const XXVar&name, int depth,XXObject*pInit=XNULL);
    void RemoveSprite(ScriptThread *thread, BOOL createPlaceholder = true);
	void RemoveObject();
	void DefineShape();
	void DefineBinary();
	void DefineMorphShape();
	void DefineButton();
	void DefineButtonExtra(int);
	void DefineSprite();
	void DefineBits();
	void DefineFont();
	void DefineFont2();
	static void PlaceCharacter(_XSObject*obj,bool bFirst);
	static void RemoveCharacter(_XSObject*obj);
	static void PlaceCharacter(_XSCharacter*ch);
	static void RemoveCharacter(_XSCharacter*ch);
//#if (__CORE_VERSION__>=0x02073000)
	void DefineFont4();
//#endif
//#if (__CORE_VERSION__>=0x02075000)
	void DefineFontName();
//#endif
	void DefineFontInfo();
	void DefineText();
	bool DoSymbolClass(int len,bool bEnd,bool bDummy);

#ifdef EDITTEXT
	void DefineEditText();
#endif

#ifdef USE_VIDEO
	void DefineVideo();
	void DefineVideoFrame();
#endif

	void DefineJPEGTables(XU8*pData,int nLen);

#ifdef SOUND
	void DefineSound();
	void StartSound(int nCode);

	void SoundStreamHead();
	void SoundStreamBlock();
    void AdjustMp3Streaming();

	void StopStream();
	BOOL DoSoundFrame();
	void PlayButtonSound(_XSObject* btn, int newState);
#endif

	#ifdef _CELANIMATOR
	BOOL protect;			// set if this file should not be imported
	BOOL pathsArePostScript;	// set if paths should be treated as PostScript
	void DefineBitsPtr();
	#endif

	#ifdef FSSERVER
	U8 * AttachData(S32 start, S32 end);
	void ReleaseData(U8 *&);
	#else
	U8* AttachData(S32 start, S32 end,XS32*l=XNULL) {if(l) *l=end-start;return script+start;}
	//{p->data=script+start;p->length=end-start;}
	void ReleaseData(U8 *&) {}
	#endif

	int DoTag();
	int DoTags(int);

	void BuildBits(SCharacter*);

public:
	// Support for Variables
	//ScriptVariable *firstVariable, *lastVariable;

	//ScriptVariable* FindVariable(char* name);
	//void SetVariable(char* name, char* value, BOOL updateFocus = true);

public:
	XBOOL DoEvent(XU32 nEvent,XINT nKey);
	ScriptThread();
	void InitThread();
	~ScriptThread();

	void ClearState();
	void ClearVariables();

	void SetDisplay(DisplayList* d,bool bAdd)
	{
		FLASHASSERT(!display);
		display = d;
		if(bAdd)//display->root.IsParentOf(rootObject))
			display->AddThread(this);
		//InitRoot();
	}

	// Play the script
	int GetFrame() { return curFrame; }
	BOOL AtEnd() { return atEnd; }

	int DrawFrame(int fnum, BOOL seek,BOOL bCall);	// advance or seek to the specified frame number
	void DoFrame(int frameDelta = 1, BOOL bForceRebuild = false); // handle a clock tick for the thread
public:
	
	void Play(BOOL rewind=true);
	void StopPlay();
	enum {SK_KEEP,SK_PLAY,SK_STOP};
	void Seek(int f,bool bCall,int nType);
public:
	DisplayList::XCTRLCMD* m_pCmds;
	void AddCommand(DisplayList::XCTRLCMD*cmd);
	inline void CMDStop()
	{
		DisplayList::XCTRLCMD*cmd=display->allocCmd(DisplayList::XCMD_STOP,0);
		AddCommand(cmd);
	}
	inline void CMDPlay()
	{
		DisplayList::XCTRLCMD*cmd=display->allocCmd(DisplayList::XCMD_PLAY,0);
		AddCommand(cmd);
	}
	inline void CMDGotoAndStop(int nFrame)
	{
		DisplayList::XCTRLCMD*cmd=display->allocCmd(DisplayList::XCMD_GOTOANDSTOP,nFrame);
		AddCommand(cmd);
	}
	inline void CMDGotoAndPlay(int nFrame)
	{
		DisplayList::XCTRLCMD*cmd=display->allocCmd(DisplayList::XCMD_GOTOANDPLAY,nFrame);
		AddCommand(cmd);
	}
	inline void CMDGotoAndKeep(int nFrame)
	{
		DisplayList::XCTRLCMD*cmd=display->allocCmd(DisplayList::XCMD_GOTOANDKEEP,nFrame);
		AddCommand(cmd);
	}
	bool doCommand(bool bCalling);
	int FindLabel(char*);
//#if (__CORE_VERSION__>=0x02075000)
	bool setCurrentLabel(int fnum);
	void doLabelCommand();
//#endif
// #ifdef SPRITE_XTRA
// 	BOOL GetFrameLabel(int frameNum, char* label);
// #endif
protected:
	bool DoABC(XU32 nLength,bool bDummy);
	void FreeCommands(XCOMMAND*cmds);
	void AddCommand(XCOMMAND*pCmd);
	XBOOL InitDrawObj();
	//XDrawObj* m_pDrawObj;
	//SObject* m_pDrawObj;
	//void InitDrawObject();

	XU32 GetEventFlags();
	void PlaceAction(_XPlaceInfo*info);
	void ExportAssets();
	void PlaceObject3();
public:
	void ClearActions();
	SObject* AttachClip(XSWFCONTEXT&cnt,SCharacter*p,const XXVar&name,int depth,XXObject*pInit);
	void MoveTo(int x,int y);
	void LineTo(int x,int y,XBOOL bMove=XFALSE);
	void LineStyle(int nWidth,XU32 nColor);
	void EndFill();
	void CurveTo(int ax,int ay,int cx,int cy);
	_XSObject* CreateEmptyClip(XSWFCONTEXT&cnt,XPCTSTR strName,int nDepth);
	void ReleaseDraw();
	void BeginFill(XU32 nColor,int nWidth);
	void BeginFill(XXObjectBitmap*pBitmap,MATRIX*pMatrix,XBOOL bRepeat,XBOOL bCreate);
	XBOOL AttachBitmap(XXObjectBitmap*pBitmap,int nDepth,XBOOL  IsCreated);

	void BeginFill(XU8 nType,MATRIX*pMatrix,XU32Array&colors,XU8Array&ratios,XU8 nSpread,XU8 nInterMode);
	void  AddVariable(XPCTSTR strKey,XPCTSTR strValue,XBOOL updateFocus);
	//for Action Script
	XXObjectClip* m_pClip;
public:
	void DefineMorphShape2();
	XBOOL HasEvent(XU32 f);
	_XClipAction* CloneActions();
	ScriptPlayer* CreateLoader(bool bNew);
	XBOOL StartSound(int nOffset,int nLoops);
	void BuildEdges(ScriptPlayer*player,BwShapeRecord*pRec);
	void RemoveCalling(XActionScript*pAction);
	void AddCalling(XActionScript*pAction);
	void RemoveDepth(int depth);

	//XBOOL SetVariable(XSWFCONTEXT&cnt,XPCTSTR name,XXVar&var,XBOOL update);
	//XBOOL GetVariable(XSWFCONTEXT&cnt,XPCTSTR name,XXVar&var);
	//XBOOL FindVariable(XPCTSTR strName,XXVar&var);
	void SetForeColor(XU32 nColor);
//	XU32 nBackColor,nForeColor;
};


//
// The ScriptPlayer handles a stream header, the character dictionary and streaming
//

// Special reserved character tag ids
#define	ctagBitmapBug           65535,	// reserved as a workaround for an old export bug
#define ctagThreadRoot          65534
#define ctagExternalThreadRoot  65533
#define ctagSpritePlaceholder   65532
#define ctagSpriteEmpty			65531
#define ctagAttachBitmap		65530
#define ctagBitmapRoot			65529
#define ctagBitmapLoader		65528
#define ctagSWFLoader			65527
struct JPEGPlayState;

typedef void (*LostCharacterProc)(void* data, U16 tag);

class XExports:public XSort
{
public:
	virtual ~XExports()
	{
		Release();
	}
	struct XKEY
	{
		XString8 strKey;
		XINT	 nID;
	};
	XArray<XKEY*> m_keys;
protected:
	XINT  Compare(void* iSrt,void* iDst,XU8 nArg)
	{
		return CompareString8((XPCTSTR)iSrt,(XPCTSTR)iDst,nArg);
	}
	XU32  GetCount()
	{
		return m_keys.GetSize();
	}
	void* GetData(XU32 iData)
	{
		return m_keys[iData]->strKey;
	}
public:
	XINT ValueOf(int iKey){return m_keys[iKey]->nID;}
	XKEY* Add(XPCTSTR strKey,XINT nID,XU8 nMode)
	{
		int im;
		int id=Index((void*)strKey,im,nMode);
		if(id>=0)
		{
			m_keys[id]->nID=nID;
			return m_keys[id];
		}
		else
		{
			XKEY*pKey=new XKEY;
			pKey->strKey=strKey;
			pKey->nID=nID;
			m_keys.InsertAt(im,pKey);
			return pKey;
		}
	}
	void Release() {m_keys.FreeAll();}

};

class XXObjectCreator;

namespace avmshell
{
	class XAVM2;
	class ShellCodeContext;
	class LoaderInfoObject;
	class LoaderObject;
}
//class XXObjectEvent;

class ScriptPlayer : public ScriptThread {
/*public:
	int m_nRefs;
	ScriptPlayer* m_pRefPlayer;
	void Ref();
	void UnRef();
	void SetRef(ScriptPlayer*player);*/
public:
	//XU8 nCacheCount;
	//bool CounterCache();
	
	void HoldInit();
	void* m_lockURL;
	XBOOL m_bHold;
	SObject* AS3AttachBitmap(SCharacter*ch,void*pCode);
	SObject* AS3AttachBitmap(XImage*pImage,void*pCode);
	bool IsImage()
	{
		return nCompressType==XT_IMAGE;
	}
	_XSObject* GetBitmapContent()
	{
		if(!IsImage()) return XNULL;
		_XSObject*obj=rootObject->bottomChild;
		while(obj)
		{
			if(obj->character&&obj->character->type==bitmapEmptyChar)
				return obj;
			obj=obj->above;
		}
		return NULL;
	}
	//bool AS3CreateSObject(_XSObject*&obj,int type,void*pInstance,SCharacter*ch);
	bool AS3Unload(bool bClear);
	const char* GetAppName(int&l);
public:
	void PrevData(void*gcEnter,bool bDummy);
	XBOOL DefineClass(XPCTSTR strClass)
	{
	}
	void SetWaitClass(XPCTSTR strClass,int nID)
	{
		m_strWaitClass=strClass;
		m_strWaitClass.ToString(XTRUE);
		this->m_nWaitID=nID;
	}
	//XString8 m_strAppName;
	int  m_nWaitID,m_nABCID;
	bool m_bInitSet;
	void InitRoot(bool bPlace);
//	void SetEvent(XXObjectEvent*p)l//l//{m_pEvent=p;}
//	void AS3CreateLoader();
	void AS3InitImageLoader();
	void AS3Complete(bool bOK);
	XXObjectCreator* m_pGlobal;
	avmshell::XAVM2* m_pAVM2;
	bool	m_bCanceled,m_bFlex;
	avmshell::LoaderInfoObject* m_pLoaderInfo;//ShellCodeContext*	m_pContext;
	avmshell::ShellCodeContext* m_pCodeContext;
	//avmshell::LoaderObject*		m_pLoader;
	//void*	m_pLoaderInfo;
	void*	pASSuper;
	//void*	m_pLoaderInfo;
	//void*	m_pLoader;
	bool m_bInitOK,m_bABCOK,m_bLoadedOK;
	XArray<XXVar> m_heads;
	XBOOL m_bRoot;
	// The URL this script thread was downloaded from
	//char *url;
	XXVar   m_strWaitClass;
	XXVar	m_url;
	XXVar   m_loadUrl;
#ifdef _WINEMU
	XXVar	m_savedUrl;
#endif
	XXVar	m_contentType;

	// The script
	S32 scriptLen;	// the length of the script from the header
    int *scriptRefCount;  // used for cloning external sprites

	S32 headerLen;	// temp data for holding a partial header
	U8 headerBuf[8];

	BOOL gotHeader,resize;


	U32 numFramesCompletePos;
	//U32 bytesTotal,bytesLoaded;
	// The header info
	SRECT frame;	// the picture frame
	SFIXED frameRate;// the number of frames to show per second
	S32 frameDelay;	// the number of milliseconds to between frames given the frame rate
	int version,fileAttribs;
	bool bAS3,bAttach,bScript;	// the version number from the header

	//JPEGPlayState* jpegInfo;
	XU8Array jpegTable;

	// For handling layers and streaming in SPlayer
// 	SPlayer* splayer;				// moved to ScriptThread in order to support multiple sound objects
 	_XStreamData* stream;			    // a slot for the stream object to store data

	ScriptPlayer* nextLayer,*pNextPlayer;//,*pNextInfo;

	BOOL mute;		// don't play any sounds

	// These members are used for Load Variable actions
	BOOL variableLoader;
	int  variableLayer;
	XXVar variableSprite;
#define VAR_INCR 2048
	XBOOL NeedLowCase(){return version<=7;}
	bool IsAS3(){return bAS3;}
	//bool  AS3PostEvent(int id,int type,int argc=0,int*argv=NULL);
public:
	// The Character list
	enum { charIndexSize = 128, charIndexMask = 127 };
	SCharacter* charIndex[charIndexSize];	// a hash table sorted by character tag values

	// Memory Allocators
	//XFastAlloc characterAlloc;

    // add one to the refcount and return the new value
    inline int ScriptAddRefCount()
    {
        if (!scriptRefCount)
        {
            scriptRefCount = new int;
            *scriptRefCount = 0;
        }

        *scriptRefCount += 1;
        return *scriptRefCount;
    }

    // sub one from the refcount and return the new value
    inline int ScriptSubRefCount()
    {
        if (scriptRefCount)
        {
            *scriptRefCount -= 1;
            int result = *scriptRefCount;
            if (result <= 0)
            {
                delete scriptRefCount;
                scriptRefCount = 0;
            }
            return result;
        }
        else
            return 0;
    }


	//inline SCharacter* CreateCharacter() { return (SCharacter*)(characterAlloc.Alloc()); }
	//void FreeCharacter(SCharacter* c);

	void FreeAll(bool bAll=true);
	void FreeCache();

public:
	// Internal Methods
	SCharacter* CreateCharacter(U16 tag);
	SCharacter* FindCharacter(U16 tag);
	void FreeCharacter(U16 tag);

public:
	void SetEvent(XXObject*p);

	int CompString(XPCTSTR srt,XPCTSTR dst)
	{
		return XString8::Compare(srt,dst,0,(XU8)NeedLowCase());//?XSS_NOCASE:XSS_NORMAL);
	}
	SCharacter* ImportChar(char*charName);
	void ExportChar(char*charName,XU16 tag);
	//XBOOL bCompressed;
	XBOOL PushDataDecode(U8* data,S32 chunkLen);
	ScriptPlayer(XBOOL bRoot,XBOOL bInit=XTRUE);
	~ScriptPlayer();
	int  nCompressType;
	enum {XT_NOCOMPRESS=0,XT_COMPRESSED=1,XT_IMAGE=2,XT_LZMA=3};
	void ClearScript(bool bAll=true);

	void PushDataBuf(U8* data, S32 chunkLen,void*gcEnter,bool bFull,bool bDummy);	// push a partial chunk of data
	int  PushDataComplete();					// the current chunk of data is complete, can execute actions and abort a stream...
	void SetDataComplete(XSWFCONTEXT&cnt,XBOOL bOK);						// the stream is complete, close it

	// This is a deprecated method - it is not safe to call this in a loop since PushDataComplete can trigger actions and potentially clear the stream we are writing to
	int PushData(U8* data, S32 chunkLen,void*gcEnter,bool bFull=false);

	// return true if the script data for a given frame is loaded
	BOOL FrameComplete(int f) { return numFramesComplete >= f; }
	BOOL ScriptComplete()
	{ return script && len >= scriptLen; }

	BOOL ParseHeader() { return scriptErr == noErr && gotHeader; } // return true if we have a valid header
	void SetDummy(){m_bDummy=true;}
	
public:
	XBOOL	 m_bDummy;
	XBOOL InitAS3(void*gcEnter,void*pLoader,void*pContext,void*pInfo);
//	XAVM2* CreateAVM2(XPCTSTR strName,XU32 flags,XU8*pABCData,XU32 nLength);
	XXObject* m_pEvent;
protected:
//	XAVM2*	 m_pAVM2;
	
	XExports m_exports;
//#if (__CORE_VERSION__>=0x02075000)	
public:
	XExports m_fontNames;
	int FindFont(const char*fontName);
protected:
//#endif
	XBOOL	 InitDecode();
	XStream* m_pOutput;
	XStream* m_pInput;
	XZLib*	 m_pDecode;
	void*	 m_pLZMADec;
	XBOOL	 m_bLZMAInitOK;
public:
	void AS3_OnOpen();
	void AS3_OnHttpStatus(int nStatus);
	void AS3_OnComplete();
	void AS3_OnInit();
	void AS3_OnIOError(const char*msg);
	void AS3_OnProgress(int nSize=0);
	void AS3_OnUnload();

};

/*! \page STRINGUTILITY String Utilities.
 *	The string utilities are defined in splay.h and provide general
 *	functions for working with Flash strings.<p>
 *  <b>BOOL ConvertStringToInteger(const char *s, int *value)</b><br>
 *	Converts a string to an integer value. Returns true if it succeeds, false if it fails.<p>
 *	<b>char* SkipPrefix( char* str, char* pre )</b><br>
 *  If the prefix pre is found, returns a pointer
 *	past the prefix. If not found, returns null.<p>
 *	<b>BOOL StrEqual( const char* a, const char* b)</b><br>
 *  Returns true if both strings equal.<p>
 *	<b>int ToInt(const char* c)</b><br>
 *  String to integer conversion.<p>
 *	<b>void FreeStr(char* s)</b><br>
 *  Free's memory created with CreateStr.<p>
 *	<b>char* CreateStr( const char* s)</b><br>
 *  Creates a string (alloctating memory)
 *	by copying the string passed in. Returns a pointer to
 *	the created string.<p>
 *	<b>char *CreateStr( const char *s, int count)</b><br>
 *  String utility. Creates a string (alloctating memory)
 *	by copying the string passed in. Count specifies the
 *	amount of memory allocated and must be sufficient.<p>
 *	<b>BOOL IsTrue( const char* )</b><br>
 *  Returns true if the string passed in
 *	a string representation of a non zero number, the
 *  word "true" or the word "yes".<br>
 */

/*!
 *  String utility. Converts a string to an integer value. Returns
 *	true if it succeeds, false if it fails.
 */
BOOL ConvertStringToInteger(const char *s, int *value);

/*! String utility. If the prefix pre is found, returns a pointer
 *	past the prefix. If not found, returns null.
 */
char* SkipPrefix( char* str, char* pre );

/*! String utility. True if both strings equal.
 */
BOOL StrEqual( const char* a, const char* b);

/*! String utility. String to integer conversion.
 */
int ToInt(const char* c);

/*! String utility. Free's memory created with CreateStr.
 */
void FreeStr(char* s);

/*! String utility. Creates a string (alloctating memory)
 *	by copying the string passed in. Returns a pointer to
 *	the created string.
 */
char* CreateStr( const char* s);

/*! String utility. Creates a string (alloctating memory)
 *	by copying the string passed in. Count specifies the
 *	amount of memory allocated and must be sufficient.
 */
char *CreateStr( const char *s, int count);

/*! String utility. Returns true if the string passed in
 *	a string representation of a non zero number, the
 *  word "true" or the word "yes"
 */
BOOL IsTrue( const char* );

//
// Inlines for our basic parser
//

inline U8 SParser::GetByte()
{
	//if ( pos >= end ) return 0;
	return script[pos++];
}

inline U8 SParser::GetBitsByte()
{
	U8 v=script[pos++];
	if(bitPos) v>>=bitPos;
	bitPos=0;
	return v;
}

inline U16 SParser::GetWord()
{
	U8 * s = script+pos;
	pos+=2;
	//if ( pos > end ) return 0;
	return (U16)s[0] | ((U16)s[1]<<8);
}

inline U32 SParser::GetDWord()
{
	U8 * s = script+pos;
	pos+=4;
	//if ( pos > end ) return 0;
	return (U32)s[0] | ((U32)s[1]<<8) | ((U32)s[2]<<16) | ((U32)s[3]<<24);
}

inline GLfloat SParser::GetFixedtoFloat()
{
	U8 * s = script+pos;
	pos+=4;
	if(s[3]>>7==0)
		return (U32)s[2]+((U32)s[3]<<8)+((U32)s[0]+((U32)s[1]<<8))/65536.0;
	else
		return (-1.0)*((256-(U32)s[2])+((255-(U32)s[3])<<8)+((U32)s[0]+((U32)s[1]<<8))/65536.0);
}

inline GLfloat SParser::GetFixedtoFloat8()
{
	U8 * s = script+pos;
	pos+=2;
	if(s[1]>>7==0)
		return (U32)s[1]+(U32)s[0]/256.0;
	else
		return (-1.0)*((256-(U32)s[1])+(U32)s[0]/256.0);
}

inline GLfloat SParser::GetFloat32()
{
	U8 * s = script+pos;
	pos+=4;

	int temp_i = (((U32)s[3]&127)<<1)+((U32)s[2]>>7);
	if(temp_i<=0||temp_i>=255)
		return 0.0;
	float b;
	if (temp_i==127)
	{
		b=1;
	}
	else if(temp_i>127)
	{
		b=1<<(temp_i-127);
	}
	else if(temp_i<127)
	{
		b=1.0/(1<<(127-temp_i));
	}

	return ((U32)s[3]>>7==0?1:-1)*b*(1.0+((((U32)s[2]&127)<<16)|(U32)s[1]<<8|(U32)s[0])*1.0/(1<<23));
}

inline XU32 SParser::GetU30()
{
	XU32 result = script[pos];

	if (!(result & 0x00000080))
	{
		pos++;
		return result;
	}
	pos++;
	result = (result & 0x0000007f) | script[pos]<<7;
	if (!(result & 0x00004000))
	{
		pos ++;
		return result;
	}
	pos++;
	result = (result & 0x00003fff) | script[pos]<<14;
	if (!(result & 0x00200000))
	{
		pos ++;
		return result;
	}
	pos++;
	result = (result & 0x001fffff) | script[pos]<<21;
	if (!(result & 0x10000000))
	{
		pos++;
		return result;
	}
	pos++;
	result = (result & 0x0fffffff) | script[pos]<<28;
	pos++;
	return result;
}

inline void SParser::SkipU30(int nCount)
{
	while (nCount-- > 0)
	{
		if (!(script[pos] & 0x80)) { pos ++; continue; }
		pos++;
		if (!(script[pos] & 0x80)) { pos ++; continue; }
		pos++;
		if (!(script[pos] & 0x80)) { pos ++; continue; }
		pos++;
		if (!(script[pos] & 0x80)) { pos ++; continue; }
		//if (!(p[1] & 0x80)) { p += 2; continue; }
		//if (!(p[2] & 0x80)) { p += 3; continue; }
		//if (!(p[3] & 0x80)) { p += 4; continue; }
		//if (!(*p[4] & 0x80)) { p += 5; continue; }	// test should be unnecessary
		//AvmAssert(!(p[4] & 0x80));
		pos+=2;
		//p += 5;
	}
}

//
// LoadInfo
//
// This structure is used for Load Sprite and Load Variable actions
//
namespace avmshell
{
	class DisplayObjectObject;
	class EventDispatcherObject;
	class LoaderObject;
};

#define XReleaseINFO(INFO)\
{\
		INFO->strURL.Release();\
		INFO->strTarget.Release();\
		INFO->strContext.Release();\
		INFO->strHeaders.RemoveAll();\
		INFO->strRefer.Release();\
		INFO->postData.RemoveAll(XTRUE);\
		if(INFO->pLoader) m_pAVM2->RemoveObject(INFO->pLoader);\
		FREEOBJECT(INFO->pObject);\
		display.FreeLoad(INFO);\
}

typedef struct _XLoadInfo
{
	//enum {XL_HTML=1,XL_TARGET=2,XL_VARS=4};
//#if (__CORE_VERSION__>=0x02075000)
	XU32 nLoadID;
//#endif
	int layer,nTarget;
	//char *spriteName;
	//char *url;
	//char *window;
	XXVar strURL;
	XXVar strRefer;
	//XXVar strRefer;
	XXVar strTarget;
	XXVar strContext;
	XString8Array strHeaders;
	//XXVar strSpriteName;
	XXObject*pObject;
	avmshell::EventDispatcherObject*pLoader;

	//BOOL loadVars;
	int nType;
	XU8Array postData;
	_XLoadInfo*pNext;
//#if (__CORE_VERSION__>=0x02077000)
	void* pClient;
//#endif
	void Init(int l,const char*url,const char*target,
			  XXObject*po,void*pa,int t,const char*context);
	bool IsTargetLoader()
	{
		if((nType&(kHttpLoadVariables|kHttpLoadContext|kHttpLoadRPC))) return true;
		return false;
	}
}LoadInfo;

/*! The structure associated with a download stream. The structure needs to be allocated and
 *  attached to the stream, but the Flash core will do all the writing that is needed.
 */
typedef struct _XStreamData
{
	XXVar			url;			//!< The URL of the stream.
	ScriptPlayer*	scriptPlayer;	//!< Pointer to the ScriptPlayer associated with this download stream.
	void*		    pLoader;
	BOOL			resize;			//!< Should the window be resized to this stream dimension?
	int			    nTotalBytes;
	int				nLoadedBytes;
	bool		    notFirst;
	bool			isBDT;
	int				nOffset;
	_XLoadInfo*	    loadInfo;
#ifdef _WINEMU_DEBUGFILE
	XFile			m_downFile;
#endif
//	LoadInfo*		info;
}StreamData;


#endif



