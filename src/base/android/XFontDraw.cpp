#include "StdAfxGGBase.h"
#include "XFontDraw.h"
#include "XString.h"
#include "GlobalFun.h"
//#include <math.h>

XFontDraw* XFontDraw::m_pInstance = NULL;
#define _FONTBLOCK 256*256*4
#define _FONTALPHABLOCK 256*256
static XU8 _fontData[_FONTBLOCK];
static XU8 _fontAlphaData[_FONTALPHABLOCK];

#include "ft2build.h"
#include FT_FREETYPE_H
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftoutln.h"
#include "FontBuffer.h"
#include <unistd.h>
#include "MultiStringUtil.h"

FontBuffer gFlashFonts[MAX_FONTS];

FT_Library gFTLibrary = NULL;
FT_Face    gFTFace = NULL;

XFontDraw::XFontDraw()
{
	gFTLibrary = NULL;
	gFTFace = NULL;
	m_fontSize = 0;
	m_fontID = 0;
}

XFontDraw::~XFontDraw()
{
	if( gFTFace != NULL )
	{
		FT_Done_Face( gFTFace );
		FT_Done_FreeType( gFTLibrary );
		m_fontSize = 0;
		m_fontID = 0;
	}
}

void XFontDraw::SetFont(const char* fontName,int id,int nSize)
{
	if( m_fontID == id && m_fontSize == nSize )
		return;

	m_fontSize = nSize;
	m_fontID = id;
	GFunSetFont( nSize );
	return;
}

bool XFontDraw::Draw(XPCWSTR str, int len,_FONTINFO&info)
{
	char *putf8 = unicodeToUtf8( str );
	GFunDrawString( putf8 );
	delete []putf8;

	info.fw = gTxtWidth;
	info.w = gTxtWidth;
	info.h = gTxtHeight;

	int ilen = info.w * info.h;
//	if( ilen < _FONTALPHABLOCK )
//	{
//		info.pData = _fontAlphaData;
//		info.bAlloc = false;
//	}
//	else
	{
		info.pData = new XU8[ilen ];
		//info.pData = gTxtBmpData;
		info.bAlloc = true;
	}

	//unsigned char * pTmp = gTxtBmpData;
	//for( int h = 0; h < gTxtHeight; )

	memcpy( info.pData, gTxtBmpData, ilen );

	return true;
}


////////////////////////////////

//XFontDraw::XFontDraw()
//{
//	mFTLibrary = NULL;
//	mFTFace = NULL;
//	m_fontSize = 0;
//	m_fontID = 0;
//}
//
//XFontDraw::~XFontDraw()
//{
//	if( mFTFace != NULL )
//	{
//		FT_Done_Face( mFTFace );
//		FT_Done_FreeType( mFTLibrary );
//		m_fontSize = 0;
//		m_fontID = 0;
//	}
//}
//
//void XFontDraw::SetFont(const char* fontName,int id,int nSize)
//{
//	if( m_fontID == id && m_fontSize == nSize )
//		return;
//
//	if (mFTFace == 0 )
//	{
//		LOGWHERE();
//		if (FT_Init_FreeType( &mFTLibrary ))
//			return;
//		char chTmp[256] = {0};
//		sprintf( chTmp,"%s", "/system/fonts/DroidSansFallback.ttf");
//		if( access(chTmp, F_OK ) != 0 )
//		{
//			memset( chTmp, 0, 256);
//			sprintf( chTmp,"%s", "/system/fonts/Chinese-Hei.ttf");
//			if( access(chTmp, F_OK ) != 0 )
//			{
//				memset( chTmp, 0, 256);
//				sprintf( chTmp,"%s", "/system/fonts/AndroidClock.ttf");
//			}
//		}
//		//LOGWHEREMSG( chTmp );
//		if ( FT_New_Face( mFTLibrary, chTmp, 0, &mFTFace ) )
//		{
//			LOGWHERE();
//		}
//		FT_Select_Charmap(mFTFace, FT_ENCODING_UNICODE);
//		//FT_Set_Pixel_Sizes(mFTFace,0, 12);
//		InitFontBuffer( gFlashFonts );
//
//		m_fontID = id;
//		LOGWHERE();
//	}
//	if( m_fontSize != nSize )
//	{
//		m_fontSize = nSize;
//		FT_Set_Pixel_Sizes( mFTFace, 0, nSize );
//	}
//}
//
//bool XFontDraw::Draw(XPCWSTR str, int len,_FONTINFO&info)
//{
//	char *putf8 = unicodeToUtf8( str );
//	LOGE("%s, %d, %s, len:%d", __FUNCTION__, __LINE__, putf8, len );
//	delete []putf8;
//
//	char pTmp[128];
//
//	FT_UInt glyph_index;
//	FT_Error error;
//
//	int pos = 0;
//
//		pos = FindFontIndex( gFlashFonts, str[0] );
//		if( pos == -1 )
//		{
//			FT_GlyphSlot slot = mFTFace->glyph;
//			FT_Error error = FT_Load_Char( mFTFace, str[0], FT_LOAD_DEFAULT);
//			error = FT_Render_Glyph(mFTFace->glyph, FT_RENDER_MODE_NORMAL);
//			int delpos = DeleteOneFont( gFlashFonts );
//			FT_Bitmap_Done( mFTLibrary, gFlashFonts[delpos].bitmap );
//			pos = InsertFont( gFlashFonts, str[0], delpos );
//			FT_Bitmap_Copy( mFTLibrary, &mFTFace->glyph->bitmap, gFlashFonts[pos].bitmap );
//			gFlashFonts[pos].index = str[0];
//			gFlashFonts[pos].left = mFTFace->glyph->bitmap_left;
//			gFlashFonts[pos].top = mFTFace->glyph->bitmap_top;
//			gFlashFonts[pos].width = slot->advance.x >> 6;
//			gFlashFonts[pos].fontStyle = 0;
//		}
//		gFlashFonts[pos].usecount++;
//
//	FT_Bitmap *bitmap = gFlashFonts[pos].bitmap;
//	info.fw = bitmap->width;
//	int width = bitmap->width;// + 3) /4 * 4;
//	int height = bitmap->rows;
//	int posl = gFlashFonts[pos].left;
//	int post = gFlashFonts[pos].top;
//	sprintf( pTmp, "w:%d, h:%d, pos:%d, posl:%d, post:%d", width, height, pos, posl, post );
//	LOGWHEREMSG( pTmp );
//
//	if( width * height > _FONTBLOCK )
//	{
//		info.pData = new XU8[width * height];
//		info.bAlloc = true;
//	}
//	else
//	{
//		LOGWHERE();
//		info.pData = _fontData;
//		info.bAlloc = false;
//	}
//
//	XU8 * psrc = bitmap->buffer;
//	XU8 *pdst = info.pData;// + ( posl + post * width );
//	for(int j=0; j < height; j++)
//	{
//		XU8 *pixel = pdst;
//		//pdst += width;
//		//sprintf( pTmp, "--------->j:%d", j );
//		//LOGWHEREMSG( pTmp );
//		//memcpy( pdst, psrc, width );
//		pdst += width;
//		for(int i=0; i < width; i++)
//		{
//			//sprintf( pTmp, "i:%d, psrc:%d", i, *psrc );
//			//LOGWHEREMSG( pTmp );
//			if( *psrc )
//			{
//				*pixel = *psrc;
//			}
//			psrc++;
//			pixel++;
//		}
//	}
//	memset( info.pData, 0xFF, (width * height ));
//
//	info.w = width;
//	info.h = height;
//
//	return true;
//}


