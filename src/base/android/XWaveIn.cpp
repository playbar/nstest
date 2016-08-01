// XWaveIn.cpp: implementation of the XWaveIn class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XWaveIn.h"
#include "XMicrophone.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XWaveIn::XWaveIn(XMicrophone*p)
{
	//    WORD        wFormatTag;         /* format type */
    //WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
    //DWORD       nSamplesPerSec;     /* sample rate */
    //DWORD       nAvgBytesPerSec;    /* for buffer estimation */
    //WORD        nBlockAlign;        /* block size of data */
    //WORD        wBitsPerSample;     /* number of bits per sample of mono data */
    //WORD        cbSize;             /* the count in bytes of the size of */
				    /* extra information (after cbSize) */
	m_pMicrophone=p;
	isRunning = false;
	
	//m_nBufferSize=m_waveFmt.nAvgBytesPerSec/100;
}

XWaveIn::~XWaveIn()
{
	Stop();
}

//static void CALLBACK CallBack(
//  HWAVEIN hwi,       
//  UINT uMsg,         
//  DWORD dwInstance,  
//  DWORD dwParam1,    
//  DWORD dwParam2     
//)
//{
//	if(!dwParam1) return;
//	XWaveIn* pWave=(XWaveIn*)dwInstance;
//	switch(uMsg)
//	{
//	case WIM_OPEN:
//		 break;
//	case WIM_CLOSE:
//		 break;
//	case WIM_DATA:
//		{
//			WAVEHDR*pHeader=(WAVEHDR*)dwParam1;
//			//pHeader->dwFlags=WHDR_PREPARED;
//			//pHeader->dwBytesRecorded=0;
//			pWave->WaveIn(pHeader);
//		}
//		 break;
//	}
//}

bool XWaveIn::GetName(int id,XString8&name)
{

	return false;
	//	list.Add(str);
}

int XWaveIn::GetNames(XString8Array &list)
{
	return 0;
}

bool XWaveIn::Start()
{
	
	 return true;
}

bool XWaveIn::Stop()
{
	
	return true;
}


