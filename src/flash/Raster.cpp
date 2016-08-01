#include "StdAfxflash.h"
#ifdef _SYMBIAN
	#include <string.h>
#else
	#include <memory.h>	   // lee@middlesoft - thanks E. Hyche, RealNetworks, for providing header corrections.
#endif
#include "Raster.h"
#include "Bitbuf.h"
#include "sbitmap.h"
#include "XXVar.h"
#include "edittext.h"
#include "splayer.h"
#include "XDibSrv.h"

#define _DIRECTDRAW_
//#define _LINEALPHA_
//#define _FASTBITMAP_
#define HALFOFF 32768

#define MAXCOLORSTACK 16
//#define MAXCOLORSTACK 255
#ifdef _DIRECTDRAW_
	#define _ROWADDR(R,X) ((U32 *)R->rowAddr + X);
#endif

#ifdef _LINEALPHA_
inline bool IsCopyLine		(_XRColor* color,S32 xmin,S32 xmax)
{	
	if(color->colorType!=colorBitmap) return false;
	if(!color->bm.bitmap->lineAlpha||color->gridBy0||color->bm.colorMap) return false;
	if(color->bm.bi.dy) return false;

	XRaster* r = color->raster;
	SPOINT pt;
	int y = r->bitY;
	pt.x = (xmin << XBITMAPOFF)+HALFOFF;
	pt.y = y << XBITMAPOFF;
	MatrixTransformPoint(&color->bm.invMat,&pt,&pt);
	y=(pt.y>>(XBITMAPOFF));
	if(y<0||y>=color->bm.bi.height) return false;
	y<<=1;
	if(color->bm.bi.dx>0)
	{
		xmax=pt.x+(xmax-xmin)*color->bm.bi.dx;
		xmin=pt.x;		
	}
	else
	{
		xmin=pt.x+(xmax-xmin)*color->bm.bi.dx;
		xmax=pt.x;
	}
	xmin>>=XBITMAPOFF;
	xmax>>=XBITMAPOFF;
	XU16* data=color->bm.bitmap->lineAlpha+y;
	if(xmin<data[0]||xmax>data[1]) return false;
	return true;
}
#endif

enum { RGBSlabChunkSize=1024 };

// Rendering tables

CoverEntry PixCoverage[8][8];
BOOL renderTablesBuilt = false;

enum { dd = 0x7 };
const U8 ditherNoise[8*8] = {
		dd,00,00,dd,00,dd,dd,00,
		00,dd,00,dd,00,dd,00,dd,
		00,dd,dd,00,dd,00,dd,dd,
		00,dd,dd,00,dd,00,dd,dd,
		dd,00,dd,00,00,dd,00,dd,
		00,dd,00,dd,dd,00,dd,00,
		dd,dd,00,dd,00,dd,00,00,
		00,dd,dd,00,dd,00,dd,dd,
	};

const U8 ditherNoiseX[8*8] = {
		0,1,2,3,1,2,1,0,
		2,3,0,1,0,3,2,3,
		3,1,2,0,2,1,2,0,
		1,0,3,2,1,0,1,3,
		2,3,1,0,3,2,3,2,
		0,0,3,1,2,3,1,3,
		3,2,1,0,0,2,0,1,
		2,3,0,1,0,3,2,3,
	};

// A dither matrix for reducing 24bit to 16 bit color
const U8 ditherNoise16[4*4] = {
		7,0,5,1,	// Random dither
		2,6,4,3,
		4,3,7,2,
		0,6,1,5,
	};

// Tables for our ordered dither
const U8 ditherNoise8[8*8] = {	// 0..31
		 1/2, 17/2,  0/2, 16/2,  2/2, 18/2, 30/2, 15/2,
		21/2, 13/2,  5/2, 28/2, 22/2, 14/2,  6/2, 27/2,
		 9/2, 29/2, 20/2,  4/2, 10/2, 31/2, 23/2,  3/2,
		25/2,  8/2, 24/2, 12/2, 26/2,  7/2, 11/2, 19/2,

		 3/2, 19/2, 30/2, 14/2,  4/2, 20/2,  0/2, 13/2,
		23/2, 15/2,  7/2, 26/2, 24/2, 16/2,  8/2, 25/2,
		11/2, 31/2, 18/2,  2/2, 12/2, 29/2, 21/2,  1/2,
		27/2,  6/2, 22/2, 10/2, 28/2,  5/2,  9/2, 17/2,
	};

// Tables for the the 16 bit dither
#define p16pR(x) (x<<10)
#define p16pG(x) (x<<5)
#define p16pB(x) (x<<0)
const U16 pix16PackR[33] = {
				p16pR(0x00), p16pR(0x01), p16pR(0x02), p16pR(0x03),
				p16pR(0x04), p16pR(0x05), p16pR(0x06), p16pR(0x07),
				p16pR(0x08), p16pR(0x09), p16pR(0x0A), p16pR(0x0B),
				p16pR(0x0C), p16pR(0x0D), p16pR(0x0E), p16pR(0x0F),

				p16pR(0x10), p16pR(0x11), p16pR(0x12), p16pR(0x13),
				p16pR(0x14), p16pR(0x15), p16pR(0x16), p16pR(0x17),
				p16pR(0x18), p16pR(0x19), p16pR(0x1A), p16pR(0x1B),
				p16pR(0x1C), p16pR(0x1D), p16pR(0x1E), p16pR(0x1F), p16pR(0x1F)
			};
const U16 pix16PackG[33] = {
				p16pG(0x00), p16pG(0x01), p16pG(0x02), p16pG(0x03),
				p16pG(0x04), p16pG(0x05), p16pG(0x06), p16pG(0x07),
				p16pG(0x08), p16pG(0x09), p16pG(0x0A), p16pG(0x0B),
				p16pG(0x0C), p16pG(0x0D), p16pG(0x0E), p16pG(0x0F),

				p16pG(0x10), p16pG(0x11), p16pG(0x12), p16pG(0x13),
				p16pG(0x14), p16pG(0x15), p16pG(0x16), p16pG(0x17),
				p16pG(0x18), p16pG(0x19), p16pG(0x1A), p16pG(0x1B),
				p16pG(0x1C), p16pG(0x1D), p16pG(0x1E), p16pG(0x1F), p16pG(0x1F)
			};
const U8 pix16PackB[33] = {
				p16pB(0x00), p16pB(0x01), p16pB(0x02), p16pB(0x03),
				p16pB(0x04), p16pB(0x05), p16pB(0x06), p16pB(0x07),
				p16pB(0x08), p16pB(0x09), p16pB(0x0A), p16pB(0x0B),
				p16pB(0x0C), p16pB(0x0D), p16pB(0x0E), p16pB(0x0F),

				p16pB(0x10), p16pB(0x11), p16pB(0x12), p16pB(0x13),
				p16pB(0x14), p16pB(0x15), p16pB(0x16), p16pB(0x17),
				p16pB(0x18), p16pB(0x19), p16pB(0x1A), p16pB(0x1B),
				p16pB(0x1C), p16pB(0x1D), p16pB(0x1E), p16pB(0x1F), p16pB(0x1F)
			};

// Tables for extracting pixels from bytes
const U8 shiftTab1[8] = {7,6,5,4,3,2,1,0};
const U8 shiftTab2[4] = {6,4,2,0};
const U8 shiftTab4[2] = {4,0};
const U8 shiftTab8[1] = {0};

							// bm1bit, bm2bit, bm4bit, bm8bit
const U8* shiftTabs[4] = {shiftTab1, shiftTab2, shiftTab4, shiftTab8};
const U8  rbShifts[4]  = {  3,   2,   1,   0 };
const U8  pxMasks[4]   = {0x1, 0x3, 0xF, 0xFF};
const U8  stMasks[4]   = {0x7, 0x3, 0x1, 0x0 };

#define dr(x) ((x)<<8)
#define dg(x) ((x)<<4)
#define db(x) ((x)<<0)
const U16 ditherTabR[]={dr(0), dr(1), dr(2), dr(3), dr(4), dr(5), dr(6), dr(7), dr(8),
				  dr(9), dr(10), dr(11), dr(12), dr(13), dr(14), dr(15), dr(15), dr(15), dr(15)};
const U8 ditherTabG[] ={dg(0), dg(1), dg(2), dg(3), dg(4), dg(5), dg(6), dg(7), dg(8),
				  dg(9), dg(10), dg(11), dg(12), dg(13), dg(14), dg(15), dg(15), dg(15), dg(15)};
const U8 ditherTabB[] ={db(0), db(1), db(2), db(3), db(4), db(5), db(6), db(7), db(8),
				  db(9), db(10), db(11), db(12), db(13), db(14), db(15), db(15), db(15), db(15)};


static U16 Sq256[258];

enum {XPadMode=0,XReflectMode=1,XRepeatMode=2};

void BuildBitmapSlabGrid(_XRColor* color, S32 xmin, S32 xmax, RGB8* buf);  //add by hgl


inline void GradientFmt(int mode,XS32&l)
{
	switch(mode)
	{
	case XPadMode:
		 if(l>256) l=256;
		 if(l<0) l=0;
		 break;
	case XReflectMode:
		 while(1)
		 {
			 if(l<0) l=-l;
			 else if(l>256) l=512-l;
			 else break;
		 }
		 break;
	case XRepeatMode:
		 l=l&0xff;
		 break;
	}
}

void BuildRenderTables()
{
	renderTablesBuilt = true;

	// Build the coverage table
 	//FLASHOUTPUT("Coverage Table\n");
 	for ( int xf = 0; xf < 8; xf++ ) {
		for ( int yf = 0; yf < 8; yf++ ) {
			int* ce = PixCoverage[xf][yf].ce;
			ce[0] = (8-xf)*(8-yf);
			ce[1] = xf*(8-yf);
			ce[2] = (8-xf)*yf;
			ce[3] = xf*yf;

			// Normalize the sum of coverages to 8
			int big = 0;	// add the error to the largest weight
			int sum = 0;
			for ( int j = 0; j <= 3; j++ ) {
				ce[j] = (ce[j]+4)/8;
				sum += ce[j];
				if ( ce[j] > ce[big] ) big = j;
			}
			int err = 8-sum;
			ce[big] += err;
			//FLASHOUTPUT("%i, ", ce[0]);
		}
		//FLASHOUTPUT("\n");
	}

	// Build a table of the first 256 squares for radial fills
	for ( U32 j = 0; j < 258; j++ )
		Sq256[j] = (U16)Min(0xFFFF, j*j);
}

static const U16 grayPatList[] = {
		0xFFFF, //0
		0x7FFF,
		0x7FDF,
		0x5FDF,
		0x5F5F, //4
		0x5B5F,
		0x5B5E,
		0x5A5E,
		0x5A5A, //8
		0x1A5A,
		0x1A4A,
		0x0A4A,
		0x0A0A, //12
		0x020A,
		0x0208,
		0x0008,
		0x0000  //16
	};

// Use more distinctive patterns on PC for higher dpi screens
#define linePatBits 	0x39C6 // diag lines
#define fillPatBits 	0x33CC // 2x2 blocks

#define solidPatBits 	0xFFFF // solid
#define disablePatBits 	0xA5A5 // 50%

// #endif


//
// Proc Tables
//



const DrawRGBSlabProc DrawRGBSlabProcs[9] = {
//	DrawRGBSlab1,
	//DrawRGBSlab2,
	//DrawRGBSlab4,
	0,//DrawRGBSlab8,
	0,//DrawRGBSlab16,
	//DrawRGBSlab16A,
	0,//DrawRGBSlab24,
	DrawRGBSlab32,
	DrawRGBSlab32,//DrawRGBSlab32A
};

const GetBackgroundProc GetBackgroundProcs[9] = {
	//GetBackground1,
	//GetBackground2,
	//GetBackground4,
	0,//GetBackground8,
	0,//GetBackground16,
	//GetBackground16A,
	0,//GetBackground24,
	GetBackground32,
	GetBackground32
};



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


//
// Helpers for wrapping texture coordinates

inline S32 LimitAbs(S32 v, S32 range)
// An absolute wrapper
{
	S32 i = v/range;
	if ( v < 0 )
		i--;
	return v - i*range;
}

inline int LimitAbsI(int v, S32 range)
// An absolute wrapper
{
	int i = (int)(v/range);
	if ( v < 0 )
		i--;
	return (int)(v - i*range);
}

inline void CalcLimit(int& n, S32& x, S32 dx, S32 limit)
{
	int lim;
	if ( dx > 0 ) {
		while ( x >= limit )
			x -= limit;
		lim = (int)((limit-x+dx-1)/dx);
		if ( n > lim )
			n = lim;
	} else if ( dx < 0 ) {
		while ( x <= 0 )
			x += limit;
		lim = (int)((x-dx-1)/-dx);
		if ( n > lim )
			n = lim;
	}
}


void ApplyRGBPattern(int pattern, int y, int xmin, int xmax, RGBI* pix);

//
// Low-Level Compositing Functions
//
#ifdef _USE64BITS

inline void CompositeRGBPixel(RGBI* src, RGBI* dst)
{
	int a1 = 256-src->alpha;
	*((XU64*)dst)=((*((XU64*)src))+((*((XU64*)dst))*a1>>8))&_MASK64;
	//((RGBIL*)dst)->c0 = (((RGBIL*)src)->c0 + (((RGBIL*)dst)->c0*a1>>8)) & 0x00FF00FF;
	//((RGBIL*)dst)->c1 = (((RGBIL*)src)->c1 + (((RGBIL*)dst)->c1*a1>>8)) & 0x00FF00FF;
}

inline void CompositeRGB(RGBI* src, RGBI* dst, int n)
{
	while ( n-- > 0 ) {
		int a1 = 256-src->alpha;
		*((XU64*)dst)=((*((XU64*)src))+((*((XU64*)dst))*a1>>8))&_MASK64;//
		//((RGBIL*)dst)->c0 = (((RGBIL*)src)->c0 + (((RGBIL*)dst)->c0*a1>>8)) & 0x00FF00FF;
		//((RGBIL*)dst)->c1 = (((RGBIL*)src)->c1 + (((RGBIL*)dst)->c1*a1>>8)) & 0x00FF00FF;
		src++; dst++;
	}
}

inline void CompositeRGBSolid(RGBI* src, RGBI* dst, int n)
{
	while ( n-- > 0 ) {
		int a1 = 256-src->alpha;
		*((XU64*)dst)=((*((XU64*)src))+((*((XU64*)dst))*a1>>8))&_MASK64;
		//((RGBIL*)dst)->c0 = (((RGBIL*)src)->c0 + (((RGBIL*)dst)->c0*a1>>8)) & 0x00FF00FF;
		//((RGBIL*)dst)->c1 = (((RGBIL*)src)->c1 + (((RGBIL*)dst)->c1*a1>>8)) & 0x00FF00FF;
		dst++;
	}
}

#else
inline void CompositeRGBPixel(RGBI* src, RGBI* dst)
{
	switch(src->alpha)
	{
	case 0xff:
		((RGBIL*)dst)->c0=((RGBIL*)src)->c0;
		((RGBIL*)dst)->c1=((RGBIL*)src)->c1;
		break;
	case 0:
		 break;
	default:
		{
		int a1 = 256-src->alpha;
		((RGBIL*)dst)->c0 = (((RGBIL*)src)->c0 + ((((RGBIL*)dst)->c0/*-((RGBIL*)src)->c0*/)*a1>>8)) & 0x00FF00FF;
		((RGBIL*)dst)->c1 = (((RGBIL*)src)->c1 + ((((RGBIL*)dst)->c1/*-((RGBIL*)src)->c1*/)*a1>>8)) & 0x00FF00FF;
		}break;
	}
}

inline void CompositeRGB(RGBI* src, RGBI* dst, int n)
{
	while ( n-- > 0 ) {
		//int a1 = 256-src->alpha;
		//((RGBIL*)dst)->c0 = (((RGBIL*)src)->c0 + ((((RGBIL*)dst)->c0)*a1>>8)) & 0x00FF00FF;
		//((RGBIL*)dst)->c1 = (((RGBIL*)src)->c1 + ((((RGBIL*)dst)->c1)*a1>>8)) & 0x00FF00FF;
		switch(src->alpha)
		{
		case 0xff:
			((RGBIL*)dst)->c0=((RGBIL*)src)->c0;
			((RGBIL*)dst)->c1=((RGBIL*)src)->c1;
			break;
		case 0:
			 break;
		default:
			{
			int a1 = 256-src->alpha;
			((RGBIL*)dst)->c0 = (((RGBIL*)src)->c0 + ((((RGBIL*)dst)->c0/*-((RGBIL*)src)->c0*/)*a1>>8)) & 0x00FF00FF;
			((RGBIL*)dst)->c1 = (((RGBIL*)src)->c1 + ((((RGBIL*)dst)->c1/*-((RGBIL*)src)->c1*/)*a1>>8)) & 0x00FF00FF;
			}break;
		}
		src++; dst++;
	}
}

inline void CompositeRGBSolid(RGBI* src, RGBI* dst, int n)
{

	while ( n-- > 0 ) {
		//int a1 = 256-src->alpha;
		//((RGBIL*)dst)->c0 = (((RGBIL*)src)->c0 + ((((RGBIL*)dst)->c0)*a1>>8)) & 0x00FF00FF;
		//((RGBIL*)dst)->c1 = (((RGBIL*)src)->c1 + ((((RGBIL*)dst)->c1)*a1>>8)) & 0x00FF00FF;
		switch(src->alpha)
		{
		case 0xff:
			((RGBIL*)dst)->c0=((RGBIL*)src)->c0;
			((RGBIL*)dst)->c1=((RGBIL*)src)->c1;
			break;
		case 0:
			 break;
		default:
			{
			register int a1 = 256-src->alpha;
			((RGBIL*)dst)->c0 = (((RGBIL*)src)->c0 + ((((RGBIL*)dst)->c0/*-((RGBIL*)src)->c0*/)*a1>>8)) & 0x00FF00FF;
			((RGBIL*)dst)->c1 = (((RGBIL*)src)->c1 + ((((RGBIL*)dst)->c1/*-((RGBIL*)src)->c1*/)*a1>>8)) & 0x00FF00FF;
			}break;
		}
		dst++;
	}
}
#endif


/*inline void PreBitmapAlpha(RGB8* src,int n)
{
	for(int i=0;i<n;i++)
	{
		switch(src[i].alpha)
		{
		case 0:
			src[i].red=0;
			src[i].blue=0;
			src[i].green=0;
			break;
		case 0xff:break;
		default:

		}
	}
}*/

inline void CompositeRGBPixel8(RGB8* src, RGB8* dst)
{
	switch(src->alpha)
	{
	case 0xff:
		//if(src->alpha==0xff)
		*dst=*src;
		break;
	case 0:break;
	default:
		{
			register int a1 = 256-src->alpha;
			dst->alpha=(src->alpha+(dst->alpha*a1>>8))&0xff;
			dst->red=(src->red+((dst->red)*a1>>8))&0xff;
			dst->green=(src->green+((dst->green)*a1>>8))&0xff;
			dst->blue=(src->blue+((dst->blue)*a1>>8))&0xff;
			//*((XU64*)dst)=((*((XU64*)src))+((*((XU64*)dst))*a1>>8))&_MASK64;
		}break;
	}
	//((RGBIL*)dst)->c0 = (((RGBIL*)src)->c0 + (((RGBIL*)dst)->c0*a1>>8)) & 0x00FF00FF;
	//((RGBIL*)dst)->c1 = (((RGBIL*)src)->c1 + (((RGBIL*)dst)->c1*a1>>8)) & 0x00FF00FF;
}

inline void CompositeRGB8(RGB8* src, RGB8* dst, int n)
{
	//return;
	while ( n-- > 0 ) {
#ifdef _TABCOMPOSIT_
		register int a1=255-src->alpha;
		switch(a1)
		{
		case 0:
			*dst=*src;break;
		case 0xff:break;
		default:
			XU8* data=_compositeTab+(a1<<8);
			dst->alpha=src->alpha+data[dst->alpha];
			dst->red=src->red+data[dst->red];
			dst->green=src->green+data[dst->green];
			dst->blue=src->blue+data[dst->blue];
			break;
		}
#else
		switch(src->alpha)
		{

		case 0xff:
				*dst=*src;break;
		case 0:break;
		default:
			{	register int a1 = 256-src->alpha;
				//int a0 = src->alpha;
				dst->alpha=(src->alpha+(dst->alpha*a1>>8))&0xff;

				dst->red=(src->red+((dst->red/*-src->red*/)*a1>>8))&0xff;
				dst->green=(src->green+((dst->green/*-src->green*/)*a1>>8))&0xff;
				dst->blue=(src->blue+((dst->blue/*-src->blue*/)*a1>>8))&0xff;

				//dst->red=((src->red*a0>>8)+(dst->red*a1>>8))&0xff;
				//dst->green=((src->green*a0>>8)+(dst->green*a1>>8))&0xff;
				//dst->blue=((src->blue*a0>>8)+(dst->blue*a1>>8))&0xff;
			}break;
		}
#endif
		src++; dst++;
	}
}

inline void CompositeRGB8Bitmap(RGB8* src, RGB8* dst, int n)
{

	while ( n-- > 0 ) {
		switch(src->alpha)
		{

		case 0xff:
				*dst=*src;break;
		case 0:break;
		default:
			{	int a1 = 256-src->alpha;
				//int a0 = src->alpha;
				dst->alpha=(src->alpha+(dst->alpha*a1>>8))&0xff;

				dst->red=(src->red+((dst->red-src->red)*a1>>8))&0xff;
				dst->green=(src->green+((dst->green-src->green)*a1>>8))&0xff;
				dst->blue=(src->blue+((dst->blue-src->blue)*a1>>8))&0xff;

				//dst->red=((src->red*a0>>8)+(dst->red*a1>>8))&0xff;
				//dst->green=((src->green*a0>>8)+(dst->green*a1>>8))&0xff;
				//dst->blue=((src->blue*a0>>8)+(dst->blue*a1>>8))&0xff;
			}break;
		}
		src++; dst++;
	}
}

inline void CompositeRGBSolid8(RGB8* src, RGB8* dst, int n)
{
	switch(src->alpha)
	{
	case 0xff:
		while(n-- >0)
		{
			*dst=*src;
			dst++;
		}break;
	case 0:break;
	default:
		while ( n-- > 0 ) {
			int a1 = 256-src->alpha;
			dst->alpha=(src->alpha+(dst->alpha*a1>>8))&0xff;
			dst->red=(src->red+(dst->red*a1>>8))&0xff;
			dst->green=(src->green+(dst->green*a1>>8))&0xff;
			dst->blue=(src->blue+(dst->blue*a1>>8))&0xff;
			dst++;
		}break;
	}
}

//
// Raster Inline functions
//

inline void XRaster::IndexToRGB(int index, RGBI* color)
// Find the color that corresponds to an index into the color table
{
	FLASHASSERT(cinfo && index < cinfo->ctab.n);
	ExpandColor(cinfo->ctab.colors[index], color);
}

