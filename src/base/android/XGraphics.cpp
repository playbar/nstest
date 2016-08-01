// XGraphics.cpp: implementation of the XGraphics class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XGraphics.h"
#include "XString.h"
#include "string_comp.h"
#include "XResource.h"
#include "XWindow.h"

#ifdef __ANDROID__
#include "GlobalFun.h"
#include <jni.h>
#include <math.h>
#include "freetype/ftbitmap.h"
#include "FontBuffer.h"
#include "MultiStringUtil.h"
#include <unistd.h>
#endif

#define ISALPHA(CC) (CC<0x80 && isalpha(CC))
#define ISNUM(CC) (CC<0x80 && isdigit(CC))
#define ISMARK(CC) (wcschr((wchar_t*)XResource::LoadWString(XIDS_MARK),(wchar_t)CC)!=XNULL)
#define ISLEFTMARK(CC) (wcschr((wchar_t*)XResource::LoadWString(XIDS_LEFTMARK),(wchar_t)CC)!=XNULL)

#if 0
#define LOGWHEREGRAPHICS()  LOGWHERE()
#else
#define LOGWHEREGRAPHICS()
#endif

FontBuffer gFonts[MAX_FONTS];

//#define MAX_GLYPHS 65536
//
//struct FontGlyph
//{
//	//int index;
//	FT_Bitmap bitmap;
//	int left;
//	int top;
//	int width;
//	int unlinepos;
//	int textheight;
//};

//FontGlyph gGlyphs[ MAX_GLYPHS ] = {0};

//void CvRGBtoBGR( XU8 *data, int len )
//{
//	LOGWHEREGRAPHICS();
//	XU8 *pTmp = data;
//	for( int i = 0; i < len; i = i + 4 )
//	{
//		XU8 ch = pTmp[0];
//		pTmp[0] = pTmp[2];
//		pTmp[2] = ch;
//		pTmp = pTmp + 4;
//	}
//}

XGraphics::XGraphics()
{
	LOGWHEREGRAPHICS();
	Init();
}

XGraphics::~XGraphics()
{
	LOGWHEREGRAPHICS();
	Release();

	if( mFTFace != 0 )
	{
		FT_Done_Face(mFTFace);
		FT_Done_FreeType(mFTLibrary);
		m_nFontStyle &= ~XF_UPDATE;
		mFTFace = 0;
		mFTLibrary = 0;
		ReleaseFontBuffer( gFonts );
	}
}

#ifdef __ANDROID__

void XGraphics::SaveImage(char*pName )
{
	LOGWHEREGRAPHICS();
}

XImage *XGraphics::GetXImage( )
{
	LOGWHEREGRAPHICS();
	return m_pBitmap;
}

void XGraphics::Reset()
{
	LOGWHEREGRAPHICS();
	gdx2d_clear( m_pBitmap->GetImage(), m_nBackColor );
	return;
}
#endif

void XGraphics::DrawString(XPCWSTR pText, const XPoint &pt,XINT nLength)
{
	LOGWHEREGRAPHICS();
	if( pText == NULL  )
		return;
	if( nLength == -1 )
		nLength = gg_wcslen( pText );

	char *putf8 = unicodeToUtf8(pText);
	//LOGE("%s, %s, %d", __FUNCTION__, putf8, m_nTextColor );
	delete []putf8;

	GetFont();
	FTDrawString(pText, pt.x  + m_origin.x, pt.y + m_origin.y, nLength);
}

//implement origin and clip
void XGraphics::DrawString(XPCWSTR pText, const XRect &rect,XINT nLength)
{
	LOGWHEREGRAPHICS();
	if( pText == NULL  )
		return;
	if( nLength == -1 )
		nLength = gg_wcslen( pText );

//	char *putf8 = unicodeToUtf8(pText);
//	LOGE("%s, %s ", __FUNCTION__, putf8 );
//	delete []putf8;

	GetFont();
	FTDrawString(pText, rect.left  + m_origin.x,rect.top + m_origin.y, nLength);
}

//implement origin and clip
void XGraphics::DrawString(XPCWSTR pText, int x,int y,XINT nLength)
{
	LOGWHEREGRAPHICS();
	if( pText == NULL  )
		return;
	if( nLength == -1 )
		nLength = gg_wcslen( pText );

//	char *putf8 = unicodeToUtf8(pText);
//	LOGE("%s, %s ", __FUNCTION__, putf8 );
//	delete []putf8;

	GetFont();
	FTDrawString(pText, x + m_origin.x, y + m_origin.y, nLength);
}

