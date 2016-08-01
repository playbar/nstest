#ifndef _XIMAGE_
#define _XIMAGE_

#include "GGType.h"
#include "XData.h"
#include "XVector.h"

#ifdef __ANDROID__
#include "gdx2d.h"
class XStream;
#endif

#if defined(__APPLE__)
#include "GifLibary.h"
class XStream;
#endif

class _XEXT_CLASS XImage
{
public:
	XBOOL Create(XPCWSTR strFile);
	XINT FrameCount();
	void Release();
	XBOOL IsEmpty() const;
	XImage();
	virtual ~XImage();
	XU32 Width() const;
	XU32 Height() const;
	XBOOL Append(const XU8Array&data)
	{
		return Append(data,data.GetSize());
	}
public:
	XBOOL GetBitmapData(XU8*pDst,int w,int h,int nBits,int nLength);
	XBOOL GetBitmapDataSkip(XU8*pDst,int w,int h,int nBits,int nSkip);
	XBOOL GetBitmapData(XU8* pBuffer,XU32 nLength);
	void ResetStream();
	XU8 GetFrameDelay(int id);
	XBOOL Append(const void*pData,XINT nSize);
	XBOOL Final(XU8 bBitmap=XFALSE);
	XBOOL SetActive(XINT id);
	XBOOL GetPalette(XU32 rgba[], int nCount);
	XU8   GetPixelBits();
	XBOOL CreateFromResource(XPCTSTR strName,XBOOL bBitmap=XFALSE);
#ifdef _SYMBIAN
	CFbsBitmap* m_pImage;
#elif defined ( __ANDROID__ )
#else
	void* m_pImage;
#endif
	XU8   m_guid[16];
	XBOOL m_bBitmap;
#ifdef __ANDROID__
	XStream* m_pStream;
	gdx2d_pixmap* GetImage() { return &mImgData; }
#else
	void* m_pStream;
	void* GetImage()const{return m_pImage;}
#endif

#ifdef __APPLE__
	static void* decompress(void* img);
	void* decompressImgRef()const { return m_decompressImgRef; }
#endif

protected:
	void InitDelay();
	XU8* m_pDelay;
	void SetFrames();
	XU8 m_nFrames;
#if defined(__APPLE__)
	unsigned char* m_gifBuffer;
	int m_gifBufferTotalSize;
	int m_curFrame;
	int m_bufferFrame;
	Gif* m_gifImage;
	XBOOL loadFromGif(XStream* stream);
	void updateGifImage();
	void toGifFrameForBuffer();	
	void* m_decompressImgRef;
	unsigned char* m_cachedImageData;// total data
#elif defined(__ANDROID__)	// zhengjl 2011.10
public:
	XImage( int width, int height, int bitdepth );
	XBOOL Create(int width, int height, int bitdepth);
	unsigned char* GetImgData()
	{
		return mImgData.pixels;
	}

	gdx2d_pixmap mImgData;	//图像数据
protected:

	bool LoadFromBuf(unsigned char* buffer,int len,int req_format);
	bool LoadFromFile(char* filename);

	void DeleteImage();
	XBOOL loadFromGif( );
	void toGifFrameForBuffer();

	int m_curFrame;
	void* m_gifImage;

#endif
//	XU8Array m_data;
};

#endif // !defined(AFX_XIMAGE_H__116E2A63_E9B3_4932_A8DC_C8CBC4E4262A__INCLUDED_)
