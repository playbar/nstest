#ifndef _AS3TouchEventIntent_
#define _AS3TouchEventIntent_

namespace avmplus
{
	namespace NativeID
	{
		class TouchEventIntentClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

			//ERASER:String = "eraser"
			//PEN:String = "pen"
			//UNKNOWN:String = "unknown"

			Stringp ERASER;
			Stringp PEN;
			Stringp UNKNOWN;
		private:
		};
		class TouchEventIntentObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class TouchEventIntentClass : public ClassClosure//EventClass
	{
	public:
		TouchEventIntentClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		inline Stringp getSlotERASER(){return m_slots_TouchEventIntentClass.ERASER;}
		inline Stringp getSlotPEN(){return m_slots_TouchEventIntentClass.PEN;}
		inline Stringp getSlotUNKNOWN(){return m_slots_TouchEventIntentClass.UNKNOWN;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TouchEventIntentClassSlots m_slots_TouchEventIntentClass;
	};
	class TouchEventIntentObject : public ScriptObject
	{
	public:
		TouchEventIntentObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData; // Set your Data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TouchEventIntentObjectSlots m_slots_TouchEventIntentObject;
	};
}
#endif