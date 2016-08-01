// XXSoundMix.h: interface for the XXSoundMix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXSoundMix_H__571FC837_2DF0_40FA_8F2E_981BD260F68F__INCLUDED_)
#define AFX_XXSoundMix_H__571FC837_2DF0_40FA_8F2E_981BD260F68F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XMutex.h"
#include "GGType.h"	// Added by ClassView

struct XSoundTransform
{
	int m_dLeftToLeft;
	int m_dLeftToRight;
	int m_dPan;
	int m_dRightToLeft;
	int m_dRightToRight;
	int m_dVolume;
};

#ifdef __APPLE__
#include <vector>
struct _XWAVEHDR {
	XPCTSTR    lpData;                 /* pointer to locked data buffer */
	XU32       dwBufferLength;         /* length of data buffer */
	XU32       dwBytesRecorded;        /* used for input only */
	XU32       dwUser;                 /* for client's use */
	XU32       dwFlags;                /* assorted flags (see defines) */
	XU32       dwLoops;                /* loop control counter */
	struct	   _XWAVEHDR  *lpNext;     /* reserved for driver */
	XU32       reserved;               /* reserved for driver */
};

#define WHDR_DONE       0x00000001  /* done bit */
#define WHDR_PREPARED   0x00000002  /* set if this header has been prepared */
#define WHDR_BEGINLOOP  0x00000004  /* loop start block */
#define WHDR_ENDLOOP    0x00000008  /* loop end block */
#define WHDR_INQUEUE    0x00000010  /* reserved for driver */
#endif

#ifdef __ANDROID__

struct _XWAVEHDR
{
	XPCTSTR    lpData;                 /* pointer to locked data buffer */
	XU32       dwBufferLength;         /* length of data buffer */
	XU32       dwBytesRecorded;        /* used for input only */
	XU32       dwUser;                 /* for client's use */
	XU32       dwFlags;                /* assorted flags (see defines) */
	XU32       dwLoops;                /* loop control counter */
	struct	   _XWAVEHDR  *lpNext;     /* reserved for driver */
	XU32       reserved;               /* reserved for driver */
};

#define WHDR_DONE       0x00000001  /* done bit */
#define WHDR_PREPARED   0x00000002  /* set if this header has been prepared */
#define WHDR_BEGINLOOP  0x00000004  /* loop start block */
#define WHDR_ENDLOOP    0x00000008  /* loop end block */
#define WHDR_INQUEUE    0x00000010  /* reserved for driver */
#endif

#ifdef _SYMBIAN
#include "XThreadPool.h"
#include "symbiandebug.h"

#define iMixer iStream
class MMdaAudioOutputStreamCallback;
class CMdaAudioOutputStream;
#include <MdaAudioOutputStream.h>
#include <mda\common\audio.h>
struct _XWAVEHDR {
	XPCTSTR    lpData;                 /* pointer to locked data buffer */
	XU32       dwBufferLength;         /* length of data buffer */
	XU32       dwBytesRecorded;        /* used for input only */
	XU32       dwUser;                 /* for client's use */
	XU32       dwFlags;                /* assorted flags (see defines) */
	XU32       dwLoops;                /* loop control counter */
	struct	   _XWAVEHDR  *lpNext;     /* reserved for driver */
	XU32       reserved;               /* reserved for driver */
};

#define WHDR_DONE       0x00000001  /* done bit */
#define WHDR_PREPARED   0x00000002  /* set if this header has been prepared */
#define WHDR_BEGINLOOP  0x00000004  /* loop start block */
#define WHDR_ENDLOOP    0x00000008  /* loop end block */
#define WHDR_INQUEUE    0x00000010  /* reserved for driver */

