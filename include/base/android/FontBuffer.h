#ifndef __FONTBUFFER_H__
#define __FONTBUFFER_H__

#include "ft2build.h"
#include FT_FREETYPE_H
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftoutln.h"
#include "freetype/ftbitmap.h"
#include "GGType.h"

struct FontBuffer
{
public:
	FontBuffer()
	{
		index = 0;
		usecount = 0;
		bitmap = NULL;
		left = 0;
		top = 0;
		width = 0;
		fontStyle = 0;
	}
	void operator=( FontBuffer & src )
	{
		FT_Bitmap *tmp = bitmap;
		index = src.index;
		usecount = src.usecount;
		bitmap = src.bitmap;
		src.bitmap = tmp;
		left = src.left;
		top = src.top;
		width = src.width;
		fontStyle = src.fontStyle;
	}
public:
	int index;
	int usecount;
	FT_Bitmap *bitmap;
	int left;
	int top;
	int width;
	XU32 fontStyle;
};

#define MAX_FONTS 1024

//extern FontBuffer gFonts[MAX_FONTS];
//extern FontBuffer gFlashFonts[MAX_FONTS];

int InsertFont(FontBuffer*fonts, unsigned short charindex, int delpos );
bool InitFontBuffer(FontBuffer*fonts);
bool ReleaseFontBuffer(FontBuffer*fonts);
int DeleteOneFont(FontBuffer*fonts);
int FindFontIndex(FontBuffer*fonts, unsigned short charindex );


#endif