void XGraphics::FTDrawString(XPCWSTR pText,XINT x,XINT y,XINT nLength )
{
//	char *putf8 = unicodeToUtf8(pText);
//	LOGE("%s, %d, %s, color=%0x", __FUNCTION__, __LINE__, putf8, m_nTextColor );
//	delete []putf8;

	LOGWHEREGRAPHICS();
	FT_UInt glyph_index;
	int pen_x = 0, pen_y = 0;
	pen_x = x;
	pen_y = y + m_nTextHeight * 2/3 + 2;
	FT_Error error;

	XU32 rgb = m_nTextColor & 0xFFFFFFFF;

	XU32 src_r = (m_nTextColor & 0xff000000) >> 24;
	XU32 src_g = (m_nTextColor & 0x00ff0000) >> 16;
	XU32 src_b = (m_nTextColor & 0x0000ff00) >> 8;
	XU32 src_a = (m_nTextColor & 0x000000ff);

	gdx2d_pixmap* pImg = m_pBitmap->GetImage();

	//gdx2d_set_blend(1);
	for(int i = 0; i < nLength; i++)
	{
		int pos = FindFontIndex(gFonts, pText[i] );
		if( pos == -1 )
		{
			FT_GlyphSlot slot = mFTFace->glyph;
			FT_Error error = FT_Load_Char( mFTFace, pText[i], FT_LOAD_DEFAULT);
			//if(m_nFontStyle & XF_BOLD)
			//{
			//	int strength = 1 << 6;
			//	FT_Outline_Embolden(&(mFTFace->glyph->outline), 1 << 6);
			//}
			error = FT_Render_Glyph(mFTFace->glyph, FT_RENDER_MODE_NORMAL);
			int delpos = DeleteOneFont( gFonts );
			FT_Bitmap_Done( mFTLibrary, gFonts[delpos].bitmap );
			pos = InsertFont( gFonts, pText[i], delpos );
			FT_Bitmap_Copy( mFTLibrary, &mFTFace->glyph->bitmap, gFonts[pos].bitmap );
			gFonts[pos].index = pText[i];
			gFonts[pos].left = mFTFace->glyph->bitmap_left;
			gFonts[pos].top = mFTFace->glyph->bitmap_top;
			gFonts[pos].width = slot->advance.x >> 6;
			gFonts[pos].fontStyle = m_nFontStyle;
		}
		else if( (m_nFontStyle & 0x0000ff00) != (gFonts[pos].fontStyle & 0x0000ff00 ) )
		{
			FT_GlyphSlot slot = mFTFace->glyph;
			FT_Error error = FT_Load_Char( mFTFace, pText[i], FT_LOAD_DEFAULT);
			//if(m_nFontStyle & XF_BOLD)
			//{
			//	int strength = 1 << 6;
			//	FT_Outline_Embolden(&(mFTFace->glyph->outline), 1 << 6);
			//}
			error = FT_Render_Glyph(mFTFace->glyph, FT_RENDER_MODE_NORMAL);
			FT_Bitmap_Done( mFTLibrary, gFonts[pos].bitmap );
			FT_Bitmap_Copy( mFTLibrary, &mFTFace->glyph->bitmap, gFonts[pos].bitmap );

			gFonts[pos].index = pText[i];
			gFonts[pos].left = mFTFace->glyph->bitmap_left;
			gFonts[pos].top = mFTFace->glyph->bitmap_top;
			gFonts[pos].width = slot->advance.x >> 6;
			gFonts[pos].fontStyle = m_nFontStyle;
		}
//		else if( (m_nFontStyle & XF_BOLD) && !(gFonts[pos].fontStyle & XF_BOLD) )
//		{
//			FT_GlyphSlot slot = mFTFace->glyph;
//			FT_Error error = FT_Load_Char( mFTFace, pText[i], FT_LOAD_DEFAULT);
//			//int strength = 1 << 6;
//			//FT_Outline_Embolden(&(mFTFace->glyph->outline), 1 << 6);
//			error = FT_Render_Glyph(mFTFace->glyph, FT_RENDER_MODE_NORMAL);
//			FT_Bitmap_Done( mFTLibrary, gFonts[pos].bitmap );
//			FT_Bitmap_Copy( mFTLibrary, &mFTFace->glyph->bitmap, gFonts[pos].bitmap );
//
//			gFonts[pos].index = pText[i];
//			gFonts[pos].left = mFTFace->glyph->bitmap_left;
//			gFonts[pos].top = mFTFace->glyph->bitmap_top;
//			gFonts[pos].width = slot->advance.x >> 6;
//			gFonts[pos].fontStyle = m_nFontStyle;
//		}
//		else if( !(m_nFontStyle & XF_BOLD) && (gFonts[pos].fontStyle & XF_BOLD) )
//		{
//			FT_GlyphSlot slot = mFTFace->glyph;
//			FT_Error error = FT_Load_Char( mFTFace, pText[i], FT_LOAD_DEFAULT);
//			error = FT_Render_Glyph(mFTFace->glyph, FT_RENDER_MODE_NORMAL);
//			FT_Bitmap_Done( mFTLibrary, gFonts[pos].bitmap );
//			FT_Bitmap_Copy( mFTLibrary, &mFTFace->glyph->bitmap, gFonts[pos].bitmap );
//
//			gFonts[pos].index = pText[i];
//			gFonts[pos].left = mFTFace->glyph->bitmap_left;
//			gFonts[pos].top = mFTFace->glyph->bitmap_top;
//			gFonts[pos].width = slot->advance.x >> 6;
//			gFonts[pos].fontStyle = m_nFontStyle;
//		}
		gFonts[pos].usecount++;

		//LOGWHEREVAL( pos );

		FT_Bitmap *bitmap = gFonts[pos].bitmap;
		int width = bitmap->width;
		int height = bitmap->rows;

		int posx = pen_x + gFonts[pos].left;
		int posy = pen_y - gFonts[pos].top;

		//LOGE("m_clip->left: %d, right: %d, top: %d, bottom: %d", m_clip.left, m_clip.right, m_clip.top, m_clip.bottom );

		if( posy < 0 )
		{
			height = height + posy;
			unsigned char * psrc = bitmap->buffer - posy * width;
			unsigned char * pdst = pImg->pixels + ( posx << 2 );
			for(int j=0; j < height; j++)
			{
				unsigned char *pixel = pdst;
				pdst += (pImg->width << 2);
				for(int i=0; i < width; i++)
				{
					if( posx + i >= m_clip.right )
					{
						psrc += (width - i);
						break;
					}
					if( posx + i < m_clip.left )
					{
						psrc++;
						pixel += 4;
						continue;
					}
					if( *psrc )
					{
						src_a = (*psrc);
						XU32 dst_r = pixel[0];
						XU32 dst_g = pixel[1];
						XU32 dst_b = pixel[2];
						//XU32 dst_a = pixel[3];

						dst_r = dst_r + (src_a * (src_r - dst_r) >> 8 );
						dst_g = dst_g + (src_a * (src_g - dst_g) >> 8 );
						dst_b = dst_b + (src_a * (src_b - dst_b) >> 8 );
						//dst_a = (int32_t)((1.0f - (1.0f - src_a / 255.0f) * (1.0f - dst_a / 255.0f)) * 255);

						pixel[0] = dst_r & 0xFF;
						pixel[1] = dst_g & 0xFF;
						pixel[2] = dst_b & 0xFF;
						//pixel[3] = src_a & 0xFF;
					}
					psrc++;
					pixel += 4;
				}  // end of width
			}  // end of height
		}  // end of if
		else if( m_clip.top > 0 && posy < m_clip.top )
		{
			height = height - ( m_clip.top -posy);
			unsigned char * psrc = bitmap->buffer + (m_clip.top -posy) * width;
			unsigned char * pdst = pImg->pixels + ((posx + m_clip.top * pImg->width) << 2);
			for(int j=0; j < height; j++)
			{
				if( posy + j >= m_clip.bottom )
				{
					break;
				}
				unsigned char *pixel = pdst;
				pdst += (pImg->width << 2);
				for(int i=0; i < width; i++)
				{
					if( posx + i >= m_clip.right )
					{
						psrc += (width - i);
						break;
					}
					if( posx + i < m_clip.left )
					{
						psrc++;
						pixel += 4;
						continue;
					}
					if( *psrc )
					{
						src_a = (*psrc);
						XU32 dst_r = pixel[0];
						XU32 dst_g = pixel[1];
						XU32 dst_b = pixel[2];
						//XU32 dst_a = pixel[3];

						dst_r = dst_r + (src_a * (src_r - dst_r) >> 8 );
						dst_g = dst_g + (src_a * (src_g - dst_g) >> 8 );
						dst_b = dst_b + (src_a * (src_b - dst_b) >> 8 );
						//dst_a = (int32_t)((1.0f - (1.0f - src_a / 255.0f) * (1.0f - dst_a / 255.0f)) * 255);

						pixel[0] = dst_r & 0xFF;
						pixel[1] = dst_g & 0xFF;
						pixel[2] = dst_b & 0xFF;
						//pixel[3] = src_a & 0xFF;
					}
					psrc++;
					pixel += 4;
				}  // end of width
			}  // end of height
		}  // end of if
		else
		{
			unsigned char * psrc = bitmap->buffer;
			unsigned char * pdst = pImg->pixels + ((posx + posy * pImg->width) << 2);
			for(int j=0; j < height; j++)
			{
				if( posy + j >= m_clip.bottom )
				{
					break;
				}
				unsigned char *pixel = pdst;
				pdst += (pImg->width << 2);
				for(int i=0; i < width; i++)
				{
					if( posx + i >= m_clip.right )
					{
						psrc += (width - i);
						break;
					}
					if( posx + i < m_clip.left )
					{
						psrc++;
						pixel += 4;
						continue;
					}

					if( *psrc )
					{
						src_a = (*psrc);
						XU32 dst_r = pixel[0];
						XU32 dst_g = pixel[1];
						XU32 dst_b = pixel[2];

						dst_r = dst_r + (src_a * (src_r - dst_r) >> 8 );
						dst_g = dst_g + (src_a * (src_g - dst_g) >> 8 );
						dst_b = dst_b + (src_a * (src_b - dst_b) >> 8 );
						//dst_a = (int32_t)((1.0f - (1.0f - src_a / 255.0f) * (1.0f - dst_a / 255.0f)) * 255);

						pixel[0] = dst_r & 0xFF;
						pixel[1] = dst_g & 0xFF;
						pixel[2] = dst_b & 0xFF;
					}
					psrc++;
					pixel += 4;
				}
			}

		}  // end of else
		pen_x += gFonts[pos].width;
	}
//	// underline
	if (m_nFontStyle & XF_UNDERLINE)
	{
		int iUnderlinePos = 0;
		if( FT_IS_SCALABLE(mFTFace) )
		{
			iUnderlinePos = FT_MulFix( mFTFace->underline_position , mFTFace->size->metrics.y_scale);
			iUnderlinePos >>= 6;
		}
		gdx2d_draw_line( m_pBitmap->GetImage(), x, pen_y - iUnderlinePos, pen_x, pen_y - iUnderlinePos, m_nTextColor );
	}
	//gdx2d_set_blend(0);
	return;
}


