/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc.
****************************************************************************/
#include "StdAfxflash.h"
#include <string.h>
#include "XSystem.h"
#include "sbitmap.h"
#include "XDrawDib.h"
#include "flashmemcop.h"
#include "Raster_gl.h"
#include "splayer.h"

#include "avm2.h"

XSWFPlayer* SBitmapCore::m_pPlayer=XNULL;

#define MINDIVSIZE 128
//
// Utility
//
#ifdef _WINEMU
class XSortBitmap:public XSort
{
public:
	XSortBitmap()
	{		
	}
	~XSortBitmap()
	{
		//Release();
	}
	
	
	void Add(SBitmapCore*core)
	{
		if(core == NULL) return;
		int mi=0;
		int si=Index((void*)core,mi);
		m_data.InsertAt(mi,core);
		//_MEMINFO*pInfo=NULL;
	}

	void Remove(SBitmapCore* core)
	{
		int mi=0;
		int si=Index((void*)core,mi);
		if(si<0) return;
		m_data.RemoveAt(si);
	}
	
	void RemoveAt(int index)
	{
		m_data.RemoveAt(index);
	}

	XU32 GetCount(){return m_data.GetSize();}
	SBitmapCore* GetAt(XINT i)
	{
		return m_data[i];
	}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareUInt((XU32)iSrt,(XU32)iDst);
	}
	void* GetData(XU32 nID)
	{
		if(nID>=GetCount()) return XNULL;
		return (void*)m_data[nID];
	}
	XVector<SBitmapCore*> m_data;
	
};

XSortBitmap _bmps;

int SBitmapCore::GetBitmapCount()
{
	return _bmps.GetCount();
}
SBitmapCore* SBitmapCore::GetBitmapByID(int id)
{
	return _bmps.GetAt(id);
}

inline static void _AddReport(SBitmapCore*core)
{
	//if(core->width==97&&core->height==98)
	//	int v=0;
	XSystem::_nTotalImages++;
	XSystem::_nTotalImageMemory+=core->rowBytes*core->height;
	_bmps.Add(core);
}
inline static void _RemoveReport(SBitmapCore*core)
{
	XSystem::_nTotalImages--;
	XSystem::_nTotalImageMemory-=core->rowBytes*core->height;
	_bmps.Remove(core);
}
#else
#define _AddReport(BMP)
#define _RemoveReport(BMP)
#endif

int SBitmapBitsPerPixel(int bmFormat)
{
	switch ( bmFormat ) {
		case bm1Bit: return 1;
		case bm2Bit: return 2;
		case bm4Bit: return 4;
		case bm8Bit: return 8;
		case bm16Bit: return 16;
		case bm32Bit: return 32;
	}
	FLASHASSERT(false);
	return 0;
}

int SBitmapCalcRowbytes(int bmFormat, int width)
{
	switch ( bmFormat )
    {
		case bm1Bit:
			return ((width + 31) >> 3) & ~3;

		case bm2Bit:
			return ((width + 15) >> 2) & ~3;

		case bm4Bit:
			return ((width + 7) >> 1) & ~3;

		case bm8Bit:
			return (width + 3) & ~3;

		case bm16Bit:
			return (2*width + 3) & ~3;

		case bm32Bit:
			return 4*width;
	}
	FLASHASSERT(false);
	return 0;
}

void SBitmapCore::GetColorBounds(XRect&rect,XU32 mask,XU32 color,XBOOL bFind)
{
	if(!baseAddr) return;
	XU32 maxx=0,maxy=0,minx=width,miny=height;
	U8 * rowAddr = (U8 *)baseAddr;
	
	XU32 nColor=0;
	RGB8* c=(RGB8*)&nColor;
	for(XU32 y=0;y<height;y++)
	{
		for(XU32 x=0;x<width;x++)
		{
			
			switch ( bmFormat ) 
			{
				case bm1Bit: {
					U8  *bits = rowAddr + (x>>3);
					int shift = 7 - (x & 0x07);
					*c=cTab->colors[(*bits >> shift) & 1];
					//ExpandColor(cTab->colors[(*bits >> shift) & 1], c);
				} break;

				case bm2Bit: {
					U8  *bits = rowAddr + (x>>2);
					int shift = (3 -(x & 0x03))<<1;
					*c=cTab->colors[(*bits >> shift) & 0x3];
					//ExpandColor(cTab->colors[(*bits >> shift) & 0x3], c);
				} break;

				case bm4Bit: {
					U8  *bits = rowAddr + (x>>1);
					int shift = (1 - (x & 0x01))<<2;
					*c=cTab->colors[(*bits >> shift) & 0xF];
					//ExpandColor(cTab->colors[(*bits >> shift) & 0xF], c);
				} break;

				case bm8Bit: {
					*c=cTab->colors[rowAddr[x]];
					//ExpandColor(cTab->colors[rowAddr[x]], c);
				} break;

				case bm16Bit: {
					U16 pix = *((U16 *)rowAddr + x);
					c->alpha = 255;
					c->red   = pix16Expand[(pix>>10)&0x1f];
					c->green = pix16Expand[(pix>> 5)&0x1f];
					c->blue  = pix16Expand[(pix    )&0x1f];
					//UnpackPix16(pix, c);
				} break;

				case bm32Bit: {
					*((XU32*)c) = *((U32 *)rowAddr + x);
					//cTab->colors[rowAddr[x]]
					//UnpackPix32(pix, c);
				} break;
			}			
			if(bFind)
			{
				if((nColor&mask)!=color) continue;
			}
			else
			{
				if((nColor&mask)==color) continue;
			}
			minx=XMIN(minx,x);
			maxx=XMAX(maxx,x);
			miny=XMIN(miny,y);
			maxy=XMAX(maxy,y);
		}
		rowAddr+=this->rowBytes;
	}
	if(maxx>=minx)
	{
		rect.left=minx;
		rect.right=maxx+1;
	}
	if(maxy>=miny)
	{
		rect.top=miny;
		rect.bottom=maxy+1;
	}
}

void SBitmapCore::GetRGBPixel(int x, int y, RGBI* c)
{
	//return;
	//FLASHASSERT(baseAddr);
	if(!baseAddr) return;

	// Clip to the edges
	if ( x < 0 ) x = 0;
	if ( y < 0 ) y = 0;
	if ( y >= height ) y = height-1;
	if ( x >= width ) x = width-1;

	U8 * rowAddr = (U8 *)baseAddr + y*rowBytes;
	switch ( bmFormat ) {
		case bm1Bit: {
			U8  *bits = rowAddr + (x>>3);
			int shift = 7 - (x & 0x07);
			ExpandColor(cTab->colors[(*bits >> shift) & 1], c);
		} return;

		case bm2Bit: {
			U8  *bits = rowAddr + (x>>2);
			int shift = (3 -(x & 0x03))<<1;
			ExpandColor(cTab->colors[(*bits >> shift) & 0x3], c);
		} return;

		case bm4Bit: {
			U8  *bits = rowAddr + (x>>1);
			int shift = (1 - (x & 0x01))<<2;
			ExpandColor(cTab->colors[(*bits >> shift) & 0xF], c);
		} return;

		case bm8Bit:
		{
#ifdef __ANDROID__  //修改摩尔庄园工具条颜色问题， add by hgl
			c->blue = cTab->colors[rowAddr[x]].red;
			c->green = cTab->colors[rowAddr[x]].green;
			c->red = cTab->colors[rowAddr[x]].blue;
			c->alpha = cTab->colors[rowAddr[x]].alpha;
#else
			//RGBI*dst=c;
			//RGB8&src=cTab->colors[rowAddr[x]];
			//dst->alpha = src.blue;//src.alpha;
			//dst->blue  = src.alpha;//src.blue;
			//dst->red   = src.green;//src.red;
			//dst->green = src.red;//src.green;
			
			ExpandColor(cTab->colors[rowAddr[x]], c);
#endif
		} return;

		case bm16Bit: {
			U16 pix = *((U16 *)rowAddr + x);
			UnpackPix16(pix, c);
		} return;

		case bm32Bit: {
			U32 pix = *((U32 *)rowAddr + x);
			UnpackPix32(pix, c);
		} return;
	}
}

void SBitmapCore::GetRGBPixel8(int x, int y, RGB8* c)
{
	//return;
	//FLASHASSERT(baseAddr);
	if(!baseAddr)
		return;

	// Clip to the edges
	if ( x < 0 ) x = 0;
	if ( y < 0 ) y = 0;
	if ( y >= height ) y = height-1;
	if ( x >= width ) x = width-1;

	U8 * rowAddr = (U8 *)baseAddr + y*rowBytes;
	switch ( bmFormat ) {
		case bm1Bit: {
			U8  *bits = rowAddr + (x>>3);
			int shift = 7 - (x & 0x07);
			*c=cTab->colors[(*bits >> shift) & 1];
			//ExpandColor(cTab->colors[(*bits >> shift) & 1], c);
		} return;

		case bm2Bit: {
			U8  *bits = rowAddr + (x>>2);
			int shift = (3 -(x & 0x03))<<1;
			*c=cTab->colors[(*bits >> shift) & 0x3];
			//ExpandColor(cTab->colors[(*bits >> shift) & 0x3], c);
		} return;

		case bm4Bit: {
			U8  *bits = rowAddr + (x>>1);
			int shift = (1 - (x & 0x01))<<2;
			*c=cTab->colors[(*bits >> shift) & 0xF];
			//ExpandColor(cTab->colors[(*bits >> shift) & 0xF], c);
		} return;

		case bm8Bit: {
			*c=cTab->colors[rowAddr[x]];
			//ExpandColor(cTab->colors[rowAddr[x]], c);
		} return;

		case bm16Bit: {
			U16 pix = *((U16 *)rowAddr + x);
			c->alpha = 255;
			c->red   = pix16Expand[(pix>>10)&0x1f];
			c->green = pix16Expand[(pix>> 5)&0x1f];
			c->blue  = pix16Expand[(pix    )&0x1f];
			//UnpackPix16(pix, c);
		} return;

		case bm32Bit: {
			*((XU32*)c) = *((U32 *)rowAddr + x);
			//cTab->colors[rowAddr[x]]
			//UnpackPix32(pix, c);
		} return;
	}
}


