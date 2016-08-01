#ifndef _AS3SoftKeyboardType_
#define _AS3SoftKeyboardType_

namespace avmplus
{
	namespace NativeID
	{
		class SoftKeyboardTypeClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			// CONTACT:String = "contact"
			// DEFAULT:String = "default"
			// EMAIL:String = "email"
			// NUMBER:String = "number"
			// PUNCTUATION:String = "punctuation"
			// URL:String = "url"
			Stringp CONTACT;
			Stringp DEFAULT;
			Stringp EMAIL;
			Stringp NUMBER;
			Stringp PUNCTUATION;
			Stringp URL;

		private:
		};
		class SoftKeyboardTypeObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class SoftKeyboardTypeClass : public ClassClosure//EventClass
	{
	public:
		SoftKeyboardTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:

		inline Stringp getSlotCONTACT(){return m_slots_SoftKeyboardTypeClass.CONTACT;}
		inline Stringp getSlotDEFAULT(){return m_slots_SoftKeyboardTypeClass.DEFAULT;}
		inline Stringp getSlotEMAIL(){return m_slots_SoftKeyboardTypeClass.EMAIL;}
		inline Stringp getSlotNUMBER(){return m_slots_SoftKeyboardTypeClass.NUMBER;}
		inline Stringp getSlotPUNCTUATION(){return m_slots_SoftKeyboardTypeClass.PUNCTUATION;}
		inline Stringp getSlotURL(){return m_slots_SoftKeyboardTypeClass.URL;}

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoftKeyboardTypeClassSlots m_slots_SoftKeyboardTypeClass;
	};
	class SoftKeyboardTypeObject : public ScriptObject
	{
	public:
		SoftKeyboardTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);




	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoftKeyboardTypeObjectSlots m_slots_SoftKeyboardTypeObject;
	};
}
#endif