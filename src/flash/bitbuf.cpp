/***************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc. 
****************************************************************************/
#include "StdAfxflash.h"
#include "Bitbuf.h"

// Build an Inverse Table from a set of palette entries
// - we always use a 4-bit inverse table
//

_XCOLORINFO* firstColorInfo = 0;

class _ITabBuilder {
public:
	U8* itab;
	U8 error[16*16*16];
	int nEmpty;

	_ITabBuilder(U8*);
	void FillCube(_XRGB8& c, int index, int d);
};

inline _ITabBuilder::_ITabBuilder(U8* it)
{
	itab = it;
	for ( int i = 0; i < 16*16*16; i++ )
		error[i] = 255;	// flag the cell as unfilled
	nEmpty = 16*16*16;
}

inline int ColorDist(int a, int b) 
{ 
	int e = a-b; 
	return e < 0 ? -e : e; 
}

void _ITabBuilder::FillCube(_XRGB8& c, int index, int d)
{
	int ri = c.red   >> 4;
	int gi = c.green >> 4;
	int bi = c.blue  >> 4;

	int rmin = Max(0, ri-d);
	int rmax = Min(0xf, ri+d);
	int gmin = Max(0, gi-d);
	int gmax = Min(0xf, gi+d);
	int bmin = Max(0, bi-d);
	int bmax = Min(0xf, bi+d);

 	for ( ri = rmin; ri <= rmax; ri++ ) {
		int r = ri == 0xf ? 0xff : ri << 4;
	 	for ( gi = gmin; gi <= gmax; gi++ ) {
			int g = gi == 0xf ? 0xff : gi << 4;

			int errG = ColorDist(r, c.red) + ColorDist(g, c.green);	// move these calcs out of inner loop
			int iG = (ri << 8) | (gi << 4);

		 	for ( bi = bmin; bi <= bmax; bi++ ) {
				int b = bi == 0xf ? 0xff : bi << 4;

				int i = iG | bi;
				int err = errG + ColorDist(b, c.blue);
				if ( err > 254 ) // limit the maximum error so that we can use a byte sized table and save RAM
					err = 254;
				if ( err < error[i] ) {
					if ( error[i] == 255 )
						nEmpty--;
					error[i] = err;
					itab[i] = index;
				}
			}
		}
	}
}

void BuildInverseTable(_XCOLORTABLE* ctab, U8* itab)
{
	_ITabBuilder* builder = new _ITabBuilder(itab);
	if ( !builder ) return;
	// Note we guess that we need to check a distance of 8 for the first pass now
	for ( int d = 3; d < 16 && builder->nEmpty > 0; d++ ) {
		for ( int i = 0; i < ctab->n; i++ )
			builder->FillCube(ctab->colors[i], i, d);
	}
	delete builder;
}

XBOOL ColorTableEqual(_XCOLORTABLE* c1, _XCOLORTABLE* c2)
{
	if ( c1->n != c2->n ) return false;

	_XRGB8* cc1 = c1->colors;
	_XRGB8* cc2 = c2->colors;
	for ( int i = c1->n; i--; cc1++, cc2++ )
		if ( cc1->red   != cc2->red   || 
			 cc1->green != cc2->green || 
			 cc1->blue  != cc2->blue  ) 
			return false;
		//if ( *(U32*)cc1 != *(U32*)cc2 ) return false;
	return true;
}

void ColorTableCopy(_XCOLORTABLE* src, _XCOLORTABLE* dst)
{
	dst->n = src->n;
	_XRGB8* cc1 = src->colors;
	_XRGB8* cc2 = dst->colors;
	for ( int i = src->n; i--; cc1++, cc2++ )
		*cc2 = *cc1;
}

_XCOLORINFO* CreateColorInfo(_XCOLORTABLE* ctab)
{
	// Create a new inverse table
	_XCOLORINFO* cinfo = new _XCOLORINFO;
	if ( !cinfo ) return 0;
	cinfo->next = 0;
	cinfo->refCount = 1;
	ColorTableCopy(ctab, &cinfo->ctab);
	BuildInverseTable(ctab, cinfo->itab);

	//cinfo->CreateOSPalette();

	return cinfo;
}

_XCOLORINFO* GetColorInfo(_XCOLORTABLE* ctab)
{
	// Find an existing color table
	_XCOLORINFO* cinfo = firstColorInfo;
	while ( cinfo ) {
		FLASHASSERT( cinfo->refCount );
		if ( ColorTableEqual(ctab, &cinfo->ctab) ) {
			cinfo->refCount++;
			return cinfo;
		}
		cinfo = cinfo->next;
	}

	// Create a new inverse table
	cinfo = CreateColorInfo(ctab);
	if ( !cinfo ) return 0;
	cinfo->next = firstColorInfo;
	firstColorInfo = cinfo;
	return cinfo;
}

void ReleaseColorInfo(_XCOLORINFO* cinfo)
{
	if ( !cinfo ) return;
	FLASHASSERT( cinfo->refCount );
	if ( --cinfo->refCount == 0 ) {
		_XCOLORINFO** link = &firstColorInfo;
		if(*link)
		{
			while ( *link ) {
				if ( *link == cinfo ) {
					*link = cinfo->next;

					//cinfo->DestroyOSPalette();
					memset( cinfo, 0, sizeof( _XCOLORINFO ) );
					delete cinfo;
					return;
				}
				link = &cinfo->next;
			}
		}
		else
		{
			//cinfo->DestroyOSPalette();
			delete cinfo;
			return;
		}
	}
}

