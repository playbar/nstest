#include "StdAfxGGBase.h"
#include "XFontDraw.h"
#include "XString.h"
//#include <math.h>
#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>

XFontDraw* XFontDraw::m_pInstance = NULL;
#define _FONTBLOCK 256*256*4
#define _FONTALPHABLOCK 256*256
static XU8 _fontData[_FONTBLOCK];
static XU8 _fontAlphaData[_FONTALPHABLOCK];

static CGSize _calculateStringSize( NSString *str, id font )
{
    NSArray *listItems = [str componentsSeparatedByString:@"\n"];
    CGSize dim = CGSizeZero;
    CGSize textRect = CGSizeZero;
    textRect.width = 0xffff;
    textRect.height = 0xffff;
    
    for (NSString *s in listItems )
    {
        CGSize tmp = [s sizeWithFont:font constrainedToSize:textRect ];
        if ( tmp.width > dim.width ) {
            dim.width = tmp.width;
        }
        dim.height += tmp.height;
    }
    
    return dim;
}

NSString* WStrToNSStr(XPCWSTR pText, int nLength)
{
    if (nLength <= 0)
        nLength = gg_wcslen(pText);
    NSData* data = [[NSData alloc] initWithBytes:pText length:nLength * 2];
    NSString* str = [[NSString alloc] initWithData:data encoding:NSUTF16LittleEndianStringEncoding];
    [data release];
    return str;
}

XFontDraw::XFontDraw()
{
	//m_pFont = new CFont();
	//m_fontSize = 0;
	//m_fontID = 0;
}

XFontDraw::~XFontDraw()
{
//	if(m_pFont)
//	{
//		CFont* pFont=(CFont*)m_pFont;
//		pFont->DeleteObject();
//		delete (CFont*)m_pFont;
//	}
}

void XFontDraw::SetFont(const char* fontName,int id,int nSize)
{
   if(m_fontID == id && m_fontSize==nSize)
       return;
    m_fontID = id;
    m_fontSize = nSize;
    if(!fontName)
    {
        m_pFont = nil;
        return;
    }
    
    NSString *fntName = [NSString stringWithUTF8String:fontName ];
    
    fntName = [[fntName lastPathComponent] stringByDeletingPathExtension ];
    
    m_pFont = [UIFont fontWithName:fntName size:nSize ];

}

bool XFontDraw::Draw(XPCWSTR pText, int len,_FONTINFO&info)
{
	if(m_fontSize<=0)
        return false;
    
    CGSize dim;
    
    //NSString *str = [NSString stringWithUTF8String:pText ];
    NSString *str = WStrToNSStr(pText, 0 );
    
    if ( m_pFont )
    {
        dim = _calculateStringSize(str, (id)m_pFont );
    }
    else{
        if ( !m_pFont ) {
            m_pFont = [UIFont systemFontOfSize:m_fontSize ];
        }
        if ( m_pFont ) {
            dim = _calculateStringSize(str, (id)m_pFont );
        }
    }
    
    int width = ((int)dim.width+3)/4*4;
    int height = m_fontSize;//(int)dim.height;
    XU8 *data = _fontData;
    bool bAlloc = false;
    if ( width * height * 4 > _FONTBLOCK )
    {
        data = new XU8[(width *height * 4)];
        bAlloc = true;
    }
    memset( data, 0, (width * height * 4 ));
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(data, width, height, 8, width * 4, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    CGColorSpaceRelease( colorSpace );
    
    if ( !context ) {
        if (bAlloc)
            delete []data;
        return false;
    }
    CGContextSetRGBFillColor(context, 1, 1, 1, 1);
    CGContextTranslateCTM( context, 0.0f, dim.height );
    CGContextScaleCTM(context, 1.0f, -1.0f);
    UIGraphicsPushContext(context );
    
    UITextAlignment align = UITextAlignmentCenter;
    
    //int startH = dim.height;
    
    [str drawInRect:CGRectMake(0, 0, width, height) withFont:(id)m_pFont
      lineBreakMode:(UILineBreakMode)UILineBreakModeClip alignment:align ];
    
    UIGraphicsPopContext();
    CGContextRelease( context );
/*
    CGImageRef imageRef = CGBitmapContextCreateImage(context);
    CGContextDrawImage(context, boundingBox, imageRef);
    void *bitmapData = CGBitmapContextGetData(context);
    CGColorSpaceRelease( colorSpace );
    CGContextRelease( context );
    CGImageRelease(imageRef);
*/
    int l = width * height;
    if (l>_FONTALPHABLOCK)
    {
        info.pData = new XU8[l];
        info.bAlloc = true;
    }
    else
    {
        info.pData = _fontAlphaData;
        info.bAlloc = false;
    }
    XU8* pSrc = data+3;
    XU8* pDst = info.pData;
    for (int i = 0; i < l; i++)
    {
        //if (*pSrc)
        {
            pDst[i] = *pSrc;
            //pDst[i+1] = 255;
        }
        //printf(" %d",*pSrc?1:0 );
        //if ((1+i)%(width) == 0) printf("\n");
        pSrc += 4;
    }

    //NSLog(@"%@: size %d",str,m_fontSize);
    if (bAlloc)
        delete []data;
    //info.pData = data;
    info.fw = dim.width;
    info.w = width;
    info.h = height;
    //info.bAlloc = false;
    

	return true;
}
