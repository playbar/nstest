// FontSample.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FontSample.h"
#include "VGFontCreate.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// VGFontSample




VGFontSample::VGFontSample()
{
	m_nEnPecent=100;
}

VGFontSample::~VGFontSample()
{
}


BEGIN_MESSAGE_MAP(VGFontSample, CButton)
	//{{AFX_MSG_MAP(VGFontSample)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// VGFontSample message handlers
//绘制示例效果
void VGFontSample::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
   if(m_ch.GetSafeHandle()==NULL||
	   m_en.GetSafeHandle()==NULL) return;
   //绘制背景
   CRect rect(lpDrawItemStruct->rcItem);
   CRgn c_rgn;
   c_rgn . CreateRectRgnIndirect( &rect );
   VGBitmapDC dc(rect.Width(),rect.Height());
   CDC *MemDC = &dc;
   MemDC->Rectangle(rect);
	  //CDC::FromHandle(lpDrawItemStruct->hDC);
   if( MemDC == NULL ) return;
   MemDC -> SelectClipRgn(&c_rgn);   
   //CFont *cf = m_pChinese, *ef = m_pEnglish, *oldFont;
   CSize cSize, eSize;   
   
   CFont *oldFont;
   /*LOGFONT logFont;
   cf -> GetLogFont( &logFont );
   logFont . lfItalic = m_italic;
   cFont.CreateFontIndirect( &logFont );
   ef -> GetLogFont( &logFont );
   logFont . lfItalic = m_italic;
   eFont.CreateFontIndirect( &logFont );*/
   //绘制示例中的中文部分
   oldFont = MemDC -> SelectObject( &m_ch );
   cSize = MemDC -> GetTextExtent( "汉字在这" );
   MemDC -> SelectObject( &m_en );
   eSize = MemDC -> GetTextExtent( "ABCabcwy" );
   CPoint m_begin;
   MemDC -> SetTextColor( 0 );
   MemDC -> SetBkMode( TRANSPARENT );
   m_begin . x = ( rect . left + rect . right ) / 2 -
				 ( cSize . cx + eSize . cx ) / 2;
   m_begin . y = rect.top+4;
				 //( rect . top + rect . bottom ) / 2 -
				 //( cSize . cy + eSize . cy ) / 4;
   MemDC -> SelectObject( &m_ch );
   MemDC -> TextOut( m_begin . x, m_begin . y, "汉字在这");//
					 //"汉字在这" );
   //绘制示例中的英文部分

   MemDC -> SelectObject( &m_en );
   MemDC -> TextOut( m_begin . x + cSize . cx, m_begin . y
					 + ( cSize . cy - eSize . cy ) / 2, 
					 "ABCabcwy" );   
   MemDC -> SelectObject( oldFont );
   VGFontCreate cr;
   cr.Init(m_strCh,m_strEn,VGFontCreate::SIZE_16,m_nEnPecent);

   unsigned short out[128];
   char* str16="16 点汉字";
   int bx=rect.left+16;
   int i,by=rect.top+(rect.Height())/2;
   ToWide(str16,out);
   for(i=0;i<128;i++)
   {
	   if(!out[i]) break;
	   int nCharWidth=0,nDataSize=0;
	   unsigned char*pData=cr.CreateChar(out[i],nCharWidth,nDataSize);
	   bx+=cr.DrawChar(MemDC,bx,by,0xff0000,0);
   }
   char* str16f="16 点汉字带边";      
   ToWide(str16f,out);
   for(i=0;i<128;i++)
   {
	   if(!out[i]) break;
	   int nCharWidth=0,nDataSize=0;
	   unsigned char*pData=cr.CreateChar(out[i],nCharWidth,nDataSize);
	   bx+=cr.DrawChar(MemDC,bx,by,0xb040,0xff000000);
   }

   char* str12="12点汉字";
   bx=rect.left+16;
   by+=18;
   ToWide(str12,out);
   cr.Init(m_strCh,m_strEn,VGFontCreate::SIZE_12,m_nEnPecent);
   for(i=0;i<128;i++)
   {
	   if(!out[i]) break;
	   int nCharWidth=0,nDataSize=0;
	   unsigned char*pData=cr.CreateChar(out[i],nCharWidth,nDataSize);
	   bx+=cr.DrawChar(MemDC,bx,by,0xff0000,0);
   }
   
   char* str12f="12点汉字带边";      
   ToWide(str12f,out);
   for(i=0;i<128;i++)
   {
	   if(!out[i]) break;
	   int nCharWidth=0,nDataSize=0;
	   unsigned char*pData=cr.CreateChar(out[i],nCharWidth,nDataSize);
	   bx+=cr.DrawChar(MemDC,bx,by,0xb040,0xff000000);
   }

   char* str8="8 点汉字";
   bx=rect.left+16;
   by+=14;
   ToWide(str8,out);
   cr.Init(m_strCh,m_strEn,VGFontCreate::SIZE_8,m_nEnPecent);
   for(i=0;i<128;i++)
   {
	   if(!out[i]) break;
	   int nCharWidth=0,nDataSize=0;
	   unsigned char*pData=cr.CreateChar(out[i],nCharWidth,nDataSize);
	   bx+=cr.DrawChar(MemDC,bx,by,0xff0000,0);
   }
   
   char* str8f="8点汉字带边";      
   ToWide(str8f,out);
   for(i=0;i<128;i++)
   {
	   if(!out[i]) break;
	   int nCharWidth=0,nDataSize=0;
	   unsigned char*pData=cr.CreateChar(out[i],nCharWidth,nDataSize);
	   bx+=cr.DrawChar(MemDC,bx,by,0xb040,0);
   }


   CDC*pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
   pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),MemDC,0,0,SRCCOPY);
}
//设置要使用的字体
//pFont表示要设置的字体
//bChinese表示是要设置中文或英文字体
void VGFontSample::SetFont(CFont *pFont, BOOL bChinese)
{
   LOGFONT logFont;
   pFont->GetLogFont(&logFont);   
   if(bChinese)
   {
	   if(m_ch.GetSafeHandle()!=NULL) m_ch.DeleteObject();
	   m_ch.CreateFontIndirect(&logFont);
   }
   else
   {
	   if(m_en.GetSafeHandle()!=NULL) m_en.DeleteObject();
	   m_en.CreateFontIndirect(&logFont);
   }   
}