struct _XCRITICAL_SECTION {
	void* DebugInfo;
	XS32 LockCount;
	XS32 RecursionCount;
	XHANDLE OwningThread;        // from the thread's ClientId->UniqueThread
	XHANDLE LockSemaphore;
	XU32 SpinCount;
};
// add mediaclientaudiostream.lib into mmpfile
#elif defined(_WIN32)
struct _XWAVEHDR {
	XPCTSTR    lpData;                 /* pointer to locked data buffer */
	XU32       dwBufferLength;         /* length of data buffer */
	XU32       dwBytesRecorded;        /* used for input only */
	XU32       dwUser;                 /* for client's use */
	XU32       dwFlags;                /* assorted flags (see defines) */
	XU32       dwLoops;                /* loop control counter */
	struct	   _XWAVEHDR  *lpNext;     /* reserved for driver */
	XU32       reserved;               /* reserved for driver */
};

struct _XCRITICAL_SECTION {
	void* DebugInfo;
	XS32 LockCount;
	XS32 RecursionCount;
	XHANDLE OwningThread;        // from the thread's ClientId->UniqueThread
	XHANDLE LockSemaphore;
	XU32 SpinCount;
};
//#include <mmsystem.h>
#endif


struct _XWaveHeader
{
	void* data;
	XS32 bufferId;
	XU32 userFlags;
	XU32 userData;
};

enum {// Sound format types
	snd5K8Mono = 0,		snd5K8Stereo,
	snd5K16Mono,		snd5K16Stereo,
	snd11K8Mono,		snd11K8Stereo,
	snd11K16Mono,		snd11K16Stereo,
	snd22K8Mono,		snd22K8Stereo,
	snd22K16Mono,		snd22K16Stereo,
	snd44K8Mono,		snd44K8Stereo,
	snd44K16Mono,		snd44K16Stereo
};


const int MAX_SOUND_BUFFERS			= 32;
const int MAX_SOUND_CHANNELS		= 8;
const int SOUND_SAMPLES44			= 2048;	// ~50 msec
const int INITIAL_SOUND_BUFFERS		= 2;
// const int BEST_SOUND_FORMAT			= snd22K16Stereo;

// Our supported sample rates
// Note that sndRate5K is really 5512.5khz
// Use defines instead of an enum since these must be 32 bits on all platforms
#define sndRate5K	5512L
#define sndRate11K	11025L
#define sndRate22K	22050L
#define sndRate44K	44100L

const int sndMono			= 0x0;
const int sndStereo			= 0x1;

const int snd8Bit			= 0x0;
const int snd16Bit			= 0x2;

const int snd5K				= 0<<2;
const int snd11K			= 1<<2;
const int snd22K			= 2<<2;
const int snd44K			= 3<<2;

const int sndCompressNone	= 0x00; // we could add 14 more compression types here...
const int sndCompressADPCM	= 0x10;
const int sndCompressMP3	= 0x20;
const int sndCompressNoneI	= 0x30; // save out in intel byte order
const int sndRateMask		= 0x3 << 2;
const int sndCompressMask	= 0xF0;

#define WAVEHEADER_PREPARED		0x01
#define WAVEHEADER_DONE			0x02

extern const XS32 kRateTable[4];
extern const int kRateShiftTable[4];

#ifdef _SYMBIAN
extern const TInt kSBRateTable[4];
#endif

enum
{
	syncNoMultiple		= 0x1,	// don't start the sound if already playing
	syncStop			= 0x2	// stop the sound
};
class XSound
{
public:
	XS32	format;
	XS32	nSamples; 	// the number of samples - duration = nSamples/Rate()
	void*	samples;  	// this should probably be a handle on Mac
	XS32	dataLen;   	// length in bytes of samples - set only if needed
	XS32    delay;      // MP3 compression has a delay before the real sound data

public:
	void Init();

#ifdef __ANDROID__
//	XSound();
//	~XSound();
	XS32 Rate();
	int RateShift();
	XBOOL Stereo();
	int NChannels();
	XBOOL Is8Bit();
	int BitsPerSample();
	int BytesPerSample();
	int CompressFormat();
	XBOOL Compressed();

