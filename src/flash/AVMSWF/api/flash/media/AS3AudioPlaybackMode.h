#ifndef _AS3AudioPlaybackMode_
#define _AS3AudioPlaybackMode_

namespace avmplus
{
	namespace NativeID
	{
		class AudioPlaybackModeClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			// AMBIENT:String = "ambient"
			// MEDIA:String = "media"
			//VOICE:String = "voice"

			Stringp AMBIENT;
			Stringp MEDIA;
			Stringp VOICE;


		private:
		};
		class AudioPlaybackModeObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class AudioPlaybackModeClass : public ClassClosure//EventClass
	{
	public:
		AudioPlaybackModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		inline Stringp getSlotAMBIENT(){return m_slots_AudioPlaybackModeClass.AMBIENT;}
		inline Stringp getSlotMEDIA(){return m_slots_AudioPlaybackModeClass.MEDIA;}
		inline Stringp getSlotVOICE(){return m_slots_AudioPlaybackModeClass.VOICE;}

	public:



	//private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AudioPlaybackModeClassSlots m_slots_AudioPlaybackModeClass;
	};
	class AudioPlaybackModeObject : public ScriptObject
	{
	public:
		AudioPlaybackModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);




	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AudioPlaybackModeObjectSlots m_slots_AudioPlaybackModeObject;
	};
}
#endif