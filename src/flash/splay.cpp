/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc.
****************************************************************************/
//	990325	mnk	"fixed1" -> "fixed_1"
#include "StdAfxflash.h"

#include <stddef.h>
#include <string.h>
#include <math.h>

#include "splay.h"

//#include "Bitbuf.h"
#include "stags.h"
#include "sobject.h"
#include "splayer.h"
#include "morphinter.h"
#include "edittext.h"
#include "flashmemcop.h"
#include "XFilter.h"
#include <stdio.h>
#include "ObjectCreator.h"
#include "XDrawObj.h"
#include "XActionScript.h"
#include "XXObjectLoadVars.h"
#include "avmplus.h"
#include "avm2.h"
// Simple Parser

#include "ShellCore.h"
#include "AS3MovieClip.h"
#include "AS3FrameLabel.h"
#include "AS3Scene.h"
#include "AS3Loader.h"
#include "AS3LoaderInfo.h"
#include "AS3Event.h"
#include "AS3BitmapData.h"
#include "AS3Bitmap.h"
#include "Raster_gl.h"
//
#include "7zwrapper.h"
#include "BwMatrix.h"
#include "BWShapeToGL.h"

//#ifdef _WINEMU
//extern void* _pDebugP;
//#endif

#ifdef _DEBUG
static XU32 _nCloneSprites=0;
#endif

int SParser::GetTag(S32 len)
{
	if(pos<0) return -1;
	tagPos = pos;
	if ( len-pos < 2 )
		return -1;	// we need more data before we can process this tag

	tagCode = GetWord();
//	if(tagCode==0x15ff&&script[pos]==61)
//		int v=0;
	S32 tagLen = tagCode & 0x3f;
	if ( tagLen == 0x3f ) {
		if ( len-pos < 4 ) {
			pos = tagPos;
			return -1;	// we need more data before we can process this tag
		}
		tagLen = GetDWord();
	}
	tagEnd = pos + tagLen;
	if ( tagEnd > len ) {
		pos = tagPos;
		return -1;	// we need more data before we can process this tag
	}
	tagCode = tagCode >> 6;
	return tagCode;
}

S32 SParser::GetData(void * data, S32 len)
// returns the actual number of bytes read could
//	be less than len if we hit the end of a tag
{
 	S32 n = Min(len, tagEnd-pos);
// 	hmemcpy(data, script+pos, n);
	memcpy(data, script+pos, n);
	pos+=n;
	return n;
}

void SParser::GetColor(RGBI* color, BOOL alpha)
{
#if defined(_ARGB_PIXEL_)
	color->red = GetByte();
	color->green = GetByte();
	color->blue = GetByte();
//#elif (__ANDROID__)
//	color->blue = GetByte();
//	color->green = GetByte();
//	color->red = GetByte();
#else
	color->red = GetByte();
	color->green = GetByte();
	color->blue = GetByte();
#endif
	if ( alpha ) {
		color->alpha = GetByte();
	} else {
		color->alpha = 255;
	}
}

SRGB SParser::GetColor(BOOL alpha)
{
	SRGB color;
#ifdef _ARGB_PIXEL_
	color.rgb.red = GetByte();
	color.rgb.green = GetByte();
	color.rgb.blue = GetByte();
//#elif __ANDROID__
//	color.rgb.blue = GetByte();
//	color.rgb.green = GetByte();
//	color.rgb.red = GetByte();
#else
	color.rgb.red = GetByte();
	color.rgb.green = GetByte();
	color.rgb.blue = GetByte();
#endif
	if ( alpha ) {
		color.rgb.transparency = GetByte();
	} else {
		color.rgb.transparency = 0xFF;
	}
	return color;
//	SRGB color;
//	color.rgb.red = GetByte();
//	color.rgb.green = GetByte();
//	color.rgb.blue = GetByte();
//	if ( alpha ) {
//		color.rgb.transparency = GetByte();
//	} else {
//		color.rgb.transparency = 0xFF;
//	}
//	return color;
}

void SParser::GetRect(SRECT* r)
{
	InitBits();
	int nBits = (int)GetBits(5);
	r->xmin = GetSBits(nBits);
	r->xmax = GetSBits(nBits);
	r->ymin = GetSBits(nBits);
	r->ymax = GetSBits(nBits);
}

void SParser::GetBwMatrix( BwMatrix *mat )
{
	InitBits();
	// Scale terms
	if ( GetBits(1) ) 
	{
		int nBits = (int)GetBits(5);
		mat->a = GetSBits(nBits);
		mat->d = GetSBits(nBits);
	} 
	else
	{
		mat->a = mat->d = fixed_1;
	}

	// Rotate/skew terms
	if ( GetBits(1) )
	{
		int nBits = (int)GetBits(5);
		mat->b = GetSBits(nBits);
		mat->c = GetSBits(nBits);
	} 
	else 
	{
		mat->b = mat->c = 0;
	}
	int nBits = (int)GetBits(5);
	mat->tx = GetSBits(nBits);
	mat->ty = GetSBits(nBits);
	return;
}

void SParser::GetMatrix(MATRIX* mat)
{
	InitBits();
	// Scale terms
	if ( GetBits(1) ) {
		int nBits = (int)GetBits(5);
		mat->a = GetSBits(nBits);
		mat->d = GetSBits(nBits);
	} else {
	 	mat->a = mat->d = fixed_1;
	}

	// Rotate/skew terms
	if ( GetBits(1) ) {
		int nBits = (int)GetBits(5);
		mat->b = GetSBits(nBits);
		mat->c = GetSBits(nBits);
	} else {
	 	mat->b = mat->c = 0;
	}

	{// Translate terms
		int nBits = (int)GetBits(5);
		mat->tx = GetSBits(nBits);
		mat->ty = GetSBits(nBits);
	}
}

void SParser::GetColorTransform(ColorTransform* cx, BOOL alpha)
{
	InitBits();
	cx->nForeColor=0;
	cx->flags = (int)GetBits(2);
	int nBits = (int)GetBits(4);
	cx->aa = 256; cx->ab = 0;
	if ( cx->flags & ColorTransform::needA )
	{
#ifdef _ARGB_PIXEL_
		cx->ra = (S16)GetSBits(nBits);
		cx->ga = (S16)GetSBits(nBits);
		cx->ba = (S16)GetSBits(nBits);
//#elif __ANDROID__
//		cx->ba = (S16)GetSBits(nBits);
//		cx->ga = (S16)GetSBits(nBits);
//		cx->ra = (S16)GetSBits(nBits);
#else
		cx->ra = (S16)GetSBits(nBits);
		cx->ga = (S16)GetSBits(nBits);
		cx->ba = (S16)GetSBits(nBits);
#endif
		if ( alpha )
			cx->aa = (S16)GetSBits(nBits);
	}
	else
	{
		cx->ra = cx->ga = cx->ba = 256;
	}
	if ( cx->flags & ColorTransform::needB )
	{
#ifdef _ARGB_PIXEL_
		cx->rb = (S16)GetSBits(nBits);
		cx->gb = (S16)GetSBits(nBits);
		cx->bb = (S16)GetSBits(nBits);
//#elif __ANDROID__
//		cx->bb = (S16)GetSBits(nBits);
//		cx->gb = (S16)GetSBits(nBits);
//		cx->rb = (S16)GetSBits(nBits);
#else
		cx->rb = (S16)GetSBits(nBits);
		cx->gb = (S16)GetSBits(nBits);
		cx->bb = (S16)GetSBits(nBits);
#endif
		if ( alpha )
			cx->ab = (S16)GetSBits(nBits);
	}
	else
	{
		cx->rb = cx->gb = cx->bb = 0;
	}
	/*if(bitPos)
	{
		bitBuf=0;
		bitPos=0;
		pos++;
	}*/
}

char* SParser::GetString()
// caller must free memory with a delete operator
{
#if 1
	return CreateStr(GetStringP());
#else
	S32 len = 0;
	S32 savedPos = pos;
	while ( true ) {
		U8 ch = GetByte();
		len++;
		if ( !ch ) break;
	}

	char* ch = new char[len];
	if ( !ch )
		return 0;

	pos = savedPos;

	char* c = ch;
	while ( true ) {
		U8 ch = GetByte();
		*c++ = ch;
		if ( !ch ) break;
	}

	return ch;
#endif
}

char* SParser::GetStringP()
{
	char* str = (char*)script+pos;	// get the string address
	while ( GetByte() ) {}			// advance past the string
	return str;
}

#ifdef SOUND
void SParser::GetSoundInfo(XSoundChannel* c)
{
	int code = GetByte();
	if ( !c ) {
		// Skip the info
		if ( code & soundHasInPoint )
			SkipBytes(4);//GetDWord();
		if ( code & soundHasOutPoint )
			SkipBytes(4);//GetDWord();
		if ( code & soundHasLoops )
			SkipBytes(2);//GetWord();
		if ( code & soundHasEnvelope ) {
			int nPoints = GetByte();
			SkipBytes(nPoints*(4+2+2));
			//for ( int i = 0; i < nPoints; i++ ) {
			//	SkipBytes(4+2+2);//GetDWord();
			//	//GetWord();
			//	//GetWord();
			//}
		}
	} else {
		// Fill in the info
		c->syncFlags = code>>4;
		if ( code & soundHasInPoint )
			c->inPoint44 = GetDWord();
		if ( code & soundHasOutPoint )
			c->outPoint44 = GetDWord();
		if ( code & soundHasLoops )
			c->loops = GetWord();
		if ( code & soundHasEnvelope ) {
			c->envelope.nPoints = GetByte();
			for ( int i = 0; i < c->envelope.nPoints; i++ ) {
				c->envelope.points[i].mark44 = GetDWord();
				c->envelope.points[i].level0 = GetWord();
				c->envelope.points[i].level1 = GetWord();
			}
		}
	}
}
#endif

void SParser::InitBits()
{
	bitPos = 0;
	bitBuf = 0;
}

U32 SParser::GetBits(int n)
// get n bits from the stream
{
	U32 v = 0;
	for (;;) {
		int s = n-bitPos;
		if ( s > 0 ) {
			// Consume the entire buffer
			v |= bitBuf << s;
			n -= bitPos;

			// Get the next buffer
			bitBuf = GetByte();
			bitPos = 8;
		} else {
		 	// Consume a portion of the buffer
			v |= bitBuf >> -s;
			bitPos -= n;
			bitBuf &= 0xFF >> (8-bitPos);	// mask off the consumed bits
			return v;
		}
	}
}

S32 SParser::GetSBits(int n)
// Get bits w/ sign extension
{
 	S32 v = GetBits(n);
	if ( v & (1L<<(n-1)) ) {
		// Number is negative, extend the sign
		v |= -1L << n;
	}
	return v;
}


//
// The Shape Parser
//

SShapeParser::SShapeParser(ScriptPlayer* p, U8 * data, S32 start, BwShapeRecord* pRec)
{
	m_bODD = false;
	pShapeRec = pRec;
	//info = NULL;
;
	ratio = 0;
	//layer = 0;
	nLines = nFills = 0;
	//nFillBase = nLineBase = 0;

	// Init the default state
	line = fill[0] = fill[1] = 0;
	curPt.x = curPt.y = 0;
	//MatrixTransformPoint(&mat, &curPt, &curPtX);

	//cxform.Clear();	// the color transform

	// Attach the parser to the script
	player = p;
	//display = p->display;
	SParser::Attach(data, start);
//	FLASHASSERT(player && display);

//	fillIndex = fillIndexMem;
//	lineIndex = lineIndexMem;

	getAlpha = false;
	lineStyle2=false;
}

SShapeParser::~SShapeParser()
{
	// Free the index mem if they are "large"
//	if ( fillIndex != fillIndexMem )
//		delete [] fillIndex;
 //	if ( lineIndex != lineIndexMem )
//		delete [] lineIndex;

	// Free the colors if we have a local list
//	RColor* color = localColors;
//	while ( color ) {
//	 	RColor* next = color->nextColor;
//		display->FreeColor(color);
//		color = next;
//	}
}

//typedef RColor *PRColor;

#define SETMAT(MAT0,MAT1)\
{ if(MAT0){if(MAT0>0) MAT1=1;else MAT1=-1;}\
  else MAT1=0;}


/*void SShapeParser::SetupColor(RColor* color)
// Set up the colors for the current CRaster
{
	if ( !raster ) return;

	//color->raster = raster;
	switch ( color->colorType ) {
		case colorSolid:
			if(!display->player->m_pDomFilter||
				!display->player->m_pDomFilter->bEnuFilter)
				break;
			if(info&&color->rgb.alpha>=0xaf)
			{
#if (__CORE_VERSION__>=0x02074000)
				if(color->rgb.alpha>0xc0)
					color->rgb.alpha=0xc0;//=0;///=4;
#else
				color->rgb.alpha=0;///=4;
#endif
			}
			break;
		case colorGradient: {
			// Set up the inverse matrix
			MATRIX gradMat, devMat;
			gradMat = color->grad.savedMat;
			if ( raster->antialias_b ) {
				MATRIX aaToDev;
				switch(raster->antiShift)
				{
				case 2:
					MatrixScale(fixed_1/4, fixed_1/4, &aaToDev);
					break;
				case 1:
					MatrixScale(fixed_1/2, fixed_1/2, &aaToDev);
					break;
				case 3:
					MatrixScale(fixed_1/8, fixed_1/8, &aaToDev);
					break;
				}
				//MatrixScale(fixed_1/4, fixed_1/4, &aaToDev);
				MatrixConcat(&mat, &aaToDev, &devMat);	// we don't want the supersampled coords
			} else {
				devMat = mat;
			}

			// We need to work with the translation components in fixed point
			devMat.tx <<= 8;	devMat.ty <<= 8;
			gradMat.tx <<= 8;	gradMat.ty <<= 8;
			MatrixConcat(&gradMat, &devMat, &gradMat);
			MatrixInvert(&gradMat, &color->grad.invMat);
		} break;

		case colorBitmap: {
			// Set up the transforms
			MATRIX bitsMat, devMat;
			if ( raster->antialias_b ) {
				MATRIX aaToDev;
				switch(raster->antiShift)
				{
				case 2:
					MatrixScale(fixed_1/4, fixed_1/4, &aaToDev);
					break;
				case 1:
					MatrixScale(fixed_1/2, fixed_1/2, &aaToDev);
					break;
				case 3:
					MatrixScale(fixed_1/8, fixed_1/8, &aaToDev);
					break;
				}
				//MatrixScale(fixed_1/4, fixed_1/4, &aaToDev);
				MatrixConcat(&mat, &aaToDev, &devMat);	// we don't want the supersampled coords
			} else {
				devMat = mat;
			}

			// We need to work with the translation components in fixed point
			bitsMat = color->bm.savedMat;

			
			//color->bm.scaleMat=devMat;
			//MatrixConcat(&bitsMat,&devMat,&color->bm.scaleMat);
			bitsMat.tx <<= XBITMAPOFF;	bitsMat.ty <<= XBITMAPOFF;
			devMat.tx <<= XBITMAPOFF;	devMat.ty <<= XBITMAPOFF;
			if(info)
			{

				MatrixInvert(&devMat,&color->bm.invMat);
				MATRIX mat;

				//::MatrixIdentity(&mat);
				//mat.a=fixed_1/20;
				//mat.d=fixed_1/20;
				//::MatrixConcat(&color->bm.invMat,&mat,&color->bm.invMat);
				//::MatrixConcat(&bitsMat,&mat,&bitsMat);
				MATRIX&mm=color->bm.invMat;
				mm.a=mm.a/20;
				mm.b=mm.b/20;
				mm.c=mm.c/20;
				mm.d=mm.d/20;
				mm.tx=mm.tx/20;
				mm.ty=mm.ty/20;
				bitsMat.a=bitsMat.a/20;
				bitsMat.b=bitsMat.b/20;
				bitsMat.c=bitsMat.c/20;
				bitsMat.d=bitsMat.d/20;
				bitsMat.tx=bitsMat.tx/20;
				bitsMat.ty=bitsMat.ty/20;

				::MatrixIdentity(&mat);
				int w=RectWidth(&info->orgRect);//RectWidth(&bm.gridRect);
				int h=RectHeight(&info->orgRect);//RectHeight(&bm.gridRect);
				SRECT*gridBy=&info->gridBy;
				mat.a=(w-gridBy->xmin-gridBy->xmax)*fixed_1/w;
				mat.d=(h-gridBy->ymin-gridBy->ymax)*fixed_1/h;
				mat.tx=((info->orgRect.xmin+gridBy->xmin))<< XBITMAPOFF;//w*10;//(w<<(XBITMAPOFF-1));
				mat.ty=((info->orgRect.ymin+gridBy->ymin))<< XBITMAPOFF;//h*10;//(h<<(XBITMAPOFF-1));
				MATRIX mat0;
				::MatrixIdentity(&mat0);
				mat0.tx=-mat.tx;
				mat0.ty=-mat.ty;
				//MatrixConcat(&mat0,&mat,&mat);

				MatrixConcat(&mat0,&color->bm.invMat,&color->bm.invMat);
				MatrixConcat(&color->bm.invMat,&mat,&color->bm.invMat);

				MatrixInvert(&bitsMat,&mat);
				
				color->bm.orgMat=color->bm.invMat;
				color->bm.gridMat=mat;
				//color->bm.orgMat.tx<<=XBITMAPOFF
				//color->bm.invMat=mat;
				MatrixConcat(&color->bm.invMat,&mat,&color->bm.invMat);

				//color->bm.invMat.tx<<=XBITMAPOFF;
				//color->bm.invMat.ty<<=XBITMAPOFF;
				//if(color->bm.savedMat.a<0)
				//	int v=0;
			}
			else
			{
				//bitsMat.tx <<= XBITMAPOFF;	bitsMat.ty <<= XBITMAPOFF;
				//devMat.tx <<= XBITMAPOFF;	devMat.ty <<= XBITMAPOFF;

				MatrixConcat(&bitsMat, &devMat, &bitsMat);
				//color->bm.devMat=bitsMat;
				MatrixInvert(&bitsMat, &color->bm.invMat);
			}

			SBitmapCore* bits = color->bm.bitmap;
			color->transparent = int( cxform.HasTransparency() ) | int( bits->transparent );
			if ( bits->HasBits() ) {
				color->BuildCache();	// this locks the bitmap so it cannot be purged...
				if(info)
				{
					color->gridBy0=info;
					color->BuildGrid();
				}
			} else {
				color->colorType = colorSolid;
			}
		} break;
	}
	color->BuildCache();
}*/

XBOOL SShapeParser::ReadGLColor(GLColor *color)
{
		//color->SetUp(raster);

		//color->nextColor = *colorList;
		//*colorList = color;

		//FLASHASSERT(color->order < 0x10000);

		int fillStyle = GetByte();
		if ( fillStyle & fillGradient ) {
			color->colorType = glColorGradient;
			color->grad.gradStyle = fillStyle;
 			//color->grad.colorRamp = 0;

			GetMatrix(&color->grad.savedMat);
			XU8 nColor=GetByte();
			color->grad.ramp.nColors	= nColor&0xf;
			color->grad.ramp.spreadMode	= (nColor>>6);
			color->grad.ramp.interMode	= (nColor>>4)&3;
				//GetByte();
			for ( int j = 0; j < color->grad.ramp.nColors; j++ ) {
				color->grad.ramp.colorRatio[j] = GetByte();
					SRGB c = GetColor(getAlpha);
					//if ( c.rgb.transparency < 255 )
					//	color->transparent = true;
					color->grad.ramp.color[j] = c;
				}

				// merge fix
				//if ( cxform.HasTransform() )
				//{
				//	cxform.Apply(&color->grad.ramp);
					//if ( cxform.HasTransparency() )
					//	color->transparent = true;
				//}
			 if(fillStyle&1)
				 SkipBytes(2);
            } else if ( fillStyle & fillBits ) {
				// A bitmap fill
				U16 tag = GetWord();		// the bitmap tag
				GetMatrix(&color->bm.savedMat);	// the bitmap matrix
				SCharacter* ch = player->FindCharacter(tag);

				// Default to red if we can't find the bits
				color->rgb.alpha = 255;
				color->rgb.red = 0;
				color->rgb.green = color->rgb.blue = 0;

				if ( ch && ch->type == bitsChar ) {
					// We found the bits, set up the color
					color->colorType = glColorBitmap;
					color->bm.bitmap = &ch->bits;
					color->bm.bitsStyle = (U8)fillStyle;
					color->bm.repeat = (fillStyle&fillBitsClip)?0:1;
					//memcpy(&color->bm.cxform,&cxform,sizeof(GLColorTransform));
				//	color->bm.rowIndex = 0;
					//if(ch->tag==141)
					//	int v=0;
					//	color->bm.savedMat.ty=0;
					if(!ch->bits.bitmapID)
					{
						player->BuildBits(ch);	// be sure the bits are decompressed and available
						//Create Texture
						if(ch->bits.HasBits())
						{
							ch->bits.CreateTexture();
							ch->bits.ReleaseData();//PIFree(false);
						}
					}
					//color->SetBitmapDiv();
					//if(cxform.nBlendMode)
					//	color->bm.bitmap->SetBlendMode(cxform.nBlendMode);
				}
				//color->colorType=colorSolid;

			} else {
				// A solid color
				GetColor(&color->rgb, getAlpha);
				//if ( cxform.HasTransform() )
				//	cxform.Apply(&color->rgb);
				//color->transparent = (U8)PreMulAlpha(&color->rgb);
			}
			color->SetUp();
			//SetupColor(color);
			return true;
}

/*XBOOL SShapeParser::ReadColor(RColor *color)
{
		color->SetUp(raster);

		//color->nextColor = *colorList;
		//*colorList = color;

		FLASHASSERT(color->order < 0x10000);

		int fillStyle = GetByte();
		if ( fillStyle & fillGradient ) {
			color->colorType = colorGradient;
			color->grad.gradStyle = fillStyle;
 			color->grad.colorRamp = 0;

			GetMatrix(&color->grad.savedMat);
			XU8 nColor=GetByte();
			color->grad.ramp.nColors	= nColor&0xf;
			color->grad.ramp.spreadMode	= (nColor>>6);
			color->grad.ramp.interMode	= (nColor>>4)&3;
				//GetByte();
			for ( int j = 0; j < color->grad.ramp.nColors; j++ ) {
				color->grad.ramp.colorRatio[j] = GetByte();
					SRGB c = GetColor(getAlpha);
					if ( c.rgb.transparency < 255 )
						color->transparent = true;
					color->grad.ramp.color[j] = c;
				}

				// merge fix
				if ( cxform.HasTransform() )
				{
					cxform.Apply(&color->grad.ramp);
					if ( cxform.HasTransparency() )
						color->transparent = true;
				}
			 if(fillStyle&1)
				 SkipBytes(2);
            } else if ( fillStyle & fillBits ) {
				// A bitmap fill
				U16 tag = GetWord();		// the bitmap tag
				GetMatrix(&color->bm.savedMat);	// the bitmap matrix
				SCharacter* ch = player->FindCharacter(tag);

				// Default to red if we can't find the bits
				color->rgb.alpha = 255;
				color->rgb.red = 0;
				color->rgb.green = color->rgb.blue = 0;

				if ( ch && ch->type == bitsChar ) {
					// We found the bits, set up the color
					color->colorType = colorBitmap;
					color->bm.bitmap = &ch->bits;
					color->bm.bitsStyle = (U8)fillStyle;
					color->bm.cxform = cxform;
				//	color->bm.rowIndex = 0;
					//if(ch->tag==141)
					//	int v=0;
					//	color->bm.savedMat.ty=0;

					player->BuildBits(ch);	// be sure the bits are decompressed and available
					color->SetBitmapDiv();
					if(cxform.nBlendMode)
						color->bm.bitmap->SetBlendMode(cxform.nBlendMode);
				}
				//color->colorType=colorSolid;

			} else {
				// A solid color
				GetColor(&color->rgb, getAlpha);
				if ( cxform.HasTransform() )
					cxform.Apply(&color->rgb);
				color->transparent = (U8)PreMulAlpha(&color->rgb);
			}
			//SetupColor(color);
			return true;
}*/
//
//BOOL SShapeParser::GetStyles()
//{
////	if(lineStyle2)
////		int v=0;
////	FLASHASSERT(!*colorList || (*colorList)->colorType != colorClip);
//	RColor* clipColor = XNULL;
//	if(*colorList&&(*colorList)->colorType == colorClip)
//		clipColor=*colorList;
//	//else clipColor=CreateClipColor(display, colorList);
//	{// Get the fills
//		nFills = GetByte();
//		if ( nFills == 255 ) {
//			// We have a "large number"
//			nFills = GetWord();
//			if ( fillIndex != fillIndexMem )
//				delete [] fillIndex;
//			fillIndex = new PRColor[nFills+1];
//			if ( !fillIndex ) {
//				fillIndex = fillIndexMem;
//				return false;
//			}
//		}
//
//		fillIndex[0] = 0;
//		for ( int i = 1; i <= nFills; i++ ) {
//			if(pos>=tagEnd) return false;
//			RColor* color = display->CreateColor();
//			if ( !color )
//				return false;
//			if(!ReadColor(color))
//			{
//				display->FreeColor(color);
//				//delete color;
//				return false;
//			}
//			if(clipColor)
//			{
//				fillIndex[i]=clipColor;
//				display->FreeColor(color);
//			}
//			else
//			{
//				color->nextColor = *colorList;
//				*colorList = color;
//				fillIndex[i] = color;
//				color->order = layer + i;
//				//this->
//				SetupColor(color);
//			}
//				// the buildcache in this function must be called immediately after BuildBits()
//		}
//	}
//
//	{// Get the lines
//		nLines = GetByte();
//		if ( nLines == 255 ) {
//			// We have a "large number"
//			nLines = GetWord();
//			if ( lineIndex != lineIndexMem )
//				delete [] lineIndex;
//			lineIndex = new SLine[nLines+1];
//			if ( !lineIndex ) {
//				lineIndex = lineIndexMem;
//				return false;
//			}
//		}
//
//		lineIndex[0].color = 0;
//		lineIndex[0].thickness = 0;
//		for ( int i = 1; i <= nLines; i++ ) {
//			//XU16 thick=GetWord();
//			//if(!thick) return false;
//			if(pos>=tagEnd) return false;
//			RColor* color = display->CreateColor();
//			if ( !color )
//				return false;
//
//			color->SetUp(raster);
//
//			lineIndex[i].color = color;
//			lineIndex[i].thickness = (int)MatrixTransformThickness(&mat, GetWord());
//			//if(lineIndex[i].thickness>=4)
//			//	int v=0;
//			lineIndex[i].nMiterFactor=0;
//			lineIndex[i].nFlags=0;
//			color->order = layer + (i+nFills);
//			FLASHASSERT(color->order < 0x10000);
//
//			if(lineStyle2)
//			{
//				XU16 flags=(GetByte()<<8)|GetByte();
//				lineIndex[i].nFlags=flags;
//				if((flags&XLS_JOINSTYLEMITER))
//					lineIndex[i].nMiterFactor=GetWord();
//				if((flags&XLS_FILLFLAG))
//					ReadColor(color);
//				else
//				{
//					GetColor(&color->rgb, getAlpha);
//					if ( cxform.HasTransform() )
//						cxform.Apply(&color->rgb);
//					color->transparent = (U8)PreMulAlpha(&color->rgb);
//				}
//			}
//			else
//			{
//				GetColor(&color->rgb, getAlpha);
//				if ( cxform.HasTransform() )
//				cxform.Apply(&color->rgb);
//				color->transparent = (U8)PreMulAlpha(&color->rgb);
//			}
//			if(clipColor)
//			{
//				lineIndex[i].color=clipColor;
//				//color->FreeCache();
//				display->FreeColor(color);
//			}
//			else
//			{
//				color->nextColor = *colorList;
//				*colorList = color;
//				_GRIDINFO*fo=this->info;
//				SetupColor(color);
//				this->info=fo;
//			}
//		}
//	}
//
//	InitBits();
//	nFillBits = (int)GetBits(4);
//	nLineBits = (int)GetBits(4);
//	return true;
//}
//
//BOOL SShapeParser::GetMorphStyles()
//{
//	MATRIX mat1, mat2;
//	int i;
//	RColor* clipColor = XNULL;
//	if(*colorList&&(*colorList)->colorType == colorClip)
//		clipColor=*colorList;
//	// Get the fills
//	nFills = GetByte();
//	if ( nFills >= 255 )
//	{
//		// we have a lot of fills to alloc for
//		nFills = GetWord();
//		if ( fillIndex != fillIndexMem )
//			delete fillIndex;
//		fillIndex = new PRColor[nFills+1];
//		if ( !fillIndex )
//		{
//			fillIndex = fillIndexMem;
//			return false;
//		}
//	}
//
//	fillIndex[0] = 0;
//	for ( i = 1; i <= nFills; i++ )
//	{
//		RColor* color = display->CreateColor();
//		if ( !color )
//			return false;
//
//		color->SetUp(raster);
//
//		if(!clipColor)
//		{
//			color->nextColor = *colorList;
//			*colorList = color;
//			fillIndex[i] = color;
//		}
//		else fillIndex[i]=clipColor;
//
//		color->order = layer + i;
//		FLASHASSERT(color->order < 0x10000);
//
//		int fillStyle = GetByte();
//		if ( fillStyle & fillGradient )
//		{
//// 			#if defined(GENERATOR) || defined(_CELANIMATOR)
//// 			if (!player->solidGradients) {
//// 			#endif
//			// Gradient fill
//			color->colorType = colorGradient;
//			color->grad.gradStyle = fillStyle;
//			color->grad.colorRamp = 0;
//
//			GetMatrix(&mat1);
//			GetMatrix(&mat2);
//			Interpolate(&mat1, &mat2, ratio, &color->grad.savedMat);
//
//			//if(bitPos)
//			//	int v=0;
//			U8 nColor=GetByte();
//			color->grad.ramp.nColors = nColor&0xf;
//			color->grad.ramp.spreadMode	= (nColor>>6);
//			color->grad.ramp.interMode	= (nColor>>4)&3;
//			for ( int j = 0; j < color->grad.ramp.nColors; j++ )
//			{
//				U8 r1, r2;
//				SRGB c1, c2;
//
//				r1 = GetByte();
//				c1 = GetColor(getAlpha);
//				r2 = GetByte();
//				c2 = GetColor(getAlpha);
//				if(j<fillMaxGradientColors)
//				{
//					color->grad.ramp.colorRatio[j] = (U8)Interpolate(r1, r2, ratio);
//					Interpolate(&c1, &c2, ratio, &color->grad.ramp.color[j]);
//
//					if ( c1.rgb.transparency < 255 || c2.rgb.transparency < 255)
//					{
//						color->transparent = true;
//					}
//				}
//			}
//
//			if ( cxform.HasTransform() )
//			{
//				cxform.Apply(&color->grad.ramp);
//				if ( cxform.HasTransparency() )
//				{
//					color->transparent = true;
//				}
//			}
//// 			#if defined(GENERATOR) || defined(_CELANIMATOR)
//// 			} else {
//// 				// Skip the morph gradient matrices.
//// 				MATRIX matrix;
//// 				GetMatrix(&matrix);
//// 				GetMatrix(&matrix);
////
//// 				// Get the number of colors.
//// 				int nColors = GetByte();
//// 				for (int j = 0; j < nColors; j++) {
//// 					// Is this the first color?
//// 					if (!j) {
//// 						RGBI rgb1, rgb2;
////
//// 						// Handle the first color as a solid color.
//// 						GetByte();
//// 						GetColor(&rgb1, getAlpha);
//// 						GetByte();
//// 						GetColor(&rgb2, getAlpha);
////
//// 						Interpolate(&rgb1, &rgb2, ratio, &color->rgb);
//// 						if ( cxform.HasTransform() )
//// 						{
//// 							cxform.Apply(&color->rgb);
//// 						}
//// 						color->transparent = PreMulAlpha(&color->rgb);
//// 					} else {
//// 						// Skip colors other than the first color.
//// 						GetByte();
//// 						GetColor(getAlpha);
//// 						GetByte();
//// 						GetColor(getAlpha);
//// 					}
//// 				}
//// 			}
//// 			#endif // GENERATOR || _CELANIMATOR
//		}
//		else if ( fillStyle & fillBits )
//		{
//			// A bitmap fill
//			U16 tag = GetWord();		// the bitmap tag
//
//			GetMatrix(&mat1);
//			GetMatrix(&mat2);
////			Interpolate(&mat1, &mat2, 0, &color->bm.savedMat);	// The original line is missing the "ratio"
//																// Bug from David Michie (Middlesoft) and
//																// Jeff Martin. Changed lee@middlesoft
//			Interpolate(&mat1, &mat2, ratio, &color->bm.savedMat);
//			SCharacter* ch = player->FindCharacter(tag);
//
//			// Default to red if we can't find the bits
//			color->rgb.alpha = 255;
//			color->rgb.red = 255;
//			color->rgb.green = color->rgb.blue = 0;
//
//			if ( ch && ch->type == bitsChar )
//			{
//				// We found the bits, set up the color
//				color->colorType = colorBitmap;
//				color->bm.bitmap = &ch->bits;
//				color->bm.bitsStyle = (U16)fillStyle;
//				color->bm.cxform = cxform;
//				//	color->bm.rowIndex = 0;
//
//				player->BuildBits(ch);	// be sure the bits are decompressed and available
//				color->SetBitmapDiv();
//				if(cxform.nBlendMode)
//						color->bm.bitmap->SetBlendMode(cxform.nBlendMode);
//			}
//		}
//		else
//		{
//			RGBI rgb1, rgb2;
//			// A solid color
//			GetColor(&rgb1, getAlpha);
//			GetColor(&rgb2, getAlpha);
//
//			Interpolate(&rgb1, &rgb2, ratio, &color->rgb);
//			if ( cxform.HasTransform() )
//			{
//				cxform.Apply(&color->rgb);
//			}
//			color->transparent = (U8)PreMulAlpha(&color->rgb);
//		}
//		SetupColor(color);		// the buildcache in this function must be called immediately after BuildBits()
//		if(clipColor)
//			display->FreeColor(color);
//	}
//
//	// Get the lines
//	nLines = GetByte();
//	if ( nLines >= 255 )
//	{
//		// We have a "large number"
//		nLines = GetWord();
//		if ( lineIndex != lineIndexMem )
//			delete lineIndex;
//		lineIndex = new SLine[nLines+1];
//		if ( !lineIndex )
//		{
//			lineIndex = lineIndexMem;
//			return false;
//		}
//	}
//
//	lineIndex[0].color = 0;
//	lineIndex[0].thickness = 0;
//	for (i = 1; i <= nLines; i++ )
//	{
//		U16		thickness, thick1,	thick2;
//		RGBI	rgb1, rgb2;
//
//		RColor* color = display->CreateColor();
//		if ( !color )
//			return false;
//
//		color->SetUp(raster);
//		if(clipColor)
//			lineIndex[i].color=clipColor;
//		else
//		{
//			color->nextColor = *colorList;
//			*colorList = color;
//			lineIndex[i].color = color;
//		}
//
//		// get the thickness
//		thick1 = GetWord();
//		thick2 = GetWord();
//		thickness = (U16)Interpolate(thick1, thick2, ratio);
//
//		lineIndex[i].thickness = (int)MatrixTransformThickness(&mat, thickness);
//
//		color->order = layer + (i+nFills);
//		FLASHASSERT(color->order < 0x10000);
//		XBOOL hasFill=0;
//		if(lineStyle2)
//		{
//			XU8 f1=GetByte();
//			hasFill=f1&0x08;
//			SkipBytes(1);
//			if((f1&0x30)==0x20)
//				SkipBytes(2);
//		}
//
//		// get the color
//		if(hasFill)
//		{
//			ReadColor(color);
//		}
//		else
//		{
//			GetColor(&rgb1, getAlpha);
//			GetColor(&rgb2, getAlpha);
//
//		// interpolate the color value
//			Interpolate(&rgb1, &rgb2, ratio, &color->rgb);
//		}
//
//		if ( cxform.HasTransform() )
//			cxform.Apply(&color->rgb);
//		color->transparent = (U8)PreMulAlpha(&color->rgb);
//		SetupColor(color);
//		if(clipColor)
//			display->FreeColor(color);
//	}
//	return true;
//}


