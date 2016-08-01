// XGraphics.cpp: implementation of the XGraphics class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XGraphics.h"
#include "XString.h"
#include "string_comp.h"
#include "IPhoneDeviceData.h"
#include "XResource.h"
#include "XWindow.h"


#define USE_BLEND_MODE kCGBlendModeNormal

extern iOSDeviceRelateData g_deviceData;
extern iOSDeviceType g_deviceType;

#define _strFont _XW("新宋体")
//Times New Roman")
//宋体")

#define _XGDIDRAW


inline CGRect XRectToCGRect(const XRect& rect, bool scale)
{
        if ((!g_deviceData.mbOnSale) && scale)
            return CGRectMake(rect.left/g_deviceData.m_scale, rect.top/g_deviceData.m_scale, (rect.right - rect.left)/g_deviceData.m_scale, (rect.bottom - rect.top)/g_deviceData.m_scale);
        else
            return CGRectMake(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
}


XGraphics::XGraphics()
{
	m_isPushContextForClip = false;
	m_width = 0;
	m_height = 0;
	m_originX = 0;
	m_originY = 0;
	m_pBitmap=XNULL;	
	m_pFont=XNULL;
	m_pRgn=XNULL;
	m_pGraphics=XNULL;
	Init();
}

XGraphics::XGraphics(void* context, int width, int height)
{
	m_isPushContextForClip = false;
	m_width = width;
	m_height = height;
	m_originX = 0;
	m_originY = 0;
	m_pBitmap = 0;
	m_pFont = 0;
	m_pRgn = 0;
	m_pGraphics = context;
	Init();
}


XGraphics::~XGraphics()
{
	Release();
}

void XGraphics::begin()
{
	BWASSERT(m_pGraphics);
	CGContextRef context = (CGContextRef)m_pGraphics;
	if (!context) return;
	UIGraphicsPushContext(context);
}

void XGraphics::end()
{
	BWASSERT(m_pGraphics);
	if (!m_pGraphics) return;
	UIGraphicsPopContext();
}

void XGraphics::clipToRectInGlobal(const XRect& rect)
{
	CGContextRef context = (CGContextRef)m_pGraphics;
	CGContextClipToRect(context, CGRectMake(rect.left - m_originX, rect.top - m_originY, rect.right - rect.left, rect.bottom - rect.top));
}

XGraphics::XGraphics(XHANDLE hHandle)
{
	m_isPushContextForClip = false;
	m_originX = 0;
	m_originY = 0;
	m_pGraphics=XNULL;
	m_pFont=XNULL;
	m_pRgn=XNULL;
	m_pBitmap=XNULL;
	m_nTextHeight=16;
	m_pGraphics = hHandle;
	Init();
}

NSString* WStringToNSString(XPCWSTR pText, int nLength)
{
	if (nLength <= 0)
		nLength = gg_wcslen(pText);
	NSData* data = [[NSData alloc] initWithBytes:pText length:nLength * 2];
	NSString* str = [[NSString alloc] initWithData:data encoding:NSUTF16LittleEndianStringEncoding];
	[data release];
	return str;
}

void XGraphics::DrawString(XPCWSTR pText, const XPoint &pt,XINT nLength)
{
//	GetFont();
//	BWASSERT(m_pFont);
//	if (!m_pFont) return;
//
//	if (nLength <= 0)
//		nLength = gg_wcslen(pText);
//	BWASSERT(gg_wcslen(pText) >= nLength);
//	
//	int y = pt.y - 2;
//
//	CGContextRef context = (CGContextRef)m_pGraphics;
//	
//	CGContextSetRGBStrokeColor(context, m_textColor[0], m_textColor[1], m_textColor[2], m_textColor[3]);
//	CGContextSetRGBFillColor(context, m_textColor[0], m_textColor[1], m_textColor[2], m_textColor[3]);
//	NSString* str = WStringToNSString(pText, nLength);
////	[str drawAtPoint:CGPointMake(pt.x, y) withFont:(UIFont*)m_pFont];
//    NSMutableAttributedString *mabstring = [[NSMutableAttributedString alloc]initWithString:str];
//	//CTFontRef font = CTFontCreateWithName(CFSTR("Georgia"), ((UIFont *)m_pFont).pointSize, NULL);
//    UIFont *fontRef = (UIFont *)m_pFont;
//	CTFontRef font = CTFontCreateWithName((CFStringRef)fontRef.fontName, fontRef.pointSize, NULL);
//    [mabstring addAttribute:(id)kCTFontAttributeName value:(id)font range:NSMakeRange(0, [str length])];
//    printf("%d\n",(int)((UIFont *)m_pFont).pointSize);
//    printf("----->%ld,%d\n", pt.x, y);
//    NSLog(@"%@",str);
//    CGColorSpaceRef rgbSpace = CGColorSpaceCreateDeviceRGB();
//    CGColorRef colorRGB = CGColorCreate(rgbSpace, m_textColor);
//    //CGColorCreateGenericRGB(context, m_textColor[0], m_textColor[1], m_textColor[2], m_textColor[3]);
//    CGColorSpaceRelease(rgbSpace);
//    [mabstring addAttribute:(id)kCTForegroundColorAttributeName value:(id)colorRGB range:NSMakeRange(0, [str length])];
//    if (m_nFontStyle & XF_UNDERLINE)
//    {
//        [mabstring addAttribute:(id)kCTUnderlineStyleAttributeName value:(id)[NSNumber numberWithInt:kCTUnderlineStyleSingle] range:NSMakeRange(0, [str length])];
//        [mabstring addAttribute:(id)kCTUnderlineColorAttributeName value:(id)colorRGB range:NSMakeRange(0, [str length])];
//    }
//    CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString((CFAttributedStringRef)mabstring);
//    
//    CGMutablePathRef Path = CGPathCreateMutable();
//    
//    CGPathAddRect(Path, NULL ,CGRectMake(pt.x , -y ,g_deviceData.m_screenWidth * 2 + ((UIFont *)m_pFont).pointSize, g_deviceData.m_screenHeight));
//    
//    CTFrameRef frame = CTFramesetterCreateFrame(framesetter, CFRangeMake(0, 0), Path, NULL);
//    
//    //获取当前(View)上下文以便于之后的绘画，这个是一个离屏。
//    CGContextSetTextMatrix(context , CGAffineTransformIdentity);
//    
//    //压栈，压入图形状态栈中.每个图形上下文维护一个图形状态栈，并不是所有的当前绘画环境的图形状态的元素都被保存。图形状态中不考虑当前路径，所以不保存
//    //保存现在得上下文图形状态。不管后续对context上绘制什么都不会影响真正得屏幕。
//    CGContextSaveGState(context);
//    
//    //x，y轴方向移动
//    CGContextTranslateCTM(context , 0 ,g_deviceData.m_screenHeight/2);
//    
//    //缩放x，y轴方向缩放，－1.0为反向1.0倍,坐标系转换,沿x轴翻转180度
//    CGContextScaleCTM(context, 0.5 ,-0.5);
//    CGContextTranslateCTM(context, m_xtr, m_ytr);
//    CTFrameDraw(frame,context);
//    
//    CGPathRelease(Path);
//    CFRelease(framesetter);
//    
//    CGContextRestoreGState(context);
//    
//	// draw line
//	if (m_nFontStyle & XF_UNDERLINE)
//	{
//		// draw line
////		UIFont* font = (UIFont*)m_pFont;
////		CGSize size = [str sizeWithFont:font];
////		DrawLine(pt.x, y + size.height - 2, pt.x + size.width, y + size.height - 2);
//	}
//	[str release];
    
    GetFont();
	BWASSERT(m_pFont);
	if (!m_pFont) return;
    
	if (nLength <= 0)
		nLength = gg_wcslen(pText);
	BWASSERT(gg_wcslen(pText) >= nLength);
	
	int y = pt.y - 2;
    
	CGContextRef context = (CGContextRef)m_pGraphics;
	
	CGContextSetRGBStrokeColor(context, m_textColor[0], m_textColor[1], m_textColor[2], m_textColor[3]);
	CGContextSetRGBFillColor(context, m_textColor[0], m_textColor[1], m_textColor[2], m_textColor[3]);
	NSString* str = WStringToNSString(pText, nLength);
    CGContextSaveGState(context);
    if (!g_deviceData.mbOnSale)
    {
        CGContextScaleCTM(context, 1/g_deviceData.m_scale , 1/g_deviceData.m_scale);
    }
	[str drawAtPoint:CGPointMake(pt.x, y) withFont:(UIFont*)m_pFont];
	CGContextRestoreGState(context);
	// draw line
	if (m_nFontStyle & XF_UNDERLINE)
	{
		// draw line
		UIFont* font = (UIFont*)m_pFont;
		CGSize size = [str sizeWithFont:font];
		DrawLine(pt.x, y + size.height - 2, pt.x + size.width, y + size.height - 2);
	}
	[str release];
}

void XGraphics::DrawString(XPCWSTR pText, const XRect &rect,XINT nLength)
{

    GetFont();
	BWASSERT(m_pFont);
	if (!m_pFont)
        return;
	if (nLength <= 0)
		nLength = gg_wcslen(pText);
	BWASSERT(gg_wcslen(pText) >= nLength);
	CGContextRef context = (CGContextRef)m_pGraphics;
	
	CGContextSetRGBStrokeColor(context, m_textColor[0], m_textColor[1], m_textColor[2], m_textColor[3]);
	CGContextSetRGBFillColor(context, m_textColor[0], m_textColor[1], m_textColor[2], m_textColor[3]);
	NSString* str = WStringToNSString(pText, nLength);
    CGContextSaveGState(context);
    if (!g_deviceData.mbOnSale)
    {
        CGContextScaleCTM(context, 1/g_deviceData.m_scale , 1/g_deviceData.m_scale);
    }
	[str drawInRect:XRectToCGRect(rect, false) withFont:(UIFont*)m_pFont];
    CGContextRestoreGState(context);
	[str release];
}

void XGraphics::DrawString(XPCWSTR pText, int x,int y,XINT nLength)
{
	DrawString(pText, XPoint(x, y), nLength);
}


void XGraphics::MeasureString(XPCWSTR pText,XSIZE &size,XINT nLength)
{
	GetFont();
	size.cx = 0;
	size.cy = 0;
	assert(m_pFont);
	if (!m_pFont)
        return;
	
	if (nLength == 0)
	{
		size.cy = m_nTextHeight;
		size.cx = 0;
		return;
	}

	if (nLength < 0)
		nLength = gg_wcslen(pText);
	
	assert(gg_wcslen(pText) >= nLength);

	NSString* str = WStringToNSString(pText, nLength);
	CGSize cgSize = [str sizeWithFont:(UIFont*)m_pFont];
	size.cx = cgSize.width;
	size.cy = m_nTextHeight;
	[str release];
}

void XGraphics::FillRectangle(const XRect &rect)
{
	CGContextRef context = (CGContextRef)m_pGraphics;
	BWASSERT(context);
	CGContextSetRGBFillColor(context, m_backColor[0], m_backColor[1], m_backColor[2], m_backColor[3]);
	CGContextFillRect(context, XRectToCGRect(rect, true));
}

void XGraphics::DrawImage(const XImage &img,const XRect &rect)
{
	UIImage* uiImage = (UIImage*)img.GetImage();
	CGContextRef context = (CGContextRef)m_pGraphics;
	assert(context);
	assert(uiImage);
	[uiImage drawInRect:XRectToCGRect(rect, true)];
}

void XGraphics::DrawImage(const XImage&img,const XPoint&dst,const XRect&rst)
{

	UIImage* uiImage = (UIImage*)img.GetImage();
	CGContextRef context = (CGContextRef)m_pGraphics;
	assert(context);
	assert(uiImage);
	CGContextSaveGState(context);
    CGImageRef imagRef = CGImageCreateWithImageInRect([uiImage CGImage], CGRectMake(rst.left, rst.top, rst.right, rst.bottom));
	UIImage *imag = [UIImage imageWithCGImage:imagRef];
    if (!g_deviceData.mbOnSale)
    {
        [imag drawInRect:CGRectMake(dst.x/g_deviceData.m_scale, dst.y/g_deviceData.m_scale, (rst.right - rst.left+ g_deviceData.m_scale - 1)/g_deviceData.m_scale, (rst.bottom - rst.top + g_deviceData.m_scale -1)/g_deviceData.m_scale) blendMode:kCGBlendModeNormal alpha:1];
    }
	else
    {
        [imag drawInRect:CGRectMake(dst.x, dst.y, rst.right - rst.left, rst.bottom - rst.top) blendMode:kCGBlendModeNormal alpha:1];
    }
	CGContextRestoreGState(context);
}

void XGraphics::DrawImage(const XImage &img,const XPoint &point,int rotate)
{
	UIImage* uiImage = (UIImage*)(img.GetImage());
	CGContextRef context = (CGContextRef)m_pGraphics;
	assert(context);
	//assert(uiImage);
    if ( uiImage == NULL ) {
        return;
    }
	CGContextSaveGState(context);
	int width = img.Width();
	int height = img.Height();
	int x = point.x;
	int y = point.y;
	CGContextConcatCTM(context, CGAffineTransformMakeTranslation(x + width / 2.0, y + height / 2.0));
	CGContextConcatCTM(context, CGAffineTransformMakeRotation(rotate * 3.1415 / 180));
	CGContextConcatCTM(context, CGAffineTransformMakeTranslation(-x - width / 2.0, -y - height / 2.0));
    if (!g_deviceData.mbOnSale)
    {
        CGContextConcatCTM(context, CGAffineTransformMakeScale(1/g_deviceData.m_scale, 1/g_deviceData.m_scale));
    }
	[uiImage drawAtPoint:CGPointMake(point.x, point.y) blendMode:USE_BLEND_MODE alpha:1];
	CGContextRestoreGState(context);
}

void XGraphics::FrameRectangle(const XRect &rect)
{	
	CGContextRef context = (CGContextRef)m_pGraphics;
	assert(context);
	CGContextSetRGBStrokeColor(context, m_textColor[0], m_textColor[1], m_textColor[2], m_textColor[3]);
    CGContextStrokeRectWithWidth(context, CGRectInset(XRectToCGRect(rect, true), 1.0, 1.0), 1);
}

void XGraphics::DrawLine(int sx, int sy, int ex, int ey,XU8 w,XU8 nStyle)
{
	CGContextRef context = (CGContextRef)m_pGraphics;
	assert(context);
	CGContextSetRGBStrokeColor(context, m_textColor[0], m_textColor[1], m_textColor[2], m_textColor[3]);
//	CGContextSetRGBStrokeColor(context, 1.0, 1.0, 1.0, 1.0);
	CGContextSetLineWidth(context, 1.0); 
	
	// Draw a single line from left to right
    if (!g_deviceData.mbOnSale)
    {
        CGContextMoveToPoint(context, sx/g_deviceData.m_scale, sy/g_deviceData.m_scale);
        CGContextAddLineToPoint(context, ex/g_deviceData.m_scale, ey/g_deviceData.m_scale);
    }
	else
    {
        CGContextMoveToPoint(context, sx, sy);
        CGContextAddLineToPoint(context, ex, ey);
    }
	CGContextStrokePath(context);
}

void XGraphics::DrawButton(const XRect &r, XU32 c,XU8 b)
{
   XRect rect=r;
   XColor cr(c);
   if(!(b&XBS_NOFILL))
   {
	SetBackColor(cr);
	FillRectangle(rect);
   }
   //((Graphics*)m_pGraphics)->FillRectangle(brush,(Rect)rect);
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
   else {rect.right--;rect.bottom--;}
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
   //if(b&XBS_HHAND)
   {
	   for(int i=0;i<3;i++)
	   {
		  if(b&XBS_HHAND)
			DrawLine(rect.left+2,rect.top+(cy>>1)-3+i*3,rect.right-3,rect.top+(cy>>1)-3+i*3);
		  else if(b&XBS_VHAND)
			DrawLine(rect.left+(cx>>1)-3+i*3,rect.top+2,rect.left+(cx>>1)-3+i*3,rect.bottom-3);
	   }
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

   {
	   for(int i=0;i<3;i++)
	   {
		  if(b&XBS_HHAND)
			DrawLine(rect.left+3,rect.top+(cy>>1)-2+i*3,rect.right-2,rect.top+(cy>>1)-2+i*3);
		  else if(b&XBS_VHAND)
			DrawLine(rect.left+(cx>>1)-2+i*3,rect.top+3,rect.left+(cx>>1)-2+i*3,rect.bottom-2);
	   }
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
   m_clip=rect;
	CGContextRef context = (CGContextRef)m_pGraphics;
	assert(context);
	if (m_isPushContextForClip)
	{
		CGContextRestoreGState(context);

		SetOffsetForClipErrorAdjust(m_originX - m_originXWhenPushContextForClip, m_originY - m_originYWhenPushContextForClip);
	}
	CGContextSaveGState(context);
	CGContextClipToRect(context, XRectToCGRect(rect, true));
	m_isPushContextForClip = true;
	m_originXWhenPushContextForClip = m_originX;
	m_originYWhenPushContextForClip = m_originY;
}

void XGraphics::GetClip(XRect &clip)
{
   clip=m_clip;
}

void XGraphics::SetOffsetForClipErrorAdjust(XINT x, XINT y)
{
	CGContextRef context = (CGContextRef)m_pGraphics;
	CGContextTranslateCTM(context, x, y);
}

void XGraphics::SetOrigin(XINT x, XINT y)
{
    XINT x1 = x, y1 = y;
    if (!g_deviceData.mbOnSale)
    {
        x1/=g_deviceData.m_scale;
        y1/=g_deviceData.m_scale;
    }
	CGContextRef context = (CGContextRef)m_pGraphics;
	CGContextTranslateCTM(context, x1 - m_originX, y1 - m_originY);
	m_originX = x1;
	m_originY = y1;
}

void XGraphics::GetOrigin(XINT&x,XINT&y)
{
	x = m_originX;
	y = m_originY;
    if (!g_deviceData.mbOnSale)
    {
        x*=g_deviceData.m_scale;
        y*=g_deviceData.m_scale;
    }
}

void XGraphics::DrawDirect(const XRect&rect,XU8 nDirect, XU32 c,XU8 bGray)
{
	XColor cc(c);
	cc.Bright(bGray?50:70);
	SetColor(cc);
	int dd=(rect.Width()-8)>>1;
	int bx=rect.left+4,by=rect.bottom-4;
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
	//cc.Dark(bGray?20:50);
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
	int dd=rect.Width()/3;
	
	for(int i=0;i<3;i++)
	{		
		if(i!=1)
			SetColor(c1);
		else SetColor(c);
		DrawLine(rect.left,rect.bottom-dd-i,rect.left+dd,rect.bottom-i);
		DrawLine(rect.left+dd,rect.bottom-i,rect.left+dd*3,rect.bottom-i-dd*2);
	}
}


#define ISALPHA(CC) (CC<0x80&&::isalpha(CC))
#define ISNUM(CC) (CC<0x80&&::isdigit(CC))
#define ISMARK(CC) (::gg_wcschr(_strMark,CC)!=XNULL)
#define ISLEFTMARK(CC) (gg_wcschr(_strLeftMark,CC)!=XNULL)

static const XWCHAR* _strMark=_XW(",.?:;'!)]}>，。？”’〉；：、》」』】〕〗〉！");
static const XWCHAR* _strLeftMark=_XW("<({[“‘〈《「『【〔〖");

XU16 XGraphics::FormatString(XPCWSTR pData,int len,int bx, int win, DOMPOSARRAY &list,XU8 bWord,DOMPOS*pSpan)
{
	XSIZE sz;//(0,0);
	sz.cx=0;
	sz.cy=0;
	
	
	XU16 nMax=(XU16)(XMIN(len,0xfffe));
	XU8  bStart=bx==0;
	
	int w=win,ssy=0;
	if(pSpan!=XNULL)
	{
	   if(ssy<pSpan[3])
           w=pSpan[2];
	}
	XU16 iBegin=0,ei=XMIN(XMAX(2+((w-bx)>>5),2),nMax),si=0,mi=ei,ci;
	while(iBegin<nMax)
	{
		MeasureString(pData,sz,ei);
		ci=ei;
		while(sz.cx+bx<=w)
		{
			si=ei;
			if(ei+iBegin>=nMax) break;
		    ei=XMIN(ei+mi,nMax-iBegin);			
			MeasureString(pData,sz,ei);
			ci=ei;
		}
		mi=(ei+si)>>1;
		while(ei>si+1)
		{
			MeasureString(pData,sz,mi);
			ci=mi;
			if(sz.cx+bx>w)
			{
				ei=mi;
			}
			else
				si=mi;
			mi=(ei+si)>>1;
		}
		ei=mi;
		if(XFALSE)
		{
			if(ISALPHA(pData[si-1]))
			{
			  mi=si;
			  while(mi>0&&(ISALPHA(pData[mi-1])||ISLEFTMARK(pData[mi-1]))) 
				  mi--;
			  if(mi>0)
                  si=mi;
			}
		   else if(ISNUM(pData[si-1]))
		   {
			  mi=si;
			  while(mi>1&&(ISNUM(pData[mi-1])||ISLEFTMARK(pData[mi-1])))
				  mi--;
			  if(mi>1)
                  si=mi;
		   }
		   else if(si+iBegin<nMax&&ISMARK(pData[si]))
		   {
			   if(si+1+iBegin>=nMax||
				  !ISMARK(pData[si+1]))
			   si++;
		   }
		   else if(si>2&&ISLEFTMARK(pData[si-1]))
			   si--;
		}

		if(si==0&&bStart)
            si++;
		//if(si>0)
		{
			if(si!=ci) MeasureString(pData,sz,si);
			list.Add(iBegin);
			list.Add(si);
			list.Add(si>0?sz.cx:0);
		}

		bStart=XTRUE;
		if(pSpan!=XNULL)
		{
		   ssy+=sz.cy;
		   if(ssy<pSpan[3]) w=pSpan[2];
		   else w=win;
		   if(ssy<pSpan[1]) bx=pSpan[0];
		   else bx=0;
		}
		else
			bx=0;
		bStart=XTRUE;
		iBegin+=si;
		pData+=si;
		ei=XMIN(2+((XMAX(w-bx,0))>>5),nMax-iBegin);
		mi=ei;
		si=0;
		if((bWord&XFMT_ONELINE)&&list.GetSize()>0) return (XU16)sz.cy;
	}
	return (XU16)sz.cy;
}

void XGraphics::DrawFrame(const XRect &rect, XU32 c, XU8 s,XU8 bSet)
{
	XRect r=rect;
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
	int totalSize = m_width * m_height * 4;
	unsigned char* newBuffer = new unsigned char[totalSize];
	memcpy(newBuffer, m_pBitmap, totalSize);
	
	CGDataProviderRef ref = CGDataProviderCreateWithData(NULL, newBuffer, totalSize, NULL);
	CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
	CGImageRef iref = CGImageCreate(m_width, m_height, 8, 32, m_width * 4, colorspace, kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast,
									ref, NULL, true, kCGRenderingIntentDefault);
	CGContextRef context = (CGContextRef)m_pGraphics;
	
	CGContextSaveGState(context);
	clipToRectInGlobal(XRect(dst.x, dst.y, dst.x + (rst.right - rst.left), dst.y + (rst.bottom - rst.top)));
//	CGContextClipToRect(context, CGRectMake(dst.x, dst.y, rst.right - rst.left, rst.bottom - rst.top));
	CGContextSetBlendMode(context, USE_BLEND_MODE);
	CGContextDrawImage(context, CGRectMake(dst.x, dst.y, m_width, m_height), iref);
	
	CGContextRestoreGState(context);
	CGImageRelease(iref);
	CGColorSpaceRelease(colorspace);
	CGDataProviderRelease(ref);
	delete[] newBuffer;
	return XTRUE;
}


void XGraphics::DrawRadio(const XRect &inRect, XU32 nColor,XU8 bSelect)
{
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
		 rect.top--;
		 rect.bottom--;
		 cc=c;
		 cc.Dark(80);
		 DrawCheck(rect,cc,cg);
	 }
}

void XGraphics::DrawEllipse(const XRect &rect)
{
	CGContextRef context = (CGContextRef)m_pGraphics;
	assert(context);
	CGContextSetRGBStrokeColor(context, m_textColor[0], m_textColor[1], m_textColor[2], m_textColor[3]);
	CGContextStrokeEllipseInRect(context, XRectToCGRect(rect, true));
}

void XGraphics::FillEllipse(const XRect &rect)
{
	CGContextRef context = (CGContextRef)m_pGraphics;
	CGContextSetRGBFillColor(context, m_backColor[0], m_backColor[1], m_backColor[2], m_backColor[3]);
	CGContextFillEllipseInRect(context, XRectToCGRect(rect, true));
}

XGraphics::XGraphics(int w, int h)
{
	m_isPushContextForClip = false;
	m_width = 0;
	m_height = 0;
	m_pBitmap=XNULL;
	m_pFont=XNULL;
	m_pRgn=XNULL;
	m_pGraphics=XNULL;
	Create(w,h);
	Init();
}

void XGraphics::Create(int w, int h,XU8 nBits)
{
	// bitmap is device related. in iphone. it will be a CGBitmap.
	Release();
	m_width = w;
	m_height = h;
	// ignore nBits. use 32 bit now.
	int totalSize = w * h * 4;
	m_pBitmap = new unsigned char[totalSize];
	memset(m_pBitmap, 0, totalSize);
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	CGContextRef bmpContext = CGBitmapContextCreate(m_pBitmap, w, h, 8, w * 4, colorSpace, kCGImageAlphaPremultipliedLast);
	m_pGraphics = bmpContext;
	
	CGContextSetRGBStrokeColor(bmpContext, 1, 1, 1, 1);
	CGContextSetRGBFillColor(bmpContext, 1, 1, 1, 1);
	
	CGColorSpaceRelease(colorSpace);
}

XBOOL XGraphics::Copy(XGraphics&g, const XPoint&dst, const XRect&rst)
{
	if(g.m_pBitmap==XNULL)
        return XFALSE;

	int totalSize = g.m_width * g.m_height * 4;
	
	CGDataProviderRef ref = CGDataProviderCreateWithData(NULL, g.m_pBitmap, totalSize, NULL);
	CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
	CGImageRef iref = CGImageCreate(g.m_width, g.m_height, 8, 32, g.m_width * 4, colorspace, kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast,
									ref, NULL, true, kCGRenderingIntentDefault);
	CGContextRef context = (CGContextRef)m_pGraphics;
	
	CGContextSaveGState(context);
//	CGContextClipToRect(context, CGRectMake(dst.x, dst.y, rst.right - rst.left, rst.bottom - rst.top));
//	CGContextSetBlendMode(context, kCGBlendModeOverlay);
	CGContextDrawImage(context, CGRectMake(dst.x, dst.y, rst.right - rst.left, rst.bottom - rst.top), iref);
	
	CGContextRestoreGState(context);
	CGImageRelease(iref);
	CGColorSpaceRelease(colorspace);
	CGDataProviderRelease(ref);
	return XTRUE;
}

XU32 XGraphics::GetPixel(int x, int y)
{
    if(m_pBitmap==XNULL)
        return 0;

	assert(x >= 0 && x < m_width);
	assert(y >= 0 && y < m_height);
	if (x < 0 || x >= m_width) return 0;
	if (y < 0 || y > m_height) return 0;
	unsigned char* p = (unsigned char*)m_pBitmap;
	p += y * m_width * 4 + x * 4;
	XU32 ret = p[2];
	ret &= p[1] << 8;
	ret &= p[0] << 16;
	ret &= p[3] << 24;
	return ret;
}

XU32 XGraphics::SetColor(XU32 nColor) 
{
		XU32 c=m_nTextColor;
		m_nTextColor=nColor;
		m_textColor[0] = ((nColor >> 16) & 0xff) * (1.0 / 255.0);
		m_textColor[1] = ((nColor >> 8) & 0xff) * (1.0 / 255.0);
		m_textColor[2] = ((nColor) & 0xff) * (1.0 / 255.0);
		m_textColor[3] = ((nColor >> 24) & 0xff) * (1.0 / 255.0);
		return c;
}

XU32 XGraphics::SetBackColor(XU32 nColor)
{
	XU32 c=m_nBackColor;
	m_nBackColor=nColor;
	m_backColor[0] = ((nColor >> 16) & 0xff) * (1.0 / 255.0);
	m_backColor[1] = ((nColor >> 8) & 0xff) * (1.0 / 255.0);
	m_backColor[2] = ((nColor) & 0xff) * (1.0 / 255.0);
	m_backColor[3] = ((nColor >> 24) & 0xff) * (1.0 / 255.0);
	return c;
}


void XGraphics::Release()
{
	if (m_isPushContextForClip)
	{
		if (m_pGraphics)
		{
			CGContextRestoreGState((CGContextRef)m_pGraphics);
		}
		m_isPushContextForClip = false;
	}
	
	if (m_pFont)
	{
		[(UIFont*)m_pFont release];
		m_pFont = 0;
	}
	if (m_pBitmap)
	{
		CGContextRelease((CGContextRef)m_pGraphics);
		delete[] (unsigned char*)m_pBitmap;
		m_pBitmap = 0;
	}
	m_pGraphics = 0;
	
	
	m_width = 0;
	m_height = 0;
}

void XGraphics::Init()
{
//	m_nTextHeight=18;
    m_nTextHeight=16;
	m_nTextColor=0xff000000;
	m_nBackColor=0xffefffef;
	m_textColor[0] = 0;
	m_textColor[1] = 0;
	m_textColor[2] = 0;
	m_textColor[3] = 1;
	
	m_backColor[0] = (float)(0xef) / 255.0;
	m_backColor[1] = 1;
	m_backColor[2] = m_backColor[0];
	m_backColor[3] = 1;
	
	m_nFontStyle=XS_NORMAL<<8;
	SetFontSize(XWindow::m_nTextSize);
}

void* XGraphics::GetFont()
{

	if (m_nFontStyle & XF_UPDATE)
	{
		if (m_pFont != NULL)
			[(UIFont*)m_pFont release];
		m_pFont = 0;
		m_nFontStyle &=~XF_UPDATE;
	}
	if (m_pFont == NULL)
	{        
        int h = (m_nFontStyle>>8)&0xff;
        h = (h-1)*2+14;
        if (m_nFontStyle&XF_ITALIC)
        {
            m_pFont = [[UIFont italicSystemFontOfSize:h] retain];
        }
        else if (m_nFontStyle&XF_BOLD)
        {
            m_pFont = [[UIFont boldSystemFontOfSize:h] retain];
        }
        else
        {
            m_pFont = [[UIFont systemFontOfSize:h]retain];
        }
        m_nTextHeight = h+2;
	}
	return m_pFont;
}

void XGraphics::GetBitmapData(void*pBuf,int nLength,XU8 nBits)
{
	assert(m_pBitmap);
	assert(nBits == 32);
	assert(nLength == m_width * m_height * 4);
	memcpy(pBuf, m_pBitmap, nLength);
}

XBOOL XGraphics::DrawDIB(XDib &dib, const XPOINT &pt, const XRECT &rect)
{
	if(dib.IsEmpty()||dib.GetBits()!=XDib::BITS32)
        return XFALSE;
	int w=dib.Width();
    int h=dib.Height();
    
	int oldX = m_originX;
	int oldY = m_originY;
	
	SetOrigin(0, 0);
 
    XU32 * pDst = (XU32*)CGBitmapContextGetData((CGContextRef)m_pGraphics);
    XU32 * pSrt = (XU32 *)dib.GetBuffer();
    pSrt += rect.top * w + rect.left;
    int len = rect.right - rect.left;
    int deviceW = g_deviceData.m_screenWidth;
    if ( ! g_deviceData.mbOnSale)
    {
        deviceW = deviceW * g_deviceData.m_scale;
    }
    pDst += deviceW * pt.y + pt.x;
   
    for (int y = rect.top; y < rect.bottom; y++ )
    {
        //NSLog(@"test %d", y);
        memcpy( pDst, pSrt, len << 2);
        pDst += deviceW;
        //Modify by xiepeifu
        pSrt += w;
    }
    
    SetOrigin(oldX, oldY);
	return XTRUE;
	
}

XBOOL XGraphics::DrawDIB(XDib &dib, const XPOINT &pt, const XRECT &rect, XBOOL isButton)
{
	if(dib.IsEmpty()||dib.GetBits()!=XDib::BITS32)
        return XFALSE;
	
	int w=dib.Width();
    int h=dib.Height();
    
	int oldX = m_originX;
	int oldY = m_originY;
	
	SetOrigin(0, 0);
    if( !isButton )
    {
        //NSLog(@"w:%d, h:%d, left:%d, right:%d, top:%d, bottom:%d", w, h, rect.left, rect.right, rect.top, rect.bottom );
        XU32 * pDst = (XU32*)CGBitmapContextGetData((CGContextRef)m_pGraphics);
        XU32 * pSrt = (XU32 *)dib.GetBuffer();
        pSrt += rect.top * w + rect.left;
        int len = rect.right - rect.left;
        
        int deviceW = g_deviceData.m_screenWidth;
        if ( ! g_deviceData.mbOnSale)
        {
            deviceW = deviceW * g_deviceData.m_scale;
        }
        pDst += deviceW * pt.y + pt.x;
        for (int y = rect.top; y < rect.bottom; y++ )
        {
            //NSLog(@"test %d", y);
            memcpy( pDst, pSrt, len << 2 );
            pDst += deviceW;
            pSrt += len;
        }
    }
    else
    {
        int totalSize = w * h * 4;
        CGDataProviderRef ref = CGDataProviderCreateWithData(NULL, dib.GetBuffer(), totalSize, NULL);
        CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
        CGImageRef iref = CGImageCreate(w, h, 8, 32, w * 4, colorspace, kCGBitmapByteOrder32Little | kCGImageAlphaFirst,
                                        ref, NULL, true, kCGRenderingIntentDefault);
        //	CGImageRef iref = CGImageCreate(w, h, 8, 32, w * 4, colorspace, kCGBitmapByteOrder32Big | kCGImageAlphaFirst,
        //									ref, NULL, true, kCGRenderingIntentDefault);
        
        //UIImage* uiImage = [[UIImage alloc] initWithCGImage:iref];
        UIImage *uiImage = [[UIImage alloc] initWithCGImage:iref scale:2 orientation:UIImageOrientationUp ];
        
        CGContextRef context = (CGContextRef)m_pGraphics;
        
        CGContextSaveGState(context);
        //clipToRectInGlobal(XRect(pt.x * 2, pt.y * 2, (rect.right - rect.left + pt.x) * 2, (rect.bottom - rect.top + pt.y) * 2 ));
        
        CGContextClipToRect(context, CGRectMake(pt.x - m_originX, pt.y - m_originY, (rect.right - rect.left),(rect.bottom - rect.top)));
        
        [uiImage drawAtPoint:CGPointMake(pt.x - rect.left, pt.y - rect.top)];
        //[uiImage drawInRect:CGRectMake(pt.x - rect.left, pt.y -rect.top, (rect.right - rect.left), (rect.bottom - rect.top))];
        
        CGContextRestoreGState(context);
        [uiImage release];
        CGDataProviderRelease(ref);
        CGColorSpaceRelease(colorspace);
        CGImageRelease(iref);
    }
    SetOrigin(oldX, oldY);
	return XTRUE;
}


void XGraphics::GetBitmapMask(int sx, int sy, XDib &dib)
{
	if(dib.GetBits()!=8)
        return;
	
	int wb = dib.Width();
	int ws = m_width * 4;
	XU8* pDst = (XU8*)dib.GetBuffer()+sy*wb+sx;
	
	XU8* pSrt = (XU8*)m_pBitmap;
	
	int w = m_width;
	int h = m_height;
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			pDst[x] = pSrt[(x << 2) + 1] >> 2;
		}
		pSrt += ws;
		pDst += wb;
	}
}