	// Manage the duration in 44kHz units
	XS32 GetDuration44();
	void SetDuration44(XS32 d);

	int BytesPerBlock();
	XS32 SizeBytes();
#else
	XS32 Rate() { return kRateTable[(format >> 2)&0x3]; }
	int RateShift() { return kRateShiftTable[(format >> 2)&0x3]; }
	XBOOL Stereo() { return (format & sndStereo) != 0; }
	int NChannels() { return (format & sndStereo) ? 2 : 1; }
	XBOOL Is8Bit() { return (format & snd16Bit) == 0; }
	int BitsPerSample() { return (format & snd16Bit) ? 16 : 8; }
	int BytesPerSample() { return (format & snd16Bit) ? 2 : 1; }
	int CompressFormat() { return format & sndCompressMask; }
	XBOOL Compressed() { return (format & sndCompressMask) != 0; }

	// Manage the duration in 44kHz units
	XS32 GetDuration44() { return nSamples<<RateShift(); }
	void SetDuration44(XS32 d) { nSamples = d>>RateShift(); }

	int BytesPerBlock() { return BytesPerSample()*NChannels(); }
	XS32 SizeBytes() { return nSamples*BytesPerBlock(); }
#endif

#ifdef _SYMBIAN
	TInt SBRate() { return kSBRateTable[(format >> 2)&0x3]; }
	TInt NSBChannels() { return (format & sndStereo) ? TMdaAudioDataSettings::EChannelsStereo : TMdaAudioDataSettings::EChannelsMono; }
#endif
};

class XSoundMix;

enum
{
	sndMinLevel = 0,
	sndMaxLevel = 0x8000,
	sndEnvelopeMaxPoints = 8	// 980910 mnk deleted comma after last member to avoid warning
};

class XSoundChannel;

typedef XBOOL (*StreamBlockProc)(XSoundChannel*);
typedef void  (*_FCALLBACK)(void*,bool bRemove);

class XSoundDecomp
{
public:
	virtual void Setup(XSound* snd, XBOOL reset)=0;
	virtual void Decompress(XS16* dst, XS32 n)=0;	   // must decompress into the format specified by snd
	virtual ~XSoundDecomp() {};
};

struct _XSoundEnvelopePoint
{
	XS32 mark44;
	XU16 level0;	// left channel - level is a 1.15 unsigned fixed point number (0..0x8000)
	XU16 level1;	// right channel
};

struct _XSoundEnvelope
{
	int nPoints;
	_XSoundEnvelopePoint points[sndEnvelopeMaxPoints];
};

class XSoundChannel
{
public:

	// These are set is the channel is currently playing
	XSoundMix* mixer;
	XSoundChannel* next;
	int refCount,nVolume;
	XBOOL remove;		// set if this channel is finished playing and needs to be removed
	XU32 tag;

	XSound* sound;		// the samples that define this sound

	// The user can set these values for event sounds
	_XSoundEnvelope envelope;
	int syncFlags;		// borrowed from stags.h can contain, soundNoMultiple or soundStop
	int loops;			// loop this sound
	XS32	inPoint44;		// the first sample to play
	XS32	outPoint44;		// the last sample to play

	// This is for streaming sounds
	StreamBlockProc streamProc;
	_FCALLBACK		completeProc;
	bool completeStop,streamEnd;
	void* pArgs;
	void SetCallBack(_FCALLBACK call,void*p)
	{
		completeProc=call;
		pArgs=p;
	}
	void* refPtr;

	// Support for checking the amount of the stream that has been played
	XS32 bufferCounts[MAX_SOUND_BUFFERS];
	XS32 samplesPlayed;				// the sound samples that have been played from this stream
	XS32 bufferCompletionTime;		// the time the most recent mixer buffer completed playing for this stream

