#ifndef _AS3StageTextInitOptions_
#define _AS3StageTextInitOptions_

namespace avmplus
{
	namespace NativeID
	{
		class StageTextInitOptionsClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

		private:
		};
		class StageTextInitOptionsObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class StageTextInitOptionsClass : public ClassClosure//EventClass
	{
	public:
		StageTextInitOptionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:



	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageTextInitOptionsClassSlots m_slots_StageTextInitOptionsClass;
	};
	class StageTextInitOptionsObject : public ScriptObject
	{
	public:
		StageTextInitOptionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

	public:
		bool m_bMultiline;
		bool AS3_multiline_get();
		void AS3_multiline_set(bool bMul);


	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StageTextInitOptionsObjectSlots m_slots_StageTextInitOptionsObject;
	};
}
#endif