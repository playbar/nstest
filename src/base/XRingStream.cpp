// XRingStream.cpp: implementation of the XRingStream class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XRingStream.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XRingStream::XRingStream(XU32 nSize)
{
	m_nMemSize=0;
	m_nReadPos=0;
	m_pBuffer=XNULL;
	m_nWritePos=0;
	if(nSize)
		SetMemSize(nSize);
}

XRingStream::~XRingStream()
{
	if(m_pBuffer!=XNULL)
		delete m_pBuffer;
}

