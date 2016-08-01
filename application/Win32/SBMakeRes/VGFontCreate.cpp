// VGFontCreate.cpp: implementation of the VGFontCreate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sbmakeres.h"
#include "VGFontCreate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef unsigned long XU32;
typedef unsigned short XU16;
typedef unsigned char XU8;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
VGBitmapDC::~VGBitmapDC()
{
   if(m_pOldBmp!=NULL)
	   SelectObject(m_pOldBmp);
}

VGBitmapDC::VGBitmapDC(CBitmap *bmp)
{
	CWindowDC dc( NULL );
	CDC* pDC = &dc;
	CreateCompatibleDC( pDC );
	m_pOldBmp = SelectObject( bmp );
}

VGBitmapDC::VGBitmapDC(int W, int H)
{
	m_pOldBmp=NULL;
   CreateBitmapDC(W,H);
}

BOOL VGBitmapDC::CreateBitmapDC(int W, int H)
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

BOOL VGBitmapDC::GetDib(HGLOBAL pDib, BITMAPINFOHEADER *pHead)
{
	LPBITMAPINFO pInfo = (LPBITMAPINFO)pHead;
	if( pDib == NULL ) 
	   { return FALSE; }
	LPSTR lpBits= (LPSTR)pInfo + sizeof( BITMAPINFOHEADER );
	return ::GetDIBits( m_hDC, (HBITMAP)m_bitmap, 0, pInfo -> bmiHeader . biHeight, 
				 lpBits, pInfo, DIB_RGB_COLORS )!=0;
}


VGFontCreate::VGFontCreate()
{
	m_nSize=16;
	m_nCharWidth=16;
	m_pDC=NULL;
}

VGFontCreate::~VGFontCreate()
{
	if(m_pDC)
		delete m_pDC;
}

