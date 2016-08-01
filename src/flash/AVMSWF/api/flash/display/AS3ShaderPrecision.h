#ifndef _AS3ShaderPrecision_
#define _AS3ShaderPrecision_
namespace avmplus{namespace NativeID{
class ShaderPrecisionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//FAST : String = "fast"
//FULL : String = "full"

	Stringp FAST;
	Stringp FULL;

private:};
class ShaderPrecisionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ShaderPrecisionClass : public ClassClosure
	{
	public:
		ShaderPrecisionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp getFULL(){return m_slots_ShaderPrecisionClass.FULL;}
		Stringp getFAST(){return m_slots_ShaderPrecisionClass.FAST;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderPrecisionClassSlots m_slots_ShaderPrecisionClass;
};
class ShaderPrecisionObject : public ScriptObject
{
	public:
		ShaderPrecisionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderPrecisionObjectSlots m_slots_ShaderPrecisionObject;
};}
#endif