void SBitmapCore::GetHorzData(int x,int y,int ey,RGB8*c)
{
	if(!baseAddr) return;

	if(x<0) x=0;
	else if(x>=width) x=width-1;
	if(y<0)
	{

		int di=ey<0?ey:0;
		GetRGBPixel8(x,y,c);
		XU32 color=*((XU32*)c);
		y++;
		c++;

		while(y<di)
		{
			*((XU32*)c)=color;
			y++;
			c++;
		}
	}

	if(ey>height)
	{
		int di=y>height?y:height;
		XU32* pix=(XU32*)(c+ey-y-1);
		GetRGBPixel8(x,ey-1,(RGB8*)pix);
		XU32 color=*pix;
		pix--;
		ey--;
		while(ey>di)
		{
			*pix=color;
			pix--;
			ey--;
		}
	}
	//return;
	U8 * rowAddr = (U8 *)baseAddr + y*rowBytes;
	while(y<ey)
	{
	switch ( bmFormat ) {
			case bm1Bit: {
				U8  *bits = rowAddr + (x>>3);
				int shift = 7 - (x & 0x07);
				*c=cTab->colors[(*bits >> shift) & 1];
				//ExpandColor(cTab->colors[(*bits >> shift) & 1], c);
			} break;

			case bm2Bit: {
				U8  *bits = rowAddr + (x>>2);
				int shift = (3 -(x & 0x03))<<1;
				*c=cTab->colors[(*bits >> shift) & 0x3];
				//ExpandColor(cTab->colors[(*bits >> shift) & 0x3], c);
			} break;

			case bm4Bit: {
				U8  *bits = rowAddr + (x>>1);
				int shift = (1 - (x & 0x01))<<2;
				*c=cTab->colors[(*bits >> shift) & 0xF];
				//ExpandColor(cTab->colors[(*bits >> shift) & 0xF], c);
			} break;

			case bm8Bit: {
				*c=cTab->colors[rowAddr[x]];
				//ExpandColor(cTab->colors[rowAddr[x]], c);
			} break;

			case bm16Bit: {
				U16 pix = *((U16 *)rowAddr + x);
				c->alpha = 255;
				c->red   = pix16Expand[(pix>>10)&0x1f];
				c->green = pix16Expand[(pix>> 5)&0x1f];
				c->blue  = pix16Expand[(pix    )&0x1f];
				//UnpackPix16(pix, c);
			} break;

			case bm32Bit: {
				*((XU32*)c) = *((U32 *)rowAddr + x);
				//cTab->colors[rowAddr[x]]
				//UnpackPix32(pix, c);
			} break;
		}
		y++;
		rowAddr+=rowBytes;
		c++;
	}
}
void SBitmapCore::GetHorzDataR(int x,int y,int ey,RGB8*c)
{

	if(!baseAddr) return;

	if(x<0) x=0;
	else if(x>=width) x=width-1;
	if(y>=height)
	{
		int di=ey>=height?ey+1:height;
		GetRGBPixel8(x,y,c);
		XU32 color=*((XU32*)c);
		y--;
		c++;
		while(y>=di)
		{
			*((XU32*)c)=color;
			y--;
			c++;
		}
	}
	if(ey<0)
	{
		int di=y<0?y:0;
		XU32* pix=(XU32*)(c+y-ey-1);
		GetRGBPixel8(x,ey,(RGB8*)pix);
		XU32 color=*pix;
		pix--;
		ey++;
		while(ey<di)
		{
			*pix=color;
			pix--;
			ey++;
		}
	}

	U8 * rowAddr = (U8 *)baseAddr + y*rowBytes;
	while(y>ey)
	{
	switch ( bmFormat ) {
			case bm1Bit: {
				U8  *bits = rowAddr + (x>>3);
				int shift = 7 - (x & 0x07);
				*c=cTab->colors[(*bits >> shift) & 1];
				//ExpandColor(cTab->colors[(*bits >> shift) & 1], c);
			} break;

			case bm2Bit: {
				U8  *bits = rowAddr + (x>>2);
				int shift = (3 -(x & 0x03))<<1;
				*c=cTab->colors[(*bits >> shift) & 0x3];
				//ExpandColor(cTab->colors[(*bits >> shift) & 0x3], c);
			} break;

			case bm4Bit: {
				U8  *bits = rowAddr + (x>>1);
				int shift = (1 - (x & 0x01))<<2;
				*c=cTab->colors[(*bits >> shift) & 0xF];
				//ExpandColor(cTab->colors[(*bits >> shift) & 0xF], c);
			} break;

			case bm8Bit: {
				*c=cTab->colors[rowAddr[x]];
				//ExpandColor(cTab->colors[rowAddr[x]], c);
			} break;

			case bm16Bit: {
				U16 pix = *((U16 *)rowAddr + x);
				c->alpha = 255;
				c->red   = pix16Expand[(pix>>10)&0x1f];
				c->green = pix16Expand[(pix>> 5)&0x1f];
				c->blue  = pix16Expand[(pix    )&0x1f];
				//UnpackPix16(pix, c);
			} break;

			case bm32Bit: {
				*((XU32*)c) = *((U32 *)rowAddr + x);
				//cTab->colors[rowAddr[x]]
				//UnpackPix32(pix, c);
			} break;
		}
		y--;
		rowAddr-=rowBytes;
		c++;
	}
}

void SBitmapCore::GetLineDataR(int x, int y,int ex, RGB8* c)
{
	//FLASHASSERT(baseAddr);
	if(!baseAddr) return;


	if(y<0) y=0;
	else if(y>=height) y=height-1;

	if(x>=width)
	{
		int di=ex>=width?ex+1:width;
		//if(di<ex) di=ex;
		GetRGBPixel8(x,y,c);
		XU32 color=*((XU32*)c);
		x--;
		c++;

		//XU32 color=*(((XU32*)rowAddr)+width-1);
		while(x>=di)
		{
			*((XU32*)c)=color;
			x--;
			c++;
		}
	}
	if(ex<0)
	{
		int di=x<=0?x:0;
		//if(di>x) di=x;
		//GetRGBPixel8(x,y,c);
		//XU32 color=*((XU32*)c);
		XU32* pix=(XU32*)(c+x-ex-1);
		GetRGBPixel8(ex,y,(RGB8*)pix);
		XU32 color=*pix;
		pix--;
		ex++;
		while(ex<di)
		{
			*pix=color;
			pix--;
			ex++;
		}
	}

	U8 * rowAddr = (U8 *)baseAddr + y*rowBytes;
	while(x>ex)
	{
		switch ( bmFormat ) {
			case bm1Bit: {
				U8  *bits = rowAddr + (x>>3);
				int shift = 7 - (x & 0x07);
				*c=cTab->colors[(*bits >> shift) & 1];
				//ExpandColor(cTab->colors[(*bits >> shift) & 1], c);
			} break;

			case bm2Bit: {
				U8  *bits = rowAddr + (x>>2);
				int shift = (3 -(x & 0x03))<<1;
				*c=cTab->colors[(*bits >> shift) & 0x3];
				//ExpandColor(cTab->colors[(*bits >> shift) & 0x3], c);
			} break;

			case bm4Bit: {
				U8  *bits = rowAddr + (x>>1);
				int shift = (1 - (x & 0x01))<<2;
				*c=cTab->colors[(*bits >> shift) & 0xF];
				//ExpandColor(cTab->colors[(*bits >> shift) & 0xF], c);
			} break;

			case bm8Bit: {
				*c=cTab->colors[rowAddr[x]];
				//ExpandColor(cTab->colors[rowAddr[x]], c);
			} break;

			case bm16Bit: {
				U16 pix = *((U16 *)rowAddr + x);
				c->alpha = 255;
				c->red   = pix16Expand[(pix>>10)&0x1f];
				c->green = pix16Expand[(pix>> 5)&0x1f];
				c->blue  = pix16Expand[(pix    )&0x1f];
				//UnpackPix16(pix, c);
			} break;

			case bm32Bit: {
				*((XU32*)c) = *((U32 *)rowAddr + x);
				//cTab->colors[rowAddr[x]]
				//UnpackPix32(pix, c);
			} break;
		}
		x--;
		c++;
	}
}

