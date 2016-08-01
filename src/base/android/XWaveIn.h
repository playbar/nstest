// XWaveIn.h: interface for the XWaveIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWAVEIN_H__BAAADC66_F3E1_46D7_A29D_CB93C791C8DB__INCLUDED_)
#define AFX_XWAVEIN_H__BAAADC66_F3E1_46D7_A29D_CB93C791C8DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMutex.h"
#include "XString.h"

//#include "XRingStream.h"
#define WAVEBUFSIZE 18*4*16
//128*4
enum
{
	XPCM_8K=8000,
	XPCM_11K=11025,
	XPCM_12K=12000,
	XPCM_16K=16000,
	XPCM_22K=22050,
	XPCM_24K=24000,
	XPCM_32K=32000,
	XPCM_44K=44100,
	XPCM_48K=48000
};
class XMicrophone;
class XWaveIn  
{
public:
	//bool GetData(void*pData,int&l);
	void WaveIn( );
	bool Stop();
	bool Start();
	XWaveIn(XMicrophone*p);
	virtual ~XWaveIn();
	static int GetNames(XString8Array&list);
	static bool GetName(int id,XString8&name);
	bool SetChannel(int nc)
	{
		return true;
	}
	bool SetBits(int bits)
	{
		return true;
	}
	bool SetSamples(int ns)
	{
		return true;
	}
	
protected:
	XMicrophone* m_pMicrophone;
	XU8 m_buffer1[WAVEBUFSIZE];
	XU8 m_buffer2[WAVEBUFSIZE];


	bool isRunning;
	
};

#endif // !defined(AFX_XWAVEIN_H__BAAADC66_F3E1_46D7_A29D_CB93C791C8DB__INCLUDED_)