inline int XRaster::RGBToIndex(RGBI* color)
// Search the inverse table for an entry that most closely matches the color
{
	FLASHASSERT( cinfo );
	FLASHASSERT(cinfo->itab);
	return cinfo->itab[CalcITabIndexRGB(color)];
}

inline void XRaster::DrawRGBAPixel(S32 x, RGB8* rgb)
{
	if ( rgb->alpha == 255 ) {
		drawRGBSlab(this, x, x+1, rgb);
	} else {
#ifdef _DIRECTDRAW_
		RGB8*pix=(RGB8*)_ROWADDR(this,x);
		//doGetBackground(this, x, x+1, pix);
		CompositeRGBPixel8(rgb,pix);
#else
		RGB8 pix;
		doGetBackground(this, x, x+1, &pix);
		CompositeRGBPixel8(rgb, &pix);
		drawRGBSlab(this, x, x+1, &pix);
#endif
	}
}

inline void XRaster::DrawRGBASlab(S32 xleft, S32 xright, RGB8* rgb)
{
	FLASHASSERT(xright - xleft <= RGBSlabChunkSize);
#ifdef _DIRECTDRAW_
	RGB8*pixBuf=(RGB8*)_ROWADDR(this,xleft);
	//doGetBackground(this,xleft,xright,pixBuf);
	CompositeRGB8(rgb,pixBuf,xright-xleft);
#else
	RGB8 pixBuf[RGBSlabChunkSize];
	doGetBackground(this, xleft, xright, pixBuf);
	CompositeRGB8(rgb, pixBuf, xright - xleft);
	drawRGBSlab(this, xleft, xright, pixBuf);
#endif
}

void CalcSolidPixel(_XRColor* color, S32 x, RGBI* pix)
{
	*pix = color->rgb;
}



void _XRColor::SetUp(XRaster* r)
{
	raster = r;
	nextColor = nextActive = 0;
	visible = 0;
#ifdef _MultiThread_
	memset(visibleThread,0,sizeof(visibleThread));
#endif
	transparent = false;
	colorType = colorSolid;
	cacheValid = false;
	clippedBy = 0;
	nFilterColor = 0;
	//priority = 0;
	order = 0;
	copyColor = 0;
	//color = c;
	//rgb.red = green = blue = 0;
	//pattern = 0;
}

void _XRColor::FreeCache()
{
	if ( cacheValid ) {
		cacheValid = false;
		switch ( colorType ) {
			case colorBitmap:
				if(bm.bitmap)
					bm.bitmap->UnlockBits();
				//bm.bitmap->DecRef(true);
				if ( bm.colorMap ) {
					raster->FreeMap(bm.colorMap);
					bm.colorMap = 0;
				}
				break;
			case colorGradient:
				if ( grad.colorRamp ) {
					raster->FreeRamp(grad.colorRamp);
					grad.colorRamp = 0;
				}
				break;
		}
	}
}


//
// Helpers for building dither patterns
//

inline int PinColor(int c)
{
	if ( c < 0 ) c = 0;
	if ( c > 255 ) c = 255;
	return c;
}

/*void ExpandPat(int src, int xPatAlign, U32 px1, U32 px2, int pixelFormat, int rowLongs, U32* dst)
{
	for ( int y = 0; y < 4; y++, dst += rowLongs ) {
		U32* d = dst;

		int s = (src >> (3-y)*4) & 0xF;	// get the 4 bits for this row from the pattern word

		// Align the pattern on the x axis
		s <<= xPatAlign&3;
		s = (s | (s >> 4)) & 0xF;	// handle the wrap around for alignment

		switch ( pixelFormat ) {


			case pixFmt8: {
				// the pat expands to 4 byte rows
				U8* db = (U8*)d;
				for ( int x = 4; x--; db++, s <<= 1 )
					*db = (U8)(s & 0x8 ? px1 : px2);
			} break;

			case pixFmt16:
				{
				// the pat expands to 8 byte rows
				U16* dw = (U16*)d;
				for ( int x = 4; x--; dw++, s <<= 1 )
					*dw = (U16)(s & 0x8 ? px1 : px2);
			} break;

			case pixFmt24: {
				// the pat expands to 12 byte rows
				U8* db = (U8*)d;
				for ( int x = 4; x--; db+=3, s <<= 1 ) {
					U32 px = (s & 0x8 ? px1 : px2);
					db[2] = (U8)(px >> 16);	// red
					db[1] = (U8)(px >> 8);	// green
					db[0] = (U8)(px);		// blue
				}
			} break;

			case pixFmt32:
			case pixFmt32A: {
				// the pat expands to 16 byte rows
				for ( int x = 4; x--; d++, s <<= 1 )
					*d = (s & 0x8 ? px1 : px2);
			} break;
		}
	}
}*/

static const DrawSlabProc solidSlabProcTable[] = {
//		DrawSolidSlab1,
//		DrawSolidSlab2,
//		DrawSolidSlab4,
		0,//DrawSolidSlab8,
		0,//DrawSolidSlab16,
//		DrawSolidSlab16,
		0,//DrawSolidSlab24,
		DrawSolidSlab32,
		DrawSolidSlab32
	};


static const BltProc FastBltProcs32[] = {	// index by dest pixel format
		0,	//bm1Bit
		0,	//bm2Bit
		0,	//bm4Bit
		(BltProc)Blt8to32,	//bm8Bit
		(BltProc)Blt16to32,	//bm16Bit
		(BltProc)Blt32to32	//bm32Bit
	};

static const BltProc* FastBltProcs[] = {	// index by source bitmap format
//		0,		// pix1
//		0,		// pix2
//		0,		// pix4
		0,//FastBltProcs8D,
		0,//FastBltProcs16,
//		FastBltProcs16A,		// pix16A
		0,		// pixFmt24
		FastBltProcs32,
		FastBltProcs32,//0		// pixFmt32A
	};

static const BltProc GeneralBltProcs[] = {	// index by bitmap format
		(BltProc)BltXto,
		(BltProc)BltXto,
		(BltProc)BltXto,
		(BltProc)Blt8to32,	//bm8Bit
		(BltProc)Blt16to32,	//bm16Bit
		(BltProc)Blt32to32	//bm32Bit
	};

static const BltProcI GeneralBltProcsI[] = {	// index by bitmap format
		(BltProcI)BltXto,
		(BltProcI)BltXto,
		(BltProcI)BltXto,
		Blt8toI,	//bm8Bit
		Blt16toI,	//bm16Bit
		Blt32toI	//bm32Bit
	};

#ifdef SMOOTHBITS
static const BltProc SmoothBltProcs[] = {	// index by bitmap format
		(BltProc)BltXto,
		(BltProc)BltXto,
		(BltProc)BltXto,
		(BltProc)Blt8to32S,
		(BltProc)Blt16to32S,
		(BltProc)Blt32to32S
	};

static const BltProc SmoothBltProcsA[] = {	// index by bitmap format
		(BltProc)BltXto,
		(BltProc)BltXto,
		(BltProc)BltXto,
		(BltProc)Blt8to32SA,
		(BltProc)Blt16to32SA,
		(BltProc)Blt32to32SA
	};

static const BltProcI SmoothBltProcsI[] = {	// index by bitmap format
		(BltProcI)BltXto,
		(BltProcI)BltXto,
		(BltProcI)BltXto,
		Blt8toIS,
		Blt16toIS,
		Blt32toIS
	};

static const BltProcI SmoothBltProcsAI[] = {	// index by bitmap format
		(BltProcI)BltXto,
		(BltProcI)BltXto,
		(BltProcI)BltXto,
		Blt8toISA,
		Blt16toISA,
		Blt32toISA
	};

#endif


//
// Color Transform helpers for bitmaps
//

void BuildMapChannel(int a, int b, U8* map)
{
	S32 c = (S32)b << 8;
	int n = 256;
	while ( n-- ) {
		if ( (c & 0xFFFF0000) == 0 )
			*map++ = (U8)(c>>8);
		else if ( c > 0 )
			*map++ = 255;
		else
			*map++ = 0;
		c += a;
	}
}

// Applying a color transform to bitmap values with alpha is messy...
// 1) Do an inverse pre-multiply on the the pixel
// 2) Apply the color transform
// 3) Do the pre-multiply on the transformed pixel
void ApplyColorMap(_XRColorMap* map, RGBI* c)
{
	FLASHASSERT(map);
	FLASHASSERT(c->red <= c->alpha && c->green <= c->alpha && c->blue <= c->alpha);
	if ( map->hasAlpha ) {
		FLASHASSERT(c->red <= c->alpha && c->green <= c->alpha && c->blue <= c->alpha);
		int a = map->alpha[c->alpha];
		if ( a == 0 ) {
			c->alpha = c->red = c->green = c->blue = 0;
		} else {
			int k = (256*255L)/c->alpha;
			if(k>256) k=256;
			c->alpha = a;
			a++;
			c->red   = a*map->red  [k*c->red  >>8] >> 8;
			c->green = a*map->green[k*c->green>>8] >> 8;
			c->blue  = a*map->blue [k*c->blue >>8] >> 8;
			FLASHASSERT((unsigned)c->red < 256 && (unsigned)c->green < 256 && (unsigned)c->blue < 256);
		}
	} else {
		c->red   = map->red  [c->red  ];
		c->green = map->green[c->green];
		c->blue  = map->blue [c->blue ];
		c->alpha = map->alpha[c->alpha];
	}
}

void ApplyColorMap8(_XRColorMap* map, RGB8* c)
{
	FLASHASSERT(map);
	FLASHASSERT(c->red <= c->alpha && c->green <= c->alpha && c->blue <= c->alpha);
	if ( map->hasAlpha ) {
		FLASHASSERT(c->red <= c->alpha && c->green <= c->alpha && c->blue <= c->alpha);
		int a = map->alpha[c->alpha];
		if ( a == 0 ) {
			c->alpha = c->red = c->green = c->blue = 0;
		} else {
			int k = (256*255L)/c->alpha;
			if(k>255) k=255;
			c->alpha = a;
			a++;
			c->red   = a*map->red  [k*c->red  >>8] >> 8;
			c->green = a*map->green[k*c->green>>8] >> 8;
			c->blue  = a*map->blue [k*c->blue >>8] >> 8;
			FLASHASSERT((unsigned)c->red < 256 && (unsigned)c->green < 256 && (unsigned)c->blue < 256);
		}
	} else {
		c->red   = map->red  [c->red  ];
		c->green = map->green[c->green];
		c->blue  = map->blue [c->blue ];
		c->alpha = map->alpha[c->alpha];
	}
}

void ApplyColorMap(_XRColorMap* map, RGB8* c, int n)
{
	FLASHASSERT(map);
	if ( map->hasAlpha ) {
		while ( n-- ) {
			FLASHASSERT(c->red <= c->alpha && c->green <= c->alpha && c->blue <= c->alpha);
			int a = map->alpha[c->alpha];
			if ( a == 0 ) {
				c->alpha = c->red = c->green = c->blue = 0;
			} else {
				int k = (256*255L)/c->alpha;
				if(k>255) k=255;
				c->alpha = a;
				a++;
				FLASHASSERT((k*c->red>>8) < 256 && (k*c->green>>8) < 256 && (k*c->blue>>8) < 256);
				c->red   = a*map->red  [k*c->red  >>8] >> 8;
				c->green = a*map->green[k*c->green>>8] >> 8;
				c->blue  = a*map->blue [k*c->blue >>8] >> 8;
				FLASHASSERT((unsigned)c->red < 256 && (unsigned)c->green < 256 && (unsigned)c->blue < 256);
			}
			c++;
		}
	} else {
		while ( n-- ) {
			c->red   = map->red  [c->red  ];
			c->green = map->green[c->green];
			c->blue  = map->blue [c->blue ];
			c->alpha = map->alpha[c->alpha];
			c++;
		}
	}
}

void _XRColor::SetBitmapDiv()
{
	if(!bm.bitmap) return;
	if(colorType!=colorBitmap||!bm.bitmap->nImgDiv) return;
	MATRIX&mat=bm.savedMat;
	MATRIX dm;
	::MatrixIdentity(&dm);
	dm.a<<=bm.bitmap->nImgDiv;
	dm.d<<=bm.bitmap->nImgDiv;
	::MatrixConcat(&dm,&mat,&mat);
}


void CalcBitmapPixel(_XRColor* color, S32 x, RGBI* buf)
{
	// Calc the start point in bitmap src coordinates
	//return;
	if(!color->bm.bi.baseAddr) return;
	if(!color->bm.bitmap) return;
	buf->alpha=0;
	SPOINT pt;
	pt.x = (x <<XBITMAPOFF)+HALFOFF;
	pt.y = color->raster->bitY <<XBITMAPOFF;
	//if(!color->gridBy0)
	MatrixTransformPoint(&color->bm.invMat, &pt, &pt);




	/*if(color->gridBy0)
	{
		//bool bSetX=false,bSetY=false;
		SRECT*gridBy=&color->bm.gridBy;
		int y=color->raster->bitY;

		if(y==167)
			int v=0;

		SRECT&lr=color->bm.gridRect;
		int lb=gridBy->ymax;//color->bm.bi.height-
		//if(sy<color->gridBy->ymin)
		if(color->bm.savedMat.d>0)
		{
			if(y<lr.ymin+gridBy->ymin)
			{
				pt.y=(y-lr.ymin)<<XBITMAPOFF;
			}
			else if(y>lr.ymax-lb)
			{
				pt.y=(color->bm.orgHeigt+(y-lr.ymax)+color->bm.offY)<<XBITMAPOFF;
			}
			else
			{
				 //pt.y=(pt.y-color->bm.scaleMat.ty)>>XBITMAPOFF;
				 //pt.y=pt.y*color->bm.scaleMat.d+color->bm.scaleMat.ty;
			}
		}
		else
		{
			if(y<lr.ymin+gridBy->ymin)
			{
				pt.y=(color->bm.orgHeigt-(y-lr.ymin)+color->bm.offY)<<XBITMAPOFF;
			}
			else if(y>lr.ymax-lb)
			{
				pt.y=((lr.ymax-y))<<XBITMAPOFF;
			}
			else
			{
				 //pt.y=(pt.y-color->bm.scaleMat.ty)>>XBITMAPOFF;
				 //pt.y=pt.y*color->bm.scaleMat.d+color->bm.scaleMat.ty;
			}
		}

		if(color->bm.savedMat.a>0)
		{
			int rr=//color->bm.bi.width
				gridBy->xmax;

			if(x<lr.xmin+gridBy->xmin)
			{
				pt.x=(x-lr.xmin)<<XBITMAPOFF;
			}
			else if(x>lr.xmax-rr)
			{
				pt.x= (color->bm.orgWidth-(lr.xmax-x))<<XBITMAPOFF;
			}
			else
			{
				//pt.x=(pt.x-color->bm.scaleMat.tx)>>XBITMAPOFF;
				//pt.x=pt.x*color->bm.scaleMat.a+color->bm.scaleMat.tx;
			}
		}
		else
		{
			int rr=//color->bm.bi.width
				gridBy->xmax;

			if(x<lr.xmin+gridBy->xmin)
			{
				pt.x=(color->bm.orgWidth-x+lr.xmin)<<XBITMAPOFF;
			}
			else if(x>lr.xmax-rr)
			{
				pt.x= ((lr.xmax-x))<<XBITMAPOFF;
			}
			else
			{
				//pt.x=(pt.x-color->bm.scaleMat.tx)>>XBITMAPOFF;
				//pt.x=pt.x*color->bm.scaleMat.a+color->bm.scaleMat.tx;
			}
		}

	}*/

	//if(xmin>=xmax) return;


	// Set up the slab start point
	if ( color->bm.bitsStyle & fillBitsClip ) {
		// Let GetRGBPixel handle the edge conditions
#ifndef _REFECT_
		#ifdef SMOOTHBITS
		if ( color->bm.smooth )
			color->bm.bitmap->GetSSRGBPixel(pt.x, pt.y, buf);
		else
		#endif
			color->bm.bitmap->GetRGBPixel(pt.x>>XBITMAPOFF, pt.y>>XBITMAPOFF, buf);
#else
		int wLimit=color->bm.bi.width<<XBITMAPOFF;
		int hLimit=color->bm.bi.height<<XBITMAPOFF;
		pt.x=Refect(pt.x,wLimit);
		pt.y=Refect(pt.y,hLimit);
		color->bm.bitmap->GetRGBPixel(pt.x>>XBITMAPOFF, pt.y>>XBITMAPOFF, buf);
#endif
		//color->bm.bltProcI(color->bm.bi,pt,1,buf
	} else {
		// Do an initial coordinate wrap for textures

		pt.x = LimitAbs(pt.x, (S32)color->bm.bi.width<<XBITMAPOFF);
		pt.y = LimitAbs(pt.y, (S32)color->bm.bi.height<<XBITMAPOFF);
		//RGB8 rgb;
		color->bm.bltProcI(color->bm.bi, pt, 1, buf);//&rgb);
		//ExpandColor(rgb,buf);
	}

	// Apply a color transform
	if ( color->bm.colorMap )
		ApplyColorMap(color->bm.colorMap, buf);

	// Set Bitmap Alpha
	if(!color->bm.bitmap->preAlpha)
	{
		switch(buf->alpha)
		{
		case 0:
			buf->blue=0;
			buf->green=0;
			buf->red=0;break;
		case 0xff:break;
		default:
			buf->red=buf->red*buf->alpha>>8;
			buf->green=buf->green*buf->alpha>>8;
			buf->blue=buf->blue*buf->blue>>8;
			break;
		}
	}

}

