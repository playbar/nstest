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
#include <mmsystem.h>
//#include "XRingStream.h"
#define WAVEBUFSIZE 18*4*16
#define DCTBLOCK 18
typedef signed int dct_fixed_t;
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
	void WaveIn(WAVEHDR*pHeader);
	bool Stop();
	bool Start();
	XWaveIn(XMicrophone*p);
	virtual ~XWaveIn();
	static int GetNames(XString8Array&list);
	static bool GetName(int id,XString8&name);
	bool SetChannel(int nc)
	{
		if(isRunning) return false;
		if(nc!=1&&nc!=2) return false;
		m_waveFmt.nChannels=nc;
		m_waveFmt.nBlockAlign=m_waveFmt.nChannels*m_waveFmt.wBitsPerSample/8;
		m_waveFmt.nAvgBytesPerSec=nc*m_waveFmt.nSamplesPerSec*m_waveFmt.wBitsPerSample/8;
		return true;
	}
	bool SetBits(int bits)
	{
		if(isRunning) return false;
		if(bits!=8&&bits!=16) return false;
		m_waveFmt.wBitsPerSample=bits;
		m_waveFmt.nBlockAlign=m_waveFmt.nChannels*m_waveFmt.wBitsPerSample/8;
		m_waveFmt.nAvgBytesPerSec=m_waveFmt.nChannels*m_waveFmt.nSamplesPerSec*m_waveFmt.wBitsPerSample/8;
		return true;
	}
	bool SetSamples(int ns)
	{
		if(isRunning) return false;
		m_waveFmt.nSamplesPerSec=ns;
		m_waveFmt.nAvgBytesPerSec=m_waveFmt.nChannels*m_waveFmt.nSamplesPerSec*m_waveFmt.wBitsPerSample/8;
		return true;
	}
	WAVEFORMATEX* GetFormat(){return &m_waveFmt;}
	XS32* GetDCTData(int&l){l=DCTBLOCK;return (XS32*)m_dctResult;}
//	int GetBufferSize(){return m_nBufferSize;}
protected:
	dct_fixed_t m_dct[64][DCTBLOCK];
	dct_fixed_t m_dctResult[DCTBLOCK];
	void FDCT(void*,int);
	int m_nBlocks;
	XMicrophone* m_pMicrophone;
	XU8 m_buffer1[WAVEBUFSIZE];
	XU8 m_buffer2[WAVEBUFSIZE];
//	XRingStream m_data;
//	XMutex m_dataLock;
	//int m_nBufferSize;
	void PrePareHeader(WAVEHDR&head,XU8*pBuf);
	WAVEHDR m_header1;
	WAVEHDR m_header2;
	HWAVEIN m_handle;
	bool isRunning;
	WAVEFORMATEX m_waveFmt;
};

#endif // !defined(AFX_XWAVEIN_H__BAAADC66_F3E1_46D7_A29D_CB93C791C8DB__INCLUDED_)