void XGraphics::MeasureString(XPCWSTR pText,XSIZE &size,XINT nLength)
{
	LOGWHEREGRAPHICS();
	if( nLength == -1 )
		nLength = gg_wcslen( pText );

//	char *putf8 = unicodeToUtf8(pText);
//	LOGE("%s, %s ", __FUNCTION__, putf8 );
//	delete []putf8;

	//LOGWHEREVAL( nLength );
	GetFont();
	if( nLength == 0 )
	{
		size.cx = 0;
		size.cy = m_nTextHeight + 4;
	}
	else
	{
		size.cx = 0;
		for( int i = 0; i < nLength; i++ )
		{
			int pos = FindFontIndex( gFonts, pText[i] );
			if( pos == -1 )
			{
				FT_GlyphSlot slot = mFTFace->glyph;
				FT_Error error = FT_Load_Char( mFTFace, pText[i], FT_LOAD_DEFAULT);
				//if(m_nFontStyle & XF_BOLD)
				//{
				//	int strength = 1 << 6;
				//	FT_Outline_Embolden(&(mFTFace->glyph->outline), 1 << 6);
				//}
				error = FT_Render_Glyph(mFTFace->glyph, FT_RENDER_MODE_NORMAL);
				int delpos = DeleteOneFont( gFonts );
				FT_Bitmap_Done( mFTLibrary, gFonts[delpos].bitmap );
				pos = InsertFont( gFonts, pText[i], delpos );
				FT_Bitmap_Copy( mFTLibrary, &mFTFace->glyph->bitmap, gFonts[pos].bitmap );
				gFonts[pos].index = pText[i];
				gFonts[pos].left = mFTFace->glyph->bitmap_left;
				gFonts[pos].top = mFTFace->glyph->bitmap_top;
				gFonts[pos].width = slot->advance.x >> 6;
				gFonts[pos].fontStyle = m_nFontStyle;
			}
			else if( (m_nFontStyle & 0x0000ff00) != (gFonts[pos].fontStyle & 0x0000ff00 ) )
			{
				FT_GlyphSlot slot = mFTFace->glyph;
				FT_Error error = FT_Load_Char( mFTFace, pText[i], FT_LOAD_DEFAULT);
				//if(m_nFontStyle & XF_BOLD)
				//{
				//	int strength = 1 << 6;
				//	FT_Outline_Embolden(&(mFTFace->glyph->outline), 1 << 6);
				//}
				error = FT_Render_Glyph(mFTFace->glyph, FT_RENDER_MODE_NORMAL);
				FT_Bitmap_Done( mFTLibrary, gFonts[pos].bitmap );
				FT_Bitmap_Copy( mFTLibrary, &mFTFace->glyph->bitmap, gFonts[pos].bitmap );

				gFonts[pos].index = pText[i];
				gFonts[pos].left = mFTFace->glyph->bitmap_left;
				gFonts[pos].top = mFTFace->glyph->bitmap_top;
				gFonts[pos].width = slot->advance.x >> 6;
				gFonts[pos].fontStyle = m_nFontStyle;
			}
//			else if( (m_nFontStyle & XF_BOLD) && !(gFonts[pos].fontStyle & XF_BOLD) )
//			{
//				FT_GlyphSlot slot = mFTFace->glyph;
//				FT_Error error = FT_Load_Char( mFTFace, pText[i], FT_LOAD_DEFAULT);
//				//int strength = 1 << 6;
//				//FT_Outline_Embolden(&(mFTFace->glyph->outline), 1 << 6);
//				error = FT_Render_Glyph(mFTFace->glyph, FT_RENDER_MODE_NORMAL);
//				FT_Bitmap_Done( mFTLibrary, gFonts[pos].bitmap );
//				FT_Bitmap_Copy( mFTLibrary, &mFTFace->glyph->bitmap, gFonts[pos].bitmap );
//
//				gFonts[pos].index = pText[i];
//				gFonts[pos].left = mFTFace->glyph->bitmap_left;
//				gFonts[pos].top = mFTFace->glyph->bitmap_top;
//				gFonts[pos].width = slot->advance.x >> 6;
//				gFonts[pos].fontStyle = m_nFontStyle;
//			}
//			else if( !(m_nFontStyle & XF_BOLD) && (gFonts[pos].fontStyle & XF_BOLD) )
//			{
//				FT_GlyphSlot slot = mFTFace->glyph;
//				FT_Error error = FT_Load_Char( mFTFace, pText[i], FT_LOAD_DEFAULT);
//				error = FT_Render_Glyph(mFTFace->glyph, FT_RENDER_MODE_NORMAL);
//				FT_Bitmap_Done( mFTLibrary, gFonts[pos].bitmap );
//				FT_Bitmap_Copy( mFTLibrary, &mFTFace->glyph->bitmap, gFonts[pos].bitmap );
//
//				gFonts[pos].index = pText[i];
//				gFonts[pos].left = mFTFace->glyph->bitmap_left;
//				gFonts[pos].top = mFTFace->glyph->bitmap_top;
//				gFonts[pos].width = slot->advance.x >> 6;
//				gFonts[pos].fontStyle = m_nFontStyle;
//			}
			gFonts[pos].usecount++;
			size.cx += gFonts[pos].width;
		}
		size.cy = m_nTextHeight + 4;
	}
	//LOGWHEREVAL(size.cx);
	return;
}

int XGraphics::MeasureChar( unsigned short ch )
{
	LOGWHEREGRAPHICS();
	//return 16;

	FT_GlyphSlot slot = mFTFace->glyph;
	FT_Error error = FT_Load_Char( mFTFace, ch, FT_LOAD_RENDER | FT_LOAD_TARGET_LIGHT );
	if( error )
		return m_nTextHeight + 4;
	else
		return slot->advance.x >> 6;

}

//implement origin and clip
void XGraphics::FillRectangle(const XRect &rect)
{
	LOGWHEREGRAPHICS();
	XU32 alpha = (m_nBackColor & 0xff);
	if( alpha == 0 )
		return;

	XRect drt(rect);
	drt += m_origin;
	drt &= m_clip;

	if( drt.IsRectEmpty() )
		return;

	//LOGE("drt:left: %d, right: %d, top: %d, bottom: %d",
	//		drt.left, drt.right, drt.top, drt.bottom );
	//gdx2d_fill_rect( m_pBitmap->GetImage(), drt.left, drt.top,
	//		drt.Width(), drt.Height(), m_nBackColor);
	//return;

	XU32 color = ((m_nBackColor & 0xff000000) >> 24) |
			((m_nBackColor & 0xff0000) >> 8) |
			((m_nBackColor & 0xff00) << 8) |
			((m_nBackColor & 0xff) << 24);

	gdx2d_pixmap* pImg = m_pBitmap->GetImage();
	unsigned char * pdst = pImg->pixels + (drt.top * pImg->width  << 2);
	unsigned char * pix = pdst + (drt.left << 2 );
	unsigned char * pixtmp = pix;
	unsigned char * pcolor = (unsigned char *)&color;
	for( int i = drt.left; i < drt.right; i++ )
	{
		memcpy( pixtmp, pcolor, 4 );
		pixtmp += 4;
	}

	pixtmp += ((pImg->width - drt.Width()) << 2);

	int iwidth = drt.Width() * 4;
	for( int j = drt.top + 1; j < drt.bottom; j++ )
	{
		//pixtmp = pix + ((drt.top - j + 1 ) * pImg->width  << 2);
		memcpy( pixtmp, pix, iwidth );
		pixtmp += (pImg->width << 2);
	}
	//LOGWHEREGRAPHICS();

	return;
}

void XGraphics::DrawImage(const XImage &img, const XRect&dstRect, const XRect &srcRect )
{
	LOGWHEREGRAPHICS();
	if( m_pBitmap == NULL )
		return;

	float xd = (float)dstRect.Width() / (float)srcRect.Width();
	float yd = (float)dstRect.Height() / (float)srcRect.Height();
	XRect drt(dstRect);
	drt+=m_origin;
	drt&=m_clip;

	if( drt.IsRectEmpty() )
		return;
	int td,bd,ld,rd;
	XRect temp(dstRect);
	temp += m_origin;
	td = drt.top - temp.top;
	bd = temp.bottom - drt.bottom;
	ld = drt.left - temp.left;
	rd = temp.right - drt.right;

	XRect srt(srcRect);
	if(xd>1.0001 && xd<0.9999)
	{
		srt.left += ld*xd;
		srt.right -= rd*xd;
	}
	else
	{
		srt.left += ld;
		srt.right -= rd;
	}
	if(yd>1.0001 && yd<0.9999)
	{
		srt.top += td*yd;
		srt.bottom -= bd*yd;
	}
	else
	{
		srt.top += td;
		srt.bottom -= bd;
	}

//	LOGE("m_origin->x: %d, y: %d", m_origin.x, m_origin.y );
//	LOGE("dstRect->left: %d, right: %d, top: %d, bottom: %d", dstRect.left, dstRect.right, dstRect.top, dstRect.bottom );
//	LOGE("srcRect->left: %d, right: %d, top: %d, bottom: %d", srcRect.left, srcRect.right, srcRect.top, srcRect.bottom );
//	LOGE("m_clip->left: %d, right: %d, top: %d, bottom: %d", m_clip.left, m_clip.right, m_clip.top, m_clip.bottom );
//	LOGE("srt->left: %d, right: %d, top: %d, bottom: %d", srt.left, srt.right, srt.top, srt.bottom );
//	LOGE("drt->left: %d, right: %d, top: %d, bottom: %d", drt.left, drt.right, drt.top, drt.bottom );

	/////
	gdx2d_pixmap* pImgDst = m_pBitmap->GetImage();
	const gdx2d_pixmap* pImgSrc = &img.mImgData;
	DrawImage( pImgDst, pImgSrc, drt, srt );

	return;
}

