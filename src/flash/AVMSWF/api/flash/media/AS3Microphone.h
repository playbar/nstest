#ifndef _AS3Microphone_
#define _AS3Microphone_

#include "AS3EventDispatcher.h"
#include "AS3MicrophoneEnhancedOptions.h"
#include "XMicrophone.h"
namespace avmplus{namespace NativeID{
class MicrophoneClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class MicrophoneObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MicrophoneObject;
	class SoundTransformObject;
	class NetStreamObject;
	class MicrophoneEnhancedOptionsObject;
	class BXWaveStreamObject;
	class MicrophoneClass : public ClassClosure
	{
	public:
		MicrophoneClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		ArrayObject* AS3_names_get();
		
		//new adds
		MicrophoneObject* AS3_getEnhancedMicrophone(int index);
		//new adds end

	public:
		MicrophoneObject* AS3_getMicrophone(int id);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MicrophoneClassSlots m_slots_MicrophoneClass;
};
class MicrophoneObject : public EventDispatcherObject
{
	public:
		MicrophoneObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
	public:
		void DataIn(void*pData,int l);
		void AttachNetStream(NetStreamObject*);
		void AttachWaveStream(BXWaveStreamObject*);
		void UnAttachNetStream(NetStreamObject*);
		void UnAttachWaveStream(BXWaveStreamObject*);
		bool m_bEchoSupperession;
		int  m_nNoiseSuppression;
		bool m_bFirst;
		bool m_bVAD;
		int  m_nQuality;
		int  m_nFramesPacket;
		XArray<NetStreamObject*> m_netAttachs;
		XArray<BXWaveStreamObject*> m_waveAttachs;
		DRCWB(SoundTransformObject*) m_pTransform;
		DRCWB(Stringp) m_pCodec;
		DRCWB(MicrophoneEnhancedOptionsObject*) m_pEnhancedOption;
		XMicrophone* m_pMicrophone;//phone;
		bool SetID(int id);
		
		double AS3_activityLevel_get();
		double AS3_gain_get();
		void AS3_gain_set(double);
		int AS3_index_get();
		bool AS3_muted_get();
		Stringp AS3_name_get();
		int AS3_rate_get();
		void AS3_rate_set(int);
		double AS3_silenceLevel_get();
		int AS3_silenceTimeout_get();
		SoundTransformObject* AS3_soundTransform_get(){return m_pTransform;}
		void AS3_soundTransform_set(SoundTransformObject*p);
		bool AS3_useEchoSuppression_get();
		void AS3_setLoopBack(bool);
		void AS3_setSilenceLevel(double,int);
		void AS3_setUseEchoSuppression(bool);

		//new adds
		Stringp AS3_codec_get();
		void AS3_codec_set(Stringp pCodec);
		bool AS3_enableVAD_get();
		void AS3_enableVAD_set(bool enableVAD);
		int AS3_encodeQuality_get();
		void AS3_encodeQuality_set(int encodeQuality);
		MicrophoneEnhancedOptionsObject* AS3_enhancedOptions_get();
		void AS3_enhancedOptions_set(MicrophoneEnhancedOptionsObject *pMicrophoneEnhancedOptions);
		int AS3_framesPerPacket_get();
		void AS3_framesPerPacket_set(int framesPerPacket);
//		bool AS3_isSupported_get();
		int AS3_noiseSuppressionLevel_get();
		void AS3_noiseSuppressionLevel_set(int noiseSuppressionLevel);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MicrophoneObjectSlots m_slots_MicrophoneObject;
};}
#endif