BOOL SShapeParser::GetBwStyles(bool bTess )
{
	//RColor* clipColor = XNULL;
	//if(*colorList&&(*colorList)->colorType == colorClip)
	//	clipColor=*colorList;
	// Get the fills
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	nFills = GetByte();
	if( nFills > 0 )
	{
		if( nFills == 255 )
			nFills = GetWord();
		int nfs = pShapeRec->mnFills;
		pShapeRec->SetFills(nfs+nFills+1);
		pShapeRec->AddFillSyle(nfs,XNULL);
		nfs++;
		//pShapeRec->AddFillSyle( XNULL );//new BwFillStyle() );
		for( int i = 1; i <= nFills; i++ )
		{
			GLColor*pColor = pAlloc->AllocColor();
			//memset(pColor,0,sizeof(GLColor));
			if(bTess)
			{
				if(!ReadGLColor(pColor))
				{
					pAlloc->FreeColor(pColor);
					return XFALSE;
				}
			}
			else
			{
				SkipColor();
				pColor->colorType = glColorSolid;
			}
			pShapeRec->AddFillSyle(nfs,pColor);
			nfs++;
			//nFillBase++;
			//ReadFillStyel( pShapeRec->mVecFillStyles );
		}
	}


	// Get the lines
	nLines = GetByte();
	if( nLines > 0 )
	{
		if( nLines == 0xFF )
			nLines = GetWord();
		int nls = pShapeRec->mnLines;
		pShapeRec->SetLines(nls+nLines+1);
		pShapeRec->AddLineStyle( nls,XNULL );nls++;
		for( int i = 1; i <= nLines; i++ )
		{//MatrixTransformThickness
			//RColor *color = display->CreateColor();
			BwLineStyle* pLineStyle=pAlloc->AllocLineStyle();// = new BwLineStyle();
			pLineStyle->mWidth=GetWord();//( MatrixTransformThickness( &mat, GetWord() ));
			if( lineStyle2 )
			{
				XU16 flags = (GetByte()<<8) | GetByte();
				if( (flags & XLS_JOINSTYLEMITER ) )
					pLineStyle->mfMiterLimitFactor = ( GetWord() );
				if( flags & XLS_FILLFLAG )
				{
					pLineStyle->color=pAlloc->AllocColor();
					if(bTess)
						ReadGLColor(pLineStyle->color);
					else
					{
						pLineStyle->color->colorType = glColorSolid;
						SkipColor();

					}
					//ReadFillStyel( pShapeRec->mVecFillStyles );
				}
				else
				{
					//RGBI rgb;
					//memset( &rgb, 0, sizeof( RGBI ) );
					GetColor( &pLineStyle->mColor,getAlpha);//&rgb, getAlpha );
					//if( cxform.HasTransform() )
					//	cxform.Apply( &pLineStyle->color->rgb );
					//pLineStyle.LineColor( rgb );
				}
			}
			else
			{
				//RGBI rgb;
				//memset( &rgb, 0, sizeof( RGBI ) );
				GetColor( &pLineStyle->mColor, getAlpha );
				//if( cxform.HasTransform() )
				//	cxform.Apply( &pLineStyle->color->rgb );
				//pLineStyle.LineColor( rgb );
			}
			//SetupColor( color );

			pShapeRec->AddLineStyle(nls, pLineStyle );
			nls++;
		}
	}

	InitBits();
	nFillBits = (int)GetBits(4);
	nLineBits = (int)GetBits(4);
	return true;
}

/*BOOL SShapeParser::ReadFillStyel(vector<BwFillStyle *> &fillStyles )
{
	FLASHASSERT(color->order < 0x10000);
	BwFillStyle *pFillStyle = NULL;
	int styleType = GetByte();
	if ( styleType & fillGradient ) 
	{
		pFillStyle = new BwGradientFill();
		//color->colorType = colorGradient;
		//color->grad.gradStyle = styleType;
		//color->grad.colorRamp = 0;
		pFillStyle->GradientType( styleType );

		BwMatrix mat;
		GetBwMatrix( &mat );
		//mat.invert();
		pFillStyle->StylelMatrix( mat );
		XU8 nColor=GetByte();
		int iNumGradient = nColor & 0xf;
		pFillStyle->SpreadMode( nColor>>6 );
		pFillStyle->InterpolationMode( (nColor>>4)&3 );
		//color->grad.ramp.spreadMode	= (nColor>>6);
		//color->grad.ramp.interMode	= (nColor>>4)&3;
		//GetByte();
		for ( int j = 0; j < iNumGradient; j++ )
		{
			BwGradientRecord grarec;
			grarec.GraRecRatio( GetByte() );
			RGBI rgb;
			memset( &rgb, 0, sizeof( RGBI ) );
			GetColor( &rgb, getAlpha );
			if( cxform.HasTransform() )
				cxform.Apply( &rgb );
			grarec.GraRecColor( rgb );
			pFillStyle->AddGradientRec( grarec );
		}

		if( styleType & 1 )
			SkipBytes(2);
	} 
	else if ( styleType & fillBits ) 
	{
		pFillStyle = new BwBitmapFill();
		// A bitmap fill
		U16 tag = GetWord();		// the bitmap tag
		BwMatrix mat;
		GetBwMatrix( &mat );
		mat.invert();
		pFillStyle->BitmapID( tag );
		pFillStyle->StylelMatrix( mat );
		SCharacter* ch = player->FindCharacter(tag);
		if( ch && ch->type == bitsChar )
		{
			player->BuildBits( ch );

		}

		//// Default to red if we can't find the bits
		//color->rgb.alpha = 255;
		//color->rgb.red = 0;
		//color->rgb.green = color->rgb.blue = 0;
		//if ( ch && ch->type == bitsChar )
		//{
		//	// We found the bits, set up the color
		//	color->colorType = colorBitmap;
		//	color->bm.bitmap = &ch->bits;
		//	color->bm.bitsStyle = (U8)styleType;
		//	color->bm.cxform = cxform;
		//	//	color->bm.rowIndex = 0;
		//	//if(ch->tag==141)
		//	//	int v=0;
		//	//	color->bm.savedMat.ty=0;

		//	player->BuildBits(ch);	// be sure the bits are decompressed and available
		//	color->SetBitmapDiv();
		//	if(cxform.nBlendMode)
		//		color->bm.bitmap->SetBlendMode(cxform.nBlendMode);
		//}
		////color->colorType=colorSolid;

	} 
	else 
	{
		pFillStyle = new BwSolidFill();
		RGBI rgb;
		memset( &rgb, 0, sizeof( RGBI ) );
		GetColor( &rgb, getAlpha );
		if( cxform.HasTransform() )
			cxform.Apply( &rgb );
		pFillStyle->StyleColor( rgb );
		// A solid color
	}
	fillStyles.push_back( pFillStyle );
	//SetupColor(color);

	return true;
}*/


int SShapeParser::GetBwEdge( BwEdge &edge )
{
	int icou = 0;
	bool isEdge = (bool)GetSBits( 1 );
	if( !isEdge )
	{
		int flags = (int )GetBits( 5 );
		if( flags == 0 )
			return eflagsEnd;
		if( flags & eflagsMoveTo )
		{
			int nBits = (int)GetBits( 5 );
			curPt.x = GetSBits( nBits );
			curPt.y = GetSBits( nBits );
			curPtX = curPt;
			//xxMatrixDeltaTransformPoint( &mat, &curPt, &curPtX );
		}
		if( flags & eflagsFill0 )
		{
			fill[0] = (int)GetBits( nFillBits );
		}
		if( flags & eflagsFill1 )
		{
			fill[1] = (int )GetBits( nFillBits );
		}
		if( flags & eflagsLine )
		{
			line = (int)GetBits( nLineBits );
		}
		/*if( flags & eflagsNewStyles )
		{
			layer += nFillBits + nLineBits;
			//GetBwStyles()
		}*/
		return flags;
	}
	else
	{
		edge.IsLine( (int) GetBits(1) );
		if ( edge.IsLine() == 0 ) 
		{
			int nBits = (int)GetBits(4)+2;	// nBits is biased by 2

			curPt.x += GetSBits(nBits);
			curPt.y += GetSBits(nBits);
			//MatrixTransformPoint(&mat, &curPt, &edge.mControlPt );
			edge.mControlPt = curPt;
			curPt.x += GetSBits(nBits);
			curPt.y += GetSBits(nBits);
			//MatrixTransformPoint(&mat, &curPt, &edge.mEndPt );
			edge.mEndPt = curPt;

			/*char pTmp[256];
			memset(pTmp, 0, 256 );
			sprintf(pTmp, "line:%d,cou:%d,ControlPt->x:%d, y:%d, EndPt->x:%d, y:%d", __LINE__,
				icou, edge.mControlPt.x, edge.mControlPt.y, edge.mEndPt.x, edge.mEndPt.y );					
			OutputDebugStringA( pTmp );*/

		}
		else
		{
			// Handle a line
			int nBits = (int)GetBits(4)+2;	// nBits is biased by 2

			BOOL generalLine = (int)GetBits(1);

			if ( generalLine ) 
			{
				curPt.x += GetSBits(nBits);//x;
				curPt.y += GetSBits(nBits);//y;
			}
			else 
			{
				BOOL vertical = (int)GetBits(1);
				if ( vertical ) 
				{
					curPt.y += GetSBits(nBits);
				} 
				else
				{
					// Horizontal line
					curPt.x += GetSBits(nBits);
				}
			}
			//MatrixTransformPoint(&mat, &curPt, &edge.mEndPt );
			edge.mEndPt = curPt;
			edge.mControlPt = edge.mEndPt;
			/*char pTmp[256];
			memset(pTmp, 0, 256 );
			sprintf(pTmp, "line:%d,cou:%d,ControlPt->x:%d, y:%d, EndPt->x:%d, y:%d", __LINE__,
				icou, edge.mControlPt.x, edge.mControlPt.y, edge.mEndPt.x, edge.mEndPt.y );					
			OutputDebugStringA( pTmp );*/
			//PointAverage(&curPtX, &edge.mEndPt, &edge.mControlPt );
		}
		//curPtX = edge.mEndPt;
	}
	return 0;
}

int SShapeParser::GetBwPaths(BOOL useWinding )
{
	//BwPath path;
	BwShapeRecord *paths = pShapeRec;
	int fill_base = 0;
	int line_base = 0;
	int icou = 0;
	//path->StartPt( curPtX );
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	BwPath* path=pAlloc->AllocPath();
	int fLeft = 0;
	int fRight = 0;
	int lineStyle = 0;
	bool bFirst=true;
	while( true )
	{
		BOOL isEdge = (int)GetBits(1);
		if ( !isEdge )
		{
			// Handle a state change
			int flags = (int)GetBits(5);
			if ( flags == 0 )
			{
				if( !path->EdgeEmpty() )
				{
					paths->AddPath( path );
					//path->ClearEdges();
					//path = pAlloc->AllocPath();
				}
				else pAlloc->FreePath(path);
				return eflagsEnd;
			}
			if ( flags & eflagsMoveTo ) 
			{
				if( !path->EdgeEmpty() )
				{
					paths->AddPath( path );
					//path.ClearEdges();
					path = pAlloc->AllocPath();
					path->FillLeft(fLeft);
					path->FillRight(fRight);
					path->LineStyle(lineStyle);
				}
				int nBits = (int)GetBits(5);
				curPt.x = GetSBits(nBits);
				curPt.y = GetSBits(nBits);
				//MatrixTransformPoint(&mat, &curPt, &curPtX);
				curPtX = curPt;
				path->StartPt( curPtX );	
				/*char pTmp[256];
				memset(pTmp, 0, 256 );
				sprintf(pTmp, "line:%d,startPt->x:%d, y:%d", __LINE__, curPtX.x, curPtX.y );
				OutputDebugStringA( pTmp );*/
			}

			// Get new fill info
			if ( flags & eflagsFill0 )
			{
				if( !path->EdgeEmpty() )
				{
					paths->AddPath( path );
					//path.ClearEdges();
					path = pAlloc->AllocPath();
					path->StartPt( curPtX );
					/*char pTmp[256];
					memset(pTmp, 0, 256 );
					sprintf(pTmp, "line:%d,startPt->x:%d, y:%d", __LINE__, curPtX.x, curPtX.y );	
					OutputDebugStringA( pTmp );*/
				}
				int style = ( int )GetBits( nFillBits );
				if( style > 0 )
				{
					style += fill_base;
				}
				fLeft = style;
				path->FillLeft( style );
				path->FillRight( fRight);
				path->LineStyle(lineStyle);
			}
			if ( flags & eflagsFill1 )
			{
				if( !path->EdgeEmpty() )
				{
					paths->AddPath( path );
					//path.ClearEdges();
					path = pAlloc->AllocPath();
					path->StartPt( curPtX );
					/*char pTmp[256];
					memset(pTmp, 0, 256 );
					sprintf(pTmp, "line:%d,startPt->x:%d, y:%d", __LINE__, curPtX.x, curPtX.y );					OutputDebugStringA( pTmp );
					OutputDebugStringA( pTmp );*/
				}
				int style = ( int)GetBits( nFillBits );
				if( style > 0 )
				{
					style += fill_base;
				}
				fRight = style;
				path->FillRight( style );
				path->FillLeft( fLeft);
				path->LineStyle(lineStyle);
			}
			if(bFirst&&(flags&(eflagsFill1|eflagsFill0)))
			{
				m_bODD=(path->FillRight()&&path->FillLeft())?false:(useWinding?false:true);
				bFirst=false;
			}
			// Get new line info
			if ( flags & eflagsLine )
			{
				if( !path->EdgeEmpty() )
				{
					paths->AddPath( path );
					//path.ClearEdges();
					path = pAlloc->AllocPath();
					path->StartPt( curPtX );
					/*char pTmp[256];
					memset(pTmp, 0, 256 );
					sprintf(pTmp, "line:%d,startPt->x:%d, y:%d", __LINE__, curPtX.x, curPtX.y );					OutputDebugStringA( pTmp );
					OutputDebugStringA( pTmp );*/
				}
				int style = (int)GetBits(nLineBits);
				if( style > 0 )
				{
					style += line_base;
				}
				//line = (int)GetBits(nLineBits);
				path->LineStyle( style );
				lineStyle = style;
				path->FillLeft( fLeft);
				path->FillRight( fRight);
			}

			// Check to get a new set of styles for a new shape layer
			if ( flags & eflagsNewStyles )
			{
				if( !path->EdgeEmpty() )
				{
					paths->AddPath( path );
					//path.ClearEdges();
					path = pAlloc->AllocPath();
					//path->FillLeft( fLeft );
					//path->FillRight( fRight );
				}
				//BwPath bpt;
				//bpt.mbNewShape = true;
				path->mbNewShape = true;
				//paths->AddPath( bpt );
				//layer += nFills+nLines;
				fill_base = paths->mnFills;//mVecFillStyles.size();
				line_base = paths->mnLines;//mVecLineStyles.size();
				GetBwStyles( );
			}
		} 
		else
		{
			//if( path.EdgeEmpty() )
			//{
			//	path.StartPt( curPtX );
			//	char pTmp[256];
			//	memset(pTmp, 0, 256 );
			//	sprintf(pTmp, "line:%d,startPt->x:%d, y:%d", __LINE__, curPtX.x, curPtX.y );					OutputDebugStringA( pTmp );
			//	OutputDebugStringA( pTmp );
			//	//paths->AddPath( path );
			//}
			// Create an edge
			icou++;
			BwEdge* edge=pAlloc->AllocEdge();
			edge->IsLine( (int) GetBits(1) );
			if ( edge->IsLine() == 0 ) 
			{
				int nBits = (int)GetBits(4)+2;	// nBits is biased by 2

				curPt.x += GetSBits(nBits);
				curPt.y += GetSBits(nBits);
				//MatrixTransformPoint(&mat, &curPt, &edge->mControlPt );
				edge->mControlPt = curPt;
				curPt.x += GetSBits(nBits);
				curPt.y += GetSBits(nBits);
				//MatrixTransformPoint(&mat, &curPt, &edge->mEndPt );
				edge->mEndPt = curPt;

				/*char pTmp[256];
				memset(pTmp, 0, 256 );
				sprintf(pTmp, "line:%d,cou:%d,ControlPt->x:%d, y:%d, EndPt->x:%d, y:%d", __LINE__,
					icou, edge.mControlPt.x, edge.mControlPt.y, edge.mEndPt.x, edge.mEndPt.y );					
				OutputDebugStringA( pTmp );*/

			}
			else
			{
				// Handle a line
				int nBits = (int)GetBits(4)+2;	// nBits is biased by 2

				BOOL generalLine = (int)GetBits(1);

				if ( generalLine ) 
				{
					curPt.x += GetSBits(nBits);//x;
					curPt.y += GetSBits(nBits);//y;
				}
				else 
				{
					BOOL vertical = (int)GetBits(1);
					if ( vertical ) 
					{
						curPt.y += GetSBits(nBits);
					} 
					else
					{
						// Horizontal line
						curPt.x += GetSBits(nBits);
					}
				}
				//MatrixTransformPoint(&mat, &curPt, &edge->mEndPt );
				edge->mEndPt = curPt;
				edge->mControlPt = edge->mEndPt;
				/*char pTmp[256];
				memset(pTmp, 0, 256 );
				sprintf(pTmp, "line:%d,cou:%d,ControlPt->x:%d, y:%d, EndPt->x:%d, y:%d", __LINE__,
					icou, edge.mControlPt.x, edge.mControlPt.y, edge.mEndPt.x, edge.mEndPt.y );					
				OutputDebugStringA( pTmp );*/
				//PointAverage(&curPtX, &edge.mEndPt, &edge.mControlPt );
			}
			path->AddEdge( edge );
			curPtX = edge->mEndPt;
			// FLASHOUTPUT("Edge-f1=%i, f2=%i, l=%i, a2.x=%i, a2.y=%i\n", fill[0], fill[1], line, pt.x, pt.y);
		}
	}
	return 0;
}


//
//int SShapeParser::GetEdge(CURVE* c,bool bBG)//,int div)
//{
//	BOOL isEdge = (int)GetBits(1);
//	if ( !isEdge ) {
//		// Handle a state change
//		int flags = (int)GetBits(5);
//
//		if ( flags == 0 )
//			// at end, do nothing
//			return eflagsEnd;
//
//		// Process a state change
//
//		// Process a MoveTo
//		if ( flags & eflagsMoveTo ) {
//			int nBits = (int)GetBits(5);
//			curPt.x = GetSBits(nBits);
//			curPt.y = GetSBits(nBits);
//			/*if(div)
//			{
//				curPt.x/=div;
//				curPt.y/=div;
//			}*/
//			MatrixTransformPoint(&mat, &curPt, &curPtX);
//		}
//
//		// Get new fill info
//		if ( flags & eflagsFill0 )
//			fill[0] = (int)GetBits(nFillBits);
//		if ( flags & eflagsFill1 )
//			fill[1] = (int)GetBits(nFillBits);
//
//		// Get new line info
//		if ( flags & eflagsLine )
//		{
//			line = (int)GetBits(nLineBits);
//			//if(line>nLines)
//			//	int v=0;
//		}
//
//		// Check to get a new set of styles for a new shape layer
//		if ( flags & eflagsNewStyles ) {
//		 	layer += nFills+nLines;
//#ifdef _BACKLAYER
//			if(bBG)
//				GetBGColors();
//			else
//				GetStyles();
//#else
//			GetStyles();
//#endif
//
//		}
//
//		if(fill[0]>nFills)
//			fill[0]=0;
//		if(fill[1]>nFills)
//			fill[1]=0;
//		if(line>nLines)
//			line=0;
//
//		//FLASHASSERT(fill[0]<=nFills && fill[1]<=nFills && line<=nLines);	// be sure the styles are loaded
//
//		return flags;
//
//	} else {
//		// Create an edge
//		c->anchor1 = curPtX;
//
//		c->isLine = (int) GetBits(1);
//		if ( c->isLine ) {
//			// Handle a line
//			int nBits = (int)GetBits(4)+2;	// nBits is biased by 2
//
//			// Save the deltas
//			BOOL generalLine = (int)GetBits(1);
//			if ( generalLine ) {
//				// Handle a general line
//				/*int x=GetSBits(nBits);
//				int y=GetSBits(nBits);
//				if(div)
//				{
//					x/=div;
//					y/=div;
//				}*/
//				curPt.x += GetSBits(nBits);//x;
//				curPt.y += GetSBits(nBits);//y;
//			} else {
//				// Handle a vert or horiz line
//				BOOL vertical = (int)GetBits(1);
//				//int v=GetSBits(nBits);
//				//if(div) v/=div;
//				if ( vertical ) {
//					// Vertical line
//					curPt.y += GetSBits(nBits);
//
//				} else {
//					// Horizontal line
//					curPt.x += GetSBits(nBits);
//				}
//			}
//
//			MatrixTransformPoint(&mat, &curPt, &c->anchor2);
//			PointAverage(&c->anchor1, &c->anchor2, &c->control);
//
//		} else {
//		 	// Handle a curve
//			int nBits = (int)GetBits(4)+2;	// nBits is biased by 2
//
//			// Get the control
//			/*int x=GetSBits(nBits);
//			int y=GetSBits(nBits);
//			if(div)
//			{
//				x/=div;
//				y/=div;
//			}*/
//			curPt.x += GetSBits(nBits);
//			curPt.y += GetSBits(nBits);
//
//
//
//			MatrixTransformPoint(&mat, &curPt, &c->control);
//
//			// Get the anchor
//			/*x=GetSBits(nBits);
//			y=GetSBits(nBits);
//			if(div)
//			{
//				x/=div;
//				y/=div;
//			}*/
//
//			curPt.x += GetSBits(nBits);
//			curPt.y += GetSBits(nBits);
//
//			MatrixTransformPoint(&mat, &curPt, &c->anchor2);
//		}
//
//		curPtX = c->anchor2;
//		// FLASHOUTPUT("Edge-f1=%i, f2=%i, l=%i, a2.x=%i, a2.y=%i\n", fill[0], fill[1], line, pt.x, pt.y);
//		return 0;
//	}
//}

//
//void SCharacterParser::AddCurve(P_CURVE src)
//{
//	CURVE c;
//	S32 dir;
//	
//	int dx=src->anchor2.x-src->anchor1.x;
//	int dy=src->anchor2.y-src->anchor1.y;
//	if(dx<0) dx=-dx;
//	if(dy<0) dy=-dy;
//
//	c = *src;
//
//	// Sort the anchors
//	if ( c.anchor1.y <= c.anchor2.y )
//	{
//		dir = 1;
//	} 
//	else
//	{
//		dir = -1;
//		Swap(c.anchor1, c.anchor2, SPOINT);
//	}
//	
//	if ( !c.isLine )
//	{
//		if ( c.control.y < c.anchor1.y || c.control.y > c.anchor2.y ) {
//			// Handle a minimum or maximum
//		
//			// If it is close, just adjust the curve
//			if ( c.control.y < c.anchor1.y && c.anchor1.y - c.control.y < 3 ) {
//				c.control.y = c.anchor1.y;
//			
//			} else if ( c.control.y > c.anchor2.y && c.control.y - c.anchor2.y < 3 ) {
//				c.control.y = c.anchor2.y;   
//			
//			} else {
//				SCOORD a, b;
//				CURVE c1, c2;
//				
//				// Use the curve with the original order
//				a = src->anchor1.y - 2*src->control.y + src->anchor2.y;
//				b = src->anchor1.y - src->control.y;
//			
//				// See if b/a is in the range 0..1
//				// a and b must have the same sign and abs(b) <= abs(a)
//				FLASHASSERT(((a > 0 && b >= 0) || (a < 0 && b <= 0)) && Abs(b) <= Abs(a));
//				
//				CurveDivide(src, FixedDiv(b, a), &c1, &c2);
//				if ( ++depth > 16 ) {
//					FLASHASSERT(XFALSE);
//					return;
//				}
//				AddCurve(&c1);
//				AddCurve(&c2);
//				depth--;
//				return;
//			}
//		}
//
//		// Split any curves that are too long
//		if ( c.anchor2.y - c.anchor1.y > 256 ) {
//			CURVE c1, c2;
//			CurveDivide(src, &c1, &c2);
//			if ( ++depth > 16 ) {
//				FLASHASSERT(XFALSE);
//				return;
//			}
//			AddCurve(&c1);
//			AddCurve(&c2);
//			depth--;
//			return;
//		}
//	}
//
//	if ( c.anchor1.y == c.anchor2.y ) 
//		return;	// this is a horizontal edge, we can ignore it
//
//	if ( preclip ) {
//		SRECT b;
//
//		// Check to clip
//		CurveBounds(&c, &b);
//
//		// If the curve is above or below, we can completely ignore it
//		// We assumme that the largest image will be 4000x4000 pixels
//		if ( b.ymax < -16000 || b.ymin > 16000 ) return;
//
//		// Pre-clip to prevent overflow...
//		if ( b.xmin < -32000 || b.xmax > 32000 || b.ymin < -32000 || b.ymax > 32000 ) {
//			if ( b.xmax < -16000 ) {
//				// The curve is completely off to the left, make a vertical line
//				MakeVertLine(&c, -32000);
//			} else if ( b.xmin > 16000 ) {
//				// Curve is completely off to the right, make a vertical line
//				MakeVertLine(&c, 32000);
//			} else if ( depth < 16 ) {
//				// Split the curve
//				CURVE c1, c2;
//				CurveDivide(src, &c1, &c2);
//				depth++;
//				AddCurve(&c1);
//				AddCurve(&c2);
//				depth--;
//				return;
//			}
//		}
//	}
//
//	/*if(obj&&obj->clipRect)
//	{
//		int xmin=obj->clipRect->xmin;
//		int ymin=obj->clipRect->ymin;
//		int xmax=obj->clipRect->xmax;
//		int ymax=obj->clipRect->ymax;
//		SRECT b;
//		// Check to clip
//		CurveBounds(&c, &b);
//		if(b.ymax<ymin||b.ymin>ymax) 
//			return;
//		//if(b.ymin>ymax) return;
//	}*/
//
//	// Set up a new edge
//	REdge* edge = display->CreateEdge();
//	if ( !edge ) return;
//
//	edge->nextActive=NULL;
//	/*REdge*t=obj->edges;
//	while(t)
//	{
//		if(t==edge)
//			int v=0;
//		t=t->nextObj;
//	}*/
//
//    // Set up the curve
//	edge->Set(&c);
//	edge->dir = (S8)dir;
//	
//	// Track the colors
//	edge->fillRule = (U8)fillRule;
//	edge->color1 = color1;
//	edge->color2 = color2;
//	
//	// Add to the edge list
//	edge->nextObj = obj->edges;
//	obj->edges = edge;
//}


//void SCharacterParser::MakeClipper()
//{
//	// Create a clip color 
//	//if((*colorList)->colorType == colorClip) return;
//	RColor* clipColor = XNULL;
//	if(*colorList&&(*colorList)->colorType == colorClip)
//		clipColor=*colorList;
//	else clipColor=CreateClipColor(display, colorList);
//
//	{// Set all of the fills to our clip color
//		for ( int i = 1; i <= nFills; i++ ) {
//			fillIndex[i] = clipColor;
//		}
//	}
//
//	{// We don't clip with lines
//		for ( int i = 1; i <= nLines; i++ ) {
//			lineIndex[i].color = 0;
//		}
//	}
//}


void SCharacterParser::BuildEdges(BOOL getStyles,bool bTess)//, SRECT* devBounds,XU32 nFilterColor)
{

	//depth = 0;
	//if(!display->player->m_pDomFilter|| !display->player->m_pDomFilter->bEnuFilter)
	//	nFilterColor=0;

	if ( getStyles ) 
	{
		if( !GetBwStyles(bTess))
		{
			return;
		}
	} 
	else 
	{
		// See how large we need the fill and line tags to be
		InitBits();
		nFillBits = (int)GetBits(4);
		nLineBits = (int)GetBits(4);
	}
	int flags = GetBwPaths( );//&obj->mShapeRecord );
	/*if(!getStyles)
	{
		BwPath*path=pShapeRec->mpPaths;
		while(path)
		{
			if(path->mFillLeft)
			{
				path->FillLeft(0);
				path->FillRight(1);
			}
			path = path->pNext;
		}
	}*/
	if(bTess&&!pShapeRec->IsEmpty())
	{
		if(!getStyles)
			pShapeRec->fMeter=400;
		pShapeRec->m_bODD = m_bODD;
		TessPloy::Tess(pShapeRec,pShapeRec->fMeter,m_bODD);
	}
	return;
}

void SCharacterParser::BuildMorphEdges()//SRECT* devBounds)
{
	/*char chTmp[256];
	sprintf( chTmp, "File:%s, L:%d, Fun:%s", __FILE__, __LINE__, __FUNCTION__ );
	OutputDebugStringA( chTmp );*/

	//depth = 0;

	// Set up the second shape parser
	S32 offset = GetDWord();

	// Note that we always ignore the styles from the second set of edges, they should always be zero
	SShapeParser parseEdges(player, script, pos+offset, pShapeRec);//&mat);
	parseEdges.InitBits();
	parseEdges.nFillBits = parseEdges.GetBits(4);
	parseEdges.nLineBits = parseEdges.GetBits(4);

	// build the interpolated fill and line styles for this morph shape
	if ( !GetBwMorphStyles( ratio) ) 
		return;

	//if ( obj->clipDepth )
	//	MakeClipper();

	InitBits();
	nFillBits = (int)GetBits(4);
	nLineBits = (int)GetBits(4);
	int flags = GetBwMorphPaths( &parseEdges, ratio );
	if(!pShapeRec->IsEmpty())
	{
		pShapeRec->m_bODD = m_bODD;
		TessPloy::Tess(pShapeRec,pShapeRec->fMeter,m_bODD);
	}
	return;
}


