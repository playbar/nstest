// XBitmap.cpp: implementation of the XBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XBitmap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XBitmap::XBitmap()
{

}

XBitmap::~XBitmap()
{

}

XBitmap* XBitmap::Clone()
{
	XBitmap*pBmp=new XBitmap;
	if(pBmp)
	{
		pBmp->m_dib.FromDib(m_dib);
	}
	return pBmp;
}