void DrawBitmapSlabGrid(_XRColor* color, S32 xmin, S32 xmax)
{
	if(!color->bm.bi.baseAddr) return;
	if(!color->bm.bitmap) return;

	XRaster* r = color->raster;

	// Calc the start point in bitmap src coordinates
	SPOINT pt;
	int y=r->bitY;
	//if(y>=274)
	//	int v=0;
	pt.x = (xmin << XBITMAPOFF)+HALFOFF;
	pt.y = y << XBITMAPOFF;
	MatrixTransformPoint(&color->bm.orgMat, &pt, &pt);

	SRECT*gridBy=&color->bm.gridBy;
	//if(color->gridBy)
	{
		SRECT&lr=color->bm.gridRect;
		SRECT&org=color->bm.orgRect;
		//bool   bSetY=false;

		if(y<lr.ymin+gridBy->ymin)
		{
			pt.y=(y-lr.ymin+org.ymin)<<XBITMAPOFF;
		}
		else if(y>=lr.ymax-gridBy->ymax)
		{
			pt.y=(org.ymax+(y-lr.ymax)+1)<<XBITMAPOFF;
		}
		int n=lr.xmin+gridBy->xmin-xmin;

		if(n>0)
		{
			if(n>xmax-xmin) n=xmax-xmin;

			while(n)
			{
				SPOINT ptt={((xmin-lr.xmin+org.xmin)<<XBITMAPOFF)+HALFOFF,pt.y};
				MatrixTransformPoint(&color->bm.gridMat,&ptt,&ptt);
#ifdef _DIRECTDRAW_
				RGB8*pixBuf=(RGB8*)_ROWADDR(r,xmin);
				
				int l=XMIN(n,RGBSlabChunkSize);
				color->bm.bitmap->GetData(ptt.x>>XBITMAPOFF,ptt.y>>XBITMAPOFF,pixBuf,l,color->bm.gridMat.a,color->bm.gridMat.b);
				if(color->bm.colorMap)
					ApplyColorMap(color->bm.colorMap,pixBuf,l);
#else
				RGB8 pixBuf[RGBSlabChunkSize];
				
				int l=XMIN(n,RGBSlabChunkSize);
				color->bm.bitmap->GetData(ptt.x>>XBITMAPOFF,ptt.y>>XBITMAPOFF,pixBuf,l,color->bm.gridMat.a,color->bm.gridMat.b);
				if(color->bm.colorMap)
					ApplyColorMap(color->bm.colorMap,pixBuf,l);
				r->drawRGBSlab(r, xmin, xmin+l, pixBuf);
#endif
				xmin+=l;
				n-=l;
				//buf+=n;
			}
			//color->bm.bitmap->GetData(ptt.x>>XBITMAPOFF,ptt.y>>XBITMAPOFF,buf,n,color->bm.gridMat.a,color->bm.gridMat.b);
			//if(color->bm.colorMap)
			//	ApplyColorMap(color->bm.colorMap, buf, n);
			pt.x=((gridBy->xmin+org.xmin)<<XBITMAPOFF)+HALFOFF;
			//buf+=n;
			//xmin+=n;
		}
		n=xmax-(lr.xmax-gridBy->xmax);

		if(n>0)
		{
			if(n>xmax-xmin)
				n=xmax-xmin;
			while(n)
			{
				
				int l=XMIN(n,RGBSlabChunkSize);
				SPOINT ptt={((org.xmax+xmax-l-lr.xmax+1)<<XBITMAPOFF)+HALFOFF,pt.y};
				MatrixTransformPoint(&color->bm.gridMat,&ptt,&ptt);
#ifdef _DIRECTDRAW_
				RGB8*pixBuf=(RGB8*)_ROWADDR(r,xmax-l);
				color->bm.bitmap->GetData(ptt.x>>XBITMAPOFF,ptt.y>>XBITMAPOFF,pixBuf,l,color->bm.gridMat.a,color->bm.gridMat.b);				
				if(color->bm.colorMap)
					ApplyColorMap(color->bm.colorMap, pixBuf, l);
#else
				RGB8 pixBuf[RGBSlabChunkSize];
				color->bm.bitmap->GetData(ptt.x>>XBITMAPOFF,ptt.y>>XBITMAPOFF,pixBuf,l,color->bm.gridMat.a,color->bm.gridMat.b);
				//color->bm.bitmap->GetData(ptt.x>>XBITMAPOFF,ptt.y>>XBITMAPOFF,buf+xmax-xmin-n,n,color->bm.gridMat.a,color->bm.gridMat.b);
				if(color->bm.colorMap)
					ApplyColorMap(color->bm.colorMap, pixBuf, l);
				r->drawRGBSlab(r, xmax-l, xmax, pixBuf);
#endif
				xmax-=l;
				n-=l;
			}
		}

	}

	if(xmin>=xmax) return;

	MatrixTransformPoint(&color->bm.gridMat,&pt,&pt);
	//pt.x+=fixed_1/2;
	//pt.y+=fixed_1/2;

	//pt.x=//(pt.x>>XBITMAPOFF)-gridBy->xmin;
	//	 (pt.x-color->bm.scaleMat.tx)>>XBITMAPOFF;
	//pt.x=pt.x*color->bm.scaleMat.a+color->bm.scaleMat.tx;

	// Set up the slab start point
	S32 wLimit = (S32)color->bm.bi.width<<XBITMAPOFF;
	S32 hLimit = (S32)color->bm.bi.height<<XBITMAPOFF;
	if ( color->bm.bitsStyle & fillBitsClip ) {
		// Treat the edges of a cliped bitmap special to prevent rounding problems
		// Check for out of range values
#ifndef _REFECT_
		RGB8 pix;
		int x,y;

		//pt.x=Refect(pt.x,wLimit);
		//pt.y=Refect(pt.y,hLimit);

		SBitmapCore* bitmap = color->bm.bitmap;
		int w = bitmap->width;
		int h = bitmap->height;
		if ( color->bm.smooth ) {
			h--; w--;		// allow for the extra row and column when getting 4 samples
		}

		SPOINT tail;
		S32 count = xmax-xmin;
		tail.x = pt.x + color->bm.bi.dx*count;
		tail.y = pt.y + color->bm.bi.dy*count;

		// Adjust the head
		for (;;) {
			x = (int)(pt.x>>XBITMAPOFF);
			y = (int)(pt.y>>XBITMAPOFF);
			if ( x >= 0 && y >= 0 && x < w && y < h ) break;
			if ( xmin >= xmax ) break;
			#ifdef SMOOTHBITS
			if ( color->bm.smooth )
				bitmap->GetSSRGBPixel8(pt.x, pt.y, &pix);
			else
			#endif
				bitmap->GetRGBPixel8(x, y, &pix);

			if ( color->bm.colorMap )
				ApplyColorMap8(color->bm.colorMap, &pix);
			r->drawRGBSlab(r, xmin, xmin+1, &pix);

			pt.x += color->bm.bi.dx;
			pt.y += color->bm.bi.dy;
			xmin++;
		}

		// Adjust the tail
		for (;;) {
			x = (int)(tail.x>>XBITMAPOFF);
			y = (int)(tail.y>>XBITMAPOFF);
			if ( x >= 0 && y >= 0 && x < w && y < h ) break;
			if ( xmin >= xmax ) break;
			#ifdef SMOOTHBITS
			if ( color->bm.smooth )
				bitmap->GetSSRGBPixel8(tail.x, tail.y, &pix);
			else
			#endif
				bitmap->GetRGBPixel8(x, y, &pix);
			xmax--;
			if ( color->bm.colorMap )
				ApplyColorMap8(color->bm.colorMap, &pix);
			r->drawRGBSlab(r, xmax, xmax+1, &pix);

			tail.x -= color->bm.bi.dx;
			tail.y -= color->bm.bi.dy;
		}
#else
		pt.y=Refect(pt.y,hLimit);


		while ( xmin < xmax ) {
			RGB8 pixBuf[RGBSlabChunkSize];

			// Expand the pixels to RGB values
			int n = Min((int)(xmax-xmin), RGBSlabChunkSize);
			CalcLimitRefect(n, pt.x, color->bm.bi.dx, wLimit);	// limit the slab size so we don't overwrap textures
			CalcLimitRefect(n, pt.y, color->bm.bi.dy, hLimit);

			color->bm.bltProc(color->bm.bi, pt, n, pixBuf);

			// Apply a color transform
			if ( color->bm.colorMap )
				ApplyColorMap(color->bm.colorMap, pixBuf, n);

			// Apply a highlight pattern...
			//if ( color->pattern )
			//	ApplyRGBPattern(color->pattern, r->bitY, xmin, xmin+n, pixBuf);

			// Draw the RGB values
			r->drawRGBSlab(r, xmin, xmin+n, pixBuf);
			xmin += n;
		}
		return;
#endif
		//pt.x = LimitAbs(pt.x, wLimit);
		//pt.y = LimitAbs(pt.y, hLimit);
		//return;
		//return;
		//int di=-w;
		//if(pt.x>=w
		//pt.y=Refect(pt.y,hLimit);
		//if(pt.x<0) pt.x=0;
		//else if(pt.x>=wLimit) pt.x=wLimit-1;
		//pt.x = LimitAbs(pt.x, wLimit);

	} else {
		// Do an initial coordinate wrap for textures
		
		pt.x = LimitAbs(pt.x, wLimit);
		pt.y = LimitAbs(pt.y, hLimit);

		//pt.x = LimitAbs(pt.x, wLimit);
		//pt.y = LimitAbs(pt.y, hLimit);
	}

	if ( color->bm.fastBltProc ) {
		// Draw the slab for the fast case
// 		int pixSize = mapPixelFormatToDepth[r->pixelFormat]/8;
		int pixSize = pixelSize[r->pixelFormat];
			//NativeDisplayTester::PixelFormatToDepth( r->pixelFormat ) / 8;
		FLASHASSERT(pixSize > 0);

		//xmin += r->xorg;
		//xmax += r->xorg;

		while ( xmin < xmax ) {
			// Expand the pixels to RGB values
			int n = Min((int)(xmax-xmin), RGBSlabChunkSize);
			CalcLimit(n, pt.x, color->bm.bi.dx, wLimit);	// limit the slab size so we don't overwrap textures
			CalcLimit(n, pt.y, color->bm.bi.dy, hLimit);
			color->bm.fastBltProc(color->bm.bi, pt, n, r->rowAddr + xmin*pixSize);
			xmin += n;
		}
	} else {
		// Draw the slab for the general case
		while ( xmin < xmax ) {
			

			// Expand the pixels to RGB values
			int n = Min((int)(xmax-xmin), RGBSlabChunkSize);
			CalcLimit(n, pt.x, color->bm.bi.dx, wLimit);	// limit the slab size so we don't overwrap textures
			CalcLimit(n, pt.y, color->bm.bi.dy, hLimit);
#ifdef _DIRECTDRAW_
			RGB8* pixBuf=(RGB8*)_ROWADDR(r,xmin);
			color->bm.bltProc(color->bm.bi, pt, n, pixBuf);
			// Apply a color transform
			if ( color->bm.colorMap )
				ApplyColorMap(color->bm.colorMap, pixBuf, n);			
#else
			RGB8 pixBuf[RGBSlabChunkSize];
			color->bm.bltProc(color->bm.bi, pt, n, pixBuf);
			// Apply a color transform
			if ( color->bm.colorMap )
				ApplyColorMap(color->bm.colorMap, pixBuf, n);

			r->drawRGBSlab(r, xmin, xmin+n, pixBuf);
#endif
			xmin += n;
		}
	}
}

void CompositeBitmapSlabGrid(_XRColor* color, S32 xmin, S32 xmax, RGB8* buf)
{
	if(!color->bm.bi.baseAddr) return;
	if(!color->bm.bitmap) return;

	FLASHASSERT(xmax-xmin<=RGBSlabChunkSize);
	RGB8 pixBuf[RGBSlabChunkSize];
	memset(pixBuf,0,RGBSlabChunkSize*sizeof(RGB8));

	BuildBitmapSlabGrid(color, xmin, xmax, pixBuf);
	if(color->bm.bitmap->preAlpha)
		CompositeRGB8(pixBuf, buf, xmax-xmin);
	else
		CompositeRGB8Bitmap(pixBuf, buf, xmax-xmin);
}

void BuildBitmapSlabGrid(_XRColor* color, S32 xmin, S32 xmax, RGB8* buf)
{
	if(!color->bm.bi.baseAddr) return;
	if(!color->bm.bitmap) return;
	// Calc the start point in bitmap src coordinates
	//return;
	//if(color->bm.savedMat.a>0) return;
//	if(color->raster->bitY==170)
	//	int v=0; 
	SPOINT pt;
	int y=color->raster->bitY;




	pt.x = (xmin << XBITMAPOFF)+HALFOFF;
	pt.y = y << XBITMAPOFF;

	//if(y==25)
	//	int v=0;

	MatrixTransformPoint(&color->bm.orgMat, &pt, &pt);

	SRECT*gridBy=&color->bm.gridBy;
	//if(color->gridBy)
	{


		SRECT&lr=color->bm.gridRect;
		SRECT&org=color->bm.orgRect;
		//bool   bSetY=false;

		if(y<lr.ymin+gridBy->ymin)
		{
			pt.y=(y-lr.ymin+org.ymin)<<XBITMAPOFF;
		}
		else if(y>=lr.ymax-gridBy->ymax)
		{
			pt.y=(org.ymax+(y-lr.ymax)+1)<<XBITMAPOFF;
		}
		int n=lr.xmin+gridBy->xmin-xmin;

		if(n>0)
		{
			if(n>xmax-xmin) n=xmax-xmin;

			SPOINT ptt={((xmin-lr.xmin+org.xmin)<<XBITMAPOFF)+HALFOFF,pt.y};
			//TransformPoint(ptt,color->bm.gridMat);
			MatrixTransformPoint(&color->bm.gridMat,&ptt,&ptt);

			color->bm.bitmap->GetData(ptt.x>>XBITMAPOFF,ptt.y>>XBITMAPOFF,buf,n,color->bm.gridMat.a,color->bm.gridMat.b);
			if(color->bm.colorMap)
				ApplyColorMap(color->bm.colorMap, buf, n);
			pt.x=((gridBy->xmin+org.xmin)<<XBITMAPOFF)+HALFOFF;
			buf+=n;
			xmin+=n;
		}
		n=xmax-(lr.xmax-gridBy->xmax);

		if(n>0)
		{
			if(n>xmax-xmin)
				n=xmax-xmin;
			SPOINT ptt={((org.xmax+xmax-n-lr.xmax+1)<<XBITMAPOFF)+HALFOFF,pt.y};
			MatrixTransformPoint(&color->bm.gridMat,&ptt,&ptt);
			color->bm.bitmap->GetData(ptt.x>>XBITMAPOFF,ptt.y>>XBITMAPOFF,buf+xmax-xmin-n,n,color->bm.gridMat.a,color->bm.gridMat.b);
			if(color->bm.colorMap)
				ApplyColorMap(color->bm.colorMap, buf+xmax-xmin-n, n);
			xmax-=n;
		}
		/*int lb=gridBy->ymax;//color->bm.bi.height-
		//	lr.ymax-lr.ymin-gridBy->ymax;
		//if(sy<color->gridBy->ymin)
		if(color->bm.savedMat.d>0)
		{
			//int ddy=color->bm.savedMat.ty>>color->raster->antialias_b;
			if(y<lr.ymin+gridBy->ymin)
			{
				pt.y=(y-lr.ymin)<<XBITMAPOFF;
			}
			else if(y>lr.ymax-lb)
			{
				pt.y=(color->bm.orgHeigt+(y-lr.ymax)+color->bm.offY)<<XBITMAPOFF;
				//return;
			}
			else
			{
				//return;
				//pt.y=((pt.y-color->bm.scaleMat.ty)>>XBITMAPOFF)+color->bm.offY;
				//pt.y=pt.y*color->bm.scaleMat.d+color->bm.scaleMat.ty;
			}
		}
		else
		{
			if(y<lr.ymin+gridBy->ymin)
			{
				pt.y=(color->bm.orgHeigt-(y-lr.ymin)+color->bm.offY)<<XBITMAPOFF;
			}
			else if(y>lr.ymax-lb)
			{
				pt.y=((lr.ymax-y))<<XBITMAPOFF;
			}
			else
			{
				 //pt.y=(pt.y-color->bm.scaleMat.ty)>>XBITMAPOFF;
				 //pt.y=pt.y*color->bm.scaleMat.d+color->bm.scaleMat.ty;
			}
		}
		if(color->bm.savedMat.a>0)
		{
			y=pt.y>>XBITMAPOFF;
			int limity=color->bm.bi.height;
			if(y<0) y=0;
			else if(y>=limity) y=limity-1;

			int n=lr.xmin+gridBy->xmin-xmin;
			if(n>xmax-xmin)
				n=xmax-xmin;

			if(n>0)
			{
				int x=//color->bm.gridBy.xmin-n;
					xmin-color->bm.gridRect.xmin;

				if(n>0)
				{
					color->bm.bitmap->GetLineData(x,y,n+x,buf);
					if(color->bm.colorMap)
						ApplyColorMap(color->bm.colorMap, buf, n);
					xmin+=n;
					buf+=n;
					pt.x=gridBy->xmin<<XBITMAPOFF;
				}
			}

			int rr=//color->bm.bi.width
				gridBy->xmax;
				//lr.xmax-lr.xmin-gridBy->xmax;

			n=xmax-(lr.xmax-rr);
			if(n>xmax-xmin)
				n=xmax-xmin;
					//xmax-color->bm.gridRect.xmax+(color->bm.bi.width-color->gridBy->xmax+1);
			if(n>0)
			{
				int x= color->bm.orgWidth-(lr.xmax-xmax+n);
				RGB8*dst=buf+(xmax-xmin);

				{
					color->bm.bitmap->GetLineData(x,y,n+x,dst-n);
					if(color->bm.colorMap)
						ApplyColorMap(color->bm.colorMap, dst-n, n);
					xmax-=n;
				}
			}
		}
		else //if(0)
		{
			y=pt.y>>XBITMAPOFF;
			int limity=color->bm.bi.height;
			if(y<0) y=0;
			else if(y>=limity) y=limity-1;

			int n=lr.xmin+gridBy->xmin-xmin;
			if(n>xmax-xmin) n=xmax-xmin;

			if(n>0)
			{
				int x=color->bm.orgWidth-(color->bm.gridBy.xmin-n);
				if(x>=color->bm.bi.width)//color->bm.orgWidth)
				{
					n-=(x-color->bm.bi.width+1);
					x=color->bm.bi.width-1;

				}
				if(n>0)
				{
					if(x-n<0) x=n;
					//if(x<0)
					//	x=0;
					color->bm.bitmap->GetLineDataR(x,y,x-n,buf);
					if(color->bm.colorMap)
						ApplyColorMap(color->bm.colorMap, buf, n);
					xmin+=n;
					buf+=n;
					pt.x=gridBy->xmin<<XBITMAPOFF;
				}
			}

			int rr=//color->bm.bi.width
				gridBy->xmax;
				//lr.xmax-lr.xmin-gridBy->xmax;

			n=xmax-(lr.xmax-rr);
			if(n>xmax-xmin)
				n=xmax-xmin;
					//xmax-color->bm.gridRect.xmax+(color->bm.bi.width-color->gridBy->xmax+1);
			if(n>0)
			{
				int x=(lr.xmax-xmax+n);
				if(x>=color->bm.bi.width)//color->bm.orgWidth)
				{
					n-=(x-color->bm.bi.width+1);
					x=color->bm.bi.width-1;
					//n-=(x-color->bm.orgWidth+1);
					//x=color->bm.orgWidth-1;
				}
				if(n>0)
				{
					if(x-n<0) x=n;
						//(color->bm.orgWidth-rr);
						//gridBy->xmax;

					color->bm.bitmap->GetLineDataR(x,y,x-n,buf+(xmax-xmin)-n);
					if(color->bm.colorMap)
						ApplyColorMap(color->bm.colorMap, buf+(xmax-xmin)-n, n);
					xmax-=n;
				}
			}
		}*/

	}
	//return;
	if(xmin>=xmax) return;

	::MatrixTransformPoint(&color->bm.gridMat,&pt,&pt);
	//return;
	//pt.x=//(pt.x>>XBITMAPOFF)-gridBy->xmin;
	//	 (pt.x-color->bm.scaleMat.tx)>>XBITMAPOFF;
	//pt.x=pt.x*color->bm.scaleMat.a+color->bm.scaleMat.tx;


	// Set up the slab start point
	S32 wLimit = (S32)color->bm.bi.width<<XBITMAPOFF;
	S32 hLimit = (S32)color->bm.bi.height<<XBITMAPOFF;
	if ( color->bm.bitsStyle & fillBitsClip ) {
		// Treat the edges of a clipped bitmap special to prevent rounding problems
		// Check for out of range values
#ifndef _REFECT_
		int x,y;

		SBitmapCore* bitmap = color->bm.bitmap;
		int w = bitmap->width;
		int h = bitmap->height;
		if ( color->bm.smooth ) {
			h--; w--;		// allow for the extra row and column when getting 4 samples
		}

		SPOINT tail;
		S32 count = xmax-xmin;
		tail.x = pt.x + color->bm.bi.dx*count;
		tail.y = pt.y + color->bm.bi.dy*count;

		// Adjust the head
		for (;;) {
			x = (int)(pt.x>>XBITMAPOFF);
			y = (int)(pt.y>>XBITMAPOFF);
			if ( x >= 0 && y >= 0 && x < w && y < h ) break;
			if ( xmin >= xmax ) break;
			#ifdef SMOOTHBITS
			if ( color->bm.smooth )
				bitmap->GetSSRGBPixel8(pt.x, pt.y, buf);
			else
			#endif
				bitmap->GetRGBPixel8(x, y, buf);

			if ( color->bm.colorMap )
				ApplyColorMap8(color->bm.colorMap, buf);

			pt.x += color->bm.bi.dx;
			pt.y += color->bm.bi.dy;
			xmin++;
			buf++;
		}

		// Adjust the tail
		for (;;) {
			x = (int)(tail.x>>XBITMAPOFF);
			y = (int)(tail.y>>XBITMAPOFF);
			if ( x >= 0 && y >= 0 && x < w && y < h ) break;
			if ( xmin >= xmax ) break;
			RGB8* pix = buf+xmax-xmin-1;
			#ifdef SMOOTHBITS
			if ( color->bm.smooth )
				bitmap->GetSSRGBPixel8(tail.x, tail.y, pix);
			else
			#endif
				bitmap->GetRGBPixel8(x, y, pix);
			xmax--;
			if ( color->bm.colorMap )
				ApplyColorMap8(color->bm.colorMap, pix);

			tail.x -= color->bm.bi.dx;
			tail.y -= color->bm.bi.dy;
		}
#else
		pt.y=Refect(pt.y,hLimit);
		while ( xmin < xmax ) {
		// Expand the pixels to RGB values
		//int n = Min((int)(xmax-xmin), RGBSlabChunkSize);
			int n = (int)(xmax-xmin);
			CalcLimitRefect(n, pt.x, color->bm.bi.dx, wLimit);	// limit the slab size so we don't overwrap textures
			CalcLimitRefect(n, pt.y, color->bm.bi.dy, hLimit);

			color->bm.bltProc(color->bm.bi, pt, n, buf);

			// Apply a color transform
			if ( color->bm.colorMap )
				ApplyColorMap(color->bm.colorMap, buf, n);

			xmin += n;
			buf += n;
		}
		return;
#endif
	} else {
		// Do an initial coordinate wrap for textures
//#if (__CORE_VERSION__>=0x02075000)
		if(pt.x>=wLimit)
			pt.x=wLimit-1;
		if(pt.y>=hLimit)
			pt.y=hLimit-1;
		if(pt.x<0) pt.x=0;
		if(pt.y<0) pt.y=0;

	}

	// Draw the slab for the general case
	while ( xmin < xmax ) {
		// Expand the pixels to RGB values
		//int n = Min((int)(xmax-xmin), RGBSlabChunkSize);
		int n = (int)(xmax-xmin);
		CalcLimit(n, pt.x, color->bm.bi.dx, wLimit);	// limit the slab size so we don't overwrap textures
		CalcLimit(n, pt.y, color->bm.bi.dy, hLimit);

		color->bm.bltProc(color->bm.bi, pt, n, buf);

		// Apply a color transform
		if ( color->bm.colorMap )
			ApplyColorMap(color->bm.colorMap, buf, n);

		xmin += n;
		buf += n;
	}
}

#define SETMAT(MAT0,MAT1)\
{ if(MAT0){if(MAT0>0) MAT1=1;else MAT1=-1;}\
  else MAT1=0;}


