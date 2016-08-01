#ifndef _AS3GraphicsPathCommand_
#define _AS3GraphicsPathCommand_
namespace avmplus{namespace NativeID{
class GraphicsPathCommandClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//CURVE_TO : int = 3
//LINE_TO : int = 2
//MOVE_TO : int = 1
//NO_OP : int = 0
//WIDE_LINE_TO : int = 5
//WIDE_MOVE_TO : int = 4

	int CURVE_TO;
	int LINE_TO;
	int MOVE_TO;
	int NO_OP;
	int WIDE_LINE_TO;
	int WIDE_MOVE_TO;

private:};
class GraphicsPathCommandObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class GraphicsPathCommandClass : public ClassClosure
	{
	public:
		GraphicsPathCommandClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsPathCommandClassSlots m_slots_GraphicsPathCommandClass;
};
class GraphicsPathCommandObject : public ScriptObject
{
	public:
		GraphicsPathCommandObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsPathCommandObjectSlots m_slots_GraphicsPathCommandObject;
};}
#endif