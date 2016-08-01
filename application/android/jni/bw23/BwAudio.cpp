#include "jni.h"
#include <SLES/OpenSLES.h>
#include "SLES/OpenSLES_Android.h"
#include <sys/types.h>
#include <assert.h>
#include <stdlib.h>

typedef void (*AudioCallBackAT)(int event, void* user, void *info);
typedef void (*AudioCallBackSL)(SLAndroidSimpleBufferQueueItf bq, void *soundMix);

// engine interfaces
static SLObjectItf gEngineObject = NULL;
static SLEngineItf gEngineEngine;

// output mix interfaces
static SLObjectItf gOutputMixObject = NULL;
static SLEnvironmentalReverbItf gOutputMixEnvironmentalReverb = NULL;

// buffer queue player interfaces
static SLObjectItf		gBQObject = NULL;
static SLPlayItf		gBQPlay = NULL;
static SLVolumeItf		gVolume;
static SLAndroidSimpleBufferQueueItf gBQBufferQueue;
static SLEffectSendItf	gBQEffectSend;

// aux effect on the output mix, used by the buffer queue player
static const SLEnvironmentalReverbSettings gReverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

// zhengjl 2012.06
static char * tmpBuffer = NULL;
static int bufferSize = 0;


// create the engine and output mix objects
extern "C"
void SlesCreateEngine( )
{
    SLresult result;
    // create engine
    result = slCreateEngine(&gEngineObject, 0, NULL, 0, NULL, NULL);
    //assert(SL_RESULT_SUCCESS == result);

    // realize the engine
    result = (*gEngineObject)->Realize(gEngineObject, SL_BOOLEAN_FALSE);
    //assert(SL_RESULT_SUCCESS == result);

    // get the engine interface, which is needed in order to create other objects
    result = (*gEngineObject)->GetInterface(gEngineObject, SL_IID_ENGINE, &gEngineEngine);
    //assert(SL_RESULT_SUCCESS == result);

    // create output mix, with environmental reverb specified as a non-required interface
    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*gEngineEngine)->CreateOutputMix(gEngineEngine, &gOutputMixObject, 1, ids, req);
    //assert(SL_RESULT_SUCCESS == result);

    // realize the output mix
    result = (*gOutputMixObject)->Realize(gOutputMixObject, SL_BOOLEAN_FALSE);
    //assert(SL_RESULT_SUCCESS == result);

    // get the environmental reverb interface
    // this could fail if the environmental reverb effect is not available,
    // either because the feature is not present, excessive CPU load, or
    // the required MODIFY_AUDIO_SETTINGS permission was not requested and granted
    result = (*gOutputMixObject)->GetInterface(gOutputMixObject, SL_IID_ENVIRONMENTALREVERB,
            &gOutputMixEnvironmentalReverb);
    if (SL_RESULT_SUCCESS == result) {
        result = (*gOutputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                gOutputMixEnvironmentalReverb, &gReverbSettings);
    }
    // ignore unsuccessful result codes for environmental reverb, as it is optional for this example
}

// create buffer queue audio player
extern "C"
void SlesCreateBQPlayer(AudioCallBackSL funCallback, void *soundMix, int rate, int nChannel, int bitsPerSample )
{
    SLresult result;
    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm;
    format_pcm.formatType = SL_DATAFORMAT_PCM;
    format_pcm.numChannels = nChannel;
    format_pcm.samplesPerSec = rate * 1000; //SL_SAMPLINGRATE_22_05;
    format_pcm.bitsPerSample = bitsPerSample;
    format_pcm.containerSize = 16;
    if( nChannel == 2 )
    	format_pcm.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
    else
    	format_pcm.channelMask = SL_SPEAKER_FRONT_CENTER;
    format_pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;

    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, gOutputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // create audio player
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*gEngineEngine)->CreateAudioPlayer(gEngineEngine, &gBQObject, &audioSrc, &audioSnk,
            3, ids, req);
    //assert(SL_RESULT_SUCCESS == result);

    // realize the player
    result = (*gBQObject)->Realize(gBQObject, SL_BOOLEAN_FALSE);
    //assert(SL_RESULT_SUCCESS == result);

    // get the play interface
    result = (*gBQObject)->GetInterface(gBQObject, SL_IID_PLAY, &gBQPlay);
    //assert(SL_RESULT_SUCCESS == result);

    // get the buffer queue interface
    result = (*gBQObject)->GetInterface(gBQObject, SL_IID_BUFFERQUEUE,
            &gBQBufferQueue);
    //assert(SL_RESULT_SUCCESS == result);

    // register callback on the buffer queue
    result = (*gBQBufferQueue)->RegisterCallback(gBQBufferQueue, funCallback, soundMix );
    //assert(SL_RESULT_SUCCESS == result);

    // get the effect send interface
    result = (*gBQObject)->GetInterface(gBQObject, SL_IID_EFFECTSEND,
            &gBQEffectSend);
    //assert(SL_RESULT_SUCCESS == result);

    // set the player's state to playing
    result = (*gBQPlay)->SetPlayState(gBQPlay, SL_PLAYSTATE_PLAYING );
    //assert(SL_RESULT_SUCCESS == result);