//char *putf8 = unicodeToUtf8( str );
//LOGE("%s, %d, len:%d, %s", __FUNCTION__, __LINE__, len, putf8 );
//delete []putf8;
//
//char pTmp[128];
//
//FT_UInt glyph_index;
//FT_Error error;
//
//int pos = 0;
//
//pos = FindFontIndex( gFlashFonts, str[0] );
//if( pos == -1 )
//{
//	FT_GlyphSlot slot = gFTFace->glyph;
//	FT_Error error = FT_Load_Char( gFTFace, str[0], FT_LOAD_DEFAULT);
//	error = FT_Render_Glyph(gFTFace->glyph, FT_RENDER_MODE_NORMAL);
//	int delpos = DeleteOneFont( gFlashFonts );
//	FT_Bitmap_Done( gFTLibrary, gFlashFonts[delpos].bitmap );
//	pos = InsertFont( gFlashFonts, str[0], delpos );
//	FT_Bitmap_Copy( gFTLibrary, &gFTFace->glyph->bitmap, gFlashFonts[pos].bitmap );
//	gFlashFonts[pos].index = str[0];
//	gFlashFonts[pos].left = gFTFace->glyph->bitmap_left;
//	gFlashFonts[pos].top = gFTFace->glyph->bitmap_top;
//	gFlashFonts[pos].width = slot->advance.x >> 6;
//	gFlashFonts[pos].fontStyle = 0;
//}
//gFlashFonts[pos].usecount++;
//
//int posx = gFlashFonts[pos].left;
//int posy = m_fontSize * 2 / 3 + 2 - gFlashFonts[pos].top;
//
//FT_Bitmap *bitmap = gFlashFonts[pos].bitmap;
//info.fw = bitmap->width;
//int width = bitmap->width;// + 3) /4 * 4;
//int height = bitmap->rows;
//
//sprintf( pTmp, "w:%d, h:%d, pos:%d, posx:%d, posy:%d", width, height, pos, posx, posy );
//LOGWHEREMSG( pTmp );
//
//if( width * height > _FONTBLOCK )
//{
//	info.pData = new XU8[width * height];
//	info.bAlloc = true;
//}
//else
//{
//	LOGWHERE();
//	info.pData = _fontData;
//	info.bAlloc = false;
//}
//
//XU8 * psrc = bitmap->buffer;
//XU8 *pdst = info.pData + (posx + posy * width);
//
//for(int j=0; j < height; j++)
//{
//	XU8 *pixel = pdst;
//	pdst += width;
//	for(int i=0; i < width; i++)
//	{
//		if( *psrc )
//		{
//			*pixel = *psrc;
//		}
//		psrc++;
//		pixel++;
//	}
//}
////memset( info.pData, 0xFF, (width * height ));
//
//info.w = width;
//info.h = height;
//
//return true;