unsigned char* VGFontCreate::CreateChar(unsigned short nChar,int&nCharWidth,int&nDataSize)
{
	unsigned short txt[]={nChar,0};
	CBrush brush;//(BS_SOLID,0x0);//BS_SOLID,0xff000000);	
	brush.CreateSolidBrush(0x0);
	m_pDC->FillRect(CRect(0,0,m_nSize,m_nSize),&brush);	
	m_pDC->SetTextColor(0xffffff);
	m_pDC->SetBkMode( TRANSPARENT );
	void*pOld=NULL;
	//if(nChar<0xff)
	//	pOld=m_pDC->SelectObject(m_en);
	//else
		pOld=m_pDC->SelectObject(m_ch);
	//m_pDC->TextOut(0,0,"H");
	::TextOutW(m_pDC->m_hDC,0,0,txt,1);
	CBitmap*pBmp=m_pDC->GetBitmap();
	unsigned long data[64][24];
	int x,y;
	nCharWidth= 0;//::GetTextExtent();
		//pDC -> GetTextExtent( "ABCabcwy" );;
	SIZE size;
	::GetTextExtentPoint32W(m_pDC->m_hDC,txt,1, &size);
	nCharWidth=size.cx;

	/*switch(nChar)
	{
	case ' ':
		 nCharWidth=8+m_nSize*4;
		 break;
	} */
	nDataSize=m_nDataSize;
	memset(m_data,0,nDataSize);
	switch(m_nType)
	{
	case SIZE_16:
		 pBmp->GetBitmapBits(16*16*4,data);
		 {
			XU16*pDst=(XU16*)m_data;
			XU32*pSrt=(XU32*)data;
			for(y=0;y<16;y++)
			{
				for(x=0;x<16;x++)
				{
					int iOff=x;
					if(pSrt[x]&0xff)
					{
						*pDst|=(1<<iOff);
						if(nCharWidth<x)
							nCharWidth=x;
					}
				}
				pDst++;
				pSrt+=16;
			}
		 }
		 //nCharWidth++;
		 //if(nCharWidth)
		 //	 nCharWidth+=2;
		 //if(nCharWidth>12&&nChar>0xff) 
		 //	 nCharWidth=16;
		 break;
	case SIZE_12:
		 pBmp->GetBitmapBits(12*12*4,data);
		 {
			XU16*pDst=(XU16*)m_data;
			XU32*pSrt=(XU32*)data;
			for(y=0;y<12;y++)
			{
				for(x=0;x<12;x++)
				{
					int iOff=x;
					if(pSrt[x]&0xff)
					{
						*pDst|=(1<<iOff);
						if(nCharWidth<x)
							nCharWidth=x;
					}
				}
				pDst++;
				pSrt+=12;
			}
		 }
		 //nCharWidth++;
		 //if(nCharWidth)
		 //	 nCharWidth+=2;
		 //if(nCharWidth>12&&nChar>0xff) 
		//	 nCharWidth=12;
		 break;
	case SIZE_8:
		 pBmp->GetBitmapBits(24*24*4,data);
		 {
			 XU32*pDst=(XU32*)m_data;
			 XU32*pSrt=((XU32*)data)+24;
			 for(y=0;y<8;y++)
			 {
				 for(x=1;x<24;x+=3)
				 {
					 XU32 iValue=(pSrt[x]&0xff)*4;

					 /*int iAdd=(pSrt[x-1]&0xff)*2;
					 int iAdd1=(pSrt[x+1]&0xff)*2;
					 if(iAdd<iAdd1) 
						 iAdd=iAdd1;
					 iAdd1=(pSrt[x-24]&0xff)*2;
					 if(iAdd<iAdd1) 
						 iAdd=iAdd1;
					 iAdd1=(pSrt[x+24]&0xff)*2;
					 if(iAdd<iAdd1) 
						 iAdd=iAdd1;
					 iValue+=iAdd*2;*/
					 iValue+=(pSrt[x-1]&0xff)*2;
					 iValue+=(pSrt[x+1]&0xff)*2;
					 iValue+=(pSrt[x-24]&0xff)*2;
					 iValue+=(pSrt[x+24]&0xff)*2;

					 iValue+=(pSrt[x-1-24]&0xff);
					 iValue+=(pSrt[x+1-24]&0xff);
					 iValue+=(pSrt[x-1+24]&0xff);
					 iValue+=(pSrt[x+1+24]&0xff);
					
					 //iValue>>=4;
					 //if(iValue)
					 //	 int v=0;
					 iValue=((iValue/8)>>4);
					 if(iValue>0xf) iValue=0xf;
					 if(iValue)
					 {
						int ix=x/3;
						*pDst|=iValue<<(ix*4);
						if(nCharWidth<ix)
							nCharWidth=ix;
					 }					 
				 }
				 pDst++;
				 pSrt+=24*3;
			 }

		 }
		 nCharWidth/=3;
		// if(nCharWidth)
		//	 nCharWidth+=2;
		 //if(nCharWidth>6&&nChar>0xff) 
		//	 nCharWidth=8;
		 break;
	}
	m_nCharWidth=nCharWidth;
	m_pDC->SelectObject(pOld);
	return m_data;
}

