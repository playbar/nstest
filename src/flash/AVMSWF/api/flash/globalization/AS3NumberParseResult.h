#ifndef _AS3NumberParseResult_
#define _AS3NumberParseResult_
namespace avmplus{namespace NativeID{
class NumberParseResultClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NumberParseResultObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NumberParseResultClass : public ClassClosure
	{
	public:
		NumberParseResultClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NumberParseResultClassSlots m_slots_NumberParseResultClass;
};
class NumberParseResultObject : public ScriptObject
{
	public:
		NumberParseResultObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		int AS3_endIndex_get();
		int AS3_startIndex_get();
		double AS3_value_get();

		void AS3_constructor(double value, int startIndex, int endIndex);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NumberParseResultObjectSlots m_slots_NumberParseResultObject;
};}
#endif