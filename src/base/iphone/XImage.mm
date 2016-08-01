// XImage.cpp: implementation of the XImage class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XFile.h"
#include "MultiStringUtil.h"
#include "XImage.h"
#include "XResource.h"
#include "XDib.h"
#include "XStream.h"
#include "string_comp.h"
#include <assert.h>

//changed image decoder
//#define __ZCTESTimagedecode

//#ifdef __ZCTESTimagedecode
//#include "stb_image.h"
//#endif//__ZCTESTimagedecode


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 XImage::XImage()
{
	m_curFrame = 0;
	m_gifImage = 0;
	m_gifBuffer = 0;
	m_bufferFrame = -1;
	m_gifBufferTotalSize = 0;
	m_cachedImageData = 0;
  m_pImage=XNULL;
  m_pStream=XNULL;
  m_nFrames=0;
  m_pDelay=XNULL;
  m_bBitmap=XFALSE;
}

XImage::~XImage()
{
	Release();  
}

XU32 XImage::Width() const
{
	if (m_pImage != XNULL)
	{
		UIImage* img = (UIImage*)m_pImage;
		CGSize size = [img size];
		return size.width;
	}
	return 0;
}

XU32 XImage::Height() const
{
	if (m_pImage != XNULL)
	{
		UIImage* img = (UIImage*)m_pImage;
		return [img size].height;
	}
	return 0;
}

XBOOL XImage::IsEmpty() const
{	 
   if(m_pImage==XNULL)
       return XTRUE;
   /*if(((Image*)m_pImage)->GetLastStatus()!=Ok) return XTRUE;
   if(Width()<=0||
	  Height()<=0) return XTRUE;*/
   return XFALSE;
}

void XImage::Release()
{
	if (m_pImage != XNULL)
	{
		//CGImageRelease((CGImageRef)m_pImage);
		UIImage* img = (UIImage*)m_pImage;
		[img release];
		m_pImage = XNULL;
	}
	if (m_pDelay != XNULL)
	{
		delete[] m_pDelay;
		m_pDelay = 0;
	}
	if (m_pStream != XNULL)
	{
		delete (XStream*)m_pStream;
		m_pStream = 0;
	}
	m_nFrames = 0;
	if (m_gifImage)
	{
		del_gif(m_gifImage);
		m_gifImage = 0;
		delete[] m_gifBuffer;
		m_gifBuffer = 0;
		m_bufferFrame = -1;
	}
	if (m_cachedImageData)
	{
		delete[] m_cachedImageData;
		m_cachedImageData = 0;
	}
}

XINT XImage::FrameCount()
{
   return m_nFrames;
}

XBOOL XImage::Create(XPCWSTR strFile)
{
	Release();
	
	//if gif. special handle.
	
	XFile file;
	char* path = unicodeToUtf8(strFile);
	bool is_open = file.Open(path, XFile::XREAD | XFile::XBINARY);
	delete[] path;
	if (!is_open)
	{
		return XFALSE;
	}
	int file_len = file.GetLength();
	unsigned char* buffer = new unsigned char[file_len];
	file.Read(buffer, file_len);
	bool ret = Append(buffer, file_len);
	delete buffer;
	assert(ret);
	return ret && Final(false);
}

XBOOL XImage::SetActive(XINT index)
{
//	m_curFrame = index;
//	if (m_gifImage)
//	{
//		// updateImage.
//		if (m_curFrame >= m_gifImage->getFrames())
//			m_curFrame = m_gifImage->getFrames() - 1;
//		updateGifImage();
//	}
	if (m_gifImage==XNULL) return XFALSE;
	
	m_curFrame = index;
	if (m_curFrame>=m_gifImage->getFrames())
		m_curFrame=m_gifImage->getFrames()-1;
	updateGifImage();
	return XTRUE;
}

void XImage::toGifFrameForBuffer()
{
	int toFrame = m_curFrame;
	int startFrame = m_bufferFrame;
	m_bufferFrame = m_curFrame;
//	memset(m_gifBuffer, 0, m_gifBufferTotalSize);
//	startFrame = -1;
	//startFrame = m_curFrame - 1;
	GifScreen* screen = m_gifImage->screen;
	
	for (int i = startFrame + 1; i <= toFrame; ++i)
	{
		assert(i < m_gifImage->getFrames());
		GifPicture* pic = m_gifImage->m_data[i].m_pic;
		GifPalette* pal = pic->has_cmap ? pic->cmap : screen->cmap;
		
		// fill color buffer
		assert(pic);
		assert(pal);
		assert(pic->left >= 0);
		assert(pic->top >= 0);
		assert(pic->left + pic->width <= screen->width);
		assert(pic->top + pic->height <= screen->height);
		
//		printf("disposal value = %d\n", m_gifImage->m_disposal[i]);
		
		int transparent = m_gifImage->m_data[i].m_transparent;
		int disposal = false;
		if (i > 0 && m_gifImage->m_data[i - 1].m_disposal > 1)
			disposal = true;
		for (int row = 0; row < pic->height; ++row)
		{
			int left = pic->left;
			int top = pic->top;
			unsigned char* row_start = m_gifBuffer + 4 * ((row + top) * screen->width + left);
			
			for (int col = 0; col < pic->width; ++col)
			{
				int index = pic->data[row][col];
				assert(index < pal->length && index >= 0);
				if (index != transparent)
				{
					row_start[3] = 255;
					row_start[2] = pal->colours[index].red;
					row_start[1] = pal->colours[index].green;
					row_start[0] = pal->colours[index].blue;
				}
				else if (disposal)
				{
					row_start[3] = 0;
				}

				row_start += 4;
			}
		}
	}
}