//implement origin and clip
void XGraphics::DrawImage(const XImage &img,const XRect &rect)
{
	LOGWHEREGRAPHICS();
	XRect src;
	src.left = 0;
	src.right = img.Width();
	src.top = 0;
	src.bottom = img.Height();
	XRect dst;
	dst.left = rect.left;
	dst.right = rect.right;
	dst.top = rect.top;
	dst.bottom = rect.bottom;
	DrawImage( img, dst, src );
}

void XGraphics::DrawImage( gdx2d_pixmap* pImgDst, const gdx2d_pixmap* pImgSrc,
		const XRect & drt, const XRect &srt )
{
	unsigned char * pdst = pImgDst->pixels;
	unsigned char * psrc = pImgSrc->pixels;

	int dstx = drt.left;
	int dstx1 = drt.right;
	int dsty = drt.top;
	int dsty1 = drt.bottom;

	int srcx = srt.left;
	int srcx1 = srt.right;
	int srcy = srt.top;
	int srcy1 = srt.bottom;

	//LOGE("srt->left: %d, right: %d, top: %d, bottom: %d", srt.left, srt.right, srt.top, srt.bottom );
	//LOGE("drt->left: %d, right: %d, top: %d, bottom: %d", drt.left, drt.right, drt.top, drt.bottom );

	if( abs(drt.Width() - srt.Width()) < 5 && abs(drt.Height() - srt.Height()) < 5 )
	{
		pdst = pImgDst->pixels + ((dsty * pImgDst->width + dstx ) << 2);
		psrc = pImgSrc->pixels + ((srcy * pImgSrc->width + srcx ) << 2);
		int ilen = srt.Width() << 2;
		int srcH = pImgSrc->height - srcy;
		int srcW = pImgSrc->width - srcx;
		for( int j = dsty; j < dsty1; j++ )
		{
			if( j < pImgDst->height && (j - dsty) < srcH )
			{
				unsigned char * psrcTmp = psrc;
				unsigned char * pdstTmp = pdst;
				for( int i = dstx; i < dstx1; i++ )
				{
					if( i > pImgDst->width || ( i - dstx) > srcW )
						continue;
					if( psrcTmp[3] < 100 )
					{
						pdstTmp += 4;
						psrcTmp += 4;
						continue;
					}
					memcpy( pdstTmp, psrcTmp, 4 );
					pdstTmp += 4;
					psrcTmp += 4;
				}
			}
			else
			{
				break;
			}
			pdst += ( pImgDst->width << 2 );
			psrc += ( pImgSrc->width << 2 );
		}
	}
	else	//缩放
	{
		float rateW = (float)srt.Width() / (float)drt.Width();
		float rateH = (float)srt.Height() / (float)drt.Height();

		for( int j = dsty; j < dsty1; j++ )
		{
			int tSrcH = (int)(rateH * (j - dsty) + 0.5 + srcy );
			psrc = pImgSrc->pixels + ((tSrcH * pImgSrc->width + srcx ) << 2);
			pdst = pImgDst->pixels + ((j * pImgDst->width + dstx ) << 2);
			//LOGE( "tSrcH   = %d" , tSrcH );
			if( tSrcH < 0 || j < 0 )
				continue;
			if( tSrcH >= pImgSrc->height || j >= pImgDst->height )
				break;

			//psrc +=  ((srcy * pImgSrc->width + srt.left)  << 2);
			unsigned char * psrcTmp = psrc;
			unsigned char * pdstTmp = pdst;
			for( int i = dstx; i < dstx1; i++ )
			{
				int tSrcW = (int)(rateW * (i - dstx) + 0.5 + srcx );
				//LOGE( "tSrcW = %d" , tSrcW );
				if( i < 0 || tSrcW < 0  )
					continue;
				if( tSrcW >= pImgSrc->width || i >= pImgDst->width )
					break;

				if( psrcTmp[3] == 0 )
				{
					psrcTmp = psrc + ( tSrcW << 2 );
					pdst += 4;
					continue;
				}
				memcpy( pdst, psrcTmp, 4 );
				//psrcTmp += 4;
				psrcTmp = psrc + ( tSrcW << 2 );
				pdst += 4;
			}
			//pdst += ( pImgDst->width << 2 );
		}
	}
}

//implement origin and clip
void XGraphics::DrawImage(const XImage&img,const XPoint&dst,const XRect&rst)
{
	LOGWHEREGRAPHICS();

	XRect dstRect;
	dstRect.left = dst.x;
	dstRect.right = dst.x + rst.Width();
	dstRect.top = dst.y;
	dstRect.bottom = dst.y + rst.Height();
	DrawImage( img, dstRect, rst );
}

//implement origin and clip
void XGraphics::DrawImage(const XImage &img,const XPoint &point,int rotate)
{
	LOGWHEREGRAPHICS();
	XRect dst;
	XRect src;
	dst.left = point.x;
	dst.right = point.x + img.Width();
	dst.top = point.y;
	dst.bottom = point.y + img.Height();

	src.left = 0;
	src.right = img.Width();
	src.top = 0;
	src.bottom = img.Height();
	DrawImage( img, dst, src );
}

//implement origin and clip
void XGraphics::FrameRectangle(const XRect &rect)
{
	LOGWHEREGRAPHICS();
	XRect tdrt(rect);
	tdrt += m_origin;
	XRect drt( tdrt );
	tdrt &= m_clip;

	if( tdrt.IsRectEmpty() )
			return;

	if(tdrt == XRect(0,0,0,0))
		return;
	if(tdrt != drt)
	{
		int sx,sy,ex,ey;
		if(drt.top == tdrt.top)
			gdx2d_draw_line(m_pBitmap->GetImage(),tdrt.left,tdrt.top,tdrt.right,tdrt.top,m_nTextColor);
		if(drt.left == tdrt.left)
			gdx2d_draw_line(m_pBitmap->GetImage(),tdrt.left,tdrt.top,tdrt.left,tdrt.bottom,m_nTextColor);
		if(drt.right == tdrt.right)
			gdx2d_draw_line(m_pBitmap->GetImage(),tdrt.right,tdrt.top,tdrt.right,tdrt.bottom,m_nTextColor);
		if(drt.bottom == tdrt.bottom)
			gdx2d_draw_line(m_pBitmap->GetImage(),tdrt.left,tdrt.bottom,tdrt.right,tdrt.bottom,m_nTextColor);
	}
	else
		gdx2d_draw_rect( m_pBitmap->GetImage(),
				tdrt.left, tdrt.top, tdrt.Width(), tdrt.Height(), m_nTextColor);
	return;
}

//implement origin and clip
void XGraphics::DrawLine(int sx, int sy, int ex, int ey,XU8 w,XU8 nStyle)
{
	LOGWHEREGRAPHICS();
	sx+=m_origin.x;
	sy+=m_origin.y;
	ex+=m_origin.x;
	ey+=m_origin.y;

	if( m_clip.PtInRect( sx, sy ) && m_clip.PtInRect( ex, ey ))
	{
		gdx2d_draw_line( m_pBitmap->GetImage(), sx, sy, ex, ey, m_nTextColor );
	}
	else
	{
		float fx0 = 0.0f;
		float fy0 = 0.0f;
		float fx1 = 0.0f;
		float fy1 = 0.0f;
		float ix = 0.0f;
		float iy = 0.0f;
		int count = 0;
		if( TwoLineIsIntersect( sx, sy, ex, ey, m_clip.left, m_clip.top,
				m_clip.right, m_clip.top, ix, iy ))
		{
			count++;
			fx0 = ix;
			fy0 = iy;
		}
		if( TwoLineIsIntersect( sx, sy, ex, ey, m_clip.left, m_clip.top,
				m_clip.left, m_clip.bottom, ix, iy ))
		{
			count++;
			if( count == 1 )
			{
				fx0 = ix;
				fy0 = iy;
			}
			else
			{
				fx1 = ix;
				fy1 = iy;
			}
		}
		if( count < 2 && TwoLineIsIntersect( sx, sy, ex, ey, m_clip.right, m_clip.top,
						m_clip.right, m_clip.bottom, ix, iy ))
		{
			count++;
			if( count == 1 )
			{
				fx0 = ix;
				fy0 = iy;
			}
			else
			{
				fx1 = ix;
				fy1 = iy;
			}
		}
		if( count < 2 && TwoLineIsIntersect( sx, sy, ex, ey, m_clip.left, m_clip.bottom,
								m_clip.right, m_clip.bottom, ix, iy ))
		{
			count++;
			if( count == 1 )
			{
				fx0 = ix;
				fy0 = iy;
			}
			else
			{
				fx1 = ix;
				fy1 = iy;
			}
		}
		if( m_clip.PtInRect( sx, sy ) )
		{
			ex = fx0;
			ey = fy0;
			gdx2d_draw_line( m_pBitmap->GetImage(), sx, sy, ex, ey, m_nTextColor );
		}
		else if( m_clip.PtInRect( ex, ey ))
		{
			sx = fx0;
			sy = fy0;
			gdx2d_draw_line( m_pBitmap->GetImage(), sx, sy, ex, ey, m_nTextColor );
		}
		else if( (fx0 > 0.5 || fy0 > 0.5) && (fx1 > 0.5 || fy1 > 0.5) )
		{
			sx = fx0;
			sy = fy0;
			ex = fx1;
			ey = fy1;
			gdx2d_draw_line( m_pBitmap->GetImage(), sx, sy, ex, ey, m_nTextColor );
		}
	}

	//需要调整
	//gdx2d_draw_line( m_pBitmap->GetImage(), sx, sy, ex, ey, m_nTextColor );
}