	XS32 blockSamples;	// samples left in the current block
	XS32 seekSamples;    // used for mp3 streaming sound
	XU8 blockCanBeZero;  // used for mp3 streaming sound
	XU8 fadeIn;
	XU8 silentBlock;		// set if the stream is playing a silent block of data
	XU8 streamStalled;	// set if the stream is waiting for data from the network...

public:
	// These are setup by AddSound
	XS32 envelopePos44;	// the current position in the envelope
	XS32 pos;			// the current position in the channel
	XS32 outPos;			// the sample to stop at

	XBOOL prevSampleInited;	// set if prevSample has been inited...
	XS32 prevSample;		    // save the previous sample from the unconverted sound so we can do correct interpolation when upsampling need 4 bytes to store a single 16 bit stereo sample

	XSoundDecomp *decomp;   // The current decompression state

public:
	void Init();
	XSoundChannel();
	~XSoundChannel();
	XSoundTransform* m_pTransform;
	void Stop();
	XBOOL CreateDecompressor();
	XBOOL IsPlaying() { return mixer != 0 && !remove; }

	XS32 ElapsedFrames();	// return the number of frames that have been played in this stream

#ifdef __ANDROID__
	void AddRef();
	void Release();
#else
	void AddRef() { refCount++; }
	void Release() { if ( --refCount == 0 ) delete this; }
#endif
};


#ifdef __APPLE__
class OpaqueAudioQueue;
class AudioQueueBuffer;
#endif

