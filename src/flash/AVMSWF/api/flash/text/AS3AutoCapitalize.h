#ifndef _AS3AutoCapitalize_
#define _AS3AutoCapitalize_

namespace avmplus
{
	namespace NativeID
	{
		class AutoCapitalizeClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			// ALL:String = "all"
			// NONE:String = "none"
			// SENTENCE:String = "sentence"
			// WORD:String = "word"

			Stringp ALL;
			Stringp NONE;
			Stringp SENTENCE;
			Stringp WORD;


		private:
		};
		class AutoCapitalizeObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class AutoCapitalizeClass : public ClassClosure//EventClass
	{
	public:
		AutoCapitalizeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		inline Stringp getSlotALL(){return m_slots_AutoCapitalizeClass.ALL;}
		inline Stringp getSlotNONE(){return m_slots_AutoCapitalizeClass.NONE;}
		inline Stringp getSlotSENTENCE(){return m_slots_AutoCapitalizeClass.SENTENCE;}
		inline Stringp getSlotWORD(){return m_slots_AutoCapitalizeClass.WORD;}


	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AutoCapitalizeClassSlots m_slots_AutoCapitalizeClass;
	};
	class AutoCapitalizeObject : public ScriptObject
	{
	public:
		AutoCapitalizeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);




	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AutoCapitalizeObjectSlots m_slots_AutoCapitalizeObject;
	};
}
#endif