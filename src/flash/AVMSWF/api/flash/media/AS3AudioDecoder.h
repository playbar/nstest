#ifndef _AS3AudioDecoder_
#define _AS3AudioDecoder_

namespace avmplus
{
	namespace NativeID
	{
		class AudioDecoderClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			// DOLBY_DIGITAL:String = "DolbyDigital"
			// DOLBY_DIGITAL_PLUS:String = "DolbyDigitalPlus"
			// DTS:String = "DTS"
			// DTS_EXPRESS:String = "DTSExpress"
			// DTS_HD_HIGH_RESOLUTION_AUDIO:String = "DTSHDHighResolutionAudio"
			// DTS_HD_MASTER_AUDIO:String = "DTSHDMasterAudio"

			Stringp DOLBY_DIGITAL;
			Stringp DOLBY_DIGITAL_PLUS;
			Stringp DTS;
			Stringp DTS_EXPRESS;
			Stringp DTS_HD_HIGH_RESOLUTION_AUDIO;
			Stringp DTS_HD_MASTER_AUDIO;

		private:
		};
		class AudioDecoderObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class AudioDecoderClass : public ClassClosure//EventClass
	{
	public:
		AudioDecoderClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		inline Stringp getSlotDOLBY_DIGITAL(){return m_slots_AudioDecoderClass.DOLBY_DIGITAL;}
		inline Stringp getSlotDOLBY_DIGITAL_PLUS(){return m_slots_AudioDecoderClass.DOLBY_DIGITAL_PLUS;}
		inline Stringp getSlotDTS(){return m_slots_AudioDecoderClass.DTS;}
		inline Stringp getSlotDTS_EXPRESS(){return m_slots_AudioDecoderClass.DTS_EXPRESS;}
		inline Stringp getSlotDTS_HD_HIGH_RESOLUTION_AUDIO(){return m_slots_AudioDecoderClass.DTS_HD_HIGH_RESOLUTION_AUDIO;}
		inline Stringp getSlotDTS_HD_MASTER_AUDIO(){return m_slots_AudioDecoderClass.DTS_HD_MASTER_AUDIO;}

	public:



	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AudioDecoderClassSlots m_slots_AudioDecoderClass;
	};
	class AudioDecoderObject : public ScriptObject
	{
	public:
		AudioDecoderObject(VTable* _vtable, ScriptObject* _delegate, int capacity);




	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AudioDecoderObjectSlots m_slots_AudioDecoderObject;
	};
}
#endif