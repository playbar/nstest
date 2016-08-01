// XImage.cpp: implementation of the XImage class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include <assert.h>
#include "XFile.h"
#include "MultiStringUtil.h"
#include "XResource.h"
#include "XDib.h"
#include "string_comp.h"

#ifdef __ANDROID__
#include <jni.h>
#include "StbImage.h"
#include "XStream.h"
#include "GifLibary.h"
#include "BwImageDecode.h"
#endif

#include "XImage.h"

#if 0
#define LOGWHEREIMG()  LOGWHERE()
#else
#define LOGWHEREIMG()
#endif

#ifdef _ARGB_PIXEL_
	#define COLOR16TO32A(C16,C32) {C32=0;if(C16&0x8000) C32|=0xff000000;\
								  C32|=((((C16>>10)&0x3f)<<3));\
								  C32|=((((C16>>5)&0x3f)<<3)<<8);\
								  C32|=((((C16)&0x3f)<<3)<<16);}
	#define COLOR24TO32A(C24,C32) (C32=0xff000000|(C24[0]<<16)|(C24[1]<<8)|C24[2])
	#define SWAPPIXELRB(COLOR) {XU8 n=((XU8*)&COLOR)[0];((XU8*)&COLOR)[0]=((XU8*)&COLOR)[2];((XU8*)&COLOR)[2]=n;}
#else
	#define COLOR16TO32A COLOR16TO32
	#define COLOR24TO32A COLOR24TO32
#endif

#define COLOR16TO32_T(C16,C32) {C32=0;if(C16&0x8000) \
		C32 = 0xFF000000 | (((C16) &0x1f)<<19)|(((C16>>5) &0x3f)<<10)|(((C16>>11) &0x1f)<<3);}

extern void CvRGBtoBGR(XU8 *data, int len);

#define DEFAULT_DELAY 4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XImage::XImage()
{
	LOGWHEREIMG();
	m_pStream = XNULL;
	m_nFrames = 0;
	m_pDelay = XNULL;
	m_bBitmap = XFALSE;

	m_curFrame = 0;
	m_gifImage = 0;

	mImgData.width = 0;
	mImgData.height = 0;
	mImgData.format = STBI_default;
	mImgData.pixels = NULL;
}

XImage::XImage( int width, int height, int bitdepth )
{
	LOGWHEREIMG();
	m_pStream = XNULL;
	m_nFrames = 0;
	m_pDelay = XNULL;
	m_bBitmap = XFALSE;

	m_curFrame = 0;
	m_gifImage = 0;

	mImgData.width = 0;
	mImgData.height = 0;
	mImgData.format = STBI_default;
	mImgData.pixels = NULL;

	Create( width, height, bitdepth );
	return;
}

int XImage::Create(int width, int height, int bitdepth)
{
	LOGWHEREIMG();
	mImgData.width = width;
	mImgData.height = height;
	switch(bitdepth)
	{
	case 32:
		mImgData.format = STBI_rgb_alpha;
		break;
	case 24:
		mImgData.format = STBI_rgb;
		break;
	case 16:
		mImgData.format = STBI_grey_alpha;
		break;
	case 8:
		mImgData.format = STBI_grey;
		break;
	default:
		mImgData.format = STBI_default;
		break;
	}

	int ilen = width * height * bitdepth / 8;
	mImgData.pixels = new unsigned char[ ilen ];
	if(mImgData.pixels != NULL)
	{
		memset(mImgData.pixels, 0, ilen );
		return true;
	}
	return false;
}

XImage::~XImage()
{
	LOGWHEREIMG();
	Release();
}

XU32 XImage::Width() const
{
	LOGWHEREIMG();
	return mImgData.width;
}

XU32 XImage::Height() const
{
	LOGWHEREIMG();
	return mImgData.height;
}

XBOOL XImage::IsEmpty() const
{
	LOGWHEREIMG();
	if(mImgData.pixels ==NULL )
	{
		//LOGWHERE();
		return true;
	}
	return false;
}