//    result = (*gBQObject)->GetInterface(gBQObject, SL_IID_VOLUME,  &gVolume);
//    assert(SL_RESULT_SUCCESS == result);
//    result = (*gVolume)->SetMute( gVolume, SL_BOOLEAN_FALSE );
//    assert(SL_RESULT_SUCCESS == result);
}


// shut down the native audio system
extern "C" void SlesShutdown( )
{
    // destroy buffer queue audio player object, and invalidate all associated interfaces
    if (gBQObject != NULL)
    {
        (*gBQObject)->Destroy(gBQObject);
        gBQObject = NULL;
        gBQPlay = NULL;
        gBQBufferQueue = NULL;
        gBQEffectSend = NULL;
    }

    // destroy output mix object, and invalidate all associated interfaces
    if (gOutputMixObject != NULL)
    {
        (*gOutputMixObject)->Destroy(gOutputMixObject);
        gOutputMixObject = NULL;
        gOutputMixEnvironmentalReverb = NULL;
    }

    // destroy engine object, and invalidate all associated interfaces
    if (gEngineObject != NULL)
    {
        (*gEngineObject)->Destroy(gEngineObject);
        gEngineObject = NULL;
        gEngineEngine = NULL;
    }
}

extern "C" void BwAudioCreate(int rate)
{
	SlesCreateEngine( );
}

void BwAudioCreateAT(AudioCallBackAT funCallback, void *soundMix, int rate )
{
}

extern "C"
void BwAudioCreateSL(AudioCallBackSL funCallback, void *soundMix, int rate, int nChannel, int bitsPerSample)
{
	SlesCreateEngine( );
	SlesCreateBQPlayer(funCallback, soundMix, rate, nChannel, bitsPerSample);
	return;
}

extern "C" void BwAudioStart()
{
	(*gBQPlay)->SetPlayState(gBQPlay, SL_PLAYSTATE_PLAYING );
}

extern "C" void BwAudioStop()
{
	(*gBQPlay)->SetPlayState(gBQPlay, SL_PLAYSTATE_PAUSED );
}

extern "C" int BwAudioWrite( const void* buffer, int size)
{
	// zhengjl 2012.06
	if (size > bufferSize)
	{
		if (tmpBuffer != NULL)
		{
			free(tmpBuffer);
		}

		tmpBuffer = (char*)malloc(size);
		bufferSize = size;
	}
	memset(tmpBuffer,0,bufferSize);
	memcpy(tmpBuffer, buffer, size);
	(*gBQBufferQueue)->Enqueue(gBQBufferQueue, tmpBuffer, size );


//	(*gBQBufferQueue)->Enqueue(gBQBufferQueue, buffer, size );
	return 0;
}

extern "C" void BwAudioDestroy()
{
	SlesShutdown( );

	// zhengjl 2012.06
	if (tmpBuffer != NULL)
	{
		free(tmpBuffer);
		tmpBuffer = NULL;
		bufferSize = 0;
	}

}

extern "C" void BwAudioGetVolume( float* left, float* right)
{
	//SLmillibel level = 0;
	//(*gVolume)->GetVolumeLevel( gVolume, &level );
	//*left = *right = level;
}

extern "C" void BwAudioSetVolume( float left, float right)
{
	 //(*gVolume)->SetVolumeLevel( gVolume, right );
}
