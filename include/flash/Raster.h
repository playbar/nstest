/****************************************************************************
 CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
 is Copyright � Macromedia, Inc. 1993-1998, All Rights Reserved.  This
 document is an unpublished trade secret of Macromedia, Inc. and may not be
 viewed, copied or distributed by anyone, without the specific, written
 permission of Macromedia, Inc. 
 ****************************************************************************/
#include "XMutex.h"

#ifndef RASTER_INCLUDED
#define RASTER_INCLUDED

#ifndef SHAPE_INCLUDED
#include "shape.h"
#endif
#ifndef BITBUF_INCLUDED
#include "Bitbuf.h"
#endif
#ifndef SBITMAP_INCLUDED
#include "sbitmap.h"
#endif
#include "XDrawDib.h"
#include "Raster_GL.h"
#define SMOOTHBITS 1
#define XMAXTHREAD 8
class XRaster;
class XBitmapCore;

struct _XRActiveEdge;
struct _XRRun;
struct _XRColor;
struct _XRColorMap;
struct _XREdge;
struct _XBltInfo;
struct _XRBit;
typedef void (*DrawSlabProc)(_XRColor* color, S32 xmin, S32 xmax);
typedef void (*CompositeSlabProc)(_XRColor* color, S32 xmin, S32 xmax, RGB8* buf);
typedef void (*CalcPixelProc)(_XRColor* color, S32 x, RGBI* buf);

typedef void (*GetBackgroundProc)(XRaster*, S32 xmin, S32 xmax, RGB8* pix);
typedef void (*DrawRGBSlabProc)(XRaster*, S32 xmin, S32 xmax, RGB8* pix);
typedef void (*DitherRGBSlabProc)(XRaster*, S32 xmin, S32 xmax, RGB8* pix, U8 * dst);


typedef void (*BltProc)(_XBltInfo bi, SPOINT& pt, int n, void* dst);
typedef void (*BltProcI)(_XBltInfo bi, SPOINT& pt, int n, RGBI* dst);

#ifdef ENABLE_MASK
typedef void (*GetBackProc)(void* data, int x, int y, RGBI* pix);
#endif

//
// The Scan Line Renderer
//

//void DrawSolidSlab8	(_XRColor*, S32 xmin, S32 xmax);
//void DrawSolidSlab16	(_XRColor*, S32 xmin, S32 xmax);
//void DrawSolidSlab24	(_XRColor*, S32 xmin, S32 xmax);
void DrawSolidSlab32	(_XRColor*, S32 xmin, S32 xmax);
void DrawDitherSlab	(_XRColor*, S32 xmin, S32 xmax);

void BuildSolidSlab  (_XRColor*, S32 xmin, S32 xmax, RGB8* pixBuf);
void BuildBitmapSlab (_XRColor*, S32 xmin, S32 xmax, RGB8* pixBuf);
void BuildGradientSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* pixBuf);
void BuildLinearGradientSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* pixBuf);
void BuildRadialGradientSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* pixBuf);

void CalcSolidPixel   (_XRColor*, S32 x, RGBI* pixBuf);
void CalcBitmapPixel  (_XRColor*, S32 x, RGBI* pixBuf);
void CalcGradientPixel(_XRColor*, S32 x, RGBI* pixBuf);

void DrawBitmapSlab	(_XRColor*, S32 xmin, S32 xmax);
void DrawGradientSlab(_XRColor*, S32 xmin, S32 xmax);

//friend void DrawRGBSlab1(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
//friend void DrawRGBSlab2(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
//friend void DrawRGBSlab4(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
//void DrawRGBSlab8(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
//void DrawRGBSlab16(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
//friend void DrawRGBSlab16A(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
//void DrawRGBSlab24(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
void DrawRGBSlab32(XRaster*, S32 xmin, S32 xmax, RGB8* pix);
//void DrawRGBSlab32A(XRaster*, S32 xmin, S32 xmax, RGBI* pix);