void _XRColor::BuildCache()
{
	if ( cacheValid ) return;

	switch ( colorType ) {
		case colorSolid: {
			// Pick a pattern and a background RGB color
			drawSlabProc = solidSlabProcTable[raster->pixelFormat];
			compositeSlabProc = CompositeSolidSlab;

			pat=PackPix32(&rgb);
			/*RGBI bkColor;
			U16 patSrc;
			switch ( pattern ) {
		 	 	case solidPat:
					if ( raster->solidDither == errorDiffusionDither ) {
					 	drawSlabProc = DrawDitherSlab;
						return;
					}

					bkColor = rgb;
					patSrc = solidPatBits;
		 	 		break;

		 	 	case disablePat: {
					patSrc = disablePatBits;
					bkColor.red = bkColor.green = bkColor.blue = 255;
		 	 	} break;

		 	 	case highlightPat:
		 	 	case lineHighlightPat: {
					patSrc = pattern == lineHighlightPat ? linePatBits : fillPatBits;

					bkColor.red   = rgb.red ^ 0xFF;
					bkColor.green = rgb.green ^ 0xFF;
					bkColor.blue  = rgb.blue ^ 0xFF;

		 	 	} break;
			}

			// Pick two pixel values
			U32 px1, px2;
			switch ( raster->pixelFormat ) {
				case pixFmt8:
				{
					px1 = raster->RGBToIndex(&rgb);

					if ( pattern == solidPat ) {
						if ( raster->solidDither == noDither ) {
							px2 = px1;
						} else {
							// Implement a simple dither here...
							RGBI c1, c2;

							raster->IndexToRGB((int)px1, &c1);
							int redErr1   = c1.red   - rgb.red;
							int greenErr1 = c1.green - rgb.green;
							int blueErr1  = c1.blue  - rgb.blue;
							int err1 = Abs(redErr1) + Abs(greenErr1) + Abs(blueErr1);
							if ( err1 < 0x08 ) {
								px2 = px1;
							} else {
								c2.red   = PinColor(rgb.red   - redErr1);
								c2.green = PinColor(rgb.green - greenErr1);
								c2.blue  = PinColor(rgb.blue  - blueErr1);
								px2 = raster->RGBToIndex(&c2);
								raster->IndexToRGB((int)px2, &c2);
								int redErr2   = c2.red   - rgb.red;
								int greenErr2 = c2.green - rgb.green;
								int blueErr2  = c2.blue  - rgb.blue;

								// Pick a pattern based on the relative error
								int err2 = Abs(redErr2) + Abs(greenErr2) + Abs(blueErr2);
								int gray = (16*err1) / (err1 + err2);
								if ( gray > 15 ) gray = 15;
								patSrc = grayPatList[gray];
							}
						}

					} else {
						px2 = raster->RGBToIndex(&bkColor);
					}
				} break;

				case pixFmt16: {
					px1 = PackPix16(&rgb);
					px2 = PackPix16(&bkColor);
				} break;


				case pixFmt24:
				case pixFmt32: {
					px1 = PackPix32(&rgb);
					px2 = PackPix32(&bkColor);
				} break;

				case pixFmt32A: {
					px1 = PackPix32A(&rgb, 0xFF);
					px2 = PackPix32A(&bkColor, 0xFF);
				} break;
			}

			FLASHASSERT(raster->bits);
			ExpandPat(patSrc, (int)raster->patAlign.x & 0x3, px1, px2, raster->pixelFormat, 4, pat);*/
		} break;

		case colorBitmap: {
			if ( !bm.bitmap || !bm.bitmap->HasBits() ) {
				colorType = colorSolid;
			 	drawSlabProc = DrawDitherSlab;
				compositeSlabProc = CompositeSolidSlab;

			} else {
				drawSlabProc = DrawBitmapSlab;
				compositeSlabProc = transparent ? CompositeBitmapSlab : BuildBitmapSlab;

				::RectSetEmpty(&bm.gridRect);

				bm.bitmap->LockBits();	// the Mac puts bits in handles

				// Set up the blt info
				bm.bi.dx = bm.invMat.a;
				bm.bi.dy = bm.invMat.b;

				bm.bi.color = this;
				bm.bi.baseAddr = (U8*)bm.bitmap->baseAddr;
				bm.bi.rowBytes = bm.bitmap->rowBytes;
				bm.bi.width = bm.bitmap->width;
				bm.bi.height = bm.bitmap->height;
				bm.bi.colors = bm.bitmap->cTab ? bm.bitmap->cTab->colors : 0;
				bm.bi.itab = raster->cinfo ? raster->cinfo->itab : 0;

				if ( bm.cxform.HasTransform() ) {
					// build a color map
					_XRColorMap* cm = bm.colorMap = raster->CreateMap();
					if ( cm ) {
						cm->hasAlpha = bm.cxform.aa < 256 || bm.cxform.ab < 0 || transparent;
						BuildMapChannel(bm.cxform.ra, bm.cxform.rb, cm->red);
						BuildMapChannel(bm.cxform.ga, bm.cxform.gb, cm->green);
						BuildMapChannel(bm.cxform.ba, bm.cxform.bb, cm->blue);
						BuildMapChannel(bm.cxform.aa, bm.cxform.ab, cm->alpha);
						cm->alpha[0] = 0;	// this is required for the map transform
					}

				} else {
					bm.colorMap = 0;
#ifdef _LINEALPHA_
					if(transparent&&
						bm.bi.dy==0)
						bm.bitmap->BuildLineAlpha();
#endif
				}

				bm.fastBltProc = 0;

				#ifdef SMOOTHBITS
				if ( raster->smoothBitmaps && !(bm.bitsStyle&fillBitsNoSmooth) &&
							( ((Abs(bm.invMat.a)+0xF)&~0x1F) != fixed_1 ||
							  ((Abs(bm.invMat.b)+0xF)&~0x1F) != 0 ||
							  bm.invMat.b != 0 || bm.invMat.c != 0) ) {
					// Smooth the bitmap
					bm.smooth = true;

					if ( transparent )
					{
						bm.bltProc = SmoothBltProcsA[bm.bitmap->bmFormat];	// preserve the alpha
						bm.bltProcI= SmoothBltProcsAI[bm.bitmap->bmFormat];	// preserve the alpha
					}
					else
					{
						bm.bltProc = SmoothBltProcs[bm.bitmap->bmFormat];	// assume opaque alpha
						bm.bltProcI= SmoothBltProcsI[bm.bitmap->bmFormat];	// assume opaque alpha
					}
				} else
				#endif
				{// Don't smooth the bitmap, so check for fast blt procs
					bm.smooth = false;
					bm.bltProc = GeneralBltProcs[bm.bitmap->bmFormat];
					bm.bltProcI = GeneralBltProcsI[bm.bitmap->bmFormat];
					if ( !bm.colorMap &&
						 (raster->pixelFormat != pixFmt8 || raster->bitmapDither < errorDiffusionDither) ) {
						const BltProc* procs = FastBltProcs[raster->pixelFormat];
						if ( procs )
							bm.fastBltProc = procs[bm.bitmap->bmFormat];
					}
				}
			}
		} break;

		case colorGradient: {
			drawSlabProc = DrawGradientSlab;
			compositeSlabProc = transparent ?

								CompositeGradientSlab :
								BuildGradientSlab;

			// Create the color ramp
			RGB8* c = grad.colorRamp = raster->CreateRamp();
			if ( !c ) {
			 	drawSlabProc = DrawDitherSlab;
				compositeSlabProc = CompositeSolidSlab;
				return;
			}
			SRGB c1, c2;
			S32 cr1, cr2;
			S32 w1, w2, wT;

			cr1 = 0;
			cr2 = grad.ramp.colorRatio[0];
			c1 = c2 = grad.ramp.color[0];
			int ci = 1;

			for ( int i = 0; i <= 256; i++, c++ ) {
				if ( i > cr2 ) {
					cr1 = cr2;
					c1 = c2;
				 	if ( ci < grad.ramp.nColors ) {
						// Get the next color in the ramp
						cr2 = grad.ramp.colorRatio[ci];
						c2 = grad.ramp.color[ci];
						ci++;
					} else {
						// We went past the end
						cr2 = 256;
					}
				}
				w1 = cr2 - i;
				w2 = i - cr1;
				wT = w1+w2;
				if ( wT > 0 ) {
					// change div to fixed mul
					c->alpha = (U8)((w1*c1.rgb.transparency + w2*c2.rgb.transparency)/wT);
					c->red   = (U8)((w1*c1.rgb.red   + w2*c2.rgb.red  )/wT);
					c->green = (U8)((w1*c1.rgb.green + w2*c2.rgb.green)/wT);
					c->blue  = (U8)((w1*c1.rgb.blue  + w2*c2.rgb.blue )/wT);
				} else {
					c->alpha = c1.rgb.transparency;
					c->red   = c1.rgb.red;
					c->green = c1.rgb.green;
					c->blue  = c1.rgb.blue;
				}

				{// Pre-multiply the RGB value
					int a = c->alpha;
					if ( a < 255 ) {
						if ( a == 0 ) {
							c->red = c->green = c->blue = 0;
						} else {
							a++;
							c->red   = (c->red   * a)>>8;
							c->green = (c->green * a)>>8;
							c->blue  = (c->blue  * a)>>8;
						}
					}
				}
			}
		} break;
	}
	cacheValid = true;
	rgbPoint = 0x80008000L;
}


//
// The Edge structures
//

typedef void (*DoEdgeProc)(XRaster*, _XRActiveEdge*);
typedef void (*StepProc)(_XRActiveEdge*, S32 yline);

typedef struct _XRActiveEdge {
	_XRActiveEdge *next, *prev;
	S32 x;
	S32 ymax;

	SPOINT d, d2;		// difference stored as 10.22
	SCOORD xl, yl;		// location stored as 16.16
	S32 stepLimit;		// countdown to zero so we know when t == 1

	_XRColor *color1, *color2;
	int dir;

	DoEdgeProc doEdgeProc;
	StepProc stepProc;

	void SetUp(REdge* e, S32 y, BOOL interpolate);
}RActiveEdge;

//
// Difference Equation
//
//	P = A*u^2 + B*u + C
//
//	A = anchor1 - 2*control + anchor2
//	B = 2*control - 2*anchor1
//	C = anchor1
//
//	D(P, u) = 2*u*A + B
//
//	D2(P, u) = 2*A
//

static const DoEdgeProc edgeProcTable[] = {
		DoEdgeEdgeRule,
		DoEdgeEvenOddRule,
		DoEdgeWindingRule
	};

void StepLine(RActiveEdge* edge, S32 yline)
{
 	// Step down a line
	edge->xl += edge->d.x;
	edge->x = (edge->xl + 0x8000L) >> 16;
}

void StepCurve(RActiveEdge* edge, S32 yline)
{
	// Step down a curve
	S32 ymin = yline << 16;
	while ( edge->yl < ymin && edge->stepLimit >= 0 ) {
		// Step
		edge->yl += edge->d.y >> 8;
		edge->d.y += edge->d2.y;

		edge->xl += edge->d.x >> 8;
		edge->d.x += edge->d2.x;

		edge->stepLimit--;
	}

	edge->x = (edge->xl + 0x8000L) >> 16;
}

void StepCurveInterpolate(RActiveEdge* edge, S32 yline)
{
	// Step down a curve
	S32 ymin = yline << 16;
	S32 prevX = edge->xl;
	S32 prevY = edge->yl;

	while ( edge->yl < ymin && edge->stepLimit >= 0 ) {
		// Step
		edge->yl += edge->d.y >> 8;
		edge->d.y += edge->d2.y;

		edge->xl += edge->d.x >> 8;
		edge->d.x += edge->d2.x;

		edge->stepLimit--;
	}

	// Calculate a precise x position
	SCOORD dy= edge->yl - prevY;
	if ( dy != 0 ) {
		// Calculate a position along the line
		SCOORD dx = edge->xl - prevX;
		SCOORD ky = ymin - edge->yl;
		edge->x = (edge->xl + FixedDiv(FixedMul(ky, dx), dy) + 0x8000L) >> 16;
	} else {
		edge->x = (edge->xl + 0x8000L) >> 16;
	}
}

void RActiveEdge::SetUp(REdge* e, S32 y, BOOL interpolate)
{
	// Set up the difference equation
	if ( e->isLine ) {
		// Do a straight line
		stepProc = StepLine;

	 	SCOORD dx = (SCOORD)e->anchor2x - (SCOORD)e->anchor1x;
	 	SCOORD dy = (SCOORD)e->anchor2y - (SCOORD)e->anchor1y;

		// Set up the difference equation
		d.x = (dx << 16)/dy;
		xl = (SCOORD)e->anchor1x << 16;

		// Adjust for missed lines
		dy = y - (SCOORD)e->anchor1y;
		if ( dy != 0 )
			xl += d.x*dy;

		x = (xl+0x8000L)>>16;

	} else {
		SPOINT a, b;
		SCOORD dt, d2t;

		// Set up for a curve
		stepProc = interpolate ? StepCurveInterpolate : StepCurve;

		// compute quadratic coeffs w/ 16 bits of fraction
		a.x = (SCOORD)e->anchor1x - ((SCOORD)e->controlx<<1) + (SCOORD)e->anchor2x;
		b.x = ((SCOORD)e->controlx - (SCOORD)e->anchor1x)<<1;

		a.y = (SCOORD)e->anchor1y - ((SCOORD)e->controly<<1) + (SCOORD)e->anchor2y;
		b.y = ((SCOORD)e->controly - (SCOORD)e->anchor1y)<<1;

		stepLimit = ((SCOORD)e->anchor2y - (SCOORD)e->anchor1y)<<1;
		dt = (1L<<24)/stepLimit;	// Guess at two steps per scanline
		d2t = _FPMul(dt, dt, 24);

		// Calc step values
		d.x = b.x * dt;
		d2.x = (a.x<<1) * d2t;

		d.y = b.y * dt;
		d2.y = (a.y<<1) * d2t;

		FLASHASSERT(d.y > 0 || d2.y > 0);

		// Setup the location
		xl = (SCOORD)e->anchor1x << 16;
		yl = (SCOORD)e->anchor1y << 16;

		x = e->anchor1x;

		// Handle starting in the middle of a curve
		if ( y > e->anchor1y )
		 	stepProc(this, y);
	}

	ymax = e->anchor2y;

	color1 = e->color1;
	color2 = e->color2;
//	FLASHASSERT(e->fillRule!=fillWindingRule);
//	if(e->fillRule==0&&!color1||!color2)
//		int v=0;
	doEdgeProc = edgeProcTable[e->fillRule];
	dir = e->dir;
}


//
// Pixel Color Calculation
//

const CalcPixelProc CalcPixelProcs[] = {
	CalcSolidPixel,		// colorSolid
	CalcBitmapPixel,	// colorBitmap
	CalcGradientPixel,	// colorGradient
	NULL
};


//
// The Antialiasing run object
//

inline void RRun::Init(S32 l, S32 r)
{
//	if(r>240*2)
//		int v=0;
	next = 0;
	xmin = l;
	xmax = r;
	nColors = 0;
	isPure = true;
	isComplex = false;
	transparent = false;
	hasCopyLayer = false;
}

inline void RRun::AddColor(_XRColor* rc)
{
	//return;
	FLASHASSERT(nColors < 4);
	FLASHASSERT(rc);

	// Add a new color entry
	_XRColor** stack = stacks[nColors].colors;

	int n = 0;
	while ( true ) {
		// Skip this color if it is clipped out
		if ( rc->clippedBy ) {
			if ( rc->colorType == colorClip )
				goto SkipColor; // this is a clipping color
			_XRColor* cc = rc->clippedBy;
			do {
				if ( !cc->visible )
					goto SkipColor;
				cc = cc->clippedBy;
				FLASHASSERT(cc);
			} while ( cc != (void*)1 );
		}

		if(!rc->drawSlabProc)
			goto SkipColor;

		stack[n] = rc;
		if ( stacks[0].colors[n] != rc )
			isPure = false;
		isComplex |= rc->colorType;	// NOTE: assumes colorSolid == 0, and the isComplex is only tested for true or false
		//if ( rc->colorType != colorSolid )
		//	isComplex = true;
		n++;
		if ( !rc->transparent )
			break;
		transparent = true;

	SkipColor:
		rc = rc->nextActive;
		if ( !rc ) break;

		if ( n == maxRunStack ) // if we overflow the stack always drop the bottom color
			break;

	}
	if ( n > 0 ) {
		stackN[nColors] = n;
		if ( n != stackN[0] )
			isPure = false;
		nColors++;
	}
}

void RRun::CalcColor(S32 x, S32 y, RGBI* rgb)
// Update the color of this run based on the given device pixel location
{
//	_ANALYENTER("CalcColor");
//	return;
	U32 rgbPoint = (y<<16) | (x&0xFFFF);
#ifdef _USE64BITS
	*((XU64*)rgb)=0;
#else
	((RGBIL*)rgb)->c0 = ((RGBIL*)rgb)->c1 = 0;
#endif
	if ( transparent ) {
		// Handle the most general case of transparent colors
		for ( int i = nColors; i--; ) {
			int n = stackN[i];
			_XRColor** c = stacks[i].colors+n-1;
			RGBI pix;
			BOOL bottom = true;
			while ( n-- > 0 ) {
				_XRColor* rc = *c;
				if ( rc->rgbPoint != rgbPoint ) {
					if ( rc->colorType != colorSolid )
						(CalcPixelProcs[rc->colorType])(rc, x, &rc->rgb);
					rc->rgbPoint = rgbPoint;
				}
				if ( bottom ) {
					pix = rc->rgb;
					bottom = false;
				} else {
					CompositeRGBPixel(&rc->rgb, &pix);
				}
				c--;
			}
#ifdef _USE64BITS
			*((XU64*)rgb)+=*((XU64*)&pix);
			//((RGBIL*)rgb)->c0 += ((RGBIL*)&pix)->c0;
			//((RGBIL*)rgb)->c1 += ((RGBIL*)&pix)->c1;
			if ( isPure ) {
				(*((XU64*)rgb))*=nColors;
				//((RGBIL*)rgb)->c0 *= nColors;
				//((RGBIL*)rgb)->c1 *= nColors;
				break;
			}
#else
			((RGBIL*)rgb)->c0 += ((RGBIL*)&pix)->c0;
			((RGBIL*)rgb)->c1 += ((RGBIL*)&pix)->c1;
			if ( isPure ) {
				((RGBIL*)rgb)->c0 *= nColors;
				((RGBIL*)rgb)->c1 *= nColors;
				break;
			}
#endif
		}
	} else {
		// The colors are all opaque
		for ( int i = nColors; i--; ) {
			_XRColor* rc = stacks[i].colors[0];
			if ( rc->rgbPoint != rgbPoint ) {
				if ( rc->colorType != colorSolid ) {
					(CalcPixelProcs[rc->colorType])(rc, x, &rc->rgb);
					rc->rgb.alpha = 255;
				}
				rc->rgbPoint = rgbPoint;
			}
#ifdef _USE64BITS
			*((XU64*)rgb)+=*((XU64*)&rc->rgb);
			//((RGBIL*)rgb)->c0 += ((RGBIL*)&rc->rgb)->c0;
			//((RGBIL*)rgb)->c1 += ((RGBIL*)&rc->rgb)->c1;
			if ( isPure ) {
				(*((XU64*)rgb))*=nColors;
				//((RGBIL*)rgb)->c0 *= nColors;
				//((RGBIL*)rgb)->c1 *= nColors;
				break;
			}
#else
			((RGBIL*)rgb)->c0 += ((RGBIL*)&rc->rgb)->c0;
			((RGBIL*)rgb)->c1 += ((RGBIL*)&rc->rgb)->c1;
			if ( isPure ) {
				((RGBIL*)rgb)->c0 *= nColors;
				((RGBIL*)rgb)->c1 *= nColors;
				break;
			}
#endif
		}
	}
}

inline RRun* RRun::Split(XRaster* render, S32 x)
{
	// Split the run
	RRun *n = render->CreateRun();
	if ( !n ) return 0;
	if ( transparent ) {
		// Copy the entire stacks
		*n = *this;
	} else {
		// Copy just the tops of the stacks if there are not transparent colors
		*(RRunCore*)n = *(RRunCore*)this;
		n->stacks[0].colors[0] = stacks[0].colors[0];
		n->stacks[1].colors[0] = stacks[1].colors[0];
		n->stacks[2].colors[0] = stacks[2].colors[0];
		n->stacks[3].colors[0] = stacks[3].colors[0];
	}
	xmax = n->xmin = x;
	next = n;
	return n;
}


//
// The pixel object used to render partially covered pixels
//

typedef struct _XRPixel {
	XRaster* raster;
	RGBI rgb;
	S32 x, y,nShift;

	void Init(XRaster* r, S32 py,int shift);
	void Add(RGBI* rgb, int yw, S32 px, int xw);
	void Paint();
}RPixel;

#ifdef _USE64BITS

inline void _XRPixel::Init(XRaster* r, S32 py,int shift)
{
	//((RGBIL*)&rgb)->c0 = 0;
	//((RGBIL*)&rgb)->c1 = 0;
	*((XU64*)&rgb)=0;
	x = -32000;
	y = py;
	nShift=shift;
	raster = r;
}

inline void _XRPixel::Paint()
{
	if ( rgb.alpha ) {
		// Divide by 16
		*((XU64*)&rgb)=( (*((XU64*)&rgb))>>nShift)&_MASK64;
		//((RGBIL*)&rgb)->c0 = (((RGBIL*)&rgb)->c0 >> nShift) & 0x00FF00FFL;
		//((RGBIL*)&rgb)->c1 = (((RGBIL*)&rgb)->c1 >> nShift) & 0x00FF00FFL;

//		if(rgb.alpha!=0xff)
//			int v=0;

		raster->DrawRGBAPixelI(x, &rgb);

		#ifdef ENABLE_MASK
		if ( raster->maskBits )
			raster->SetMask(x, x+1);
		#endif
	}
	// Reset
	*((XU64*)&rgb)=0;
	//((RGBIL*)&rgb)->c0 = 0;
	//((RGBIL*)&rgb)->c1 = 0;
}

inline void _XRPixel::Add(RGBI* rc, int yw, S32 px, int xw)
// xw is the x coverage it is multiplied by the y coverage in r to get the area coverage
{
	if ( x != px ) {
		Paint();
	 	x = px;
	}

	*((XU64*)&rgb)+=xw*(*((XU64*)rc));

	//((RGBIL*)&rgb)->c0 += xw*((RGBIL*)rc)->c0;
	//((RGBIL*)&rgb)->c1 += xw*((RGBIL*)rc)->c1;
}

#else
inline void _XRPixel::Init(XRaster* r, S32 py,int shift)
{
	((RGBIL*)&rgb)->c0 = 0;
	((RGBIL*)&rgb)->c1 = 0;
	x = -32000;
	y = py;
	nShift=shift;
	raster = r;
}

inline void _XRPixel::Paint()
{
	if ( rgb.alpha ) {
		// Divide by 16
//		((RGBIL*)&rgb)->c0 = (((RGBIL*)&rgb)->c0 >> nShift) & 0x00FF00FFL;
//		((RGBIL*)&rgb)->c1 = (((RGBIL*)&rgb)->c1 >> nShift) & 0x00FF00FFL;

//		if(rgb.alpha!=0xff)
//			int v=0;
		RGB8 rgb8;
		rgb8.alpha=(rgb.alpha>>nShift)&0xff;
		rgb8.red=(rgb.red>>nShift)&0xff;
		rgb8.green=(rgb.green>>nShift)&0xff;
		rgb8.blue=(rgb.blue>>nShift)&0xff;

		raster->DrawRGBAPixel(x, &rgb8);

		#ifdef ENABLE_MASK
		if ( raster->maskBits )
			raster->SetMask(x, x+1);
		#endif
	}
	// Reset
	((RGBIL*)&rgb)->c0 = 0;
	((RGBIL*)&rgb)->c1 = 0;
}

inline void _XRPixel::Add(RGBI* rc, int yw, S32 px, int xw)
// xw is the x coverage it is multiplied by the y coverage in r to get the area coverage
{
	if ( x != px ) {
		Paint();
	 	x = px;
	}

	((RGBIL*)&rgb)->c0 += xw*((RGBIL*)rc)->c0;
	((RGBIL*)&rgb)->c1 += xw*((RGBIL*)rc)->c1;
}
#endif
//
// The Renderer Object
//
XRaster* XRaster::instance=XNULL;

XRaster::XRaster() :
				activeEdgeAlloc(sizeof(RActiveEdge), 128 ),
				runAlloc(sizeof(RRun), 256 ),
				rampAlloc(sizeof(RGB8)*257, 32 ),
				mapAlloc(sizeof(_XRColorMap), 16 )
				//activeEdgeAlloc(sizeof(RActiveEdge), 64, true, 0x33333333),
				//runAlloc(sizeof(RRun), 128, true, 0x55555555),
				//rampAlloc(sizeof(RGB8)*257, 16, true, 0xEEEEEEEE),
				//mapAlloc(sizeof(_XRColorMap), 8, true)
{
	if(instance==XNULL)
		instance=this;
#ifdef _BACKLAYER
	rbits=XNULL;
	//hbits=XNULL;
#endif
	if ( !renderTablesBuilt )
		BuildRenderTables();
	antialias_b = true;
	getBackground = false;
	smoothBitmaps = false;
	antiShift=1;
	antiMask=1;
	antiStep=1;
	solidDither = orderedDither;
	bitmapDither = orderedDither;

	patAlign.x = patAlign.y = 0;

	//activeColors = 0;
	topColor = 0;

	firstActive = 0;
	yindex = 0;
	yindexSize = 0;

	firstRun = 0;

	ditherData = 0;

	bits = 0;
	cinfo = 0;
	baseAddr = 0;

	topColorXleft = 0;

#ifdef ENABLE_MASK
	maskBits = 0;
	getBackProc = 0;
#endif
}

XRaster::~XRaster()
{
	if(instance==this)
		instance=XNULL;
	delete [] yindex;
	delete [] ditherData;
}

void XRaster::FreeEmpties()
{
//	activeEdgeAlloc.FreeEmpties();
//	runAlloc.FreeEmpties();
//	rampAlloc.FreeEmpties();
}

//
// Painting code
//

BOOL XRaster::BitsValid()
{
	return bits && bits->BitsValid();
}