void XGraphics::DrawButton(const XRect &r, XU32 c,XU8 b)
{
	LOGWHEREGRAPHICS();
	XRect rect( r );
	XColor cr(c);
	if(!(b&XBS_NOFILL))
	{
		SetBackColor(cr);
		FillRectangle(rect);
	}
	if(b&XBS_WFRAME)
	{
	   XColor cc(c);
	   cc.Bright(50);
	   SetColor(cc);
	   FrameRectangle(rect);
	   rect.DeflateRect(1,1);
	}
	if(b&XBS_BFRAME)
	{
	   cr.Dark(30);
	   SetColor(cr);
	   rect.bottom--;
	   rect.right--;
	   DrawLine(rect.left+1,rect.top,rect.right-1,rect.top);
	   DrawLine(rect.left,rect.top+1,rect.left,rect.bottom-1);
	   cr.Dark(20);
	   SetColor(cr);
	   DrawLine(rect.left+1,rect.bottom,rect.right-1,rect.bottom);
	   DrawLine(rect.right,rect.top+1,rect.right,rect.bottom-1);
	   rect.DeflateRect(1,1);
	}
	else
	{
	   rect.right--;
	   rect.bottom--;
	}
	cr=c;
	cr.Bright(20);
	SetColor(cr);

	int cx=rect.Width(),cy=rect.Height();
	DrawLine(rect.left+1,rect.top,rect.left+(cx>>1),rect.top);
	DrawLine(rect.left,rect.top+1,rect.left,rect.top+(cy>>1));
	cr=c;
	cr.Bright(16);
	SetColor(cr);
	DrawLine(rect.left+(cx>>1)+1,rect.top,rect.right-1,rect.top);
	DrawLine(rect.left,rect.top+(cy>>1)+1,rect.left,rect.bottom-1);

   for(int i=0;i<3;i++)
   {
	  if(b&XBS_HHAND)
		DrawLine(rect.left+2,rect.top+(cy>>1)-3+i*3,rect.right-3,rect.top+(cy>>1)-3+i*3);
	  else if(b&XBS_VHAND)
		DrawLine(rect.left+(cx>>1)-3+i*3,rect.top+2,rect.left+(cx>>1)-3+i*3,rect.bottom-3);
   }

	if(b&XBS_THICK)
	{
		DrawLine(rect.left+2,rect.top+1,rect.left+(cx/3),rect.top+1);
		DrawLine(rect.left+1,rect.top+2,rect.left+1,rect.top+(cy/3));
		cr=c;
		cr.Bright(10);
		SetColor(cr);
		DrawLine(rect.left+(cx/3)+1,rect.top+1,rect.right-2,rect.top+1);
		DrawLine(rect.left+1,rect.top+(cy/3)+1,rect.left+1,rect.bottom-2);
	}
	cr=c;
	cr.Dark(24);
	SetColor(cr);
	DrawLine(rect.left+(cx>>1),rect.bottom,rect.right-1,rect.bottom);
	DrawLine(rect.right,rect.top+(cy>>1),rect.right,rect.bottom-1);

   for(int i=0;i<3;i++)
   {
	  if(b&XBS_HHAND)
		DrawLine(rect.left+3,rect.top+(cy>>1)-2+i*3,rect.right-2,rect.top+(cy>>1)-2+i*3);
	  else if(b&XBS_VHAND)
		DrawLine(rect.left+(cx>>1)-2+i*3,rect.top+3,rect.left+(cx>>1)-2+i*3,rect.bottom-2);
   }

	cr=c;
	cr.Dark(16);
	SetColor(cr);
	DrawLine(rect.left+1,rect.bottom,rect.left+(cx>>1)-1,rect.bottom);
	DrawLine(rect.right,rect.top+1,rect.right,rect.top+(cy>>1)-1);

	if(b&XBS_THICK)
	{
		 DrawLine(rect.right-(cx/3),rect.bottom-1,rect.right-2,rect.bottom-1);
		 DrawLine(rect.right-1,rect.bottom-(cy/3),rect.right-1,rect.bottom-2);
		 cr=c;
		 cr.Dark(8);
		 SetColor(cr);
		 DrawLine(rect.left+2,rect.bottom-1,rect.right-(cx/3)-1,rect.bottom-1);
		 DrawLine(rect.right-1,rect.top+2,rect.right-1,rect.bottom-(cy/3)-1);
	}
}

void XGraphics::SetClip(const XRect &rect)
{
	LOGWHEREGRAPHICS();
	m_clip = rect +  m_origin;
	return;
}

void XGraphics::GetClip(XRect &clip)
{
	LOGWHEREGRAPHICS();
	clip = m_clip - m_origin;
}
//void XGraphics::GetColor(){
//	LOGWHEREGRAPHICS();
//		//LOGE("line:%d, color:%0x", __LINE__, nColor);
//		XU32 c;
//		c=m_nTextColor << 8 | m_nTextColor >> 24;
//		//m_nTextColor=nColor << 8 ;
//		//m_nTextColor = nColor;
//		LOGWHEREVAL(m_nTextColor);
//		//LOGE("line:%d, color:%0x", __LINE__, m_nTextColor);
//}

void XGraphics::SetOrigin(XINT x, XINT y)
{
	LOGWHEREGRAPHICS();
	m_clip-=m_origin;
	m_origin.x = x;
	m_origin.y = y;
	m_clip+=m_origin;
   return;
}

void XGraphics::GetOrigin(XINT&x,XINT&y)
{
	LOGWHEREGRAPHICS();
	x = m_origin.x;
	y = m_origin.y;
	return;
	//TODO by hgl
}

void XGraphics::DrawDirect(const XRect&rect,XU8 nDirect, XU32 c,XU8 bGray)
{
	LOGWHEREGRAPHICS();
	XColor cc(c);
	cc.Bright(bGray?50:70);
	SetColor(cc);

	XRect drt(drt);
	drt+=m_origin;
	drt&=m_clip;

	int dd=(drt.Width()-8)>>1;
	int bx=drt.left+4,by=drt.bottom-4;
	switch(nDirect)
	{
	case XK_UP:
		 DrawLine(bx,by-1,bx+dd,by-dd-1);
		 DrawLine(bx+dd,by-dd-1,bx+(dd<<1),by-1);
		 break;
	case XK_DOWN:
		 DrawLine(bx,by-dd,bx+dd,by);
		 DrawLine(bx+dd,by,bx+(dd<<1),by-dd);
		 break;
	case XK_LEFT:
		 DrawLine(bx+dd+2,by-(dd<<1),bx+2,by-dd);
		 DrawLine(bx+2,by-dd,bx+dd+2,by);
		 break;
	case XK_RIGHT:
		 DrawLine(bx+3,by-(dd<<1),bx+3+dd,by-dd);
		 DrawLine(bx+3+dd,by-dd,bx+3,by);
	}
	cc=c;

	if(bGray) cc.Bright(20);
	SetColor(cc);
	for(int i=1;i<3;i++)
	{
	switch(nDirect)
	{
		case XK_UP:
			 DrawLine(bx,by-1-i,bx+dd,by-dd-1-i);
			 DrawLine(bx+dd,by-dd-1-i,bx+(dd<<1),by-1-i);
			 break;
		case XK_DOWN:
			 DrawLine(bx,by-dd-i,bx+dd,by-i);
			 DrawLine(bx+dd,by-i,bx+(dd<<1),by-dd-i);
			 break;
		case XK_LEFT:
			 DrawLine(bx+dd+2-i,by-(dd<<1),bx+2-i,by-dd);
			 DrawLine(bx+2-i,by-dd,bx+dd+2-i,by);
			 break;
		case XK_RIGHT:
			 DrawLine(bx+3-i,by-(dd<<1),bx+3+dd-i,by-dd);
			 DrawLine(bx+3-i+dd,by-dd,bx+3-i,by);
		}
	}
}

