#ifndef _AS3TriangleCulling_
#define _AS3TriangleCulling_
namespace avmplus{namespace NativeID{
class TriangleCullingClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//NEGATIVE : String = "negative"
//NONE : String = "none"
//POSITIVE : String = "positive"

	Stringp NEGATIVE;
	Stringp NONE;
	Stringp POSITIVE;

private:};
class TriangleCullingObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TriangleCullingClass : public ClassClosure
	{
	public:
		TriangleCullingClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TriangleCullingClassSlots m_slots_TriangleCullingClass;
};
class TriangleCullingObject : public ScriptObject
{
	public:
		TriangleCullingObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TriangleCullingObjectSlots m_slots_TriangleCullingObject;
};}
#endif