void XImage::Release()
{
	LOGWHEREIMG();

	if( m_pStream != NULL )
	{
		delete m_pStream;
		m_pStream = NULL;
	}
	if (m_gifImage != NULL )
	{
		del_gif((Gif*) m_gifImage);
		m_gifImage = 0;
	}
	if( mImgData.pixels != NULL )
	{
		delete []mImgData.pixels;
		mImgData.pixels = NULL;
	}
	mImgData.width = 0;
	mImgData.height = 0;
	mImgData.format = 0;
	m_curFrame = 0;
	m_nFrames = 0;
}

XINT XImage::FrameCount()
{
	LOGWHEREIMG();
	//LOGWHEREVAL( m_nFrames );
	return m_nFrames;
}


XBOOL XImage::Create(XPCWSTR strFile)
{
	 char* strTmp = unicodeToUtf8(strFile);
	 //LoadFromFile( strTpm );
	 BwImageDecode bwImg;
	 int width = 0;
	 int height = 0;
	 mImgData.pixels = bwImg.DecodeFile( strTmp, width, height );
	 mImgData.width = width;
	 mImgData.height = height;
	 mImgData.format = 4;
	 delete[] strTmp;
	 return true;
}

XBOOL XImage::SetActive(XINT index)
{
	LOGWHEREIMG();

	if (m_gifImage == XNULL)
	{
		return XFALSE;
	}
	m_curFrame = index;
	if (m_curFrame >= ((Gif*) m_gifImage)->getFrames())
	{
		m_curFrame = ((Gif*) m_gifImage)->getFrames() - 1;
	}
	toGifFrameForBuffer();
	return XTRUE;
}

void XImage::toGifFrameForBuffer()
{
	GifScreen* screen = ((Gif*) m_gifImage)->screen;
	GifPicture* pic = ((Gif*) m_gifImage)->m_data[m_curFrame].m_pic;
	GifPalette* pal = pic->has_cmap ? pic->cmap : screen->cmap;

	int transparent = ((Gif*) m_gifImage)->m_data[m_curFrame].m_transparent;
	int disposal = false;
	int left = pic->left;
	int top = pic->top;
	//LOGE("line=%d, left=%d, top=%d, width=%d, height=%d, curFrame=%d", __LINE__, left, top,
	//		pic->width, pic->height, m_curFrame );
	if (m_curFrame > 0 && ((Gif*) m_gifImage)->m_data[m_curFrame - 1].m_disposal > 1)
	{
		disposal = true;
	}
	for (int row = 0; row < pic->height; ++row)
	{
		unsigned char* row_start = mImgData.pixels + 4 * ((row + top) * screen->width + left);

		for (int col = 0; col < pic->width; ++col)
		{
			int index = pic->data[row][col];
			//LOGE("line=%d, row=%d, col=%d, index=%d, trans=%d, disposal=%d, r=%d, g=%d,b=%d",
			//		__LINE__, row, col, index, transparent, disposal,
			//		pal->colours[index].red, pal->colours[index].green, pal->colours[index].blue);
			if (index != transparent)
			{
				row_start[3] = 255;
				row_start[2] = pal->colours[index].blue;
				row_start[1] = pal->colours[index].green;
				row_start[0] = pal->colours[index].red;
			}
			else if (disposal)
			{
				row_start[3] = 0;
				row_start[2] = 0;
				row_start[1] = 0;
				row_start[0] = 0;
			}
			row_start += 4;
		}
	}
}

XBOOL XImage::loadFromGif( )
{
	if (m_gifImage != NULL )
	{
		del_gif((Gif*) m_gifImage);
		m_gifImage = NULL;
	}
	if( m_pDelay != NULL )
	{
		delete []m_pDelay;
		m_pDelay = NULL;
	}
	m_gifImage = read_gif_file(m_pStream);
	((Gif*) m_gifImage)->calculateHelperData();

	GifScreen* screen = ((Gif*) m_gifImage)->screen;
	mImgData.width = screen->width;
	mImgData.height = screen->height;
	mImgData.format = 4;
	int totalSize = mImgData.width * mImgData.height * 4;
	if( mImgData.pixels != NULL )
	{
		delete []mImgData.pixels;
		mImgData.pixels = NULL;
	}
	mImgData.pixels = new unsigned char[totalSize];
	memset( mImgData.pixels, 0, totalSize );

	SetFrames();
	toGifFrameForBuffer();

	return XTRUE;
}

