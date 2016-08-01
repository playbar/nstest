#include "jni.h"
#include "media/AudioSystem.h"
#include "media/AudioTrack.h"
#include "media/AudioRecord.h"

using namespace android;

static AudioTrack * gsAudioTrack = NULL;  // AudioTrack 类型

typedef void (*AudioCallBackAT)(int event, void* user, void *info);
typedef void (*AudioCallBackSL)(void* bq, void *soundMix);

extern "C"
void BwAudioCreate(int rate)
{
	gsAudioTrack = new AudioTrack( 3,rate,
									1,
									12,
									0, 0, 0, 0, 0);
}

extern "C"
void BwAudioCreateAT(AudioCallBackAT funCallback, void *soundMix, int rate )
{
	gsAudioTrack = new AudioTrack( 3, rate,
									1,
									12,
									0, 0, funCallback, soundMix, 0);

}

extern "C"
void BwAudioCreateSL(AudioCallBackSL funCallback, void *soundMix, int rate, int nChannel, int bitsPerSample)
{
}

extern "C" void BwAudioStart()
{
	gsAudioTrack->start();
}

extern "C" void BwAudioStop()
{
	gsAudioTrack->stop();
}

extern "C" int BwAudioWrite( const void* buffer, int size)
{
	int len = gsAudioTrack->write(buffer,size);
	return len;
}

extern "C" void BwAudioDestroy()
{
	if( gsAudioTrack != NULL )
	{
		delete gsAudioTrack;
		gsAudioTrack = NULL;
	}
}

extern "C" void BwAudioGetVolume( float* left, float* right)
{
	gsAudioTrack->getVolume(left,right);
}

extern "C" void BwAudioSetVolume( float left, float right)
{
	gsAudioTrack->setVolume(left,right);
}
