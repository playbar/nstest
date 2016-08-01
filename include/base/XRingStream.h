// XRingStream.h: interface for the XRingStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XRINGSTREAM_H__B73A4716_676D_4F3A_B514_23ECD9E7798C__INCLUDED_)
#define AFX_XRINGSTREAM_H__B73A4716_676D_4F3A_B514_23ECD9E7798C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GGType.h"
#include "XMutex.h"
#include "string.h"
#define XRBLOCK 32*1024

class XRingStream
{
public:
	XRingStream(XU32 nSize=XRBLOCK);
	virtual ~XRingStream();
public:
	//BOOL Lock(int t=XSHORTTIME);
	//XBOOL SetMemSize(XU32 nSize);
	void Clear()
	{
		m_nReadPos=m_nWritePos=0;
	}
	inline XU32 GetDataSize()
	{
		if(m_nWritePos>=m_nReadPos)
			return m_nWritePos-m_nReadPos;
		else
		{
			return m_nWritePos+m_nMemSize-m_nReadPos;
		}
	}
	inline XU32 GetFreeSize()
	{
		return m_nMemSize-GetDataSize()-1;
	}
	inline XBOOL IsEmpty(){return m_nWritePos==m_nReadPos;}
	inline XBOOL IsFull()
	{
		if(m_nWritePos+1==m_nReadPos) return XTRUE;
		if(m_nWritePos+1-m_nMemSize==m_nReadPos) return XTRUE;
		return XFALSE;
	}
	inline XU32 SetMemSize(XU32 nSize)
	{
		if(nSize==m_nMemSize) return XTRUE;
		XU32 nDataSize=GetDataSize();
		if(nSize<nDataSize) return XFALSE;
		XU8* pBuffer=new XU8[nSize];
		if(pBuffer==XNULL) return XFALSE;
		if(nDataSize)
		{
			if(m_nWritePos>m_nReadPos)
				memcpy(pBuffer,m_pBuffer+m_nReadPos,nDataSize);
			else
			{
				memcpy(pBuffer,m_pBuffer+m_nReadPos,m_nMemSize-m_nReadPos);
				if(m_nWritePos)
					memcpy(pBuffer+m_nMemSize-m_nReadPos,m_pBuffer,m_nWritePos);
			}
			m_nReadPos=0;
			m_nWritePos=nDataSize;
		}		
		if(m_pBuffer)
			delete m_pBuffer;
		m_pBuffer=pBuffer;
		m_nMemSize=nSize;
		return XTRUE;
	}
	XBOOL WriteData(void*pData,XU32 nSize)
	{
		XU32 nFree=GetFreeSize();
		if(nFree<nSize)
		{
			if(!SetMemSize(m_nMemSize*2+nSize*2))
				return XFALSE;
		}
		if(pData)
		{
			if(m_nWritePos+nSize<=m_nMemSize)
			{
				memcpy(m_pBuffer+m_nWritePos,pData,nSize);
			}
			else
			{
				int nl=m_nMemSize-m_nWritePos;
				memcpy(m_pBuffer+m_nWritePos,(unsigned char*)pData,nl);
				memcpy(m_pBuffer,(unsigned char*)pData+nl,nSize-nl);
			}
		}
		m_nWritePos+=nSize;
		if(m_nWritePos>=m_nMemSize)
			m_nWritePos-=m_nMemSize;
		return XTRUE;
	}
	XBOOL ReadData(void*pData,XU32 nSize)
	{
		if(GetDataSize()<nSize) return XFALSE;
		if(pData)
		{
			if(m_nReadPos+nSize<=m_nMemSize)
			{
				memcpy(pData,m_pBuffer+m_nReadPos,nSize);
			}
			else
			{
				int nl=m_nMemSize-m_nReadPos;
				memcpy(pData,m_pBuffer+m_nReadPos,nl);
				memcpy((unsigned char*)pData+nl,m_pBuffer,nSize-nl);
			}
		}
		m_nReadPos+=nSize;
		if(m_nReadPos>=m_nMemSize)
			m_nReadPos-=m_nMemSize;
		if(m_nReadPos==m_nWritePos)
		{
			m_nReadPos=m_nWritePos=0;
		}
		return XTRUE;
	}
	XBOOL SkipBytes(XU32 nSize)
	{
		if(GetDataSize()<nSize) return XFALSE;
		m_nReadPos+=nSize;
		if(m_nReadPos>=m_nMemSize)
			m_nReadPos-=m_nMemSize;
		if(m_nReadPos==m_nWritePos)
		{
			m_nReadPos=m_nWritePos=0;
		}
	}
	XU8* ReadAddr(){return m_pBuffer+m_nReadPos;}
	XU8* WriteAddr(){return m_pBuffer+m_nWritePos;}
public:
	XMutex m_lock;
protected:
	//XMutex m_lock;
	XU8* m_pBuffer;
	XU32 m_nReadPos,m_nWritePos;
	XU32 m_nMemSize;
};

#endif // !defined(AFX_XRINGSTREAM_H__B73A4716_676D_4F3A_B514_23ECD9E7798C__INCLUDED_)
