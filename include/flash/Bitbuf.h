/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc. 
****************************************************************************/

#ifndef BITBUF_INCLUDED
#define BITBUF_INCLUDED

#include "XDrawDib.h"
#include "flashglobal.h"

#define CalcITabIndex(r, g, b) (((int)(r)&0xF0) << 4 | ((int)(g)&0xF0) | ((int)(b)&0xF0) >> 4)
#define CalcITabIndexRGB(rgb) (((rgb)->red&0xF0) << 4 | ((rgb)->green&0xF0) | ((rgb)->blue&0xF0) >> 4)

const U8 colorRamp6[] = { 0, 0x33, 0x66, 0x99, 0xCC, 0xff };

/*!	The bitmap formats supported by Flash. 
 */

/*!	Flash palette object. 
 */
/*typedef struct _XCOLORTABLE {
	int n;
	XCOLOR colors[256];
}SColorTable;*/

typedef struct _XCOLORINFO {
	// ---------------- Platform independant -----------------
	_XCOLORINFO* next;	// Pointer to the next SColorInfo.
	int refCount;

	_XCOLORTABLE ctab;	// This is the 256 Palette entries.
	U8 itab[16*16*16];	// The inverse color table - a mapping of RGB to indexes.

}SColorInfo;


class XBitBuffer {
  public:
	XBitBuffer();
	~XBitBuffer();

	XBOOL inverted()		{ return ( m_inverted != 0 ); }
	int  pixelFormat()	{ FLASHASSERT( bitmap ); return bitmap->PixelFormat();  }
	int  depth()		{ FLASHASSERT( bitmap ); return bitmap->GetBits();  }
	char* baseAddess()	{ FLASHASSERT( bitmap ); return (char*) bitmap->GetBuffer();  }
	int  scanLine()		{ FLASHASSERT( bitmap ); return bitmap->GetLinePitch();  }
	int  width()		{ FLASHASSERT( bitmap ); return bitmap->Width();   }
	int  height()		{ FLASHASSERT( bitmap ); return bitmap->Height();  }

	void invalidate();	// Deletes the current palette and sets the entire buffer to
						// be invalid. (Be careful when you call this.)
	
	int xorg()					{ return m_xorg; }
  	_XCOLORINFO* getSColorInfo() { return m_cinfo; }
	void setAllowPurge( XBOOL );
	XBOOL allowPurge()			{ return ( m_allowPurge != 0 ); }

	void Paint(	void* Handle,				// where to go get platform dependant info
				int x, int y,//XRECT&rect	// where to start drawing from
				int cx,int cy,
				int ox,int oy
				);
	void Paint(XGraphics&g,
			   int dx,int dy,
			   XRECT&clip);

	XBOOL CreateScreenBits( XSIZE& size,XSIZE&max);	// pass frame in global screen coordinates
	XBOOL IsEmpty()
	{
		return !bitmap||bitmap->IsEmpty();
	}
	XBOOL LockBits() 
	{
		if(!bitmap||bitmap->IsEmpty()) return XFALSE;
		//lock.SetObject(&m_lock);
		//return lock.Lock(XSHORTTIME);
		return XTRUE;
		//if(!lock.Lock(XSHORTTIME)) return XFALSE;
		//return bitmap && !bitmap->IsEmpty(); 
	}
	void UnlockBits() 
	{
		//lock.UnLock();
	}

	void FreeBits();

	XBOOL BitsValid();

	void Flush()		{ }

	XDrawDib* bitmap;
  private:
	//XMutex m_lock;
	// The bitmap info
	XBOOL m_inverted;	// set if we are using the bottom up DIB format

	int m_xorg;			// an offset for aligning the offscreen pixels to the screen pixels

	// Color table info
	_XCOLORINFO* m_cinfo;

	// Flags the determine whether the destructor should free these objects
	XBOOL m_allowPurge;

	int m_bmiSize;
};

typedef XBitBuffer CBitBuffer;

_XCOLORINFO* CreateColorInfo(_XCOLORINFO*);	// create a new color info
_XCOLORINFO* GetColorInfo(_XCOLORINFO*);		// search the shared color infos for a match, otherwise create a new one

inline void AttachColorInfo(_XCOLORINFO* cinfo) {cinfo->refCount++;}
void ReleaseColorInfo(_XCOLORINFO*);

XBOOL ColorTableEqual(_XCOLORINFO* c1, _XCOLORINFO* c2);
void ColorTableCopy(_XCOLORINFO* src, _XCOLORINFO* dst);

void SetStandardColors(_XCOLORINFO*);

extern const U8 colorRamp6[6];

#endif