BOOL SCharacterParser::ReadGLMorphColor(GLColor*color)
{
	MATRIX mat1, mat2;
	int fillStyle = GetByte();
	if ( fillStyle & fillGradient )
	{

		color->colorType = glColorGradient;
		color->grad.gradStyle = fillStyle;

		GetMatrix(&mat1);
		GetMatrix(&mat2);
		Interpolate(&mat1, &mat2, ratio, &color->grad.savedMat);

		//if(bitPos)
		//	int v=0;
		U8 nColor=GetByte();
		color->grad.ramp.nColors = nColor&0xf;
		color->grad.ramp.spreadMode	= (nColor>>6);
		color->grad.ramp.interMode	= (nColor>>4)&3;
		for ( int j = 0; j < color->grad.ramp.nColors; j++ )
		{
			U8 r1, r2;
			SRGB c1, c2;

			r1 = GetByte();
			c1 = GetColor(getAlpha);
			r2 = GetByte();
			c2 = GetColor(getAlpha);
			if(j<fillMaxGradientColors)
			{
				color->grad.ramp.colorRatio[j] = (U8)Interpolate(r1, r2, ratio);
				Interpolate(&c1, &c2, ratio, &color->grad.ramp.color[j]);

				//if ( c1.rgb.transparency < 255 || c2.rgb.transparency < 255)
				//{
				//	color->transparent = true;
				//}
			}
		}

		/*if ( cxform.HasTransform() )
		{
		cxform.Apply(&color->grad.ramp);
		//if ( cxform.HasTransparency() )
		//{
		//	color->transparent = true;
		//}
		}*/

	}
	else if ( fillStyle & fillBits )
	{
		// A bitmap fill
		U16 tag = GetWord();		// the bitmap tag

		GetMatrix(&mat1);
		GetMatrix(&mat2);
		//			Interpolate(&mat1, &mat2, 0, &color->bm.savedMat);	// The original line is missing the "ratio"
		// Bug from David Michie (Middlesoft) and
		// Jeff Martin. Changed lee@middlesoft
		Interpolate(&mat1, &mat2, ratio, &color->bm.savedMat);
		SCharacter* ch = player->FindCharacter(tag);

		// Default to red if we can't find the bits
		color->rgb.alpha = 255;
		color->rgb.red = 255;
		color->rgb.green = color->rgb.blue = 0;

		if ( ch && ch->type == bitsChar )
		{
			// We found the bits, set up the color
			color->colorType = glColorBitmap;
			color->bm.bitmap = &ch->bits;
			color->bm.bitsStyle = (U16)fillStyle;
			color->bm.repeat = (fillStyle&fillBitsClip)?0:1;
			//color->bm.cxform = cxform;
			//memcpy(&color->bm.cxform,&cxform,sizeof(GLColorTransform));
			//	color->bm.rowIndex = 0;

			player->BuildBits(ch);	// be sure the bits are decompressed and available
			//color->SetBitmapDiv();
			//if(cxform.nBlendMode)
			//		color->bm.bitmap->SetBlendMode(cxform.nBlendMode);
		}
	}
	else
	{
		RGBI rgb1, rgb2;
		// A solid color
		GetColor(&rgb1, getAlpha);
		GetColor(&rgb2, getAlpha);

		Interpolate(&rgb1, &rgb2, ratio, &color->rgb);
		/*if ( cxform.HasTransform() )
		{
		cxform.Apply(&color->rgb);
		}*/
		//color->transparent = (U8)PreMulAlpha(&color->rgb);
	}
	color->SetUp();
	return XTRUE;

}

BOOL SCharacterParser::GetBwMorphStyles( int ratio )
{
	
	int i;
	ShapeAlloc*pAlloc = ShapeAlloc::GetInst();	
	nFills = GetByte();
	if ( nFills >= 255 )
		nFills = GetWord();
	int nfs = pShapeRec->mnFills;
	pShapeRec->SetFills(nfs+nFills+1);nfs++;
	for ( i = 1; i <= nFills; i++ )
	{
		//RColor* color = display->CreateColor();
		GLColor* color = pAlloc->AllocColor();
		if ( !color )
			return false;

		pShapeRec->AddFillSyle(nfs,color);nfs++;
		
		ReadGLMorphColor(color);
		//SetupColor(color);		// the buildcache in this function must be called immediately after BuildBits()
		//if(clipColor)
		//	display->FreeColor(color);
	}

	// Get the lines
	nLines = GetByte();
	if ( nLines >= 255 )
	{
		// We have a "large number"
		nLines = GetWord();
		/*if ( lineIndex != lineIndexMem )
			delete lineIndex;
		lineIndex = new SLine[nLines+1];
		if ( !lineIndex )
		{
			lineIndex = lineIndexMem;
			return false;
		}*/
	}

	//lineIndex[0].color = 0;
	//lineIndex[0].thickness = 0;
	int nls = pShapeRec->mnLines;
	pShapeRec->SetLines(nls+nLines+1);
	nls++;
	for (i = 1; i <= nLines; i++ )
	{
		U16		thickness, thick1,	thick2;
		RGBI	rgb1, rgb2;

		BwLineStyle* pLineStyle=pAlloc->AllocLineStyle();
		pShapeRec->AddLineStyle(nls,pLineStyle);nls++;
		//RColor* color = display->CreateColor();
		//if ( !color )
		//	return false;

		//color->SetUp(raster);
		//if(clipColor)
		//	lineIndex[i].color=clipColor;
		//else
		//{
		//	color->nextColor = *colorList;
		//	*colorList = color;
		//	lineIndex[i].color = color;
		//}

		// get the thickness
		thick1 = GetWord();
		thick2 = GetWord();
		thickness = (U16)Interpolate(thick1, thick2, ratio);

		pLineStyle->mWidth = thickness;//(int)MatrixTransformThickness(&mat, thickness);

		//color->order = layer + (i+nFills);
		//FLASHASSERT(color->order < 0x10000);
		XBOOL hasFill=0;
		if(lineStyle2)
		{
			XU8 f1=GetByte();
			hasFill=f1&0x08;
			SkipBytes(1);
			if((f1&0x30)==0x20)
				SkipBytes(2);
		}

		// get the color
		if(hasFill)
		{
			GLColor*pColor=pAlloc->AllocColor();
			pLineStyle->color = pColor;
			//ReadGLColor(pColor);
			ReadGLMorphColor(pColor);
			//ReadColor(color);
		}
		else
		{
			GetColor(&rgb1, getAlpha);
			GetColor(&rgb2, getAlpha);

		// interpolate the color value
			Interpolate(&rgb1, &rgb2, ratio, &pLineStyle->mColor);
			//if ( cxform.HasTransform() )
				//cxform.Apply(&);
		}
		
		//color->transparent = (U8)PreMulAlpha(&color->rgb);
		//SetupColor(color);
		//if(clipColor)
		//	display->FreeColor(color);
	}
	return true;
}

/*BOOL SCharacterParser::GetBwMorphStyles( BwShapeRecord * pShapeRec, int ratio )
{
	nFills = GetByte();
	if ( nFills > 0 )
	{
		if( nFills == 255 )
			nFills = GetWord();
		pShapeRec->AddFillSyle( new BwFillStyle() );
		for( int i = 1; i <= nFills; i++ )
		{
			ReadMorphFillStyle( pShapeRec->mVecFillStyles, ratio );
		}
	}

	// Get the lines
	nLines = GetByte();
	if ( nLines > 0 )
	{
		if( nLines == 0xFF )
			nLines = GetWord();
		pShapeRec->AddLineStyle( BwLineStyle() );
		for( int i = 1; i <= nLines; i++ )
		{
			BwLineStyle lineStyle;
			U16 thickness, thick1, thick2;
			RGBI rgb1, rgb2, color;
			thick1 = GetWord();
			thick2 = GetWord();
			thickness = ( U16 )Interpolate( thick1, thick2, ratio );
			lineStyle.LineWidth( MatrixTransformThickness( &mat, thickness ) ) ;
			XBOOL hasFill = 0;
			if( lineStyle2 )
			{
				XU8 f1 = GetByte();
				hasFill = f1 & 0x08;
				SkipBytes( 1 );
				if( (f1 & 0x30) == 0x20 )
					SkipBytes( 2 );
			}
			if( hasFill )
			{
				ReadMorphFillStyle( pShapeRec->mVecFillStyles, ratio );
			}
			else
			{
				GetColor( &rgb1, getAlpha );
				GetColor( &rgb2, getAlpha );
				Interpolate( &rgb1, &rgb2, ratio, &color );
			}
			if( cxform.HasTransform() )
				cxform.Apply( &color );
			lineStyle.LineColor( color );
			pShapeRec->AddLineStyle( lineStyle );

		}
	}
	return true;
}*/

int SCharacterParser::GetBwMorphPaths( SShapeParser *parser, int ratio )
{
	//BwPath* path;
	int fill_base = 0;
	int line_base = 0;
	int icou = 0;
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	BwPath *path= pAlloc->AllocPath();
	//path->StartPt( curPtX );
	int fLeft = 0;
	int fRight = 0;
	int lineStyle = 0;
	BwShapeRecord* paths = pShapeRec;
	bool bFirst = true;
	while( true )
	{
		BOOL isEdge = (int)GetBits(1);
		if ( !isEdge )
		{
			// Handle a state change
			int flags = (int)GetBits(5);
			if ( flags == 0 )
			{
				if( !path->EdgeEmpty() )
				{
					//XPOINT pt;
					//pt.x = Interpolate( curPtX.x, parser->curPtX.x, ratio );
					//pt.y = Interpolate( curPtX.y, parser->curPtX.y, ratio );
					//path->StartPt( pt );	
					paths->AddPath( path );
					//path.ClearEdges();
					/*char pTmp[256];
					memset(pTmp, 0, 256 );
					sprintf(pTmp, "line:%d,startPt->x:%d, y:%d", __LINE__, pt.x, pt.y );
					OutputDebugStringA( pTmp );*/
				}
				else pAlloc->FreePath(path);

				return eflagsEnd;
			}
			if ( flags & eflagsMoveTo ) 
			{
				if( !path->EdgeEmpty() )
				{
					paths->AddPath( path );
					//path.ClearEdges();
					path = pAlloc->AllocPath();
					XPOINT pt;
					pt.x = Interpolate( curPtX.x, parser->curPtX.x, ratio );
					pt.y = Interpolate( curPtX.y, parser->curPtX.y, ratio );
					path->StartPt( pt );	
					path->FillLeft(fLeft);
					path->FillRight(fRight);
					path->LineStyle(lineStyle);
				}
				int nBits = (int)GetBits(5);
				curPt.x = GetSBits(nBits);
				curPt.y = GetSBits(nBits);
				//MatrixTransformPoint(&mat, &curPt, &curPtX);
				curPtX = curPt;
				//parser->GetBwPaths()
				BwEdge edge2;
				parser->GetBwEdge( edge2 );
				SPOINT pt;
				pt.x = Interpolate( curPtX.x, parser->curPtX.x, ratio );
				pt.y = Interpolate( curPtX.y, parser->curPtX.y, ratio );
				path->StartPt( pt );
				/*char pTmp[256];
				memset(pTmp, 0, 256 );
				sprintf(pTmp, "line:%d,startPt->x:%d, y:%d", __LINE__, curPtX.x, curPtX.y );
				OutputDebugStringA( pTmp );*/
			}

			// Get new fill info
			if ( flags & eflagsFill0 )
			{
				if( !path->EdgeEmpty() )
				{
					/*XPOINT pt;
					pt.x = Interpolate( curPtX.x, parser->curPtX.x, ratio );
					pt.y = Interpolate( curPtX.y, parser->curPtX.y, ratio );
					path->StartPt( pt );*/	
					paths->AddPath( path );
					//path.ClearEdges();
					path = pAlloc->AllocPath();
					//path = pAlloc->AllocPath();
					XPOINT pt;
					pt.x = Interpolate( curPtX.x, parser->curPtX.x, ratio );
					pt.y = Interpolate( curPtX.y, parser->curPtX.y, ratio );
					path->StartPt( pt );	
					path->FillLeft(fLeft);
					path->FillRight(fRight);
					path->LineStyle(lineStyle);
					/*char pTmp[256];
					memset(pTmp, 0, 256 );
					sprintf(pTmp, "line:%d,startPt->x:%d, y:%d", __LINE__, pt.x, pt.y );	
					OutputDebugStringA( pTmp );*/
				}
				int style = ( int )GetBits( nFillBits );
				if( style > 0 )
				{
					style += fill_base;
				}
				fLeft = style;
				path->FillLeft( style );
			}
			if ( flags & eflagsFill1 )
			{
				if( !path->EdgeEmpty() )
				{
					/*XPOINT pt;
					pt.x = Interpolate( curPtX.x, parser->curPtX.x, ratio );
					pt.y = Interpolate( curPtX.y, parser->curPtX.y, ratio );
					path->StartPt( pt );*/
					paths->AddPath( path );
					//path.ClearEdges();
					path = pAlloc->AllocPath();
					//path = pAlloc->AllocPath();
					XPOINT pt;
					pt.x = Interpolate( curPtX.x, parser->curPtX.x, ratio );
					pt.y = Interpolate( curPtX.y, parser->curPtX.y, ratio );
					path->StartPt( pt );	
					path->FillLeft(fLeft);
					path->FillRight(fRight);
					path->LineStyle(lineStyle);
					/*char pTmp[256];
					memset(pTmp, 0, 256 );
					sprintf(pTmp, "line:%d,startPt->x:%d, y:%d", __LINE__, pt.x, pt.y );
					OutputDebugStringA( pTmp );*/
				}
				int style = ( int)GetBits( nFillBits );
				if( style > 0 )
				{
					style += fill_base;
				}
				fRight = style;
				path->FillRight( style );
			}
			if(bFirst&&(flags&(eflagsFill1|eflagsFill0)))
			{
				m_bODD=(path->FillRight()&&path->FillLeft())?false:(this->useWinding?false:true);
				bFirst=false;
			}

			// Get new line info
			if ( flags & eflagsLine )
			{
				if( !path->EdgeEmpty() )
				{
					/*XPOINT pt;
					pt.x = Interpolate( curPtX.x, parser->curPtX.x, ratio );
					pt.y = Interpolate( curPtX.y, parser->curPtX.y, ratio );
					path->StartPt( pt );*/
					paths->AddPath( path );
					//path.ClearEdges();
					path = pAlloc->AllocPath();
					//path = pAlloc->AllocPath();
					XPOINT pt;
					pt.x = Interpolate( curPtX.x, parser->curPtX.x, ratio );
					pt.y = Interpolate( curPtX.y, parser->curPtX.y, ratio );
					path->StartPt( pt );	
					path->FillLeft(fLeft);
					path->FillRight(fRight);
					/*char pTmp[256];
					memset(pTmp, 0, 256 );
					sprintf(pTmp, "line:%d,startPt->x:%d, y:%d", __LINE__, pt.x, pt.y );					
					OutputDebugStringA( pTmp );*/
				}
				int style = (int)GetBits(nLineBits);
				if( style > 0 )
				{
					style += line_base;
				}
				//line = (int)GetBits(nLineBits);
				path->LineStyle( style );
				lineStyle = style;
			}

			// Check to get a new set of styles for a new shape layer
			if ( flags & eflagsNewStyles )
			{
				if( !path->EdgeEmpty() )
				{
					/*XPOINT pt;
					pt.x = Interpolate( curPtX.x, parser->curPtX.x, ratio );
					pt.y = Interpolate( curPtX.y, parser->curPtX.y, ratio );
					path->StartPt( pt );*/
					paths->AddPath( path );
					//path.ClearEdges();
					path = pAlloc->AllocPath();
					//path = pAlloc->AllocPath();
					XPOINT pt;
					pt.x = Interpolate( curPtX.x, parser->curPtX.x, ratio );
					pt.y = Interpolate( curPtX.y, parser->curPtX.y, ratio );
					path->StartPt( pt );	
				}
				path->mbNewShape = true;
				//BwPath bpt;
				//bpt.mbNewShape = true;
				//paths->AddPath( bpt );
				//layer += nFills+nLines;
				fill_base = paths->mnFills;//mVecFillStyles.size();
				line_base = paths->mnLines;//mVecLineStyles.size();
				GetBwStyles( );
			}
		} 
		else
		{
			icou++;
			BwEdge edge;
			edge.IsLine( (int) GetBits(1) );
			if ( edge.IsLine() == 0 ) 
			{
				int nBits = (int)GetBits(4)+2;	// nBits is biased by 2

				curPt.x += GetSBits(nBits);
				curPt.y += GetSBits(nBits);
				//MatrixTransformPoint(&mat, &curPt, &edge.mControlPt );
				edge.mControlPt = curPt;
				curPt.x += GetSBits(nBits);
				curPt.y += GetSBits(nBits);
				//MatrixTransformPoint(&mat, &curPt, &edge.mEndPt );
				edge.mEndPt = curPt;
				curPtX = curPt;
				/*char pTmp[256];
				memset(pTmp, 0, 256 );
				sprintf(pTmp, "line:%d,cou:%d,ControlPt->x:%d, y:%d, EndPt->x:%d, y:%d", __LINE__,
					icou, edge.mControlPt.x, edge.mControlPt.y, edge.mEndPt.x, edge.mEndPt.y );					
				OutputDebugStringA( pTmp );*/

			}
			else
			{
				// Handle a line
				int nBits = (int)GetBits(4)+2;	// nBits is biased by 2

				BOOL generalLine = (int)GetBits(1);

				if ( generalLine ) 
				{
					curPt.x += GetSBits(nBits);//x;
					curPt.y += GetSBits(nBits);//y;
				}
				else 
				{
					BOOL vertical = (int)GetBits(1);
					if ( vertical ) 
					{
						curPt.y += GetSBits(nBits);
					} 
					else
					{
						// Horizontal line
						curPt.x += GetSBits(nBits);
					}
				}
				//MatrixTransformPoint(&mat, &curPt, &edge.mEndPt );
				curPtX = curPt;
				edge.mEndPt = curPt;
				edge.mControlPt = edge.mEndPt;
				/*char pTmp[256];
				memset(pTmp, 0, 256 );
				sprintf(pTmp, "line:%d,cou:%d,ControlPt->x:%d, y:%d, EndPt->x:%d, y:%d", __LINE__,
					icou, edge.mControlPt.x, edge.mControlPt.y, edge.mEndPt.x, edge.mEndPt.y );					
				OutputDebugStringA( pTmp );*/
				//PointAverage(&curPtX, &edge.mEndPt, &edge.mControlPt );
			}
			BwEdge edge2;
			//if( parser->GetBwEdge( edge2 ))
				parser->GetBwEdge( edge2 );
			BwEdge* finalEdge=pAlloc->AllocEdge(); 
			finalEdge->IsLine( edge.IsLine() );
			finalEdge->mControlPt.x = Interpolate( edge.mControlPt.x, edge2.mControlPt.x, ratio );
			finalEdge->mControlPt.y = Interpolate( edge.mControlPt.y, edge2.mControlPt.y, ratio );
			finalEdge->mEndPt.x = Interpolate( edge.mEndPt.x, edge2.mEndPt.x, ratio );
			finalEdge->mEndPt.y = Interpolate( edge.mEndPt.y, edge2.mEndPt.y, ratio );
			//Interpolate( edge.mControlPt,)
			path->AddEdge( finalEdge );
			// FLASHOUTPUT("Edge-f1=%i, f2=%i, l=%i, a2.x=%i, a2.y=%i\n", fill[0], fill[1], line, pt.x, pt.y);
		}
	}
	return 0;
}

//
// Script Player
//

static int nCount=0;

void ScriptThread::InitThread()
{
	//m_nClasses = 0;
	m_nShapes = 0;
	this->m_nAS3SencePos=0;
	m_bNeedCallFrame=false;
//	needCallFrame=XFALSE;
	//this->m_bNoEnter=false;
	this->m_nAS3Frames=0;
	this->m_bAS3Frame0=false;
	this->m_pCmds=XNULL;
//this->m_bLoadedOK=true;
	numFramesComplete=0x7fffffff;	// info for tracking the numbers of frames loaded
	m_bInDisplay=false;
	m_nCurX=0;
	m_nCurY=0;
	script = 0;
	display = 0;
	rootObject = 0;
	layerDepth = 0;
	nEventFlags=0;
	pActions=XNULL;
	player = 0;
	next = 0;
    len = 0;
    startPos = 0;
	m_cmds=XNULL;
	m_bFill=XFALSE;
//	nForeColor=0;
//	nBackColor=0;
//	m_pDrawObj=XNULL;
//	firstVariable = 0;
//	lastVariable = 0;

    justSeeked = true;
    justSeekedNegative = false;
	bNewLoader=XFALSE;
#ifdef SOUND
	needBuffer = true;
	sndChannel = 0;
    snd.Init();
#endif
//	m_pCalling=XNULL;
	ClearState();
}

ScriptThread::ScriptThread()
{
	//if(rootObject&&rootObject->character&&rootObject->character->tag==43)
	//nCount++;
	InitThread();
//	m_pClip=XXObjectCreator::CreateClip(this);
}

ScriptThread::~ScriptThread()
{
	//nCount--;
//	XActionScript*p=m_pCalling;
//	while(p)
//	{
//		p->RemoveThread(this);
//		p=p->m_pNext;
//	}
	if(display)
	{
		while(m_pCmds)
		{
			display->freeCmd(m_pCmds);
			m_pCmds=m_pCmds->pNext;
		}
		if(m_cmds)
				FreeCommands(m_cmds);
			m_cmds=XNULL;
	}
#ifdef SOUND
	StopStream();
#endif

	if ( display )
		display->RemoveThread(this);

	// Clear all the variables
	// ClearVariables();
	ClearActions();

	if(bNewLoader)
		delete player;
	//if(m_pDrawObj)
	//	delete m_pDrawObj;
	//m_pDrawObj=XNULL;
//	if(m_pClip)
//	{
		//m_pClip->SetParent(XNULL);
//		m_pClip->Destroy();
//		m_pClip=XNULL;
//	}

}

void ScriptThread::ClearVariables()
{
	if(rootObject&&rootObject->pObject)
		rootObject->pObject->ClearMembers();
	/*while ( firstVariable ) {
		ScriptVariable* v = firstVariable;
		firstVariable = v->next;

		FreeStr(v->name);
		FreeStr(v->value);
		delete v;
	}
	lastVariable = 0;*/
//	if(m_pClip)
//		m_pClip->ClearMembers();
}

void ScriptThread::ClearState()
{
	// Reset the script attributes
	curFrame = -1;
	currentLabel = 0;
	labelFrame = 0;
	atEnd = false;
	seeking = false;

	len = 0;
	pos = 0;
	script = 0;

#ifdef SOUND
	StopStream();
	mixFormat = snd22K16Stereo;
#endif

	numFrames = 0;
	scriptErr = 0;

// 	#ifdef _CELANIMATOR
// 	protect = false;
// 	pathsArePostScript = false;
// 	#endif

	playing = true;
	loop = true;
}


//
// Handle Specific Tags
//

void ScriptThread::PlaceObject()
{
	FLASHASSERT(rootObject);

	PlaceInfo info;
	info.flags = splaceCharacter|splaceMatrix;
	U16 tag = GetWord();
	info.character = player->FindCharacter(tag);

	info.depth = GetWord();

	//FLASHOUTPUT("Place tag=%i depth=%i\n", tag, depth);

	#ifdef FSSERVER
	if ( !info.character )
		info.character = lostCh;
	#endif
	if ( !info.character ) return;

	// Get the position
	GetMatrix(&info.mat);

	// Get a color transform
	if ( pos < tagEnd ) {
		GetColorTransform(&info.cxform);
		info.flags |= splaceColorTransform;
	} else {
		info.cxform.Clear();
	}

	info.ratio = 0;
	info.name = _strEmpty;
	info.bAdd=this->m_bInDisplay;
		//rootObject?rootObject->IsInDisplay():true;
		//this->m_bInDisplay;

	display->PlaceObject2(rootObject, &info, XNULL,false);//!seeking);
}

void ScriptThread::PlaceObject2()
{
	FLASHASSERT(rootObject);

	PlaceInfo info;
	info.flags = GetByte();
	info.depth = GetWord();

	if ( info.flags & splaceCharacter ) {
		info.character = player->FindCharacter(GetWord());

		#ifdef FSSERVER
		if ( !info.character )
			info.character = lostCh;
		#endif
		if ( !info.character )
			return;

//		if(info.character->tag==116)
//			int v=0;
	} else
		info.character = 0;

	if ( info.flags & splaceMatrix )
		GetMatrix(&info.mat);
	else
		MatrixIdentity(&info.mat);

	if ( info.flags & splaceColorTransform )
		GetColorTransform(&info.cxform, true);
	else
		info.cxform.Clear();

	if ( info.flags & splaceRatio )
		info.ratio = GetWord();
	else
		info.ratio = 0;

	if ( info.flags & splaceName )
	{
		info.name = GetStringP();
			//(char*)AttachData(pos, tagEnd);//script+pos;
		//pos+=XString8::SafeStrlen(info.name)+1;
	}
	else
		info.name = _strEmpty;

	if ( info.flags & splaceDefineClip )
		info.clipDepth = GetWord();
	else
		info.clipDepth = 0;

	if((info.flags&extPlaceClipActions))
	{
	//	if(info.character&&info.character->tag==175)
	//		int v=0;
		PlaceAction(&info);
	}
	//SObject*pObj=XNULL;
	info.bAdd=this->m_bInDisplay;
		//rootObject?rootObject->IsInDisplay():true;//this->m_bInDisplay;

	if ( info.flags & splaceMove )
	{
		//if ( info.flags & splaceCharacter)
		//	RemoveDepth(info.depth);
		//else
			display->MoveObject(rootObject, &info);
	}
	else if ( info.flags & splaceCharacter)
		display->PlaceObject2(rootObject, &info,XNULL,false);//!seeking);


}

void ScriptThread::PlaceObject3()
{
	FLASHASSERT(rootObject);

	PlaceInfo info;
	info.flags = GetByte();
	info.flags3= GetByte();
	info.depth = GetWord();

	if((info.flags3&extPlaceHasClassName))//||
	   //((info.flags3&extPlaceHasImage)&&(
	     //info.flags&splaceCharacter)))
	{
		info.className=GetStringP();//(char*)AttachData(pos, tagEnd);
		//pos+=XString8::SafeStrlen(info.className)+1;
		//strClass.ReadFromStream(pStream);
	}
	else info.className.nType=XODT_STRING;

	if(info.flags3&extPlaceHasImage)
		info.bImage = true;
	else
		info.bImage = false;

	if ( info.flags & splaceCharacter ) {
		info.character = player->FindCharacter(GetWord());

		#ifdef FSSERVER
		if ( !info.character )
			info.character = lostCh;
		#endif
		if ( !info.character )
			return;


		//if(info.character->tag==116)
		//	int v=0;
	} else
		info.character = 0;




	if ( info.flags & splaceMatrix )
		GetMatrix(&info.mat);
	else
		MatrixIdentity(&info.mat);

	if ( info.flags & splaceColorTransform )
		GetColorTransform(&info.cxform, true);
	else
		info.cxform.Clear();

	if ( info.flags & splaceRatio )
		info.ratio = GetWord();
	else
		info.ratio = 0;

	if ( info.flags & splaceName )
		info.name = GetStringP();//(char*)AttachData(pos, tagEnd);//script+pos;
	else
		info.name = _strEmpty;


	if ( info.flags & splaceDefineClip )
		info.clipDepth = GetWord();
	else
		info.clipDepth = 0;

	//filter list
	//blend Mode
	//cache
	//SObject*pObj=XNULL;
	if(info.flags3&extPlaceHasFilterList)
	{
	   XU8 num=GetByte();
	   XFilter**root=&info.pFilter;
	   for(int i=0;i<num;i++)
	   {
		   XFilter*pFilter=XFilter::CreateFilter(this,splayer);
		   if(!pFilter) break;
		   //pFilter->pNext=XNULL;
		   *root=pFilter;
		   pFilter->m_pNext=NULL;
		   root=&pFilter->m_pNext;
		   if(info.pFilter->m_nFilterID==XFilterConvolution||info.pFilter->m_nFilterID==XFilterColorMatrix)
		   {
				if(pFilter->m_nFilterID==XFilterDropShadow||pFilter->m_nFilterID==XFilterBlur||
					pFilter->m_nFilterID==XFilterGlow||pFilter->m_nFilterID==XFilterBevel||
					pFilter->m_nFilterID==XFilterGradientGlow||pFilter->m_nFilterID==XFilterGradientBevel)
				{
					info.pFilter=pFilter;
				}
		   }
	   }
	}
	if(info.flags3&extPlaceHasBlendMode)
	{
		info.blendMode=GetByte();

	}
	if(info.flags3&extPlaceHasHasCache)
	{
		info.cacheMode=GetByte();
	}
	if((info.flags&extPlaceClipActions))
	{
		//FLASHASSERT(info.character&&info.character->tag!=175);
		PlaceAction(&info);
	}

	//if ( info.flags & splaceMove )
	//	display->MoveObject(rootObject, &info);
	//else
	//	display->PlaceObject(rootObject, &info,XNULL,!seeking);
	info.bAdd=this->m_bInDisplay;
		//rootObject?rootObject->IsInDisplay():true;//this->m_bInDisplay;

	if ( info.flags & splaceMove )
	{
		//if ( info.flags & splaceCharacter)
		//	RemoveDepth(info.depth);
		//else
			display->MoveObject(rootObject, &info);
	}
	else if ( info.flags & splaceCharacter)
	{
		if(info.flags3&extPlaceHasImage)
			display->PlaceObject3(rootObject, &info,player->m_pCodeContext);//!seeking);
		else
			display->PlaceObject2(rootObject, &info,XNULL,false);//!seeking);
	}
	else if( info.flags3&extPlaceHasClassName)
			display->PlaceObject3(rootObject, &info,player->m_pCodeContext);//!seeking);

	//if(pObj)


}



SObject* ScriptThread::AttachClip(XSWFCONTEXT&cnt,SCharacter *p, const XXVar&name, int depth, XXObject *pInit)
{
	//return XNULL;
	FLASHASSERT(rootObject);
#ifdef _DEBUG
	_nCloneSprites++;
#endif
	PlaceInfo info;
	info.flags = splaceCharacter;//|splaceCloneExternalSprite;
	info.depth = depth;
	info.character=p;

	MatrixIdentity(&info.mat);

	info.cxform.Clear();
	info.bClone=XTRUE;

	info.ratio = 0;

	info.name = name;
	if(!name.IsNull())
		info.flags|=splaceName;

	info.clipDepth = 0;
	info.pFilter=XNULL;

	info.blendMode=0;//GetByte();
	info.cacheMode=0;//GetByte();

	if (info.character->type == spriteExternalChar)
    {
		info.flags |= splaceCloneExternalSprite;
    }

	display->RemoveObject(rootObject,depth);
	info.bAdd=true;
	SObject*pObj=display->PlaceObject2(rootObject, &info,&cnt, true);//XTRUE);

	if(pObj&&pObj->pObject&&pInit)
	{
		XXObject*pClip=pObj->pObject;
		int id=0;
		XXVar var;
		for(;;)
		{
			XXVar name;
			if(!pInit->MemberOf(id,var,name)) break;
			//if(!strName) break;
				pClip->SetMember(cnt,name,var,XNULL);//,XFALSE,XFALSE);
			id++;
		}
		//p->m_pDisp->InitControl();
	}
	//if(pObj)
	return pObj;
}


XBOOL ScriptThread::InitDrawObj()
{
	return XTRUE;
	//return XFALSE;
/*	if(m_pDrawObj) return XTRUE;
	//m_pDrawObj=new XDrawObj
	FLASHASSERT(rootObject);
    FLASHASSERT(this == thread->rootObject->parent->thread);
    SObject*pObj=XNULL;
    PlaceInfo info;

	info.flags = splaceCharacter;
	info.depth = rootObject->GetMaxDepth()+1;
	SCharacter*ch=new SCharacter;
	if(!ch)
		goto exit_gracefully;

    ch->type=drawChar;
	ch->data=XNULL;
	ch->player=player;
	ch->cmds.head=XNULL;
	ch->cmds.tail=XNULL;

	::MatrixIdentity(&info.mat);

    info.cxform.Clear();
	info.name=XNULL;

    // cloned sprites don't have a ratio
    // they always stay on untill their
    // parent gets removed
    info.ratio = (unsigned short)-1;

    info.clipDepth = 0;

	info.character=ch;


//	display->RemoveObject(rootObject, depth);
    m_pDrawObj=display->PlaceObject(rootObject, &info);

exit_gracefully:
    return m_pDrawObj!=XNULL;*/


}