void XImage::updateGifImage()
{
	if (!m_gifImage) return;
	// frame 0
	if (m_bufferFrame == m_curFrame) 
		return;
    if (m_gifImage == NULL ) {
        return;
    }
    if ( m_curFrame >= m_gifImage->getFrames() ) {
        return;
    }
	//assert(m_gifImage);
	//assert(m_curFrame < m_gifImage->getFrames());
	GifScreen* screen = m_gifImage->screen;
	int width = screen->width;
	int height = screen->height;
	int totalSize = width * height * 4;
	unsigned char* newBuffer = 0;
	bool resetBuffer = false;
	if (m_gifBufferTotalSize > 0)
	{
		assert(totalSize == m_gifBufferTotalSize);
		newBuffer = m_gifBuffer;
	}
	else
	{
		assert(m_gifBufferTotalSize == 0);
		newBuffer = new unsigned char[totalSize];
		m_gifBufferTotalSize = totalSize;
		resetBuffer = true;
	}
	if (m_curFrame == 0)
		resetBuffer = true;
	if (resetBuffer)
	{
		memset(newBuffer, 0, totalSize);
		m_bufferFrame = -1;
	}
	m_gifBuffer = newBuffer;
	toGifFrameForBuffer();

	if (m_pImage)
	{
		[(UIImage*)m_pImage release];
		m_pImage = 0;
	}
	CGDataProviderRef ref = CGDataProviderCreateWithData(NULL, newBuffer, totalSize, NULL);
	CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
	CGImageRef iref = CGImageCreate(width, height, 8, 32, width * 4, colorspace, kCGBitmapByteOrder32Little | kCGImageAlphaFirst,
									ref, NULL, true, kCGRenderingIntentDefault);
	
	UIImage* img = [[UIImage alloc] initWithCGImage:iref];
	m_pImage = img;
	CGImageRelease(iref);
	CGColorSpaceRelease(colorspace);
	CGDataProviderRelease(ref);

}

XBOOL XImage::loadFromGif(XStream* stream)
{
	if (m_gifImage)
	{
		del_gif(m_gifImage);
		m_gifImage = 0;
		delete[] m_gifBuffer;
		m_gifBuffer = 0;
		m_gifBufferTotalSize = 0;
	}
	m_bufferFrame = -1;
	m_gifImage = read_gif_file(stream);
	m_gifImage->calculateHelperData();
	delete stream;
	m_pStream = 0;
	
	SetFrames();
	updateGifImage();
	
	
	return XTRUE;
}

