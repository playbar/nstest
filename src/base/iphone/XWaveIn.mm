// XWaveIn.cpp: implementation of the XWaveIn class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XWaveIn.h"
#include "XMicrophone.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// Derive the Buffer Size. I punt with the max buffer size.
void DeriveBufferSize (AudioQueueRef audioQueue, AudioStreamBasicDescription ASBDescription, Float64 seconds, UInt32 *outBufferSize)
{
    
    static const int maxBufferSize = 0x50000; // punting with 50k
    int maxPacketSize = ASBDescription.mBytesPerPacket;
    if (maxPacketSize == 0)
	{
        UInt32 maxVBRPacketSize = sizeof(maxPacketSize);
        AudioQueueGetProperty(audioQueue, kAudioConverterPropertyMaximumOutputPacketSize, &maxPacketSize, &maxVBRPacketSize);
    }
    
    Float64 numBytesForTime = ASBDescription.mSampleRate * maxPacketSize * seconds;
    *outBufferSize =  (UInt32)((numBytesForTime < maxBufferSize) ? numBytesForTime : maxBufferSize);
}
MicrophoneRecordState * XWaveIn::recordState = XNULL;
XWaveIn::XWaveIn(XMicrophone*p)
{
    if (!recordState)
    {
        recordState = new MicrophoneRecordState;
    }
	recordState->pMicrophone=p;
	isRunning = false;
    recordState->stopIndication = true;
    recordState->dataFormat.mSampleRate = 22050.0f;
    recordState->dataFormat.mFormatID = kAudioFormatLinearPCM;
//    //recordState.dataFormat.mFormatFlags = kLinearPCMFormatFlagIsBigEndian |  kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
    recordState->dataFormat.mFormatFlags = 12;
    recordState->dataFormat.mChannelsPerFrame = 1; // mono
    recordState->dataFormat.mBitsPerChannel = 8;
    recordState->dataFormat.mFramesPerPacket = 1;
    recordState->dataFormat.mBytesPerPacket = 1;
    recordState->dataFormat.mBytesPerFrame = 1;
    recordState->dataFormat.mReserved = 0;
//    recordState->dataFormat.mSampleRate = 22050;
//	recordState->dataFormat.mFormatID = kAudioFormatLinearPCM;
//	recordState->dataFormat.mFormatFlags = 2;
//	recordState->dataFormat.mBytesPerPacket = 2;
//	recordState->dataFormat.mFramesPerPacket = 1;
//	recordState->dataFormat.mBytesPerFrame = 2;
//	recordState->dataFormat.mChannelsPerFrame = 1;
//	recordState->dataFormat.mBitsPerChannel = 16;
//	recordState->dataFormat.mReserved = 0;
}

XWaveIn::~XWaveIn()
{
	Stop();
}


bool XWaveIn::GetName(int id,XString8&name)
{
    char deviceName[15] = "iosMicroPhone";
    name = deviceName;
	return true;
}

int XWaveIn::GetNames(XString8Array &list)
{
    char deviceName[15] = "iosMicroPhone";
    XString8 str(deviceName);
    list.Add(str);
	return 1;
}

// Handle new input
static void HandleInputBuffer (void *aqData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer, const AudioTimeStamp *inStartTime,
							   UInt32 inNumPackets, const AudioStreamPacketDescription *inPacketDesc)
{
    printf("input callback\n");
    MicrophoneRecordState *pAqData = (MicrophoneRecordState *) aqData;
    
    if(((MicrophoneRecordState *)aqData)->pMicrophone)
        ((MicrophoneRecordState *)aqData)->pMicrophone->DataIn(inBuffer, aqData);
    
    if (inNumPackets == 0 && pAqData->dataFormat.mBytesPerPacket != 0)
        inNumPackets = inBuffer->mAudioDataByteSize / pAqData->dataFormat.mBytesPerPacket;
    

    pAqData->currentPacket += inNumPackets;
    AudioQueueEnqueueBuffer(inAQ, inBuffer, 0, NULL);

}

bool XWaveIn::Start()
{
    // new input queue
    if (isRunning) return false;
    isRunning = true;
    
    if ( !recordState ) return false;
    recordState->stopIndication = false;
    OSStatus status;
    status = AudioQueueNewInput(&recordState->dataFormat, HandleInputBuffer, recordState, 0,kCFRunLoopCommonModes, 0, &recordState->queue);
	   
	// figure out the buffer size
    DeriveBufferSize(recordState->queue, recordState->dataFormat, 0.05, &bufferByteSize);
	
	// allocate those buffers and enqueue them
    for(int i = 0; i < NUM_BUFFERS; i++)
    {
        status = AudioQueueAllocateBuffer(recordState->queue, bufferByteSize, &recordState->buffers[i]);
        if (status)
        {BWLOG(@"Error allocating buffer %d\n", i); return NO;}
        
        status = AudioQueueEnqueueBuffer(recordState->queue, recordState->buffers[i], 0, NULL);
        if (status) {BWLOG(@"Error enqueuing buffer %d\n", i); return NO;}
    }
	
	// enable metering
    UInt32 enableMetering = YES;
    status = AudioQueueSetProperty(recordState->queue, kAudioQueueProperty_EnableLevelMetering, &enableMetering,sizeof(enableMetering));
    if (status) {BWLOG(@"Could not enable metering\n"); return NO;}
    
	// start recording
    status = AudioQueueStart(recordState->queue, NULL);
    if (status) {BWLOG(@"Could not start Audio Queue\n"); return NO;}
    recordState->currentPacket = 0;
    //recordState.recording = YES;
    

	
	 return true;
}
 //There's generally about a one-second delay before the buffers fully empty

bool XWaveIn::Stop()
{
    if (!isRunning) return false;
    isRunning = false;
    AudioQueueFlush(recordState->queue);
    AudioQueueStop(recordState->queue, NO);
    sleep(1);
    recordState->pMicrophone = XNULL;
    for(int i = 0; i < NUM_BUFFERS; i++)
    {
        AudioQueueFreeBuffer(recordState->queue, recordState->buffers[i]);
        
    }
    AudioQueueDispose(recordState->queue, YES);
    delete recordState;
    recordState = XNULL;
	return true;
}