void SBitmapCore::GetLineData(int x, int y,int ex, RGB8* c)
{
	//FLASHASSERT(baseAddr);
	if(!baseAddr) 
		return;

	if(y<0) y=0;
	else if(y>height-1) y=height-1;
	if(x<0)
	{
		int di=ex<0?ex:0;
		this->GetRGBPixel8(x,y,c);
		XU32 color= *((XU32*)c);
		c++;
		x++;
		while(x<di)
		{
			*((XU32*)c)=color;
			c++;
			x++;
		}
	}
	if(ex>width)
	{
		XU32*pix=(XU32*)(c+(ex-x)-1);
		GetRGBPixel8(ex,y,(RGB8*)pix);
		XU32 color=*pix;
		int di=x>=width?x:width;

		pix--;
		ex--;
		//XU32 color= *(((XU32*)rowAddr)+width-1);
		while(ex>di)
		{
			*pix=color;
			ex--;
			pix--;
		}
	}
	U8 * rowAddr = (U8 *)baseAddr + y*rowBytes;
	while(x<ex)
	{
		switch ( bmFormat ) {
			case bm1Bit: {
				U8  *bits = rowAddr + (x>>3);
				int shift = 7 - (x & 0x07);
				*c=cTab->colors[(*bits >> shift) & 1];
				//ExpandColor(cTab->colors[(*bits >> shift) & 1], c);
			} break;

			case bm2Bit: {
				U8  *bits = rowAddr + (x>>2);
				int shift = (3 -(x & 0x03))<<1;
				*c=cTab->colors[(*bits >> shift) & 0x3];
				//ExpandColor(cTab->colors[(*bits >> shift) & 0x3], c);
			} break;

			case bm4Bit: {
				U8  *bits = rowAddr + (x>>1);
				int shift = (1 - (x & 0x01))<<2;
				*c=cTab->colors[(*bits >> shift) & 0xF];
				//ExpandColor(cTab->colors[(*bits >> shift) & 0xF], c);
			} break;

			case bm8Bit: {
				*c=cTab->colors[rowAddr[x]];
				//ExpandColor(cTab->colors[rowAddr[x]], c);
			} break;

			case bm16Bit: {
				U16 pix = *((U16 *)rowAddr + x);
				c->alpha = 255;
				c->red   = pix16Expand[(pix>>10)&0x1f];
				c->green = pix16Expand[(pix>> 5)&0x1f];
				c->blue  = pix16Expand[(pix    )&0x1f];
				//UnpackPix16(pix, c);
			} break;

			case bm32Bit: {
				*((XU32*)c) = *((U32 *)rowAddr + x);
				//cTab->colors[rowAddr[x]]
				//UnpackPix32(pix, c);
			} break;
		}
		x++;
		c++;
	}
}

void SBitmapCore::GetLineData2(int xi, int y,int ex, RGB8* c)
{
	//FLASHASSERT(baseAddr);
	if(!baseAddr) return;

	U8 * rowAddr = (U8 *)baseAddr + (y)*rowBytes;
	while(xi<ex)
	{
		int x=xi;
		switch ( bmFormat ) {
			case bm1Bit: {
				U8  *bits = rowAddr + (x>>3);
				int shift = 7 - (x & 0x07);
				*c=cTab->colors[(*bits >> shift) & 1];
				//ExpandColor(cTab->colors[(*bits >> shift) & 1], c);
			} break;

			case bm2Bit: {
				U8  *bits = rowAddr + (x>>2);
				int shift = (3 -(x & 0x03))<<1;
				*c=cTab->colors[(*bits >> shift) & 0x3];
				//ExpandColor(cTab->colors[(*bits >> shift) & 0x3], c);
			} break;

			case bm4Bit: {
				U8  *bits = rowAddr + (x>>1);
				int shift = (1 - (x & 0x01))<<2;
				*c=cTab->colors[(*bits >> shift) & 0xF];
				//ExpandColor(cTab->colors[(*bits >> shift) & 0xF], c);
			} break;

			case bm8Bit: {
				*c=cTab->colors[rowAddr[x]];
				//ExpandColor(cTab->colors[rowAddr[x]], c);
			} break;

			case bm16Bit: {
				U16 pix = *((U16 *)rowAddr + x);
				c->alpha = 255;
				c->red   = pix16Expand[(pix>>10)&0x1f];
				c->green = pix16Expand[(pix>> 5)&0x1f];
				c->blue  = pix16Expand[(pix    )&0x1f];
				//UnpackPix16(pix, c);
			} break;

			case bm32Bit: {
				*((XU32*)c) = *((U32 *)rowAddr + x);
				//cTab->colors[rowAddr[x]]
				//UnpackPix32(pix, c);
			} break;
		}
		xi++;
		c++;
	}
}


void SBitmapCore::GetData(int x,int y,RGB8*c,int n,int dx,int dy)
{
	//return;
	if(!baseAddr) return;

	if(dy==0)
	{
		if(dx>0)
			GetLineData(x,y,x+n,c);
		else
			GetLineDataR(x,y,x-n,c);
	}
	else
	{
		//return;
		if(dy>0)
			GetHorzData(x,y,y+n,c);
		else
			GetHorzDataR(x,y,y-n,c);
	}
}