XBOOL XImage::Final(XU8 bBitmap)
{
	if (m_pStream == XNULL) return XFALSE;
	if (m_pImage != XNULL)
	{
		Release();
	}
	// ignore bBitmap
	XStream* stream = (XStream*)m_pStream;
	XU8* data = (XU8*)stream->ReadAddr();
	if (data[0] == 'G' && data[1] == 'I' && data[2] == 'F')
	{
		return loadFromGif(stream);
	}
#ifdef __ZCTESTimagedecode
    int len = stream->GetSize();
    int w,h;
    int comp;
    stbi_convert_iphone_png_to_rgb(XTRUE);
    XU8* buf = stbi_load_from_memory(stream->ReadAddr(), len, &w, &h, &comp, STBI_rgb_alpha);
    delete stream;
    m_pStream = NULL;
    if(m_pImage)
    {
        [(UIImage*)m_pImage release];
        m_pImage = NULL;
    }
    
    CGDataProviderRef ref = CGDataProviderCreateWithData(NULL, buf, w*h*comp, NULL);
    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
    CGImageRef imgref = CGImageCreate(w,
                                      h,
                                      8,
                                      32,
                                      w*4,
                                      colorspace,
                                      kCGBitmapByteOrder32Little|kCGImageAlphaFirst,
                                      ref,
                                      NULL,
                                      true,
                                      kCGRenderingIntentDefault);
    UIImage *img = [[UIImage alloc] initWithCGImage:imgref];
    m_pImage = img;
//    delete buf;
    CGImageRelease(imgref);
    CGColorSpaceRelease(colorspace);
    CGDataProviderRelease(ref);
    
//    CGDataProviderRef ref = CGDataProviderCreateWithData(NULL, newBuffer, totalSize, NULL);
//	CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
//	CGImageRef iref = CGImageCreate(width, height, 8, 32, width * 4, colorspace, kCGBitmapByteOrder32Little | kCGImageAlphaFirst,
//									ref, NULL, true, kCGRenderingIntentDefault);
//	
//	UIImage* img = [[UIImage alloc] initWithCGImage:iref];
//	m_pImage = img;
//	CGImageRelease(iref);
//	CGColorSpaceRelease(colorspace);
//	CGDataProviderRelease(ref);
    
#else//__ZCTESTimagedecode
	// non gif
	NSData* ns_data = [[NSData alloc] initWithBytes:stream->ReadAddr() length:stream->GetDataSize()];
	UIImage* img = [[UIImage alloc] initWithData:ns_data];
	if (img == nil)
	{
		return XFALSE;
	}
	m_pImage = img;
	delete stream;
	m_pStream = 0;
#endif//__ZCTESTimagedecode
	
	SetFrames();
	return XTRUE;
}

XBOOL XImage::Append(const void *pData, XINT nSize)
{
	if (m_pStream == XNULL)
	{
		XStream* stream = new XStream();
		m_pStream = stream;
	}
	assert(m_pStream);
	((XStream*)m_pStream)->WriteData(pData, nSize);
	return XTRUE;
}

void XImage::SetFrames()
{
	// if gif, do some thing.
	m_curFrame = 0;
	m_nFrames = 1;
	if (m_gifImage)
	{
		m_nFrames = m_gifImage->getFrames();
		InitDelay();
	}
}

XBOOL XImage::CreateFromResource(XPCTSTR strName,XBOOL bBitmap)
{
	return XResource::LoadImage(this,strName,bBitmap); 
}


#define DEFAULT_DELAY 4

void XImage::InitDelay()
{
	//if is gif. do some thing.
	if (m_gifImage)
	{
		m_pDelay = new XU8[m_nFrames];
		if (m_gifImage->m_data.empty())
		{
			for (XU32 i = 0; i < m_nFrames; ++i)
			{
				m_pDelay[i] = DEFAULT_DELAY;
			}
		}
		else
		{
			assert(m_gifImage->m_data.size() == m_nFrames);
			for (XU32 i = 0; i< m_nFrames; ++i)
			{
				m_pDelay[i] = m_gifImage->m_data[i].m_delayTime / 5;
			}
		}

	}
}

XU8 XImage::GetFrameDelay(int id)
{
	if(m_pDelay==XNULL) return DEFAULT_DELAY;
	else if(id<0||id>m_nFrames) return DEFAULT_DELAY;
	return m_pDelay[id];
}

void XImage::ResetStream()
{
	if(m_pStream==XNULL) return ;
	((XStream*)m_pStream)->Reset();
	m_pStream=XNULL; 
	//SetFrames();
}

XBOOL XImage::GetPalette(XU32 rgba[], int nCount)
{
	assert(false);
	return XFALSE;
	//ColorPalette pal={0,256,rgba};
}

XU8 XImage::GetPixelBits()
{
	// only support 32bit image now. 
	return 32;
	// modified by handong
	if (IsEmpty()) return 0;

	UIImage* uiImage = (UIImage*)m_pImage;
	XU8 bits = CGImageGetBitsPerPixel([uiImage CGImage]);
	return bits;
}

XBOOL XImage::GetBitmapData(XU8 *pBuffer, XU32 nLength)
{
	if (IsEmpty())
        return XFALSE;
	UIImage* uiImage = (UIImage*)m_pImage;
	CGSize size = [uiImage size];
	int shouldSize = (int)size.width * (int)size.height * 4;
	APPLEASSERT(shouldSize == nLength);
	if (shouldSize != nLength)
        return XFALSE;
	
    memset(pBuffer, 0, nLength);
	if (m_gifImage)
	{
		memcpy(pBuffer, m_gifBuffer, nLength);
		return XTRUE;
	}
	CGImage* cgImage = [uiImage CGImage];

    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(pBuffer,
                                                 size.width,
                                                 size.height,
                                                 8,
                                                 size.width * 4,
                                                 colorspace,
                                                 kCGBitmapByteOrder32Little |
                                                 kCGImageAlphaPremultipliedFirst);
	CGContextDrawImage(context, CGRectMake(0, 0, size.width, size.height), cgImage);
	CGContextRelease(context);
    CGColorSpaceRelease(colorspace);

	return XTRUE;
}

