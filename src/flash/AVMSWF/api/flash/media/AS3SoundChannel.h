#ifndef _AS3SoundChannel_
#define _AS3SoundChannel_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class SoundChannelClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SoundChannelObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SoundTransformObject;
	class SoundChannelObject;
	class SoundObject;
	class SoundChannelClass : public ClassClosure
	{
	public:
		SoundChannelClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		SoundChannelObject* CreateSoundChannel(struct _XSCharacter*,SoundTransformObject*,int,int);
		SoundChannelObject* CreateSoundChannel(SoundObject*,SoundTransformObject*,int,int,bool bInitOK);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoundChannelClassSlots m_slots_SoundChannelClass;
};
class SoundChannelObject : public EventDispatcherObject
{
	public:
		bool m_bAdded;
		bool InitOK();
		void PlayEnd(bool bRemove);
		SoundChannelObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		//~SoundChannelObject();
	public:
		DRCWB(SoundObject*) m_pSoundObj;
		//void OnComplete();
		bool DoSoundFrame();
		virtual void Release();
		DRCWB(SoundTransformObject*) m_pTransform;
		XSoundChannel* m_pChannel;
		double AS3_leftPeak_get();
		double AS3_position_get();
		double AS3_rightPeak_get();
		SoundTransformObject* AS3_soundTransform_get();
		void AS3_soundTransform_set(SoundTransformObject*pObj);
		void AS3_stop();
		void Stop();
	public:
		//bool m_bEvent;
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoundChannelObjectSlots m_slots_SoundChannelObject;
};}
#endif