int VGFontCreate::DrawChar(CDC*pDC,int ox,int oy,unsigned long nColor,unsigned long nFrame)
{
	XU16*pData=(XU16*)m_data;
	//void*pOld=pDC->SelectObject(m_en);
	//pDC->SetTextColor(0xff000000);
	//pDC->TextOut(ox,oy,"Hello");
	//pDC->SelectObject(pOld);
	switch(m_nType)
	{
	case SIZE_16:
		{
			int x,y;
			if(nFrame)
			{
				XU16*ppData=pData;
				for(y=0;y<16;y++)
				{
					for(x=0;x<m_nCharWidth;x++)
					{
						int nData=(*ppData)>>(x);
						if(nData&1)
						{
							pDC->SetPixel(ox+x-1,oy+y,nFrame);
							pDC->SetPixel(ox+x+1,oy+y,nFrame);
							pDC->SetPixel(ox+x,oy+y-1,nFrame);
							pDC->SetPixel(ox+x,oy+y+1,nFrame);
						}
							//pDC->SetPixel(ox+x,oy+y,nColor);
					}
					ppData++;
				}
			}
			for(y=0;y<16;y++)
			{
				for(x=0;x<m_nCharWidth;x++)
				{
					int nData=(*pData)>>(x);
					if(nData&1)
						pDC->SetPixel(ox+x,oy+y,nColor);
				}
				pData++;
			}
		}
		break;
	case SIZE_12:
		{
			int x,y;
			if(nFrame)
			{
				XU16*ppData=pData;
				for(y=0;y<12;y++)
				{
					for(x=0;x<m_nCharWidth;x++)
					{
						int nData=(*ppData)>>(x);
						if(nData&1)
						{
							pDC->SetPixel(ox+x-1,oy+y,nFrame);
							pDC->SetPixel(ox+x+1,oy+y,nFrame);
							pDC->SetPixel(ox+x,oy+y-1,nFrame);
							pDC->SetPixel(ox+x,oy+y+1,nFrame);
						}
							//pDC->SetPixel(ox+x,oy+y,nColor);
					}
					ppData++;
				}
			}
			for(y=0;y<12;y++)
			{
				for(x=0;x<m_nCharWidth;x++)
				{
					int nData=(*pData)>>(x);
					if(nData&1)
						pDC->SetPixel(ox+x,oy+y,nColor);
				}
				pData++;
			}
		}
		break;
	case SIZE_8:
		{
			XU32*pDraw=(XU32*)pData;
			int x,y;
			if(nFrame)
			{
				for(y=0;y<8;y++)
				{
					for(x=0;x<8;x++)
					{
						int d=((*pDraw)>>(x<<2))&0xf;
						if(!d) continue;
						if(d)
						{
							XU32 c=nFrame;
							XU8* ac=(XU8*)&c;
							XU8  nBack=(0xff*(16-d))>>4;
							ac[3]=0;
							ac[0]=(nBack+((ac[0]*d)>>4))&0xff;
							ac[1]=(nBack+((ac[1]*d)>>4))&0xff;
							ac[2]=(nBack+((ac[2]*d)>>4))&0xff;							
							pDC->SetPixel(ox+x-1,oy+y,c);
							pDC->SetPixel(ox+x+1,oy+y,c);
							pDC->SetPixel(ox+x,oy+y-1,c);
							pDC->SetPixel(ox+x,oy+y+1,c);
						}					
					}
					pDraw++;
				}
			}
			pDraw=(XU32*)pData;
			for(y=0;y<8;y++)
			{
				for(x=0;x<8;x++)
				{
					int d=((*pDraw)>>(x<<2))&0xf;
					if(!d) continue;
					if(d==0xf)
					{
						pDC->SetPixel(ox+x,oy+y,nColor);
					}
					else
					{
						XU32 c=nColor;
						XU8* ac=(XU8*)&c;
						//XU8  nBack=(0xff*(16-d))>>4;
						XU32 nBack=pDC->GetPixel(ox+x,oy+y);
						XU8*bc=(XU8*)&nBack;
						bc[0]=(bc[0]*(16-d))>>4;
						bc[1]=(bc[1]*(16-d))>>4;
						bc[2]=(bc[2]*(16-d))>>4;
						
						ac[0]=(bc[0]+((ac[0]*d)>>4))&0xff;
						ac[1]=(bc[1]+((ac[1]*d)>>4))&0xff;
						ac[2]=(bc[2]+((ac[2]*d)>>4))&0xff;
						pDC->SetPixel(ox+x,oy+y,c);
					}
				}
				pDraw++;
			}
		}
		break;
	}
	return m_nCharWidth;
}

void VGFontCreate::Init(LPCTSTR strCh, LPCTSTR strEn, int nSize,int nPecent)
{
	if(m_pDC)
		delete m_pDC;
	int nWidth=16;
	m_nType=nSize;
	switch(nSize)
	{
	case SIZE_16:nWidth=16;
				 m_nDataSize=32;
				 break;
	case SIZE_12:nWidth=12;
				 m_nDataSize=24;
				 break;
	case SIZE_8: nWidth=24;
				 m_nDataSize=32;
				 break;
	}
   m_pDC=new VGBitmapDC(nWidth,nWidth);
   m_nSize=nWidth;
   m_ch.DeleteObject();
   m_ch.CreateFont(nWidth,(nWidth)>>1,
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
			strCh); 
   m_en.DeleteObject();
   //nWidth=nWidth*nPecent/100;
   m_en.CreateFont(nWidth,nWidth>>1,
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
			strEn);
   
}