// #ifndef WIN16
void SBitmapCore::GetSSRGBPixel(SFIXED xH, SFIXED yH, RGBI* c)
{
	//FLASHASSERT(baseAddr);
	if(!baseAddr) return;

	int x = (int)(xH >> 16);
	int y = (int)(yH >> 16);
	int xf = (U16)xH >> 13;
	int yf = (U16)yH >> 13;

	CoverEntry cv = PixCoverage[xf][yf];

	// Clip to the edges
	if ( x < 0 ) {
		x = 0;
		cv.ce[0]+=cv.ce[1]; cv.ce[1]=0; // shift the coverage to the leftmost column
		cv.ce[2]+=cv.ce[3]; cv.ce[3]=0;
	} else if ( x >= width-1 ) {
		x = width-2;
		cv.ce[1]+=cv.ce[0]; cv.ce[0]=0; // shift the coverage to the rightmost column
		cv.ce[3]+=cv.ce[2]; cv.ce[2]=0;
	}
	if ( y < 0 ) {
		y = 0;
		cv.ce[0]+=cv.ce[2]; cv.ce[2]=0; // shift the coverage to the top row
		cv.ce[1]+=cv.ce[3]; cv.ce[3]=0;
	} else if ( y >= height-1 ) {
		y = height-2;
		cv.ce[2]+=cv.ce[0]; cv.ce[0]=0; // shift the coverage to the bottom row
		cv.ce[3]+=cv.ce[1]; cv.ce[1]=0;
	}

// #if 1
	RGBIU pix;
	RGBIU p;
	U8 * rowAddr = (U8 *)baseAddr + y*rowBytes;
	switch ( bmFormat ) {
		//case bm1Bit:
		//case bm2Bit:
		//case bm4Bit:
		case bm8Bit: {
			//const U8* shiftTab = shiftTabs[bmFormat];	// table of shift values
			//int pxMask   = pxMasks[bmFormat];		// mask the same width as a pixel
			//int rbShift  = rbShifts[bmFormat];	// shift count to calc rowbytes
			//int stMask   = stMasks[bmFormat];		// mask to extract index for shiftTab

			// Get the first column
			U8 * bits = rowAddr + (x);//>>rbShift);
			//int shift = shiftTab[x&stMask];
#ifdef _USE64BITS
			ExpandColor(cTab->colors[(bits[0])], &pix.i);
			pix.l *= cv.ce[0];
			ExpandColor(cTab->colors[(bits[rowBytes])], &p.i);
			pix.l += p.l*cv.ce[2];
			// Get the second column
			//x++;
			//bits = rowAddr + (x>>rbShift);
			bits++;
		//	shift = shiftTab[x&stMask];
			ExpandColor(cTab->colors[(bits[0])], &p.i);
			pix.l += p.l*cv.ce[1];
			ExpandColor(cTab->colors[(bits[rowBytes])], &p.i);
			pix.l += p.l*cv.ce[3];
		} break;

		case bm16Bit: {
			U16 * bits = (U16 *)rowAddr + x;
			UnpackPix16(bits[0], &pix.i);
			pix.l *= cv.ce[0];
			UnpackPix16(bits[1], &p.i);
			pix.l += p.l*cv.ce[1];

			// Get the second row
			bits = (U16 *)((U8 *)bits + rowBytes);
			UnpackPix16(bits[0], &p.i);
			pix.l += p.l*cv.ce[2];
			UnpackPix16(bits[1], &p.i);
			pix.l += p.l*cv.ce[3];
		} break;

		case bm32Bit: {
			U32 * bits = (U32 *)rowAddr + x;
			UnpackPix32(bits[0], &pix.i);
			pix.l *= cv.ce[0];
			UnpackPix32(bits[1], &p.i);
			pix.l += p.l*cv.ce[1];

			// Get the second row
			bits = (U32 *)((U8 *)bits + rowBytes);
			UnpackPix32(bits[0], &p.i);
			pix.l += p.l*cv.ce[2];
			UnpackPix32(bits[1], &p.i);
			pix.l += p.l*cv.ce[3];
		} break;
	}
	*((XU64*)c)=(pix.l>>3)&_MASK64;

#else
			ExpandColor(cTab->colors[(bits[0])], &pix.i);
			pix.l.c0 *= cv.ce[0];
			pix.l.c1 *= cv.ce[0];
			ExpandColor(cTab->colors[(bits[rowBytes])], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[2];
			pix.l.c1 += p.l.c1*cv.ce[2];

			// Get the second column
			//x++;
			//bits = rowAddr + (x>>rbShift);
			bits++;
		//	shift = shiftTab[x&stMask];
			ExpandColor(cTab->colors[(bits[0])], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[1];
			pix.l.c1 += p.l.c1*cv.ce[1];
			ExpandColor(cTab->colors[(bits[rowBytes])], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[3];
			pix.l.c1 += p.l.c1*cv.ce[3];
		} break;

		case bm16Bit: {
			U16 * bits = (U16 *)rowAddr + x;
			UnpackPix16(bits[0], &pix.i);
			pix.l.c0 *= cv.ce[0];
			pix.l.c1 *= cv.ce[0];
			UnpackPix16(bits[1], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[1];
			pix.l.c1 += p.l.c1*cv.ce[1];

			// Get the second row
			bits = (U16 *)((U8 *)bits + rowBytes);
			UnpackPix16(bits[0], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[2];
			pix.l.c1 += p.l.c1*cv.ce[2];
			UnpackPix16(bits[1], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[3];
			pix.l.c1 += p.l.c1*cv.ce[3];
		} break;

		case bm32Bit: {
			U32 * bits = (U32 *)rowAddr + x;
			UnpackPix32(bits[0], &pix.i);
			pix.l.c0 *= cv.ce[0];
			pix.l.c1 *= cv.ce[0];
			UnpackPix32(bits[1], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[1];
			pix.l.c1 += p.l.c1*cv.ce[1];

			// Get the second row
			bits = (U32 *)((U8 *)bits + rowBytes);
			UnpackPix32(bits[0], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[2];
			pix.l.c1 += p.l.c1*cv.ce[2];
			UnpackPix32(bits[1], &p.i);
			pix.l.c0 += p.l.c0*cv.ce[3];
			pix.l.c1 += p.l.c1*cv.ce[3];
		} break;
	}

	((RGBIL*)c)->c0 = (pix.l.c0>>3) & 0x00FF00FF;
	((RGBIL*)c)->c1 = (pix.l.c1>>3) & 0x00FF00FF;
#endif
}



void SBitmapCore::PIInit(int nDiv)
{
	//nDiv=1;
	m_nLockTime = 0;
	useCount = 0;
	//isAttach = false;
	m_pCH = NULL;
	bitmapID = 0;
	bAdded = false;
//	lineAlpha = NULL;
	baseAddr = 0;
	cTab = 0;
	nImgDiv = nDiv;//nDiv;
//	lockCount = 0;
//	useCount = 0;
//	width0=0;
//	height0=0;
	pAlloc=0;
//	nBlendMode=0;
	transparent = false;
	preAlpha= false;
//	m_pBitmapData = NULL;
//	m_nMaxCacheTime=8000;
//	if(m_pPlayer->m_pDomFilter)
//		m_nMaxCacheTime=m_pPlayer->m_pDomFilter->nCacheImage;
}

BOOL HasTransparent(SColorTable* ctab)
{
	if ( !ctab )
		return false;
	RGB8* c = ctab->colors;
	for ( int i = ctab->n; i--; c++ ) {
		if ( c->alpha < 255 )
			return true;
	}
	return false;
}

/*BOOL SBitmapCore::PICreate(int format, int w, int h, SColorTable* c, BOOL allowPurge)
{
	baseAddr = 0;
	cTab = 0;
	lockCount = 0;
	transparent = ( HasTransparent(c) != 0 );

	bmFormat = format;
	width = w;
	height = h;
	rowBytes = SBitmapCalcRowbytes(bmFormat, width);

	if ( bmFormat <= bm8Bit ) {
		FLASHASSERT(c);

		// Create our color table
		int ctabSize = sizeof(SColorTable) - (256-c->n)*sizeof(RGB8);
		cTab = new SColorTable;
		if ( !cTab )
			return false;
		memcpy(cTab, c, ctabSize);
	}

	S32 size = (S32)rowBytes*height;

	baseAddr = (char*)malloc(size);
	return baseAddr != 0;
}*/

BOOL SBitmapCore::PICreate(int format, int nColors,int w, int h, XStream&stream,XBOOL bAlpha)//SColorTable* c, BOOL allowPurge)
{
	//if(!bitmapID)
	//	m_pPlayer->AddBitmap(this);
	/*if(w*h>2000*2000)
	{
		return CreateEmpty();
		//return XTRUE;
	}*/
	//width0 = w;
	//height0 = h;
	//CalcImageDiv(w,h);
	baseAddr = 0;
	pAlloc=0;
	cTab = 0;
//	lockCount = 0;
	transparent = bAlpha;//( HasTransparent(c) != 0 );

	bmFormat = format;
	width = w;
	height = h;
	width0 = w;
	height0 = h;
	rowBytes = SBitmapCalcRowbytes(bmFormat, width);

	int tabSize=0,s=bAlpha?4:3,offset=0;
	S32 size = (S32)rowBytes*height;

	if ( bmFormat <= bm8Bit ) {
		tabSize=sizeof(_XCOLORTABLE);
		size+=tabSize;
	}

	//S32 size = (S32)rowBytes*height;
	pAlloc=(char*)new XU8[size];
	if(pAlloc==XNULL) return XFALSE;
	
	_AddReport(this);

	baseAddr=pAlloc;
	//baseAddr=pAlloc+tabSize;
	XStream out;
	if(bmFormat<=bm8Bit)
	{
		cTab=(_XCOLORTABLE*)pAlloc;
		cTab->n=nColors;
		baseAddr=pAlloc+tabSize;
		offset=nColors*s;
		//memset(cTab->colors,0xff,sizeof(cTab->colors));
			//sizeof(int);
		//if(!bAlpha) offset+=(nColors)+(256-nColors)*s;
		out.AttachFrom(baseAddr-offset,size-tabSize+offset,XTRUE);

			//pAlloc+offset,size-offset,XTRUE);
	}
	else
		out.AttachFrom(pAlloc,size,XTRUE);
	XZLib lib;
	lib.Init(&stream,&out);
	lib.Decode(XNULL);

	//memset(baseAddr,0xff,size-tabSize);

	//XU8*pSrt=out.DataTo();
	if(cTab)
	{
		/*for(int j=0;j<256;j++)
		{
			cTab->colors[j].alpha=0xff;
			cTab->colors[j].blue=0xff;
			cTab->colors[j].red=0xff;
			cTab->colors[j].green=0xff;
		}*/
		XU8*pSrt=(XU8*)baseAddr-offset;
		if(!bAlpha)
		{
			for(int i=0;i<nColors;i++)
			{
//#ifdef 	_ARGB_PIXEL_  //可能引起其他图像颜色错误， modify by hgl
				cTab->colors[i].red=pSrt[2];
				cTab->colors[i].green=pSrt[1];
				cTab->colors[i].blue=pSrt[0];
//#elif __ANDROID__
//				cTab->colors[i].red=pSrt[2];
//				cTab->colors[i].green=pSrt[1];
//				cTab->colors[i].blue=pSrt[0];
//#else
//				cTab->colors[i].red=pSrt[0];
//				cTab->colors[i].green=pSrt[1];
//				cTab->colors[i].blue=pSrt[2];
//#endif
				cTab->colors[i].alpha=0xff;//bAlpha?pSrt[3]:0xff;
				pSrt+=s;
			}
		}
		else
		{
			XU32 v;
			XU8* rgb=(XU8*)&v;
			for(int i=0;i<nColors;i++)
			{
				v=*((XU32*)pSrt);
				/*switch(rgb[3])
				{
				case 0:
					cTab->colors[i].red=0;
					cTab->colors[i].green=0;
					cTab->colors[i].blue=0;
					cTab->colors[i].alpha=0;
					break;
				case 0xff:
					cTab->colors[i].red=rgb[2];
					cTab->colors[i].green=rgb[1];
					cTab->colors[i].blue=rgb[0];
					cTab->colors[i].alpha=0xff;
					break;
				default:
					cTab->colors[i].red=(rgb[2]*rgb[3])>>8;
					cTab->colors[i].green=(rgb[1]*rgb[3])>>8;
					cTab->colors[i].blue=(rgb[0]*rgb[3]>>8;
					cTab->colors[i].alpha=(rgb[3]*rgb[3])>>8;
					break;
					break;
				}*/
//#ifdef _ARGB_PIXEL_    //可能引起其他图像颜色错误， modify by hgl
				cTab->colors[i].red=rgb[2];//pSrt[0];//pSrt[0];
				cTab->colors[i].green=rgb[1];//pSrt[1];//pSrt[1];
				cTab->colors[i].blue=rgb[0];//pSrt[2];//pSrt[2];
//#elif __ANDROID__
//				cTab->colors[i].red=rgb[2];//pSrt[0];//pSrt[0];
//				cTab->colors[i].green=rgb[1];//pSrt[1];//pSrt[1];
//				cTab->colors[i].blue=rgb[0];//pSrt[2];//pSrt[2];
//#else
//				cTab->colors[i].red=rgb[0];//pSrt[0];//pSrt[0];
//				cTab->colors[i].green=rgb[1];//pSrt[1];//pSrt[1];
//				cTab->colors[i].blue=rgb[2];//pSrt[2];//pSrt[2];
//#endif
				cTab->colors[i].alpha=rgb[3];//pSrt[3];//bAlpha?pSrt[3]:0xff;
				pSrt+=s;
			}
		}
	}
	else
	{
		SBitmapSwapBits(baseAddr, size-tabSize,bmFormat);
	}
//#if (__CORE_VERSION__>=0x02075000)
////	SetBlockDiv(w,h);
//#endif
//	DivImage();
	preAlpha = false;
	//baseAddr = (char*)malloc(size);
	/*if(nBlendMode)
	{
		int nMode=nBlendMode;
		nBlendMode=0;
		SetBlendMode(nMode);
	}*/
	return baseAddr != 0;
}


/*void SBitmapCore::BuildLineAlpha()
{
	if(lineAlpha) return;
	if(!baseAddr) return;
	lineAlpha=(XU16*)XXVar::_AllocData(height*4);
	//if(width>2000)
	//	int v=0;
	int x,y;
	U8  *rowAddr = (U8 *)baseAddr;
	XU16*lineData=lineAlpha;
	for(y=0;y<height;y++)
	{	
		//int iStart=0;
//		if(y==411)
//			int v=0;
		bool bStart=false;
		lineData[0]=0;
		lineData[1]=0;
		int iStart=0;
		for(x=0;x<width;x++)
		{
			int alpha=0;
			switch ( bmFormat ) {
				case bm1Bit: {
					U8  *bits = rowAddr + (x>>3);
					int shift = 7 - (x & 0x07);
					//*c=cTab->colors[(*bits >> shift) & 1];
					alpha=cTab->colors[(*bits >> shift) & 1].alpha;
					//ExpandColor(cTab->colors[(*bits >> shift) & 1], c);
				} break;

				case bm2Bit: {
					U8  *bits = rowAddr + (x>>2);
					int shift = (3 -(x & 0x03))<<1;
					alpha=cTab->colors[(*bits >> shift) & 0x3].alpha;
					//ExpandColor(cTab->colors[(*bits >> shift) & 0x3], c);
				} break;

				case bm4Bit: {
					U8  *bits = rowAddr + (x>>1);
					int shift = (1 - (x & 0x01))<<2;
					alpha=cTab->colors[(*bits >> shift) & 0xF].alpha;
					//ExpandColor(cTab->colors[(*bits >> shift) & 0xF], c);
				} break;

				case bm8Bit: {
					alpha=cTab->colors[rowAddr[x]].alpha;
					//ExpandColor(cTab->colors[rowAddr[x]], c);
				} break;

				case bm16Bit: {
					alpha=255;					
					//UnpackPix16(pix, c);
				} break;

				case bm32Bit: {
					//*((XU32*)c) = *((U32 *)rowAddr + x);
					alpha= (*((U32 *)rowAddr + x))>>24;
					//cTab->colors[rowAddr[x]]
					//UnpackPix32(pix, c);
				} break;
			}
			if(alpha==0xff)
			{
				if(!bStart)
				{
					iStart=x;
					bStart=true;
				}
			}
			else if(bStart)
			{
				if(x-iStart>lineData[1]-lineData[0])
				{
					lineData[0]=iStart;
					lineData[1]=x;
				}
				bStart=false;
			}

		}
		if(bStart)
		{
			lineData[0]=iStart;
			lineData[1]=width;
		}
		rowAddr+=rowBytes;
		lineData+=2;
	}
}*/

void SBitmapCore::ReleaseData()
{
	if(pAlloc)
	{
#ifdef _WINEMU_BMTRACE
		if(IsRunning())
		{
			XString16 s("[Memory] Free image memroy ");
			XString16 t;
			t.FromInt(rowBytes*height);
			s+=t;
			s+=XString16(" Bytes");
			_tracePlayer->Trace(s);
		}
#endif
		delete pAlloc;
		pAlloc=0;
		_RemoveReport(this);
	}
	baseAddr = NULL;
}

void SBitmapCore::PIFree()
{
	//delete [] cTab;//free(cTab);
	//cTab = 0;
	//record.ReleaseAll();
	//if(!isAttach)
	if(bitmapID&&bAdded)
	{
		m_pPlayer->RemoveBitmap(this);
		bAdded = false;
	}
	RasterGL::sharedRasterGL()->DeleteTexture(bitmapID);
	bitmapID = 0;
	
#ifdef _DEBUG
	//if(lockCount)
	//	int v=0;
#endif
	
//	if(m_pBitmapData)
//		m_pPlayer->m_pAVM2->RemoveObject(m_pBitmapData);
	if(pAlloc)//&&!isAttach)
	{
#ifdef _WINEMU_BMTRACE
		if(IsRunning())
		{
			XString16 s("[Memory] Free image memroy ");
			XString16 t;
			t.FromInt(rowBytes*height);
			s+=t;
			s+=XString16(" Bytes");
			_tracePlayer->Trace(s);
		}
#endif
		delete pAlloc;
		pAlloc=0;
		_RemoveReport(this);
	}
	//if(lineAlpha)
	//	XXVar::_FreeData(lineAlpha);
	//lineAlpha=XNULL;
	baseAddr=XNULL;
//	lockCount=0;
//#if (__CORE_VERSION__>=0x02074000)
///*	if(m_pDib&&!attachDib)
//	{
//		m_pDib->Release();
//		delete m_pDib;
//	}
//	m_pDib=NULL;*/
//#endif
	/*if ( baseAddr ) {
		free(baseAddr);
		baseAddr = 0;
	}*/
}

#ifdef BIG_ENDIAN
// Big-endian system: Defined as an empty inline function.
#define SBitmapSwapBits()
#else //elif _ARGB_PIXEL_
void SBitmapSwapBits(void* data, S32 bytes, int bmFormat)
{
	/*if ( bmFormat == bm16Bit ) {
		U8 * w = (U8*)data;
		U8 tmp;
		for ( S32 i = bytes/4; i--; ) {
			tmp = w[0];
			w[0] = w[1];
			w[1] = tmp;
			w += 2;

			tmp = w[0];
			w[0] = w[1];
			w[1] = tmp;
			w += 2;
		}
	} else*/
	if ( bmFormat == bm32Bit ) 
	{
		XU32* w=(XU32*)data;
		for(S32 i=0;i<bytes/4;i++)
		{
			w[i]=((w[i]&0xff)<<24)|(w[i]>>8);
		}
		/*U8 * w = (U8*)data;
		U8 tmp;
		for ( S32 i = bytes/4; i--; ) {
			tmp = w[3];
			w[3]=w[0];
			w[0]=w[1];
			w[1]=w[2];
			w[2]=tmp;//w[3];
			//w[0]=w[3];
			
			
			//w[3]=0xff;
			w += 4;
		}*/
	}
}
#endif
/*#else
// Little-endian system: Swap the bits within this function.
void SBitmapSwapBits(void* data, S32 bytes, int bmFormat)
{
	if ( bmFormat == bm16Bit ) {
		U8 * w = (U8*)data;
		U8 tmp;
		for ( S32 i = bytes/4; i--; ) {
			tmp = w[0];
			w[0] = w[1];
			w[1] = tmp;
			w += 2;

			tmp = w[0];
			w[0] = w[1];
			w[1] = tmp;
			w += 2;
		}
	} else if ( bmFormat == bm32Bit ) {
		U8 * w = (U8*)data;
		U8 tmp;
		for ( S32 i = bytes/4; i--; ) {
			tmp = w[0];
			w[0] = w[3];
			w[3] = tmp;
			tmp = w[1];
			w[1] = w[2];
			w[2] = tmp;
			//w[3]=0xff;
			w += 4;
		}
	}
}
#endif*/

//PLAYER

//
// Pixel Packing support
//
//#ifdef _ARGB_PIXEL_
#define Epnd16R(x)  (((x)<<3)|0x7)
#define Epnd16(x) ((Epnd16R(x)<<16)|0xff000000L)	// add the alpha value
#define Epnd16G(x)  (Epnd16R(x)<< 8)
//#else
//#define Epnd16(x)  (((x)<<3)|0x7)
//#define Epnd16R(x) ((Epnd16(x)<<16)|0xff000000L)	// add the alpha value
//#define Epnd16G(x)  (Epnd16(x)<< 8)
//#endif
const int pix16Expand[32] = {
		0, 			  Epnd16(0x01), Epnd16(0x02), Epnd16(0x03),
		Epnd16(0x04), Epnd16(0x05), Epnd16(0x06), Epnd16(0x07),
		Epnd16(0x08), Epnd16(0x09), Epnd16(0x0A), Epnd16(0x0B),
		Epnd16(0x0B), Epnd16(0x0D), Epnd16(0x0E), Epnd16(0x0F),
		Epnd16(0x10), Epnd16(0x11), Epnd16(0x12), Epnd16(0x13),
		Epnd16(0x14), Epnd16(0x15), Epnd16(0x16), Epnd16(0x17),
		Epnd16(0x18), Epnd16(0x19), Epnd16(0x1A), Epnd16(0x1B),
		Epnd16(0x1B), Epnd16(0x1D), Epnd16(0x1E), Epnd16(0x1F),
	};


const U32 pix16ExpandR[32] = {
		0xFF000000L,   Epnd16R(0x01), Epnd16R(0x02), Epnd16R(0x03),
		Epnd16R(0x04), Epnd16R(0x05), Epnd16R(0x06), Epnd16R(0x07),
		Epnd16R(0x08), Epnd16R(0x09), Epnd16R(0x0A), Epnd16R(0x0B),
		Epnd16R(0x0B), Epnd16R(0x0D), Epnd16R(0x0E), Epnd16R(0x0F),
		Epnd16R(0x10), Epnd16R(0x11), Epnd16R(0x12), Epnd16R(0x13),
		Epnd16R(0x14), Epnd16R(0x15), Epnd16R(0x16), Epnd16R(0x17),
		Epnd16R(0x18), Epnd16R(0x19), Epnd16R(0x1A), Epnd16R(0x1B),
		Epnd16R(0x1B), Epnd16R(0x1D), Epnd16R(0x1E), Epnd16R(0x1F),
	};


const U32 pix16ExpandG[32] = {
		0, 			   Epnd16G(0x01), Epnd16G(0x02), Epnd16G(0x03),
		Epnd16G(0x04), Epnd16G(0x05), Epnd16G(0x06), Epnd16G(0x07),
		Epnd16G(0x08), Epnd16G(0x09), Epnd16G(0x0A), Epnd16G(0x0B),
		Epnd16G(0x0B), Epnd16G(0x0D), Epnd16G(0x0E), Epnd16G(0x0F),
		Epnd16G(0x10), Epnd16G(0x11), Epnd16G(0x12), Epnd16G(0x13),
		Epnd16G(0x14), Epnd16G(0x15), Epnd16G(0x16), Epnd16G(0x17),
		Epnd16G(0x18), Epnd16G(0x19), Epnd16G(0x1A), Epnd16G(0x1B),
		Epnd16G(0x1B), Epnd16G(0x1D), Epnd16G(0x1E), Epnd16G(0x1F),
	};


XBOOL SBitmapCore::PICreate(XStream *pStream)
{
	XU32 nSize;
	if(!pStream->ReadDWord(nSize)) return XFALSE;
	if(nSize<=0||nSize>pStream->GetDataSize())
		nSize=pStream->GetDataSize();
	XImage image;
	//int nSize=pStream->GetDataSize();
	//if(nSize<=0) return XFALSE;
	XU32 as=pStream->GetDataSize();
	as-=nSize;

	XU8*pData=pStream->ReadAddr();

	XU8 bHeader=XFALSE;
	while(nSize>0)
	{
		XU16 nCode=*(XU16*)pData;
		XU16 s=2;
		XU8 bSkip=XFALSE;
		switch(nCode)
		{
		case 0xDAFF:
			 goto ImageData;
		case 0xD8FF:
			 if(!bHeader)
				 bHeader=XTRUE;
			 else
				 bSkip=XTRUE;
			 break;
		case 0xD9FF:
			 bSkip=XTRUE;
			 break;
		default:
			 s+=((pData[2]<<8)|pData[3]);
			 if(nSize<=s) return XFALSE;
		}
		//if(nCode==0xDAFF) break;
		if(!bSkip)
		{
			if(!image.Append(pData,s)) return XFALSE;
		}
		nSize-=s;
		pData+=s;
		pStream->Skip(s);
	}
	if(nSize<=0) return XFALSE;
ImageData:

	//XFile file;
	//file.Open("D:\\test.jpg",XFile::XCREATE|XFile::XBINARY|XFile::XWRITE);
	//file.Write(pData,nSize);
	//file.Close();

	if(!image.Append(pData,nSize)) return XFALSE;

	pStream->Skip(nSize);

	if(!image.Final(XTRUE)) return XFALSE;
	int w=image.Width();
	int h=image.Height();
	//if(!bitmapID)
	//	m_pPlayer->AddBitmap(this);
	/*if(w*h>2000*2000)
	{
		return CreateEmpty();
		//return XTRUE;
	}*/
	CalcImageDiv(w,h);
 
	width0=w;
	height0=h;
	if(nImgDiv)
	{
		w=width0>>nImgDiv;
		h=height0>>nImgDiv;
	}
	
	//else nImgDiv=0;

	transparent = true;//( HasTransparent(c) != 0 );

	bmFormat = bm32Bit;
	width = w;
	height = h;
	rowBytes = SBitmapCalcRowbytes(bmFormat, width);

	S32 size = (S32)rowBytes*height;

	//S32 size = (S32)rowBytes*height;
	pAlloc=(char*)new XU8[size];

	if(pAlloc==XNULL) return XFALSE;

	_AddReport(this);

	baseAddr=pAlloc;
	//baseAddr=pAlloc+tabSize;
	if(nImgDiv)
		image.GetBitmapDataSkip((XU8*)pAlloc,width,height,32,nImgDiv);
	else
		image.GetBitmapData((XU8*)pAlloc,width,height,32,size);

	if(as>0)
	{
		XStream out;
		out.SetSize(image.Width()*image.Height());
		XZLib lib;
		lib.Init(pStream,&out);
		lib.Decode(XNULL);
		//lib.Release();
		nSize=w*h;
		nSize=XMIN(nSize,out.GetDataSize());
		XU8*pDst=(XU8*)baseAddr;
		XU8*pSrt=out.ReadAddr();
		if(this->nImgDiv)
		{
			int y,x,dw=image.Width()<<nImgDiv;
			pSrt+=((dw>>1))+(1<<(nImgDiv-1));
			for(y=0;y<h;y++)
			{
				for(x=0;x<w;x++)
				//for(XU32 i=0;i<nSize;i++)
				{
					XU8 a=pSrt[x<<nImgDiv];
					/*switch(a)
					{
					case 0:
					case 0xff:break;
					default:
					//if(a<0xff)
					//{
						if(pDst[0]>a) pDst[0]=a;
						if(pDst[1]>a) pDst[1]=a;
						if(pDst[2]>a) pDst[2]=a;
						break;
					}*/
					//}
					pDst[3]=a;
					pDst+=4;
					//((XU8*)&pDst[i])[3]=pSrt[i];
				}
				pSrt+=dw;
			}
		}
		else
		{
			for(XU32 i=0;i<nSize;i++)
			{
				XU8 a=pSrt[i];
				//if(a<0xff)
				/*switch(a)
				{
				case 0:
				case 0xff:
					break;
				default:
					if(pDst[0]>a) pDst[0]=a;
					if(pDst[1]>a) pDst[1]=a;
					if(pDst[2]>a) pDst[2]=a;
					break;
				}*/
				pDst[3]=a;
				pDst+=4;

				//((XU8*)&pDst[i])[3]=pSrt[i];
			}
		}
	}
	SwapRB();
	preAlpha=false;
/*	if(nBlendMode)
	{
		int nMode=nBlendMode;
		nBlendMode=0;
		SetBlendMode(nMode);
	}*/
	return XTRUE;
}

//#if (__CORE_VERSION__>=0x02075000)
//
///*void SBitmapCore::SetBlockDiv(int w,int h)
//{
//	if(nImgDiv) return;
//	if(!m_pPlayer||!m_pPlayer->m_pDomFilter) return;
//	if(w==m_pPlayer->m_pDomFilter->m_nBlockWidth&&
//	   h==m_pPlayer->m_pDomFilter->m_nBlockHeight)
//	   nImgDiv=1;
//}*/
//
//#endif

XBOOL SBitmapCore::PICreate()
{
	if(width<=0||height<=0) return XFALSE;
	return PICreate(width,height,0);
}

XBOOL SBitmapCore::CopyFrom(SBitmapCore*bits)
{
	if(bits->bmFormat == bm32Bit)
	{
		PICreate(bits->width,bits->height,bits->nImgDiv);
		memcpy(baseAddr,bits->baseAddr,width*height*4);
	}
	else
	{
		XDrawDib dib;
		dib.CreateFrom(bits,true);
		PICreate(dib,bits->transparent,NULL);
		pAlloc = (char*)dib.m_pDib;
		dib.SetAttach(true);
	}
	transparent = bits->transparent;
	return XTRUE;
}

XBOOL SBitmapCore::PICreate(int w, int h,int nDiv)
{
		transparent = true;//( HasTransparent(c) != 0 );
		nImgDiv = nDiv;
		bmFormat = bm32Bit;
		//width0=w;
		//height0=h;
		width0 = w<<nDiv;
		height0 = h<<nDiv;
		width = w;
		height = h;
		rowBytes = SBitmapCalcRowbytes(bmFormat, width);

		S32 size = (S32)rowBytes*height;

		//S32 size = (S32)rowBytes*height;
		pAlloc=(char*)new XU8[size];

		if(pAlloc==XNULL) return XFALSE;

		_AddReport(this);

		baseAddr=pAlloc;
		memset(baseAddr,0,size);
		return XTRUE;
}

XBOOL SBitmapCore::PICreate(U8*pData,int l)
{
		if(!pData)
			return XFALSE;

		XImage image;
		image.Append(pData,l);
		if(!image.Final(true)) return XFALSE;
		int w=image.Width(),h=image.Height();

//#if (__CORE_VERSION__>=0x02075000)
////		SetBlockDiv(w,h);
//#endif
		//if(!bitmapID)
		//	m_pPlayer->AddBitmap(this);
		/*if(w*h>2000*2000)
		{
			return CreateEmpty();
			//return XTRUE;
		}*/
		CalcImageDiv(w,h);
		width0 = w;
		height0 = h;
		if(nImgDiv)
		{
			w = w>>nImgDiv;
			h = h>>nImgDiv;
		}
	//	int nBits=image.GetPixelBits();
		//if(nBits<8||nBits>32) return XFALSE;
		transparent = false;//( HasTransparent(c) != 0 );

		bmFormat = bm32Bit;
		//width0=w;
		//height0=h;
		
		width = w;
		height = h;
		rowBytes = SBitmapCalcRowbytes(bmFormat, width);

		S32 size = (S32)rowBytes*height;

		//S32 size = (S32)rowBytes*height;
		pAlloc=(char*)new XU8[size];

		if(pAlloc==XNULL) return XFALSE;

		_AddReport(this);

		baseAddr=pAlloc;
		//return XTRUE;
		//baseAddr=pAlloc+tabSize;
		if(nImgDiv)
			image.GetBitmapDataSkip((XU8*)pAlloc,width,height,32,nImgDiv);
		else
			image.GetBitmapData((XU8*)pAlloc,width,height,32,size);

		RGB8*pBuf=(RGB8*)baseAddr;
		//memset(pBuf,0,size);
		size=w*h;
		for(int i=0;i<size;i++)
		{
			if(pBuf[i].alpha<0xff)
			{
				transparent = true;
				break;
			}
		}
		//		int a=pBuf[i].alpha;
		//		//if(a>=0xff) continue;
		//if(!nBlendMode)
		//{
		//	for(int i=0;i<size;i++)
		//	{
		//		int a=pBuf[i].alpha;
		//		//if(a>=0xff) continue;
		//		switch(a)
		//		{
		//		case 0xff:break;
		//		case 0:
		//			transparent=true;
		//			break;
		//		default:
		//			transparent=true;
		//		//if(pBuf[i].red>a) pBuf[i].red=a;
		//		//if(pBuf[i].blue>a) pBuf[i].blue=a;
		//		//if(pBuf[i].green>a) pBuf[i].green=a;
		//			pBuf[i].red=(pBuf[i].red*a)>>8;
		//			pBuf[i].green=(pBuf[i].green*a)>>8;
		//			pBuf[i].blue=(pBuf[i].blue*a)>>8;
		//			break;
		//		}
		//	}
		//}
		this->preAlpha=false;

#ifdef _WINEMU_BMTRACE
		if(pAlloc)
		{
			XString16 s("[Memory] Alloc image memroy ");
			XString16 t;
			t.FromInt(rowBytes*height);
			s+=t;
			s+=XString16(" Bytes");
			m_pPlayer->Trace(s);
		}
#endif
		SwapRB();
		/*if(nBlendMode)
		{
			int nMode=nBlendMode;
			nBlendMode=0;
			SetBlendMode(nMode);
		}*/
		return XTRUE;

}



XBOOL SBitmapCore::PICreate(void*p,int nSize,XU8Array *pTab)
{
	XImage image;
	//int nSize=pStream->GetDataSize();
	if(nSize<=0) return XFALSE;

	XU8*pData=(XU8*)p;//pStream->ReadAddr();

	XU16 nCode=*(XU16*)pData;
	/*if(nCode==0xD9FF)
	{							  		pData+=4;
		nSize-=4;

	}*/
//	XFile file;
//	file.Open("D:\\Error.jpg",XFile::XCREATE|XFile::XWRITE|XFile::XBINARY);


	//if(pTab!=XNULL)
	{
		XU8 bHeader=XFALSE;
		while(nSize>0)
		{
			nCode=*(XU16*)pData;
			XU16 s=2;
			XU8 bSkip=XFALSE;
			switch(nCode)
			{
			case 0xDAFF:
				 goto ImageData;
			case 0xD8FF:
				 if(!bHeader)
					 bHeader=XTRUE;
				 else
					 bSkip=XTRUE;
				 break;
			case 0xD9FF:
				 bSkip=XTRUE;
				 break;
			default:
				 s+=((pData[2]<<8)|pData[3]);
				 if(nSize<=s) return XFALSE;
			}
			//if(nCode==0xDAFF) break;
			if(!bSkip)
			{
				//file.Write(pData,s);
				if(!image.Append(pData,s)) return XFALSE;
			}
			nSize-=s;
			pData+=s;
		}
		if(nSize<=0) return XFALSE;
	ImageData:
		//if(!image.Append(pStream->ReadAddr(),nSize)) return XFALSE;
		if(pTab!=XNULL)
		{
			//file.Write(pTab->GetData()+2,pTab->GetSize()-4);
			if(!image.Append(pTab->GetData()+2,pTab->GetSize()-4)) return XFALSE;
		}
	}


	//file.Write(pData,nSize);
	//file.Close();
	if(!image.Append(pData,nSize)) return XFALSE;


	if(!image.Final(XTRUE)) return XFALSE;
	int w=image.Width();
	int h=image.Height();
	//if(!bitmapID)
	//	m_pPlayer->AddBitmap(this);
	/*if(w*h>2000*2000)
	{
		return CreateEmpty();
		//return XTRUE;
	}*/
	CalcImageDiv(w,h);
	width0 = w;
	height0 = h;
	if(nImgDiv)
	{
		w>>=nImgDiv;
		h>>=nImgDiv;
	}
//	int nBits=image.GetPixelBits();
	//if(nBits<8||nBits>32) return XFALSE;
	transparent = false;//( HasTransparent(c) != 0 );

	bmFormat = bm32Bit;

	width = w;
	height = h;


	rowBytes = SBitmapCalcRowbytes(bmFormat, width);

	S32 size = (S32)rowBytes*height;

	//S32 size = (S32)rowBytes*height;
	pAlloc=(char*)new XU8[size];

	if(pAlloc==XNULL) return XFALSE;

	_AddReport(this);

	baseAddr=pAlloc;
	//baseAddr=pAlloc+tabSize;
	if(nImgDiv)
		image.GetBitmapDataSkip((XU8*)pAlloc,width,height,32,nImgDiv);
	else
		image.GetBitmapData((XU8*)pAlloc,width,height,32,size);
	/*if(nBlendMode)
	{
		int nMode=nBlendMode;
		nBlendMode=0;
		SetBlendMode(nMode);
	}*/
	SwapRB();
	return XTRUE;
}



inline void SBitmapCore::CalcImageDiv(int w,int h)
{
	//if(!m_pPlayer||m_pPlayer->	
	nImgDiv = 0;
	while(w>MAXTEXTURE||h>MAXTEXTURE)
	{
		nImgDiv++;
		w>>=1;
		h>>=1;
		//break;
	}
	//if(nDiv>this->nImgDiv)
	//	nImgDiv=nDiv;
}

void SBitmapCore::DivImage()
{
	if(!nImgDiv) return;
	if(!pAlloc) {nImgDiv=0;return;}
	if(width<MINDIVSIZE||height<MINDIVSIZE){nImgDiv=0;return;}
	if(bmFormat<bm8Bit){nImgDiv=0;return;}
	int nw=width0>>nImgDiv;
	int nh=height0>>nImgDiv;

	int newRowBytes = SBitmapCalcRowbytes(bmFormat, nw);

	int tabSize=0;
	S32 size = (S32)newRowBytes*height;

	if ( bmFormat <= bm8Bit ) {
		tabSize=sizeof(_XCOLORTABLE);
		size+=tabSize;
	}

	_RemoveReport(this);
	//S32 size = (S32)rowBytes*height;
	char* pNew=(char*)new XU8[size];

	if(pNew==XNULL){nImgDiv=0;return;}
	char* newBase=pNew;
	//baseAddr=pAlloc+tabSize;
	if(bmFormat<=bm8Bit)
	{
		newBase+=tabSize;
		memcpy(pNew,cTab,tabSize);
		cTab=(_XCOLORTABLE*)pNew;
	}
	int iSkip=1<<nImgDiv;
	switch(bmFormat)
	{
	case bm8Bit:
		{
			int x,y;
			XU8*dst=(XU8*)newBase;
			XU8*srt=(XU8*)baseAddr+(iSkip>>1);
			int iSkipRow=rowBytes*iSkip;
			for(y=0;y<nh;y++)
			{
				for(x=0;x<nw;x++)
				{
					dst[x]=srt[x<<nImgDiv];
				}
				dst+=newRowBytes;
				srt+=iSkipRow;
			}
		}
		 break;
	case bm16Bit:
		{
			int x,y;
			XU16*dst=(XU16*)newBase;
			XU16*srt=(XU16*)baseAddr+(iSkip>>1);
			int iSkipRow=(rowBytes*iSkip)>>1;
			for(y=0;y<nh;y++)
			{
				for(x=0;x<nw;x++)
				{
					dst[x]=srt[x<<nImgDiv];
				}
				dst+=(newRowBytes>>1);
				srt+=iSkipRow;
			}
		}
		 break;
	case bm32Bit:
		{
			int x,y;
			XU32*dst=(XU32*)newBase;
			XU32*srt=(XU32*)baseAddr+(iSkip>>1);
			int iSkipRow=(rowBytes*iSkip)>>2;
			for(y=0;y<nh;y++)
			{
				for(x=0;x<nw;x++)
				{
					dst[x]=srt[x<<nImgDiv];
				}
				dst+=(newRowBytes>>2);
				srt+=iSkipRow;
			}
		}
		 break;
	}
	delete pAlloc;
	/*if(m_pDib)
	{
		if(!attachDib)
		{
			m_pDib->Release();
			delete m_pDib;
		}
		m_pDib=XNULL;
	}*/
	rowBytes=newRowBytes;
	width=nw;
	height=nh;
	baseAddr=newBase;
	pAlloc=pNew;
	_AddReport(this);
}

/*void SBitmapCore::SetBlendMode(int nMode)
{
	//return;
	if(nBlendMode||!nMode) return;
	nBlendMode=nMode;
	if(!(m_pPlayer->m_pDomFilter&&
	     m_pPlayer->m_pDomFilter->bEnuBlendMode))
		 return;
	this->transparent=true;
	//if(!this->baseAddr) return;
	switch(bmFormat)
	{
	case bm1Bit:
	case bm2Bit:
	case bm4Bit:
	case bm8Bit:
		if(this->cTab)
		{
			for(int i=0;i<cTab->n;i++)
			{
				if(cTab->colors[i].alpha)
				{
					int argb=cTab->colors[i].red+
							 cTab->colors[i].green+
							 cTab->colors[i].blue+
							 cTab->colors[i].red;
					//argb>>=1;
					if(argb<0xff)
					{
						switch(nMode)
						{
						case drawBlendLighten:
							//argb/=2;
							argb=(argb*argb)>>8;
						default:
							cTab->colors[i].alpha=argb;
							cTab->colors[i].red=(cTab->colors[i].red*argb)>>8;
							cTab->colors[i].green=(cTab->colors[i].green*argb)>>8;
							cTab->colors[i].blue=(cTab->colors[i].blue*argb)>>8;
						}
					}
				}
			}
		}break;
	case bm16Bit:
		break;
	case bm32Bit:
		if(baseAddr)
		{
			int i,nCount=this->width*height;
			RGB8* pRgb=(RGB8*)baseAddr;
			for(i=0;i<nCount;i++)
			{
				if(pRgb[i].alpha)
				{
					int argb=pRgb[i].red+pRgb[i].red+pRgb[i].blue+pRgb[i].green;
					//argb>>=1;
					if(argb<0xff)
					{
						switch(nMode)
						{
						case drawBlendLighten:
							argb=(argb*argb)>>8;
						default:
							pRgb[i].alpha=argb;
							pRgb[i].red=(pRgb[i].red*argb)>>8;
							pRgb[i].green=(pRgb[i].green*argb)>>8;
							pRgb[i].blue=(pRgb[i].blue*argb)>>8;
							break;
						}
					}
				}
			}
		}break;
	}
}*/

void SBitmapCore::SetDib(XDib &dib)
{
	if(dib.GetBits()!=32) return;
	int x,y;
	XU32*pColor=(XU32*)dib.GetBuffer();
	XU8*rowAddr=(XU8*)baseAddr;
//	int h=XMIN((int)dib.Height(),height);
	int w=XMIN((int)dib.Width(),width);
	int lb=dib.Width();//m_nLinePitch;
	for(y=0;y<height;y++)
	{
		switch ( bmFormat )
		{
			case bm1Bit:
			{
				for(x=0;x<w;x++)
				{
					U8  *bits = rowAddr + (x>>3);
					int shift = 7 - (x & 0x07);
					//int id=(*bits >> shift) & 1;
					pColor[x]=*((XU32*)&(cTab->colors[(*bits >> shift) & 1]));
					//ExpandColor(cTab->colors[(*bits >> shift) & 1], c);
				}
			} break;

			case bm2Bit:
			{
				for(x=0;x<w;x++)
				{
					U8  *bits = rowAddr + (x>>2);
					int shift = (3 -(x & 0x03))<<1;
					pColor[x]=*((XU32*)&(cTab->colors[(*bits >> shift) & 0x3]));
				}
			} break;

			case bm4Bit: {
				for(x=0;x<w;x++)
				{
					U8  *bits = rowAddr + (x>>1);
					int shift = (1 - (x & 0x01))<<2;
					pColor[x]=*((XU32*)&(cTab->colors[(*bits >> shift) & 0xF]));
				}
			} break;

			case bm8Bit: {
				for(x=0;x<w;x++)
					pColor[x]=*((XU32*)&(cTab->colors[rowAddr[x]]));
			} break;

			case bm16Bit:
			{
				for(x=0;x<w;x++)
				{
					U16 pix = *((U16 *)rowAddr + x);

					pColor[x]=(0xff<<24)|
							  (pix16Expand[(pix>>10)&0x1f]<<16)|
							  (pix16Expand[(pix>> 5)&0x1f]<<8)|
							  pix16Expand[(pix    )&0x1f];

					//UnpackPix16(pix, c);
				}
			} break;

			case bm32Bit: {
				XGlobal::Memcmp(pColor,rowAddr,w<<2);
			} return;
		}
		rowAddr+=rowBytes;
		pColor+=lb;
	}
}

XBOOL SBitmapCore::CreateEmpty()
{
	baseAddr = 0;
	pAlloc=0;
	cTab = 0;
//	lockCount = 0;
	transparent = 0;//bAlpha;//( HasTransparent(c) != 0 );

	bmFormat = bm32Bit;
	//width0 = 24;
	//height0 = 24;
	nImgDiv = 0;
	width0 = 24;
	height0 = 24;
	width = 24;
	height = 24;
	rowBytes = SBitmapCalcRowbytes(bmFormat, width);
	S32 size = (S32)rowBytes*height;
	//S32 size = (S32)rowBytes*height;
	pAlloc=(char*)new XU8[size];
	if(pAlloc==XNULL) return XFALSE;
	_AddReport(this);
	XGlobal::Memset(pAlloc,0,size);
	return XTRUE;
}

void SBitmapCore::CalcTrans()
{

	transparent=XFALSE;
	if(!baseAddr||bmFormat!=bm32Bit) return;
	int nSize=width*height;
	XU8*alpha=(XU8*)baseAddr+3;
	for(int i=0;i<nSize;i+=4)
	{
		if(alpha[i]<0xff)
		{
			transparent=XTRUE;
			//break;
			//XU8 v=alpha[i];
			//alpha[i-1]=0;
			//alpha[i-2]=0;
			//alpha[i-3]=0;
			//if(alpha[i-1]>v) alpha[i-1]=v;
			//if(alpha[i-2]>v) alpha[i-2]=v;
			//if(alpha[i-3]>v) alpha[i-3]=v;
			//break;
		}
	}
}

void SBitmapCore::PICreate(SBitmapCore*bits)
{
	PICreate(bits->width,bits->height,bits->nImgDiv);
	memcpy(baseAddr,bits->baseAddr,bits->rowBytes*bits->height);
}

void SBitmapCore::PICreate(XDib &dib,XBOOL bTrans,void*pData)
{
	nImgDiv=0;
	/*if(pData)
	{
		//if(baseAddr&&pData==m_pBitmapData) return;
		m_pBitmapData=pData;
		m_pPlayer->AddBitmap(this);
		m_pPlayer->m_pAVM2->PushObject(pData);
	}*/
	baseAddr = (char*)dib.GetBuffer();
	pAlloc=0;
	cTab = (SColorTable*)(((char*)dib.GetColorTable())-1);
//	lockCount = 0;
	transparent = bTrans;//( HasTransparent(c) != 0 );
	bmFormat = bm32Bit;//format;
	width = dib.Width();
	height = dib.Height();
//	width0 = width;
//	height0 = height;
	rowBytes = SBitmapCalcRowbytes(bmFormat, width);
/*	if(nBlendMode)
	{
		int nMode=nBlendMode;
		nBlendMode=0;
		SetBlendMode(nMode);
	}*/
	return;
}

//
// SBitmapCore
//
#include "AS3BitmapData.h"   // modify by hgl, 头文件声明放到后面，防止链接错误
void SBitmapCore::LockBits()
{
	//if(width==1280&&height==1259)
	//	int v=0;
	//this->m_nLockTime=m_pPlayer->m_nRunTime;
	/*if(this->pAlloc==NULL&&this->m_pBitmapData)
	{
		BitmapDataObject*pBmp=(BitmapDataObject*)m_pBitmapData;
		pBmp->m_nLockTime=m_nLockTime;
		pBmp->m_sbitmap.lockCount++;
	}*/
	//lockCount++;
#ifdef _WINEMU
	if(!bitmapID&&IsRunning())//(!this->baseAddr&&!bitmapID)&&IsRunning())
	{
		XString16 s("Waring:Lock empty image!");
		_tracePlayer->Trace(s);
	}
	if(bitmapID)
	{
		RasterGL::LockTexture(bitmapID);
		RasterGL::SetLastTime(bitmapID,m_pPlayer->m_nRunTime);
		//bitmapID->nLockCount++;
		//bitmapID->nLastTime = m_pPlayer->m_nRunTime;
	}
#endif
}

void SBitmapCore::UnlockBits()
{
	//if(width==1280&&height==1259)
	//	int v=0;

	/*if(this->pAlloc==NULL&&this->m_pBitmapData)
	{
		BitmapDataObject*pBmp=(BitmapDataObject*)m_pBitmapData;
		pBmp->m_nLockTime=m_nLockTime;
		pBmp->m_sbitmap.lockCount--;
	}*/
#ifdef _WINEMU
	if(!RasterGL::IsLocked(bitmapID)&&IsRunning())
	{
		XString16 s("Waring:Unlock unlocked image!("),tmp;
		tmp.FromInt(width);
		s+=tmp;
		s+=XString16("x");
		tmp.FromInt(height);
		s+=tmp;
		s+=XString16(")");
		_tracePlayer->Trace(s);
	}
#endif
	RasterGL::UnLockTexture(bitmapID);
//	if(bitmapID&&!bitmapID->nUseCount&&bitmapID->type)
//		int v=0;
	//if(bitmapID&&bitmapID->nLockCount)
	//		bitmapID->nLockCount--;
}

void SBitmapCore::FormatBitmap()
{
	if(bmFormat!=bm8Bit||!cTab) return;
	_RemoveReport(this);
	XU8*pNewAlloc=new XU8[width*height*4];
	XU32*pTab=(XU32*)cTab->colors;
	XU8*pSrc=(XU8*)baseAddr;
	XU32*pDst=(XU32*)pNewAlloc;
	int x,y;
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			pDst[x]=pTab[pSrc[x]];
		}
		pSrc+=rowBytes;
		pDst+=width;
	}
	if(pAlloc)
		delete pAlloc;
	rowBytes=width*4;
	bmFormat=bm32Bit;
	pAlloc=(char*)pNewAlloc;
	baseAddr=(char*)pNewAlloc;
	_AddReport(this);
}

void SBitmapCore::CreateTexture(bool bAdd)
{
	if(bitmapID) return;
	if(bAdd)
	{
		bAdded = true;
		//if(m_pPlayer->m_bitmaps==this)
		//	int v=0;
		m_pPlayer->AddBitmap(this);
	}
	FormatBitmap();
	RasterGL* raster = RasterGL::sharedRasterGL();
	//width0 = width;
	//height0 = height;
	switch(bmFormat)
	{
	default:
	case bm32Bit:
		 bitmapID = raster->texInitWithData(baseAddr,Texture2DPixelFormat_RGBA8888,
			 width,height);
		 break;
	case bm16Bit:
		 bitmapID = raster->texInitWithData(baseAddr,Texture2DPixelFormat_RGB565,
			 width,height);
		 break;
	case bm8Bit:
		 bitmapID = raster->texInitWithData(baseAddr,Texture2DPixelFormat_A8,
			 width,height);
		 break;
	}
}


void SBitmapCore::SwapRB()
{
#ifdef __ANDROID__
	return;
#endif

	if(bmFormat!=bm32Bit||!baseAddr) return;
	RGB8* pRgb=(RGB8*)baseAddr;
	int i,l=width*height;
	int t;
	for(i=0;i<l;i++)
	{
		t=pRgb->red;
		pRgb->red=pRgb->blue;
		pRgb->blue=t;
		pRgb++;
	}
}