void XGraphics::DrawCheck(const XRect &rect, XU32 c,XU32 c1)
{
	LOGWHEREGRAPHICS();

	XRect drt(rect);
	drt+=m_origin;
	drt&=m_clip;

	int dd=drt.Width()/3;

	for(int i=0;i<3;i++)
	{

		if(i!=1)
			SetColor(c1);
		else SetColor(c);
		DrawLine(drt.left,drt.bottom-dd-i,drt.left+dd,drt.bottom-i);
		DrawLine(drt.left+dd,drt.bottom-i,drt.left+dd*3,drt.bottom-i-dd*2);
	}
}

XU16 XGraphics::FormatString(XPCWSTR pData,int len, int bx, int win, DOMPOSARRAY &list,
							XU8 bWord,DOMPOS*pSpan)
{
	LOGWHEREGRAPHICS();
//	char *putf8 = unicodeToUtf8(pData);
//	LOGE("%s, %s ", __FUNCTION__, putf8 );
//	delete []putf8;

	XSIZE sz;
	sz.cx=0;
	sz.cy=0;
	XWCHAR pTmpData[100] = {0};

	XU16 iStrLen=(XU16)(XMIN(len,0xfffe));
	bool bStart = (bx==0);

	int ilineWin = win;
	int ssy = 0;
	if(pSpan!=XNULL && ssy<pSpan[3])
	{
		ilineWin = pSpan[2];
	}
	XU16 iBegin=0;
	XU16 istep = XMIN(XMAX(2+((ilineWin-bx)>>5),2),iStrLen);
	XU16 ilinend = 0;
	XU16 mi = istep;
	XU16 ci = 0;
	while(iBegin<iStrLen)
	{
		memcpy( pTmpData, pData, istep* 2 );
		MeasureString(pTmpData,sz,istep);
		memset( pTmpData, 0, 200 );

		ci=istep;
		while(sz.cx+bx<=ilineWin)
		{
			ilinend=istep;
			if( istep + iBegin >= iStrLen)
				break;
			istep = XMIN(istep+mi,iStrLen-iBegin);
			memcpy( pTmpData, pData, istep* 2 );
			MeasureString(pTmpData,sz,istep);
			memset( pTmpData, 0, 200 );
			ci = istep;
		}

		mi=(istep + ilinend) >> 1;
		while(istep > ilinend+1)
		{
			memcpy( pTmpData, pData, mi* 2 );
			MeasureString(pTmpData,sz,mi);
			memset( pTmpData, 0, 200 );
			ci=mi;
			if(sz.cx+bx>ilineWin)
			{
				istep=mi;
			}
			else
			{
				ilinend=mi;
			}
			mi=(istep+ilinend)>>1;
		}
		istep=mi;
		if(XFALSE)
		{
			if(ISALPHA(pData[ilinend-1]))
			{
			  mi=ilinend;
			  while(mi>0&&(ISALPHA(pData[mi-1])||ISLEFTMARK(pData[mi-1])))
				  mi--;
			  if(mi>0)
				  ilinend=mi;
			}
		   else if(ISNUM(pData[ilinend-1]))
		   {
			  mi=ilinend;
			  while(mi>1&&(ISNUM(pData[mi-1])||ISLEFTMARK(pData[mi-1])))
				  mi--;
			  if(mi>1)
				  ilinend=mi;
		   }
		   else if(ilinend+iBegin<iStrLen&&ISMARK(pData[ilinend]))
		   {
			   if(ilinend+1+iBegin>=iStrLen || !ISMARK(pData[ilinend+1]))
				   ilinend++;
		   }
		   else if(ilinend>2&&ISLEFTMARK(pData[ilinend-1]))
			   ilinend--;
		}

		if(ilinend==0&&bStart)
			ilinend++;
		if(ilinend!=ci)
		{
			memcpy( pTmpData, pData, ilinend* 2 );
			MeasureString(pTmpData,sz,ilinend);
			memset( pTmpData, 0, 200 );
		}
		list.Add(iBegin);
		list.Add(ilinend);
		list.Add(ilinend>0?sz.cx:0);

		bStart=XTRUE;

		if(pSpan!=XNULL)
		{
		   ssy+=sz.cy;
		   if(ssy<pSpan[3])
			   ilineWin=pSpan[2];
		   else
			   ilineWin=win;
		   if(ssy<pSpan[1])
			   bx=pSpan[0];
		   else
			   bx=0;
		}
		else
			bx=0;
		bStart=XTRUE;
		iBegin+=ilinend;
		pData+=ilinend;
		istep=XMIN(2+((XMAX(ilineWin-bx,0))>>5),iStrLen-iBegin);
		mi=istep;
		ilinend=0;
		if((bWord&XFMT_ONELINE)&&list.GetSize()>0)
		{
			return (XU16)sz.cy;
		}
	}
	return (XU16)sz.cy;

}

void XGraphics::DrawFrame(const XRect &rect, XU32 c, XU8 s,XU8 bSet)
{
	LOGWHEREGRAPHICS();
	XRect r(rect);
	r += m_origin;
	r &= m_clip;
	for(XU8 i=0;i<s;i++)
	{
		int v=(i+1)*40/s;
		XColor cc=c;
		if(bSet)
			cc.Bright(50-v);
		else
			cc.Dark(v+10);
		SetColor(cc);
		DrawLine(r.left,r.top,r.right,r.top);
		DrawLine(r.left,r.top,r.left,r.bottom);
		cc=c;
		if(bSet)
			cc.Dark(v+10);
		else
			cc.Bright(50-v);
		SetColor(cc);
		DrawLine(r.right,r.top,r.right,r.bottom);
		DrawLine(r.left,r.bottom,r.right,r.bottom);
		r.InflateRect(-1,-1);
	}
}

void XGraphics::DrawBar(const XRect &rect, XU32 c, XU8 dc,XU8 bHorz)
{
	LOGWHEREGRAPHICS();
	XColor cc(c);
	if(bHorz)
	{
		int my=((rect.bottom-rect.top+1)>>1);
		for(int y=0;y<my;y++)
		{
		   SetColor(cc);
		   cc.Bright(dc);
		   DrawLine(rect.left,rect.top+y,rect.right,rect.top+y);
		   DrawLine(rect.left,rect.bottom-y-1,rect.right,rect.bottom-y-1);
		}
	}
}

XBOOL XGraphics::Scroll(const XPoint &dst, const XRect &rst)
{
	LOGWHEREGRAPHICS();

	return XTRUE;
}


void XGraphics::DrawRadio(const XRect &inRect, XU32 nColor,XU8 bSelect)
{
	LOGWHEREGRAPHICS();

	XRect rect = inRect;
	rect.DeflateRect(1,1);

	rect.right++;
	rect.bottom++;
	XColor wc(nColor);
	wc.Bright(20);
	SetBackColor(wc);
	FillEllipse(rect);

	rect-=XPoint(1,1);
	XColor gc(nColor);
	gc.Dark(30);
	SetBackColor(gc);
	FillEllipse(rect);

	rect.DeflateRect(1,1);
	XColor dc(gc);
	dc.Dark(50);
	SetBackColor(dc);
	FillEllipse(rect);

	rect.left++;
	rect.top++;
	SetBackColor(wc);
	FillEllipse(rect);

	if(bSelect)
	{
	  rect.DeflateRect(1,1);
	  SetBackColor(gc);
	  FillEllipse(rect);
	  SetBackColor(dc);
	  rect.DeflateRect(1,1);
	  FillEllipse(rect);
	}
}

void XGraphics::DrawCheckBox(const XRect &inRect, XU32 c, XU8 bCheck)
{
	LOGWHEREGRAPHICS();

	XRect rect = inRect;
	 XColor cc(c);
	 cc.Bright(30);
	 SetBackColor(cc);
	 FillRectangle(rect);
	 XColor cg(c);
	 cg.Dark(20);
	 DrawFrame(rect,cg,2);
	 if(bCheck)
	 {
		 rect.DeflateRect(2,2);
		 //rect.left++;
		 rect.top--;
		 rect.bottom--;
		 cc=c;
		 cc.Dark(80);
		 DrawCheck(rect,cc,cg);
	 }
}

