#ifndef _AS3TabStop_
#define _AS3TabStop_
namespace avmplus{namespace NativeID{
class TabStopClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TabStopObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TabStopClass : public ClassClosure
	{
	public:
		TabStopClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TabStopClassSlots m_slots_TabStopClass;
};
class TabStopObject : public ScriptObject
{
	public:
		TabStopObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Stringp AS3_alignment_get();
		void AS3_alignment_set(Stringp alignment);
		Stringp AS3_decimalAlignmentToken_get();
		void AS3_decimalAlignmentToken_set(Stringp decimalAlignmentToken);
		double AS3_position_get();
		void AS3_position_set(double position);
//		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TabStopObjectSlots m_slots_TabStopObject;
};}
#endif