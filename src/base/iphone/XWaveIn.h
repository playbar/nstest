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
#import <AudioToolbox/AudioSession.h>
#import <AudioToolbox/AudioQueue.h>
#import <AudioToolbox/AudioFile.h>
#import <AudioToolbox/AudioConverter.h>
//#import "Recorder.h"
//#include "XRingStream.h"
#define WAVEBUFSIZE 4096
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
#define NUM_BUFFERS 3
class XMicrophone;

typedef struct
{
    AudioStreamBasicDescription dataFormat;
    SInt64                      currentPacket;
    XBOOL                       stopIndication;
    XMicrophone* pMicrophone; //callback
    AudioQueueBufferRef         buffers[NUM_BUFFERS];
    AudioQueueRef               queue;
} MicrophoneRecordState;


class XWaveIn  
{
public:
	bool Stop();
	bool Start();
	XWaveIn(XMicrophone*p);
	virtual ~XWaveIn();
	static int GetNames(XString8Array&list);
	static bool GetName(int id,XString8&name);
    MicrophoneRecordState * GetFormat(){return recordState;}
	bool SetChannel(int nc)
	{
        if (isRunning) return false;
        if ((nc!=1)&&(nc!=2)) return false;
        recordState->dataFormat.mChannelsPerFrame = nc;

		return true;
	}
	bool SetBits(int bits)
	{
        if (isRunning) return false;
        if ((bits!=8)&&(bits!=16)) return false;
        recordState->dataFormat.mBitsPerChannel = bits;
		return true;
	}
	bool SetSamples(int ns)
	{
        if (isRunning) return false;
        recordState->dataFormat.mSampleRate = ns;
		return true;
	}
	
protected:
	
    static MicrophoneRecordState * recordState;
    XBOOL   recording;
	bool isRunning;
    UInt32                      bufferByteSize;
};

#endif // 