//implement origin and clip
void XGraphics::DrawEllipse(const XRect &rect)
{
	LOGWHEREGRAPHICS();
	gdx2d_draw_circle( m_pBitmap->GetImage(), rect.left + rect.Width() / 2,
			rect.top + rect.Height() / 2, rect.Width() / 2, m_nTextColor);

//	int a = rect.Width() / 2;
//	int b = rect.Height() / 2;
//	int xc= rect.left + a;
//	int yc= rect.top + b;
//	int x,y;
//	double d1,d2;
//	x=0;y=b;
//	d1 = b*b+a*a*(-b+0.25);
//
//	m_pBitmap->DrawPixel(a+300,b+200,m_nTextColor);
//	m_pBitmap->DrawPixel(-a+300,b+200,m_nTextColor);
//	m_pBitmap->DrawPixel(a+300,-b+200,m_nTextColor);
//	m_pBitmap->DrawPixel(-a+300,-b+200,m_nTextColor);
//
//	while(b*b*(x+1)<a*a*(y-0.5))
//	{
//		if(d1<0)
//		{
//			d1+=b*b*(2*x+3);
//			x++;
//		}
//		else
//		{
//			d1+=b*b*(2*x+3)+a*a*(-2*y+2);
//			x++;y--;
//		}
//		m_pBitmap->DrawPixel(x+xc,y+yc,m_nTextColor);
//		m_pBitmap->DrawPixel(-x+xc,y+yc,m_nTextColor);
//		m_pBitmap->DrawPixel(x+xc,-y+yc,m_nTextColor);
//		m_pBitmap->DrawPixel(-x+xc,-y+yc,m_nTextColor);
//	}
//	d2=sqrt(b*(x+0.5))+a*(y-1)-a*b;
//	while(y>0)
//	{
//		if(d2<0)
//		{
//			d2+=b*b*(2*x+2)+a*a*(-2*y+3);
//			x++;y--;
//		}
//		else
//		{
//			d2+=a*a*(-2*y+3);
//			y--;
//		}
//		m_pBitmap->DrawPixel(x+xc,y+yc,m_nTextColor);
//		m_pBitmap->DrawPixel(-x+xc,y+yc,m_nTextColor);
//		m_pBitmap->DrawPixel(x+xc,-y+yc,m_nTextColor);
//		m_pBitmap->DrawPixel(-x+xc,-y+yc,m_nTextColor);
//	}
}

//implement origin and clip
void XGraphics::FillEllipse(const XRect &rect)
{
	LOGWHEREGRAPHICS();
	gdx2d_fill_circle( m_pBitmap->GetImage(), rect.left + rect.Width() / 2,
			rect.top + rect.Height() / 2, rect.Width() / 2, m_nTextColor);
	return;
}

XGraphics::XGraphics(int w, int h)
{
	LOGWHEREGRAPHICS();
	Init();
	Create(w,h);
}

void XGraphics::Create(int w, int h,XU8 nBits)
{
	LOGWHEREGRAPHICS();
	Release();
	m_pBitmap = new XImage();
	m_pBitmap->Create( w, h, nBits );
	mbImgCreate = true;
}

XBOOL XGraphics::Copy(XGraphics&g, const XPoint&dst, const XRect&rst)
{
	LOGWHEREGRAPHICS();

	if( m_pBitmap == NULL )
		return false;
	gdx2d_draw_pixmap( g.m_pBitmap->GetImage(), m_pBitmap->GetImage(),
			rst.left, rst.top, rst.Width(), rst.Height(),
			dst.x, dst.y, rst.Width(), rst.Height() );

	return XTRUE;
}

XU32 XGraphics::GetPixel(int x, int y)
{
	LOGWHEREGRAPHICS();
	XU32 color = 0;
	color = gdx2d_get_pixel( m_pBitmap->GetImage(), x, y );
	return color;
}
XU32 XGraphics::SetColor(XU32 nColor)
{
	LOGWHEREGRAPHICS();
	//LOGE("line:%d, color:%0x", __LINE__, nColor);
	XU32 c=m_nTextColor;
	m_nTextColor=nColor << 8 | nColor >> 24;
	//m_nTextColor=nColor << 8 ;
	//m_nTextColor = nColor;
	//LOGWHEREVAL(m_nTextColor);
	//LOGE("line:%d, color:%0x", __LINE__, m_nTextColor);
	return c;
}

XU32 XGraphics::SetBackColor(XU32 nColor)
{
	LOGWHEREGRAPHICS();
	XU32 c=m_nBackColor;
	m_nBackColor = nColor << 8 | nColor >>24 ;

	//m_nBackColor = nColor;
	return c;
}

void XGraphics::Release()
{
	LOGWHEREGRAPHICS();
	if( m_pBitmap != NULL )
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
		mbImgCreate = false;
	}
}

void XGraphics::Init()
{
	LOGWHEREGRAPHICS();
	mFTFace = 0;
	mFTLibrary = 0;
	m_nTextHeight = 16;
	m_nTextColor=0xFF00FFFF;
	m_nBackColor=0xffefffef;
	m_nFontStyle = 16 << 8;

	mbImgCreate = false;
	m_pBitmap=XNULL;
}

void XGraphics::GetFont()
{
	LOGWHEREGRAPHICS();

	if (mFTFace == 0 )
	{
		if (FT_Init_FreeType( &mFTLibrary ))
			return;
		char chTmp[256] = {0};
		sprintf( chTmp,"%s", "/system/fonts/DroidSansFallback.ttf");
		if( access(chTmp, F_OK ) != 0 )
		{
			memset( chTmp, 0, 256);
			sprintf( chTmp,"%s", "/system/fonts/Chinese-Hei.ttf");
			if( access(chTmp, F_OK ) != 0 )
			{
				memset( chTmp, 0, 256);
				sprintf( chTmp,"%s", "/system/fonts/AndroidClock.ttf");
			}
		}
		//LOGWHEREMSG( chTmp );
		if ( FT_New_Face( mFTLibrary, chTmp, 0, &mFTFace ) )
		{
			LOGWHERE();
		}
		FT_Select_Charmap(mFTFace, FT_ENCODING_UNICODE);
		//FT_Set_Pixel_Sizes(mFTFace,0, 12);
		InitFontBuffer( gFonts );
	}
	if (m_nFontStyle & XF_UPDATE)
	{
		m_nFontStyle &= ~XF_UPDATE;
		m_nTextHeight = (m_nFontStyle & 0x0000ff00) >>8;
		m_nTextHeight += 13;
		//LOGWHEREVAL( m_nTextHeight );
		FT_Set_Pixel_Sizes(mFTFace,0,m_nTextHeight);
	}
	return;
}

void XGraphics::GetBitmapData(void*pBuf,int nLength,XU8 nBits)
{
	LOGWHEREGRAPHICS();
	void * pData = m_pBitmap->mImgData.pixels;
	memcpy( pBuf, pData, nLength );
	return;
}