void XRaster::Attach(CBitBuffer* b, SRECT* c, BOOL doAntialias,XU8 shift)
{
	if ( b ) {
		bits = b;
		cinfo = bits->getSColorInfo();	//m_cinfo;
		inverted = bits->inverted();	//m_inverted;
		bitHeight = bits->height();		//m_bufHeight;
		bitArea= bits->height()*bits->width();
		baseAddr = bits->baseAddess();	//b(char*)bits->m_baseAddr;
		rowBytes = bits->scanLine();	//m_rowBytes;

		xorg = bits->xorg();
		pixelFormat = bits->pixelFormat();

		doGetBackground = GetBackgroundProcs[pixelFormat];

		{
			drawRGBSlab = DrawRGBSlabProcs[pixelFormat];
			//ditherRGBSlab = DitherRGBSlabProcs[bitmapDither];
		}

		//if ( alphaChannel && pixelFormat == pixFmt32 )
		//	pixelFormat = pixFmt32A;
	} else {
	 	bits = 0;
		cinfo = 0;
		baseAddr = 0;
	}

	antialias_b = doAntialias;
	antiShift=shift;
	//antiMask=(0xffffffff>>shift)<<shift;
	antiStep=1<<shift;
	antiMask=antiStep-1;
//	antiRuns=antiStep-1;

// 	RectSet(0, 0, bits->m_bufWidth, bits->m_bufHeight, &bitClip);
	RectSet(0, 0, bits->width(), bits->height(), &bitClip);
	if ( c )
		RectIntersect(c, &bitClip, &bitClip);

	edgeClip = bitClip;
	if ( antialias_b ) {
		//edgeClip.xmin *= 4;
		//edgeClip.ymin *= 4;
		//edgeClip.xmax *= 4;
		//edgeClip.ymax *= 4;
		edgeClip.xmin <<=antiShift;
		edgeClip.ymin <<=antiShift;
		edgeClip.xmax <<=antiShift;
		edgeClip.ymax <<=antiShift;
		//DisplayList::ToAnti(edgeClip);
	}

	ylines = edgeClip.ymax - edgeClip.ymin + 1;
	if ( ylines <= 0 ) {
		ylines = 0;
		FLASHASSERT(false);
	}
}

void XRaster::UseAlphaChannel()
{
	if ( pixelFormat == pixFmt32 )
		pixelFormat = pixFmt32A;
}

void XRaster::FillBackground(SRGB backgroundColor)
{
	BeginPaint();

	// Add the background edges
	int savedPixelFormat = pixelFormat;
	if ( pixelFormat == pixFmt32A )
		pixelFormat = pixFmt32;

	// Set up the color
	_XRColor bkColor;
	bkColor.SetUp(this);
	bkColor.order = 0;
	bkColor.rgb.alpha = backgroundColor.rgb.transparency;
	bkColor.rgb.red   = backgroundColor.rgb.red;
	bkColor.rgb.green = backgroundColor.rgb.green;
	bkColor.rgb.blue  = backgroundColor.rgb.blue;
	bkColor.BuildCache();

	// Set up the edges
	REdge bkEdges[2];
	bkEdges[0].nextObj = &bkEdges[1];
	bkEdges[1].nextObj = 0;

	SPOINT pt1, pt2;
	CURVE c;
	pt1.y = edgeClip.ymin;
	pt2.y = edgeClip.ymax;

	pt1.x = pt2.x = edgeClip.xmin;
	CurveSetLine(&pt1, &pt2, &c);
	bkEdges[0].Set(&c);

	pt1.x = pt2.x = edgeClip.xmax;
	CurveSetLine(&pt1, &pt2, &c);
	bkEdges[1].Set(&c);

	bkEdges[0].dir 		= bkEdges[1].dir      = 0;
	bkEdges[0].fillRule = bkEdges[1].fillRule = fillEvenOddRule;
	bkEdges[0].color1 	= bkEdges[1].color1   = &bkColor;
	bkEdges[0].color2 	= bkEdges[1].color2   = 0;

	AddEdges(bkEdges);

	PaintBits();

	bkColor.FreeCache();

	pixelFormat = savedPixelFormat;

	getBackground = backgroundColor.all != SRGBWhite;
}

typedef REdge* PREdge;

void XRaster::BeginPaint()
{
	FLASHASSERT(baseAddr);

	// Setup the y index
	S32 newSize = ylines;
	if ( newSize > yindexSize ) {
		delete [] yindex;//free(yindex);
		yindex = new PREdge[newSize];
		if ( !yindex ) {
			yindexSize = 0;
			return;
		}
		yindexSize = newSize;
	}

	memset(yindex, 0, (int)(newSize * (sizeof(PREdge))));

	firstActive = 0;
	topColor = 0;

	needFlush = false;
	layerDepth = 0;
}

void XRaster::AddEdges(REdge* edge)
{
	if ( !edge ) return;
	REdge*e=edge;
	while(e)
	{
		if(!e->color1)
			return;
		e=e->nextObj;
	}
	while ( edge ) {
		if ( edge->anchor1y <= edgeClip.ymax && edge->anchor2y > edgeClip.ymin && edge->anchor1y!=edge->anchor2y) {
			// Insert this edge at the proper scanline
			S32 i = edge->anchor1y - edgeClip.ymin;
			if ( i < 0 ) i = 0;

			//if(edge->fillRule==0&&(!edge->color1||!edge->color2))
			//	int v=0;
			/*REdge*t=yindex[i];
			while(t)
			{
				if(t==edge)
					int v=0;
				t=t->nextActive;
			}*/

			edge->nextActive = yindex[i];
			yindex[i] = edge;
		}
		edge = edge->nextObj;
	}
	needFlush = true;
}

void XRaster::AddEdges(REdge* edge, _XRColor* colors, _XRColor* clipColor)//,_GRIDINFO*gridBy)
// Add the edges and adjust the color depths for the current layering
{
//	if((XU32)clipColor==1&&colors->colorType==0)
//		int v=0;
//	_ANALYENTER("AddEdges");
	//gridBy=NULL;
	AddEdges(edge);
	while ( colors ) {

		/*if(colors->colorType==colorBitmap)
		{
			if(colors->bm.bitmap->baseAddr==NULL)
				int v=0;
		}*/


		colors->order = (colors->order & 0xFFFF) | layerDepth;
		colors->clippedBy = clipColor;
		
		//colors->gridBy0=gridBy;
		//if(gridBy)
		//{
		//	colors->BuildGrid();
		//}
		colors = colors->nextColor;
	}
	layerDepth += 0x10000;
}

inline void XRaster::AddActive()
{// Add new active edge entries
//	_ANALYENTER("AddActive");
	
	REdge *e, *lastE;
	RActiveEdge *a, *pos;
	REdge** index;
	S32 x;

	pos = firstActive;
	index = yindex + y-edgeClip.ymin;
	for ( e = *index; e; e = e->nextActive ) {
		a = CreateActiveEdge();
		if ( !a ) return;

		a->SetUp(e, y, !antialias_b);

		// Insert into the active edge list
		if ( pos ) {
			// Find the proper position
			x = a->x;
			if ( pos->x < x ) {
				while ( pos->x < x && pos->next )
					pos = pos->next;
			} else if ( pos->x > x ) {
				while ( pos->x > x && pos->prev )
					pos = pos->prev;
			}

			DListInsertAfter(pos, a, RActiveEdge);

		} else {
			// Insert into an empty list
			DListAdd(firstActive, a, RActiveEdge);
		}
		pos = a;

		lastE = e;	// remember the tail of the list
	}
}

inline void XRaster::SortActive()
{// Sort the active edges

	RActiveEdge *a, *b, *stop;
	BOOL swapped;

	stop = 0;
	do {
		swapped = false;
		a = firstActive;

		while ( a != 0 && (b = a->next) != 0 ) {
			if ( a->x > b->x ) {
				// Swap
			 	DListSwap(firstActive, a, b, RActiveEdge);
			 	if ( !swapped && b->prev && b->prev->x > b->x )
			 		swapped = true;
			} else {
				// Just keep going
				a = b;
			}
		}
	} while ( swapped );
}


//
// Get Background Procs
//


inline void GetBackground32(XRaster* r, S32 xmin, S32 xmax, RGB8* pix)
{
	U32 * src = (U32 *)r->rowAddr + (xmin);// + r->xorg);
	for ( S32 i = xmax-xmin; i--; pix++, src++ ) {
		*((XU32*)pix)=*src;
		//UnpackPix32(*src, pix);
	}
}

inline void GetBackgroundWhite(XRaster* r, S32 xmin, S32 xmax, RGB8* pix)
{
	// Set a white background
	RGB8 white;
	white.alpha = 0;
	white.red = white.green = white.blue = 255;
	int n = xmax - xmin;
	while ( n-- > 0 )
		*pix++ = white;
}

inline void GetBackgroundNULL(XRaster* r, S32 xmin, S32 xmax, RGB8* pix)
{
	// Set a white background
}


inline void GetBackgroundBlack(XRaster* r, S32 xmin, S32 xmax, RGB8* pix)
{
	// Set a white background
	memset(pix,0, (xmax-xmin)*sizeof(RGB8));
	//RGB8 white;
	//white.alpha = 0;
	//white.red = white.green = white.blue = 0;
	//int n = xmax - xmin;
	//while ( n-- > 0 )
	//	*pix++ = white;
}


//
// Drawing with an alpha channel
//

void XRaster::CompositeSlab(S32 xleft, S32 xright, _XRColor** stack, int n)
{
	// Apply the colors
	//if(bitY==270)
	//	int v=0;
	//return;
#ifdef _DIRECTDRAW_
	while ( xleft < xright ) {
		RGB8* pixBuf=(RGB8*)_ROWADDR(this,xleft);
		S32 limit = Min(xright, xleft+RGBSlabChunkSize);

		_XRColor** c = stack+n-1;
		int i = n;
#ifdef _LINEALPHA_
		//if(bitY==488)
		//	int v=0;
		bool bCopy=false;
		if(xright>xleft+4)
		{
			_XRColor**c0=stack;
			int ii=0;
			while(ii<n)
			{
				if(IsCopyLine(*c0,xleft,limit))
				//if((*c0)->copyColor)
				{
					bCopy=true;
					BuildBitmapSlab(*c0,xleft,limit,pixBuf);
					c=c0-1;
					i = ii;
					break;
				}
				ii++;
				c0++;
			}
		}

		if(!bCopy)
#endif
		{
			if ( (*c)->transparent ) { // if the bottom visible color is transparent, get the bits
				//doGetBackground(this, xleft, limit, pixBuf);
			} else {
				if ( (*c)->colorType == colorSolid ){//&& (*c)->pattern == 0 ) {
					// For the lower solid color layers, step through the color stack and
					//	composite a single pixel to find the pixel value for the entire run
					RGBI rgb = (*c)->rgb;
					c--;i--;
					while ( i > 0 && (*c)->colorType == colorSolid ) {
						CompositeRGBPixel(&(*c)->rgb, &rgb);
						c--;i--;
					}

					int n = limit-xleft;
					RGB8 rgb8;
					rgb8.alpha=rgb.alpha;
					rgb8.blue=rgb.blue;
					rgb8.red=rgb.red;
					rgb8.green=rgb.green;
					RGB8* p = pixBuf;
					while ( n-- )
						*p++ = rgb8;
				}
			}
		}
		

		while ( i > 0 ) {
			// Composite the slab to the buffer
			//if(IsCopyLine(*c,xleft,limit))
			//	BuildBitmapSlabGrid(*c,xleft,limit,pixBuf);
				//((*c)->drawSlabProc)(*c,xleft,
			//else
			((*c)->compositeSlabProc)(*c, xleft, limit, pixBuf);
			c--;i--;
		}
		//drawRGBSlab(this, xleft, limit, pixBuf);
		xleft = limit;
	}
#else
	while ( xleft < xright ) {
		RGB8 pixBuf[RGBSlabChunkSize];
		S32 limit = Min(xright, xleft+RGBSlabChunkSize);

		_XRColor** c = stack+n-1;
		int i = n;
#ifdef _LINEALPHA_
		bool bCopy=false;
		{
			_XRColor**c0=stack;
			int ii=0;
			while(ii<n)
			{
				if(IsCopyLine(*c0,xleft,limit))
				{
					bCopy=true;
					BuildBitmapSlab(*c0,xleft,limit,pixBuf);
					//memset(pixBuf,0xff,RGBSlabChunkSize*4);
					c=c0-1;
					i = ii;
					break;
				}
				ii++;
				c0++;
			}
		}

		if(!bCopy)
#endif
		{
			if ( (*c)->transparent ) { // if the bottom visible color is transparent, get the bits
				doGetBackground(this, xleft, limit, pixBuf);
			} else {
				/*if ( (*c)->pattern != 0 ) {
					// Note that patterned fills should never be transparent
					((*c)->compositeSlabProc)(*c, xleft, limit, pixBuf);
					ApplyRGBPattern((*c)->pattern, bitY, xleft, limit, pixBuf);
					c--;i--;
				} else*/
				if ( (*c)->colorType == colorSolid ){//&& (*c)->pattern == 0 ) {
					// For the lower solid color layers, step through the color stack and
					//	composite a single pixel to find the pixel value for the entire run
					RGBI rgb = (*c)->rgb;
					c--;i--;
					while ( i > 0 && (*c)->colorType == colorSolid ) {
						CompositeRGBPixel(&(*c)->rgb, &rgb);
						c--;i--;
					}

					int n = limit-xleft;
					RGB8 rgb8;
					rgb8.alpha=rgb.alpha;
					rgb8.blue=rgb.blue;
					rgb8.red=rgb.red;
					rgb8.green=rgb.green;
					RGB8* p = pixBuf;
					while ( n-- )
						*p++ = rgb8;
				}
			}
		}
		

		while ( i > 0 ) {
			// Composite the slab to the buffer
			//if(IsCopyLine(*c,xleft,limit))
			//	BuildBitmapSlabGrid(*c,xleft,limit,pixBuf);
				//((*c)->drawSlabProc)(*c,xleft,
			//else
			((*c)->compositeSlabProc)(*c, xleft, limit, pixBuf);
			c--;i--;
		}
		drawRGBSlab(this, xleft, limit, pixBuf);
		xleft = limit;
	}
#endif
}


//
// Paint a slab of color
//

void XRaster::PaintSlab(S32 xright)
// Apply a slab to the current run using the topColor
{
	S32 xleft = topColorXleft;
	topColorXleft = xright;

	if ( topColor ) {
		if ( antialias_b ) {
			register RRun* r = curRun;

			if ( !r || r->xmin >= xright ) return; // we are outside the visible region

			// Skip over blank sections
			while ( r->xmax < xleft ) {
				if ( !(r = r->next) ) {
					// We went past the right edge of the clip rect
		 			curRun = 0;
					return;
				}
			}

			// Split the run if needed
			if ( r->xmin < xleft )
				r = r->Split(this, xleft);

			// Paint the covered run sections
			while ( r && r->xmin < xright ) {
				if ( r->xmax > xright ) {
					//r->Split(this, xright);
					curRun = r->Split(this, xright);
					r->AddColor(topColor);
					return;
				}

				r->AddColor(topColor);
				r = r->next;
			}

			curRun = r;

		} else {
			// Clip to the bitmap
			if ( xleft < bitClip.xmin )
				xleft = bitClip.xmin;
			if ( xright > bitClip.xmax )
				xright = bitClip.xmax;

			if ( xright > xleft ) {
				// Build a bottom up list of visible colors
				_XRColor* stack[MAXCOLORSTACK];
				int n = 0;

				{// Build a stack of colors
					_XRColor* c = topColor;
					while ( c ) {
						// Skip this color if it is clipped out
						if ( c->clippedBy ) {
							if ( c->colorType == colorClip )
								goto SkipColor; // this is a clipping color
							_XRColor* cc = c->clippedBy;
							do {
								if ( !cc->visible )
									goto SkipColor;
								cc = cc->clippedBy;
								FLASHASSERT(cc);
							} while ( cc != (void*)1 );
						}

						if ( n < MAXCOLORSTACK ) // we always want to keep the bottom color since it is usually not tranparent
							n++;
						else //drop buttom color
							break;
						stack[n-1] = c;
						if ( !c->transparent ) break;
					SkipColor:
						c = c->nextActive;
					}
				}

				if ( n > 0 ) {
					if ( !stack[0]->transparent )
						(stack[0]->drawSlabProc)(stack[0], xleft, xright);	// The top color is opaque, just draw it
					else
					{
						//if(IsCopyLine(stack[0],xleft,xright))
						//	(stack[0]->drawSlabProc)(stack[0], xleft, xright);	// The top color is opaque, just draw it
						//else
							CompositeSlab(xleft, xright, stack, n);		// composite the entire stack
					}
				}
			}

			#ifdef ENABLE_MASK
			if ( maskBits )
				SetMask(xleft, xright);
			#endif
		}
	}
}


//
// Manage the color transitions
//

inline void XRaster::ShowColor(_XRColor* c, S32 x)
{
	// Find where to insert the color
	BOOL paint = true;
	_XRColor** link = &topColor;
	while ( true ) {
		_XRColor* tc = *link;
		if ( !tc ) break;
		if ( c->order > tc->order ) {
			break;
		} else {
			if ( !tc->transparent && !tc->clippedBy )
				paint = false;
			link = &tc->nextActive;
		}
	}

	// Draw a slab if the new color is visible (not under an opaque color)
	if ( paint )
		PaintSlab(x);

	// Insert the color
	c->nextActive = *link;
	*link = c;
}

inline void XRaster::HideColor(_XRColor* c, S32 x)
{
	// Find the color
	BOOL paint = true;
	_XRColor** link = &topColor;
	while ( true ) {
		_XRColor* tc = *link;
// #ifndef SPRITE_XTRA
		FLASHASSERT(tc);	// tc should never be null since the color c must be in the list
		if(!tc) break;
		if ( tc == c ) {
// #else
// 		if ( !tc || tc == c ) {
// #endif
			break;
		} else {
			if ( !tc->transparent && !tc->clippedBy )
				paint = false;
			link = &tc->nextActive;
		}
	}

	// Draw a slab if the old color was visible needed
	if ( paint )
		PaintSlab(x);

	// Remove the color
	*link = c->nextActive;
}

//
// Support the Edge rules
//

inline void DoEdgeEdgeRule(XRaster* r, RActiveEdge* a)
{
	// Handle color1
	//if(!a->color1||!a->color2) return;
	_XRColor* c = a->color1;

	if( c->nFilterColor)
	{

	}else
	if ( c->visible ) {
		r->HideColor(c, a->x);
	 	c->visible = false;
	} else {
		r->ShowColor(c, a->x);
	 	c->visible = true;
	}

	// Handle color2
	c = a->color2;
	if( c->nFilterColor)
	{

	}else
	if ( c->visible ) {
		r->HideColor(c, a->x);
	 	c->visible = false;
	} else {
		r->ShowColor(c, a->x);
	 	c->visible = true;
	}
}

inline void DoEdgeEvenOddRule(XRaster* r, RActiveEdge* a)
{
	// Handle color1
	_XRColor* c = a->color1;
	if( c->nFilterColor)
	{

	}else
	if ( c->visible ) {
		r->HideColor(c, a->x);
	 	c->visible = false;
	} else {
		r->ShowColor(c, a->x);
	 	c->visible = true;
	}
}

void DoEdgeWindingRule(XRaster* r, RActiveEdge* a)
{
	// Handle color1
	_XRColor* c = a->color1;
	if( c->nFilterColor)
	{

	}else
	if ( c->visible == 0 ) {
		// The color is becoming active
		r->ShowColor(c, a->x);
		c->visible += a->dir;
	} else {
		int vis = c->visible + a->dir;
		if ( vis == 0 ) {
			// The color disappeared
			r->HideColor(c, a->x);
		}
		c->visible = vis;
	}
}

inline void XRaster::PaintActive()
// Draw the slabs
{
//	_ANALYENTER("PaintActive");

	S32 ynext = y+1;

	// Create a run object
    if ( antialias_b ) {
		if ( !firstRun ) {
		 	// Create an empty run
		 	firstRun = CreateRun();
		 	if ( !firstRun ) return;
		 	firstRun->Init(edgeClip.xmin, edgeClip.xmax);
		}
		curRun = firstRun;
    } else {
		SetYCoord(y);
	}

	{// This should never happen but we can minimize the effect
		//FLASHASSERT(!topColor);
		for ( _XRColor* c = topColor; c; c = c->nextActive )
		{
			c->visible = 0;
// #ifdef SPRITE_XTRA
// 			if (c == c->nextActive)
// 				break;
// #endif
		}

		topColor = 0;
	}

	register RActiveEdge* a = firstActive;
    while ( a ) {
		// Track the active colors
		(a->doEdgeProc)(this, a);

		// Remove out of date active edge entries
		// And calculate new x coordinates for the ones we keep
		if ( a->ymax > ynext ) {
			// Keep this edge
		 	a->stepProc(a, ynext);
		 	a = a->next;
		} else {
			// Remove this edge
    		RActiveEdge* next = a->next;
			DListRemove(firstActive, a, RActiveEdge);
			FreeActiveEdge(a);
			a = next;
		}
    }
}


//
// Antialiased Painting
//

// Merging runs does not seem to be an effective optimization
//#define MERGE_RUNS

#ifdef MERGE_RUNS
inline BOOL RunEqual(RRun* a, RRun* b)
{
	int i = a->nColors;
	if ( i != b->nColors ) return false;
	while ( i-- ) {
		int j = a->stackN[i];
		if ( j != b->stackN[i] ) return false;
		while ( j-- ) {
			if ( a->stacks[i].colors[j] != b->stacks[i].colors[j] )
				return false;
		}
	}
	return true;
}

int nRuns = 0;
int nPureRuns = 0;
int nPureMerged = 0;
int nMerged = 0;
#endif

