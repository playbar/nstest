#ifndef _AS3AVTagData_
#define _AS3AVTagData_

namespace avmplus
{
	namespace NativeID
	{
		class AVTagDataClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!


		private:
		};
		class AVTagDataObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class AVTagDataClass : public ClassClosure//EventClass
	{
	public:
		AVTagDataClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:



	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AVTagDataClassSlots m_slots_AVTagDataClass;
	};
	class AVTagDataObject : public ScriptObject
	{
	public:
		AVTagDataObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

		Stringp AS3_data_get();
		double AS3_localTime_get();
		void AS3_constructor(Stringp data, double localTime);
		DRCWB(Stringp) m_strData;
		double         m_nLocalTime;

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AVTagDataObjectSlots m_slots_AVTagDataObject;
	};
}
#endif