//void DitherRGBSlabNone(XRaster*, S32 xmin, S32 xmax, RGBI* pix, U8 * dst);
//void DitherRGBSlabOrdered(XRaster*, S32 xmin, S32 xmax, RGBI* pix, U8 * dst);
//void DitherRGBSlabErrorDiffusion(XRaster*, S32 xmin, S32 xmax, RGBI* pix, U8 * dst);

void CompositeSolidSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* buf);
void CompositeGradientSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* buf);
void CompositeBitmapSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* buf);

//friend void GetBackground1(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
//friend void GetBackground2(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
//friend void GetBackground4(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
//void GetBackground8(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
//void GetBackground16(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
//friend void GetBackground16A(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
//void GetBackground24(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
void GetBackground32(XRaster* r, S32 xmin, S32 xmax, RGB8* pix);
void GetBackgroundWhite(XRaster* r, S32 xmin, S32 xmax, RGB8* pix);

// Bitmap Drawing prototypes

// General Blts
#ifdef SMOOTHBITS
//void BltXto32(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
void Blt8to32S(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
void Blt16to32S(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
void Blt32to32S(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);

void BltXto32SA(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
void Blt8to32SA(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
void Blt16to32SA(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
void Blt32to32SA(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);


//void BltXtoI(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
void Blt8toIS(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
void Blt16toIS(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
void Blt32toIS(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);

void BltXtoISA(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
void Blt8toISA(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
void Blt16toISA(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
void Blt32toISA(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);

#endif

void BltXto(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
void Blt8toI(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
void Blt16toI(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
void Blt32toI(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);

// Fast Blts
//void Blt8to8(_XBltInfo bi, SPOINT& pt, int n, U8 * dst);
//void Blt8to8D(_XBltInfo bi, SPOINT& pt, int n, U8 * dst);
//void Blt8to16(_XBltInfo bi, SPOINT& pt, int n, U16 * dst);
//void Blt8to16A(_XBltInfo bi, SPOINT& pt, int n, U16 * dst);
void Blt8to32(_XBltInfo bi, SPOINT& pt, int n, U32 * dst);

//void Blt16to8(_XBltInfo bi, SPOINT& pt, int n, U8 * dst);
//void Blt16to8D(_XBltInfo bi, SPOINT& pt, int n, U8 * dst);
//void Blt16to16(_XBltInfo bi, SPOINT& pt, int n, U16 * dst);
//void Blt16to16A(_XBltInfo bi, SPOINT& pt, int n, U16 * dst);
void Blt16to32(_XBltInfo bi, SPOINT& pt, int n, U32 * dst);

//void Blt32to8(_XBltInfo bi, SPOINT& pt, int n, U8 * dst);
//void Blt32to8D(_XBltInfo bi, SPOINT& pt, int n, U8 * dst);
//void Blt32to16(_XBltInfo bi, SPOINT& pt, int n, U16 * dst);
//void Blt32to16A(_XBltInfo bi, SPOINT& pt, int n, U16 * dst);
void Blt32to32(_XBltInfo bi, SPOINT& pt, int n, U32 * dst);

void DoEdgeEdgeRule(XRaster*, _XRActiveEdge*);
void DoEdgeEvenOddRule(XRaster*, _XRActiveEdge*);
void DoEdgeWindingRule(XRaster*, _XRActiveEdge*);

enum { solidPat = 0, disablePat = 1, highlightPat = 2, lineHighlightPat = 3 };
enum { fillEdgeRule = 0, fillEvenOddRule, fillWindingRule };
enum { noDither = 0, orderedDither = 1, errorDiffusionDither = 2 };

struct _XRGBErr;

class XRaster {
	static XRaster* instance;
public:
	XMutex	dibLock;
	XU8	 antiShift,antiMask,antiStep;
	BOOL antialias_b;
	BOOL getBackground;		// set this if we might be painting on a non-white background...
	BOOL smoothBitmaps;		// turns on interpolation for scaled bitmaps
	int bitmapDither;		// dither type for bitmaps
	int solidDither;		// dither type for solid colors - note that if this is set to errorDiffusion dither, it simply uses the bitmapDither type
	
	BOOL needFlush;
	int layerDepth;
	
public:
	CBitBuffer* bits;
	SColorInfo* cinfo;
	SRECT edgeClip;	// The clip rect in higher res coords (for antialiasing)
	SRECT bitClip;	// The clip rect in bitmap coords
	SPOINT patAlign;
	
#ifdef ENABLE_MASK
public:
	void* getBackData;
	GetBackProc getBackProc;
	CBitBuffer* maskBits;
	
	char  * maskRowAddr;
	
	void SetMask(int xmin, int xmax);
#endif
	
private:
	// The active color list
	_XRColor *topColor;
	S32 topColorXleft;	// the left edge of the current slab being processed
	
	// Error diffusion context
	int ditherSize;
	U8* ditherData;
	int ditherErrSize;
	void* curDitherErr;
	void* nextDitherErr;
	
	// Context for Paint()
	_XRActiveEdge* firstActive;
	_XREdge** yindex;
	S32 yindexSize;
	S32 ylines;
	S32 y;
	
    // Antialiasling info
    _XRRun* firstRun;
    _XRRun* curRun;
	
private:
	// Fast Memory Allocators
	/*XChunkAlloc activeEdgeAlloc;
	XChunkAlloc runAlloc;
	XChunkAlloc rampAlloc;
	XChunkAlloc mapAlloc;*/
	XFastAlloc activeEdgeAlloc;
	XFastAlloc runAlloc;
	XFastAlloc rampAlloc;
	XFastAlloc mapAlloc;

	inline _XRActiveEdge* CreateActiveEdge() { return (_XRActiveEdge*)(activeEdgeAlloc.Alloc()); }
	inline void FreeActiveEdge(_XRActiveEdge* a) { activeEdgeAlloc.Free(a); }
	
	inline _XRRun* CreateRun() { return (_XRRun*)(runAlloc.Alloc()); }
	inline void FreeRun(_XRRun* c) { runAlloc.Free(c); }
	
	inline RGB8* CreateRamp() { return (RGB8*)(rampAlloc.Alloc()); }
	inline void FreeRamp(RGB8* c) { rampAlloc.Free(c); }
	
	inline _XRColorMap* CreateMap() { return (_XRColorMap*)(mapAlloc.Alloc()); }
	inline void FreeMap(_XRColorMap* c) { mapAlloc.Free(c); }
	
 	friend struct _XRColor;
 	friend struct _XRRun;
 	friend struct _XRPixel;
	
	friend void DoEdgeEdgeRule(XRaster*, _XRActiveEdge*);
	friend void DoEdgeEvenOddRule(XRaster*, _XRActiveEdge*);
	friend void DoEdgeWindingRule(XRaster*, _XRActiveEdge*);
	
private:
	// The sub steps for Painting
	void AddActive();
	void SortActive();
	
	void PaintSlab(S32 xright);
	void ShowColor(_XRColor*, S32 x);
	void HideColor(_XRColor*, S32 x);
	
	void PaintActive();
	void PaintAARuns();
	
private:
	// Our local cached bitmap information
	BOOL inverted;
	char  * baseAddr;
	S32 rowBytes;		// should always be a multiple of 4 bytes
	int pixelFormat;
	S32 bitHeight;
	S32 xorg;			// x pixel alignment
	
	// Current line context
	char  * rowAddr;
	S32 bitY;
	
	// Draw procs
	DrawRGBSlabProc drawRGBSlab;
	//DitherRGBSlabProc ditherRGBSlab;
	GetBackgroundProc doGetBackground;
	//GetBackgroundProc8 doGetBackground8;
	
public:
	//friend void DrawSolidSlab1	(_XRColor*, S32 xmin, S32 xmax);
	//friend void DrawSolidSlab2	(_XRColor*, S32 xmin, S32 xmax);
	//friend void DrawSolidSlab4	(_XRColor*, S32 xmin, S32 xmax);
	//friend void DrawSolidSlab8	(_XRColor*, S32 xmin, S32 xmax);
	//friend void DrawSolidSlab16	(_XRColor*, S32 xmin, S32 xmax);
	//friend void DrawSolidSlab24	(_XRColor*, S32 xmin, S32 xmax);
	friend void DrawFontSlab32	(_XRColor*, S32 xmin, S32 xmax);
	friend void DrawSolidSlab32	(_XRColor*, S32 xmin, S32 xmax);
	friend void DrawDitherSlab	(_XRColor*, S32 xmin, S32 xmax);
	
	friend void BuildSolidSlab  (_XRColor*, S32 xmin, S32 xmax, RGB8* pixBuf);
	friend void BuildBitmapSlab (_XRColor*, S32 xmin, S32 xmax, RGB8* pixBuf);
	friend void BuildGradientSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* pixBuf);
	friend void BuildLinearGradientSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* pixBuf);
	friend void BuildRadialGradientSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* pixBuf);
	
	friend void CalcSolidPixel   (_XRColor*, S32 x, RGBI* pixBuf);
	friend void CalcBitmapPixel  (_XRColor*, S32 x, RGBI* pixBuf);
	friend void CalcGradientPixel(_XRColor*, S32 x, RGBI* pixBuf);
	friend void CalcFontPixel   (_XRColor*, S32 x, RGBI* pixBuf);
	friend bool IsCopyLine		(_XRColor*,S32 xmin,S32 xmax);
	friend void DrawBitmapSlab	(_XRColor*, S32 xmin, S32 xmax);
	friend void DrawGradientSlab(_XRColor*, S32 xmin, S32 xmax);
	friend void DrawBitmapSlabGrid(_XRColor*, S32 xmin, S32 xmax);
	friend void BuildBitmapSlabGrid(_XRColor*, S32 xmin, S32 xmax, RGB8* pixBuf);
	
	//friend void DrawRGBSlab1(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
	//friend void DrawRGBSlab2(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
	//friend void DrawRGBSlab4(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
	//friend void DrawRGBSlab8(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
	//friend void DrawRGBSlab16(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
	//friend void DrawRGBSlab16A(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
	//friend void DrawRGBSlab24(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
	friend void DrawRGBSlab32(XRaster*, S32 xmin, S32 xmax, RGB8* pix);
	//friend void DrawRGBSlab32A(XRaster*, S32 xmin, S32 xmax, RGBI* pix);
	
	//friend void DitherRGBSlabNone(XRaster*, S32 xmin, S32 xmax, RGBI* pix, U8 * dst);
	//friend void DitherRGBSlabOrdered(XRaster*, S32 xmin, S32 xmax, RGBI* pix, U8 * dst);
	//friend void DitherRGBSlabErrorDiffusion(XRaster*, S32 xmin, S32 xmax, RGBI* pix, U8 * dst);
	friend void CompositeFontSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* buf);
	friend void CompositeFontSlabDrag(_XRColor*, S32 xmin, S32 xmax, RGB8* buf);
	friend void CompositeSolidSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* buf);
	friend void CompositeGradientSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* buf);
	friend void CompositeBitmapSlab(_XRColor*, S32 xmin, S32 xmax, RGB8* buf);
	friend void CompositeBitmap(_XRColor* color, S32 xmin, S32 xmax, RGB8* buf);
	//friend void GetBackground1(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
	//friend void GetBackground2(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
	//friend void GetBackground4(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
	//friend void GetBackground8(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
	//friend void GetBackground16(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
	//friend void GetBackground16A(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
	//friend void GetBackground24(XRaster* r, S32 xmin, S32 xmax, RGBI* pix);
	friend void GetBackground32(XRaster* r, S32 xmin, S32 xmax, RGB8* pix);
	friend void GetBackgroundWhite(XRaster* r, S32 xmin, S32 xmax, RGB8* pix);
	
	// Bitmap Drawing prototypes
	
	// General Blts
#ifdef SMOOTHBITS
	//friend void BltXto32S(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
	friend void Blt8to32S(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
	friend void Blt16to32S(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
	friend void Blt32to32S(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
	
	friend void BltXto32SA(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
	friend void Blt8to32SA(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
	friend void Blt16to32SA(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
	friend void Blt32to32SA(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);


	friend void Blt8toIS(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
	friend void Blt16toIS(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
	friend void Blt32toIS(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
	
	friend void BltXtoISA(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
	friend void Blt8toISA(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
	friend void Blt16toISA(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
	friend void Blt32toISA(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
	
#endif
	
	friend void BltXto(_XBltInfo bi, SPOINT& pt, int n, RGB8* pix);
	friend void Blt8toI(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
	friend void Blt16toI(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
	friend void Blt32toI(_XBltInfo bi, SPOINT& pt, int n, RGBI* pix);
	
	// Fast Blts
	//friend void Blt8to8(_XBltInfo bi, SPOINT& pt, int n, U8 * dst);
	//friend void Blt8to8D(_XBltInfo bi, SPOINT& pt, int n, U8 * dst);
	//friend void Blt8to16(_XBltInfo bi, SPOINT& pt, int n, U16 * dst);
	//friend void Blt8to16A(_XBltInfo bi, SPOINT& pt, int n, U16 * dst);
	friend void Blt8to32(_XBltInfo bi, SPOINT& pt, int n, U32 * dst);
	
	//friend void Blt16to8(_XBltInfo bi, SPOINT& pt, int n, U8 * dst);
	//friend void Blt16to8D(_XBltInfo bi, SPOINT& pt, int n, U8 * dst);
	//friend void Blt16to16(_XBltInfo bi, SPOINT& pt, int n, U16 * dst);
	//friend void Blt16to16A(_XBltInfo bi, SPOINT& pt, int n, U16 * dst);
	friend void Blt16to32(_XBltInfo bi, SPOINT& pt, int n, U32 * dst);
	
	//friend void Blt32to8(_XBltInfo bi, SPOINT& pt, int n, U8 * dst);
	//friend void Blt32to8D(_XBltInfo bi, SPOINT& pt, int n, U8 * dst);
	//friend void Blt32to16(_XBltInfo bi, SPOINT& pt, int n, U16 * dst);
	//friend void Blt32to16A(_XBltInfo bi, SPOINT& pt, int n, U16 * dst);
	friend void Blt32to32(_XBltInfo bi, SPOINT& pt, int n, U32 * dst);
	
private:
	void IndexToRGB(int index, RGBI* color);
	int RGBToIndex(RGBI* color);
	void DrawSubPixel(RGBI* color, int coverage, U8 * bits, int mask, int shift);
	
	void SetYCoord(S32 y);
	void DitherRGBSlab(S32 xmin, S32 xmax, RGBI* pix, U8 * dst);
	
	void DrawRGBAPixelI(S32 x, RGBI* rgb);
	void DrawRGBAPixel(S32 x, RGB8* rgb);
	void DrawRGBASlab(S32 xleft, S32 xright, RGB8*);
	void CompositeSlab(S32 xleft, S32 xright, _XRColor** stack, int n);
	void GetBackground(S32 xleft, S32 xright, RGBI* pix);
	
public:
	XRaster();
	~XRaster();
	void FreeEmpties();		// free unused memory chunks
	
	void Attach(CBitBuffer*, SRECT* clip, BOOL doAntialias,XU8 antiShift);
	BOOL BitsValid();
	void SetPatAlign(P_SPOINT pt) { patAlign = *pt; }
	void UseAlphaChannel();
	
	void FillBackground(SRGB); // fill a solid background with a transparent alpha value
	
	// Painting
	void BeginPaint();
	void AddEdges(_XREdge*, _XRColor* colors, _XRColor* clipColor=0);//,_GRIDINFO*gridBy=0);	// also adjust the colors for depth
	void AddEdges(_XREdge*);
	void PaintBits(bool bBlack=false);
	void Flush();
	XS32 bitArea;
#ifdef _MultiThread_
	friend class XImageThread;
#endif
	friend class XSWFPlayer;
};
typedef XRaster CRaster;

//
// The Color Transform
//
//
// The Edge Structure
//

typedef struct _XREdge {
	_XREdge *nextObj;		// The next edge belonging to this object
	_XREdge *nextActive;	// The next edge that becomes active on this scanline
	_XRColor *color1, *color2;
	S16 anchor1x;		// store the CURVE as 16 bit ints to save RAM
	S16 anchor1y;
	S16 controlx;
	S16 controly;
	S16 anchor2x;
	S16 anchor2y;
	int isLine;
	U8 fillRule;
	S8 dir;
	
	inline void Set(CURVE* c) {
		anchor1x = (S16)c->anchor1.x;
		anchor1y = (S16)c->anchor1.y;
		controlx = (S16)c->control.x;
		controly = (S16)c->control.y;
		anchor2x = (S16)c->anchor2.x;
		anchor2y = (S16)c->anchor2.y;
		isLine   = CurveFlatness(c) <= 1;
	}
	
	inline void Get(CURVE* c) {
		c->anchor1.x = anchor1x;
		c->anchor1.y = anchor1y;
		c->control.x = controlx;
		c->control.y = controly;
		c->anchor2.x = anchor2x;
		c->anchor2.y = anchor2y;
		c->isLine = isLine;
	}
}REdge;

//
// The Color Structure
//

typedef struct _XBltInfo {
	_XRColor* color;
	SFIXED dx;		// delta for the next sample point (moving left one dest pixel)
	SFIXED dy;
	
	U8 * baseAddr;	// source bitmap description
	S32 rowBytes;	
	RGB8* colors;
	int width;
	int height;
	
	U8* itab;		// inverse table for destination pixels
}BltInfo;

typedef struct _XRColorMap {
	U8 red[256];
	U8 green[256];
	U8 blue[256];
	U8 alpha[256];
	
	U8 hasAlpha;
}RColorMap;



typedef struct _XRColorBMInfo {
	U16 bitsStyle;
	U8 smooth;
	SBitmapCore* bitmap;
	MATRIX savedMat;
	MATRIX invMat;
	//MATRIX scaleMat;
	MATRIX gridMat;
	MATRIX orgMat;
	//MATRIX devMat;
	SRECT  gridBy;
	SRECT  gridRect;
	SRECT  orgRect;
	int    offX,offY;
	ColorTransform cxform;
	_XRColorMap* colorMap;
	//XU16*		 lineAlpha;
	
	BltProc bltProc;
	BltProc compositeProc;
	BltProcI bltProcI;
	BltProc fastBltProc;
	_XBltInfo bi;		// cached blting info
}RColorBMInfo;

typedef struct _XRColorGradInfo {
	int gradStyle;
  	MATRIX savedMat;
	MATRIX invMat;
	GradColorRamp ramp;
	RGB8* colorRamp;
}RColorGradInfo;

enum {
	colorSolid, 	// a solid RGB color
	colorBitmap, 	// a bitmap 
	colorGradient, 	// a gradient ramp
	colorClip, 	// a clip color
};




typedef struct _XRColor {
	XRaster* raster;		// The render object that owns this color
	_XRColor* nextActive;		// The next color in the active color list
	_XRColor* nextColor;		// The next entry in the color list
	U32 order;				// the stacking order
	_XRColor* clippedBy;		// A list of colors that clip (hide) this color
	//SRECT*	  gridBy0;	
	U8 transparent;			// true if this color could have partial transparency
	S8 visible;				// 0 if not visible, != 0 if visible
#ifdef _MultiThread_
	S8 visibleThread[XMAXTHREAD];
#endif
	U8 colorType;			// solid, bitmap, gradient
	U8 cacheValid;
	U8 copyColor;
	U8 nFilterColor;
	//U8 blendMode;
	DrawSlabProc drawSlabProc; // draw the bits direct to the device bits
	CompositeSlabProc compositeSlabProc; // draw or composite to an RGBI buffer
	
	RGBI rgb;				// the expanded color components
	//int pattern;			// The pattern of the object
	U32 rgbPoint;			// the x,y location that rgb currently represents in device pixels, rgbPoint = (y<<16) | x
	
	union {
		XU32  pat;
		_XRColorBMInfo bm;	// for bitmaps, the source bitmap info
		_XRColorGradInfo grad;// for gradients, the gradient info
	};
	
	void SetUp(XRaster* r);
	
	void BuildCache();
	void BuildGrid();
	void FreeCache();
	void SetBitmapDiv();
}RColor;


//
// Helper Functions
//

//
// These are helper for SBitmap::GetSSPixel()
//

// Tables for extracting pixels from bytes
//extern const U8 shiftTab1[8];
//extern const U8 shiftTab2[4];
//extern const U8 shiftTab4[2];
//extern const U8 shiftTab8[1];

// bm1bit, bm2bit, bm4bit, bm8bit
//extern const U8* shiftTabs[4];
//extern const U8  rbShifts[4];
//extern const U8  pxMasks[4];
//extern const U8  stMasks[4];


// JLH: Added for kraster.cpp
void CompositeRGB(RGBI* src, RGBI* dst, int n);
void CompositeRGB8(RGB8* src, RGB8* dst, int n);
void CompositeRGB8Bitmap(RGB8* src,RGB8* dst,int n);


//enum { maxRunStack = 12 };
enum { maxRunStack = 16 };
typedef struct _XRRunStack {
	_XRColor* colors[maxRunStack];
}RRunStack;

typedef struct _XRRunCore {// these fields are always copied on a Split()
	S32 xmin, xmax;
	_XRRun* next;
	int nColors;
	U8 isComplex;	// there are non-solid colors in this run
	U8 isPure;		// all color stacks are the same
	U8 transparent;	// there is a transparent color in this run
	U8 hasCopyLayer;
	U8 stackN[4];
}RRunCore;

typedef struct _XRRun : public _XRRunCore {
	RRunStack stacks[4];

	void Init(S32 l, S32 r);
	void AddColor(_XRColor*);
	void CalcColor(S32 x, S32 y, RGBI* rgb);
	_XRRun* Split(XRaster*, S32 x);
}RRun;

//
// Doubly Linked List
//

// head must be the pointer to the list start
// elem must have a next and prev field

// Add to the elem to the front of the list
#define DListAdd(head, elem, type) { \
		if ( head ) \
			head->prev = elem; \
		elem->next = head; \
		elem->prev = 0; \
		head = elem; \
	}

// Add elem after node
#define DListInsertAfter(node, elem, type) { \
		type *tmp; \
		if ( (tmp = node->next) != 0 ) \
			tmp->prev = elem; \
		elem->next = tmp; \
		elem->prev = node; \
		node->next = elem; \
	}

// Remove elem from the list
#define DListRemove(head, elem, type) \
	{ \
		type *tmp; \
		if ( (tmp = elem->prev) != 0 ) \
			tmp->next = elem->next; \
		else if ( head == elem ) \
			head = elem->next; \
		if ( (tmp = elem->next) != 0 ) \
			tmp->prev = elem->prev; \
		elem->next = elem->prev = 0; \
	}

// Swap two elements, first must be immediately before second
#define DListSwap(head, first, second, type) \
	{ \
		DListRemove(head, first, type); \
		DListInsertAfter(second, first, type); \
	}
#endif