#ifdef _SYMBIAN
class XSoundMix : public CBase ,public XBase, public MMdaAudioOutputStreamCallback
#else
class XSoundMix : public XBase
#endif
{
public:

	void SetPause(XBOOL b);
#ifdef _SYMBIAN
    void CloseDevice();
    XSoundMix()
    	{
    	playThread = XNULL;
    	firstPlay = false;
    	bPause = false;
    	scratchBytes = 0;
    	nextScratch = 0;
    	bufferSamples = 0;
    	bufferBytes = 0;
    	bufferDuration = 0;
    	period=0;
    	nBuffers=0;
    	nSilent=0;
    	firstChannel = NULL;
    	iVolumeAdjust = EVolumeNone;
    	iPlayStatus = EEngineNotReady;
    	iWait = NULL;
    	iStream = NULL;
    	isOpen = false;
    	m_pOutput = NULL;
    	m_nMaxVolume = 0;
    	nVolume=0xafff;
    	m_bVolumeChange = false;
    	m_nSignal = 0;
    	playEvent.ResetEvent();
    	createEvent.ResetEvent();
    	createdEvent.ResetEvent();
    	closeEvent.ResetEvent();
    	closedEvent.ResetEvent();
    	}
    ~XSoundMix()
    	{
    	CloseDevice();
    	}
#else
     void CloseDevice()
	 {
		 CloseNativeDevice();
	 }
#endif
protected:
	/*! CSoundMix does not use a contructor. For compatibility with CSoundMix,
	*	NativeConstruct is called for object "construction."
	*/
	void NativeConstruct();

	/*! CSoundMix does not use a destructor. For compatibility with CSoundMix,
	*	NativeDestruct is called for object "destruction."
	*/
	void NativeDestruct();

	/*!	The sound data needs to be protected is multi-threaded systems. Only
	*	one thread can have the critical semaphor. This call requests,
	*	and blocks, access to the critical sound semaphor.
	*/
	//void EnterCritical();
	/*! */
	//void LeaveCritical();

	/*!	BuildAndWrite is passed the WaveHeader to an available buffer. It should call
	*	BuildBuffer to repack the buffer and then send the buffer to the hardware
	*	device. If silence is true, BuildBuffer should not be called, and BuildAndWrite
	*	should write silence to the buffer and send it to the hardware.<br>
	*	BuildAndWrite is called from core code when the device is opened, and called
	*	by the Native code when a buffer completes playing.
	*/
	void BuildAndWrite( _XWaveHeader* hdr, XBOOL silence );

	/*!	GetNativeSoundCap gets the best native sound format. Called inside a critical
	*  section, when the hardware device is closed. This format is OR'd
	*	from the following variables:<br>
	*	sndMono, sndStereo<br>
	*	snd8Bit, snd16Bit<br>
	*	snd5K, snd11K, snd22K, snd44K
	*/
	void GetNativeSoundCap( XS32* format );

	/*!	Opens the hardware device in the given format. This call
	*	should not change CSound* format.
	*/
	XBOOL OpenNativeDevice( XSound* format );

	/*! BuffersReady is called when the device is open, the buffers are
	*	allocated, and the buffers are filled with data. A signal to
	*	start playing sound.
	*/
	void BuffersReady();

	/*!	When the buffers are allocated or enlarged, BuffersUpdated is called
	*	to inform the native code.
	*/
	void BuffersUpdated();

	/*! Closes the hardware device.
	*/
	void CloseNativeDevice();

	/*! Queries the state of the hardware device. Should be a fast call - it
	*	may be prudent to buffer the device state rather than actually asking
	*	the device.
	*/
	XBOOL IsDeviceOpen()			{ return isOpen; }

private:
	//XHANDLE hWaveOut;
	// Windows support
#ifdef _SYMBIAN
	enum TEngineStatus
	{
		EEngineNotReady,
		EEngineReady,
		EEngineToPause,
		EEnginePause,
		EEnginePlaying
	};
	enum TVolumeAdjust
		{
		  EVolumeNone,
		  EVolumeUp,
		  EVolumeDown
		};
	TVolumeAdjust iVolumeAdjust;
	TEngineStatus iPlayStatus;
	CActiveSchedulerWait*		iWait;
	CMdaAudioOutputStream*		iStream;
	void Stop();
public:
	void SetEvent(){playEvent.SetEvent();}
protected:
	void MaoscOpenComplete(TInt aError);
	void MaoscBufferCopied(TInt aError, const TDesC8& aBuffer);
	void MaoscPlayComplete(TInt aError);
	XUINT CallBack(XU32 nID);
	_XWAVEHDR		waveHDR[MAX_SOUND_BUFFERS];
	TPtrC8 iPtrC8;
	_XWAVEHDR* m_pOutput;
	XThread				*playThread;
	XEvent				playEvent;
#elif defined(_WIN32)
	XUINT CallBack(XU32 nID);
	void Stop(){playThread.Stop(XFALSE);}
	XHANDLE			hWaveOut;
	_XWAVEHDR		waveHDR[MAX_SOUND_BUFFERS];

	XThread				playThread;
	XEvent				playEvent;
public:
	void SetEvent();
protected:
#endif//_SYMBIAN

#ifdef __ANDROID__  // android add by hgl
private:
	XHANDLE			hWaveOut;
	_XWAVEHDR		waveHDR[MAX_SOUND_BUFFERS];

	XThread			playThread;
	XEvent			playEvent;
	void*			m_pAudioTrack;  // AudioTrack 类型
	bool			mbSilence;

public:
	
	XUINT CallBack(XU32 nID);
	void SetSilence( bool bsilence );
	void Stop();
	void SetEvent();
	void MakeDataAT( void *bufdata ); //标示已完成填充数据线
	void MakeDataSL();

	void ReSetAudio();

	typedef void (*AudioCallBackAT)(int event, void* user, void *info);
	typedef void (*AudioCallBackSL)(void* bq, void *soundMix);

	void *mpdlopen;
	typedef void (*fnAudioCreate)(int rate);
	fnAudioCreate mFnAudioCreate;

	typedef void (*fnAudioCreateAT)(AudioCallBackAT funCallback, void *soundMix, int rate );
	fnAudioCreateAT mFnAudioCreateAT;

	typedef void (*fnAudioCreateSL)(AudioCallBackSL funCallback, void *soundMix, int rate, int nChannel, int bitsPerSample );
	fnAudioCreateSL mFnAudioCreateSL;

	typedef void (*fnAudioStart)();
	fnAudioStart mFnAudioStart;

	typedef void (*fnAudioStop)();
	fnAudioStop mFnAudioStop;

	typedef void (*fnAudioDestroy)();
	fnAudioDestroy mFnAudioDestroy;

	typedef int (*fnAudioWrite)(void*,int);
	fnAudioWrite mFnAudioWrite;

	typedef void (*fnAudioSetVolume)(float,float);
	fnAudioSetVolume mFnAudioSetVolume;

	typedef void (*fnAudioGetVolume)(float*,float*);
	fnAudioGetVolume mFnAudioGetVolume;

#endif//__ANDROID__

#ifdef __APPLE__
	XUINT CallBack(XU32 nID);
	XThread				playThread;
	XEvent				playEvent;
	void* m_queue;
    bool			mbSilence;
	static void AQOutputCallback(void* userData, OpaqueAudioQueue* inAQ, AudioQueueBuffer* inBuffer);
	_XWAVEHDR		waveHDR[MAX_SOUND_BUFFERS];
	class QueueBufferData
	{
	public:
		void* m_queueBuffer;
		int m_index;
	};
	bool hasActiveQueueBuffer();
	std::vector<QueueBufferData> m_totalAllocedQueueBuffer;
	QueueBufferData* getFreeQueueBuffer();
	void freeAllQueueBuffer();
    void SetEvent();
public:
    void SetSilence( bool bsilence );
#endif//__APPLE__
public:
	XMutex			m_playLock;
	XBOOL				isOpen;
public:
	XBOOL m_bMute;
	void Mute(XBOOL bMute){m_bMute=bMute;}

	XSoundTransform*m_pTransformn;
#ifdef _SYMBIAN
	int GetMaxVolume();
#endif//_SYMBIAN
	int GetVolume();
	XBOOL SetVolume(int v);
	void StopAll();
	void Destruct();
	void Construct();

	void RemoveAll();
	XBOOL SoundPlaying(XSound*s);
	void OpenDevice();
	void FreeSound(XSound*s);
	void RemoveSound(XSoundChannel*c);
	void AddSound(XSoundChannel*c);
	void MarkBufferComplete(int i);

protected:

	void DoPlay();
	void FreeBuffers();
	void AllocBuffers();
	void ReleaseScratch(){nextScratch = 0;}
	void* GetScratch();

	void PurgeList();

	void BuildBuffer(int iBuf);
	void EnlargeBuffers();

#ifdef _SYMBIAN
protected:
	void _Open();
	void _Close();
	//bool isCreate;
	XEvent createEvent;
	XEvent createdEvent;
	XEvent closeEvent;
	XEvent closedEvent;
	XU32	m_nMaxVolume;
	int		m_nSignal;
	bool m_bVolumeChange;
#endif//_SYMBIAN

protected:
	XBOOL NeedStop();
	XU8 bPause;
	XU32 nVolume;
	XBOOL firstPlay;
	XS32 scratchBytes;
	XS32 nextScratch;
	XU8* scratchBuf[MAX_SOUND_CHANNELS];	// buffers for holding sound before mixing
	XSound mixFormat;
	XS32 bufferSamples;	// number of samples in a buffer
	XS32 bufferBytes;
	XS32 bufferDuration;	// the length of the sound buffer - set from bufferSamples44
	XU8* buffer[MAX_SOUND_BUFFERS];

	int period,nBuffers,nSilent;
	_XWaveHeader waveHdr[MAX_SOUND_BUFFERS];
	XSoundChannel* firstChannel;
};

#endif // !defined(AFX_XXSoundMix_H__571FC837_2DF0_40FA_8F2E_981BD260F68F__INCLUDED_)
