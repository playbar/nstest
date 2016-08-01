#ifndef _AS3CurrencyParseResult_
#define _AS3CurrencyParseResult_
namespace avmplus{namespace NativeID{
class CurrencyParseResultClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class CurrencyParseResultObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CurrencyParseResultClass : public ClassClosure
	{
	public:
		CurrencyParseResultClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CurrencyParseResultClassSlots m_slots_CurrencyParseResultClass;
};
class CurrencyParseResultObject : public ScriptObject
{
	public:
		CurrencyParseResultObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Stringp AS3_currencyString_get();
		double AS3_value_get();
		void AS3_constructor(double value, Stringp symbol);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CurrencyParseResultObjectSlots m_slots_CurrencyParseResultObject;
};}
#endif