////////////////////////////////////////////////////////////
// CloneSprite
//      Create a new sprite that is a copy of the current one.
//      NOTE: State information (current frame, variables, etc.)
//              is not cloned.
//
// inputs:
//      thread  - the sprite we want to clone
//      name    - the name of the new sprite that we are about to
//                  create. space for this name has been allocated
//                  no need to copy.
//      depth   - what layer we want to place the cloned sprite.
//
//  note: this ptr should be thread->rootObject->parent->thread
//
SObject* ScriptThread::CloneSprite(XSWFCONTEXT&cnt,ScriptThread *thread, const XXVar&name, int depth,XXObject*pInit)
{
	//return XNULL;
#ifdef _DEBUG
	_nCloneSprites++;
#endif
	FLASHASSERT(rootObject);
    FLASHASSERT(this == thread->rootObject->parent->thread);
    SObject*pObj=XNULL;
    PlaceInfo info;
	info.bClone=XTRUE;
	info.flags = splaceCharacter | splaceMatrix;
	info.depth = depth;

	info.pAction=thread->CloneActions();
	info.nEventFlags=thread->nEventFlags;
	if (thread->rootObject->character->type == spriteExternalChar)
    {
        // don't use regular character alloc
        // on this. Use new intead. We treat this
        // ch differently since it's external
		SCharacter *ch = splayer->CreateCharacter();//new SCharacter;
	    if (!ch)
            goto exit_gracefully;

        *ch = *thread->rootObject->character;
        info.character = ch;
    }
    else
        info.character = thread->rootObject->character;

    info.mat = thread->rootObject->xform.mat;

    if (thread->rootObject->xform.cxform.HasTransform())
    {
        info.flags |= splaceColorTransform;
        info.cxform = thread->rootObject->xform.cxform;
    }
    else
		info.cxform.Clear();

    // cloned sprites don't have a ratio
    // they always stay on untill their
    // parent gets removed
    info.ratio = (unsigned short)-1;

    info.name = name;
    if (!name.IsNull())
        info.flags |= splaceName;

    if (thread->rootObject->clipDepth > 0)
    {
        info.flags |= splaceDefineClip;
		info.clipDepth = thread->rootObject->clipDepth;
    }
    else
		info.clipDepth = 0;

    if (info.character->type == spriteExternalChar)
    {
        // We never set the character data ptr when
        // we initially loaded the sprite. But now
        // we must during the cloning, because the
        // cloned movie must know where the script
        // is and it gets it from the character
        // data ptr.
        //
        // note: we could not set the data ptr when we
        //          originally created the loaded sprite
        //          because the data was read in after the
        //          sprite structures where created
        info.character->data = thread->script;

		info.flags |= splaceCloneExternalSprite;
    }

	display->RemoveObject(rootObject, depth);
	info.bAdd=true;
    pObj=display->PlaceObject2(rootObject, &info, &cnt,true);//XTRUE);


	if(pObj&&pObj->pObject&&pInit)
	{
		XXObject*pClip=pObj->pObject;
		int id=0;
		XXVar var;
		for(;;)
		{
			XXVar name;
			if(!pInit->MemberOf(id,var,name)) break;
			pClip->SetMember(cnt,name,var,XNULL);//,XFALSE,XFALSE);
			id++;
		}
		//p->m_pDisp->InitControl();
	}

exit_gracefully:
    return pObj;
}
///////////////////////////////////////////////////////////////
//
//  note: this ptr should be thread->rootObject->parent->thread
void ScriptThread::RemoveSprite(ScriptThread *thread, BOOL createPlaceholder)
{
#ifdef _DEBUG
	_nCloneSprites--;
#endif
    PlaceInfo   info;
	SCharacter  *ch = 0;
    SObject     *oldObject = 0;

    FLASHASSERT(this == thread->rootObject->parent->thread);
    FLASHASSERT(thread->rootObject);

    if (!createPlaceholder)
    {

//		if(thread->rootObject->name&&
//			strcmp(thread->rootObject->name,"ying")==0)
//			int v=0;
	    display->RemoveObject(rootObject, thread->rootObject->depth);
        goto exit_gracefully;
    }

    // clear out the old sprite and put an empty placeholder
    // in its place which has the same name, depth and position
	ch = splayer->CreateCharacter();
    oldObject = thread->rootObject;
	if (!ch)
        goto exit_gracefully;
	ch->record.Init();
	ch->className=XNULL;
	ch->gridRect=XNULL;
	ch->classEntry=XNULL;
	ch->next = 0;
	ch->splayer = player;
	ch->tag = ctagSpritePlaceholder;
	ch->tagCode = 0;
	ch->type = spriteChar;
	ch->data = script;
    ch->sprite.length = 0;
    ch->sprite.numFrames = 0;
    ch->rbounds = oldObject->character->rbounds;

    info.flags = splaceCharacter | splaceMatrix;
	info.depth = oldObject->depth;
    info.mat = oldObject->xform.mat;
    info.puppet = oldObject->puppet;
	info.character = ch;

    if (oldObject->xform.cxform.HasTransform())
    {
        info.flags |= splaceColorTransform;
        info.cxform = thread->rootObject->xform.cxform;
    }
    else
		info.cxform.Clear();

    // make sure we olways replace the empty object during end hold
    info.ratio = thread->rootObject->ratio;

    info.clipDepth = 0;
    info.name = oldObject->name;//CreateStr(oldObject->name);
    if (!info.name.IsNull())
        info.flags |= splaceName;

    // remove old object
	display->RemoveObject(rootObject, thread->rootObject->depth);

    // place the empty placeholder
    display->PlaceObject2(rootObject, &info,XNULL, true);//XTRUE);

	info.name.Release();
//	FreeStr(info.name);
exit_gracefully:
    return;
}


void ScriptThread::RemoveDepth(int depth)
{
	display->RemoveObject(rootObject,depth);
}

void ScriptThread::RemoveObject()
{
	if ( tagCode == stagRemoveObject ) // the old style remove has the unneed tag
		SkipBytes(2);//GetWord();		//U16 tag =
	U16 depth = GetWord();


	display->RemoveObject(rootObject, depth);
	//FLASHOUTPUT("Remove tag=%i depth=%i\n", tag, depth);
}

void ScriptThread::DefineBinary()
{
	U16 tag = GetWord();

	U32 rev=  GetDWord();
	//if(tagCode==stagDefineShape4)
	//	return;
//		int v=0;

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = binaryDataChar;
	ch->tagCode = (U8)tagCode;
	ch->data = script+pos;
	ch->length=tagEnd-pos;
#ifdef _WINEMU
	char c=script[pos];
	if(ch->length>4096&&(c=='F'||c=='C'))
	{
		XString8 m_strFile=splayer->m_strWorkPath.strTxt;
		m_strFile+="NO_AD\\";
		XFile::CreateFolder(m_strFile);
		int le=0;
		const char*appName=player->GetAppName(le);
		if(appName)
		{
			XString8 tmp(appName,le);
			m_strFile+=tmp;
			tmp.FromInt(ch->tag);
			m_strFile+=tmp;
			m_strFile+="_b.swf";
			//m_strFile+=".swf";
			XFile m_file;
			m_file.Open(m_strFile,XFile::XCREATE|XFile::XWRITE|XFile::XBINARY);
			m_file.Write(ch->data,ch->length);
			m_file.Close();
		}
	}
	/*else if(ch->length>256000)
	{
		XU8 S[256],*data=new XU8[ch->length];
		memcpy(data,ch->data,ch->length);
        int n = ch->length - 32;
        int i=0;
        while (i < 256)
        {
			S[i]=i;
            //S.writeByte(i);
            i = (i + 1);
        }
        int j=0,u,v;
        i=0;
        while (i < 256)
        {

            j = j + S[i] + data[n + (i & 31)] & 255;
            u = S[i];
            S[i] = S[j];
            S[j] = u;
            i = (i + 1);
        }
		if (n > 131072)
		{
			n=131072;
		}
        j= 0;
        i = 0;
        int k = 0;
        while (k < n)
        {

            i = (i + 1) & 255;
            u = S[i];
            j = j + u & 255;
            v = S[j];
            S[i] = v;
            S[j] = u;
            data[k] = data[k] ^ S[u + v & 255];
            k = (k + 1);
        }
		XZLib lib;
		XStream in,out;
		in.AttachFrom(data,ch->length);
		int len=ch->length*4;
		XU8* outData=new XU8[len];
		out.AttachFrom(outData,len,1);
		XString8 m_strFile=splayer->m_strWorkPath.strTxt;
		m_strFile+="UN\\";
		XFile::CreateFolder(m_strFile);
		int le=0;
		const char*appName=player->GetAppName(le);\
		if(appName)
		{
			XString8 strFile=m_strFile;
			strFile+=XString8(appName,le);
			strFile+=".dat";
			XFile file;
			if(file.Open(strFile,XFile::XCREATE|XFile::XBINARY|XFile::XWRITE))
			{
				file.Write(data,ch->length);
				file.Close();
			}
		}
		lib.Init(&in,&out);
		lib.Decode(XNULL);
		XString8 strFile=m_strFile;
		strFile+=XString8(appName,le);
		strFile+=".swf";
		XFile file;
		if(file.Open(strFile,XFile::XCREATE|XFile::XBINARY|XFile::XWRITE))
		{
			file.Write(outData,out.GetDataSize());
			file.Close();
		}
		delete data;
		delete outData;
	}*/
#endif
		//AttachData(parser.pos, parser.tagEnd);//script+pos;
}

void ScriptThread::DefineShape()
{
	U16 tag = GetWord();

	//if(tagCode==stagDefineShape4)
	//	return;
//		int v=0;

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = shapeChar;
	ch->tagCode = (U8)tagCode;


	ch->shape.flags=0;
	RectSetEmpty(&ch->shape.edgeRect);
	// Get the bounds
	GetRect(&ch->rbounds);
	if(tagCode==stagDefineShape4)
	{
		GetRect(&ch->shape.edgeRect);
		ch->shape.flags=GetByte();
		//GetRect(ch->shape.flags);
		//SRECT rect;
		//GetRect(&rect);
		//SkipBytes(1);
	}
	// Simply save a pointer to the original data
	ch->data = AttachData(pos, tagEnd);//script+pos;

	m_nShapes++;
	//SCharacterParser parser(player, ch->data, 0, &ch->record);
	//if((ch->shape.flags)&0x04)
	//	parser.useWinding=XTRUE;

	////if(character->tag==249)
	////	int v=0;
	//parser.getAlpha = 
	//	ch->tagCode >= stagDefineShape3;
	//parser.lineStyle2 =
	//	ch->tagCode >= stagDefineShape4;
	////parser.cxform = x->cxform;
	////parser.obj = pClip?pClip:this;
	////parser.info = info;
	//parser.BuildEdges(XTRUE);
}

void ScriptThread::DefineMorphShape2()
{
	U16 tag = GetWord();

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = morphShapeChar;
	ch->tagCode = (U8)tagCode;

	// Get the bounds
	GetRect(&ch->rbounds);			// start bounds
	GetRect(&ch->morph.bounds2);	// end bounds

	SRECT rect;
	GetRect(&rect);
	GetRect(&rect);
	SkipBytes(1);

	// Interpolate(&rectBeg, &rectEnd, ratio, &ch->bounds);

	// Simply save a pointer to the original data
	ch->data = AttachData(pos, tagEnd);  //script+pos;
}

void ScriptThread::DefineMorphShape()
{
	U16 tag = GetWord();

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = morphShapeChar;
	ch->tagCode = (U8)tagCode;

	// Get the bounds
	GetRect(&ch->rbounds);			// start bounds
	GetRect(&ch->morph.bounds2);	// end bounds

	// Interpolate(&rectBeg, &rectEnd, ratio, &ch->bounds);

	// Simply save a pointer to the original data
	ch->data = AttachData(pos, tagEnd);  //script+pos;
}

void ScriptThread::DefineButton()
{
	U16 tag = GetWord();

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = buttonChar;
	ch->tagCode = (U8)tagCode;

	// Simply save a pointer to the original data
	ch->button.soundData = 0;
	ch->button.cxformData = 0;

	if ( tagCode == stagDefineButton2 ) {
		ch->button.trackAsMenu = GetByte();
	} else {
		ch->button.trackAsMenu = false;
	}

	ch->data = AttachData(pos, tagEnd,&ch->length);//script+pos;
}

void ScriptThread::DefineButtonExtra(int i)
// Attach a sound definition to a button
{
	U16 tag = GetWord();

	SCharacter* ch = player->FindCharacter(tag);
	if ( !ch || ch->type != buttonChar ) return;

	// Save a pointer to the info
	//XS32 l;
	if ( i == 0 ) {
		if ( !ch->button.soundData )
			ch->button.soundData = AttachData(pos, tagEnd);
	} else {
		if ( !ch->button.cxformData )
			ch->button.cxformData = AttachData(pos, tagEnd);
	}
}

void ScriptThread::DefineSprite()
// Attach a sound definition to a button
{
	U16 tag = GetWord();

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = spriteChar;

	// Simply save a pointer to the original data
	ch->sprite.numFrames = GetWord();
	ch->sprite.length = tagEnd - pos;
	ch->data = AttachData(pos, tagEnd);

	//Do Init Codes
	/*ScriptThread thread;// = new ScriptThread();
	{
		thread.nEventFlags=0;
		thread.pActions=XNULL;
		thread.ClearState();
		thread.player = ch->player;
		thread.splayer=splayer;
		thread.layerDepth = 0;//obj.depth;
		thread.rootObject = XNULL;//obj;
		//thread.SetDisplay(XNULL);//obj.display);
		thread.Attach(ch->data, 0);
		thread.len = ch->sprite.length;
		thread.startPos = 0;
		thread.numFrames = ch->sprite.numFrames;
		thread.DoInitTag();
		//thread.DrawFrame(0, XFALSE);
	}*/

}

#ifdef USE_VIDEO
void ScriptThread::DefineVideo()
// Attach a sound definition to a button
{
	U16 tag = GetWord();

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = videoChar;

	ch->video.numFrames=GetWord();
	ch->video.width=GetWord();
	ch->video.height=GetWord();
	ch->video.flag=GetByte();
	ch->video.codecID=GetWord();
	ch->video.curFrame=0;
	ch->data=0;
	// Simply save a pointer to the original data
	//ch->data = AttachData(pos, tagEnd);
}
void ScriptThread::DefineVideoFrame()
{
	U16 tag=GetWord();
	SCharacter*ch=player->FindCharacter(tag);
	if(!ch) return;
	ch->video.curFrame=GetWord();
	ch->data = AttachData(pos, tagEnd);

}
#endif


void ScriptThread::DefineJPEGTables(XU8*pData,int nLen)//SParser&parser)
// Createa a JPEG decompressor object and set up the shared tables
{
 // #ifdef USE_JPEG
	/*if ( !player->jpegInfo ) {
		player->jpegInfo = new JPEGPlayState;
		if ( !player->jpegInfo ) return;
		ReadJPEGTables(player->jpegInfo, this);
	}*/
  //#endif

	XINT s = nLen;//parser.tagEnd-parser.pos;
	//XU32 s=pStream->GetDataSize();
	if(s<=0||s>0xffffff) return;
	player->jpegTable.SetSize(s,XFALSE);
	//data.SetSize(s,XFALSE);
	if((XINT)player->jpegTable.GetSize()<s) return;
	XGlobal::Memcpy(player->jpegTable.GetData(),pData,s);
	//pCnt->pJpegTab=&data;
	//return pStream->ReadData(pCnt->jpegTab,s);
}

void ScriptThread::BuildBits(SCharacter* ch)
{
	if ( !ch->bits.HasBits() ) {
		// Decompress the bits of the bitmap
		//ch->bits.PIFree(true);

		SParser parser;
		parser.Attach(ch->data, 0);
		int code = parser.GetTag();
		parser.GetWord();	// skip character tag

		switch ( code ) {
  //#ifdef USE_JPEG
			case stagDefineBits: {
				//XStream stream;
				//stream.AttachFrom(parser.script+parser.pos,parser.tagEnd-parser.pos);
				ch->bits.PICreate(//&stream,
								  parser.script+parser.pos,parser.tagEnd-parser.pos,
								  player->jpegTable.GetSize()?&player->jpegTable:XNULL);
				// Read JPEG info
				/*if ( player->jpegInfo ) {
					player->jpegInfo->src.player = &parser;
					ReadJPEGImage(player->jpegInfo, ch, false);
				}*/
			} break;

			case stagDefineBitsJPEG2:
				{
					//XStream stream;
					//stream.AttachFrom(parser.script+parser.pos,parser.tagEnd-parser.pos);
					ch->bits.PICreate(parser.script+parser.pos,parser.tagEnd-parser.pos,XNULL);
					//ch->bits.CreateTexture();
					//ch->bits.ReleaseData();
				}break;
			case stagDefineBitsJPEG3: {
				// Read a JPEG using custom tables
					XStream stream;
					stream.AttachFrom(parser.script+parser.pos,parser.tagEnd-parser.pos);
					ch->bits.PICreate(&stream);
					//ch->bits.CreateTexture();
					//ch->bits.ReleaseData();
			} break;
//#endif

			case stagDefineBitsLossless:
			case stagDefineBitsLossless2: {
				// Decompress lossless data
				//int err;
				//z_stream d_stream; /* decompression stream */

				//d_stream.zalloc = (alloc_func)0;
				//d_stream.zfree = (free_func)0;
				//d_stream.opaque = (voidpf)0;

				{
					//if(ch->tag==141)
					//	int v=0;
					int format = parser.GetByte();
					int width = parser.GetWord();
					int height = parser.GetWord();
					BOOL readAlpha = code == stagDefineBitsLossless2;

					// Get the color table size
					int nColors = format <= bm8Bit ? parser.GetByte()+1 : 0;

					//XSWFPlayer::m_pInstance->Trace(
					{
						XStream stream;//in,out;
						//err = inflateInit(&d_stream);
						//if ( err != 0 ) goto AbortL;
						stream.AttachFrom(parser.script + parser.pos,parser.tagEnd-parser.pos);
						ch->bits.PICreate(format,nColors,width,height,stream,readAlpha);
						//ch->bits.CreateTexture();
						//ch->bits.ReleaseData();
						//stream.WriteData(parser.script + parser.pos,parser.tagEnd-parser.pos);
						//XZLib lib;
						//lib.Init(&in,&out);
						//d_stream.next_in = (Bytef*)parser.script + parser.pos;
						//d_stream.avail_in = parser.tagEnd-parser.pos;


						// Get the color table
						/*SColorTable ctab;
						if ( format <= bm8Bit ) {
							U8 buf[4];
							buf[3] = 255;	// use an opaque alpha when none is stored
							RGB8* c = ctab.colors;
							ctab.n = nColors;
							for ( int i = nColors; i--; c++ ) {
								d_stream.next_out = buf;
								d_stream.avail_out = readAlpha ? 4 : 3;
								while ( d_stream.avail_out > 0 ) {
									err = inflate(&d_stream, Z_NO_FLUSH);
									FLASHASSERT(err == 0);
								}

								c->red   = buf[0];
								c->green = buf[1];
								c->blue  = buf[2];
								c->alpha = buf[3];
							}
						}

						// Set up the bitmap
						ch->bits.PICreate(format, width, height, format <= bm8Bit ? &ctab : 0, true);
						ch->bits.transparent = readAlpha;

						if ( !ch->bits.HasBits() )
							goto AbortL;

						ch->bits.LockBits();
						S32 size = height*ch->bits.rowBytes;
						d_stream.next_out  = (Bytef*)ch->bits.baseAddr;
						d_stream.avail_out = size;
						while ( true ) {
							err = inflate(&d_stream, Z_NO_FLUSH);
							if ( err != Z_OK ) break;
						}
						FLASHASSERT(err==Z_STREAM_END);
						SBitmapSwapBits(ch->bits.baseAddr, size, ch->bits.bmFormat);
						ch->bits.UnlockBits();*/
					}
// 					#endif
				}

			//AbortL:
				//err = inflateEnd(&d_stream);
				//FLASHASSERT(err==0);
			} break;
		}
#ifdef _WINEMU_BMTRACE
		if(ch->bits.pAlloc)
		{
			XString16 s("[Memory] Alloc image memroy ");
			XString16 t;
			t.FromInt(ch->bits.rowBytes*ch->bits.height);
			s+=t;
			s+=XString16(" Bytes");
			splayer->Trace(s);
		}
#endif
	}
	//ch->bits.IncRef();
}

static inline void LockBits(_GLColor*pColor,XU32 nTimer)
{
	
	//if(pColor->colorType!=glColorBitmap) return;
	switch(pColor->colorType)
	{
	default:return;
	case glFrameBuffer:
		 //RasterGL::LockDisplay(pColor->frame.frame->mTexture);
		 RasterGL::LockTexture(pColor->frame.frame->mTexture);
		 return;
	case glColorBitmap:
		 break;
	}
	//if(pColor->bm.bitmap->width==1280)
	//	int v=0;
	
	//pColor->bm.bitmap->m_nLockTime = nTimer;
	SBitmapCore*bm=pColor->bm.bitmap;
	if(!bm->bitmapID&&bm->m_pCH)
	{
		SCharacter*ch=(SCharacter*)bm->m_pCH;
		XSWFPlayer::m_pInstance->CreateIfEmpty(ch);
		/*if(ch->splayer)
		{
			ch->splayer->BuildBits(ch);
			if(bm->HasBits())
			{
				bm->CreateTexture();
				bm->ReleaseData();
			}
		}*/
	}
	pColor->bm.bitmap->LockBits();
	//RasterGL::LockDisplay(pColor->bm.bitmap->bitmapID);
}

static inline void UnLockBits(_GLColor*pColor,XU32 nTimer)
{
	//if(pColor->colorType!=glColorBitmap) return;
	switch(pColor->colorType)
	{
	default:return;
	case glFrameBuffer:
		 //RasterGL::UnLockDisplay(pColor->frame.frame->mTexture);
		 RasterGL::UnLockTexture(pColor->frame.frame->mTexture);
		 return;
	case glColorBitmap:
		 break;
	}
	pColor->bm.bitmap->UnlockBits();
	//RasterGL::UnLockDisplay(pColor->bm.bitmap->bitmapID);
//	pColor->bm.bitmap->m_nLockTime = nTimer;
}

void ScriptThread::PlaceCharacter(_XSCharacter*ch)
{
	if(ch->placeCount<=0)
	{
		BwShapeRecord*pRec=&ch->record;
		BwFillStyle** fills=pRec->mVecFillStyles;
		BwLineStyle** lines=pRec->mVecLineStyles;
		int i;
		_GLColor*pColor;
		XU32 nTimer=XSWFPlayer::m_pInstance->m_nRunTime;
		for(i=0;i<pRec->mnFills;i++)
		{
			if(!fills[i]) continue;
			pColor=fills[i];
			LockBits(pColor,nTimer);
		}
		for(i=0;i<pRec->mnLines;i++)
		{
			if(!lines[i]||!lines[i]->color) continue;
			pColor=lines[i]->color;
			LockBits(pColor,nTimer);
		}
	}
	ch->placeCount++;
}

void ScriptThread::PlaceCharacter(_XSObject*obj,bool bFirst)
{
	//if(ch->tag==65528)
	_XSCharacter*ch = obj->character;
	switch(ch->type)
	{
	case bitmapChar:
	case loaderImageChar:
	case bitmapEmptyChar:
		 if(obj->pASObject)
		 {
			 BitmapObject* pBmp =(BitmapObject*)obj->pASObject;
			 if(!pBmp->m_pBitmapData||!pBmp->m_pBitmapData->m_pChar)
				 return;
			 ch = pBmp->m_pBitmapData->m_pChar;
			 //break;
		 }break;
	}
	//if(ch->tag==65528)
	//	int v=0;
	if(bFirst)
	{
		ch->placeCount++;
		return;
	}
	//	int v=0;
	PlaceCharacter(ch);
}

void ScriptThread::RemoveCharacter(_XSCharacter*ch)
{
	XU32 nTimer=XSWFPlayer::m_pInstance->m_nRunTime;
	ch->placeCount--;
	if(ch->record.m_pImage)
	{
		//if(!RasterGL::IsLocked(ch->record.m_pImage->id))//!ch->record.m_pImage->id->nLockCount)
		//	abort();
		RasterGL::UnLockTexture(ch->record.m_pImage->id);
		//ch->record.m_pImage->id->nLockCount--;
		//ch->record.m_pImage->nTimer = nTimer;
		//ch->record.m_pImage->useCount--;
	}
	if(ch->placeCount>0) return;
	BwShapeRecord*pRec=&ch->record;
	BwFillStyle** fills=pRec->mVecFillStyles;
	BwLineStyle** lines=pRec->mVecLineStyles;
	int i;
	_GLColor*pColor;
	
	for(i=0;i<pRec->mnFills;i++)
	{
		if(!fills[i]) continue;
		pColor=fills[i];
		UnLockBits(pColor,nTimer);
	}
	for(i=0;i<pRec->mnLines;i++)
	{
		if(!lines[i]||!lines[i]->color) continue;
		pColor=lines[i]->color;
		UnLockBits(pColor,nTimer);
	}
}

void ScriptThread::RemoveCharacter(_XSObject*obj)
{
	_XSCharacter*ch = obj->character;
	switch(ch->type)
	{
	case bitmapChar:
	case loaderImageChar:
	case bitmapEmptyChar:
		 if(obj->pASObject)
		 {
			 BitmapObject* pBmp =(BitmapObject*)obj->pASObject;
			 if(!pBmp->m_pBitmapData||!pBmp->m_pBitmapData->m_pChar)
				 return;
			 ch = pBmp->m_pBitmapData->m_pChar;
			 //break;
		 }break;
	}
	if(ch->placeCount<=0) return;
	//if(ch->tag==65528)
	//	int v=0;
	RemoveCharacter(ch);
}

void ScriptThread::DefineBits()
// Create a bitmap from JPEG data
{
	U16 tag = GetWord();

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = bitsChar;
	ch->data = AttachData(pos-8, tagEnd);//script+pos-8;
	XDOMFILTER*pFilter=splayer->GetFilter();
	ch->bits.PIInit();
	ch->bits.m_pCH = ch;
}

// #ifdef _CELANIMATOR
// void ScriptThread::DefineBitsPtr()
// // Create a bitmap that references another bitmap that has already been allocated
// // A performance optimization for the editor only
// {
// 	U16 tag = GetWord();
//
// 	SCharacter* ch = player->CreateCharacter(tag);
// 	if ( !ch ) return;
//
// 	// Set up object
// 	ch->type = bitsChar;
// 	ch->data = 0;
//
// 	SBitmapCore* bm = (SBitmapCore*)GetDWord();
// 	ch->bits = *bm;
// }
// #endif





//#if (__CORE_VERSION__>=0x02075000)
void ScriptThread::DefineFontName()
{
	U16 tag = GetWord();
	SCharacter*ch=player->FindCharacter(tag);
	if(!ch) return;
	char*fontName=GetStringP();
	if(fontName==NULL)
		return;
	ch->font.fLen = strlen(fontName);
	ch->font.strFontName=fontName;
	XExports::XKEY*pKey=player->m_fontNames.Add(fontName,tag,XSS_NOCASE);	
}

int ScriptPlayer::FindFont(const char*fontName)
{
	int iKey=m_fontNames.Index((void*)fontName,XSS_NOCASE);//NeedLowCase()?XSS_NOCASE:XSS_NORMAL);
	if(iKey<0) return 0;
	return m_fontNames.ValueOf(iKey);
}

//#endif

//#if (__CORE_VERSION__>=0x02073000)
void ScriptThread::DefineFont4()
{
	U16 tag = GetWord();

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->record.Init();
	ch->type = fontNameChar;
	ch->tagCode = (U8)tagCode;
	ch->font.infoData = 0;
	ch->font.deviceState = textDync;
	ch->font.nGlyphs = 0;
	ch->font.flags = 0;
//#if (__CORE_VERSION__>=0x02075000)
	ch->font.fLen = 0;
	ch->font.strFontName=NULL;
//#endif
// [gsg] nDataOffset now used in Player (edit text)
//#ifdef _CELANIMATOR
	ch->font.nDataOffset = 0;
//#endif

	// Simply save a pointer to the original data
	ch->data = AttachData(pos, tagEnd);//script+pos;


}
//#endif

void ScriptThread::DefineFont()
{
	U16 tag = GetWord();

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = fontChar;
	ch->tagCode = (U8)tagCode;
	ch->font.infoData = 0;
	ch->font.deviceState = textUnknown;
	ch->font.nGlyphs = 0;
	ch->font.flags = 0;
	ch->font.fLen = 0;
	ch->font.strFontName=NULL;
	//ch->font.shapeData=new XU8(
// [gsg] nDataOffset now used in Player (edit text)
//#ifdef _CELANIMATOR
	ch->font.nDataOffset = 0;
//#endif
	
	// Simply save a pointer to the original data
	ch->data = AttachData(pos, tagEnd);//script+pos;
	int off = ch->data[0]+(ch->data[1]<<8);
	ch->font.nGlyphs = off/2;
	ch->font.shapeData = (GLTranglesOrLines**)new XU8[ch->font.nGlyphs*sizeof(GLTranglesOrLines*)];
	memset(ch->font.shapeData,0,ch->font.nGlyphs*sizeof(GLTranglesOrLines*));
	//m_nShapes+=ch->font.nGlyphs;
	/*int i;
	for(i=0;i<ch->font.nGlyphs;i++)
		_XSObject::BuildFontChar(ch,i,NULL,0,0);*/
	//ch->font.shapeColor = XNULL;
}

void ScriptThread::DefineFont2()
{
	U16 tag = GetWord();

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = fontChar;
	ch->tagCode = (U8)tagCode;
	ch->font.infoData = 0;
	ch->font.deviceState = textUnknown;
	ch->font.flags = GetWord();
//#if (__CORE_VERSION__>=0x02075000)
//	ch->font.fontName=NULL;
//#endif
// [gsg] nDataOffset now used in Player (edit text)
//#ifdef _CELANIMATOR
	ch->font.nDataOffset = pos;
//#endif
	ch->font.fLen = GetByte();
	ch->font.strFontName = (char*)script + pos;
	SkipBytes(ch->font.fLen);
	// Skip the font name
	//SkipBytes(GetByte());		// save a bit of code... (JLG)
	//U8 fontNameLen = GetByte();
	//for (U8 i = 0; i < fontNameLen; ++i) GetByte();

	// Get the number of glyphs.
	ch->font.nGlyphs = GetWord();
//#ifdef _CELANIMATOR
	ch->font.nDataOffset = pos - ch->font.nDataOffset;
//#endif

	// Simply save a pointer to the original data
	ch->data = AttachData(pos, tagEnd);//script+pos;
	ch->font.shapeData = (GLTranglesOrLines**)new XU8[ch->font.nGlyphs*sizeof(GLTranglesOrLines*)];
	memset(ch->font.shapeData,0,ch->font.nGlyphs*sizeof(GLTranglesOrLines*));
	//m_nShapes+=ch->font.nGlyphs;
	/*int i;
	for(i=0;i<ch->font.nGlyphs;i++)
		_XSObject::BuildFontChar(ch,i,NULL,0,0);*/
	//ch->font.shapeColor = XNULL;
}


void ScriptThread::DefineFontInfo()
{
	U16 tag = GetWord();

	SCharacter* ch = player->FindCharacter(tag);
	if ( !ch || ch->type != fontChar ) return;

	// Save a pointer to the info
	//XS32 l;
	if ( !ch->font.infoData )
		ch->font.infoData = AttachData(pos, tagEnd);//script+pos;
}

void ScriptThread::DefineText()
{
	U16 tag = GetWord();

//	if(tag==44)
//		int v=0;

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = textChar;
	ch->tagCode = (U8)tagCode;
	ch->text.deviceState = textUnknown;

	// Get the bounds
	GetRect(&ch->rbounds);

	// Simply save a pointer to the original data
	ch->data = AttachData(pos, tagEnd);//script+pos;
}

#ifdef EDITTEXT
void ScriptThread::DefineEditText()
{
	U16 tag = GetWord();

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = editTextChar;
	ch->tagCode = (U8)tagCode;

	// Get the bounds
	GetRect(&ch->rbounds);

	// Simply save a pointer to the original data
	ch->data = AttachData(pos, tagEnd);//script+pos;

	U16 flags = GetWord();
	if (!(flags & seditTextFlagsUseOutlines)) {
		display->hasDeviceEditText = XTRUE;
	}
}
#endif

#ifdef SOUND
void ScriptThread::DefineSound()
{
	U16 tag = GetWord();

	SCharacter* ch = player->CreateCharacter(tag);
	if ( !ch ) return;

	// Set up object
	ch->type = soundChar;

	// Get the bounds
	ch->sound.format = GetByte();
	ch->sound.nSamples = GetDWord();

	switch (ch->sound.CompressFormat())
    {
        case sndCompressNone:
        case sndCompressNoneI:
        case sndCompressADPCM:
            ch->sound.delay = 0;
            break;

        case sndCompressMP3:
            // get the compression delay in samples
            ch->sound.delay = GetWord();
            break;

		// We don't support the compression format
        default:
		    //player->FreeCharacter(tag);
            //goto exit_gracefully;
			ch->sound.delay = 0;
			//ch->sound.format &= ~sndCompressMask;
			//ch->sound.format |=
#ifdef _WINEMU
			XString16 s("Waring:Unkown sound compressed type ");
			XString16 si;
			si.FromInt(ch->sound.CompressFormat());
			s+=si;
			splayer->Trace(s);
#endif
            break;
	}

    // Save a pointer to the s data
	ch->data = AttachData(pos, tagEnd); //script+pos;
	ch->sound.samples = ch->data;	    // this may be problem if we muck with the script pointer someday
    ch->sound.dataLen = tagEnd - pos;

}

XBOOL ScriptThread::StartSound(int nOffset, int nLoops)
{
	if(sndChannel) return XFALSE;
	return XTRUE;
}

void ScriptThread::StartSound(int nCode)
{
	U16 tag = 0;

	SCharacter* ch = XNULL;
	if(nCode==stagStartSound2)
	{
		char* name=GetString();
		ch=player->ImportChar(name);
		if(ch) tag=ch->tag;
	}
	else
	{
		tag=GetWord();
		ch=player->FindCharacter(tag);
	}
	if ( !ch || ch->type != soundChar || player->mute ) return;

// #ifdef SPRITE_XTRA
// 	CSoundChannel* channel = CSoundMix::Mixer()->NewChannel();
// #else
	XSoundChannel* channel = new XSoundChannel();
// #endif
	if ( !channel ) return;
	channel->AddRef();
	channel->sound = &ch->sound;
	channel->tag = (U32)this;

	GetSoundInfo(channel);

// #ifdef SPRITE_XTRA
// 	CSoundMix::Mixer()->AddSound(channel);
// #else
	splayer->theSoundMix.AddSound(channel);
// #endif
	channel->Release();
}

BOOL ScriptStreamProc(XSoundChannel* channel)
// WARNING: this is called at interupt time on the Mac, don't do any memory allocations...
{
	ScriptThread* thread = (ScriptThread*)(channel->refPtr);
	FLASHASSERT(thread->sndChannel == channel);
	return thread->DoSoundFrame();
}

void ScriptThread::StopStream()
{
	if ( sndChannel ) {
		sndChannel->Stop();
		sndChannel->Release();
		sndChannel = 0;
	}
	needBuffer = (player == this);//true;	// don't start the sound until we have a decent amount loaded (for the streaming thread)
}

void ScriptThread::AdjustMp3Streaming()
{
    if (justSeeked)
    {

        justSeeked = false;
        if (sndChannel->blockSamples > 0)
        {
            FLASHASSERT(sndChannel->seekSamples >= 0);
            justSeekedNegative = true;
            AdjustMp3Streaming(); // recurse
        }
        else if (sndChannel->blockSamples == 0 && sndChannel->seekSamples == 0)
        {
            // do nothing here
            return;
        }
        else if (sndChannel->seekSamples < 0)
        {
            justSeekedNegative = true;
            AdjustMp3Streaming(); // recurse
        }
        else
        {
            FLASHASSERT(0);
        }

    }
    else
    {
        if (justSeekedNegative && sndChannel->blockSamples > 0)
        {
            FLASHASSERT(sndChannel->seekSamples >= 0);
            justSeekedNegative = false;
            sndChannel->blockSamples -= sndChannel->seekSamples;


            // throw away the samples that don't belong in this
            // frame. These samples actually belong in the previous
            // frame. If we read ahead one frame we could actually
            // use it (!!@ should we read ahead one frame). But
            // this really should not be noticable by an average
            // human. If you are above average come and talk to me
            // SBL
            sndChannel->decomp->Decompress(0, sndChannel->seekSamples);
        }

        if (sndChannel->seekSamples < 0)
            justSeekedNegative = true;
    }
}

