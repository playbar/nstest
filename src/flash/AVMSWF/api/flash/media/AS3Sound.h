#ifndef _AS3Sound_
#define _AS3Sound_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class SoundClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SoundObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
#pragma pack(1)
	/*typedef struct MP3FrameHeader{
		unsigned int sync:11;
		unsigned int version:2;
		unsigned int layer:2;
		unsigned int error_protection:1;
		unsigned int bitrate_index:4;
		unsigned int sampling_frequency:2;
		unsigned int padding:1;
		unsigned int reserved:1;
		unsigned int mode:2;
		unsigned int mode_extension:2;
		unsigned int copyright:1;
		unsigned int original:1;
		unsigned int emphasis:2;
	}MP3HEADER,*LPMP3HEADER;*/
	typedef struct VBRHeader
	{
		XU32 XingID[4];//"Xing";
		char channelInfo[36];
		XU32 flag;
		XU32 frames;
		XU32 fileLength;
		char dirTable[100];
		XU32 vbr;
	}VBRHEADER,*LPVBRHEADER;
	typedef struct ID3Header
	{
		char tag[3];//'TAG';
		char songName[30];
		char authorName[30];
		char className[30];
		XU32 nTime;
		char echoName[30];
		XU8  nClass;
	}ID3HEADER,*LPID3HEADER;
	typedef struct ID3HeaderV2
	{
		char tag[3];
		char ver;
		char rever;
		char flag;
//#if (__CORE_VERSION__>=0x02073000)
		XU8  size[4];
//#else
//		char size[4];
//#endif
	}ID3HEADERV2,*LPID3HEADERV2;
	typedef struct ID3Frame
	{
		char frameID[4];
//#if (__CORE_VERSION__>=0x02073000)
		XU8  size[4];
//#else
//		char size[4];
//#endif
		char flag[2];
	}ID3FRAME,*LPID3FRAME;
#pragma pack()
	class ID3InfoObject;
	class URLRequestObject;
	class SoundLoaderContextObject;
	class SoundTransformObject;
	class SoundChannelObject;
	class ByteArrayObject;
	class SoundClass : public ClassClosure
	{
	public:
		SoundClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoundClassSlots m_slots_SoundClass;
};
class SoundObject : public EventDispatcherObject
{
	public:
		SoundObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual bool  StreamInNew(void*streamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loadInfo);
		virtual int StreamInWrite(void* buffer,int	  length,void*gcEnter,void*pClient);
		virtual bool StreamInDestroy(XBOOL bOK,const char*msg,void*pClient);	
		virtual void Release();
	public:
		enum {XFMT_UNKOWN=0,
			  XFMT_MP3=1,
			  XFMT_WAVE=2};
		class LoaderData:public MMgc::GCObject 
		{
		public:
			XStream		  stream;
			int			  nBytesTotal;
			DRCWB(Stringp)  strURL;
			DRCWB(SoundChannelObject*) pObject;
			bool		  bCancel;
			XSound		  snd;					// the format of all streaming sound in the file			
			int			  nFormat;
			XU32		  nSoundHeader;
			int			  nVersion,nLayer,nFrameLength,nBlockSamples;
			bool		  bInitOK,bPlaying,bVBR;
			XU32		  nPos,nPlayPos,nStartPos,nRate,nPlayedSamples,nLoadedSamples;
			void Init()
			{
				bInitOK=false;
				bPlaying=false;
				nVersion=0;
				nLayer=0;
				nPos=0;
				nPlayPos=0;
				//nSamples=0;
				nBlockSamples=0;
				nStartPos=0;
				nSoundHeader=0;
				nRate=0;
				nFormat=XFMT_UNKOWN;
				snd.Init();
				stream.Reset();
				nBytesTotal=0;
				strURL=NULL;
				bCancel=false;
				nFrameLength=0;
				nLoadedSamples=0;
				nPlayedSamples=0;
			}
		};
		void*m_pClient;
		bool m_bNeedPlay;
		DWB(LoaderData*) m_pLoaderInfo;
		DRCWB(ID3InfoObject*) m_id3;
		struct _XSCharacter* m_pCharacter;
		uint32_t AS3_bytesLoaded_get();
		uint32_t AS3_bytesTotal_get();
		ID3InfoObject* AS3_id3_get();
		bool AS3_isBuffering_get();
		double AS3_length_get();
		Stringp AS3_url_get();
		void AS3_constructor(URLRequestObject*req,SoundLoaderContextObject*);
		void AS3_close();
		void AS3_load(URLRequestObject*req,SoundLoaderContextObject*);
		SoundChannelObject* AS3_play(double,int,SoundTransformObject*trans);

	public:
		bool StreamProc(SoundChannelObject*snd);		
	protected:
		XMutex m_dataLock;
		bool doInMP3(bool bEnd=false);
	public:
		//new adds
		bool AS3_isURLInaccessible_get();
		double AS3_extract(ByteArrayObject *pTarget, double length, double startPosition /*-1*/);
		//new adds end

		//new adds 11
		void AS3_loadCompressedDataFromByteArray(ByteArrayObject *pBytes, uint32_t bytesLength);
		void AS3_loadPCMFromByteArray(ByteArrayObject *pBytes, uint32_t samples, Stringp format, bool bStereo = true, double sampleRate = 44100.0);
		//new adds 11 end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoundObjectSlots m_slots_SoundObject;
};}
#endif