inline void XRaster::PaintAARuns()
// Paint the runs
{

	S32 xl, xr;
	int xlf, xrf;
	S32 yd;
	RPixel pixel;
	RRun *r, *next;

	yd = y>>antiShift;
	SetYCoord(yd);

	pixel.Init(this, yd,1<<antiShift);


	r = firstRun;
	firstRun = 0;
/*#ifdef _LINEALPHA_
	RRun *nr=r,*nnext,*nprev=NULL;
	while(nr)
	{
		nnext = nr->next;
		if(nr->transparent)
		{
			_XRColor**c0=nr->stacks[0].colors;
			int		  n =nr->stackN[0];
			int ii=0;
			while(ii<n)
			{
				//if(IsCopyLine(*c0,xleft,limit))
				switch((*c0)->colorType)
				{
				case colorBitmap:
					 				
					{
						int xl = nr->xmin>>antiShift;
	 					int xr = nr->xmax>>antiShift;
	 					if(IsCopyLine(*c0,xl,xr))
						{
							(*c0)->copyColor=XTRUE;
							nr->hasCopyLayer=ii+1;
						}
						else
							(*c0)->copyColor=XFALSE;
					}
					break;
				default:
					if(!(*c0)->transparent)
					{
						//(*c0)->copyColor=XTRUE;
						nr->hasCopyLayer=ii+1;
					}
					break;
				}
				ii++;	
			}

			if(nprev&&nr->hasCopyLayer&&nr->hasCopyLayer==nprev->hasCopyLayer)
			{
				int ly=nr->hasCopyLayer-1;
				_XRColor*color=nprev->stacks[0].colors[ly];
				if(nr->stacks[0].colors[ly]==color)
				{
					//if(color->colorType!=colorBitmap)
					//	color->copyColor=false;
					nprev->xmax=nr->xmax;
					FreeRun(nr);
					nprev->next=nnext;
				}
				else
					nprev=nr;
			}
			else
			  nprev=nr;
		}
		else
		{
			if(nprev)
			{
				if(nprev->transparent||
				   *(nprev->stacks[0].colors)!=
				   *(nr->stacks[0].colors)) 
				   nprev=nr;
				else
				{
					nprev->xmax=nr->xmax;
					FreeRun(nr);
					nprev->next=nnext;
				}				
			}
			else nprev=nr;
		}
			//nprev=NULL;
		nr = nnext;
	}
#endif*/
	while ( r ) {
 		if ( r->nColors ) 
		{
			#ifdef MERGE_RUNS
    		// Check to merge adjacent identical runs
    		if ( r->isPure && r->nColors == antiStep ) {
				nPureRuns++;
    			while ( (next = r->next) != 0 ) {
					if ( r->stacks[0].colors[0] != next->stacks[0].colors[0] ) goto MergeDone;	// this should cause failure most of the time
					if ( !next->isPure ) goto MergeDone;
					if ( next->nColors != antiStep ) goto MergeDone;
					int i = r->stackN[0];
					if ( i != next->stackN[0] ) goto MergeDone;
					_XRColor** ra = r->stacks[0].colors+1;
					_XRColor** rb = next->stacks[0].colors+1;
					i--;
					while ( i-- > 0) {
						if ( *ra++ != *rb++ )
							goto MergeDone;
					}
    				r->xmax = next->xmax;
    				r->next = next->next;
    				FreeRun(next);
					nPureMerged++;
    			}
			  MergeDone:
				;
    		}
    		{
    			while ( (next = r->next) && RunEqual(r, next) ) {
    				r->xmax = next->xmax;
    				r->next = next->next;
    				FreeRun(next);
					nMerged++;
    			}
    		}
			nRuns++;
			#endif

    		// Draw a run
	 		/*xl = r->xmin>>2;
	 		xlf = (int)r->xmin & 3;
	 		xr = r->xmax>>2;
	 		xrf = (int)r->xmax & 3;*/
			xl = r->xmin>>antiShift;	 		
	 		xr = r->xmax>>antiShift;
#ifdef		_FASTBITMAP_
			_XRColor*color=r->stacks[0].colors[0];
			if(color->colorType==colorBitmap&&
				color->transparent)
			{
	 				if ( xl < xr ) {

		 				// Handle the solid run
		 				//if ( r->isPure && r->nColors == antiStep ) 
						{
		 					// This is a fully covered run
							if ( r->transparent ) {
								//if(IsCopyLine(r->stacks[0].colors[0],xl,xr))
									//(r->stacks[0].colors[0]->drawSlabProc)(r->stacks[0].colors[0], xl, xr);
								//{
								//	(r->stacks[0].colors[0]->drawSlabProc)(r->stacks[0].colors[0], xl, xr);
								//}
								//else 
									CompositeSlab(xl, xr, r->stacks[0].colors, r->stackN[0]);
							} else {
								(r->stacks[0].colors[0]->drawSlabProc)(r->stacks[0].colors[0], xl, xr);
							}

		 				}
					}
			}
			else
#endif
			{

				xlf = (int)r->xmin & antiMask;
	 			xrf = (int)r->xmax & antiMask;

				RGBI runRGB;
				BOOL runRGBValid = false;

	 			if ( xl == xr ) {
	 				// The run is completely within the current pixel
					if ( !runRGBValid )	{
						r->CalcColor(xl, yd, &runRGB);
						runRGBValid = true;
					}
	 				pixel.Add(&runRGB, r->nColors, xl, xrf - xlf);

	 			} else {
	 				// The run covers more than a single pixel
		 			if ( xlf ) {
		 				// Handle the leading fraction
						if ( !runRGBValid )	{
							r->CalcColor(xl, yd, &runRGB);
							runRGBValid = true;
						}
		 				pixel.Add(&runRGB, r->nColors, xl, antiStep-xlf);
			 			xl++;	// round up
		 			}

		 			if ( xl < xr ) {
		 				// Handle the solid run
		 				if ( r->isPure && r->nColors == antiStep ) {
		 					// This is a fully covered run
							if ( r->transparent ) {
								//if(IsCopyLine(r->stacks[0].colors[0],xl,xr))
									//(r->stacks[0].colors[0]->drawSlabProc)(r->stacks[0].colors[0], xl, xr);
								//{
								//	(r->stacks[0].colors[0]->drawSlabProc)(r->stacks[0].colors[0], xl, xr);
								//}
								//else 
									CompositeSlab(xl, xr, r->stacks[0].colors, r->stackN[0]);
							} else {
								(r->stacks[0].colors[0]->drawSlabProc)(r->stacks[0].colors[0], xl, xr);
							}

		 				} else {
							// Handle a non-pure or partially covered run
							int x = xl;
							do {
								RGBI pixBuf[RGBSlabChunkSize];
	#ifdef _DIRECTDRAW_
								XU32*pixBuf8=(XU32*)_ROWADDR(this,x);
	#else
								XU32 pixBuf8[RGBSlabChunkSize];
	#endif
								int limit = Min(xr, x+RGBSlabChunkSize);

	#ifdef _USE64BITS
								if ( r->isComplex ) {
									RGBI* pix = pixBuf;
									for ( S32 j = x; j < limit; j++, pix++ ) {
										r->CalcColor(j, yd, pix);
										*((XU64*)pix)= ((*((XU64*)pix))>>antiShift)&_MASK64;
										pixBuf8[j-x]=PackPix32(*pix);
										//pixelBuf8
										//(*(XU32*)pixBuf8)=Pack
										//((RGBIL*)pix)->c0 = (((RGBIL*)pix)->c0 >> antiShift) & 0x00FF00FFL;
										//((RGBIL*)pix)->c1 = (((RGBIL*)pix)->c1 >> antiShift) & 0x00FF00FFL;
									}

								} else {
									if ( !runRGBValid )	{
										r->CalcColor(xl, yd, &runRGB);
										runRGBValid = true;
									}
									*((XU64*)pixBuf)=((*((XU64*)&runRGB))>>antiShift)&_MASK64;
									pixBuf8[0]=PackPix32(*pixBuf);
									//((RGBIL*)pixBuf)->c0 = (((RGBIL*)&runRGB)->c0 >> antiShift) & 0x00FF00FFL;
									//((RGBIL*)pixBuf)->c1 = (((RGBIL*)&runRGB)->c1 >> antiShift) & 0x00FF00FFL;
	#else

								if ( r->isComplex ) {
									RGBI* pix = pixBuf;
									for ( S32 j = x; j < limit; j++, pix++ ) {
										r->CalcColor(j, yd, pix);
	#ifdef __ANDROID__
										pix->blue = (pix->blue >>antiShift)& 0xFF;
										pix->green = (pix->green >>antiShift)& 0xFF;
										pix->red = (pix->red >>antiShift)& 0xFF;
										pix->alpha = (pix->alpha >>antiShift)& 0xFF;
	#else
										((RGBIL*)pix)->c0 = (((RGBIL*)pix)->c0 >> antiShift) & 0x00FF00FFL;
										((RGBIL*)pix)->c1 = (((RGBIL*)pix)->c1 >> antiShift) & 0x00FF00FFL;
	#endif
										pixBuf8[j-x]=PackPix32(pix);
									}

								} else {
									if ( !runRGBValid )	{
										r->CalcColor(xl, yd, &runRGB);
										runRGBValid = true;
									}
	#ifdef __ANDROID__
									pixBuf->blue = (runRGB.blue >> antiShift)& 0xFF;
									pixBuf->green = (runRGB.green >> antiShift)& 0xFF;
									pixBuf->red = (runRGB.red >> antiShift)& 0xFF;
									pixBuf->alpha = (runRGB.alpha >> antiShift)& 0xFF;
	#else
									((RGBIL*)pixBuf)->c0 = (((RGBIL*)&runRGB)->c0 >> antiShift) & 0x00FF00FFL;
									((RGBIL*)pixBuf)->c1 = (((RGBIL*)&runRGB)->c1 >> antiShift) & 0x00FF00FFL;
	#endif
									pixBuf8[0]=PackPix32(pixBuf);
	#endif
									//RGBI* pix = pixBuf+1;
									XU32*pix=pixBuf8+1;
									for ( S32 j = x+1; j < limit; j++, pix++ )
										*pix=pixBuf8[0];
										//*pix = pixBuf[0];
								}
	#ifndef _DIRECTDRAW_
								if ( r->nColors == antiStep && !r->transparent )
									drawRGBSlab(this, x, limit, (RGB8*)pixBuf8);// there is no transparency - fast case
								else
									DrawRGBASlab(x, limit, (RGB8*)pixBuf8);
	#endif

								x = limit;
							} while ( x < xr );
		 				}

						#ifdef ENABLE_MASK
						if ( maskBits )
							SetMask(xl, xr);
						#endif
		 			}
		 			if ( xrf ) {
		 				// Handle the trailing fraction
						if ( r->isComplex || !runRGBValid )
							r->CalcColor(xr, yd, &runRGB);
		 				pixel.Add(&runRGB, r->nColors, xr, xrf);
		 			}
		 		}
			}
		}

		// Advance
		next = r->next;
		FreeRun(r);
		r = next;
	}
	pixel.Paint();	// Flush the pixel...
}


//
// Main Paint routine
//


void XRaster::PaintBits(bool bBlack)
{
	//return;
//	_ANALYENTER("PaintBits");
	XLock lock(&dibLock);
	if(!lock.Lock(XSHORTTIME)) return;
	bits->Flush();

	if(bBlack)
		doGetBackground = GetBackgroundBlack;
	else if ( getBackground ) {
#ifdef _DIRECTDRAW_
		doGetBackground = GetBackgroundNULL;
#else
		doGetBackground = GetBackgroundProcs[pixelFormat];
#endif
	} else {
		doGetBackground = GetBackgroundWhite;
	}

	// Paint the scanlines
#ifdef _MultiThread_
	y=edgeClip.ymin;//XDibSrv::instance->Raster(this);
	for ( ; y < edgeClip.ymax; y++ ) {
		AddActive();
		SortActive();
		PaintActive();
		if ( antialias_b && (y & antiMask) == antiMask )
		{
    		PaintAARuns();
		}

	//	if ( (y & 0x1f) == 0 ) {
	//		Spin();
	// 			#if defined(USE_MMX) || defined(USE_KATMAI)
	// 			if ( gUseMMX || gUseKatmai ) {
	// 				_asm EMMS
	// 			}
	// 			#endif
	//			PollSound();
	//	}
	}
	
#else
	for ( y = edgeClip.ymin; y < edgeClip.ymax; y++ ) {
		AddActive();
		SortActive();
		PaintActive();
		if ( antialias_b && (y & antiMask) == antiMask )
		{
	    	PaintAARuns();
		}

		if ( (y & 0x1f) == 0 ) {
			Spin();
// 			#if defined(USE_MMX) || defined(USE_KATMAI)
// 			if ( gUseMMX || gUseKatmai ) {
// 				_asm EMMS
// 			}
// 			#endif
//			PollSound();
		}
	}
#endif
	{// Free any excess active edges
		for ( RActiveEdge* a = firstActive; a; ) {
			RActiveEdge* next = a->next;
			FreeActiveEdge(a);
			a = next;
		}
		firstActive = 0;
	}
	delete [] ditherData;
	ditherData = 0;
	lock.UnLock();
// 	#if defined(USE_MMX) || defined(USE_KATMAI)
// 	if ( gUseMMX || gUseKatmai ) {
// 		_asm EMMS
// 	}
// 	#endif
}

void XRaster::Flush()
{
	if ( needFlush ) {
		PaintBits();
		BeginPaint();
		getBackground = true;
	}
}


//
// Methods for setting the actual bits
//

void XRaster::SetYCoord(S32 yCoord)
{
//	FLASHASSERT(y >= bitClip.ymin && y < bitClip.ymax);

	bitY = yCoord;

	if ( inverted ) {
		// The rows are inverted for a Windows DIB
		rowAddr = baseAddr + (bitHeight-(bitY+1))*rowBytes;
	} else {
		// We are using a standard row order
		rowAddr = baseAddr + bitY*rowBytes;
	}

	#ifdef ENABLE_MASK
	if ( maskBits ) {
		if ( maskBits->m_inverted ) {
			// The rows are inverted for a Windows DIB
			maskRowAddr = (char  *)maskBits->m_baseAddr + (bitHeight-(bitY+1))*maskBits->m_rowBytes;
		} else {
			// We are using a standard row order
			maskRowAddr = (char  *)maskBits->m_baseAddr + bitY*maskBits->m_rowBytes;
		}
	}
	#endif

	if ( ditherData ) {
		Swap(curDitherErr, nextDitherErr, void*);
		memset(nextDitherErr, 0, ditherErrSize);
	}
}


//
// Dither Methods
//


//enum { solidPat = 0, disablePat = 1, highlightPat = 2, lineHighlightPat = 3 };
void ApplyRGBPattern(int pattern, int y, int xmin, int xmax, RGBI* pix)
{
	switch ( pattern ) {
		case disablePat:{
			RGBI* p = pix;
			for ( int i = xmax-xmin; i--; p++ ) {
				p->alpha = 255;//p->red/2 + 128;
				p->red   = (p->red>>1) + 128;
				p->green = (p->green>>1) + 128;
				p->blue  = (p->blue>>1) + 128;
			}
		} break;

		/*case highlightPat:{
			int j = (y & 0x2) + xmin;
			RGBI* p = pix;
			for ( int i = xmax-xmin; i--; p++, j++ ) {
				if ( j & 0x2 ) {
					p->alpha = 255;//p->red/2 + 128;
					p->red   = p->red ^ 0xFF;
					p->green = p->green ^ 0xFF;
					p->blue  = p->blue ^ 0xFF;
				}
			}
		} break;*/

		case highlightPat:
		case lineHighlightPat:{
			int mask = (pattern == highlightPat ? fillPatBits : linePatBits) >> 4*(y&0x3);
			RGBI* p = pix;
			for ( int i = xmin; i < xmax; p++, i++ ) {
				if ( (1 << (i&3)) & mask ) {
					// Pick the "opposite" color
					p->alpha = 255;//p->red/2 + 128;
					p->red   = p->red ^ 0xFF;
					p->green = p->green ^ 0xFF;
					p->blue  = p->blue ^ 0xFF;
				}
			}
		} break;
	}
}


//
// DrawRGBSlab Procs
//




// #if !defined(SPRITE_XTRA) || !defined(_MAC)
// #pragma warning( disable : 4799 )  // Disable "no EMMS instruction" warning messages
// #endif
inline void DrawRGBSlab32(XRaster* r, S32 xmin, S32 xmax, RGB8* pix)
{
	U32 * dst = (U32 *)r->rowAddr + xmin;// + r->xorg;
	for ( S32 i = xmax-xmin; i--; pix++, dst++ ) {
		*dst = *((XU32*)pix);
			//PackPix32(pix);
	}
}

#ifdef SMOOTHBITS
#ifdef _ARGB_PIXEL_
inline U32 MakeWideColor(RGB8 c)
{
	return ((U32)c.blue << (21)) | ((U32)c.green << 10) | ((U32)c.red>>1);
}

inline void WideToRGB8(U32 pixW, RGB8* pix)
{
	pix->alpha = 255;
	pix->blue   = (int)(pixW >> 24);
	pix->green = (U8)(pixW >> 13);
	pix->red  = (U8)(pixW >> 2 );
}
#else
inline U32 MakeWideColor(RGB8 c)
{
	return ((U32)c.red << (21)) | ((U32)c.green << 10) | ((U32)c.blue>>1);
}



inline void WideToRGB8(U32 pixW, RGB8* pix)
{
	pix->alpha = 255;
	pix->red   = (int)(pixW >> 24);
	pix->green = (U8)(pixW >> 13);
	pix->blue  = (U8)(pixW >> 2 );
}
#endif

inline U32 Pix16ToWide(U32 pix)
{
	return ((U32)pix16Expand[(pix>>10)&0x1f] << 21) |
		   ((U32)pix16Expand[(pix>> 5)&0x1f] << 10) |
		   (pix16Expand[(pix    )&0x1f] >> 1 );
}

inline U32 Pix32ToWide(U32 pix)
{
	return ((pix & 0xFF0000L)<<5) |
		   ((pix &   0xFF00L)<<2) |
		   ((pix &     0xFFL)>>1);
}

void Blt8to32S(BltInfo bi, SPOINT& pt, int n, RGB8* pix)
{
	while ( n-- ) {
		// Get the first row
		int* cv = PixCoverage[(U16)pt.x>>13][(U16)pt.y>>13].ce;

		int xi = pt.x >>XBITMAPOFF;
		int yi = pt.y >>XBITMAPOFF;
		S32 xInc = xi < bi.width-1 ? 1 : 1-bi.width;
		U32 pixW;
		U8 * bits = bi.baseAddr + yi*bi.rowBytes + xi;
		pixW  = MakeWideColor(bi.colors[bits[0]]) * cv[0];
		pixW += MakeWideColor(bi.colors[bits[xInc]]) * cv[1];

		// Get the second row
		bits += yi < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height);
		pixW += MakeWideColor(bi.colors[bits[0]]) * cv[2];
		pixW += MakeWideColor(bi.colors[bits[xInc]]) * cv[3];

		WideToRGB8(pixW, pix);
		pix++;

		// Advance
		pt.x += bi.dx;
		pt.y += bi.dy;
	}
}

void Blt16to32S(BltInfo bi, SPOINT& pt, int n, RGB8* pix)
{
	while ( n-- ) {
		// Get the first row
		int* cv = PixCoverage[(U16)pt.x>>13][(U16)pt.y>>13].ce;

		int x = pt.x >>XBITMAPOFF;
		int y = pt.y >>XBITMAPOFF;
		S32 xInc = x < bi.width-1 ? 1 : 1-bi.width;
		U32 pixW;
		U16 * bits = (U16 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes) + x;
		pixW  = Pix16ToWide(bits[0]) * cv[0];
		pixW += Pix16ToWide(bits[xInc]) * cv[1];

		// Get the second row
		bits = (U16*)((U8*)bits + (y < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height)));
		pixW += Pix16ToWide(bits[0]) * cv[2];
		pixW += Pix16ToWide(bits[xInc]) * cv[3];

		WideToRGB8(pixW, pix);
		pix++;

		// Advance
		pt.x += bi.dx;
		pt.y += bi.dy;
	}
}

void Blt32to32S(BltInfo bi, SPOINT& pt, int n, RGB8* pix)
{
	while ( n-- ) {
		// Get the first row
		int* cv = PixCoverage[(U16)pt.x>>13][(U16)pt.y>>13].ce;

		int x = (pt.x) >>XBITMAPOFF;
		int y = (pt.y) >>XBITMAPOFF;
		S32 xInc = x < bi.width-1 ? 1 : 1-bi.width;
		U32 pixW;
		U32 * bits = (U32 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes) + x;
		pixW  = Pix32ToWide(bits[0]) * cv[0];
		pixW += Pix32ToWide(bits[xInc]) * cv[1];

		// Get the second row
		bits = (U32 *)((U8 *)bits + (y < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height)));
		pixW += Pix32ToWide(bits[0]) * cv[2];
		pixW += Pix32ToWide(bits[xInc]) * cv[3];

		WideToRGB8(pixW, pix);
		pix++;

		// Advance
		pt.x += bi.dx;
		pt.y += bi.dy;
	}
}

void Blt8to32SA(BltInfo bi, SPOINT& pt, int n, RGB8* pix)
{
	while ( n-- ) {
		// Get the first row
		int* cv = PixCoverage[(U16)pt.x>>13][(U16)pt.y>>13].ce;

		int xi = pt.x >>XBITMAPOFF;
		int yi = pt.y >>XBITMAPOFF;
		S32 xInc = xi < bi.width-1 ? 1 : 1-bi.width;
		RGBIU p;
		RGBIU pp;
		U8 * bits = bi.baseAddr + yi*bi.rowBytes + xi;
		ExpandColor(bi.colors[bits[0]], &p.i);
		p.l.c0 *= cv[0];
		p.l.c1 *= cv[0];
		ExpandColor(bi.colors[bits[xInc]], &pp.i);
		p.l.c0 += pp.l.c0*cv[1];
		p.l.c1 += pp.l.c1*cv[1];

		// Get the second row
		bits += yi < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height);
		ExpandColor(bi.colors[bits[0]], &pp.i);
		p.l.c0 += pp.l.c0*cv[2];
		p.l.c1 += pp.l.c1*cv[2];
		ExpandColor(bi.colors[bits[xInc]], &pp.i);
		p.l.c0 += pp.l.c0*cv[3];
		p.l.c1 += pp.l.c1*cv[3];

		p.l.c0>>=3;
		p.l.c1>>=3;

		pix->alpha=((RGBI*)&p.l)->alpha;
		pix->red=((RGBI*)&p.l)->red;
		pix->green=((RGBI*)&p.l)->green;
		pix->blue=((RGBI*)&p.l)->blue;

		//((RGBIL*)pix)->c0 = (p.l.c0>>3) & 0x00FF00FF;
		//((RGBIL*)pix)->c1 = (p.l.c1>>3) & 0x00FF00FF;
		pix++;

		// Advance
		pt.x += bi.dx;
		pt.y += bi.dy;
	}
}

void Blt16to32SA(BltInfo bi, SPOINT& pt, int n, RGB8* pix)
{
	while ( n-- ) {
		// Get the first row
		int* cv = PixCoverage[(U16)pt.x>>13][(U16)pt.y>>13].ce;

		int x = pt.x >>XBITMAPOFF;
		int y = pt.y >>XBITMAPOFF;
		S32 xInc = x < bi.width-1 ? 1 : 1-bi.width;
		RGBIU p;
		RGBIU pp;
		U16 * bits = (U16 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes) + x;
		UnpackPix16(bits[0], &p.i);
		p.l.c0 *= cv[0];
		p.l.c1 *= cv[0];
		UnpackPix16(bits[xInc], &pp.i);
		p.l.c0 += pp.l.c0*cv[1];
		p.l.c1 += pp.l.c1*cv[1];

		// Get the second row
		bits = (U16*)((U8*)bits + (y < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height)));
		UnpackPix16(bits[0], &pp.i);
		p.l.c0 += pp.l.c0*cv[2];
		p.l.c1 += pp.l.c1*cv[2];
		UnpackPix16(bits[xInc], &pp.i);
		p.l.c0 += pp.l.c0*cv[3];
		p.l.c1 += pp.l.c1*cv[3];

		p.l.c0>>=3;
		p.l.c1>>=3;

		pix->alpha=((RGBI*)&p.l)->alpha;
		pix->red=((RGBI*)&p.l)->red;
		pix->green=((RGBI*)&p.l)->green;
		pix->blue=((RGBI*)&p.l)->blue;
		//((RGBIL*)pix)->c0 = (p.l.c0>>3) & 0x00FF00FF;
		//((RGBIL*)pix)->c1 = (p.l.c1>>3) & 0x00FF00FF;
		pix++;

		// Advance
		pt.x += bi.dx;
		pt.y += bi.dy;
	}
}