BOOL ScriptThread::DoSoundFrame()
// Process the sound commands for the next frame
// WARNING: this is called at interupt time on the Mac, don't do any memory allocations...
{
#ifndef FSSERVER
	FLASHASSERT(sndChannel);

	if ( sndAtEnd )
		return false;

	// Check to prebuffer 5 seconds of sound
	// The sound prebuffering amount can be customized through the
	// _soundbuftime property.
	if ( needBuffer ) {
		FLASHASSERT(player == this);	// this should only happen on the main thread

		int soundBufferTime = 5;
		if (player->splayer) {
			soundBufferTime = player->splayer->nSoundBufferTime;
		}
		S32 count = player->frameDelay > 0 ? (soundBufferTime*1000)/player->frameDelay : 100;
		if ( !player->FrameComplete(sndFrame+count) ) {
			return false;
		}
		needBuffer = false;
	}

	SParser savedState = *(SParser*)this;
	while ( true ) {
		pos = sndPos;
		int code = GetTag(len);
		if ( code < 0 ) {
			needBuffer = true;			// we ran out so rebuffer the sound
			*(SParser*)this = savedState;
			FLASHASSERT(player == this);	// this should only happen on the main thread
			return player->ScriptComplete();	// return true at the end of the script
		}
		sndPos = tagEnd;	// start at the next pos

		switch ( code ) {
			case stagEnd:
				sndAtEnd = true;
				goto Done;
			case stagShowFrame:
				sndFrame++;
				goto Done;
			case stagDefineSound:
				DefineSound();
				break;
			case stagStartSound2:
			case stagStartSound:
				// Start a sound
// 				#ifdef SPRITE_XTRA
// 				if ( CSoundMix::Mixer()->MixAhead() )
// 				#endif
					StartSound(code);
// 				break;
			//case stagSoundStreamHead:
			//	SoundStreamHead();
				break;
			case stagSoundStreamBlock: {
				//_ANALYENTER("Stream Block");
				// Set up the decompression for the block
                if (snd.CompressFormat() == sndCompressMP3)
                {
				    sndChannel->blockSamples = GetWord();
                    sndChannel->seekSamples = (S16) GetWord();
                    sndChannel->blockCanBeZero = true;
                }
                else
                {
				    sndChannel->blockSamples = snd.nSamples;
                }
				snd.samples = script+pos;
                snd.dataLen = tagEnd - pos;

                if ( sndChannel->decomp )
                {
                    // make sure setup does not skip past delay
                    // with streaming sound we do that manually

                    int delaySav = snd.delay;
                    snd.delay = 0;
					sndChannel->decomp->Setup(&snd, justSeeked);
                    snd.delay = delaySav;

                    // adjust mp3 channel according to the
                    // hind data the outoring has provided
                    if (snd.CompressFormat() == sndCompressMP3)
                        AdjustMp3Streaming();
                }
				else
					sndChannel->pos = 0;
			} break;
		}
	}

Done:
	*(SParser*)this = savedState;
#endif
	return true;	// we got the block
}

void ScriptThread::SoundStreamHead()
// Set up the stream formats
{
// #ifndef FSSERVER
	mixFormat = GetByte();

	// The stream settings
    snd.Init();
	snd.format = GetByte();
	snd.nSamples = GetWord();


	if (snd.CompressFormat() == sndCompressMP3)
    {
        // get the compression delay in samples
        snd.delay = GetWord();
    }
// #endif
}

void ScriptThread::SoundStreamBlock()
{
// #ifndef FSSERVER
	//_ANALYENTER("Stream Block");
	FLASHASSERT(!sndChannel);
	FLASHASSERT(player);
	if ( !playing ) return;
	if ( player->mute ) return;	// the player is muted
	if ( player->splayer && !player->splayer->running ) return; // the splayer is not running
	if ( snd.CompressFormat() > sndCompressNoneI ) return;	// we don't support this compression format

	// Create a new channel
// #ifdef SPRITE_XTRA
// 	sndChannel = CSoundMix::Mixer()->NewChannel();
// #else
	sndChannel = new XSoundChannel();
// #endif
	if ( sndChannel ) {
		sndChannel->AddRef();
		sndChannel->sound = &snd;
		sndChannel->samplesPlayed = curFrame > 0 ? snd.nSamples*curFrame : 0;	// adjust the number of samples for the starting frame number
		sndChannel->refPtr = this;
		sndChannel->streamProc = ScriptStreamProc;

		sndAtEnd = false;
		sndFrame = curFrame;
		sndPos = pos-6;	// the start of the tag

// #ifdef SPRITE_XTRA
// 		CSoundMix::Mixer()->AddSound(sndChannel);
// #else
		splayer->theSoundMix.AddSound(sndChannel);
// #endif
	}
// #endif
}
#endif



int ScriptThread::DoTag()
// Process a single tag
{
	S32 savedPos = pos;

	{
		// DrawFrame checks these...
		FLASHASSERT(script && !scriptErr);

		if ( atEnd )
			return playAtEnd;

		int code = GetTag(len);
		if ( code < 0 )
			return playNeedData;	// we need more data before we can process this tag



		switch ( code ) {
			case stagFileAttrib:
				/*{
					XU8 flag=GetByte();
					if(flag&0x8)
					{
						player->InitAS3();
						player->bAS3=XTRUE;
					}
					player->fileAttribs=flag;
					//{
					//	scriptErr=notSupport;
					//}

				}*/
				break;
			case stagEnd:
				atEnd = true;
				break;
			case stagShowFrame:
#ifdef _WINEMU
				//if(this->rootObject==_pDebugP)
				//	int vv=0;
#endif
				curFrame++;
				break;
			case stagDefineBinaryData:
				DefineBinary();
				break;
			case stagDefineShape:
			case stagDefineShape2:
			case stagDefineShape3:
			case stagDefineShape4:
				//if(code==stagDefineShape4)
					//int v=0;
				DefineShape();
				break;
			case stagDefineMorphShape:
				DefineMorphShape();
				break;
			case stagDefineMorphShape2:
				DefineMorphShape2();
				break;
			case stagFreeCharacter: {
				U16 tag = GetWord();
				player->FreeCharacter(tag);
			} break;
			case stagPlaceObject:
 				PlaceObject();
				break;
			case stagPlaceObject2:
				PlaceObject2();
				break;
			case stagPlaceObject3:
				PlaceObject3();
				break;
			case stagRemoveObject:
			case stagRemoveObject2:
				RemoveObject();
				break;
			case stagDefineBits:
			case stagDefineBitsLossless:
			case stagDefineBitsJPEG2:
			case stagDefineBitsLossless2:
			case stagDefineBitsJPEG3:
				DefineBits();
				break;
			case stagJPEGTables:
				{

					DefineJPEGTables(script+pos,tagEnd-pos);
				}
				break;
			case stagDefineButton:
			case stagDefineButton2:
				DefineButton();
				break;
			case stagDefineSprite:
				DefineSprite();
				break;
			case stagExportAssets:
				ExportAssets();
				break;
			case stagDefineFont:
				DefineFont();
				break;
//#if (__CORE_VERSION__>=0x02075000)
			case stagDefineFontName:
				DefineFontName();
				break;
//#endif
			case stagDefineFontInfo:
				DefineFontInfo();
				break;
			case stagDefineFont4:
//#if (__CORE_VERSION__>=0x02073000)
				DefineFont4();
//#endif
				break;
			case stagDefineFont3:
			case stagDefineFont2:
				DefineFont2();
				break;
			case stagDefineText:
			case stagDefineText2:
				DefineText();
				break;
#ifdef USE_VIDEO
			case stagDefineVideo:
				DefineVideo();
				break;
			case stagVideoFrame:
				DefineVideoFrame();
				break;
#endif
#ifdef EDITTEXT
			case stagDefineEditText:
				DefineEditText();
				break;
#endif
			case stagSetBackgroundColor:
				//nBackColor=GetColor();SRGB
				{
					SRGB c=GetColor();
					rootObject->SetBackColor(c.all);//nBackColor=c.all;
					display->SetBackgroundColor(c);
				}
				break;
			case stagDoAction:
				//if(rootObject->character->tag==2814)//curFrame==4)
				//	int v=0;
				if ( !seeking && *(script+pos) )
					display->PushAction(script+pos, rootObject,XNULL,0);
				break;
			case stagDoInitAction:
				//if( !seeking )
				{
					XU8*pData=script+pos;
					XU16 tagID=GetWord();

					if(!tagID) break;
					//if(tagID==1185)
					//	int v=0;
					SCharacter*ch=player->FindCharacter(tagID);
					*((XU16*)pData)=0;
					pData+=2;
					if(*pData)
						display->PushAction(pData,rootObject,XNULL,XActionList::XA_TOP|(tagID<<16));
					/*SCharacter*ch=player->FindCharacter(tagID);
					if(ch==XNULL) ch=rootObject->character;
					pData+=2;
					if(pData)
					{

					}*/
				}break;
#ifdef SOUND
			case stagDefineSound:
				DefineSound();
				break;
			case stagStartSound:
			case stagStartSound2:
				if ( !sndChannel && !seeking )
					StartSound(code);
				break;

			case stagDefineButtonSound:
				DefineButtonExtra(0);
				break;
			case stagSoundStreamHead:
			case stagSoundStreamHead2:
				SoundStreamHead();
				break;
			case stagSoundStreamBlock:
				if ( !sndChannel && !seeking )
					SoundStreamBlock();
				break;
#endif

			case stagDefineButtonCxform:
				DefineButtonExtra(1);
				break;

			case stagFrameLabel:
				currentLabel = (char*)script+pos;
				labelFrame = curFrame;
				break;
			case stagDefineFontAlignZones:
				break;
			case stagProtect:
				break;
			case stagDefineScalingGrid:
				{
					XU16 id=this->GetWord();

					SCharacter*ch=player->FindCharacter(id);
					if(ch)
					{
						ch->gridRect=(char*)this->script+this->pos;

					}
				}
				break;
			case stagCSMTextSettings:
				break;
			//case stagSymbolClass:
			//	DoSymbolClass(tagEnd-pos);
			//	break;
			default:
				FLASHASSERT(false);
				//FLASHOUTPUT("Unrecognized op code.\n");
				break;
		}
		if(tagEnd<0)
			return scriptErr;
		pos = tagEnd;

		return scriptErr;
	}
}

int ScriptThread::DoTags(int fnum)
{
	int res = playOK;
	while ( curFrame < fnum && res == playOK )
	{
		res = DoTag();
	}
	return res;
}

int ScriptThread::DrawFrame(int fnum, BOOL seek,BOOL bCall)
{
//	if(!rootObject->name.IsNull()&&strcmp(rootObject->name.strTxt,"mcNumRank")==0)
//		int v=0;
	//if(fnum==120)
	//	int v=0;
	//if(rootObject&&rootObject->character&&rootObject->character->tag==142)
	//	int v=0;

	bool bDef=curFrame!=fnum;
	if(!rootObject||rootObject->IsEmptyThread())
		return playNeedData;
	if ( scriptErr )
		return scriptErr;

	if ( !player->gotHeader )
		return playNeedData;

	//InitRoot();
#ifdef EDITTEXT
	SPlayer* splayer = player->splayer;
	if ( splayer )
		splayer->saveFocus.Save(splayer);
#endif

	int res = playOK;
	if ( seek ) {
		if ( curFrame > fnum )
		{
			// Seek backwards

			// Rewind the movie...
			pos = startPos;		// seek to the beginning
			curFrame = -1;
			currentLabel = 0;
			labelFrame = 0;
			atEnd = false;

			// Marks all objects to be removed unless they are added again
		 	//rootObject->FreeChildren();
			display->BeginHold(rootObject);

			// Skip over frames
			seeking = true;
            justSeeked = true;      // tell mp3 sound stream to adjust itself
			res = DoTags(fnum-1);
			seeking = false;

			// Do the destination frame
			res = DoTags(fnum);

			display->FinishHold(player->IsAS3());

		} else {
			// Seek forwards

			// Skip over frames
			seeking = true;
            justSeeked = true;      // tell mp3 sound stream to adjust itself
			res = DoTags(fnum-1);
			seeking = false;

			// Do the destination frame
			res = DoTags(fnum);
		}
		//rootObject->OnEvent(splayer,XOBJEVENT_ENTERFRAME,XNULL);
	} else {
		// Build the frame
		//DoEvent(CLIPENTERFRAME,0);
		res = DoTags(fnum);

	}
	if(res) return res;
	if(rootObject->clipDepth)
		rootObject->Modify();
	rootObject->SetInitChild();
	bool bAdd=this->m_bInDisplay;//rootObject?rootObject->IsInDisplay():true;//this->m_bInDisplay;
	this->InstanceFrame(bAdd,false,rootObject->pASObject,bCall);
#ifdef EDITTEXT
	if ( splayer )
		splayer->saveFocus.Clear();
#endif

	/*if(rootObject&&rootObject->pASObject)
	{
		//if(!seek)//&&bDef&&rootObject&&rootObject->pASObject)
		//{
		//	rootObject->AS3FillMembers(rootObject->pASObject,this->player->m_pAVM2);

			//false;
		//}
		if(bCall)
		{
			m_bAS3Frame0=false;
			rootObject->CallFrame(fnum);
		}
	}*/

	return res;
}

//#if (__CORE_VERSION__>=0x02075000)
bool ScriptThread::setCurrentLabel(int fnum)
{
	bool found = true;
	int frameNum = 0;


	S32 savedPos = pos;
	if(fnum==this->curFrame)
		return true;
	if(fnum>this->curFrame)
	{
		frameNum=curFrame;
	}
	else
	{
		pos = startPos;
	}
	while ( frameNum < fnum ) {
		int code = GetTag(len);
		if ( code == stagShowFrame ) {
			// Advance to the next frame
			frameNum++;

		} else if ( code == stagFrameLabel ) {
			// See if we found the label
			currentLabel=(char*)(script+pos);			
		} else if ( code < 0 ) {
			// We hit the end and did not find the label
			break;
		}

		pos = tagEnd;	// advance to the next tag
	}
	pos = savedPos;

	return found;
}

void ScriptThread::doLabelCommand()
{
	if(!m_pCmds) return;
	/*if(rootObject&&rootObject->name.strTxt)
	{
		if(strstr(rootObject->name.strTxt,"instance69")!=NULL)
			int v=0;
	}*/
	bool bDef=false;
	DisplayList::XCTRLCMD*pCmds=m_pCmds;
	while(pCmds)
	{
		DisplayList::XCTRLCMD*pCmd=pCmds;
		if(pCmd->nFrame>this->numFramesComplete) return;
		pCmds=pCmds->pNext;
		switch(pCmd->nCmd)
		{
		default:
			 break;
		case DisplayList::XCMD_PLAY:			 
			 break;
		case DisplayList::XCMD_STOP:			 
			 break;
		case DisplayList::XCMD_GOTOANDKEEP:
			 setCurrentLabel(pCmd->nFrame);			 
			 break;
		case DisplayList::XCMD_GOTOANDSTOP:
			 setCurrentLabel(pCmd->nFrame);			 
			 break;
		case DisplayList::XCMD_GOTOANDPLAY:
			 setCurrentLabel(pCmd->nFrame);			 			 
			 break;
		}
	}
	return;
}

//#endif

// delta & bForceRebuild are optional parameters added for the
// flash asset xtra. they are never used by the flash player,
// so in flash, delta = 1 and bForceRebuild = false.
bool ScriptThread::doCommand(bool bCallling)
{
	if(!m_pCmds) return true;
	/*if(rootObject&&rootObject->name.strTxt)
	{
		if(strstr(rootObject->name.strTxt,"instance69")!=NULL)
			int v=0;
	}*/
	bool bDef=false;
	while(m_pCmds)
	{
		DisplayList::XCTRLCMD*pCmd=m_pCmds;
		if(pCmd->nFrame>this->numFramesComplete) return false;
		m_pCmds=m_pCmds->pNext;
		switch(pCmd->nCmd)
		{
		default:
			 break;
		case DisplayList::XCMD_PLAY:
			 playing=XTRUE;break;
		case DisplayList::XCMD_STOP:
			 playing=XFALSE;
			 break;
		case DisplayList::XCMD_GOTOANDKEEP:
			 this->Seek(pCmd->nFrame,bCallling,SK_KEEP);
			 //this->SetFrameFlag();
			 // needCallFrame=XTRUE;
			 //this->m_bAS3Frame0=true;
			 if(bCallling)
				 m_bNeedCallFrame=false;
			 break;
		case DisplayList::XCMD_GOTOANDSTOP:
			 bDef=this->curFrame!=pCmd->nFrame;
			 this->Seek(pCmd->nFrame,bCallling,SK_STOP);
			 //this->m_bAS3Frame0=true;
			 if(bDef)
				m_bNeedCallFrame=!bCallling;
			 break;
		case DisplayList::XCMD_GOTOANDPLAY:
			 bDef=this->curFrame!=pCmd->nFrame;
			 this->Seek(pCmd->nFrame,bCallling,SK_PLAY);
			 //this->m_bAS3Frame0=true;
			 if(bDef)
				m_bNeedCallFrame=!bCallling;
			 //playing=XTRUE;
			 break;
		}
		display->freeCmd(pCmd);

	}
	return true;
}

bool CounterCache();

void ScriptThread::DoFrame(int delta, BOOL bForceRebuild)
{
// #ifdef SPRITE_XTRA
// 	// If we're the sprite xtra, we still want to render
// 	// even if paused - so here we just ignore the frameDelta
// 	// and hang on the current frame
// 	if ( !playing )
// 		delta = 0;
// #endif

//	if(rootObject&&rootObject->name&&
//		strcmp(rootObject->name,"loadad")==0)
//		int v=0;


/*	if(this->playing)
	{
		if(curFrame==426)
			int v=0;
		if(rootObject->character&&rootObject->character->type==rootChar)
			int v=0;
	}*/
#ifdef _WINEMU
	//if(rootObject==_pDebugP)//&&rootObject->character&&rootObject->character->tag==19)
	//{
		//if(this->curFrame==1)
	//	int vv=0;
	//}
#endif


	if(rootObject)
	{
		//if(rootObject->character&&
		//	rootObject->character->tag==861)
		//	int v=0;
		if(rootObject->pASObject)
		{
			((EventDispatcherObject*)rootObject->pASObject)->SetEnter(true);
		}
		if(m_bAS3Frame0||m_bNeedCallFrame)
		{
			m_bAS3Frame0=false;
			m_bNeedCallFrame=false;
			rootObject->CallFrame(curFrame);
			this->doCommand(false);
			//needCallFrame=XFALSE;
		}
		/*if(rootObject->pASObject)
		{
			//this->player->m_pAVM2->OnEvent(XAS3_EVENT_enterFrame,false,rootObject->pASObject);
			//if(rootObject->character&&rootObject->character->tag==78&&
			//	this->curFrame+1==this->numFrames)
			//	int v=0;
			rootObject->AS3_OnenterFrame();
		}
		else*/

		if(!rootObject->pASObject)
			rootObject->OnEvent(0,splayer,XOBJEVENT_ENTERFRAME,XNULL);

	}


	//[Note X]
	//if(!doCommand(false)) return;



	if (!playing && !bForceRebuild)
		return;

	if(this->numFrames<=1) return;


	//if(rootObject&&rootObject->character&&rootObject->character&&rootObject->character->tag==8)
	//		int v=0;

	int thisFrame = GetFrame();				// !!sprite_xtra
	int frameNum =  thisFrame + delta;


//	if(rootObject&&rootObject->character&&rootObject->character->tag==115)//&&
	   //frameNum==30)
//	   int v=0;

	// Check to stall if the next frame is not loaded
	if ( this == player && frameNum > player->numFramesComplete ) {
		frameNum = player->numFramesComplete;
	}

	int res;

	if (bForceRebuild)
	{
		// we do not advance in the forceRebuild case either.
#ifdef _WINEMU
		//if(rootObject==_pDebugP)
		//	int vv=0;
#endif
		curFrame = thisFrame + 1;   // hack to get it to rewind, forcing rebuild of frame
 		res = DrawFrame(thisFrame /* + delta */, true,true);
	}
	else
		res = DrawFrame(frameNum, false,true);

	//[Note X]
	//if(!doCommand(false)) return;

	if ( res == ScriptPlayer::playNeedData ) {
// #ifdef SPRITE_XTRA
// 		// If we're the sprite xtra, we need to draw something
// 		// no matter what - so redraw the current frame.
// 		curFrame = thisFrame + 1;			// hack to get it to rewind
// 		res = DrawFrame(thisFrame, true);
// 		if (res == ScriptPlayer::playNeedData)
// 		{
// 			// !!@ - should just re-blit what was last in the buffer
// 			// (only good if not using transparency, though)
// 			return;
// 		}
// #else
		return;
// #endif // SPRITE_XTRA
	}
	else if ( res < ScriptPlayer::noErr )
	{
		// We got an error, let the thread terminate
#ifdef SOUND
		StopStream();
#endif
		playing = false;
		return;
	}
	else if ( res == ScriptPlayer::playAtEnd )
	{
#ifdef SOUND
		StopStream();
#endif
		if ( loop && GetFrame() != 0 )
		{
			// Never loop a single frame...
			res = DrawFrame(0, true,false);
			if(rootObject)
				rootObject->CallFrame(curFrame);
			this->m_bAS3Frame0=false;
			m_bNeedCallFrame=false;
			//needCallFrame=XFALSE;
		}
		else
		{
			//res = DrawFrame(0, true);
			playing = false;
		}
	}

#if FRAMESPERSEC
    // enable this section in order to see the frame rate in the debug window
    {
        static  long startTime = GetTimeMSec();
        static  long frameCount = 0;

        // look at only root thread
        if (rootObject == player->splayer->player.rootObject)
        {
            frameCount += 1;
            // report framerate every nth frame
//             if (!(frameCount & 0xf))
            if ( frameCount == 12 )
            {
                long currTime = GetTimeMSec();
                long fps;

                fps = (frameCount * 1000) / (currTime - startTime + 1);	// avoid divide by 0 for slow system clocks

				#ifdef DEBUG
					FLASHOUTPUT( "fps = %3d\n", fps );
				#else
					printf( "fps = %3d\n", fps );
				#endif

				frameCount = 0;
				startTime  = GetTimeMSec();
            }
        }
    }
#endif
	//m_bNoEnter=false;
}

// Start and stop the main layer
void ScriptThread::Play(BOOL rewind)
{
    // tell mp3 sound stream to adjust itself
//	if(rootObject->character->tag==290)
	//	int v=0;
    justSeeked = true;
//	if(rootObject->character->type==rootChar&&curFrame>=1)
//		int v=0;
	// Rewind if they are at the end
	if ( rewind && !loop && curFrame >= numFrames-1 )
		Seek(0,true,ScriptPlayer::SK_KEEP);

	playing = true;
//		if(rootObject->name&&
//			strcmp(rootObject->name,"ying")==0)
//		   int v=0;
}

void ScriptThread::StopPlay()
{
	playing = false;
#ifdef SOUND
	StopStream();
#endif
}

void ScriptThread::Seek(int f,bool bCall,int nType)
{
	//if(rootObject&&rootObject->character&&rootObject->character->tag==364)
	//	int v=0;
	if ( f < 0 )
		f = 0;
	else if(f>=numFrames)
		f=numFrames-1;

	if(curFrame==f||f>numFramesComplete)
	{
		if(nType==SK_PLAY)
			 Play();
		else if(nType==SK_STOP)
			 StopPlay();
	}

	if(m_bAS3Frame0&&bCall&&rootObject&&rootObject->pASObject)
	{
		m_bAS3Frame0=false;
		rootObject->CallFrame(curFrame);
		m_bNeedCallFrame=false;
		//needCallFrame=XFALSE;
	}

	if(curFrame==f||f>numFramesComplete)
	{
		if(nType==SK_PLAY)
			 Play();
		else if(nType==SK_STOP)
			 StopPlay();
		return;
	}


	//StopPlay();
	DrawFrame(f, true, false);
#ifdef _WINEMU
	//if(rootObject==_pDebugP)
	//		int vv=0;
#endif
	curFrame=f;
	if(nType==SK_PLAY)
			 Play();
	else if(nType==SK_STOP)
			 StopPlay();
	if(bCall&&rootObject&&rootObject->pASObject)
	{
		//if(f==3)
		//	int v=0;
		//rootObject->AS3FillMembers(rootObject->pASObject,this->player->m_pAVM2);
		m_bAS3Frame0=false;
		m_bNeedCallFrame=false;
		rootObject->CallFrame(f);
		//needCallFrame=XFALSE;
	}
}



int ScriptThread::FindLabel(char* label)
{
	BOOL found = false;
	int frameNum = 0;

	S32 savedPos = pos;
	pos = startPos;
	while ( true ) {
		int code = GetTag(len);
		if ( code == stagShowFrame ) {
			// Advance to the next frame
			frameNum++;

		} else if ( code == stagFrameLabel ) {
			// See if we found the label
			if ( StrEqual(label, (char*)(script+pos)) ) {
				found = true;
				break;
			}
		} else if ( code < 0 ) {
			// We hit the end and did not find the label
			break;
		}

		pos = tagEnd;	// advance to the next tag
	}
	pos = savedPos;

	return found ? frameNum : -1;
}

// #ifdef SPRITE_XTRA
// // !!SPRITE_XTRA - added function
// // Get frame label for specified frame
// // Returns TRUE if label actually exists on that frame
//
// BOOL ScriptThread::GetFrameLabel(int frameNum, char* label)
// {
// 	BOOL found = false;
// 	int testFrame = 0;
//
// 	S32 savedPos = pos;
// 	pos = startPos;
//
// 	// assume no label
// 	*label = 0;
//
// 	while ( true ) {
// 		int code = GetTag(len);
// 		if ( code == stagShowFrame ) {
// 			// Advance to the next frame
// 			testFrame++;
//
// 		} else if ( code == stagFrameLabel ) {
// 			// See if we found the label
// 			if (testFrame == frameNum)
// 			{
// 				// found label - copy to caller's buffer
// 				strcpy( label, (char*)(script+pos) );
// 				// null terminate
// 				label[ strlen( (char*)(script+pos) ) ] = 0;
// 				found = true;
// 				break;
// 			}
// 		} else if ( code < 0 ) {
// 			// We hit the end and did not find the label
// 			break;
// 		}
//
// 		// passed the target frame?
// 		if (testFrame > frameNum)
// 			break;
//
// 		pos = tagEnd;	// advance to the next tag
// 	}
// 	pos = savedPos;
//
// 	return found;
// }
// #endif

//
// Script Player
//


ScriptPlayer::ScriptPlayer(XBOOL bRoot,XBOOL bInit) //:
				//characterAlloc(sizeof(SCharacter),32)
				//characterAlloc(sizeof(SCharacter), 32, true, 0x66666666)
{
	//nCacheCount=nStartCounter;
	//nStartCounter+=27;
	//m_nClasses = 0;
	m_nWaitID=1;	
	m_nABCID=0;
	m_bDummy=XFALSE;
	pASSuper=NULL;
	m_lockURL=NULL;
//	m_pLoader=NULL;
	bAttach=false;
	m_bCanceled=false;
	m_bHold=false;
	m_bFlex=false;
	bScript=false;
	m_bInitSet=false;
	nCompressType=XT_NOCOMPRESS;
	m_bRoot=bRoot;
	m_bInitOK=false;
	m_bABCOK=false;
	m_bLoadedOK=false;
//	m_bConstructOK=false;
//	m_pAVM2=XNULL;
	bAS3=XFALSE;
	fileAttribs=0;
	resize=XFALSE;
	player = this;
	m_pInput=m_pOutput=XNULL;
	m_pDecode=XNULL;
	m_pLZMADec=XNULL;
	m_bLZMAInitOK=XFALSE;
	memset(charIndex, 0, sizeof(charIndex));
	//jpegInfo = 0;
	version = 0;
	m_pEvent=XNULL;
	m_pGlobal=XNULL;
	m_pAVM2=XNULL;
	//m_pContext=XNULL;
	m_pLoaderInfo=XNULL;
	m_pCodeContext=XNULL;
	//m_pLoaderInfo=XNULL;
	//m_pLoader=XNULL;
//	bytesTotal=0;
//	bytesLoaded=0;
// 	#ifdef WIN16
// 	scriptHand = 0;
// 	#endif

// 	#ifdef FSSERVER
// 	lostCharacter = false;
// 	lostCharProc = 0;
//
// 	lostCh = CreateCharacter();
// 	lostCh->next = 0;
// 	lostCh->player = this;
// 	lostCh->tag = 0;
// 	lostCh->type = 99;
// 	lostCh->data = 0;
// 	RectSetEmpty(&lostCh->bounds);
// 	#endif

	RectSet(0,0,oneInch, oneInch, &frame);

	splayer = 0;
	nextLayer = 0;
	stream = 0;
    scriptRefCount = 0;

	m_url = _strEmpty;
	m_loadUrl = _strEmpty;

#ifdef SOUND
	mute = false;
#endif

	variableLoader = false;
	variableSprite = 0;
	variableLayer = -1;

	startPos = 0;
	len = 0;
// #ifdef SPRITE_XTRA
// 	naturalFrameDelay = 0;	// natural frame delay associated with movie	!!sprite_xtra
// 	S32 hostFrameDelay = 0;
// #endif
// 	#if defined(GENERATOR) || defined(_CELANIMATOR)
// 	solidGradients = false;
// 	#endif

	ClearScript();
	if(bRoot&&bInit)
	{
		m_pGlobal=new XXObjectCreator(this);
		if(m_pGlobal)
		{
			//m_pGlobal->
			m_pGlobal->RefObject();
		}
	}
	//InitRoot();
}

ScriptPlayer::~ScriptPlayer()
{
#ifdef _WINEMU
	//if(bAS3)
	//	int v=0;
	XString16 s("[Note] One player loader has been destroied!URL:");
	XString8 url;
	url.ConvertFrom(m_url.strTxt,XCHARSET_GB2312);
	s+=XString16(url);
	splayer->Trace(s);
	if(this->m_nABCID)
	{
		XString16 ss(">>>>>[Waring] One player with script has been destroied!");
		splayer->Trace(ss);
	}
	//	int v=0;
#endif
	ClearScript();
//#if (__CORE_VERSION__>=0x02079000)
	if(rootObject)
		rootObject->parent=NULL;
//#endif
	m_bInitOK=false;
	//FreeStr(url);
	variableSprite.Release();
	//FreeStr(variableSprite);
	FREEOBJECT(m_pEvent);
	if(m_bRoot)
	{
		if(m_pGlobal)
		{
			//m_pGlobal=XNULL;
			m_pGlobal->Destroy();
			m_pGlobal=XNULL;
		}

	}
	XXObject::FreeObjs();//
	//characterAlloc.Free
	/*this->m_pAVM2=splayer->m_pAVM2;
	{
		XFastAlloc::XDATA*data=((XFastAlloc::XDATA*)characterAlloc.m_pData);
		while(data)
		{
			int i,nSize=characterAlloc.m_nInitCount,nBlockSize=characterAlloc.m_nBlockSize;
			XFastAlloc::XBLOCK*pBlock=(XFastAlloc::XBLOCK*)(data+1);
			for(i=0;i<characterAlloc.m_nInitCount;i++)
			{
				//if(!pBlock->pNext)
				//{
				SCharacter*obj=(SCharacter*)(pBlock+1);
				if(obj->player&&obj->player!=this)
					int v=0;
				//}
				pBlock=(XFastAlloc::XBLOCK*)(((XU8*)pBlock)+nBlockSize);
			}
			data=data->pNext;
		}
	}*/
	if(bAS3&&m_pAVM2&&!avmshell::m_bDestroying)
	{
		//if(!m_url.IsNull())
		//	m_pAVM2->ReleaseImage(m_url.strTxt);
		XFastAlloc::XDATA*data=((XFastAlloc::XDATA*)display->objectAlloc.m_pData);
		while(data)
		{
			int i,nSize=display->objectAlloc.m_nInitCount,nBlockSize=display->objectAlloc.m_nBlockSize;
			XFastAlloc::XBLOCK*pBlock=(XFastAlloc::XBLOCK*)(data+1);
			for(i=0;i<display->objectAlloc.m_nInitCount;i++)
			{
				//if(!pBlock->pNext)
				//{
					_XSObject*obj=(_XSObject*)(pBlock+1);
					if(obj->nCreateTime&&obj->pASObject&&obj->character->splayer==this)
					{
						//obj->FreeChildren(true);
						obj->FreeContext(true);
						display->FreeObject(obj);
					}
				//}
				pBlock=(XFastAlloc::XBLOCK*)(((XU8*)pBlock)+nBlockSize);
			}
			data=data->pNext;
		}
	}
	m_pAVM2=NULL;
}

void ScriptPlayer::FreeCache()
{
	{// Free the characters
		SCharacter** index = charIndex;
		for ( int i = 0; i < charIndexSize; i++, index++ ) {
			SCharacter* ch = *index;
			while ( ch ) {
				SCharacter* nextChar = ch->next;
				if ( ch->type == bitsChar /*&& ch->bits.lockCount == 0*/ ) {
					// Free the uncompressed bitmap
#ifdef _WINEMU_BMTRACE
					if(ch->bits.pAlloc)
					{
						XString16 s("[Memory] Free image memroy ");
						XString16 t;
						t.FromInt(ch->bits.rowBytes*ch->bits.height);
						s+=t;
						s+=XString16(" Bytes");
						splayer->Trace(s);
					}
#endif
					//if(ch->bits.bitmapID&&!ch->bits.bitmapID->nLockCount)
					if(!RasterGL::IsLocked(ch->bits.bitmapID))
						ch->bits.PIFree();
				}
				ch = nextChar;
			}
		}
	}

	//characterAlloc.FreeEmpties();
}

