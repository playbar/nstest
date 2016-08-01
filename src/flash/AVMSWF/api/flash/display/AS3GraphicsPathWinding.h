#ifndef _AS3GraphicsPathWinding_
#define _AS3GraphicsPathWinding_
namespace avmplus{namespace NativeID{
class GraphicsPathWindingClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//EVEN_ODD : String = "evenOdd"
//NON_ZERO : String = "nonZero"

	Stringp EVEN_ODD;
	Stringp NON_ZERO;

private:};
class GraphicsPathWindingObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class GraphicsPathWindingClass : public ClassClosure
	{
	public:
		GraphicsPathWindingClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsPathWindingClassSlots m_slots_GraphicsPathWindingClass;
};
class GraphicsPathWindingObject : public ScriptObject
{
	public:
		GraphicsPathWindingObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsPathWindingObjectSlots m_slots_GraphicsPathWindingObject;
};}
#endif