XBOOL XImage::GetBitmapDataSkip(XU8*pData,int wi,int hi,int nBitsi,int nSkip)//XDib &dib)
{
	if (IsEmpty()) return XFALSE;
	if (nBitsi!=32) return XFALSE;
	UIImage* uiImage = (UIImage*)m_pImage;
	int w = [uiImage size].width;
	int h = [uiImage size].height;
	int nBits=GetPixelBits();
	int iPitch = (((((w*nBits)>>3)+3)>>2)<<2)<<nSkip;
	int shouldSize = w * h * 4;
	XU8* data = new unsigned char[shouldSize];
	if (m_gifImage)
		memcpy(data, m_gifBuffer, shouldSize);
	CGImage* cgImage = [uiImage CGImage];
	CGContextRef context = CGBitmapContextCreate(data, w, h, 8, w * 4, CGImageGetColorSpace(cgImage), kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
	CGContextDrawImage(context, CGRectMake(0, 0, w, h), cgImage);
	CGContextRelease(context);
	
	XU8*pSrt=((XU8*)data)+(iPitch/2)+((nBits<<nSkip)>>4);
	XU32*pDst=(XU32*)pData;
	int nSkip16=1+nSkip;
	int nSkip24=3<<nSkip;
	int nSkip32=2+nSkip;
	if(hi<<nSkip>h) hi=h>>nSkip;
	if(wi<<nSkip>w) hi=w>>nSkip;
	for(int y=0;y<hi;y++)
	{
		int x1=0;
		for(int x=0;x<wi;x++)
		{
			switch(nBits)
			{
				case 8:
					// 
					break;
				case 16:
				{
					XU16 color=*((XU16*)(pSrt+(x<<nSkip16)));
					
					XU32 c;
					COLOR16TO32(color,c);
					pDst[x]=c;
				}
					break;
				case 24:
				{
					XU8*p=(pSrt+x1);
					x1+=nSkip24;
					XU32 c;
					COLOR24TO32(p,c);
					pDst[x]=c;
				}
					break;
				case 32:
				{
					pDst[x]=*(XU32*)(pSrt+(x<<nSkip32));
				}
					break;
			}
		}
		pSrt+=iPitch;
		pDst+=wi;
		//pDst+=dib.Width();
	}
	delete[] data;
    return XTRUE;
}

XBOOL XImage::GetBitmapData(XU8*pData,int wi,int hi,int nBitsi,int nLength)//XDib &dib)
{
	if (IsEmpty())
        return XFALSE;
	UIImage* uiImage = (UIImage*)m_pImage;
	CGSize size = [uiImage size];
	if (wi == (int)size.width && hi == (int)size.height)
        return GetBitmapData(pData, nLength);
	APPLEASSERT((wi * hi * nBitsi / 8) == nLength);
	if ((wi * hi * nBitsi / 8) != nLength)
		return XFALSE;
	APPLEASSERT(nBitsi == 32);
	if (nBitsi != 32)
        return XFALSE;
	int shouldSize = wi * hi * 4;
	APPLEASSERT(shouldSize > 0);
	if (shouldSize <= 0)
        return XFALSE;
	
	CGImage* cgImage = [uiImage CGImage];
    memset(pData, 0, nLength);
	CGContextRef context = CGBitmapContextCreate(pData,
                                                 wi,
                                                 hi,
                                                 8,
                                                 wi * 4,
                                                 CGImageGetColorSpace(cgImage),
                                                 kCGImageAlphaFirst);
	UIGraphicsPushContext(context);
	[uiImage drawAtPoint:CGPointMake(0, 0)];
	UIGraphicsPopContext();
	CGContextRelease(context);
	return XTRUE;
}

void* XImage::decompress(void* img)
{
	CGImageRef imageRef = (CGImageRef)img;
	
	size_t width = CGImageGetWidth(imageRef);
    size_t height = CGImageGetHeight(imageRef);
	
    CGContextRef    context = NULL;
    CGColorSpaceRef colorSpace;
    int             bitmapByteCount;
    int             bitmapBytesPerRow;
	
    bitmapBytesPerRow   = (width * 4);
    bitmapByteCount     = (bitmapBytesPerRow * height);
	
    colorSpace = CGColorSpaceCreateDeviceRGB();
    context = CGBitmapContextCreate (NULL,
									 width,
									 height,
									 8,
									 bitmapBytesPerRow,
									 colorSpace,
									 kCGImageAlphaPremultipliedLast);
    CGColorSpaceRelease( colorSpace );
	
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    CGImageRef result = CGBitmapContextCreateImage(context);
    CFRelease(context);
    return result;
}