void ScriptPlayer::FreeAll(bool bAll)
// Free the current script context
{

	/*XAVM2*p=m_pAVM2;
	while(p)
	{
		XAVM2*pNext=p->m_pNext;
		delete p;
		p=pNext;
	}
	m_pAVM2=XNULL;*/
	if(display&&rootObject)
	{

		//rootObject->AS3RemoveChild();
		if(!bAll)
		{
			SObject*pChild=rootObject->bottomChild;
			while(pChild)
			{
				SObject*pAbove=pChild->above;
				display->RemoveObject(rootObject,pChild);
				pChild=pAbove;
			}
			rootObject->bottomChild=XNULL;
			rootObject->Modify();
		}
		else
		{
			SObject*parent=rootObject->parent;
			if(parent)
			{
			//if ( display )
				display->RemoveObject(parent,rootObject);
				parent->Modify();
			}
			else
			{
	 		//display->RemoveList(rootObject->FreeChildren();
				//display->RemoveObject(&display->root,rootObject);
				//rootObject->FreeContext(true);
				//rootObject
				rootObject->FreeChildren(true);
				rootObject->FreeContext(true);
				//actionList.RemoveObject(obj);


				// Remove from list
				if ( display->button==rootObject )
					display->button = 0;
				if(splayer->m_pActiveObject==rootObject)
					splayer->m_pActiveObject=NULL;
				if(splayer->m_pActiveTarget==rootObject)
					splayer->m_pActiveTarget=NULL;
				//else if(obj==button[1])
				//	button[1]=0;
				display->FreeObject(rootObject);
				display->root.Modify();
				//display->RemoveObject(&display->root, layerDepth);	// remove the RootObject
				//rootObject = 0;
			}//*/
			rootObject=0;
		}
		display->RemoveAllThread(this);
		if(m_pAVM2)
			m_pAVM2->RemoveAllObject(this);
	}

	if(bAll)
	{// Free the characters
		SCharacter** index = charIndex;
		for ( int i = 0; i < charIndexSize; i++, index++ ) {
			SCharacter* ch = *index;
			while ( ch ) {
#ifdef _WINEMU
				if(ch->splayer!=this)
				{
					XString16 s("Error: Invalidate Player of Character ID ");
					XString16 t;
					t.FromInt(ch->tag);
					s+=t;
					splayer->Trace(s);
				}
#endif
				SCharacter* nextChar = ch->next;
				splayer->FreeCharacter(ch);
				ch = nextChar;
			}
			*index = 0;
		}
	}

// 	#ifdef FSSERVER
//
// 	#else
	// Reset the script attributes
	pos = startPos;
	curFrame = -1;
	currentLabel = 0;
	labelFrame = 0;
	atEnd = false;
// 	#endif

#ifdef USE_JPEG
	// Free the JPEG decompression info
	if ( jpegInfo ) {
		jpeg_destroy_decompress(&jpegInfo->cinfo);
		delete jpegInfo;//free(jpegInfo);
		jpegInfo = 0;
	}
#endif
}



//
// Manage the Character List
//

SCharacter* ScriptPlayer::FindCharacter(U16 tag)
{
 	SCharacter* ch = charIndex[tag & charIndexMask];
	while ( ch && ch->tag != tag )
		ch = ch->next;

	#ifdef FSSERVER
	if ( !ch && lostCharProc && tag != 65535 ) {
		lostCharacter = true;
		lostCharProc(lostCharData, tag);
	}
	#endif

	return ch;
}

SCharacter* ScriptPlayer::CreateCharacter(U16 tag)
// Create a character, add it to the list and set the tag
{

	{// Look to see if the character is already defined
		SCharacter* ch = charIndex[tag & charIndexMask];
		while ( ch && ch->tag != tag )
			ch = ch->next;
		if ( ch )
			return 0;	// the character is alrady defined
	}

	SCharacter* ch = splayer->CreateCharacter();
	if ( ch ) {
		// Add to list
		SCharacter** link = &charIndex[tag & charIndexMask];
		ch->next = *link;
		*link = ch;
		ch->record.Init();
		// Set up object
		ch->splayer = this;
#ifdef _WINEMU
		ch->bFilters=true;
#endif
		ch->tag = tag;
		ch->tagCode = 0;
		ch->data = 0;
		ch->className=XNULL;
		ch->classEntry=XNULL;
		ch->gridRect=XNULL;
		RectSetEmpty(&ch->rbounds);
	}
	return ch;
}

void ScriptPlayer::FreeCharacter(U16 tag)
{
	SCharacter** chP = &charIndex[tag & charIndexMask];
	for (;;) {
		SCharacter* ch = *chP;
		if ( !ch ) break;
	 	if ( ch->tag == tag ) {
			*chP = ch->next;
			splayer->FreeCharacter(ch);
		}
        else
            chP = &ch->next;
	}
}


#ifdef FSSERVER
U8 * ScriptThread::AttachData(S32 start, S32 end)
{
	S32 len = end-start;
	U8 * data = new U8[len];
	if ( data ) {
		memcpy(data, script+start, len);
	}
	return data;
}

void ScriptThread::ReleaseData(U8 *& data)
{
	delete [] data;
	data = 0;
}

void ScriptPlayer::SetHeader(void* data, int len)
// Parse the header block
{
	script = (U8*)data;

	// get the script len
	if ( script[0] != 'F' || script[1] != 'W' || script[2] != 'S' ) {
		FLASHASSERT(false);
		//scriptErr = badHeaderErr;
		return;
	}
	version = script[3];

	S32 scriptLen = (U32)script[4] | ((U32)script[5]<<8) |
				((U32)script[6]<<16) | ((U32)script[7]<<24);
	//scriptLen -= 8; // note we subtract off the tag and length data

	FLASHASSERT(scriptLen < (3000000));

	pos = 8;

	GetRect(&frame);
	frameRate = (SFIXED)GetWord()<<8;
	frameDelay = FC(1000)/frameRate;
	numFrames = GetWord();


}

void ScriptPlayer::SetObject(void* data, int len)
// Add a character definition
{
	script = (U8*)data;
	pos = 0;

	while ( pos < len ) {
		int code = GetTag();

		switch ( code ) {
			case stagDefineShape:
			case stagDefineShape2:
			case stagDefineShape3:
			case stagDefineShape4:
				DefineShape(code);
				break;
			case stagDefineBits:
			case stagDefineBitsLossless:
			case stagDefineBitsJPEG2:
			case stagDefineBitsLossless2:
			case stagDefineBitsJPEG3:
				DefineBits();
				break;
			case stagJPEGTables:
				DefineJPEGTables(data,len);
				break;
			case stagDefineButton:
			case stagDefineButton2:
				DefineButton();
				break;
			case stagDefineSprite:
				DefineSprite();
				break;
			case stagDefineFont:
				DefineFont();
				break;
			case stagDefineFont3:
			case stagDefineFont2:
				DefineFont2();
				break;
			case stagDefineFontInfo:
				DefineFontInfo();
				break;
			case stagDefineText:
			case stagDefineText2:
				DefineText();
				break;
#ifdef EDITTEXT
			case stagDefineEditText:
				DefineEditText();
				break;
#endif
#ifdef SOUND
			case stagDefineSound:
				DefineSound();
				break;
			case stagDefineButtonSound:
				DefineButtonExtra(0);
				break;
#endif

			case stagDefineButtonCxform:
			case stagDefineButtonCxform2:
				DefineButtonExtra(1);
				break;

			default:
				//FLASHASSERT(false);
				//FLASHOUTPUT("Unrecognized op code.\n");
		}

		pos = tagEnd;
	}
	FLASHASSERT(pos == len);
}

void ScriptPlayer::RecoverLost()
{
	if ( lostCharacter ) {
		lostCharacter = false;

		display->FreeCache();

		// Build a list of all lost objects
		SObject list[20];
		int n = 0;
		for ( SObject* obj = display->objects; obj; obj = obj->next ) {
			if ( obj->character->type == lostChar ) {
				list[n++] = *obj;
				if ( n >= 20 ) break;
			}
		}

		// Remove the lost objects
		for ( int i = 0; i < n; i++ ) {
			display->RemoveObject(list[i].id);
		}

		lostCharacter = false;

		// Place the objects again
		for ( int j = 0; j < n; j++ ) {
			SCharacter* ch = FindCharacter((U16)(list[j].id>>16));
			#ifdef FSSERVER
			if ( !ch )
				ch = lostCh;
			#endif
			display->PlaceObject(ch, list[j].id, &list[j].mat, &list[j].cxform);
		}
	}
}

#else

void ScriptPlayer::SetDataComplete(XSWFCONTEXT&cnt,XBOOL bOK)
{
	ScriptThread *thread = 0;
	if (variableLoader) {
		if (script) {
			script[len] = 0;

			// This stream is for a Load Sprite or a Load Variable action.
			if (!variableSprite.IsNull()) {
				// Loaded to a target path
				SObject*obj = splayer->FindTarget(splayer->display.root.bottomChild->bottomChild,
													variableSprite.strTxt,XNULL);
				thread=obj?obj->thread:XNULL;
				if (!thread) {
					int layerNum = LayerNum(variableSprite.strTxt);
					if (layerNum >= 0) {
						thread = splayer->OpenLayer(layerNum, XTRUE);
					}
				}
			} else {
				// Loaded to a layer number
				ScriptPlayer *p = splayer->OpenLayer(variableLayer, XTRUE);
				thread = p;

				// If the variables were loaded to a non-existent layer,
				// we need to create a root object for the layer.
				if ( !p->rootObject ) {
					// Create a root object on the display for this player
					InitRoot(true);
				}
			}

			// cleanup
			//FreeStr(variableSprite);
			variableSprite.Release();// = 0;

			if (thread&&thread->rootObject) {
				//XSWFCONTEXT cnt;
				//InitContext(cnt);
				DecodeQueryString(cnt,m_pEvent?m_pEvent:thread->rootObject->pObject, (char*)script);
				XXVar arg(bOK);
				//thread->rootObject->OnEvent(splayer,XOBJEVENT_LOAD,&arg);
				if(thread->rootObject->OnEvent(0,splayer,XOBJEVENT_LOAD,&arg))
					splayer->DoActions();
			}
			if(!bAttach)
			{
				if(m_pAVM2)
					m_pAVM2->ReleaseIntern(script,scriptLen);
				delete [] script;
			}
			script = 0;
			len = scriptLen = 0;

		}
		if(m_pEvent)
		{
			//m_pEvent->AddMember(_SYSVAR(Loaded),XXVar(bOK));
			//m_pEvent->m_bOK
			m_pEvent->AddMember(_SYSVAR(loaded),XXVar(bOK));
			XU8 vars[sizeof(XXVar)*2];
			XXVARLIST list(vars,2);
			list.PushBool(bOK);//Add(new XXVar(bOK));
			//m_pEvent->HandlerEvent(splayer,OBJOFTHREAD(thread),XOBJEVENT_LOAD,&list);
			XXObject*pObj=m_pEvent;
			if(m_pEvent->IsObject(XXOBJ_LOADVARS))
			{
				XXObjectLoadVars*ld=(XXObjectLoadVars*)m_pEvent;
				if(ld->m_target.IsObject())
					pObj=ld->m_target.pObject;
			}
			if(pObj->HandlerEvent(splayer,OBJOFTHREAD(thread),XOBJEVENT_LOAD,&list))
				splayer->DoActions();
			//list.FreeAll();
		}
		delete this;
	}
	else
	{
		if(m_pEvent)
		{
			m_pEvent->AddMember(_SYSVAR(loaded),XXVar(XTRUE));
			XU8 vars[sizeof(XXVar)*2];
			XXVARLIST list(vars,2);
			list.PushBool(bOK);//Add(new XXVar(bOK));
			m_pEvent->HandlerEvent(splayer,OBJOFTHREAD(thread),XOBJEVENT_LOAD,&list);
			if(m_pEvent->HandlerEvent(splayer,OBJOFTHREAD(thread),XOBJEVENT_LOAD,&list))
				splayer->DoActions();
			//list.FreeAll();
		}
		//thread=rootObject->thread;
		//if(thread)
		if(rootObject)
		{
			thread=rootObject->thread;
			XXVar arg(bOK);
			//rootObject->OnEvent(0,splayer,XOBJEVENT_LOAD,&arg);
			if(rootObject->OnEvent(0,splayer,XOBJEVENT_LOAD,&arg))
				splayer->DoActions();
		}
		if(m_nShapes)
			XSWFPlayer::m_pInstance->m_newDatas.Add(this);
	}

}

int ScriptPlayer::PushData(U8* data, S32 chunkLen,void*gcEnter,bool bFull)
{
	PushDataBuf(data, chunkLen,gcEnter,bFull,m_bDummy!=0);	
	return PushDataComplete();
}

XBOOL ScriptPlayer::InitDecode()
{
	if(nCompressType==XT_LZMA)
	{ //LZMA Uncompress init here
		if(m_pLZMADec)//Decode)
		{
			((C7ZDecoder*)m_pLZMADec)->End();
			delete (C7ZDecoder*)m_pLZMADec;//Decode;
		}
		m_pLZMADec=new C7ZDecoder();
		m_bLZMAInitOK=XFALSE;
		if(m_pInput) delete m_pInput;
		m_pInput=new XStream;
		//m_pLZMADec=NULL;
		return XTRUE;
	}
	if(m_pDecode) delete m_pDecode;
	m_pDecode=new XZLib;
	if(!m_pDecode) return XFALSE;
	if(m_pInput) delete m_pInput;
	m_pInput=new XStream;
	if(!m_pInput) return XFALSE;
	if(m_pOutput)
	{
		m_pOutput->DataTo();
		delete m_pOutput;
	}
	m_pOutput=new XStream;
	if(!m_pOutput) return XFALSE;
	m_pOutput->AttachFrom(script,scriptLen,XTRUE);
	m_pOutput->WriteData(XNULL,8);
	//m_pOutput->WriteData(XNULL,scriptLen);
	//m_pOutput->AttachFrom(script,scriptLen,XTRUE);

	m_pDecode->Init(m_pInput,m_pOutput);
	//script=m_pOutput->ReadAddr();
	return XTRUE;
}

#define RELEASEDEC() {pDec->End();delete pDec;m_pLZMADec=XNULL;m_bLZMAInitOK=XFALSE;if(m_pInput) delete m_pInput;m_pInput=NULL;}

XBOOL ScriptPlayer::PushDataDecode(U8* data,S32 chunkLen)
{
	if(nCompressType==XT_LZMA)
	{
		//LZMA Uncompress init here
		if(m_pLZMADec==NULL) return XFALSE;
		C7ZDecoder* pDec=(C7ZDecoder*)m_pLZMADec;//Decode;
		if(!m_bLZMAInitOK)
		{			
			if(pDec->Begin0(data)!=SZ_OK)
			{
				RELEASEDEC();
				return XFALSE;
			}
			m_bLZMAInitOK=XTRUE;
			data+=LZMA_PROPS_SIZE+4;//(4+LZMA_HDR_LEN);
			chunkLen-=LZMA_PROPS_SIZE+4;//(4+LZMA_HDR_LEN);
		}
		if(chunkLen>0&&scriptLen>len)
		{
			m_pInput->WriteData(data,chunkLen);
			SizeT nDestLen=scriptLen-len;
			SizeT nSrcLen=m_pInput->GetDataSize();
			SRes res = pDec->Decode(m_pInput->ReadAddr(),&nSrcLen,script+len,&nDestLen);//src+srcPos, &srcProcessed, dest, &destProcessed);
			if(res==SZ_OK)
			{
				len+=nDestLen;
				m_pInput->Skip(nSrcLen);
			}
			if(res!=SZ_OK||len>=scriptLen)
			{
				RELEASEDEC();
			}
			
		}
		return XTRUE;
	}
	if(!m_pDecode||!m_pInput||!m_pOutput) return XFALSE;
	m_pInput->WriteData(data,chunkLen);
	XBOOL bFinished=XFALSE,bOK=XTRUE;
	switch(m_pDecode->Decode(XNULL))
	{
		case XDC_NOTINIT:break;
		case XDC_OK:
		case XDC_CONTINUE:
			 break;
		case XDC_FINISHED:
			 bFinished=XTRUE;
			 break;
		default:
		case XDC_BAD:
			 bFinished=XTRUE;
			 bOK=XFALSE;
			 break;
	}

	//if(script!=m_pOutput->ReadAddr()) bFinished=XTRUE;
	script=m_pOutput->ReadAddr();
	len=m_pOutput->GetDataSize();
//	bytesLoaded+=len;

	if(bFinished)
	{
		delete m_pInput;
		m_pOutput->DataTo();
		delete m_pOutput;
		delete m_pDecode;
		m_pInput=XNULL;
		m_pOutput=XNULL;
		m_pDecode=XNULL;
	}

	return bOK;
}

void ScriptPlayer::PushDataBuf(U8* data, S32 chunkLen, void*gcEnter, bool bFull,bool bDummy)
{
	if (variableLoader)
	{
		if (nCompressType)
		{
			PushDataDecode(data, chunkLen);
		}
		else
		{
			if (chunkLen + len > scriptLen)
			{
				scriptLen = chunkLen + len + VAR_INCR + 1;
				U8 *newBuffer = new U8[scriptLen];
				if (script)
				{
					memcpy(newBuffer, script, len);
					delete[] script;
				}
				script = newBuffer;
			}
			memcpy(script + len, data, chunkLen);
			len += chunkLen;
		}
		return;
	}

	if (scriptErr)
		return;

	// Get the total script length from the header info
	if (scriptLen < 0)
	{
		// Copy the first 8 bytes to our temp buffer
		XU8* dataBegin=data;
		S32 n = Min(8 - headerLen, chunkLen);
		FLASHASSERT(n > 0);
		memcpy(headerBuf + headerLen, data, n);
		data += n;
		chunkLen -= n;
		headerLen += n;
		if (headerLen == 8)
		{
			switch (headerBuf[0])
			{
			case 'Z':
				nCompressType = XT_LZMA;
				bFull = false;
				break;
			case 'C':
				nCompressType = XT_COMPRESSED;
				bFull = false;
				break;
			case 'F':
				nCompressType = XT_NOCOMPRESS;
				break;
			default:
				scriptErr = badHeaderErr;
				return;
			}

			if (headerBuf[1] != 'W' || headerBuf[2] != 'S')
			{
				scriptErr = badHeaderErr;
				return;
			}
			version = headerBuf[3];
			scriptLen = (U32) headerBuf[4] | ((U32) headerBuf[5] << 8) | ((U32) headerBuf[6] << 16) | ((U32) headerBuf[7] << 24);
			if (scriptLen < 8)
			{
				scriptErr = badHeaderErr;
				return;
			}

			if (bFull)
			{
				script = dataBegin;
				bAttach = true;
			}
			else
			{
				script = new U8[scriptLen];
				if (!script)
				{
					scriptErr = noMemErr;
					return;
				}
				memcpy(script, headerBuf, 8);
				script[0]='F';
			}
			if (nCompressType)
			{
				if (!InitDecode())
				{
					scriptErr = noMemErr;
					return;
				}
			}

			len = 8;
			pos = 8;
			if (m_bRoot)
			{
				if (m_pGlobal)
				{
					m_pGlobal->Init(NeedLowCase());

				}
				XSWFCONTEXT cnt;
				splayer->InitContext(cnt);
				ExtractVariables(
						cnt,
						m_pEvent ? m_pEvent : (rootObject ? rootObject->pObject
								: XNULL), m_url.strTxt);
			}
			if (m_bInitOK)
			{
				InitRoot(true);
			}
		}
		else
		{
			return;
		}
	}

	switch (nCompressType)
	{
	case XT_IMAGE:
		((XImage*) m_pInput)->Append(data, chunkLen);
		len += chunkLen;
		if (scriptLen < len)
			scriptLen = len;
		return;
	}

	if (nCompressType)
	{
		PushDataDecode(data, chunkLen);
	}
	else
	{
		if (len + chunkLen > scriptLen)
		{
			FLASHASSERT(false);
			chunkLen = Min(chunkLen, scriptLen - len);
		}
		if (!bFull)
			memcpy(script + len, data, chunkLen);
		len += chunkLen;
	}

	if (!gotHeader && len >= 17 + 2 + 2)
	{
		// Get the Frame - NOTE be sure to adjust the len above if we add any header data
		GetRect(&frame);
		//if(frame.xmax==249)
		//	int v=0;
		frameRate = (SFIXED) GetWord() << 8;
		frameDelay = frameRate ? (FC(1000) / frameRate) : (FC(1000));
		numFrames = GetWord();

		numFramesCompletePos = startPos = m_nAS3SencePos = pos;
		symbolPos = pos;
		curFrame = -1;
		currentLabel = 0;
		labelFrame = 0;
		gotHeader = true;
	}

	// See how many frames have been loaded
	if (gotHeader&&!m_bHold)
	{
		PrevData(gcEnter,bDummy);
	}

	if (m_bInitOK)
		InitRoot(true);

	if (m_pEvent && m_pEvent->IsObject(XXOBJ_LOADVARS))
	{
		XXObjectLoadVars*pe = (XXObjectLoadVars*) m_pEvent;
		pe->SetTotal(scriptLen);
		pe->SetLoaded(len);
	}
}

void ScriptPlayer::HoldInit()
{
	PrevData(NULL,m_bDummy);
	if(DrawFrame(0,false,false)==ScriptPlayer::playOK&&player->bAS3)
	{
		m_bLoadedOK=true;
		//if(player->pASSuper||(player->m_bInitOK&&!player->bAS3))
		{
			void*pSuper=rootObject->AS3CreateInst(player->pASSuper,XTRUE);
			rootObject->AS3ConstructInst(pSuper);
			if(!m_bInitSet)
			{
				AS3_OnInit();

				m_pAVM2->OnEvent(XAS3_EVENT_added,false,player->rootObject->pASObject,player->m_pCodeContext);
				if(display->root.IsParentOf(player->rootObject))
					m_pAVM2->OnEvent(XAS3_EVENT_addedToStage,false,player->rootObject->pASObject,player->m_pCodeContext);
				m_pAVM2->OnEvent(XAS3_EVENT_complete,false,player->rootObject->pASObject,player->m_pCodeContext);
				//this->AS3_OnAddToStage();
			}
			m_bAS3Frame0=false;
			rootObject->CallFrame(0);
			//needCallFrame=XFALSE;
		}
	}
}

void ScriptPlayer::PrevData(void*gcEnter,bool bDummy)
{
		bool bLoading = true;
		bool bWait = false;
		//if ((!player->fileAttribs || bAS3))
		{
			if ((S32) numFramesCompletePos < scriptLen)
			{
				int savePos = pos;
				pos = numFramesCompletePos;
				//bool bContinue=true;
				while (true && !scriptErr)
				{
					int prevPos=pos;
					int code = GetTag(len);
					if (code < 0)
						break;

					switch (code)
					{
						case stagDoAction:
						case stagDoInitAction:
							 bScript=true;
							 break;
						case stagDefineShape:
						case stagDefineShape2:
						case stagDefineShape3:
						case stagDefineShape4:
							DefineShape();
							break;
						case stagDefineMorphShape:
							DefineMorphShape();
							break;
						case stagDefineMorphShape2:
							DefineMorphShape2();
							break;
						case stagFreeCharacter:
							{
								U16 tag = GetWord();
								player->FreeCharacter(tag);
							}
							break;
						case stagDefineBits:
						case stagDefineBitsLossless:
						case stagDefineBitsJPEG2:
						case stagDefineBitsLossless2:
						case stagDefineBitsJPEG3:
							DefineBits();
							break;
						case stagJPEGTables:
							{
								DefineJPEGTables(script + pos, tagEnd - pos);
							}
								break;
						case stagDefineButton:
						case stagDefineButton2:
							DefineButton();
							break;
						case stagDefineSprite:
							DefineSprite();
							break;
						case stagExportAssets:
							ExportAssets();
							break;
						case stagDefineFont:
							DefineFont();
							break;
						case stagDefineFontInfo:
							DefineFontInfo();
							break;
//#if (__CORE_VERSION__>=0x02075000)
						case stagDefineFontName:
							DefineFontName();
							break;
//#endif
						case stagDefineFont4:
//#if (__CORE_VERSION__>=0x02073000)
							DefineFont4();
//#endif
							break;
						case stagDefineFont3:
						case stagDefineFont2:
							DefineFont2();
							break;
						case stagDefineText:
						case stagDefineText2:
							DefineText();
							break;
	#ifdef USE_VIDEO
							case stagDefineVideo:
							DefineVideo();
							break;
	#endif
	#ifdef EDITTEXT
							case stagDefineEditText:
							DefineEditText();
							break;
	#endif
						case stagDefineSound:
#ifdef SOUND
							DefineSound();
#endif
							break;
						case stagDefineButtonSound:
							DefineButtonExtra(0);
							break;
						case stagDefineButtonCxform:
							DefineButtonExtra(1);
							break;
						case stagDefineFontAlignZones:
							break;
						case stagProtect:
							break;
						case stagDefineScalingGrid:
							break;
						case stagShowFrame:
							if (bLoading) {
								numFramesComplete++;
								m_bInitOK = true;
							}
							break;

						case stagDefineBinaryData:
							DefineBinary();
							break;
						case stagDoABC2:
						case stagDoABC:
							if (m_pAVM2) 
							{
								if(m_strWaitClass.strTxt!=NULL&&m_nABCID>=m_nWaitID)
								{
									bWait=true;
									pos=prevPos;
									goto BreakProc;
									//bContinue=false;
								}
								else
								{
									bScript=true;
									m_bInitOK = true;
									if (!DoABC(tagEnd - pos,bDummy)) {
	#ifdef _WINEMU
										splayer->Trace(XString16("Error:Can't execute ABC code!"));
	#endif
										scriptErr = as3ScriptError;
									} else {
										m_bABCOK = true;
										m_nABCID++;
										if(splayer->m_bPreLoad&&
											this!=splayer->player)
										{
											splayer->m_bPreLoad=false;
											splayer->player->PrevData(gcEnter,false);
										}
									}
								}
							}
							break;
						case stagFileAttrib:
						{
							XU8 flag = GetByte();
							InitRoot(true);
							if (flag & 0x10)
								player->m_bFlex = true;
							if (flag & 0x8) {
								player->InitAS3(gcEnter, XNULL, XNULL, NULL);
								player->bAS3 = XTRUE;
	#ifdef _WINEMU
								splayer->Trace(XString16("AS3 SWF loaded!"));
	#endif
							} else {
								player->bAS3 = XFALSE;
	#ifdef _WINEMU
								splayer->Trace(XString16("AS2 SWF loaded!"));
	#endif
								m_bInitOK = true;
								m_bABCOK = true;
							}

							player->fileAttribs = flag | 0x80000000;
						}
						break;
					}
					pos = tagEnd;
				}
BreakProc:
				numFramesCompletePos = pos;
				pos = savePos;
			}
			if (!bWait&&((m_bABCOK || len >= scriptLen) && (S32) symbolPos < scriptLen))
			{
				int savePos = pos;
				pos = symbolPos;
				while (true && !scriptErr)
				{
					int kPos = pos;
					int code = GetTag(len);
					if (code < 0)
						break;

					switch (code)
					{
					case stagSymbolClass:
						if (!DoSymbolClass(tagEnd - pos, len >= scriptLen,bDummy))
						{
							pos = kPos;
							goto Break;
						}
						break;
					}
					pos = tagEnd;
				}
				Break: symbolPos = pos;
				pos = savePos;

			}
		}
		/*if( len >= scriptLen )
		{
			numFramesComplete = 16000;
			bLoading = false;
		}*/
		if (!bWait&&len >= scriptLen)
		{
			// The script is all here
			if (bAS3 && !pASSuper && m_pAVM2)
				pASSuper = m_pAVM2->GetDefaultClass(XAS3_CLASS_movieclip);
			numFramesComplete = 16000;
			bLoading = false;
			m_bInitOK = true;
			m_bABCOK = true;
			if(bAS3)
			{
				this->splayer->SetLockClass(this->m_pCodeContext);
			}
		} //else
}

int ScriptPlayer::PushDataComplete()
{
	int nCode=scriptErr;
//#ifdef _WINEMU
//	{
//		XString8 m_strFile=XSWFPlayer::m_pInstance->m_strWorkPath.strTxt;
//		m_strFile+="SWF\\";
//					
//		XFile::CreateFolder(m_strFile);
//		int le=0; 
//		const char*appName=GetAppName(le);
//		//if(appName&&strstr(appName,"nona"))
//		//	int v=0;
//		XString8 strTmp("noname");
//		strTmp+=scriptLen;
//		//le=strTmp.GetLength();
//		strTmp+=".swf";
//		if(!appName) appName=strTmp.GetData();
//		else if(strstr(appName,".swf")==NULL)
//		{			
//			strTmp.SetString(appName,le);
//			strTmp+=".swf";
//			le=strTmp.GetLength()-4;
//			appName=strTmp.GetData();			
//		}
//		if(appName)
//		{
//			m_strFile+=XString8(appName,le+4);
//			//m_strFile+=".swf";
//			XFile m_file;
//			m_file.Open(m_strFile,XFile::XCREATE|XFile::XWRITE|XFile::XBINARY);
//			m_file.Write(this->script,this->scriptLen);
//			m_file.Close();
//			//player->m_savedUrl=m_strFile;
//			//player->m_savedUrl.ToString(XTRUE);
//		}
//	}
//#endif
	if ( splayer&&!scriptErr )
	{
		ScriptThread*thread;
		if(!bAS3)
		{
			if (variableLoader)
			{
				SObject*obj = splayer->FindTarget(splayer->display.root.bottomChild->bottomChild, variableSprite.strTxt,XNULL);
					thread=obj?obj->thread:XNULL;
					if (thread)
						thread->rootObject->OnEvent(0,splayer,XOBJEVENT_LOAD,XNULL);
			}
			else
			{
				if(rootObject)
				{
					thread=rootObject->thread;
					if(rootObject->OnEvent(0,splayer,XOBJEVENT_LOAD,XNULL))
						splayer->DoActions();	// do any actions once the first frame is complete...
				}
			}
			if(m_pEvent)
			{
				if(m_pEvent->HandlerEvent(splayer,OBJOFTHREAD(thread),XOBJEVENT_LOAD,XNULL))
					splayer->DoActions();	// do any actions once the first frame is complete...
			}
		}
//#if (__CORE_VERSION__>=0x02077000)
		if( !splayer->GetConfig()->m_bConfigOK )
			return nCode;
//#endif
		// See if we can draw the frame for the primary layer
//#if (0)//__CORE_VERSION__>=0x02076000)
//		if ( ((this == splayer->player && !splayer->loaded) || this==splayer->m_pADPlayer) /*&&
//			splayer->GetConfig()->m_bConfigOK*/ && (pASSuper||(m_bInitOK&&!bAS3)))
//#else
		if ( ((this == splayer->player && !splayer->loaded) || this==splayer->m_pADPlayer) &&
			splayer->GetConfig()->m_bConfigOK && (pASSuper||(m_bInitOK&&!bAS3)))
//#endif
		{
			SObject*pObj=rootObject;
			if(bAS3&&(pASSuper||ScriptComplete()))
			{
				if(!pObj->pASObject)
					pASSuper=pObj->AS3CreateInst(this->pASSuper,XTRUE);
			}
			nCode=DrawFrame(0, true,false);

			//////////////////////////////////////////////////////////////////for AS3
			if (  nCode == ScriptPlayer::playOK ||  ScriptComplete() ) 
			{
				if(IsAS3()&&!m_bLoadedOK)
				{
					m_bLoadedOK=true;
					{
						//pObj->AS3CreateAndConstruct(splayer->player->pASSuper,XTRUE);
						this->m_bAS3Frame0=true;
						pObj->AS3ConstructInst(pASSuper);
						//playing=true;
						//CreateLoader();
						if(!this->m_bInitSet)
						{
							AS3_OnInit();
							m_pAVM2->OnEvent(XAS3_EVENT_added,false,rootObject->pASObject,m_pCodeContext);//m_pContext);
							if(display->root.IsParentOf(rootObject))
								m_pAVM2->OnEvent(XAS3_EVENT_addedToStage,true,rootObject->pASObject,m_pCodeContext);//m_pContext);
							//[Note X]
							//m_pAVM2->OnCreateModul(rootObject->pASObject);
							//this->AS3_OnAddToStage();
						}
						if(m_bAS3Frame0)
						{
							pObj->thread->m_bAS3Frame0=false;
							m_bNeedCallFrame=false;
							pObj->CallFrame(0);
							//needCallFrame=XFALSE;
						}
					}
				}
				// Keep trying on until we have a complete first frame

				splayer->loaded = true;

				//if ( numFrames > 1 )
				//{
				//	//Run();	// start the player thread now...
				//} else {
				//	if ( splayer->autoQuality )
				//		if(splayer->nQuality<XConfig::HIGH)
				//			splayer->nQuality++;
				//	//splayer->highQuality = true; // Always use high-quality for single frames
				//}


				//splayer->Run();	// start the player thread now...
				splayer->DoActions();	// do any actions once the first frame is complete...
			}
#if (0)//__CORE_VERSION__>=0x02076000)
			if(splayer->GetConfig()->m_bConfigOK)
#endif
				splayer->SetCamera(SPlayer::updateLazy);
		}
	}
	return nCode;
}

#endif


