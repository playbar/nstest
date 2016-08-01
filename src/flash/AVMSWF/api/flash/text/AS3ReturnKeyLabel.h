#ifndef _AS3ReturnKeyLabel_
#define _AS3ReturnKeyLabel_

namespace avmplus
{
	namespace NativeID
	{
		class ReturnKeyLabelClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			// DEFAULT:String = "default"
			// DONE:String = "done"
			// GO:String = "go"
			// NEXT:String = "next"
			// SEARCH:String = "search"

			Stringp DEFAULT;
			Stringp DONE;
			Stringp GO;
			Stringp NEXT;
			Stringp SEARCH;


		private:
		};
		class ReturnKeyLabelObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class ReturnKeyLabelClass : public ClassClosure//EventClass
	{
	public:
		ReturnKeyLabelClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		inline Stringp getSlotDEFAULT(){return m_slots_ReturnKeyLabelClass.DEFAULT;}
		inline Stringp getSlotDONE(){return m_slots_ReturnKeyLabelClass.DONE;}
		inline Stringp getSlotGO(){return m_slots_ReturnKeyLabelClass.GO;}
		inline Stringp getSlotNEXT(){return m_slots_ReturnKeyLabelClass.NEXT;}
		inline Stringp getSlotSEARCH(){return m_slots_ReturnKeyLabelClass.SEARCH;}


	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ReturnKeyLabelClassSlots m_slots_ReturnKeyLabelClass;
	};
	class ReturnKeyLabelObject : public ScriptObject
	{
	public:
		ReturnKeyLabelObject(VTable* _vtable, ScriptObject* _delegate, int capacity);




	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ReturnKeyLabelObjectSlots m_slots_ReturnKeyLabelObject;
	};
}
#endif