void SetStandardColors(_XCOLORTABLE* ctab)
{
	int i = 0;

	// Do the color ramp - 6 levels of each color
	for ( int b = 0; b <= 5; b++ ) {
		for ( int g = 0; g <= 5; g++ ) {
			for ( int r = 0; r <= 5; r++ ) {
				//if ( r == g && g == b ) 
				//	continue;// don't do any grays here
				ctab->colors[i].red   = colorRamp6[r];
				ctab->colors[i].green = colorRamp6[g];
				ctab->colors[i].blue  = colorRamp6[b];
				i++;
			}
		}
	}

#if 0
	// This might make it look a little better but lets stick with the Netscape grays...
	// fill in a 16 color gray ramp - 14 colors since we skip black & white...
	for ( int g = 1; g < 0xf; g++ ) {
		ctab->colors[i].red   = 
		ctab->colors[i].green = 
		ctab->colors[i].blue  = g == 0xf ? 0xff : g << 4;
		i++;
	}
#endif

	ctab->n = i;
}


//
// The BitBuffer class
//

XBitBuffer::XBitBuffer()
{
	m_inverted = false;
	m_cinfo = 0;
	m_xorg = 0;

	m_allowPurge = false;

	bitmap = 0;
}

XBitBuffer::~XBitBuffer()
{
	FreeBits();
}

void XBitBuffer::invalidate()
{
	FreeBits();
}

XBOOL XBitBuffer::BitsValid()
{
	//XLock lock;
	if ( LockBits() ) 
	{
		UnlockBits();
		return true;
	}
	return false;
}

void XBitBuffer::FreeBits()
{
	ReleaseColorInfo(m_cinfo);
	m_cinfo = 0;
	if(bitmap)
	{
		bitmap->Release();
		delete bitmap;
	}
	bitmap = 0;

}

void XBitBuffer::setAllowPurge( XBOOL p)
{
	m_allowPurge = p;
}

int screenDepth = -1;
int screenFormat = -1;
int screenInverted = true;

XBOOL XBitBuffer::CreateScreenBits(XSIZE& size,XSIZE&max)
{
	//FreeBits();
	if(bitmap)
	{
		int t=size.cx*size.cy*4;
		//if(t==1000*768*4)
		//	int v=0;
		if(t>bitmap->m_nDataLength)
			FreeBits();
	}
	if(bitmap==NULL)
	{
		_XCOLORTABLE* table = 0;

		int depth;

		// Pick a format for the offscreen bitmap
		depth = XGlobal::ScreenBitDepth();

		if ( screenDepth != depth ) {
			if ( screenDepth > 0 )
			{
				delete m_cinfo;
				m_cinfo = 0;
			}

			// We need to profile the device
			screenDepth = depth;

			switch(depth)
			{
			default:
			case 32:
				 screenFormat=pixFmt32;
				 break;
			case 24:
				 screenFormat=pixFmt24;break;
			case 16:
				 screenFormat=pixFmt16;break;
			case 8:
				 screenFormat=pixFmt8;break;
			}	 
			screenInverted=XFALSE;
			//NativeDisplayTester tester;

			//tester.TestDepth( screenDepth );
			//screenFormat = tester.BestDetectedFormat();
			//screenInverted = tester.BestDetectedInvert();
		}

		if ( screenFormat <= pixFmt8 ) 
		{
			if ( m_cinfo )
			{
				delete m_cinfo;
				m_cinfo = 0;
			}

			_XCOLORTABLE ctab;

			//NativePalette::GetScreenPalette( &ctab );
			//_XCOLOR colors[256];
			ctab.n=XGlobal::GetScreenPalette((XCOLOR*)ctab.colors);
			m_cinfo = CreateColorInfo( &ctab );

			if ( !m_cinfo ) return false;
			AttachColorInfo(m_cinfo);
			table = &m_cinfo->ctab;
		}

		m_inverted = screenInverted;
		m_xorg = 0;

		bitmap = new XDrawDib();
		if(bitmap)
		{
			bitmap->Create(max.cx,
						   max.cy,
						   screenDepth,
						   table?table->n:0,
						   (XCOLOR*)(table?table->colors:XNULL));
			XRect rr(0,0,max.cx,max.cy);
			bitmap->Clear(rr,0xff000000);
			//bitmap->Reset();
			//XRect rect(0,0,size.cx,size.cy);
			//bitmap->FillRect(rect,0x0,XFALSE);
		}
	}
	/*	new NativeBitmap(	native, 
								table,
								frame->xmax - frame->xmin,
								frame->ymax - frame->ymin,
								screenDepth,
								screenFormat
						     );*/
	if(bitmap)
		bitmap->Resize(size);
	return ( bitmap&&!bitmap->IsEmpty());
}


void XBitBuffer::Paint( void*handle, int xDest, int yDest, int cx, int cy , int bitOrgX, int bitOrgY)
{
	//XLock lock;
	if ( LockBits() ) {

		if ( bitmap )
		{
			bitmap->Paint(handle,xDest,yDest,XRect(bitOrgX,bitOrgY,bitOrgX+cx,bitOrgY+cy));
			
		}
		UnlockBits();
	}
}


void XBitBuffer::Paint( XGraphics&g,int xDest, int yDest,XRECT&clip)
{
	//XLock lock;
	if ( LockBits() ) {

		if ( bitmap )
		{
			//bitmap->Draw(
			g.DrawDIB(*bitmap,XPoint(xDest,yDest),clip);
			//bitmap->Paint(handle,xDest,yDest,XRect(bitOrgX,bitOrgY,bitOrgX+cx,bitOrgY+cy));
			
		}
		UnlockBits();
	}
}