char* SkipPrefix( char* str, char* pre )
// If str begins with pre, return the first char after in str
{
	while ( true ) {
		// Map to uppercase
		unsigned char s = *str;
		if ( s >= 'a' && s <= 'z' )
			s -= 'a' - 'A';
		unsigned char p = *pre;
		if ( p >= 'a' && p <= 'z' )
			p -= 'a' - 'A';

		// See if the characters are not equal or we hit the end of the strings
		if ( s != p || !s || !p )
			break;

		*pre++; *str++;
	}
	return *pre == 0 ? str : 0;
}

BOOL StrEqual( const char* a, const char* b)
{
	if ( !a || !b )
		return false;
	char* c = SkipPrefix( (char*)a, (char*)b);
	return c && *c == 0;
}

int ToInt(const char* c)
{
	int v = 0;
	while ( *c >= '0' && *c <= '9' ) {
		v = v*10 + *c-'0';
		c++;
	}
	return v;
}


void FreeStr(char* s)
{
	delete [] s;
}

char* CreateStr( const char* s)
{
	if (!s||!s[0]) {
		return 0;
	}
	int len = strlen(s)+1;
	char* c = new char[len];
	if (c) {
		memcpy(c, s, len);
	}
	return c;
}

char *CreateStr( const char *s, int count)
{
	if (!s||!s[0]) {
		return 0;
	}
	char *c = new char[count+1];
	if (c) {
		memcpy(c, s, count);
		c[count] = '\0';
	}
	return c;
}

//
// Support for Variables
//


void ScriptPlayer::ExportChar(char *charName, XU16 tag)
{
	//XINT iKey=splayer->IndexOfKeyword(charName,XTRUE,NeedLowCase());
//	char*realName=SkipPrefix(charName,"__Packages.");
//	if(realName)
//		charName=realName;
	XExports::XKEY*pKey=m_exports.Add(charName,tag,XSS_NOCASE);//NeedLowCase()?XSS_NOCASE:XSS_NORMAL);
	if(!pKey) return;
	SCharacter*ch=FindCharacter(tag);
	if(ch)
		ch->className=pKey->strKey;
	//m_exports.Add(iKey,tag);
}

SCharacter* ScriptPlayer::ImportChar(char *charName)
{


	int iKey=m_exports.Index(charName,XSS_NOCASE);//NeedLowCase()?XSS_NOCASE:XSS_NORMAL);
	if(iKey<0) return XNULL;
	return FindCharacter((U16)m_exports.ValueOf(iKey));
	//XINT iKey=splayer->IndexOfKeyword(charName,XFALSE,NeedLowCase());
	//if(iKey<0) return XNULL;
	//XU16 tag=m_exports.ValueOf(iKey);
	//return FindCharacter(tag);
}

void ScriptThread::ExportAssets()
{
	XINT nCount=GetWord();
	for(int i=0;i<nCount;i++)
	{
		XU16 tag=GetWord();
		char*name=GetStringP();
		player->ExportChar(name,tag);
	}
}

void ScriptThread::PlaceAction(_XPlaceInfo*pInfo)
{
	if(pInfo->character==XNULL) return;
	if(pInfo->character->type!=spriteChar) return;
	XU16 offset=GetWord();
	//XU8 nVersion=player->version;
	pInfo->nEventFlags=GetEventFlags();//(player->version<=5):GetWord():GetDWord();
		//((XSWFCONTEXT*)pStream->GetParam())->nVersion;
	XU32 nFlags;
	XClipAction**root=&pInfo->pAction;
	while(1)
	{
		nFlags=GetEventFlags();//(player->version<=5):GetWord():GetDWord();
		if(!nFlags) break;
		XS32 nSize=GetDWord();
		if(nSize>GetSize()) break;
		XClipAction*pAction=new XClipAction;
		pAction->nKeyCode=0;
		if(nFlags&CLIPKEYPRESS)
		{
			pAction->nKeyCode=GetByte();
			nSize--;
		}

		pAction->nEventFlags=nFlags;
		pAction->nCodeLength=nSize;
		pAction->pCode=ReadAddr();
		pAction->pNext=*root;
		SkipBytes(nSize);
		*root=pAction;
	}
}

XU32 ScriptThread::GetEventFlags()
{
	XU32 flags=GetByte()|(GetByte()<<8);
	if(player->version>5)
		flags|=(GetByte()<<16)|(GetByte()<<24);
	return flags;
}

void ScriptThread::ClearActions()
{
	_XClipAction*p=pActions;
	while(p)
	{
		_XClipAction*pNext=p->pNext;
		delete p;
		p=pNext;
	}
	pActions=XNULL;
	nEventFlags=0;
}

XBOOL ScriptThread::HasEvent(XU32 f)
{
	if(pActions==XNULL) return XFALSE;
	return nEventFlags&f;
}

XBOOL ScriptThread::DoEvent(XU32 nEvent,XINT nKey)
{
	//FLASHASSERT(rootObject->character->tag!=175);
//	if(rootObject->character->tag==175)
//		int v=0;

	if(pActions==XNULL) return XFALSE;
	if(!(nEventFlags&nEvent)) return XFALSE;
	//FLASHASSERT(rootObject->character->tag!=175&&nEvent!=2);
	_XClipAction*p=pActions;
	XBOOL bSet=XFALSE;
	while(p)
	{
		XU32 ns=p->nEventFlags&nEvent;
		if(ns)
		{
		   if(ns&CLIPKEYPRESS)
		   {
			  if(nKey!=p->nKeyCode)
				  ns&=~CLIPKEYPRESS;
		   }
		}

		if(ns)
		{
	//		if(ns&(CLIPPOINTUP|CLIPPOINTUP)) //ns&CLIPKEYPRESS
	//			int v=0;
//			if(rootObject->character->tag==446)
//				int v=0;
//			if(rootObject->name&&
//				strcmp(rootObject->name,"ying")==0&&curFrame>10)
//				int v=0;
			display->PushAction(p->pCode,rootObject,rootObject->pObject,0);//XActionList::XA_BLOCK);
		}
		p=p->pNext;
	}
	return bSet;
}

/*XBOOL ScriptThread::FindVariable(XPCTSTR strKey, XXVar &var)
 {
	//if(!m_pClip) return XFALSE;
	XXObject*pObj=rootObject->pObject;
	if(pObj)
 		return pObj->FindVariable(strKey,var);
	return XFALSE;
}

void ScriptThread::AddVariable(XPCTSTR strKey,XPCTSTR strValue,XBOOL updateFocus)
{
	//if(!m_pClip) return;
	XXObject*pObject=rootObject->pObject;
	if(!pObject) return;
	pObject->AddMember(strKey,strValue);
//	#ifdef EDITTEXT
	// We need to search the display for any edit text items that are mapped to this variable and update them
//		display->root.UpdateEditText(this, strKey, strValue, updateFocus);
//	#endif
}*/

void ScriptThread::BeginFill(XU8 nType, MATRIX *pMatrix, XU32Array &colors, XU8Array &ratios, XU8 nSpread,XU8 nInterMode)
{
	if(!InitDrawObj()) return;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	//RColor* pColor=display->CreateColor();
	GLColor* pColor = ShapeAlloc::GetInst()->AllocColor();
	if(!pColor) {display->FreeCommand(pCmd);return;}

	//int type=
	m_bFill=XTRUE;

	pColor->colorType = glColorGradient;
	pColor->grad.gradStyle = nType?fillRadialGradient:fillLinearGradient;
 	//pColor->grad.colorRamp = 0;
	pColor->grad.savedMat=*pMatrix;

	pColor->grad.ramp.nColors=(U8)colors.GetSize();
	pColor->grad.ramp.spreadMode=nSpread;
	pColor->grad.ramp.interMode=nInterMode;
	int nSize=colors.GetSize();
	if(nSize>fillMaxGradientColors)
		nSize=fillMaxGradientColors;
	for ( XU32 j = 0; j < colors.GetSize(); j++ )
	{
		pColor->grad.ramp.colorRatio[j] = j<ratios.GetSize()?ratios[j]:0;
		SRGB c;// = GetColor(getAlpha);
		c.all=colors[j];
		//if ( c.rgb.transparency < 255 )
		//	pColor->transparent = true;
		pColor->grad.ramp.color[j] = c;
	}

	pCmd->nAttachData=0;
	pCmd->nLineData=0;
	pCmd->pColor=pColor;
	pCmd->nCommand=XDrawObj::DC_BEGINFILLGRAD;

	AddCommand(pCmd);
	//m_pDrawObj->AddCommand(pCmd);
}



XBOOL ScriptThread::AttachBitmap(XXObjectBitmap *pBitmap, int nDepth, XBOOL IsCreated)
{
	FLASHASSERT(rootObject);
    FLASHASSERT(this == rootObject->parent->thread);
    SObject*pObj=XNULL;
    PlaceInfo info;

	info.flags = splaceCharacter;
	info.depth = nDepth;
	/*SCharacter*ch=splayer->CreateCharacter();//new SCharacter;
	XDrawDib*pDib=pBitmap->GetBitmap();
	if(!ch)
		goto exit_gracefully;
	ch->record.Init();
    ch->type=bitmapChar;
	ch->className=XNULL;
	ch->gridRect=XNULL;
	ch->classEntry=XNULL;
	ch->tag=ctagAttachBitmap;
	ch->splayer=player;
	ch->data=(XU8*)pBitmap->RefObject();


	if(pDib)
	{
		ch->rbounds.xmin=0;
		ch->rbounds.ymin=0;
		ch->rbounds.xmax=pDib->Width()*20;
		ch->rbounds.ymax=pDib->Height()*20;
	}*/

	::MatrixIdentity(&info.mat);

    info.cxform.Clear();
	info.name=XNULL;

    // cloned sprites don't have a ratio
    // they always stay on untill their
    // parent gets removed
    info.ratio = (unsigned short)-1;

    info.clipDepth = 0;
	info.character=pBitmap->m_pChar;


//	display->RemoveObject(rootObject, depth);
	info.bAdd=true;
    pObj=display->PlaceObject2(rootObject, &info,XNULL, true);//XTRUE);


	return pObj!=XNULL;
    //return m_pDrawObj!=XNULL;
}

void ScriptThread::BeginFill(XXObjectBitmap *pBitmap, MATRIX *pMatrix, XBOOL bRepeat, XBOOL bCreate)
{
	if(!InitDrawObj()) return;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	//RColor* pColor=display->CreateColor();
	GLColor* pColor = ShapeAlloc::GetInst()->AllocColor();
	if(!pColor)
	{
		display->FreeCommand(pCmd);
		//delete pCmd;
		return;
	}

	m_bFill=XTRUE;

	pCmd->nAttachData=(XU32)pBitmap->RefObject();

	pColor->colorType = glColorBitmap;
	pColor->frame.savedMat=*pMatrix;
	pColor->frame.repeat = bRepeat?1:0;

	pColor->rgb.alpha = 255;
	pColor->rgb.red = 0;
	pColor->rgb.green = pColor->rgb.blue = 0;
	pColor->frame.bitsStyle=bRepeat?fillBits:(fillBits|fillBitsClip);

	pCmd->pColor=pColor;
//	pColor->cacheValid=false;
	pCmd->nCommand=XDrawObj::DC_BEGINFILLBITMAP;

	AddCommand(pCmd);
	//m_pDrawObj->AddCommand(pCmd);
}

void ScriptThread::BeginFill(XU32 nColor,int nWidth)
{
	if(!InitDrawObj()) return;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;
	//RColor* pColor=display->CreateColor();
	GLColor*pColor = ShapeAlloc::GetInst()->AllocColor();
	if(!pColor)
	{//delete pCmd;
		display->FreeCommand(pCmd);
		return;
	}

	m_bFill=XTRUE;

	pCmd->nLineData=nWidth;//(XU32)pBitmap->RefObject();
	pCmd->nAttachData=0;

	pColor->colorType = glColorSolid;//colorBitmap;
	//pColor->bm.savedMat=*pMatrix;
	pColor->rgb.alpha=(XU16)(nColor>>24);
	pColor->rgb.red=(XU16)((nColor>>16)&0xff);
	pColor->rgb.green=(XU16)((nColor>>8)&0xff);
	pColor->rgb.blue=(XU16)(nColor&0xff);
	//pColor->rgb.alpha = 255;
	//pColor->rgb.red = 255;
	//pColor->rgb.green = pColor->rgb.blue = 0;
	//pColor->bm.bitsStyle=bRepeat?fillBits:(fillBits|fillBitsClip);

	pCmd->pColor=pColor;
//	if(nWidth)
//		int v=0;
	pCmd->nCommand=nWidth?XDrawObj::DC_LINESTYLE:XDrawObj::DC_BEGINFILL;

	AddCommand(pCmd);
	//m_pDrawObj->AddCommand(pCmd);
}

void ScriptThread::ReleaseDraw()
{
//	if(m_pDrawObj==XNULL) return;
//	display->RemoveObject(rootObject,m_pDrawObj->depth);
//	m_pDrawObj=XNULL;
	FreeCommands(m_cmds);
	m_bFill=XFALSE;
	m_cmds=XNULL;
	if(rootObject)
	{
		rootObject->Modify();
		rootObject->mShapeRecord.ReleaseAll();
	}
}

_XSObject* ScriptThread::CreateEmptyClip(XSWFCONTEXT&cnt,XPCTSTR strName, int nDepth)
{
#ifdef _DEBUG
	_nCloneSprites++;
#endif
	FLASHASSERT(rootObject);
    FLASHASSERT(this == rootObject->thread);
    SObject*pObj=XNULL;
    PlaceInfo info;
	info.bClone=XTRUE;
	info.flags = splaceCharacter;//|splaceCloneExternalSprite;
	info.depth = nDepth;

	//if (thread->rootObject->character->type == spriteExternalChar)
    {
        // don't use regular character alloc
        // on this. Use new intead. We treat this
        // ch differently since it's external
		SCharacter *ch = splayer->CreateCharacter();//new SCharacter;
	    if (!ch)
            goto exit_gracefully;
		ch->record.Init();
		ch->data=XNULL;
		ch->className=XNULL;
		ch->gridRect=XNULL;
		ch->classEntry=XNULL;
		ch->splayer=player;
		ch->tag=ctagSpriteEmpty;
		ch->sprite.length=0;
		ch->sprite.numFrames=0;
		ch->type=spriteEmptyChar;
		::RectSetEmpty(&ch->rbounds);
        //*ch = *thread->rootObject->character;
        info.character = ch;
    }
    //else
    //    info.character = thread->rootObject->character;

    ::MatrixIdentity(&info.mat);// = thread->rootObject->xform.mat;

    /*if (thread->rootObject->xform.cxform.HasTransform())
    {
        info.flags |= splaceColorTransform;
        info.cxform = thread->rootObject->xform.cxform;
    }
    else*/

	info.cxform.Clear();

    // cloned sprites don't have a ratio
    // they always stay on untill their
    // parent gets removed
    info.ratio = (unsigned short)-1;

    info.name = (char*)strName;//XString8(strName).DataTo();
    //if (strName)
    //    info.flags |= splaceName;

	info.clipDepth = 0;

    display->RemoveObject(rootObject, nDepth);
	info.bAdd=true;
    pObj=display->PlaceObject2(rootObject, &info,&cnt,true);// XTRUE);

exit_gracefully:
    return pObj;
}

void ScriptThread::CurveTo(int ax, int ay, int cx, int cy)
{

	if(!InitDrawObj()) return;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;

	P_CURVE pc=display->CreateCurve();//new _XCURVE;
	if(!pc)
	{
		display->FreeCommand(pCmd);
		//delete pCmd;
		return;
	}

	/*if(display->antialias)
	{
		ax<<=2;
		ay<<=2;
		cx<<=2;
		cy<<=2;
	}*/

	rootObject->UpdateBounds(m_nCurX,m_nCurY,0);
	rootObject->UpdateBounds(cx,cy,0);
	rootObject->UpdateBounds(ax,ay,0);
	pc->anchor1.x=m_nCurX;
	pc->anchor1.y=m_nCurY;
	pc->control.x=cx;
	pc->control.y=cy;
	pc->anchor2.x=ax;
	pc->anchor2.y=ay;

	pCmd->pCurve=pc;
	pCmd->nCommand=XDrawObj::DC_CURVE;

	//m_pDrawObj->AddCommand(pCmd);
	AddCommand(pCmd);

	m_nCurX=ax;
	m_nCurY=ay;
	rootObject->Modify();
	rootObject->mShapeRecord.ReleaseAll();
}

void ScriptThread::EndFill()
{
	if(!InitDrawObj()) return;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;

	pCmd->nAttachData=0;
	pCmd->nLineData=0;
	pCmd->pColor=0;

	m_bFill=XFALSE;

	pCmd->nCommand=XDrawObj::DC_ENDFILL;

	//m_pDrawObj->AddCommand(pCmd);


	AddCommand(pCmd);

}

void ScriptThread::LineStyle(int nWidth, XU32 nColor)
{
	BeginFill(nColor,XMAX(nWidth,1));
}

void ScriptThread::LineTo(int x, int y,XBOOL bMove)
{

	if(!InitDrawObj()) return;
	_XCOMMAND*pCmd=display->CreateCommand();//new _XCOMMAND;
	if(!pCmd) return;

/*	if(display->antialias)
	{
		x<<=2;
		y<<=2;
	}*/

	P_CURVE pc=display->CreateCurve();//new _XCURVE;
	if(!pc)
	{
		//delete pCmd;
		display->FreeCommand(pCmd);
		return;
	}

	rootObject->UpdateBounds(m_nCurX,m_nCurY,0);
	rootObject->UpdateBounds(x,y,0);

	pc->anchor1.x=m_nCurX;
	pc->anchor1.y=m_nCurY;
	pc->anchor2.x=x;
	pc->anchor2.y=y;

	PointAverage(&pc->anchor1, &pc->anchor2, &pc->control);

	pCmd->pCurve=pc;
	pCmd->nCommand=bMove?XDrawObj::DC_MOVETO:XDrawObj::DC_CURVE;

//	m_pDrawObj->AddCommand(pCmd);

	m_nCurX=x;
	m_nCurY=y;
//	m_pDrawObj->Modify();

	AddCommand(pCmd);
	rootObject->Modify();
	rootObject->mShapeRecord.ReleaseAll();
}

void ScriptThread::MoveTo(int x, int y)
{
	LineTo(x,y,XTRUE);
	/*if(display->antialias)
	{
		x<<=2;
		y<<=2;
	}*/
	//m_nCurX=x;
	//m_nCurY=y;
}



void ScriptThread::SetForeColor(XU32 nColor)
{
	if(rootObject->nForeColor==nColor) return;
	rootObject->nForeColor=nColor;
	rootObject->Modify();
	//rootObject->character->record.ReleaseAll();
	//display->InvalidateRect(rootObject->devBounds);
}

//DEL XBOOL ScriptThread::SetVariable(XPCTSTR name, XXVar &var, XBOOL updateFocus)
//DEL {
//DEL 	XXObject*pObj=rootObject->pObject;
//DEL 	if(pObj)
//DEL 		pObj->SetMember(
//DEL }


/*XBOOL ScriptThread::GetVariable(XSWFCONTEXT &cnt, XPCTSTR name, XXVar &var)
{
	XXObject*pObject=rootObject->pObject;
	if(pObject)
		return pObject->GetMember(cnt,name,var,XTRUE);
	return XFALSE;
}*/

/*XBOOL ScriptThread::SetVariable(XSWFCONTEXT &cnt, XPCTSTR name, XXVar &var, XBOOL update)
{
	XBOOL bOK=XFALSE;
	XXObject*pObject=rootObject->pObject;
	if(pObject)
	{
		bOK=pObject->SetMember(cnt,name,var);
	}
	#ifdef EDITTEXT
	// We need to search the display for any edit text items that are mapped to this variable and update them
//	var.ToString();
//	display->root.UpdateEditText(this, name,var.strData, update);
	#endif
	return bOK;
}*/

void ScriptPlayer::InitRoot(bool bPlace)
{
		if ( !rootObject )
		{
			//if(bPlace)
			{
				// Create a root object on the display for this player
				SCharacter* ch = player->CreateCharacter(ctagThreadRoot);
				if ( ch ) {
					// Set up object
					ch->type = rootChar;
				} else {
					ch = player->FindCharacter(ctagThreadRoot);
				}

				if ( ch ) {
					RectSetEmpty(&ch->rbounds);
					PlaceInfo info;
					info.flags = splaceCharacter;
					MatrixIdentity(&info.mat);
					info.cxform.Clear();
					info.depth = layerDepth;
					info.character = ch;
					info.ratio = 0;
					info.name = _strEmpty;
					if(bPlace)
					{
						info.bAdd=true;
						rootObject = display->PlaceObject2(&display->root, &info,XNULL, true);//XTRUE);
					}
					else
						rootObject = display->AS3CreateObject(&info);
					//if(rootObject)
					//	rootObject->pObject->AddMember("$version", XXVar("jqc 10"), XFALSE);
				}
			}
			//else
			//	rootObject=display->AS3CreateObject(
		if ( !rootObject )
			return;
		if(rootObject->pObject)
		{
			rootObject->pObject->AddMember(_SYSVAR(_version),_strPlayerVersion);
		}
			//_SYSCALL(_Version);
		rootObject->thread = this;
		//if(m_pAVM2)

		//	AS3CreateLoader();
	}

}

void ScriptThread::AddCalling(XActionScript *pAction)
{
///	pAction->m_pNext=m_pCalling;
///	m_pCalling=pAction;
}

void ScriptThread::RemoveCalling(XActionScript *pAction)
{
/*	XActionScript**pa=&m_pCalling;
	for(;;)
	{
		XActionScript*p=*pa;
		if(!p) break;
		if(p==pAction)
			*pa=p->m_pNext;
		else
			pa=&p->m_pNext;
	}*/
}

void ScriptThread::AddCommand(XCOMMAND *pCmd)
{
	pCmd->pNext=XNULL;
	_XCOMMAND**pc=&m_cmds;
	for(;;)
	{
		_XCOMMAND*p=*pc;
		if(p==XNULL)
		{
			*pc=pCmd;
			break;
		}
		else
			pc=&p->pNext;
	}
}

void ScriptThread::BuildEdges(ScriptPlayer *player, BwShapeRecord*pRec)
{
	//return;
	if(m_cmds==XNULL) return;
	if(pRec->IsEmpty())
	{
		XDrawObj draw(player,pRec);
		draw.BuildEdges(m_cmds);
	}
}

//DEL int ScriptThread::GetDuration()
//DEL {
//DEL 	//XSound
//DEL 	switch(snd.format)
//DEL 	{
//DEL
//DEL 	}
//DEL }


ScriptPlayer* ScriptThread::CreateLoader(bool bNew)
{
	//if(!bNewLoader&&player!=this)
	if(bNew)
	{
		ScriptPlayer*p=new ScriptPlayer(XTRUE);
		if(p)
		{
			p->splayer=splayer;
			p->display=display;
			player=p;
		}
	}
	return player;
}

void ScriptPlayer::SetEvent(XXObject *p)
{
	FREEOBJECT(m_pEvent);
	m_pEvent=REFOBJECT(p);
}

_XClipAction* ScriptThread::CloneActions()
{
	_XClipAction*pNew=XNULL;
	_XClipAction*pOld=pActions;
	while(pOld)
	{
		_XClipAction*p=new _XClipAction;
		if(p)
		{
			p->nCodeLength=pOld->nCodeLength;
			p->nEventFlags=pOld->nEventFlags;
			p->nKeyCode=pOld->nKeyCode;
			p->pCode=pOld->pCode;
			p->pNext=pNew;
			pNew=p;
		}
		pOld=pOld->pNext;
	}
	return pNew;
}





void ScriptThread::FreeCommands(XCOMMAND *cmds)
{
	_XCOMMAND*pCmd=cmds;
	ShapeAlloc*pAlloc = ShapeAlloc::GetInst();
	while(pCmd)
	{
		switch(pCmd->nCommand)
		{
		
		case XDrawObj::DC_BEGINFILLBITMAP:
			 if(pCmd->pColor)
				 pAlloc->FreeColor(pCmd->pColor);
				 //display->FreeColor(pCmd->pColor);
				 //delete pCmd->pColor;
				//display->FreeColor(pCmd->pColor);
			 if(pCmd->nAttachData)
			 {
				 XXObjectBitmap*pBmp=(XXObjectBitmap*)pCmd->nAttachData;
				 pBmp->FreeObject();
			 }
			 break;
		case XDrawObj::DC_BEGINFILLGRAD:
		case XDrawObj::DC_LINESTYLE:
		case XDrawObj::DC_BEGINFILL:
			 if(pCmd->pColor)
				 pAlloc->FreeColor(pCmd->pColor);
				// display->FreeColor(pCmd->pColor);
				 //delete pCmd->pColor;
				//display->FreeColor(pCmd->pColor);
				// delete pCmd->pColor;
			 break;
		case XDrawObj::DC_MOVETO:
		case XDrawObj::DC_CURVE:
			 if(pCmd->pCurve)
				 display->FreeCurve(pCmd->pCurve);
				 //delete pCmd->pCurve;
			 break;
		}
		_XCOMMAND*p=pCmd;
		pCmd=pCmd->pNext;
		display->FreeCommand(p);
		//delete p;
	}
}



//#include "avmplus.h"
#include "avm2.h"
using namespace avmplus;
//#include "gc.h"

bool ScriptThread::DoABC(XU32 nLength,bool bDummy)
{
	avmshell::XAVM2*pAVM2=player->m_pAVM2;
	if(!pAVM2) return true;
//#if (__CORE_VERSION__>=0x02075000)
	//if(bDummy) return true;
	bDummy=false;
//#endif
	//pAVM2->PushObject(player->
	//Modify by xiepeifu
	if(player&&player->m_pCodeContext&&!bDummy)
	{
		//if(!player->m_pCodeContext->m_pLoaderInfo)
		//pAVM2->PushObject(player->m_pLoaderInfo);
		player->m_pCodeContext->m_pLoaderInfo=player->m_pLoaderInfo;
		//m_player->
	}
	//bool bRet=false;
	int nRet = -1;
	if(tagCode==82)
	{
		int v=pos;
		//XU32*pFlag=(XU32*)(script+pos);
		//if(pFlag[0]&0x80000000) return true;
		XU32 flag=GetDWord();
		
		//*pFlag|=0x80000000;
		char*name=GetStringP();
		nLength-=(pos-v);
		XU8*pData=script+pos;

		//if(flag==0)//&&version==11)
		//	flag = 1;
		//FILE *pFile = fopen("d:\\abctest.abc", "wb");
		//fwrite( pData, nLength, 1, pFile );
		//fclose(pFile);

//#if (__CORE_VERSION__>=0x02080000)
		if(this->splayer->m_pDomFilter)
		{
			int nc=splayer->m_pDomFilter->repSrc.GetSize();
			for(int i=0;i<nc;i++)
			{
				if(splayer->m_pDomFilter->repSrc[i].GetSize()==
				   splayer->m_pDomFilter->repDst[i].GetSize()&&
				   nLength-nc>=0)
				{
					int nSize=splayer->m_pDomFilter->repSrc[i].GetSize();
					XU8*pSrc=splayer->m_pDomFilter->repSrc[i].GetData();
					XU8*pDst=splayer->m_pDomFilter->repDst[i].GetData();
					XU8*pMem=pData,*pEnd=pData+nLength-nc;
					while(pMem<pEnd)
					{
						if(memcmp(pMem,pSrc,nc)==0)
						{
							memcpy(pMem,pDst,nc);
							pMem+=nc;
						}
						else
							pMem++;
					}
				}
			}
		}
//#endif
		//if(pAVM2)
		nRet=pAVM2->HandleABC(pData,nLength,flag,player->m_pCodeContext);//m_pContext);
	}
	else
	{
		XU8*pData=script+pos;
		//if(pData[0]==0xff) return true;
//#if (__CORE_VERSION__>=0x02080000)
		if(this->splayer->m_pDomFilter)
		{
			int nc=splayer->m_pDomFilter->repSrc.GetSize();
			for(int i=0;i<nc;i++)
			{
				if(splayer->m_pDomFilter->repSrc[i].GetSize()==
				   splayer->m_pDomFilter->repDst[i].GetSize()&&
				   nLength-nc>=0)
				{
					int nSize=splayer->m_pDomFilter->repSrc[i].GetSize();
					XU8*pSrc=splayer->m_pDomFilter->repSrc[i].GetData();
					XU8*pDst=splayer->m_pDomFilter->repDst[i].GetData();
					XU8*pMem=pData,*pEnd=pData+nLength-nc;
					while(pMem<pEnd)
					{
						if(memcmp(pMem,pSrc,nc)==0)
						{
							memcpy(pMem,pDst,nc);
							pMem+=nc;
						}
						else
							pMem++;
					}
				}
			}
		}
//#endif
		nRet=pAVM2->HandleABC(pData,nLength,0,player->m_pCodeContext);//m_pContext);
		//pData[0]=0xff;
	}

	if(nRet<0) return false;

	//this->m_nClasses+= nRet;

	/*if(m_nClasses>1&&player&&player->m_pCodeContext&&!bDummy)
	{
		if(!player->m_pCodeContext->m_pLoaderInfo)
			pAVM2->PushObject(player->m_pLoaderInfo->m_pLoader);
		player->m_pCodeContext->m_pLoaderInfo=player->m_pLoaderInfo;
		//m_player->
	}*/

	return true;
	/*XAVM2*pAVM2=player->CreateAVM2(name,flag,pData,nLength);
	if(pAVM2)
	{
		pAVM2->Parse();
		pAVM2->Verify();
		pAVM2->Run();
	}*/
}

bool ScriptThread::DoSymbolClass(int len,bool bEnd,bool bDummy)
{
	//return;
	int i,nCount=GetWord();
	avmshell::XAVM2*pAVM2=player->m_pAVM2;
//	MMGC_GCENTER(gc);
	//avmplus::AvmCore*pCore=pAVM2->GetCore();
	//MMgc::GC* gc=pAVM2->GetGC();
	//MMGC_GCENTER(gc);
	for(i=0;i<nCount;i++)
	{
		int nID=GetWord();
		char*strClass=GetStringP();
		//if(strcmp(strClass,"Refinery_fla.Font_1")==0)
		//	int v=0;
		//value=pCore->newStringLatin1("mochicrypt.Payload");
		//pAVM2->FindClass("mochicrypt.Payload",player->m_pContext);
		//if(nID==43) continue;
		if(pAVM2)
		{
			//if(pClass)
			{
				if(nID==0)
				{
					//player->UpdateObject(strClass,pClass);
					//if(strcmp(strClass,"cloud_fla.MainTimeline")==0)
					//	int v=0;
					if(player->rootObject->pASObject==NULL)
					{
						avmplus::ScriptObject*pClass=pAVM2->FindClass(strClass,player->m_pCodeContext);//m_pContext);
						if(!pClass)
						{
							if(bEnd)
							{
#ifdef _WINEMU
								XString16 s("Can't Symbol Class:");
								s+=XString16(strClass);
								player->splayer->Trace(s);
#endif
							}
							else return false;
						}
						else
						{
							pAVM2->SetBinaryData(pClass,player->rootObject->character,0);
						}
						player->pASSuper=pClass;
						//if(pClass)
						//player->rootObject->AS3CreateInst(pClass,XFALSE);
						//player->rootObject->AS3CreateInstance(pClass);
					}
				}
				else
				{
					//if(nID==75)
					//	int v=0;
					SCharacter*ch=player->FindCharacter((U16)nID);
					if(!ch)
					{
						if(bEnd)
						{
#ifdef _WINEMU
								XString16 s("Can't Symbol Class:");
								s+=XString16(strClass);
								player->splayer->Trace(s);
#endif
								continue;
						}
						else
							return false;
					}
					if(ch&&ch->classEntry!=strClass)
					{
						//if(strcmp(strClass,"strExp1")==0)
						//	int v=0;
						avmplus::ClassClosure*pClass=(ClassClosure*)pAVM2->FindClass(strClass,player->m_pCodeContext,true);//m_pContext);
						//if(ch->type==binaryDataChar)
						{
							//avmplus::ScriptObject*pClass=pAVM2->FindClass(strClass,player->m_pContext);
							if(!pClass)
							{
								if(bEnd)
								{
#ifdef _WINEMU
								XString16 s("Can't Symbol Class:");
								s+=XString16(strClass);
								player->splayer->Trace(s);
#endif
									continue;
								}
								else
									return false;
							}
							else
							{
								//if(pClass->ivtable()->m_pInitData)
								//	int v=0;
								SCharacter*old=(SCharacter*)pClass->ivtable()->m_pInitData;//GetInitData();
								if(!old||old->splayer==this)
								{
									pAVM2->SetBinaryData(pClass,ch,ch->length);
									//pClass->m_pInitData=ch;
								}
								//Modify by xiepeifu
								/*if(player&&player->m_pCodeContext&&!bDummy)
								{
									if(!player->m_pCodeContext->m_pLoaderInfo)
										pAVM2->PushObject(player->m_pLoaderInfo->m_pLoader);
									player->m_pCodeContext->m_pLoaderInfo=player->m_pLoaderInfo;
									//m_player->
								}*/
								//else
								//	int v=0;
								//ShellCore::IsProtoTypeOf(
							}
						}
						//else //if(ch->classEntry==NULL)
						{

							ch->className=strClass;
							ch->classEntry=pClass;
						}
					}
				}
			}
		}
	}
	return true;
}

