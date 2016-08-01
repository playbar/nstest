// BitmapDC.cpp: implementation of the CBitmapDC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BitmapDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CBitmapDC::~CBitmapDC()
{
   if(m_pOldBmp!=NULL)
	   SelectObject(m_pOldBmp);
}

CBitmapDC::CBitmapDC(CBitmap *bmp)
{
	CWindowDC dc( NULL );
	CDC* pDC = &dc;
	CreateCompatibleDC( pDC );
	m_pOldBmp = SelectObject( bmp );
}

CBitmapDC::CBitmapDC(int W, int H)
{
   CreateBitmapDC(W,H);
}

BOOL CBitmapDC::CreateBitmapDC(int W, int H)
{  
   CWindowDC dc(NULL);	
   CDC *pDC = &dc;
   if( GetSafeHdc()==NULL )
 	  CreateCompatibleDC( pDC );
   else
	  SelectObject( m_pOldBmp );
   if( m_bitmap.GetSafeHandle()!= NULL )
	   m_bitmap.DeleteObject();
   if( !m_bitmap . CreateCompatibleBitmap( pDC, W, H ) )
	   return FALSE;
   m_pOldBmp = SelectObject( &m_bitmap );   
   return TRUE;
}

BOOL CBitmapDC::GetDib(HGLOBAL pDib, BITMAPINFOHEADER *pHead)
{
	LPBITMAPINFO pInfo = (LPBITMAPINFO)pHead;
	if( pDib == NULL ) 
	   { return FALSE; }
	LPSTR lpBits= (LPSTR)pInfo + sizeof( BITMAPINFOHEADER );
	return ::GetDIBits( m_hDC, (HBITMAP)m_bitmap, 0, pInfo -> bmiHeader . biHeight, 
				 lpBits, pInfo, DIB_RGB_COLORS )!=0;
}