void Blt32to32SA(BltInfo bi, SPOINT& pt, int n, RGB8* pix)
{
	while ( n-- ) {
		// Get the first row
		int* cv = PixCoverage[(U16)pt.x>>13][(U16)pt.y>>13].ce;

		int x = (pt.x) >>XBITMAPOFF;
		int y = (pt.y) >>XBITMAPOFF;
		S32 xInc = x < bi.width-1 ? 1 : 1-bi.width;
		RGBIU p;
		RGBIU pp;
		U32 * bits = (U32 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes) + x;
		UnpackPix32(bits[0], &p.i);
		p.l.c0 *= cv[0];
		p.l.c1 *= cv[0];
		UnpackPix32(bits[xInc], &pp.i);
		p.l.c0 += pp.l.c0*cv[1];
		p.l.c1 += pp.l.c1*cv[1];

		// Get the second row
		bits = (U32 *)((U8 *)bits + (y < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height)));
		UnpackPix32(bits[0], &pp.i);
		p.l.c0 += pp.l.c0*cv[2];
		p.l.c1 += pp.l.c1*cv[2];
		UnpackPix32(bits[xInc], &pp.i);
		p.l.c0 += pp.l.c0*cv[3];
		p.l.c1 += pp.l.c1*cv[3];

		p.l.c0>>=3;
		p.l.c1>>=3;
		pix->alpha=((RGBI*)&p.l)->alpha;
		pix->red=((RGBI*)&p.l)->red;
		pix->green=((RGBI*)&p.l)->green;
		pix->blue=((RGBI*)&p.l)->blue;
		//((RGBIL*)pix)->c0 = (p.l.c0>>3) & 0x00FF00FF;
		//((RGBIL*)pix)->c1 = (p.l.c1>>3) & 0x00FF00FF;
		pix++;

		// Advance
		pt.x += bi.dx;
		pt.y += bi.dy;
	}
}

#endif

void Blt8to32(BltInfo bi, SPOINT& pt, int n, U32 * dst)
{
	if ( bi.dy == 0 ) {
		U8 * src = bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes;
		while ( n-- ) {
			RGB8 c = bi.colors[*(src + (pt.x>>XBITMAPOFF))];
// #if defined(_UNIX)  && !defined(GENERATOR)
// 			*dst++ = MakePix32(c.red,c.green,c.blue);
// #else
			*dst++ = (U32)c.red   << 16 |
					 (U32)c.green << 8  |
						  c.blue | (c.alpha<<24);
// #endif
			pt.x += bi.dx;
		}

	} else {
		while ( n-- ) {
			RGB8 c = bi.colors[*(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes + (pt.x>>XBITMAPOFF))];
// #if defined(_UNIX) && !defined(GENERATOR)
// 			*dst++ = MakePix32(c.red,c.green,c.blue);
// #else
			*dst++ = (U32)c.red   << 16 |
					 (U32)c.green << 8  |
						  c.blue | (c.alpha<<24);
// #endif
			pt.x += bi.dx;
			pt.y += bi.dy;
		}
	}
}


// #endif


void Blt16to32(BltInfo bi, SPOINT& pt, int n, U32 * dst)
{
	if ( bi.dy == 0 ) {
		U16 * src = (U16 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes);
		while ( n-- ) {
			int pix = *(src + (pt.x>>XBITMAPOFF));
			*dst++ = Pix16To32(pix);
			pt.x += bi.dx;
		}

	} else {
		while ( n-- ) {
			int pix = *((U16 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes) + (pt.x>>XBITMAPOFF));
			*dst++ = Pix16To32(pix);
			pt.x += bi.dx;
			pt.y += bi.dy;
		}
	}
}

#include "splayer.h"
#define XBITMAPOFF1 14

void Blt32to32(BltInfo bi, SPOINT& pt, int n, U32 * dst)
{
	//_ANALYENTER("B32");
	/*static int nCount=0;
	nCount++;
	if((nCount&0x3fff)==0x3ff0)
	{
		XString16 strInt;
		strInt.FromInt(nCount);
		XSWFPlayer::m_pInstance->Trace(strInt);
	}*/

	if ( bi.dy == 0 ) {
		U32 * src = (U32 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes);
		if ( Abs(bi.dx-fixed_1) < 0x100 ) {
			src += ((pt.x)>>XBITMAPOFF);
			pt.x += n*bi.dx;
			memcpy(dst,src,n<<2);
			//while ( n-- )
			//	*dst++ = *src++;
		} else {
			/*int iOff=bi.dx>>XBITMAPOFF;
			if(iOff)
			{
				int dx=bi.dx;
				int dd=4;
				int dm=iOff*4;
				XU8* src0=(XU8*)src;
				union 
				{
					XU16 f[2];
					XU32 delta;
				}m;
				m.delta=pt.x&0xffff;
				if(dx<0) {dd=-4;dx=-dx;m.delta=0xffff-m.delta;}
				
				while(n--)
				{
					*dst++=*((XU32*)src0);
					m.delta+=dx;
					if(m.f[0])
					{
						m.f[0]=0;
						src0+=dd;
					}
					src0+=dm;
				}
			}
			else*/
			{
				register int xx=pt.x,dx=bi.dx;
				while ( n-- ) {

					//XU32* sss=src+(pt.x>>XBITMAPOFF);

					
					*dst++ = *((XU32*)((XU8*)src+((xx>>XBITMAPOFF1)&~3)));//0.96
					//*dst++ = *((XU32*)((XU8*)src+((pt.x>>XBITMAPOFF1)&~3)));//0.96
					//*dst++ = src[pt.x >>XBITMAPOFF];//1.03
							//*((XU32*)((XU8*)src+((pt.x>>XBITMAPOFF1)&~3)));
							//src[pt.x >>XBITMAPOFF];
					
					//pt.x += bi.dx;
					xx+=dx;
				}
				pt.x=xx;
			}
		}
	} 
	else {
		while ( n-- ) {
			*dst++ = *((U32 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes) + (pt.x>>XBITMAPOFF));
			pt.x += bi.dx;
			pt.y += bi.dy;
		}
	}
}


//
// General Blts
//

#ifdef SMOOTHBITS

// A wide pixel is a specially scaled pixel used for blending pixels when
//	transforming bitmaps - the top 3 bits are reserved for scaling
// 8+3 bits are reserved for red   - bits 21..31
// 8+3 bits are reserved for green - bits 10..20
// 7+3 bits are reserved for blue  - bits 0..9

inline void WideToRGBI(U32 pixW, RGBI* pix)
{
	pix->alpha = 255;
	pix->red   = (int)(pixW >> 24);
	pix->green = (U8)(pixW >> 13);
	pix->blue  = (U8)(pixW >> 2 );
}

void Blt8toIS(BltInfo bi, SPOINT& pt, int n, RGBI* pix)
{
	while ( n-- ) {
		// Get the first row
		int* cv = PixCoverage[(U16)pt.x>>13][(U16)pt.y>>13].ce;

		int xi = pt.x >>XBITMAPOFF;
		int yi = pt.y >>XBITMAPOFF;
		S32 xInc = xi < bi.width-1 ? 1 : 1-bi.width;
		U32 pixW;
		U8 * bits = bi.baseAddr + yi*bi.rowBytes + xi;
		pixW  = MakeWideColor(bi.colors[bits[0]]) * cv[0];
		pixW += MakeWideColor(bi.colors[bits[xInc]]) * cv[1];

		// Get the second row
		bits += yi < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height);
		pixW += MakeWideColor(bi.colors[bits[0]]) * cv[2];
		pixW += MakeWideColor(bi.colors[bits[xInc]]) * cv[3];

		WideToRGBI(pixW, pix);
		pix++;

		// Advance
		pt.x += bi.dx;
		pt.y += bi.dy;
	}
}

void Blt16toIS(BltInfo bi, SPOINT& pt, int n, RGBI* pix)
{
	while ( n-- ) {
		// Get the first row
		int* cv = PixCoverage[(U16)pt.x>>13][(U16)pt.y>>13].ce;

		int x = pt.x >>XBITMAPOFF;
		int y = pt.y >>XBITMAPOFF;
		S32 xInc = x < bi.width-1 ? 1 : 1-bi.width;
		U32 pixW;
		U16 * bits = (U16 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes) + x;
		pixW  = Pix16ToWide(bits[0]) * cv[0];
		pixW += Pix16ToWide(bits[xInc]) * cv[1];

		// Get the second row
		bits = (U16*)((U8*)bits + (y < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height)));
		pixW += Pix16ToWide(bits[0]) * cv[2];
		pixW += Pix16ToWide(bits[xInc]) * cv[3];

		WideToRGBI(pixW, pix);
		pix++;

		// Advance
		pt.x += bi.dx;
		pt.y += bi.dy;
	}
}

void Blt32toIS(BltInfo bi, SPOINT& pt, int n, RGBI* pix)
{
	while ( n-- ) {
		// Get the first row
		int* cv = PixCoverage[(U16)pt.x>>13][(U16)pt.y>>13].ce;

		int x = pt.x >>XBITMAPOFF;
		int y = pt.y >>XBITMAPOFF;
		S32 xInc = x < bi.width-1 ? 1 : 1-bi.width;
		U32 pixW;
		U32 * bits = (U32 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes) + x;
		pixW  = Pix32ToWide(bits[0]) * cv[0];
		pixW += Pix32ToWide(bits[xInc]) * cv[1];

		// Get the second row
		bits = (U32 *)((U8 *)bits + (y < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height)));
		pixW += Pix32ToWide(bits[0]) * cv[2];
		pixW += Pix32ToWide(bits[xInc]) * cv[3];

		WideToRGBI(pixW, pix);
		pix++;

		// Advance
		pt.x += bi.dx;
		pt.y += bi.dy;
	}
}

void Blt8toISA(BltInfo bi, SPOINT& pt, int n, RGBI* pix)
{
	while ( n-- ) {
		// Get the first row
		int* cv = PixCoverage[(U16)pt.x>>13][(U16)pt.y>>13].ce;
#ifdef _USE64BITS
		int xi = pt.x >>XBITMAPOFF;
		int yi = pt.y >>XBITMAPOFF;
		S32 xInc = xi < bi.width-1 ? 1 : 1-bi.width;
		RGBIU p;
		RGBIU pp;
		U8 * bits = bi.baseAddr + yi*bi.rowBytes + xi;
		ExpandColor(bi.colors[bits[0]], &p.i);
		p.l *= cv[0];
		//p.l.c1 *= cv[0];
		ExpandColor(bi.colors[bits[xInc]], &pp.i);
		p.l += pp.l*cv[1];
		//p.l.c1 += pp.l.c1*cv[1];

		// Get the second row
		bits += yi < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height);
		ExpandColor(bi.colors[bits[0]], &pp.i);
		p.l += pp.l*cv[2];
		ExpandColor(bi.colors[bits[xInc]], &pp.i);
		p.l += pp.l*cv[3];

		*((XU64*)pix)=(p.l>>3)&_MASK64;

		//((RGBIL*)pix)->c0 = (p.l.c0>>3) & 0x00FF00FF;
		//((RGBIL*)pix)->c1 = (p.l.c1>>3) & 0x00FF00FF;
		pix++;
#else
		int xi = pt.x >>XBITMAPOFF;
		int yi = pt.y >>XBITMAPOFF;
		S32 xInc = xi < bi.width-1 ? 1 : 1-bi.width;
		RGBIU p;
		RGBIU pp;
		U8 * bits = bi.baseAddr + yi*bi.rowBytes + xi;
		ExpandColor(bi.colors[bits[0]], &p.i);
		p.l.c0 *= cv[0];
		p.l.c1 *= cv[0];
		ExpandColor(bi.colors[bits[xInc]], &pp.i);
		p.l.c0 += pp.l.c0*cv[1];
		p.l.c1 += pp.l.c1*cv[1];

		// Get the second row
		bits += yi < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height);
		ExpandColor(bi.colors[bits[0]], &pp.i);
		p.l.c0 += pp.l.c0*cv[2];
		p.l.c1 += pp.l.c1*cv[2];
		ExpandColor(bi.colors[bits[xInc]], &pp.i);
		p.l.c0 += pp.l.c0*cv[3];
		p.l.c1 += pp.l.c1*cv[3];

		((RGBIL*)pix)->c0 = (p.l.c0>>3) & 0x00FF00FF;
		((RGBIL*)pix)->c1 = (p.l.c1>>3) & 0x00FF00FF;
		pix++;
#endif
		// Advance
		pt.x += bi.dx;
		pt.y += bi.dy;
	}
}

void Blt16toISA(BltInfo bi, SPOINT& pt, int n, RGBI* pix)
{
	while ( n-- ) {
		// Get the first row
		int* cv = PixCoverage[(U16)pt.x>>13][(U16)pt.y>>13].ce;

		int x = pt.x >>XBITMAPOFF;
		int y = pt.y >>XBITMAPOFF;
		S32 xInc = x < bi.width-1 ? 1 : 1-bi.width;
		RGBIU p;
		RGBIU pp;
		U16 * bits = (U16 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes) + x;
		UnpackPix16(bits[0], &p.i);
#ifdef _USE64BITS
		p.l *= cv[0];
		UnpackPix16(bits[xInc], &pp.i);
		p.l += pp.l*cv[1];
		// Get the second row
		bits = (U16*)((U8*)bits + (y < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height)));
		UnpackPix16(bits[0], &pp.i);
		p.l += pp.l*cv[2];
		UnpackPix16(bits[xInc], &pp.i);
		p.l += pp.l*cv[3];

		*((XU64*)pix)=(p.l>>3)&_MASK64;

#else
		p.l.c0 *= cv[0];
		p.l.c1 *= cv[0];
		UnpackPix16(bits[xInc], &pp.i);
		p.l.c0 += pp.l.c0*cv[1];
		p.l.c1 += pp.l.c1*cv[1];

		// Get the second row
		bits = (U16*)((U8*)bits + (y < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height)));
		UnpackPix16(bits[0], &pp.i);
		p.l.c0 += pp.l.c0*cv[2];
		p.l.c1 += pp.l.c1*cv[2];
		UnpackPix16(bits[xInc], &pp.i);
		p.l.c0 += pp.l.c0*cv[3];
		p.l.c1 += pp.l.c1*cv[3];

		((RGBIL*)pix)->c0 = (p.l.c0>>3) & 0x00FF00FF;
		((RGBIL*)pix)->c1 = (p.l.c1>>3) & 0x00FF00FF;
#endif
		pix++;

		// Advance
		pt.x += bi.dx;
		pt.y += bi.dy;
	}
}

void Blt32toISA(BltInfo bi, SPOINT& pt, int n, RGBI* pix)
{
	while ( n-- ) {
		// Get the first row
		int* cv = PixCoverage[(U16)pt.x>>13][(U16)pt.y>>13].ce;

		int x = pt.x >>XBITMAPOFF;
		int y = pt.y >>XBITMAPOFF;
		S32 xInc = x < bi.width-1 ? 1 : 1-bi.width;
		RGBIU p;
		RGBIU pp;
		U32 * bits = (U32 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes) + x;
		UnpackPix32(bits[0], &p.i);
#ifdef _USE64BITS
		p.l *= cv[0];
		UnpackPix32(bits[xInc], &pp.i);
		p.l += pp.l*cv[1];

		// Get the second row
		bits = (U32 *)((U8 *)bits + (y < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height)));
		UnpackPix32(bits[0], &pp.i);
		p.l += pp.l*cv[2];
		UnpackPix32(bits[xInc], &pp.i);
		p.l += pp.l*cv[3];

		*((XU64*)pix) =(p.l>>3)&_MASK64;

#else
		p.l.c0 *= cv[0];
		p.l.c1 *= cv[0];
		UnpackPix32(bits[xInc], &pp.i);
		p.l.c0 += pp.l.c0*cv[1];
		p.l.c1 += pp.l.c1*cv[1];

		// Get the second row
		bits = (U32 *)((U8 *)bits + (y < bi.height-1 ? bi.rowBytes : bi.rowBytes*(1-bi.height)));
		UnpackPix32(bits[0], &pp.i);
		p.l.c0 += pp.l.c0*cv[2];
		p.l.c1 += pp.l.c1*cv[2];
		UnpackPix32(bits[xInc], &pp.i);
		p.l.c0 += pp.l.c0*cv[3];
		p.l.c1 += pp.l.c1*cv[3];

		((RGBIL*)pix)->c0 = (p.l.c0>>3) & 0x00FF00FF;
		((RGBIL*)pix)->c1 = (p.l.c1>>3) & 0x00FF00FF;
#endif
		pix++;

		// Advance
		pt.x += bi.dx;
		pt.y += bi.dy;
	}
}


#endif			//SMOOTHBITS

void BltXtoI(BltInfo bi, SPOINT& pt, int n, RGB8* pix)
{
	FLASHASSERT(false);
}


#ifdef _REFECT_

inline int Refect(int v,int m)
{
	while(v<0||v>=m)
	{
		if(v<0) v=-v;
		else if(v>=m)
			v=m*2-v;
	}
	//if(v<0) v=-v;
	//v=v%m;
	//if(v<0) v=0;
	//else if(v>=m) v=m-1;
	return v;
}

inline void CalcLimitRefect(int& n, S32& x, S32 dx, S32 limit)
{
	int lim;
	if ( dx > 0 ) {
		//while ( x >= limit )
		//	x -= limit;
		x=Refect(x,limit);
		lim = (int)((limit-x+dx-1)/dx);
		if ( n > lim )
			n = lim;
	} else if ( dx < 0 ) {
		//while ( x <= 0 )
		//	x += limit;
		x=Refect(x,limit);
		lim = (int)((x-dx-1)/-dx);
		if ( n > lim )
			n = lim;
	}
}

#endif

void DrawBitmapSlab(_XRColor* color, S32 xmin, S32 xmax)
{
	if(!color->bm.bi.baseAddr) return;
	if(!color->bm.bitmap) return;

	XRaster* r = color->raster;

	// Calc the start point in bitmap src coordinates
	SPOINT pt;
	pt.x = (xmin << XBITMAPOFF)+HALFOFF;
	pt.y = r->bitY << XBITMAPOFF;
	MatrixTransformPoint(&color->bm.invMat, &pt, &pt);

	// Set up the slab start point
	S32 wLimit = (S32)color->bm.bi.width<<XBITMAPOFF;
	S32 hLimit = (S32)color->bm.bi.height<<XBITMAPOFF;
	if ( color->bm.bitsStyle & fillBitsClip ) {
		// Treat the edges of a cliped bitmap special to prevent rounding problems
		// Check for out of range values
#ifndef _REFECT_
		RGB8 pix;
		int x,y;

		//pt.x=Refect(pt.x,wLimit);
		//pt.y=Refect(pt.y,hLimit);

		SBitmapCore* bitmap = color->bm.bitmap;
		int w = bitmap->width;
		int h = bitmap->height;
		if ( color->bm.smooth ) {
			h--; w--;		// allow for the extra row and column when getting 4 samples
		}

		SPOINT tail;
		S32 count = xmax-xmin;
		tail.x = pt.x + color->bm.bi.dx*count;
		tail.y = pt.y + color->bm.bi.dy*count;

		// Adjust the head
		for (;;) {
			x = (int)(pt.x>>XBITMAPOFF);
			y = (int)(pt.y>>XBITMAPOFF);
			if ( x >= 0 && y >= 0 && x < w && y < h ) break;
			if ( xmin >= xmax ) break;
			#ifdef SMOOTHBITS
			if ( color->bm.smooth )
				bitmap->GetSSRGBPixel8(pt.x, pt.y, &pix);
			else
			#endif
				bitmap->GetRGBPixel8(x, y, &pix);

			if ( color->bm.colorMap )
				ApplyColorMap8(color->bm.colorMap, &pix);
			r->drawRGBSlab(r, xmin, xmin+1, &pix);

			pt.x += color->bm.bi.dx;
			pt.y += color->bm.bi.dy;
			xmin++;
		}

		// Adjust the tail
		for (;;) {
			x = (int)(tail.x>>XBITMAPOFF);
			y = (int)(tail.y>>XBITMAPOFF);
			if ( x >= 0 && y >= 0 && x < w && y < h ) break;
			if ( xmin >= xmax ) break;
			#ifdef SMOOTHBITS
			if ( color->bm.smooth )
				bitmap->GetSSRGBPixel8(tail.x, tail.y, &pix);
			else
			#endif
				bitmap->GetRGBPixel8(x, y, &pix);
			xmax--;
			if ( color->bm.colorMap )
				ApplyColorMap8(color->bm.colorMap, &pix);
			r->drawRGBSlab(r, xmax, xmax+1, &pix);

			tail.x -= color->bm.bi.dx;
			tail.y -= color->bm.bi.dy;
		}
#else
		pt.y=Refect(pt.y,hLimit);


		while ( xmin < xmax ) {
			RGB8 pixBuf[RGBSlabChunkSize];

			// Expand the pixels to RGB values
			int n = Min((int)(xmax-xmin), RGBSlabChunkSize);
			CalcLimitRefect(n, pt.x, color->bm.bi.dx, wLimit);	// limit the slab size so we don't overwrap textures
			CalcLimitRefect(n, pt.y, color->bm.bi.dy, hLimit);

			color->bm.bltProc(color->bm.bi, pt, n, pixBuf);

			// Apply a color transform
			if ( color->bm.colorMap )
				ApplyColorMap(color->bm.colorMap, pixBuf, n);

			// Apply a highlight pattern...
			//if ( color->pattern )
			//	ApplyRGBPattern(color->pattern, r->bitY, xmin, xmin+n, pixBuf);

			// Draw the RGB values
			r->drawRGBSlab(r, xmin, xmin+n, pixBuf);
			xmin += n;
		}
		return;
#endif
		//pt.x = LimitAbs(pt.x, wLimit);
		//pt.y = LimitAbs(pt.y, hLimit);
		//return;
		//return;
		//int di=-w;
		//if(pt.x>=w
		//pt.y=Refect(pt.y,hLimit);
		//if(pt.x<0) pt.x=0;
		//else if(pt.x>=wLimit) pt.x=wLimit-1;
		//pt.x = LimitAbs(pt.x, wLimit);

	} else {
		// Do an initial coordinate wrap for textures
		pt.x = LimitAbs(pt.x, wLimit);
		pt.y = LimitAbs(pt.y, hLimit);
	}

	if ( color->bm.fastBltProc ) {
		// Draw the slab for the fast case
// 		int pixSize = mapPixelFormatToDepth[r->pixelFormat]/8;
		int pixSize = pixelSize[r->pixelFormat];
			//NativeDisplayTester::PixelFormatToDepth( r->pixelFormat ) / 8;
		FLASHASSERT(pixSize > 0);

		//xmin += r->xorg;
		//xmax += r->xorg;

		while ( xmin < xmax ) {
			// Expand the pixels to RGB values
			int n = Min((int)(xmax-xmin), RGBSlabChunkSize);
			CalcLimit(n, pt.x, color->bm.bi.dx, wLimit);	// limit the slab size so we don't overwrap textures
			CalcLimit(n, pt.y, color->bm.bi.dy, hLimit);
			color->bm.fastBltProc(color->bm.bi, pt, n, r->rowAddr + xmin*pixSize);
			xmin += n;
		}
	} else {
		// Draw the slab for the general case
		while ( xmin < xmax ) {
#ifdef _DIRECTDRAW_
			RGB8*pixBuf=(RGB8*)_ROWADDR(r,xmin);

			// Expand the pixels to RGB values
			int n = Min((int)(xmax-xmin), RGBSlabChunkSize);
			CalcLimit(n, pt.x, color->bm.bi.dx, wLimit);	// limit the slab size so we don't overwrap textures
			CalcLimit(n, pt.y, color->bm.bi.dy, hLimit);

			color->bm.bltProc(color->bm.bi, pt, n, pixBuf);

			// Apply a color transform
			if ( color->bm.colorMap )
				ApplyColorMap(color->bm.colorMap, pixBuf, n);

			// Apply a highlight pattern...

#else
			RGB8 pixBuf[RGBSlabChunkSize];

			// Expand the pixels to RGB values
			int n = Min((int)(xmax-xmin), RGBSlabChunkSize);
			CalcLimit(n, pt.x, color->bm.bi.dx, wLimit);	// limit the slab size so we don't overwrap textures
			CalcLimit(n, pt.y, color->bm.bi.dy, hLimit);

			color->bm.bltProc(color->bm.bi, pt, n, pixBuf);

			// Apply a color transform
			if ( color->bm.colorMap )
				ApplyColorMap(color->bm.colorMap, pixBuf, n);

			// Apply a highlight pattern...
			//if ( color->pattern )
			//	ApplyRGBPattern(color->pattern, r->bitY, xmin, xmin+n, pixBuf);

			// Draw the RGB values
			r->drawRGBSlab(r, xmin, xmin+n, pixBuf);
#endif
			xmin += n;
		}
	}
}

