#include "stdafx.h"
#include "DibShow.h"

CDibShow::CDibShow()
{
	m_nOffX=0;
	m_nOffY=0;
	m_isMoving=false;
	m_pDib = NULL;
	m_bScale = true;
}

CDibShow::~CDibShow()
{
}


BEGIN_MESSAGE_MAP(CDibShow, CButton)
	//{{AFX_MSG_MAP(CDibShow)
	ON_WM_DRAWITEM()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDibShow message handlers

void CDibShow::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
	
}

void CDibShow::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	if(m_pDib==NULL) return;
	XRect rect(0,0,m_pDib->Width(),m_pDib->Height());
	CRect rd;
	this->GetClientRect(rd);
	if(!m_bScale||(rd.Width()>=m_pDib->Width()&&rd.Height()>=m_pDib->Height()))
		m_pDib->Paint(lpDrawItemStruct->hDC,m_nOffX,m_nOffY,rect);
	else
	{
		BWBITMAPINFOHEADER m_header;
		BWBITMAPINFOHEADER* pHeader=&m_header;
		int dw=rd.Width();
		int dh=rd.Height();
		int rw=m_pDib->Width();
		int rh=m_pDib->Height();
		double fx=(double)dw/rw;
		double fy=(double)dh/rh;
		double ff=fx<=fy?fx:fy;
		int nOffX = (dw-rw*ff)/2;
		int nOffY = (dh-rh*ff)/2;
		//m_pColor  =  m_pMemory + headSize + nSize;			
		pHeader -> biSize = sizeof( BWBITMAPINFOHEADER );
		pHeader -> biWidth = m_pDib->Width();
		pHeader -> biHeight = -(LONG)m_pDib->Height();
		pHeader -> biPlanes = 1;
		pHeader -> biBitCount = m_pDib->m_nBits;
		pHeader -> biCompression = BI_RGB;
		pHeader -> biSizeImage = m_pDib->m_nLinePitch*m_pDib->m_nHeight;//m_nWidth*m_nHeight<<2;
		pHeader -> biClrImportant = 0;
		pHeader -> biClrUsed = 0;
		pHeader -> biXPelsPerMeter=3780;
		pHeader -> biYPelsPerMeter=3780;

		StretchDIBits(
			lpDrawItemStruct->hDC,//HDC hdc,                      // handle to DC
			nOffX,//int XDest,                    // x-coord of destination upper-left corner
			nOffY,//int YDest,                    // y-coord of destination upper-left corner
			(int)rw*ff,//int nDestWidth,               // width of destination rectangle
			(int)rh*ff,//int nDestHeight,              // height of destination rectangle
			0,//int XSrc,                     // x-coord of source upper-left corner
			0,//int YSrc,                     // y-coord of source upper-left corner
			rw,//int nSrcWidth,                // width of source rectangle
			rh,//int nSrcHeight,               // height of source rectangle
			m_pDib->GetBuffer(),//CONST VOID *lpBits,           // bitmap bits
			(LPBITMAPINFO)&m_header,     // lpBitsInfo
			DIB_RGB_COLORS,//(CONST BITMAPINFO *lpBitsInfo, // bitmap data
			SRCCOPY
			//UINT iUsage,                  // usage options
				//DWORD dwRop                   // raster operation code
			);

		/*::SetDIBitsToDevice((HDC)hHandle,                    // hDC
							m_nOffX,//rect.left,             // DestX
							m_nOffY,//rect.top,              // DestY
							rect.Width(  ),
							rect.Height(  ),
							rect.left,
							-m_header.biHeight-rect.bottom,
							0,//rect.top,                          // nStartScan
							-m_header.biHeight,
							GetBuffer(),
							(LPBITMAPINFO)&m_header,     // lpBitsInfo
							DIB_RGB_COLORS);*/
	}

}

void CDibShow::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_nDownX=point.x;
	m_nDownY=point.y;
	m_nDownMX=m_nOffX;
	m_nDownMY=m_nOffY;
	m_isMoving=true;
	this->SetCapture();
	CButton::OnLButtonDown(nFlags, point);
}

void CDibShow::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_isMoving)
	{
		m_nOffX=m_nDownMX+(point.x-m_nDownX);
		m_nOffY=m_nDownMY+(point.y-m_nDownY);
		Invalidate();
	}
	CButton::OnMouseMove(nFlags, point);
}

void CDibShow::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	if(point.x==m_nDownX&&
	   point.y==m_nDownY)
	{
		m_bScale=!m_bScale;
		Invalidate();
	}
	m_isMoving=false;
	CButton::OnLButtonUp(nFlags, point);
}

BOOL CDibShow::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//return TRUE;
	CBrush brush(0x7f7f7f);
	CRect rect;
	this->GetClientRect(rect);
	if(m_pDib==NULL)
	{
		pDC->FillRect(CRect(0,0,rect.Width(),rect.Height()),&brush);
		return TRUE;
	}
	int dibw=m_pDib->Width();
	int dibh=m_pDib->Height();
	if(dibw>rect.Width()||dibh>rect.Height())
	{
		pDC->FillRect(CRect(0,0,rect.Width(),rect.Height()),&brush);
	}
	else
	{
		if(m_nOffX>0)
		{
			pDC->FillRect(CRect(0,0,m_nOffX,rect.bottom),&brush);
		}
		if(m_nOffY>0)
		{
			pDC->FillRect(CRect(0,0,rect.right,m_nOffY),&brush);
		}
		if(m_nOffX+m_pDib->Width()<rect.right)
		{
			pDC->FillRect(CRect(m_nOffX+m_pDib->Width(),0,rect.right,rect.bottom),&brush);
		}
		if(m_nOffY+m_pDib->Height()<rect.bottom)
		{
			pDC->FillRect(CRect(0,m_nOffY+m_pDib->Height(),rect.right,rect.bottom),&brush);
		}
	}
	return TRUE;
	//return CButton::OnEraseBkgnd(pDC);
}