void VGFontSample::SetEnPecent(int p)
{
	m_nEnPecent=p;
	SetFont(m_strCh,m_strEn);
	Invalidate();
}

//设置要使用的字体
void VGFontSample::SetFont(LOGFONT &logFont, BOOL bChinese)
{
   LOGFONT f1;
   memcpy(&f1,&logFont,sizeof(LOGFONT));
   f1.lfHeight=64;
   f1.lfWidth=32;
   f1.lfEscapement=0;
   if(bChinese)
   {
	   if(m_ch.GetSafeHandle()!=NULL) m_ch.DeleteObject();
	   m_ch.CreateFontIndirect(&f1);
   }
   else
   {
	   if(m_en.GetSafeHandle()!=NULL) m_en.DeleteObject();
	   m_en.CreateFontIndirect(&f1);
   }   
}

BOOL VGFontSample::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CButton::OnEraseBkgnd(pDC);
}

void VGFontSample::SetFont(LPCTSTR strFont,LPCSTR strFontEn)
{  
   CRect rect;
   GetClientRect(rect);

   m_strCh=strFont;
   m_strEn=strFontEn;

   int width=rect.Height()/2-12;
   m_ch.DeleteObject();
   m_ch.CreateFont(width,width>>1,
			0,//(m_nFontStyle&XF_ITALIC)?10:0,
			0,
			FW_NORMAL,
			0,
			0,
			0,
			GB2312_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH,
			strFont); 
   m_en.DeleteObject();
   int nwidth=width*m_nEnPecent/100;
   m_en.CreateFont(width,nwidth>>1,
			0,//(m_nFontStyle&XF_ITALIC)?10:0,
			0,
			FW_NORMAL,
			0,
			0,
			0,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH,
			strFontEn);
}

void VGFontSample::ToWide(LPCTSTR str,unsigned short*out)
{
	int iLen=strlen(str);
	iLen=MultiByteToWideChar(CP_ACP,NULL,str,
							iLen,out,iLen);
	out[iLen]=0;
}