void XImage::DeleteImage()
{
	LOGWHEREIMG();

	if( m_pStream != NULL )
	{
		delete m_pStream;
		m_pStream = NULL;
	}

	if (m_gifImage != NULL )
	{
		del_gif((Gif*) m_gifImage);
		m_gifImage = 0;
	}
	if( mImgData.pixels != NULL )
	{
		delete []mImgData.pixels;
		mImgData.pixels = NULL;
	}
	mImgData.width = 0;
	mImgData.height = 0;
	mImgData.format = 0;
	m_curFrame = 0;
	m_nFrames = 0;
}

XBOOL XImage::Final(XU8 bBitmap)
{
	LOGWHEREIMG();

	//LOGWHERE();
	if (m_pStream == XNULL)
	{
		return XFALSE;
	}
	// ignore bBitmap
	XU8* data = m_pStream->ReadAddr();
	int ilen = m_pStream->GetDataSize();
	BwImageDecode bwImg;
	int enImgType = bwImg.checkFormatMem( data );
	if( enImgType == EN_IMG_TYIE_GIF )
	{
		loadFromGif( );
	}
	else if( enImgType == EN_IMG_TYIE_JPG )
	{
		 int width = 0;
		 int height = 0;
		 mImgData.pixels = bwImg.DecodeMemoryJPG( data, ilen, width, height );
		 mImgData.width = width;
		 mImgData.height = height;
		 mImgData.format = 4;
	}
	else
	{
		LoadFromBuf( data, ilen, 4 );
	}
	delete m_pStream;
	m_pStream = NULL;
	m_bBitmap = bBitmap;
	return XTRUE;

}

XBOOL XImage::Append(const void *pData, XINT nSize)
{
	LOGWHEREIMG();
	// TODO by hgl
	if (m_pStream == XNULL)
	{
		m_pStream = new XStream();
	}
	m_pStream->WriteData(pData, nSize);
	return XTRUE;
}

void XImage::SetFrames()
{
	LOGWHEREIMG();

	m_curFrame = 0;
	m_nFrames = 1;
	if (m_gifImage != NULL )
	{
		m_nFrames = ((Gif*) m_gifImage)->getFrames();
		InitDelay();
	}
}

XBOOL XImage::CreateFromResource(XPCTSTR strName, XBOOL bBitmap)
{
	LOGWHEREIMG();
	bool b = XResource::LoadImage(this, strName, bBitmap);
	return b;
}

void XImage::InitDelay()
{
	LOGWHEREIMG();

	if( m_pDelay != NULL )
	{
		delete []m_pDelay;
		m_pDelay = NULL;
	}
	m_pDelay = new XU8[m_nFrames];
	if (((Gif*) m_gifImage)->m_data.empty())
	{
		for (XU32 i = 0; i < m_nFrames; ++i)
		{
			m_pDelay[i] = DEFAULT_DELAY;
		}
	}
	else
	{
		for (XU32 i = 0; i < m_nFrames; ++i)
		{
			m_pDelay[i] = ((Gif*) m_gifImage)->m_data[i].m_delayTime / 5;
		}
	}
}

XU8 XImage::GetFrameDelay(int id)
{
	LOGWHEREIMG();
	if (m_pDelay == XNULL)
		return DEFAULT_DELAY;
	else if (id < 0 || id > m_nFrames)
		return DEFAULT_DELAY;
	return m_pDelay[id];
}

void XImage::ResetStream()
{
	LOGWHEREIMG();
	if (m_pStream != XNULL)
	{
		delete m_pStream;
		m_pStream = XNULL;
	}
}

XBOOL XImage::GetPalette(XU32 rgba[], int nCount)
{
	LOGWHEREIMG();
	if (IsEmpty() || !m_bBitmap)
		return XFALSE;

	return XFALSE;
}

XU8 XImage::GetPixelBits()
{
	LOGWHEREIMG();
	return 32;
}

XBOOL XImage::GetBitmapData(XU8 *pBuffer, XU32 nLength)
{
	LOGWHEREIMG();
	if (IsEmpty() || !m_bBitmap)
		return XFALSE;
	memcpy(pBuffer, mImgData.pixels, nLength);
	return XTRUE;
}