XU32 XGraphics::SetBold(XBOOL b) 
{
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
	return n;
}
XU32 XGraphics::SetStrike(XBOOL b) 
{
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
	return n;
}
XU32 XGraphics::SetUnderline(XBOOL b)
{
	XU32 n=m_nFontStyle;
	if(b) 
	{
		if(!(m_nFontStyle&XF_UNDERLINE))
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle|=(XF_UNDERLINE);
	}
	else 
	{
		if(m_nFontStyle&XF_UNDERLINE)
			m_nFontStyle|=XF_UPDATE;
		m_nFontStyle&=~(XF_UNDERLINE);
	}
	
	return n;

}
XU32 XGraphics::SetFontStyle(XU32 ns)
{
	XU32 n=m_nFontStyle&0x7fffffff;
	if((n&0xff1f)!=(ns&0xff1f))
		m_nFontStyle|=XF_UPDATE;
	m_nFontStyle&=0x80000000;
	m_nFontStyle|=ns;
	return n;
}
XU32 XGraphics::SetFontSize(XU8 nSize)
{
	   XU32 n=m_nFontStyle;
	   XU32 ns=nSize<<8;
	   if((m_nFontStyle&0x0000ff00)!=ns)
	   {
		m_nFontStyle&=0xffff00ff;
		m_nFontStyle|=ns;
		m_nFontStyle|=XF_UPDATE;
	   }
	   return n;
}