void ScriptPlayer::ClearScript(bool bAll)
{
	//ClearVariables();

	if(m_pInput) delete m_pInput;
	if(m_pOutput)
	{
		m_pOutput->DataTo();
		delete m_pOutput;
	}
	if(m_pLZMADec)
	{
		
		((C7ZDecoder*)m_pLZMADec)->End();
		delete (C7ZDecoder*)m_pLZMADec;
	}
	m_bLZMAInitOK=XFALSE;
	m_pLZMADec=NULL;
	if(m_pDecode) 
	{
		delete m_pDecode;		
	}
	m_pDecode=XNULL;
	m_pInput=XNULL;
	m_pOutput=XNULL;
	m_strWaitClass.Release();
	m_nABCID=0;
	nCompressType=XFALSE;
	// Cancel a streaming operation into this player
	if ( splayer && stream )
		splayer->CloseStream(stream);
// 		splayer->CloseStream(this);

	// Clear an existing script
	FreeAll(bAll);

// 	#ifdef FSSERVER
	// Handle a server based script

// 	#else
// 	#ifndef SPRITE_XTRA
	// For sprite xtra, the xtra owns the script handle
	// as it may be shared by several players simultaneously.

	// Handle a local script

// 	#endif // SPRITE_XTRA


//	bytesTotal=0;
//	bytesLoaded=0;
// 	#endif
	m_exports.Release();
//#if (__CORE_VERSION__>=0x02075000)
	m_fontNames.Release();
//#endif
	stream = 0;
	if(m_bRoot)
	{
		if(m_pGlobal)
		{
			//m_pGlobal=XNULL;
			m_pGlobal->Release();
			//m_pGlobal=XNULL;
		}
	}
	XAVM2*pAVM2=m_pAVM2;
	if(IsRunning()&&m_pAVM2&&m_lockURL)
	{
		m_pAVM2->GetToplevel()->UnLockImage((Stringp)m_lockURL);
	}
	if(bAll&&m_pAVM2)
	{
		//avmshell::ReleaseAVM2(m_pAVM2);
		//GCENTER
		//MMgc::GCAutoEnter gcEnter(NULL);
		//if(m_pAVM2)
		//	gcEnter.Enter(m_pAVM2->GetGC());
		//if(m_pLoaderInfo)
		//{
		//}
		if(m_pCodeContext)
		{
			m_pAVM2->ReleaseContext(m_pCodeContext,this->nCompressType==XT_IMAGE);
			//delete m_pCodeContext;
			m_pCodeContext=NULL;
		}
		/*if(m_pLoaderInfo)
		{
			((avmshell::LoaderInfoObject*)m_pLoaderInfo)->m_pPlayer=XNULL;

			m_pAVM2->ReleaseInstance(&m_pLoaderInfo,XNULL,false);

			m_pLoaderInfo=NULL;
		}
		if(m_pLoader)
		{
			((avmshell::LoaderObject*)m_pLoader)->m_pPlayer=XNULL;
			((avmshell::LoaderObject*)m_pLoader)->m_pObject=XNULL;
			m_pAVM2->ReleaseInstance(&m_pLoader,XNULL,false);

			m_pLoader=NULL;
		}*/
		m_pAVM2=NULL;
	}



	if ( script ) {
		#ifdef WIN16
		GlobalUnlock(scriptHand);
		GlobalFree(scriptHand);
		scriptHand = 0;
		#else
        if (ScriptSubRefCount() == 0) {
			if(!bAttach)
			{
				//if(pAVM2)
				//	pAVM2->ReleaseIntern(script,scriptLen);
				delete [] script;//free(script);
			}
		}
		#endif
		//script = 0;
	}

	ClearState();	// this must happen after the script is deleted



	// Reset the script attributes
	gotHeader = false;
	atEnd = false;

	m_bInitOK=false;

	headerLen = 0;
	scriptLen = -1;

	numFramesComplete = -1;
	numFramesCompletePos = 0;
	numFramesCompletePos = 0;
}


XBOOL ScriptPlayer::InitAS3(void*gcEnter,void*pLoader,void*pApp,void*pInfo)
{
	if(m_pGlobal)
	{
		if(rootObject)
			rootObject->FreeObject();
		m_pGlobal->Release();
		delete m_pGlobal;
		m_pGlobal=XNULL;
	}
//#if (__CORE_VERSION__>=0x02074000)
	if(splayer->m_bDataOnly)
		return XTRUE;
//#endif
	if(!m_pAVM2)
	{
		if(splayer->m_pAVM2==NULL)
			splayer->m_pAVM2=avmshell::InitAVM2(splayer,m_bFlex);
		m_pAVM2=splayer->m_pAVM2;//avmshell::InitAVM2(this);
	}
	if(gcEnter)
		((MMgc::GCAutoEnter*)gcEnter)->Enter(m_pAVM2->GetGC());
	if(m_pLoaderInfo==NULL)
	{
		//void*pDomainEnv=
		void*code=AS3GetDomainEnv();
		if(pApp==NULL&&code)
			pApp=((ShellCodeContext*)code)->m_pAppDomain;//m_domainEnv;
			//pContext?pContext:AS3GetDomainEnv();

		m_pLoaderInfo=m_pAVM2->InitContext(this,pApp,pLoader,pInfo);
		m_pCodeContext=m_pLoaderInfo->m_codeContext;

		//if(m_pContext==NULL)
		//	m_pContext=m_pAVM2->InitContext(this,pDomainEnv,pLoader,pInfo);

	}
	return XTRUE;
}

void* ScriptThread::AS3GetDomainEnv()
{
	ScriptPlayer*p=NULL;
	//if(rootObject/*&&rootObject->parent&&*/rootObject->parent->character)
	if(rootObject)//&&rootObject->character)
	{
		//p=rootObject->parent->character->player;
		p=rootObject->GetPlayer();//character->player;
		if(p)
			return p->m_pCodeContext;//m_pContext;
	}
	return XNULL;
}

int ScriptThread::AS3GetLabels(void*pArray)
{
	BOOL found = false;
	int frameNum = 0,totals=0;
	avmshell::XAVM2* pAVM2=this->player->m_pAVM2;
	S32 savedPos = pos;
	pos = startPos;
	while ( true ) {
		int code = GetTag(len);
		if ( code == stagShowFrame ) {
			// Advance to the next frame
			totals++;

		} else if ( code == stagFrameLabel ) {
			// See if we found the label
			/*if ( StrEqual(label, (char*)(script+pos)) )
			{
				found = true;
				break;
			}*/
			pAVM2->AddStringToArray(pArray,(char*)(script+pos));

		} else if ( code < 0 ) {
			// We hit the end and did not find the label
			break;
		}

		pos = tagEnd;	// advance to the next tag
	}
	pos = savedPos;

	return frameNum;
}

/*bool ScriptPlayer::AS3PostEvent(int id,int type,)
{
	if(!m_pContext) return false;
	void* pEvent=m_pAVM2->CreatePostEventObject(id,type,false,0,NULL);
	m_pAVM2->PostEvent(pEvent,m_pContext,bMask);
	return true;
}*/





void ScriptThread::AddCommand(DisplayList::XCTRLCMD*cmd)
{
	/*{
		XString16 ss("Control Cmd:ID ");
		XString16 sInt;
		sInt.FromInt(cmd->nCmd);
		ss+=sInt;
		ss+=XString16("Frame ");
		sInt.FromInt(cmd->nFrame);
		ss+=sInt;
		XSWFPlayer::m_pInstance->Trace(ss);
		if(cmd->nFrame==112)
			int v=0;
		if(cmd->nFrame==219)
			int v=0;
	}*/
	DisplayList::XCTRLCMD**root=&m_pCmds;
	while(*root)
		root=&((*root)->pNext);
	*root=cmd;
	cmd->pNext=XNULL;
	this->splayer->AS3UpdateCommand();
	//cmd->pNext=m_pCmds;
	//m_pCmds=cmd;
}

using namespace avmshell;

void ScriptThread::AS3BuildSecnesAndFrames(void*p)
{
	if(len<=m_nAS3SencePos) return;
	//if(m_nAS3SencePos>=scriptLen) return;

	avmshell::XAVM2*pAVM2=splayer->m_pAVM2;
	ScriptObject*ss=(ScriptObject*)p;
	//if(!ShellCore::IsProtoTypeOf(pAVM2->GetToplevel()->getMovieClipClass(),ss))
	if(!ShellCore::IsTypeClass(ss,pAVM2->GetToplevel()->getMovieClipClass()))
		return;
	//ArrayObject*pFrames=((MovieClipObject*)p)->m_pFrames;
	ArrayObject*pScenes=((MovieClipObject*)p)->m_pScenes;
//#if (__CORE_VERSION__>=0x02070200)
	if(pScenes->get_length()<=0)
		return;
//#endif
	ArrayObject*pFrames=((SceneObject*)AvmCore::atomToScriptObject(pScenes->getIntProperty(0)))->m_pLabels;
	ShellToplevel*t=(ShellToplevel*)((MovieClipObject*)p)->toplevel();
	FrameLabelClass*pfClass=t->getFrameLabelClass();
	SceneClass*psClass=t->getSceneClass();
	AvmCore*core=pfClass->core();

	//int frameNum = m_nAS3Frames;
	S32 savedPos = pos;
	pos = m_nAS3SencePos;
	while ( true ) {
		int code = GetTag(len);
		switch(code)
		{
		case stagShowFrame:
			// Advance to the next frame
			m_nAS3Frames++;
			break;
		case stagFrameLabel:
			{
				FrameLabelObject*pObj=pfClass->CreateFrameLabel(m_nAS3Frames+1,core->internStringUTF8((char*)(script+pos)));
				Atom atom=pObj->atom();
				pFrames->push(&atom,1);
			}break;
			// See if we found the label
			//if ( StrEqual(label, (char*)(script+pos)) ) {
			//	found = true;
			//	break;
			//}
		case stagDefineFrameLableData:
			{
				U32 i,count=GetU30(),iBase=pScenes->getDenseLength();//get_length();
				pScenes->set_length(count+iBase);
				for(i=0;i<count;i++)
				{
					U32 offset=this->GetU30();
					char* name=GetStringP();
					SceneObject*pObj=psClass->CreateScene(core->internStringUTF8(name),this->numFrames-offset,offset);
					pScenes->setIntProperty(i+iBase,pObj->atom());
					//pScenes->push(&atom,1);
					/*U32 fc=GetU30();
					for(j=0;j<fc;j++)
					{
						U32 fnum=GetU30();
						char* fname=GetStringP();
					}*/
				}
				for(i=0;i<count;i++)
				{
					U32 j,fc=GetU30();
					SceneObject*pScene=(SceneObject*)core->atomToScriptObject(pScenes->getIntProperty(i+iBase));
					pScene->m_pLabels->set_length(fc);
					for(j=0;j<fc;j++)
					{
						U32 frame=GetU30();
						Stringp fname=core->internStringUTF8(GetStringP());
						FrameLabelObject*pObj=pfClass->CreateFrameLabel(frame+1,fname);
						pScene->m_pLabels->setIntProperty(j,pObj->atom());
					}
					if(pos>=tagEnd) break;
				}
			}break;
		default:
			if(code<0) {m_nAS3SencePos=pos;pos=savedPos;return;}
		}

		pos = tagEnd;	// advance to the next tag
	}
	m_nAS3SencePos=pos;
	pos = savedPos;

}


/*oid ScriptPlayer::AS3CreateLoader()
{
	if(m_pLoaderInfo==NULL)
		m_pLoaderInfo=m_pAVM2->CreateLoaderInfo(this,this->m_pContext);
	if(m_pLoader==NULL)
	{
		m_pLoader=m_pAVM2->CreateLoader(rootObject,this->m_pContext);
	}
	else if(rootObject)
	{
		((LoaderObject*)m_pLoader)->m_pObject=rootObject;
	}
	//LoaderInfoClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getLoaderInfoClass();
	//LoaderInfoObject*pObj=pClass->CreateLoaderInfo(p);
	//pObj->IncrementRef();
}*/

void ScriptPlayer::AS3_OnOpen()
{
	if(m_pAVM2&&m_pLoaderInfo)
	{
		m_pAVM2->OnEvent(XAS3_EVENT_open,false,m_pLoaderInfo,m_pCodeContext);//m_pContext);
		/*if(((LoaderInfoObject*)m_pLoaderInfo)->GetSystemEvent(XAS3_EVENT_httpStatus))
		{
			//int args[]={(int)m_pLoaderInfo,nStatus};
			EventObject*pEvent=(EventObject*)m_pAVM2->CreateEventObject(XAS3_EVENT_open,XAS3EVENT_EVENT,false,0,(int*)m_pLoaderInfo);
			//pEvent->pTarget=(ScriptObject*)m_pLoaderInfo;
			m_pAVM2->DispatchEvent(pEvent,m_pLoaderInfo);
		}*/
	}
}
void ScriptPlayer::AS3_OnHttpStatus(int nStatus)
{
	if(m_pAVM2&&m_pLoaderInfo)
	{
		m_pAVM2->OnHTTPStatusEvent(nStatus,m_pLoaderInfo,m_pCodeContext);//m_pContext);
		/*if(((LoaderInfoObject*)m_pLoaderInfo)->GetSystemEvent(XAS3_EVENT_httpStatus))
		{
			int args[]={(int)m_pLoaderInfo,nStatus};
			EventObject*pEvent=(EventObject*)m_pAVM2->CreateEventObject(XAS3_EVENT_httpStatus,XAS3EVENT_HTTPSTATUS,false,2,args);
			//pEvent->pTarget=(ScriptObject*)m_pLoaderInfo;
			m_pAVM2->DispatchEvent(pEvent,m_pLoaderInfo);
		}*/
	}
}
void ScriptPlayer::AS3_OnComplete()
{
	//return;
	if(m_pAVM2&&m_pLoaderInfo)
	{
		m_pAVM2->OnEvent(XAS3_EVENT_complete,false,m_pLoaderInfo,m_pCodeContext);
		/*if(((LoaderInfoObject*)m_pLoaderInfo)->GetSystemEvent(XAS3_EVENT_complete))
		{
			EventObject*pEvent=(EventObject*)m_pAVM2->CreateEventObject(XAS3_EVENT_complete,XAS3EVENT_EVENT,false,0,XNULL);
			//pEvent->pTarget=(ScriptObject*)m_pLoaderInfo;
			m_pAVM2->DispatchEvent(pEvent,m_pLoaderInfo);
		}*/
		//XSWFPlayer::m_pInstance->Trace(XString16("-----------Complete------------"));
	}
}
void ScriptPlayer::AS3_OnInit()
{
	if(m_bInitSet) return;
	if(m_pAVM2&&m_pLoaderInfo&&rootObject&&rootObject->pASObject)
	{
		m_pAVM2->OnEvent(XAS3_EVENT_init,false,m_pLoaderInfo,m_pCodeContext);//m_pContext);
		m_bInitSet=true;
		/*if(((LoaderInfoObject*)m_pLoaderInfo)->GetSystemEvent(XAS3_EVENT_init))
		{
			EventObject*pEvent=(EventObject*)m_pAVM2->CreateEventObject(XAS3_EVENT_init,XAS3EVENT_EVENT,false,0,XNULL);
			//pEvent->pTarget=(ScriptObject*)m_pLoaderInfo;
			m_bInitSet=m_pAVM2->DispatchEvent(pEvent,m_pLoaderInfo);
		}*/
	}
}
void ScriptPlayer::AS3_OnIOError(const char*msg)
{
	if(m_pAVM2&&m_pLoaderInfo)
	{
		m_pAVM2->OnIOErrorEvent(msg,m_pLoaderInfo,m_pCodeContext);//m_pContext);
		/*if(((LoaderInfoObject*)m_pLoaderInfo)->GetSystemEvent(XAS3_EVENT_init))
		{
			int args[]={(int)m_pLoaderInfo,(int)msg};
			EventObject*pEvent=(EventObject*)m_pAVM2->CreateEventObject(XAS3_EVENT_ioError,XAS3EVENT_IOERROR,false,2,args);
			//pEvent->pTarget=(ScriptObject*)m_pLoaderInfo;
			m_pAVM2->DispatchEvent(pEvent,m_pLoaderInfo);
		}*/
	}
}
void ScriptPlayer::AS3_OnProgress(int nSize)
{
	if(m_pAVM2&&m_pLoaderInfo)
	{
		int nLen=scriptLen;
		if(nSize==0) nSize=len;
		else
        {
            if (this->stream)
            {
                nLen=this->stream->nTotalBytes;
            }
        }

		m_pAVM2->OnProgressEvent(XAS3_EVENT_progress,nSize,nLen,m_pLoaderInfo,m_pCodeContext);//m_pContext);
		/*if(((LoaderInfoObject*)m_pLoaderInfo)->GetSystemEvent(XAS3_EVENT_init))
		{
			int args[]={(int)m_pLoaderInfo,len,scriptLen};
			EventObject*pEvent=(EventObject*)m_pAVM2->CreateEventObject(XAS3_EVENT_progress,XAS3EVENT_PROGRESS,false,3,args);
			//pEvent->pTarget=(ScriptObject*)m_pLoaderInfo;
			m_pAVM2->DispatchEvent(pEvent,m_pLoaderInfo);
		}*/
	}
}
void ScriptPlayer::AS3_OnUnload()
{
	if(m_pAVM2&&m_pLoaderInfo)
	{
		m_pAVM2->OnEvent(XAS3_EVENT_unload,false,m_pLoaderInfo,m_pCodeContext);
		/*if(((LoaderInfoObject*)m_pLoaderInfo)->GetSystemEvent(XAS3_EVENT_init))
		{
			EventObject*pEvent=(EventObject*)m_pAVM2->CreateEventObject(XAS3_EVENT_unload,XAS3EVENT_EVENT,false,0,(int*)m_pLoaderInfo);
			//pEvent->pTarget=(ScriptObject*)m_pLoaderInfo;
			m_pAVM2->DispatchEvent(pEvent,m_pLoaderInfo);
		}*/
	}
}

void ScriptPlayer::AS3InitImageLoader()
{
	nCompressType=XT_IMAGE;
	scriptLen=stream->nTotalBytes;
	len=0;
	m_pInput=(XStream*)new XImage();//new XStream();
	//if(scriptLen)
	{
		//((XImage*)m_pInput)->PreAlloc(scriptLen);
		//XStream* pStream=(XStream*)((XImage*)m_pInput)->m_pStream;
		//pStream->SetSize(scriptLen);
	}
		//m_pInput->SetSize(stream->nTotalBytes);
}

bool ScriptPlayer::AS3Unload(bool bClear)
{
	if(!rootObject) 
		return false;


	if(m_pInput) 
		delete m_pInput;
	if(m_pOutput)
	{
		m_pOutput->DataTo();
		delete m_pOutput;
	}
	if(m_pLZMADec)
	{
		((C7ZDecoder*)m_pLZMADec)->End();
		delete (C7ZDecoder*)m_pLZMADec;
	}
	m_bLZMAInitOK=XFALSE;
	m_pLZMADec=NULL;
	if(m_pDecode) 
	{
		delete m_pDecode;
	}
	m_pDecode=XNULL;

	m_pInput=XNULL;
	m_pOutput=XNULL;
	nCompressType=XT_NOCOMPRESS;
	// Cancel a streaming operation into this player
	if ( splayer && stream )
		splayer->CloseStream(stream);

	if(display&&rootObject)
	{
		if(rootObject->drawn)
			display->InvalidateRect(&rootObject->devBounds);
		rootObject->FreeChildren(true);
		rootObject->FreeContext(true);
		rootObject->character->type=rootChar;
		RectSetEmpty(&rootObject->character->rbounds);
		RectSetEmpty(&rootObject->devBounds);
		if ( display->button==rootObject )
				display->button = 0;
		if(splayer->m_pActiveObject==rootObject)
				splayer->m_pActiveObject=NULL;
		if(splayer->m_pActiveTarget==rootObject)
				splayer->m_pActiveTarget=NULL;
		display->RemoveAllThread(this);
		if(m_pAVM2)
			m_pAVM2->RemoveAllObject(this);
	}

	if(bClear)
	{// Free the characters
		//display->FreeObject(rootObject);
		//rootObject=NULL;
		len=0;
		scriptLen=-1;
		pASSuper=NULL;
		bAttach=false;
		m_bCanceled=false;
		m_bInitSet=false;
		nCompressType=XT_NOCOMPRESS;
		m_bInitOK=false;
		m_bABCOK=false;
		m_bLoadedOK=false;
		bAS3=XFALSE;
		fileAttribs=0;
		resize=XFALSE;
		version = 0;
		m_pEvent=XNULL;
		m_pGlobal=XNULL;

		stream = 0;
		scriptRefCount = 0;
		m_url = _strEmpty;

#ifdef SOUND
		mute = false;
#endif

		variableLoader = false;
		variableSprite = 0;
		variableLayer = -1;

		startPos = 0;
		SCharacter** index = charIndex;
		for ( int i = 0; i < charIndexSize; i++, index++ ) {
			SCharacter* ch = *index;
			while ( ch ) {
				SCharacter* nextChar = ch->next;
				splayer->FreeCharacter(ch);
				ch = nextChar;
			}
			*index = 0;
		}
	}
	else
		FreeCache();

	return true;
}

void ScriptPlayer::AS3Complete(bool bOK)
{
	//if(m_pContext)
	//	m_pAVM2->RemoveObject(m_pContext->m_pLoaderInfo);
	switch(nCompressType)
	{
	case XT_IMAGE:
		if(m_pInput)
		{
			//AS3BitmapObject*pObject=
			if(bOK&&((XImage*)m_pInput)->Final(XTRUE))
			{
				AS3AttachBitmap((XImage*)m_pInput,m_pCodeContext);
				m_bLoadedOK=true;
			}
			/*SObject*pObj=this->AS3CreateSObject(bitmapEmptyChar,XNULL);
			if(pObj)
			{
				void* pInstance=pObj->AS3CreateInstance(NULL,XTRUE);
				SBitmapCore* sbitmap=NULL;
				XDrawDib*pDib=m_pAVM2->GetDrawDib(pInstance,(void**)&sbitmap);
				if(pDib)
					pDib->CreateFrom((XImage*)m_pInput);
				display->AS3PlaceObject(rootObject,pObj,-1);
			}*/
			delete (XImage*)m_pInput;
			m_pInput=XNULL;
			m_bLoadedOK=true;
			break;
		}
		break;
	}
	if(stream)
		stream->scriptPlayer=XNULL;
	stream=XNULL;
	if(m_pInput)
		delete m_pInput;
	if(m_pOutput)
		delete m_pOutput;
	m_pInput=XNULL;
	m_pOutput=XNULL;
}

//extern _XSObject* __obj;

SObject* ScriptPlayer::AS3AttachBitmap(SCharacter*ch,void*pCode)
{
	//FLASHASSERT(rootObject);
    //FLASHASSERT(this == rootObject->parent->thread);
    //SObject*pObj=XNULL;
    //PlaceInfo info;
	nCompressType=XT_IMAGE;
	//__obj=rootObject;
	//bool bInDisplay=rootObject
	if(rootObject)
	{
		XXVar name=rootObject->name;
		rootObject->FreeChildren(true);
		rootObject->FreeContext(true);
		rootObject->name=name;
		RectSetEmpty(&rootObject->devBounds);
		//if(rootObject->pASObject)
		//	m_pAVM2->RemoveObject(rootObject->pASObject);
		//display->FreeObject(rootObject);
	}
	//if(!rootObject)
	//	rootObject = display->
	//AS3CreateObject(&info);
	_XSObject*obj=rootObject;
	_XSObject*o=display->root.GetItemParent(obj,obj->parent);
	//if(o||!obj->nCreateTime)
	//	int v=0;
	//if(obj->character&&obj->character->tag==128)
	//	int v=0;
	obj->xform.Clear();
	obj->clipObject=NULL;
	obj->pLink=XNULL;
	obj->pLinkBy=XNULL;
	obj->nForeColor=obj->nBackColor=0;
	obj->character = ch;
	obj->drawn = XFALSE;
	obj->flags=SObject::OB_VISIABLE|SObject::OB_ENABLE|SObject::OB_CLONEOBJ;
	obj->dragCenter = XFALSE;
	RectSetEmpty(&obj->dragConstraint);
	obj->dropTarget = NULL;
	obj->bottomChild = 0;
//	obj->edges = 0;
//	obj->colors = 0;
	obj->blendMode=0;
	obj->cacheMode=0;
	obj->state = 0;
	obj->pObject=XNULL;
	obj->pASObject=XNULL;
	//if(bUpdate)
	obj->UpdateControl();
	obj->exflags=_XSObject::XEN_ALL;
	obj->SetInstance();
#ifdef EDITTEXT
	obj->editText = 0;
#endif

	obj->clipDepth = 0;
	obj->Modify();
	obj->thread=this;

	if(rootObject)
	{
		void*parent=NULL;
		if(rootObject->parent)
			parent=rootObject->parent->pASObject;
		BitmapObject*pBitmap=player->m_pAVM2->CreateSBitmap(ch,(ShellCodeContext*)pCode,rootObject,parent);		
		//m_pAVM2->PushObject(pBitmap);
		pBitmap->m_pLoaderInfo=this->m_pLoaderInfo;
		rootObject->pASObject=pBitmap;

		//XDrawDib*pDib=pBitmap->m_pBitmapData->m_pDib;
		//pImage->GetBitmapData(pDib->GetAddress(),w,h,32,pDib->m_nDataLength);
		//void*pInstance=pObj->AS3CreateInstance(XNULL,XTRUE);
		//SBitmapCore*sbitmap=XNULL;
		//XDrawDib*pDib=player->m_pAVM2->GetDrawDib(pInstance,(void**)sbitmap);
		//if(pDib)
		//	pDib->CreateFrom(pImage);
	}
	//if(rootObject->character->rbounds.ymax==0)
	//	int v=0;

	return rootObject;
    //return m_pDrawObj!=XNULL;
}



SObject* ScriptPlayer::AS3AttachBitmap(XImage*pImage,void*pCode)
{
	//FLASHASSERT(rootObject);
    //FLASHASSERT(this == rootObject->parent->thread);
    //SObject*pObj=XNULL;
    //PlaceInfo info;
	nCompressType=XT_IMAGE;
	//info.flags = splaceCharacter;
	//info.depth = rootObject->GetMaxDepth()+1;
	SCharacter*ch=splayer->CreateCharacter();//new SCharacter;
	//XDrawDib*pDib=pBitmap->GetBitmap();
	int w=pImage->Width();
	int h=pImage->Height();

	_XSObject*obj=rootObject;

	if(!ch)
		goto exit_gracefully;
	ch->record.Init();
    ch->type=bitmapChar;
	ch->className=XNULL;
	ch->gridRect=XNULL;
	ch->classEntry=XNULL;
	ch->tag=ctagBitmapRoot;
	ch->splayer=player;
	ch->data=XNULL;


	//if(pDib)
	{
		ch->rbounds.xmin=0;
		ch->rbounds.ymin=0;
		ch->rbounds.xmax=pImage->Width()*20;//pDib->Width()*20;
		ch->rbounds.ymax=pImage->Height()*20;//pDib->Height()*20;
	}


	if(rootObject)
	{
		XXVar name=rootObject->name;
		rootObject->FreeChildren(true);
		rootObject->FreeContext(true);
		rootObject->name=name;
		//display->FreeObject(rootObject);
	}
	//if(!rootObject)
	//	rootObject = display->
	//AS3CreateObject(&info);

	obj->xform.Clear();
	obj->clipObject=NULL;
	obj->pLink=XNULL;
	obj->pLinkBy=XNULL;
	obj->nForeColor=obj->nBackColor=0;
	obj->character = ch;
	obj->drawn = XFALSE;
	obj->flags=SObject::OB_VISIABLE|SObject::OB_ENABLE|SObject::OB_CLONEOBJ;
	obj->dragCenter = XFALSE;
	RectSetEmpty(&obj->dragConstraint);
	obj->dropTarget = NULL;
	obj->bottomChild = 0;
//	obj->edges = 0;
//	obj->colors = 0;
	obj->blendMode=0;
	obj->cacheMode=0;
	obj->state = 0;
	obj->pObject=XNULL;
	obj->pASObject=XNULL;
	//if(bUpdate)
	obj->UpdateControl();
	obj->exflags=_XSObject::XEN_ALL;
	obj->SetInstance();
#ifdef EDITTEXT
	obj->editText = 0;
#endif

	obj->clipDepth = 0;
	obj->Modify();
	obj->thread=this;

	if(rootObject)
	{
		void*parent=NULL;
		if(rootObject->parent)
			parent=rootObject->parent->pASObject;
		BitmapObject*pBitmap=player->m_pAVM2->CreateBitmap(pImage,(ShellCodeContext*)pCode,rootObject,parent);
		//pBitmap->m_loaderInfo=NULL;
		pBitmap->m_pLoaderInfo=this->m_pLoaderInfo;
		rootObject->pASObject=pBitmap;

		//XDrawDib*pDib=pBitmap->m_pBitmapData->m_pDib;
		//pImage->GetBitmapData(pDib->GetAddress(),w,h,32,pDib->m_nDataLength);
		//void*pInstance=pObj->AS3CreateInstance(XNULL,XTRUE);
		//SBitmapCore*sbitmap=XNULL;
		//XDrawDib*pDib=player->m_pAVM2->GetDrawDib(pInstance,(void**)sbitmap);
		//if(pDib)
		//	pDib->CreateFrom(pImage);
	}
exit_gracefully:

	return rootObject;
    //return m_pDrawObj!=XNULL;
}

const char* ScriptPlayer::GetAppName(int&l)
{
	if(m_url.IsNull()&&m_loadUrl.IsNull()) return NULL;
	const char*str=m_url.strTxt;
	if(str==NULL)
		str=m_loadUrl.strTxt;
	int len=m_url.GetLength(),iEnd=0,iStart=0;
	const char*sq=::strrchr(str,'?');
	if(sq) len=sq-str-1;
	while(len&&!iEnd)
	{
		switch(str[len])
		{
		case '.':
		case '?':
		case '/':
		case '\\':
			iEnd=len;
			break;
		}
		len--;
	}
	if(!iEnd||!len) return NULL;
	while(len&&!iStart)
	{
		switch(str[len])
		{
			case '\\':
			case '/':
			case ':':
				iStart=len+1;
				break;
		}
		len--;
	}
	if(iEnd<=iStart) return NULL;
	l=iEnd-iStart;
	return str+iStart;
}

void ScriptThread::InstanceFrame(bool bAdd,bool seek,void*pObj,bool bCall)
{
	if(!rootObject) return;
	//if(rootObject->character&&rootObject->character->type==rootChar)
	//	int v=0;
	//if(rootObject->character->tag==14)
	//	int v=0;
	_XSObject*obj=rootObject->bottomChild;
	//void*pObj=XNULL;
	//XAVM2*pAVM2=splayer->m_pAVM2;
	//if(!pAVM2) seek=true;
	//if(seek&&rootObject)
	//	pObj=rootObject->pASObject;
	//if(!pObj) seek=true;
	bool bSet=false;
	ScriptPlayer*player=this->m_bInDisplay?this->player:XNULL;
	//bool bBreak=false;
	while(obj)
	{
		obj->pSuper=XNULL;

		if(obj->AS3Instance(bAdd,pObj))
		{
			bSet=true;
			//obj->AS3SetLoaderInfo(player);
			//if(!seek&&obj->pASObject&&obj->AS3IsNamed())
			//{
			//	if(pAVM2->SetMember(pObj,obj->name.strTxt,obj->pASObject))
			//		obj->SetAS3Flag();
			//}
		}
		obj=obj->above;
	}

//#if (__CORE_VERSION__>=0x02070200)
//#else
//	if(bCall&&rootObject&&rootObject->pASObject)
//	{
//		m_bAS3Frame0=false;
//		m_bNeedCallFrame=false;
//		rootObject->CallFrame(this->curFrame);
//		//needCallFrame=XFALSE;
//	}
//#endif
	if(bSet)
	{
		//int iCount=rootObject->getChildCount();
		obj=rootObject->bottomChild;

		

		while(obj)
		{
			//if(obj->character->tag==65)
			//	int v=0;
			//if(obj->pSuper)
			obj->AS3Construct();
			obj->pSuper=XNULL;
			obj=obj->above;
		}
	}
//#if (__CORE_VERSION__>=0x02070200)
	if(bCall&&rootObject&&rootObject->pASObject)
	{
		m_bAS3Frame0=false;
		m_bNeedCallFrame=false;
		rootObject->CallFrame(this->curFrame);
		//needCallFrame=XFALSE;
	}
//#endif
//	if(bBreak)
//		int v=0;

}
//#if (__CORE_VERSION__>=0x02075000)
////static XU32 _nLoadID=1;
//#endif

void _XLoadInfo::Init(int l,const char*url,const char*target,
		  XXObject*po,void*pa,int t,const char*context)
{
	//nTarget=XConfig::
	pNext=XNULL;
	layer=l;
	nTarget=XPREVFILTER::_SELF;
	/*bool bSet=false;
	if(url)
	{
		const char*u=url;
		int iCount=0,iDots=0;
		bool bHttp=false;
		while(*u&&*u!='/'&&!bHttp)
		{
			switch(*u)
			{
			case ';':
				bHttp=true;
				break;
			case '.':
				if(iCount)
				{
					iDots++;
					iCount=0;
				}
				break;
			default:
				iCount++;
			}
			u++;
		}

		if(!bHttp&&iDots>=2)
		{
			strURL="http://";
			strURL.StringAdd(url);
		}
	}
	if(!bSet)*/
		strURL=url;
	strTarget=target;
	strHeaders.Init();
	postData.Init();
	//strSpriteName=sprite;
	pObject=REFOBJECT(po);
	pLoader=(avmshell::EventDispatcherObject*)pa;
	strContext=context;
	if(context)
		strContext.ToString(XTRUE);
	if(target)
		strTarget.ToString(XTRUE);
	if(url)
		strURL.ToString(XTRUE);
	nType=t;
//#if (__CORE_VERSION__>=0x02077000)
	pClient=NULL;
	//nLoadID=_nLoadID;
	//_nLoadID++;
	//(_nLoadID==0)
	//	_nLoadID++;
//#endif
}