XBOOL XImage::GetBitmapDataSkip(XU8*pData, int wi, int hi, int nBitsi, int nSkip)
{
	LOGWHEREIMG();
	if (IsEmpty() || !m_bBitmap)
		return XFALSE;
	if (nBitsi != 32)
		return XFALSE;
	int w = mImgData.width;
	int h = mImgData.height;
	int nBits = GetPixelBits();
	int iPitch = (((((w * nBits) >> 3) + 3) >> 2) << 2) << nSkip;

	XU32 nFmt = GetPixelBits();
	XU32 rgba[256];
	if (nBits == 8)
		GetPalette(rgba, 256);
	void *pSrcData = mImgData.pixels;

	XU8*pSrt = (XU8*) pSrcData;
	XU32*pDst = (XU32*) pData;
	int nSkip16 = 1 + nSkip;
	int nSkip24 = 3 << nSkip;
	int nSkip32 = 2 + nSkip;
	if (hi << nSkip > h)
		hi = h >> nSkip;
	if (wi << nSkip > w)
		hi = w >> nSkip;
	for (int y = 0; y < hi; y++)
	{
		int x1 = 0;
		for (int x = 0; x < wi; x++)
		{
			switch (nBits)
			{
			case 8:
				pDst[x] = rgba[pSrt[x << nSkip]];
				break;
			case 16:
				{
					XU16 color = *((XU16*) (pSrt + (x << nSkip16)));

					XU32 c;
					COLOR16TO32(color, c);
					pDst[x] = c;
				}
				break;
			case 24:
				{
					XU8*p = (pSrt + x1);
					x1 += nSkip24;
					XU32 c;
					COLOR24TO32(p, c);
					pDst[x] = c;
				}
				break;
			case 32:
				pDst[x] = *(XU32*) (pSrt + (x << nSkip32));
				break;
			}
		}
		pSrt += iPitch;
		pDst += wi;
	}

	return XTRUE;
}

XBOOL XImage::GetBitmapData(XU8*pData, int wi, int hi, int nBitsi, int nLength)
{
	LOGWHEREIMG();
	if (GetPixelBits() == nBitsi)
	{
		XBOOL bOK = GetBitmapData(pData, nLength);
		return bOK;
	}

	if (IsEmpty() || !m_bBitmap)
		return XFALSE;
	if (nBitsi != 32)
		return XFALSE;

	int w = mImgData.width;
	int h = mImgData.height;
	int nBits = GetPixelBits();
	int iPitch = ((((w * nBits) >> 3) + 3) >> 2) << 2;

	XU32 rgba[256];
	if (nBits == 8)
		GetPalette(rgba, 256);
	void *pSrcData = mImgData.pixels;

	XU8*pSrt = (XU8*) pSrcData;
	XU32*pDst = (XU32*) pData;
	h = XMIN(h, hi);
	w = XMIN(w, wi);
	for (int y = 0; y < h; y++)
	{
		int x1 = 0;
		for (int x = 0; x < w; x++)
		{
			switch (nBits)
			{
			case 8:
				pDst[x] = rgba[pSrt[x]];
				break;
			case 16:
				{
					XU16 color = *((XU16*) (pSrt + (x << 1)));
					XU32 c;
					COLOR16TO32_T(color,c);
					pDst[x] = c;
				}
				break;
			case 24:
				{
					XU8*p = (pSrt + x1);
					x1 += 3;
					XU32 c;
					COLOR24TO32(p, c);
					pDst[x] = c;
				}
				break;
			case 32:
				pDst[x] = *(XU32*) (pSrt + (x << 2));
				break;
			}
		}
		pSrt += iPitch;
		pDst += wi;
	}
	return XTRUE;
}

bool XImage::LoadFromBuf(unsigned char* buffer,int len,int req_format)
{
	mImgData.pixels = stbi_load_from_memory(buffer, len,
			(int*)&mImgData.width, (int*)&mImgData.height, (int*)&mImgData.format,req_format);
	mImgData.format = 4;
	if(mImgData.pixels == NULL)
	{
		return false;
	}
	return true;
}
bool XImage::LoadFromFile(char* filename)
{
	mImgData.pixels = stbi_load(filename,
			(int*)&mImgData.width, (int*)&mImgData.height,(int*)&mImgData.format,4);
	mImgData.format = 4;
	if(mImgData.pixels == NULL)
	{
		return false;
	}
	return true;
}


