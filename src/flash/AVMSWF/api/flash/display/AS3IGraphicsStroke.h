#ifndef _AS3IGraphicsStroke_
#define _AS3IGraphicsStroke_
namespace avmplus{namespace NativeID{
class IGraphicsStrokeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IGraphicsStrokeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IGraphicsStrokeClass : public ClassClosure
	{
	public:
		IGraphicsStrokeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IGraphicsStrokeClassSlots m_slots_IGraphicsStrokeClass;
};
class IGraphicsStrokeObject : public ScriptObject
{
	public:
		IGraphicsStrokeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IGraphicsStrokeObjectSlots m_slots_IGraphicsStrokeObject;
};}
#endif