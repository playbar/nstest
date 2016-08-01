#ifndef _AS3VideoCodec_
#define _AS3VideoCodec_

namespace avmplus
{
	namespace NativeID
	{
		class VideoCodecClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			// H264AVC:String = "H264Avc"
			// SORENSON:String = "Sorenson"

			Stringp H264AVC;
			Stringp SORENSON;


		private:
		};
		class VideoCodecObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class VideoCodecClass : public ClassClosure//EventClass
	{
	public:
		VideoCodecClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		inline Stringp getSlotH264AVC() {return m_slots_VideoCodecClass.H264AVC;}
		inline Stringp getSlotSORENSON() {return m_slots_VideoCodecClass.SORENSON;}

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::VideoCodecClassSlots m_slots_VideoCodecClass;
	};
	class VideoCodecObject : public ScriptObject
	{
	public:
		VideoCodecObject(VTable* _vtable, ScriptObject* _delegate, int capacity);




	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::VideoCodecObjectSlots m_slots_VideoCodecObject;
	};
}
#endif