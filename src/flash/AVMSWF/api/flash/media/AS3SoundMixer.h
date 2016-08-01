#ifndef _AS3SoundMixer_
#define _AS3SoundMixer_
#include "AS3AudioPlaybackMode.h"
namespace avmplus{namespace NativeID{
class SoundMixerClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SoundMixerObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SoundTransformObject;
	class ByteArrayObject;
	class SoundMixerClass : public ClassClosure
	{
	public:
		SoundMixerClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		DRCWB(SoundTransformObject*) m_pTransform;
		int AS3_bufferTime_get();
		void AS3_bufferTime_set(int);
		bool AS3_areSoundsInaccessible(){return false;}
		void AS3_computeSpectrum(ByteArrayObject*,bool,int);
		void AS3_stopAll();
		SoundTransformObject* AS3_soundTransform_get();
		void AS3_soundTransform_set(SoundTransformObject*);

		//new adds 11
 		Stringp m_strAudioPlaybackMode;
	    Stringp AS3_audioPlaybackMode_get();
		void AS3_audioPlaybackMode_set(Stringp backMode);

		bool m_bUseSpeakerphoneForVoice;
		bool AS3_useSpeakerphoneForVoice_get();
		void AS3_useSpeakerphoneForVoice_set(bool b);
 		//new adds 11 end 
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoundMixerClassSlots m_slots_SoundMixerClass;
};
class SoundMixerObject : public ScriptObject
{
	public:
		SoundMixerObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:		
		
		
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoundMixerObjectSlots m_slots_SoundMixerObject;
};}
#endif