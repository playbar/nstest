#ifndef _AS3RenderingMode_
#define _AS3RenderingMode_
namespace avmplus{namespace NativeID{
class RenderingModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//CFF : String = "cff"
//NORMAL : String = "normal"

	Stringp CFF;
	Stringp NORMAL;

private:};
class RenderingModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class RenderingModeClass : public ClassClosure
	{
	public:
		RenderingModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::RenderingModeClassSlots m_slots_RenderingModeClass;
};
class RenderingModeObject : public ScriptObject
{
	public:
		RenderingModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::RenderingModeObjectSlots m_slots_RenderingModeObject;
};}
#endif