void BuildBitmapSlab(_XRColor* color, S32 xmin, S32 xmax, RGB8* buf)
{
	// Calc the start point in bitmap src coordinates
	if(!color->bm.bi.baseAddr) return;
	if(!color->bm.bitmap) return;

	SPOINT pt;
	pt.x = (xmin <<XBITMAPOFF)+HALFOFF;
	pt.y = color->raster->bitY <<XBITMAPOFF;
	MatrixTransformPoint(&color->bm.invMat, &pt, &pt);

	// Set up the slab start point
	S32 wLimit = (S32)color->bm.bi.width<<XBITMAPOFF;
	S32 hLimit = (S32)color->bm.bi.height<<XBITMAPOFF;
	if ( color->bm.bitsStyle & fillBitsClip ) {
		// Treat the edges of a clipped bitmap special to prevent rounding problems
		// Check for out of range values
#ifndef _REFECT_
		int x,y;

		SBitmapCore* bitmap = color->bm.bitmap;
		int w = bitmap->width;
		int h = bitmap->height;
		if ( color->bm.smooth ) {
			h--; w--;		// allow for the extra row and column when getting 4 samples
		}

		SPOINT tail;
		S32 count = xmax-xmin;
		tail.x = pt.x + color->bm.bi.dx*count;
		tail.y = pt.y + color->bm.bi.dy*count;

		// Adjust the head
		for (;;) {
			x = (int)(pt.x>>XBITMAPOFF);
			y = (int)(pt.y>>XBITMAPOFF);
			if ( x >= 0 && y >= 0 && x < w && y < h ) break;
			if ( xmin >= xmax ) break;
			#ifdef SMOOTHBITS
			if ( color->bm.smooth )
				bitmap->GetSSRGBPixel8(pt.x, pt.y, buf);
			else
			#endif
				bitmap->GetRGBPixel8(x, y, buf);

			if ( color->bm.colorMap )
				ApplyColorMap8(color->bm.colorMap, buf);

			pt.x += color->bm.bi.dx;
			pt.y += color->bm.bi.dy;
			xmin++;
			buf++;
		}

		// Adjust the tail
		for (;;) {
			x = (int)(tail.x>>XBITMAPOFF);
			y = (int)(tail.y>>XBITMAPOFF);
			if ( x >= 0 && y >= 0 && x < w && y < h ) break;
			if ( xmin >= xmax ) break;
			RGB8* pix = buf+xmax-xmin-1;
			#ifdef SMOOTHBITS
			if ( color->bm.smooth )
				bitmap->GetSSRGBPixel8(tail.x, tail.y, pix);
			else
			#endif
				bitmap->GetRGBPixel8(x, y, pix);
			xmax--;
			if ( color->bm.colorMap )
				ApplyColorMap8(color->bm.colorMap, pix);

			tail.x -= color->bm.bi.dx;
			tail.y -= color->bm.bi.dy;
		}
#else
		pt.y=Refect(pt.y,hLimit);
		while ( xmin < xmax ) {
		// Expand the pixels to RGB values
		//int n = Min((int)(xmax-xmin), RGBSlabChunkSize);
			int n = (int)(xmax-xmin);
			CalcLimitRefect(n, pt.x, color->bm.bi.dx, wLimit);	// limit the slab size so we don't overwrap textures
			CalcLimitRefect(n, pt.y, color->bm.bi.dy, hLimit);

			color->bm.bltProc(color->bm.bi, pt, n, buf);

			// Apply a color transform
			if ( color->bm.colorMap )
				ApplyColorMap(color->bm.colorMap, buf, n);

			xmin += n;
			buf += n;
		}
		return;
#endif
	} else {
		// Do an initial coordinate wrap for textures
		pt.x = LimitAbs(pt.x, wLimit);
		pt.y = LimitAbs(pt.y, hLimit);
	}

	// Draw the slab for the general case
	while ( xmin < xmax ) {
		// Expand the pixels to RGB values
		//int n = Min((int)(xmax-xmin), RGBSlabChunkSize);
		int n = (int)(xmax-xmin);
		CalcLimit(n, pt.x, color->bm.bi.dx, wLimit);	// limit the slab size so we don't overwrap textures
		CalcLimit(n, pt.y, color->bm.bi.dy, hLimit);

		color->bm.bltProc(color->bm.bi, pt, n, buf);

		// Apply a color transform
		if ( color->bm.colorMap )
			ApplyColorMap(color->bm.colorMap, buf, n);

		xmin += n;
		buf += n;
	}
}


//DOT FONT

/*void GetFontData(_XRColor*color,int y,S32 xmin,S32 xmax,RGB8*buf)
{
	int n=xmax-xmin;
	memset(buf,0,n<<2);
	if(!color->fm.pEdit) return;
	int y=color->raster->bitY;
	if(y<color->fm.devRect.ymin||
       y>=color->fm.devRect.ymax) return;
}*/

void BuildSolidSlab(_XRColor* color, S32 xmin, S32 xmax, RGBI* pix)
{
	int n = xmax-xmin;
	while ( n-- > 0 ) {
		*pix++ = color->rgb;
	}
}

void CalcGradientPixel(_XRColor* color, S32 x, RGBI* pix)
{
	SPOINT pt;
	pt.x = x<<8;
	pt.y = color->raster->bitY<<8;
	MatrixTransformPoint(&color->grad.invMat, &pt, &pt);

	RGB8* ramp = color->grad.colorRamp;
	if ( !ramp ) return;

	S32 intensity;
	if ( color->grad.gradStyle == fillRadialGradient ) {
		// Radial gradient
		intensity = PointLength(&pt) >> (6+8);
		if ( intensity > 256 )
			intensity = 256;
		else if ( intensity < 0 )  // this can happen in overflow conditions
			intensity = 0;
	} else {
		// Linear gradient
		intensity = (pt.x >> (6+8+1)) + 128;
		if ( intensity > 256 )
			intensity = 256;
		else if ( intensity < 0 )
			intensity = 0;
	}

	ExpandColor(ramp[intensity], pix);
}

void BuildLinearGradientSlab(_XRColor* color, S32 xmin, S32 xmax, RGB8* pix)
{
	RGB8* ramp = color->grad.colorRamp;
	if ( !ramp ) return;

	SPOINT pt;
	pt.x = xmin<<8;
	pt.y = color->raster->bitY<<8;
	MatrixTransformPoint(&color->grad.invMat, &pt, &pt);

	// Linear gradient
	SFIXED dx = color->grad.invMat.a>>8;
	S32		n = xmax-xmin;

	XU8 sp=color->grad.ramp.spreadMode;

	while ( n-- ) {
		S32 intensity = (pt.x >> (6+8+1)) + 128;
		if ( intensity > 256 )
			intensity = 256;
		else if ( intensity < 0 )
			intensity = 0;
//		GradientFmt(sp,intensity);

		// Convert from RGB8 to RGBI
		//
		*pix=ramp[intensity];
		//pix->alpha = ramp[intensity].alpha;
		//pix->red   = ramp[intensity].red;
		//pix->green = ramp[intensity].green;
		//pix->blue  = ramp[intensity].blue;

		// Advance
		pt.x += dx;
		pix++;
	}
}

void BuildRadialGradientSlab(_XRColor* color, S32 xmin, S32 xmax, RGB8* pix)
{
	S32 n = xmax-xmin;

	SPOINT pt;
	pt.x = xmin<<8;
	pt.y = color->raster->bitY<<8;
	MatrixTransformPoint(&color->grad.invMat, &pt, &pt);

	RGB8* ramp = color->grad.colorRamp;
	if ( !ramp ) return;

	SFIXED dx = color->grad.invMat.a>>8;
	SFIXED dy = color->grad.invMat.b>>8;

	// Draw the slab
	// Expand the pixels to RGB values
	// Radial gradient
	// i = sqrt(x^2 + y^2)
	S32 t, t2, i2;
	S32 i, previ;
	U16* sq;
	S32 step = 0;
	i = PointLength(&pt) >> (6+8);	// get a good start
	if ( i > 256 )
	{
		i = 256;
	}

	while ( n-- ) {
		t = pt.x>>(22-8);	// get the coords into the -256..256 range
		i2 = t*t;
		t2 = pt.y>>(22-8);
		i2 += t2*t2;

		// Find the square root of i2
		// Increment by the same number of entries as the previous step
		// I think this search is almost guarenteed to find a pixel in less than two cycles
		//	it averages about 1.5 searches per pixel
		previ = i;
		i += step;
		if ( i < 0 ) i = 0;
		else if ( i > 256 ) i = 256;
		sq = Sq256+i;
		for (;;) {
			if ( (U32)i2 < sq[0] ) {// Cast to unsigned to handle overflows more gracefully
				// Move down an entry
				i--;
				sq--;
			} else if ( (U32)i2 > sq[1] ) {
				// Move up an entry
				if ( i >= 256 )
					break;
				i++;
				sq++;
			} else {
				// We are bracketed
				break;
			}
		}
		step = i-previ;
		*pix=ramp[i];
		//ExpandColor(ramp[i], pix);
		//i = PointLength(&pt) >> (6+8);
		//*pix = ramp[i < 256 ? i : 256];

		// Advance
		pt.x += dx;
		pt.y += dy;
		pix++;
	}
}


inline void BuildGradientSlab(_XRColor* color, S32 xmin, S32 xmax, RGB8* pix)
{

	if(color->grad.gradStyle == fillRadialGradient) {
		BuildRadialGradientSlab(color, xmin, xmax, pix);
	} else {
		BuildLinearGradientSlab(color, xmin, xmax, pix);
	}

}

void DrawGradientSlab(_XRColor* color, S32 xmin, S32 xmax)
{
	XRaster* r = color->raster;

	// Draw the slab
	while ( xmin < xmax ) {
#ifdef _DIRECTDRAW_
		RGB8* pixBuf=(RGB8*)_ROWADDR(r,xmin);

		S32 limit = Min(xmax, xmin+RGBSlabChunkSize);

		BuildGradientSlab(color, xmin, limit, pixBuf);
		
#else
		RGB8 pixBuf[RGBSlabChunkSize];

		S32 limit = Min(xmax, xmin+RGBSlabChunkSize);

// #ifdef USE_KATMAI
// 		if(gUseKatmai)
// 			BuildGradientSlab_Katmai(color, xmin, limit, pixBuf);
// 		else
// #endif
		BuildGradientSlab(color, xmin, limit, pixBuf);

		// Apply a highlight pattern...
		//if ( color->pattern )
		//	ApplyRGBPattern(color->pattern, r->bitY, xmin, limit, pixBuf);

		r->drawRGBSlab(r, xmin, limit, pixBuf);
#endif
		xmin = limit;
	}
}


//
// Composite Draw Procs
//

void CompositeSolidSlab(_XRColor* color, S32 xmin, S32 xmax, RGB8* buf)
{
	FLASHASSERT(xmax-xmin<=RGBSlabChunkSize);
	XU32 rgb=PackPix32(&color->rgb);
	CompositeRGBSolid8((RGB8*)&rgb, buf, xmax-xmin);
}

void CompositeGradientSlab(_XRColor* color, S32 xmin, S32 xmax, RGB8* buf)
{
	FLASHASSERT(xmax-xmin<=RGBSlabChunkSize);
	RGB8 pixBuf[RGBSlabChunkSize];

	BuildGradientSlab(color, xmin, xmax, pixBuf);
	CompositeRGB8(pixBuf, buf, xmax-xmin);
}

void CompositeBitmapSlab(_XRColor* color, S32 xmin, S32 xmax, RGB8* buf)
{
	//return;
	//_ANALYENTER("Composite Bitmap");
	if(!color->bm.bitmap) return;

	FLASHASSERT(xmax-xmin<=RGBSlabChunkSize);
	RGB8 pixBuf[RGBSlabChunkSize];

	//memset(pixBuf,0,RGBSlabChunkSize*4);
	BuildBitmapSlab(color, xmin, xmax, pixBuf);
	
	
	if(color->bm.bitmap->preAlpha)
	{
		CompositeRGB8(pixBuf, buf, xmax-xmin);
	}
	else
		CompositeRGB8Bitmap(pixBuf, buf, xmax-xmin);
	//CompositeRGB8Bitmap(pixBuf, buf, xmax-xmin);
}


// DrawSlab helpers
inline void DrawSolidSlab(U32 * bits, U32 pix, S32 n)
{
	// This seems to optimize very well w/ intel processor
	// For other processors, it may be worth trying other approaches.
	if ( n )
	{
	 	do
	 	{
//#ifdef __ANDROID__
//	 		XU8 *chpix = (XU8*)&pix;
//	 		XU8 ch = chpix[1];
//	 		chpix[1] = chpix[3];
//	 		chpix[3] = ch;
//	 		*bits++ = pix;
//#else
	 		*bits++ = pix;
//#endif
		} while (--n);
	}

}

// Adjust for plaform specific byte order
#ifdef BIG_ENDIAN
// Mac 68K, Mac PPC & Sparc CPUs
#define OrderPixels(x) (x)
#else
#ifdef _ARGB_PIXEL_
#define OrderPixels(x) (((x&0xFFL)<<24) | ((x)>>8) )
//((x&0xFF00L)>>8) | ((x&0xFF0000L)>>8) | ((x&0xFF000000L)>>8))
#else
// Intel CPUs
#define OrderPixels(x) (((x&0xFFL)<<24) | ((x&0xFF00L)<<8) | ((x&0xFF0000L)>>8) | ((x&0xFF000000L)>>24))
#endif
#endif


const U32 pixMaskTab[32] = {
	OrderPixels(0xFFFFFFFF),//1
	OrderPixels(0x7FFFFFFF),
	OrderPixels(0x3FFFFFFF),
	OrderPixels(0x1FFFFFFF),

	OrderPixels(0x0FFFFFFF),//2
	OrderPixels(0x07FFFFFF),
	OrderPixels(0x03FFFFFF),
	OrderPixels(0x01FFFFFF),

	OrderPixels(0x00FFFFFF),//3
	OrderPixels(0x007FFFFF),
	OrderPixels(0x003FFFFF),
	OrderPixels(0x001FFFFF),

	OrderPixels(0x000FFFFF),//4
	OrderPixels(0x0007FFFF),
	OrderPixels(0x0003FFFF),
	OrderPixels(0x0001FFFF),

	OrderPixels(0x0000FFFF),//5
	OrderPixels(0x00007FFF),
	OrderPixels(0x00003FFF),
	OrderPixels(0x00001FFF),

	OrderPixels(0x00000FFF),//6
	OrderPixels(0x000007FF),
	OrderPixels(0x000003FF),
	OrderPixels(0x000001FF),

	OrderPixels(0x000000FF),//7
	OrderPixels(0x0000007F),
	OrderPixels(0x0000003F),
	OrderPixels(0x0000001F),

	OrderPixels(0x0000000F),//8
	OrderPixels(0x00000007),
	OrderPixels(0x00000003),
	OrderPixels(0x00000001)
};

static void DrawSubSlab(U32 pix, U32 * rowAddr, S32 minWord, S32 minFrac, S32 maxWord, S32 maxFrac)
// minWord - word to start on
// minFrac - number of bits to skip over in rowAddr[minWord]
// maxWord - the last word to change
// maxFrac - the number of bits to set in rowAddr[minWord]
{
	U32 * bits = rowAddr + minWord;
	S32 n = maxWord - minWord;
	if ( !n ) {
		// Change a single word
		U32 mask = pixMaskTab[minFrac] & ~pixMaskTab[maxFrac];
		*bits = (*bits & ~mask) | (pix & mask);
	} else {
		// Handle multiple words

		// Handle the leading fraction
		if ( minFrac ) {
			U32 mask = pixMaskTab[minFrac];
			*bits = (*bits & ~mask) | (pix & mask);
			bits++;
			n--;
		}

		// Handle the middle
		DrawSolidSlab(bits, pix, n);

		// Handle the trailing fraction
		if ( maxFrac ) {
			U32 mask = ~pixMaskTab[maxFrac];
			bits = (U32 *)rowAddr + maxWord;
			*bits = (*bits & ~mask) | (pix & mask);
		}
	}
}




void DrawSolidSlab32(_XRColor* color, S32 xmin, S32 xmax)
{
	XRaster* r = color->raster;
	//xmin += r->xorg;
	//xmax += r->xorg;

	S32 n = xmax-xmin;
	if ( n < 0 ) return;
	//U32 *pat = color->pat + ((r->bitY+r->patAlign.y & 3)*4);	// calculate the pattern row...
	U32  *bits = (U32 *)r->rowAddr + xmin;
	//if ( color->pattern == solidPat )
	{
		// If we know we have a solid pattern, unroll the loop
		DrawSolidSlab(bits, color->pat, n);

	} /*else {
		// Handle the pattern properly
		U32 *p = (U32*)pat + (xmin & 0x3);
		U32 *patEnd = (U32*)pat + 4;

		while ( n-- ) {
			*bits++ = *p++;
			if ( p == patEnd ) p = (U32*)pat;
		}
	}*/
}

void DrawDitherSlab(_XRColor* color, S32 xmin, S32 xmax)
{
	XRaster* r = color->raster;

	// Draw the slab
	while ( xmin < xmax ) {
#ifdef _DIRECTDRAW_
		RGB8* pixBuf=(RGB8*)_ROWADDR(r,xmin);
#else
		RGB8 pixBuf[RGBSlabChunkSize];
#endif
		// Expand the pixels to RGB values
		S32 limit = Min(xmax, xmin+RGBSlabChunkSize);
		RGB8* pix = pixBuf;
		RGB8 rgb;
		rgb.alpha=color->rgb.alpha;
		rgb.red=color->rgb.red;
		rgb.blue=color->rgb.blue;
		rgb.green=color->rgb.green;
		for ( S32 j = limit-xmin; j--; pix++ ) {
			*pix = rgb;
		}

		// Apply a highlight pattern...
		//if ( color->pattern )
		//	ApplyRGBPattern(color->pattern, r->bitY, xmin, limit, pixBuf);
#ifndef _DIRECTDRAW_
		r->drawRGBSlab(r, xmin, limit, pixBuf);
#endif
		xmin = limit;
	}
}

#ifdef ENABLE_MASK
void XRaster::SetMask(int xmin, int xmax)
{
	if ( xmin >= xmax ) return;
	DrawSubSlab(0xffffffff, (U32 *)maskRowAddr, xmin>>5, xmin & 0x1F, xmax>>5, xmax & 0x1F);
}
#endif




extern int ApplyChannel(int x, int a, int b);
extern int ApplyChannelB(int x, int a, int b);

void BltXto(BltInfo bi, SPOINT& pt, int n, RGB8* pix)
{
	FLASHASSERT(false);
}
void Blt8toI(BltInfo bi, SPOINT& pt, int n, RGBI* pix)
// src = bm8Bit dst = RGBI
{
	if ( bi.dy == 0 ) {
		U8 * rowAddr = bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes;
		while ( n-- ) {
			ExpandColor(bi.colors[rowAddr[pt.x>>XBITMAPOFF]], pix);
			pix++;
			pt.x += bi.dx;
		}
	} else {
		while ( n-- ) {
			ExpandColor(bi.colors[*(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes + (pt.x>>XBITMAPOFF))], pix);
			pix++;
			pt.x += bi.dx;
			pt.y += bi.dy;
		}
	}
}

void Blt16toI(BltInfo bi, SPOINT& pt, int n, RGBI* pix)
{
	if ( bi.dy == 0 ) {
		U16 * rowAddr = (U16 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes);
		while ( n-- ) {
			UnpackPix16(rowAddr[pt.x>>XBITMAPOFF], pix);
			pix++;
			pt.x += bi.dx;
		}
	} else {
		while ( n-- ) {
			UnpackPix16(*((U16 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes) + (pt.x>>XBITMAPOFF)), pix);
			pix++;
			pt.x += bi.dx;
			pt.y += bi.dy;
		}
	}
}

void Blt32toI(BltInfo bi, SPOINT& pt, int n, RGBI* pix)
{
	if ( bi.dy == 0 ) {
		U32 * rowAddr = (U32 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes);
		for ( int i = n; i--; ) {
			UnpackPix32(rowAddr[pt.x>>XBITMAPOFF], pix);
			pix++;
			pt.x += bi.dx;
		}
	} else {
		for ( int i = n; i--; ) {
			UnpackPix32(*((U32 *)(bi.baseAddr + (pt.y>>XBITMAPOFF)*bi.rowBytes) + (pt.x>>XBITMAPOFF)), pix);
			pix++;
			pt.x += bi.dx;
			pt.y += bi.dy;
		}
	}
}