XBOOL XGraphics::DrawDIB(XDib &dib, const XPOINT &pt, const XRECT &rect, XBOOL isButton)
{
	LOGWHEREGRAPHICS();

	if(dib.IsEmpty()||dib.GetBits()!=XDib::BITS32)
		return XFALSE;

//	int w=dib.Width();
//	int h=dib.Height();
//	SetOrigin(0, 0);
//
//	gdx2d_pixmap tmp;
//	tmp.width = w;
//	tmp.height = h;
//	tmp.format = 4;
//	tmp.pixels = dib.GetBuffer();
//
//	gdx2d_draw_pixmap( &tmp, m_pBitmap->GetImage(),
//			rect.left, rect.top, rect.Width(), rect.Height(),
//			pt.x, pt.y, rect.right - rect.left, rect.bottom - rect.top );
	////

	int w=dib.Width();
	int h=dib.Height();
	SetOrigin(0, 0);

	gdx2d_pixmap tmp;
	tmp.width = w;
	tmp.height = h;
	tmp.format = 4;
	tmp.pixels = dib.GetBuffer();

	XRect drt;
	drt.left = pt.x;
	drt.top = pt.y;
	drt.right = pt.x + rect.right - rect.left;
	drt.bottom = pt.y + rect.bottom - rect.top;
	//drt += m_origin;
	drt &= m_clip;
	int td,bd,ld,rd;
	td = drt.top - pt.y;
	bd = (pt.y + rect.bottom - rect.top) - drt.bottom;
	ld = drt.left - pt.x;
	rd = (pt.x + rect.right - rect.left) - drt.right;

	XRect srt(rect);
	srt.left += ld;
	srt.right -= rd;
	srt.top += td;
	srt.bottom -= bd;

	if( srt.IsRectEmpty() || drt.IsRectEmpty() )
		return false;

//	gdx2d_pixmap* pImgDst = m_pBitmap->GetImage();
//	gdx2d_pixmap* pImgSrc = &tmp;
//
//	unsigned char * pdst = pImgDst->pixels;
//	unsigned char * psrc = pImgSrc->pixels;
//
//	int dstx = drt.left;
//	int dstx1 = drt.right;
//	int dsty = drt.top;
//	int dsty1 = drt.bottom;
//
//	int srcx = srt.left;
//	int srcx1 = srt.right;
//	int srcy = srt.top;
//	int srcy1 = srt.bottom;
//
//	pdst = pImgDst->pixels + ((dsty * pImgDst->width + dstx ) << 2);
//	psrc = pImgSrc->pixels + ((srcy * pImgSrc->width + srcx ) << 2);
//	int ilen = srt.Width() << 2;
//	for( int j = dsty; j < dsty1; j++ )
//	{
//		if( j < pImgDst->height )
//		{
//			unsigned char * psrcTmp = psrc;
//			unsigned char * pdstTmp = pdst;
//			for( int i = dstx; i < dstx1; i++ )
//			{
//				if( psrcTmp[3] <= 10 )
//				{
//					psrcTmp += 4;
//					pdstTmp += 4;
//					continue;
//				}
//				memcpy( pdstTmp, psrcTmp, 4 );
//				psrcTmp += 4;
//				pdstTmp += 4;
//			}
//			//memcpy( pdst, psrc, ilen );
//			pdst += ( pImgDst->width << 2 );
//			psrc += ( pImgSrc->width << 2 );
//		}
//		else
//		{
//			break;
//		}
//	}

	gdx2d_set_blend(1);
	gdx2d_draw_pixmap( &tmp, m_pBitmap->GetImage(),
			srt.left, srt.top, srt.Width(), srt.Height(),
			drt.left, drt.top, drt.Width(), drt.Height() );
	gdx2d_set_blend(0);

	return XTRUE;
}

XBOOL XGraphics::DrawDIB(XDib &dib, const XPOINT &pt, const XRECT &rect)
{
	LOGWHEREGRAPHICS();
	if(dib.IsEmpty()||dib.GetBits()!=XDib::BITS32)
		return XFALSE;

	int w=dib.Width();
	int h=dib.Height();
	SetOrigin(0, 0);

	gdx2d_pixmap tmp;
	tmp.width = w;
	tmp.height = h;
	tmp.format = 4;
	tmp.pixels = dib.GetBuffer();

	XRect drt;
	drt.left = pt.x;
	drt.top = pt.y;
	drt.right = pt.x + rect.right - rect.left;
	drt.bottom = pt.y + rect.bottom - rect.top;
	//drt += m_origin;
	drt &= m_clip;
	if( drt.IsRectEmpty() )
		return false;
	int td,bd,ld,rd;
	td = drt.top - pt.y;
	bd = (pt.y + rect.bottom - rect.top) - drt.bottom;
	ld = drt.left - pt.x;
	rd = (pt.x + rect.right - rect.left) - drt.right;

	XRect srt(rect);
	srt.left += ld;
	srt.right -= rd;
	srt.top += td;
	srt.bottom -= bd;

	if( srt.IsRectEmpty() )
		return false;

	gdx2d_pixmap* pImgDst = m_pBitmap->GetImage();
	gdx2d_pixmap* pImgSrc = &tmp;

	int dsty = drt.top;
	int dsty1 = drt.bottom < pImgDst->height ? drt.bottom : pImgDst->height;

	unsigned char * pdst = pImgDst->pixels + ((dsty * pImgDst->width + drt.left ) << 2);
	unsigned char * psrc = pImgSrc->pixels + ((srt.top * pImgSrc->width + srt.left ) << 2);
	unsigned char * pdstEnd = pImgDst->pixels + ((pImgDst->width * pImgDst->height) << 2 );
	unsigned char * psrcEnd = pImgSrc->pixels + ((pImgSrc->width * pImgSrc->height) << 2 );
	int ilen = srt.Width() << 2;
	//LOGE("line = %d,ilen = %d, width = %d", __LINE__, ilen, pImgSrc->width );

	for( int j = dsty; j < dsty1; j++ )
	{
		if( j < pImgDst->height )
		{
			if( ( psrc + ilen) >= psrcEnd || ( pdst + ilen) >= pdstEnd )
				break;
			memcpy( pdst, psrc, ilen );
			pdst += ( pImgDst->width << 2 );
			psrc += ( pImgSrc->width << 2 );
		}
		else
		{
			break;
		}
	}
	//LOGE("line = %d,ilen = %d, width = %d", __LINE__, ilen, pImgSrc->width );

//	gdx2d_draw_pixmap( &tmp, m_pBitmap->GetImage(),
//			srt.left, srt.top, srt.Width(), srt.Height(),
//			drt.left, drt.top, drt.Width(), drt.Height() );

	return XTRUE;
}
void XGraphics::GetBitmapMask(int sx, int sy, XDib &dib)
{
	LOGWHEREGRAPHICS();
	if(dib.GetBits()!=8)
		return;
	gdx2d_pixmap *pBmp = m_pBitmap->GetImage();
	int w=pBmp->width;
	int h=pBmp->height;
	XU8*pSrt=(XU8*)pBmp->pixels;

	int wb=dib.Width();
	int ws=w<<2;
	XU8*pDst=(XU8*)dib.GetBuffer()+sy*wb+sx;

	for(int y=0;y<h;y++)
	{
		for(int x=0;x<w;x++)
		{
			pDst[x]=pSrt[(x<<2)+1]>>2;
		}
		pSrt+=(ws);
		pDst+=wb;
	}

}

XU32 XGraphics::SetBold(XBOOL b)
{
	LOGWHEREGRAPHICS();

	XU32 n=m_nFontStyle;
	if(b)
	{
		if(!(m_nFontStyle&XF_BOLD))
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle|=(XF_BOLD);
	}
	else
	{
		if(m_nFontStyle&XF_BOLD)
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle&=~(XF_BOLD);
	}
	m_nFontStyle|=XF_UPDATE;
	return n;
}
XU32 XGraphics::SetItalic(XBOOL b)
{
	LOGWHEREGRAPHICS();

	XU32 n=m_nFontStyle;
	if(b)
	{
		if(!(m_nFontStyle&XF_ITALIC))
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle|=(XF_ITALIC);
	}
	else
	{
		if(m_nFontStyle&XF_ITALIC)
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle&=~(XF_ITALIC);
	}
	//m_nFontStyle|=XF_UPDATE;
	return n;
}
XU32 XGraphics::SetStrike(XBOOL b)
{
	LOGWHEREGRAPHICS();

	XU32 n=m_nFontStyle;
	if(b)
	{
		if(!(m_nFontStyle&XF_STRIKE))
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle|=(XF_STRIKE);
	}
	else
	{
		if(m_nFontStyle&XF_STRIKE)
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle&=~(XF_STRIKE);
	}
	//m_nFontStyle|=XF_UPDATE;
	return n;
}

XU32 XGraphics::SetUnderline(XBOOL b)
{
	LOGWHEREGRAPHICS();

	XU32 n = m_nFontStyle;
	if(b)
	{
		if(!(m_nFontStyle & XF_UNDERLINE))
			m_nFontStyle |= XF_UPDATE;
		m_nFontStyle |= XF_UNDERLINE;
	}
	else
	{
		if( m_nFontStyle & XF_UNDERLINE )
			m_nFontStyle |= XF_UPDATE;
		m_nFontStyle &= ~XF_UNDERLINE;
	}

	return n;

}
XU32 XGraphics::SetFontStyle(XU32 ns)
{
	LOGWHEREGRAPHICS();
	//LOGWHEREVAL( ns );
	XU32 n = m_nFontStyle & 0x7fffffff;
	if((n & 0xff1f) != ( ns & 0xff1f) )
		m_nFontStyle |= XF_UPDATE;
	m_nFontStyle &= 0x80000000;
	m_nFontStyle |= ns;
	return n;
}

XU32 XGraphics::SetFontSize(XU8 nSize)
{
	LOGWHEREGRAPHICS();

	//LOGWHEREVAL( nSize );
	XU32 n = m_nFontStyle;
	XU32 ns = nSize << 8;
	if ((m_nFontStyle & 0x0000ff00) != ns)
	{
		m_nFontStyle &= 0xffff00ff;
		m_nFontStyle |= ns;
		m_nFontStyle |= XF_UPDATE;
	}
	return n;
}

