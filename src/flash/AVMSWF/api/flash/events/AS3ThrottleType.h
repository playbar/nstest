#ifndef _AS3ThrottleType_
#define _AS3ThrottleType_

namespace avmplus
{
	namespace NativeID
	{
		class ThrottleTypeClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

			//PAUSE:String = "pause"
			//RESUME:String = "resume"
			//THROTTLE:String = "throttle"

			Stringp PAUSE;
			Stringp RESUME;
			Stringp THROTTLE;
		private:
		};
		class ThrottleTypeObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class ThrottleTypeClass : public ClassClosure//EventClass
	{
	public:
		ThrottleTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		inline Stringp getSlotPAUSE(){return m_slots_ThrottleTypeClass.PAUSE;}
		inline Stringp getSlotRESUME(){return m_slots_ThrottleTypeClass.RESUME;}
		inline Stringp getSlotTHROTTLE(){return m_slots_ThrottleTypeClass.THROTTLE;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ThrottleTypeClassSlots m_slots_ThrottleTypeClass;
	};
	class ThrottleTypeObject : public ScriptObject
	{
	public:
		ThrottleTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	void* pData; //Set your Data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ThrottleTypeObjectSlots m_slots_ThrottleTypeObject;
	};
}
#endif